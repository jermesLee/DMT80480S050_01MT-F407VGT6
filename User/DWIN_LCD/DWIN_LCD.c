#include "DWIN_LCD.h"

/***************************************DWIN_LCD���ܵ����һ֡���ݼ���*****************************/ 
#ifndef DWIN_LCD_FrameSizeMax                                                                       // ���û�к궨��DWIN_LCD_FrameSizeMax   ����ʹ��Ĭ�ϵ�DWIN_LCD_FrameSizeMax 
#define DWIN_LCD_FrameSizeMax           (DWIN_LCD_NumSize * 2 + 10 )                                // ��10��DWIN_LCD ������������ռ� ��
#endif                                                                                         


/******************************��������DWIN_LCD���ڴ����*****************************************/ 
uint8_t DWIN_LCD_Data[DWIN_LCD_NumSize * 2];                                                       //ΪDWIN_LCD���������ٵĻ�����                                                     
uint8_t DWIN_LCD_SendFrameData[DWIN_LCD_FrameSizeMax];                                             //���͵���ʱ����λ�� ��һ֡����׼�����ٷ���                      
uint8_t DWIN_LCD_ReceiveFrameData[DWIN_LCD_FrameSizeMax];                                          //���յ���ʱ����λ�� ����ȡһ֡���ݱ��浽 DWIN_LCD_ReceiveFrameData ��Ȼ����н���                  
uint16_t  DWIN_LCD_SendFrameLength ;                                                               //���ջ��跢�͵��ֽڳ���
uint16_t  DWIN_LCD_ReceiveFrameLength ;                                                            //���ջ��跢�͵��ֽڳ���



/******************************��������DWIN_LCD���û�����*****************************************/ 
void DWIN_LCD_ReceiveHandleFunction(void);                                                         //mcu���յ�����������Ӧ���� �����ݽ������ݵ��������Ӧ������д����Ӧλ��
void DWIN_LCD_SendHandleFunction(DWIN_LCD_CommandType FunSel ,uint16_t StartAddr , uint16_t Num);  //�������ݸ������� �����������ݣ����ȡ������
ErrorStatus DWIN_LCD_AddrMapDataRead(uint16_t *Data , uint16_t Num , uint16_t StartAddr );         //��ȡ�ڴ�ά���������ݱ��浽�����У�����ʵ��Ӧ��       
ErrorStatus DWIN_LCD_AddrMapDataWrite(uint16_t *Data , uint16_t Num , uint16_t StartAddr);         //����Ӧ����д�� mcu ά�����ڴ��� �������ڴ�ά��������     


/*****************************��������DWIN_LCD�����������Ӻ���************************************/
static ErrorStatus DWIN_LCD_ReadReceiveFrame(void);                                                //�ӽ��ջ�������ȡһ֡���ݣ��ж�ԭ��֡ͷ����������
static void DWIN_LCD_ReceiveFunction_0X83(void);                                                   // mcu���͸�������0x83ָ��󣬵�������Ӧmcu�� ��mcu����֡���ݽ��н������浽��Ӧ�ڴ���
static void DWIN_LCD_SendFunction_0X82(uint16_t StartAddr , uint16_t Num);                         // �������ݸ������� �����µ�����������ʾ
static void DWIN_LCD_SendFunction_0X83(uint16_t StartAddr , uint16_t Num);                         // ��ȡ���������� ������ȡ�����ݱ��浽mcuΪ���������ٵĵ�ַ��
static void HalfWordArrayCopyByteArray( uint16_t *IN , uint8_t *OUT ,uint16_t IN_Num );            // ���������鰴һ�ȵķ�ʽת�����ֽ�����
static void ByteArrayCopyHalfWordArray( uint8_t *IN , uint16_t *OUT ,uint16_t OUT_Num );           // ���ֽ����鰴һ������Ϸ�ʽ��ϳɰ�������
static void ByteArrayCopyByteArray( uint8_t *IN , uint8_t *OUT ,uint8_t Num);                      // ���ֽ����鰴˳���Ƶ��ֽ�����





/**************************************************************************************************
  * @brief  ���յ�����������Ӧ����ܵĴ����������ݽ��յ�����������Ի�����
  * @param  ��
  * @retval ��
  * @notice ������ֻʹ��������ָ�� �� 0x82 �� 0x83 ������0x82 дָ��û����Ӧ����
**************************************************************************************************/
void DWIN_LCD_ReceiveHandleFunction(void)
{	
	uint8_t   crc16[2] ;                                                                             // ������յ���һ֡���ݵ�crcֵ ����ʱ������ ����������յ���crc���жԱ�
	
	/****һ��ִ��һ��DWIN_LCD_ReceiveHandleFunction������һֱ��ȡ���ջ��������� ��ֱ������Ϊ���˳�����*****/
  /**�� ���˳�while�ķ�ʽ ֻ��һ��: ���ջ�����Ϊ��****/	
	while(1)                                                                                         // һֱ��ȡ���ջ������������ ��ֱ��������Ϊ�� ���˳�����             
	{	
ReadReceiveFrame :		                                                                             // crcУ��ʧ��ʱ�������˴�����ȡ��һ֡����
		/**��ȡһ֡���ݱ��浽 DWIN_LCD_ReceiveFrameData***/
		if(DWIN_LCD_ReadReceiveFrame() != SUCCESS ) return ;                                           // ���յ�������һ֡���ݱ��浽 DWIN_LCD_FrameData �� �����յ����ֽ������浽  DWIN_LCD_FrameLength   ���Ѿ�������֡ͷ�ıȶ�                                                                         //
   
		/*********��ȡһ֡���ݵ�CRCУ��*******************/
		DWIN_LCD_CRC16_CheckCompute(&DWIN_LCD_ReceiveFrameData[3],DWIN_LCD_ReceiveFrameData[2] - 2 ,crc16);        // ������յ�����CRCֵ ��ע�� ��crc16У��ӹ����뿪ʼ���� ,��2����Ϊ�����ֽ�������CRCУ��
		if(DWIN_LCD_CRC16_Check(&DWIN_LCD_ReceiveFrameData[DWIN_LCD_ReceiveFrameData[2] + 1 ],crc16) != SUCCESS )  // �Ƚϼ����CRC16�ͽ��յ���crc16�Ƿ���� ��1�� 3 - 2 �� ��3��2��ͷ+1�ֽ� ��2��2CRCУ��
			goto ReadReceiveFrame;                                                                       // crcУ��ʧ�� ����ȡ��һ֡����                                   
				 
		/*********��֡������ȷ������´���֡����**********/	
		switch( DWIN_LCD_ReceiveFrameData[3] )                                                         // ��ͬ������ʵ�ֲ�ͬ�Ĺ��� ��������Ϊ���� �� ����ֻ����0x82 �� 0x83 �����룩��ֻ�й�����0x83����Ӧ�� ���ڴ��� ��
		{
			case 0X83 : DWIN_LCD_ReceiveFunction_0X83(); break;                                          // �����ݴ洢��ָ�� 0x83 ������ȡ�����ݱ��浽������
			default   : break;                                                                           // �����������ݲ�֧�֣���֡���ݲ�Ҫ ����ȡ��һ֡����
		}                                                                                                    	                                                                                     
	}
}



/**************************************************************************************************
  * @brief  ���͸���������ʾ�����ܺ���  �����µ�����������ʾ �� ��ȡ����������
  * @param  FunSel�� ������ѡ�� ��������ֻʹ�������������� ��0x82 �� 0x83  
  *         StartAddr����Ҫ�������׵�ַ �� 16λ ��
  *         Num      ����Ҫ������������ �� ��λ ��16λ ����Ӧһ����ַ������ ��
  * @retval ��
  * @notice DWIN_LCD_SendFunction_0X??����ֻ��׼�����˸÷��͵����� ��DWIN_LCD_WriteSend()�������������ݷ���
**************************************************************************************************/
void DWIN_LCD_SendHandleFunction(DWIN_LCD_CommandType FunSel ,uint16_t StartAddr , uint16_t Num)
{
	/******�����������Ƿ���ȷ���������˳�����******/
	if(( Num == 0 ) || ( Num > ( DWIN_LCD_FrameSizeMax >> 1 ) - 4 ) )return ;                        // ���������� ���˳����� ��һ֡������ 8���ֽڱ����� ��2֡ͷ + 1�ֽ��� + 1������ + 2��ʼ��ַ  +  2CRC

#if DWIN_LCD_StartAddr                                                                             //�������С��ַΪ��0X000����            
	if( ( StartAddr < DWIN_LCD_StartAddr ) || ( StartAddr + Num > DWIN_LCD_StartAddr + DWIN_LCD_NumSize ) )// �����ַ�Ƿ���ȷ
		return ;                                                                                       // �����κδ��� ���˳�����
#else                                                                                              //����ʼ��ַΪ0X0000ʱ  StartAddr < DWIN_LCD_StartAddr  �����ܴ���  ��û������������뽫�о���                                         
	if( StartAddr + Num > DWIN_LCD_StartAddr + DWIN_LCD_NumSize )                                    // �����ַ�Ƿ���ȷ
		return ;                                                                                       // �����κδ��� ���˳�����
#endif

	/**����������ȷ����ͬ������FunSelִ�в�ͬ�Ĳ���*/
	switch( FunSel )                                                                                 // ���ݹ����뷢����Ӧ������
	{                                                                  
		case 0X82 : DWIN_LCD_SendFunction_0X82(StartAddr , Num);break ;                                // 0X82 :DWIN_LCD_WriteDataMemory (д���ݴ洢��) ��2֡ͷ + 1�ֽ��� + 1������ + 2��ʼ��ַ +  2N����       +  2CRC
		case 0X83 : DWIN_LCD_SendFunction_0X83(StartAddr , Num); break ;                               // 0X83 :DWIN_LCD_ReadDataMemory  (�����ݴ洢��) ��2֡ͷ + 1�ֽ��� + 1������ + 2��ʼ��ַ +  1���������  +  2CRC
		default   : return ;                                                                           // ���������ݲ�֧�� ���˳����� 
	}
  
	/**����Ĳ�ͬ������ֻ��׼�������� ������Ǵ������ݷ���*/
	DWIN_LCD_WriteSend(DWIN_LCD_SendFrameData ,DWIN_LCD_SendFrameLength);                            // ��׼���õ����ݷ��ͳ�ȥ	
}




/**************************************************************************************************
  * @brief  ���͸���������ʾ���ݣ�д�����ݴ洢������ �� ������ ��0x82 ��д���ݴ洢��ָ��
  * @param  StartAddr�� ��Ҫд���ݵ���ʼ��ֵ
  *         Num ��      ��Ҫд�������� �� ��λ ������ ��
  * @retval ��
  * @notice ��ַ�������������Ƿ��ڵ�ַ��Χ�ڵ��ж���DWIN_LCD_SendHandleFunction()�����ܺ��������ж�
**************************************************************************************************/
static void DWIN_LCD_SendFunction_0X82(uint16_t StartAddr , uint16_t Num)
{
  uint16_t  addr_index ;                                                                           // ��ַ����

	/****���������ַ���ڴ�ʵ�ʵ�ַ��Ӧ****/
  addr_index = StartAddr - DWIN_LCD_StartAddr ;                                                    //��ַ��������
  addr_index = addr_index << 1 ;                                                                   //���ʷ�ʽ ��16λ
	Num = Num << 1 ;                                                                                 //���ʷ�ʽ ��16λ

	/********׼����Ҫ���͵�����************/
	DWIN_LCD_SendFrameLength  = Num  + 8 ;                                                           //2֡ͷ�����ֽ������������룫2��ʼ��ַ+2CRC = 8 �ֽ�
	DWIN_LCD_SendFrameData[0] = DWIN_LCD_FrameHeadH ;                                                //֡ͷ
	DWIN_LCD_SendFrameData[1] = DWIN_LCD_FrameHeadL ;                                                    
	DWIN_LCD_SendFrameData[2] = Num  + 5 ;                                                           //֡��Ҫ���ֽ����� ��֡ͷ ��������
	DWIN_LCD_SendFrameData[3] = 0X82 ;                                                               //������ , 0x82 ��д���ݴ洢��
	DWIN_LCD_SendFrameData[4] = StartAddr >> 8   ;                                                   //��ʼ��ַ���ֽ�
	DWIN_LCD_SendFrameData[5] = StartAddr & 0XFF ;                                                   //��ʼ��ַ���ֽ�
	ByteArrayCopyByteArray(&DWIN_LCD_Data[addr_index], &DWIN_LCD_SendFrameData[6] ,Num);             //�������ݵ������͵�������
	
	/*************crc16����***************/
	DWIN_LCD_CRC16_CheckCompute(&DWIN_LCD_SendFrameData[3],Num  + 3 ,&DWIN_LCD_SendFrameData[Num  + 6]);//CRC16����
	
}





/**************************************************************************************************
  * @brief  ��ȡ��������ʾ������ ����ȡ�����ݴ洢������ �� ������ ��0x83 �������ݴ洢��ָ��
  * @param  StartAddr�� ��Ҫ��ȡ���ݵ���ʼ��ֵ
  *         Num ��      ��Ҫ��ȡ�������� �� ��λ ������ ��
  * @retval ��
  * @notice ��ַ�������������Ƿ��ڵ�ַ��Χ�ڵ��ж���DWIN_LCD_SendHandleFunction(�������ܺ��������ж�
**************************************************************************************************/
static void DWIN_LCD_SendFunction_0X83(uint16_t StartAddr , uint16_t Num)
{
	
	/********׼����Ҫ���͵�����************/
	DWIN_LCD_SendFrameLength  = 9 ;                                                                  //2֡ͷ�����ֽ������������룫2��ʼ��ַ+1���������+2CRC = 9�ֽ�
	DWIN_LCD_SendFrameData[0] = DWIN_LCD_FrameHeadH ;                                                //֡ͷ
	DWIN_LCD_SendFrameData[1] = DWIN_LCD_FrameHeadL ;                                                    
	DWIN_LCD_SendFrameData[2] = 0X06 ;                                                               //֡��Ҫ���ֽ����� ��֡ͷ �� ������
	DWIN_LCD_SendFrameData[3] = 0X83 ;                                                               //������ , 0x83 ����ȡ���ݴ洢��
	DWIN_LCD_SendFrameData[4] = StartAddr >> 8   ;                                                   //��ʼ��ַ���ֽ�
	DWIN_LCD_SendFrameData[5] = StartAddr & 0XFF ;                                                   //��ʼ��ַ���ֽ�
	DWIN_LCD_SendFrameData[6] = Num ;                                                                //��Ҫ��ȡ�������� ����λ������
	
	/*************crc16����***************/
	DWIN_LCD_CRC16_CheckCompute(&DWIN_LCD_SendFrameData[3],4 ,&DWIN_LCD_SendFrameData[7]);           //CRC16����
	
}





/**************************************************************************************************
  * @brief  ά��DWIN_LCD��һ�������� �������յ��ĵ��������ݱ��浽��Ӧ���ڴ���
  * @param  ��
  * @retval �� 
  * @notice �����յ�����Ӧ�ĵ�ַ����ȷʱ���˳������������κδ���
**************************************************************************************************/
static void DWIN_LCD_ReceiveFunction_0X83(void)
{
  uint16_t  start_address ;                                                                        //��Ҫ�������׵�ַ                                                                                 // 	
  uint16_t  addr_index ;                                                                           //��ַ���� 
	uint16_t  num ;                                                                                  //���յ��������� �� ��λ ��16λ ��
	
	/******���������ַ���ڴ�ʵ�ʵ�ַӳ��*******/
	start_address  =  DWIN_LCD_ReceiveFrameData[4] << 8 | DWIN_LCD_ReceiveFrameData[5] ;             //��ȡ���ݵ���ʼ��ַ
	num            =  DWIN_LCD_ReceiveFrameData[6] ;                                                 //��Ҫ��ȡ�������� ����λ ������

#if DWIN_LCD_StartAddr                                                                             //�������С��ַΪ��0X000����            
	if( ( start_address < DWIN_LCD_StartAddr ) || ( start_address + num > DWIN_LCD_StartAddr + DWIN_LCD_NumSize ) ) //��ַ����Ƿ���ȷ
		return ;	                                                                                     //��ַ���� ���˳�����
#else                                                                                              //����ʼ��ַΪ0X0000ʱ  StartAddr < DWIN_LCD_StartAddr  �����ܴ���  ��û������������뽫�о���  
	if( start_address + num > DWIN_LCD_StartAddr + DWIN_LCD_NumSize  )                               //��ַ����Ƿ���ȷ
		return ;	                                                                                     //��ַ���� ���˳�����
#endif	

	addr_index =  start_address - DWIN_LCD_StartAddr  ;                                              //��ַ����
	addr_index =  addr_index << 1 ;                                                                  //����Ϊ16λ ��������Ҫ����1λ
	num        =   num       << 1 ;                                                                  //num �ĵ�λΪ ��16λ�� ���� ��
	
	/******�����յ�������д���ڴ�ʵ�ʵ�ַ*******/
	ByteArrayCopyByteArray( &DWIN_LCD_ReceiveFrameData[7] , &DWIN_LCD_Data[addr_index] , num );      //����������DWIN_LCD������
}




/**************************************************************************************************
  * @brief  ά��DWIN_LCD��һ������������DWIN_LCD ���ڴ�����ȡ���ݵ�ʵ��Ӧ�ó���
  * @param  Data�� ���ȡ���ݵı���λ��
  *         Num �� ��Ҫ��ȡ��������
  *         StartAddr ����ȡ���ݵ��׵�ַ
  * @retval ERROR :   ������������,�����κβ������˳����� ;  
  *         SUCCESS : ����������ȷ ����ȡ���ݳɹ�
  * @notice ��DWIN_LCDά����һ������������ֽڵķ�ʽ���з��� �����ȶ����ֽ����� ��
  *         �ٶ����ֽ����� �� ������һ��16λ������
**************************************************************************************************/
ErrorStatus DWIN_LCD_AddrMapDataRead(uint16_t *Data , uint16_t Num , uint16_t StartAddr )         
{
	uint16_t Index ;                                                                                 // ��ַ����
	
	/********������������ַ�Ƿ���ȷ***********/
#if DWIN_LCD_StartAddr                                                                             //�������С��ַΪ��0X000����            
	if( ( StartAddr < DWIN_LCD_StartAddr ) || ( StartAddr + Num > DWIN_LCD_StartAddr + DWIN_LCD_NumSize ) )  //�жϵ�ַ�����Ƿ���ȷ
		return ERROR;                                                                                  // ��ַ����ȷ ���˳����� 
#else                                                                                              //����ʼ��ַΪ0X0000ʱ  StartAddr < DWIN_LCD_StartAddr  �����ܴ���  ��û������������뽫�о���                                         
	if( StartAddr + Num > DWIN_LCD_StartAddr + DWIN_LCD_NumSize  )                                   //�жϵ�ַ�����Ƿ���ȷ
		return ERROR;                                                                                  // ��ַ����ȷ ���˳����� 
#endif
		
	/*�ڵ�ַ������ȷ�£���ȡ������������ʵ��Ӧ�ñ�����*/
	Index = StartAddr - DWIN_LCD_StartAddr ;                                                         // ��ַ��������
  ByteArrayCopyHalfWordArray( &DWIN_LCD_Data[Index << 1] , Data , Num );                           // �ӻ�������ȡ���ݵ�ʵ��Ӧ�ó���
	return SUCCESS ;                                                                                 // ���ݶ�ȡ�ɹ� ������ SUCCESS
}






/**************************************************************************************************
  * @brief  ά��DWIN_LCD��һ������������DWIN_LCD ���ڴ���д����
  * @param  Data�� ��д������
  *         Num �� ��д������
  *         StartAddr ����д������DWIN_LCD���׵�ַ
  * @retval ERROR :   ������������,�����κβ������˳����� ;  
  *         SUCCESS : ����������ȷ ��д���ݳɹ�
  * @notice ��DWIN_LCDά����һ������������ֽڵķ�ʽ���з��� �����ȶ����ֽ����� ��
  *         �ٶ����ֽ����� �� ������һ��16λ������
**************************************************************************************************/
ErrorStatus DWIN_LCD_AddrMapDataWrite(uint16_t *Data , uint16_t Num , uint16_t StartAddr)       
{
	uint16_t Index ;                                                                                 // ��ַ����

	/********������������ַ�Ƿ���ȷ***********/
#if DWIN_LCD_StartAddr                                                                             //�������С��ַΪ��0X000����            
	if( ( StartAddr < DWIN_LCD_StartAddr ) || ( StartAddr + Num > DWIN_LCD_StartAddr + DWIN_LCD_NumSize ) )  //�жϵ�ַ�����Ƿ���ȷ
		return ERROR;                                                                                  // ��ַ����ȷ ���˳����� 
#else                                                                                              //����ʼ��ַΪ0X0000ʱ  StartAddr < DWIN_LCD_StartAddr  �����ܴ���  ��û������������뽫�о���                                         
	if( StartAddr + Num > DWIN_LCD_StartAddr + DWIN_LCD_NumSize  )                                   //�жϵ�ַ�����Ƿ���ȷ
		return ERROR;                                                                                  // ��ַ����ȷ ���˳����� 
#endif
	
	/*�ڵ�ַ������ȷ�£�������д��DWIN����������*/
	Index = StartAddr - DWIN_LCD_StartAddr ;                                                         // ��ַ��i������
  HalfWordArrayCopyByteArray( Data , &DWIN_LCD_Data[Index << 1] , Num );                           // ������д����������
	return SUCCESS ;                                                                                 // ���ݲ����ɹ� ������ SUCCESS
}






/**************************************************************************************************
  * @brief  �Ӵ��ڽ��ջ�������ȡһ֡���ݣ��ж�ԭ�� ��֡ͷ���ֽ��� ��
  * @param  ��
  * @retval ERR0R :���ջ������� ��SUCCESS :��ȡ��һ֡����
  * @notice ������ʹ����״̬���Ŀ�� 
**************************************************************************************************/
typedef enum {FrameHeaderH = 1 , FrameHeaderL ,FrameNum ,FrameData   }FrameStatusType ;            // һ֡������ÿ���ֽڵ���˼ ��ö��
FrameStatusType ReceiveFrameStatus = FrameHeaderH ;                                                // Ĭ��Ϊ֡ͷ���ֽ�
static ErrorStatus DWIN_LCD_ReadReceiveFrame(void)
{
  /***�����˳�while�ķ�ʽ �� 1. ��ȡ��һ������  ��  2. ���ջ�����Ϊ��***/	
	while(1)                                                                                         //һֱѭ�� ��ֱ������һ֡���� �� ���ջ�����Ϊ��
	{
		switch( ReceiveFrameStatus)                                                                    //״̬֮����л��� ��ʾ���ڽ�Ҫ��ȡ�ֽ���֡���ʾ����˼��
		{
			
			/**************����֡ͷ���ֽ�״̬***********/	
			case FrameHeaderH :                                                                          //״̬1 �� ֡ͷ���ֽ�
                				if(DWIN_LCD_ReadReceive(&DWIN_LCD_ReceiveFrameData[0],1) != SUCCESS ) return ERROR ;   // ��ȡʧ�� ���������� ���˳�����
												if(DWIN_LCD_ReceiveFrameData[0] == DWIN_LCD_FrameHeadH )                   //�ж�����ֽ���֡ͷ���ֽ��� �� ���ǲ��������´ζ��Ļ���֡ͷ���ֽ�
                        { 
													ReceiveFrameStatus = FrameHeaderL ;                                      //��֡ͷ ���л���״̬2 ����ȡ֡ͷ���ֽ�
												} 
												break ;  
		 
			/**************����֡ͷ���ֽ�״̬***********/	
			case FrameHeaderL :                                                                          //״̬2 �� ֡ͷ���ֽ�
												if(DWIN_LCD_ReadReceive(&DWIN_LCD_ReceiveFrameData[1],1) != SUCCESS ) return ERROR ;	 // ��ȡʧ�� ���������� ���˳�����			
												if(DWIN_LCD_ReceiveFrameData[1] == DWIN_LCD_FrameHeadL )                   // �ж�����ֽ���֡ͷ���ֽ��� �� �����˻ص�״̬1 �����л���ױ̨3
													ReceiveFrameStatus = FrameNum ;                                          // ��֡ͷ���ֽ� ���л���״̬3 �� ��ȡ�����ֽ��� 
											  else
													ReceiveFrameStatus = FrameHeaderH ;	                                     //����֡ͷ ���л���״̬1 ����ȡ֡����֡ͷ���ֽ�
												break ;  				 
			
			/**************����֡�ֽ���״̬************/
			case FrameNum     :                                                                          //״̬3 ��֡�����ֽ��� �� ������ 2֡ͷ + 1�ֽ���������+ 2 CRCУ�飩
												if(DWIN_LCD_ReadReceive(&DWIN_LCD_ReceiveFrameData[2],1) != SUCCESS ) return ERROR ;	 //��ȡʧ�� ���������� ���˳�����							 
			                  ReceiveFrameStatus = FrameData ;                                           //��ȡ�ֽ��������������ɹ� ���л��� ״̬4 �� ֡���ݺ�CRCУ��
			                  break ;  
			
			/************����֡���ݺ�CRCУ��**********/
			case FrameData    :                                                                          //״̬4 �� ֡���ݺ�CRCУ��
												if(DWIN_LCD_ReadReceive(&DWIN_LCD_ReceiveFrameData[3],DWIN_LCD_ReceiveFrameData[2] ) != SUCCESS ) //��ȡ���ݺ�CRC
													return ERROR ;                                                           //	��ȡʧ�� ���������� ���˳�����
			                  else                                                                       //���ݶ�ȡ�ɹ��Ĵ�����
												{
													ReceiveFrameStatus = FrameHeaderH ;                                      //�л���״̬1 �� ֡ͷ���ֽ� ���´����ݶ�ȡΪ ֡ͷ���ֽ�
													                                                                         //���յ�����֡�ֽ���
													return SUCCESS ;                                                         //�˳����� ���������ݶ�ȡ�ɹ�
												}		                                                                       //���û��break ��if����else������������¶����˳����� ��break������ִ��											 
			                                                                                              
		   /****************�����ڵ�״̬*************/
			 default           : ReceiveFrameStatus = FrameHeaderH ;                                     //�����ڵ�״̬ ��ǿ����һ���ֽ�Ϊ֡ͷ���ֽ�
		}
	}

}










/*****************************************���������Ӻ���******************************************/

/**************************************************************************************************
  * @brief  16λ���鸴�Ƶ��ַ�����
  * @param  IN �� ��������ͷָ�� ��  OUT �� �������ͷָ�� ��   Num ����Ҫ���Ƶ�������
  * @retval ��
  * @notice �ȱ���16λ���ݵĸ��ֽڣ��ٱ�����ֽ� �� ����16Ϊ������С�˴洢ģʽ�£�С��ַ������ֽ����� �����ַ������ֽ����ݣ�
**************************************************************************************************/
static void HalfWordArrayCopyByteArray( uint16_t *IN , uint8_t *OUT ,uint16_t IN_Num )
{
	uint8_t *In ;                                                           
	uint16_t i ;                                                              
	In  = (uint8_t *)IN ;                                                                            // �������16λ����ָ��ת��Ϊ8λ����ָ����з���
	for( i = 0 ; i < 2 * IN_Num ; i = i + 2 )                                                        // ���������� ������Ϊ�������
	{
		/********16λ���ݸߵ��ֽڽ��渴��***********/
		OUT[i+1] = In[i] ;                                                                             // [1] <-- [0] ,  [3] <-- [2] , [5] <-- [4]  ......  
		OUT[i]   = In[i+1] ;                                                                           // [0] <-- [1] ,  [2] <-- [3] , [4] <-- [5] ......  
	}
}





/**************************************************************************************************
  * @brief  8λ���鸴�Ƶ�16λ����
  * @param  IN �� ��������ͷָ�� ��  OUT �� �������ͷָ�� ��   Num ����Ҫ���Ƶ�������
  * @retval ��
  * @notice �ȱ���16λ���ݵĸ��ֽڣ��ٱ�����ֽ� �� ����16Ϊ������С�˴洢ģʽ�£�С��ַ������ֽ����� �����ַ������ֽ����ݣ�
**************************************************************************************************/
static void ByteArrayCopyHalfWordArray( uint8_t *IN , uint16_t *OUT ,uint16_t OUT_Num )
{                                                        
	uint16_t i,j ;                                                           
	for( i = 0 , j = 0 ; i < 2 * OUT_Num ; i = i + 2 ,j++)                                           // ���������� ������8λ�������һ��16λ����
	{
		/***�ȶ����ֽ��ڶ����ֽ����һ��16λ������***/
		OUT[j] = IN[i] << 8 | IN[i+1];                                                                 // [0] <-- [0]<<8|[1] , [1] <-- [2]<<8|[3]  ......  
	}
}



/*************************************************************************
  * @brief  ���鸴��
  * @param  IN �� ��������ͷָ�� ��  OUT �� �������ͷָ�� ��   Num ����Ҫ���Ƶ�������
  * @retval ��
  * @notice ��
*************************************************************************/
static void ByteArrayCopyByteArray( uint8_t *IN , uint8_t *OUT ,uint8_t Num)
{
  while( Num-- )                                                                                   //�������ݼ���
	{
		/****************��˳����****************/
	  *OUT++ = *IN++ ;                                                                               //��������
	}
}






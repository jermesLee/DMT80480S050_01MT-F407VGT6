#include "USART3.h"

/****************************USART3  �ڴ����******************************/
uint8_t USART3_SendData[USART3_SendSizeMax];                               //�������ݷ��ͻ�����
uint8_t USART3_ReceiveData[USART3_ReceiveSizeMax];                         //�������ݽ��ջ�����
uint16_t USART3_SendFront ,USART3_SendRear ,USART3_SendNextStart;          //���ڷ��ͻ��������ݹ���
uint16_t USART3_ReceiveFront ,USART3_ReceiveRear ;                         //���ڽ��ջ��������ݹ���
uint8_t USART3_SendBusyFlag  ;                                             //����DMA��������æµ��־λ ��1�����ڷ������� ��0��DMA���������ݷ��� ����ʱ���Դ���������һ������


/****************************USART3  �ڲ�����******************************/ 
static void USART3_GPIO_Config(void);                                      //���ô���USART3���ݴ�������I/O��                 
static void USART3_Mode_Config(uint32_t Baudrate);                         //���ô���USART3����ģʽ 
static void USART3_NVIC_Config(void);                                      //�ж����ȼ�����
static void USART3_DMA_Config(void);                                       //USART3 DMA ����
static void USART3_DataSendTrigger(void);                                  //����DMA�Ƿ����ڷ��ͣ��� �ڴ����Ƿ������ݴ�����һ�����ݵķ���
static void USART3_MemoryInit(void);                                       //��ʼ�����ڻ��������ݹ���



/****************************USART3  �û�����******************************/ 
void USART3_Init(uint32_t Baudrate);                                       //USART3�������ó�ʼ�� ,Baudrate :������
ErrorStatus USART3_WriteDMASendMulData(uint8_t *Data , uint16_t Num);      //���ڷ�������     
ErrorStatus USART3_ReadDMAReceiveMulData(uint8_t *Data , uint16_t Num);    //���ڶ�ȡ����

void USART3_Debug(void);                                                  //����ʹ�� �����ڽ���ʲô���ݾ͸����ڻ���Ŀ���ݣ�DMA��ʽ��





/***************************Ӳ����ʼ������*******************************/


/*************************************************************************
  * @brief  USART3��������ȫ����
  * @param  Baudrate �� ������
  * @retval ��
  * @notice �����ô��ڹ���ģʽ������I/O ����Ϊ����I/O�������ô���ʱ��
  *         ������I/O�����ô���֮��ʱI/O���Ż����һ��Ĭ�ϵĵ�ƽ���͵�ƽ���������˴��ڶ��һ������
**************************************************************************/ 
void USART3_Init(uint32_t Baudrate)
{
	/***************ʱ������*********************/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1   ,ENABLE);                  //Enable DMA clock 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB  ,ENABLE);	                //���ô���GPIO����ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3 ,ENABLE);                  //���ô���USART3��ʱ��
	
	/***************�ڴ�����*********************/
	USART3_MemoryInit();                                                    //�ڴ滺������ʼ��
	
	/***************Ӳ������*********************/
  USART3_NVIC_Config();                                                   //����USART3�ж����ȼ� 
	USART3_Mode_Config(Baudrate);                                           //���ô���USART3����ģ ,����������
	USART3_DMA_Config();                                                    //���ô���USART3 DMA ģʽ
  USART3_GPIO_Config();                                                   //���ô���USART3���ݴ�������I/O��
}



/*************************************************************************                
  * @brief  ���ô���USART3���ݴ�������I/O��
  * @param  ��
  * @retval �� 
  * @notice TXD������� ��RXD�������� 
*************************************************************************/
static void USART3_GPIO_Config(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		
		//����3��Ӧ���Ÿ���ӳ��
		GPIO_PinAFConfig(GPIOB,GPIO_PinSource10 ,GPIO_AF_USART3);              //GPIOB11����ΪUSART3
		GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3);               //GPIOB10����ΪUSART3
	
	  /* Configure USART3 */
	 	GPIO_InitStructure.GPIO_Pin    = GPIO_Pin_10 | GPIO_Pin_11;	          //RXD :PB11 ;  //TXD :PB10
		GPIO_InitStructure.GPIO_Mode   = GPIO_Mode_AF;                        //���÷�ʽ
		GPIO_InitStructure.GPIO_OType  = GPIO_OType_PP;                       //������� 
		GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_2MHz;                     //������������Ϊ2MHz 
		GPIO_InitStructure.GPIO_PuPd   =  GPIO_PuPd_UP;                       //����
		GPIO_Init(GPIOB, &GPIO_InitStructure);                                //���ÿ⺯������ʼ��
}




/*************************************************************************
  * @brief  ���ô���USART3����ģʽ
  * @param  Baudrate �� ������ 
  * @retval �� 
  * @notice ��
*************************************************************************/
static void USART3_Mode_Config(uint32_t Baudrate)
{
	USART_InitTypeDef USART_InitStructure;
  
	/* USART3 mode config */	
	USART_InitStructure.USART_BaudRate   = Baudrate ;                       //���ڲ����ʣ�USART3_BAUDRATE
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;             //֡���ݳ��ȣ�8λ
	USART_InitStructure.USART_StopBits   = USART_StopBits_1;                //ֹͣλλ����1λ
	USART_InitStructure.USART_Parity     = USART_Parity_No ;                //��żУ�� ����У��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //Ӳ����
	USART_InitStructure.USART_Mode       = USART_Mode_Rx | USART_Mode_Tx;   //���գ�����ʹ��
	USART_Init(USART3, &USART_InitStructure);                               //����
	USART_ITConfig(USART3, USART_IT_IDLE , ENABLE );                        //�������߿����ж�ʹ��
	USART_ITConfig(USART3, USART_IT_TXE  , DISABLE);                        //���ͻ��������жϹر�
	USART_ITConfig(USART3, USART_IT_TC   , DISABLE);                        //��������жϹر�
	USART_Cmd(USART3, ENABLE);                                              //����USART3ʹ�� 
}




/*************************************************************************
  * @brief  ���ô���USART3�ж����ȼ�
  * @param  ��
  * @retval �� 
  * @notice ���ȼ� �� #include "USART3.h" ���� define �궨��
*************************************************************************/
static void USART3_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	
	/* USART3_RX :USART3 RX ILDE configuration */
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;	 	                    // ָ�� USART3 ���տ����ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USART3_IRQPreemptionPrio ;//��ռʽ���ȼ�����
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = USART3_IRQSubPrio;      //�����ȼ�����
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                         //�ж�ʹ��
	NVIC_Init(&NVIC_InitStructure);                                         //���üĴ���
	
	/* USART3_RX :DMA1 configuration */
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream1_IRQn;	 	              // ָ�� USART3 ����DMA����ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = DMA1_Stream1_IRQPreemptionPrio  ;//��ռʽ���ȼ�����
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = DMA1_Stream1_IRQSubPrio ;               //�����ȼ�����
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                         //�ж�ʹ��
	NVIC_Init(&NVIC_InitStructure);                                         //���üĴ���

	/* USART3_TX :DMA1  configuration */
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream4_IRQn;                 // ָ������USART3 dma��������ж�
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = DMA1_Stream4_IRQPreemptionPrio ;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = DMA1_Stream4_IRQSubPrio;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                         //�ж�ʹ��
  NVIC_Init(&NVIC_InitStructure);                                         //���üĴ���
	

} 



/*************************************************************************
  * @brief  ���ô���USART3��DMAģʽ 
  * @param  ��
  * @retval �� 
  * @notice ��ʹ�ܷ���DMA,��������Ҫ����ʱʹ��DMA
*************************************************************************/
static void USART3_DMA_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	  
  /* USART3_TX :DMA1 Stream4 channel7 configuration */
  DMA_InitStructure.DMA_Channel            = DMA_Channel_7 ;              //ͨ�� 
  DMA_InitStructure.DMA_DIR                = DMA_DIR_MemoryToPeripheral ; //���򣺴��ڴ浽����
  DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)USART3_SendData;	  //DMAԴ��ַ ���ڴ��ַ
  DMA_InitStructure.DMA_BufferSize         = USART3_SendSizeMax;          //DMA�����������
  DMA_InitStructure.DMA_FIFOMode           = DMA_FIFOMode_Disable ;       //ֱ�Ӵ��� ����ʹ��FIFO
  DMA_InitStructure.DMA_FIFOThreshold      = DMA_FIFOThreshold_1QuarterFull;  //FIFO ��բ��
  DMA_InitStructure.DMA_MemoryBurst        = DMA_MemoryBurst_Single ;     //����
  DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;     //�洢�����ݳ���:8λ
  DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;        //�ڴ��ַ��1
  DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;				  	  //���δ���
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART3->DR ;      //DMAĿ�ĵ�ַ   : ����1�����ݷ��͵�ַ
  DMA_InitStructure.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;  //����
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //�������ݳ���:8λ
  DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;   //�����ַ�̶�
  DMA_InitStructure.DMA_Priority           = DMA_Priority_High;           //DMA���ȼ�
  DMA_Init(DMA1_Stream4,&DMA_InitStructure);                              //����
	
	DMA_ITConfig(DMA1_Stream4,DMA_IT_TC,ENABLE);                            //DMA1 Stream4 ���ݴ�������ж�ʹ��
	USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE);                          //USART3 ����DMAʹ��                
	DMA_Cmd(DMA1_Stream4, DISABLE);                                         //��ʹ��DMA,��������Ҫ����ʱʹ��DMA



	/* USART3_RX :DMA1 Stream1 channel4 configuration */
  DMA_InitStructure.DMA_Channel         = DMA_Channel_4 ;                //ͨ�� 
  DMA_InitStructure.DMA_DIR             = DMA_DIR_PeripheralToMemory ;   //���򣺴����赽�ڴ�
  DMA_InitStructure.DMA_Mode            = DMA_Mode_Circular;					   //ѭ������
	DMA_InitStructure.DMA_Memory0BaseAddr =(uint32_t)USART3_ReceiveData;   //DMAĿ�ĵ�ַ ���ڴ��ַ
  DMA_Init(DMA1_Stream1,&DMA_InitStructure);                             //����
 
	DMA_ITConfig(DMA1_Stream1,DMA_IT_TC, ENABLE);                          //DMA1 Stream1 ���ݴ�������жϴ�
	USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);                         //ʹ�� USART3 ���ݽ���DMAʹ��  
	DMA_Cmd(DMA1_Stream1, ENABLE);                                         //ʹ�ܽ���DMA,�ȴ����ݵĵ���

}





/***************************�жϷ�����*********************************/


/*************************************************************************
  * @brief  USART3 �жϷ�����(����DMA���ݽ���ʱ�����ڿ��м��)
  * @param  ��
  * @retval ��
  * @notice �����õ������߿����ж�  �� һ���ֽ�ʱ��Ŀ���ʱ������ж� ��
  *         ������߿����жϱ�־λ  ֻ��ͨ���ȶ�USART_SR ,�ٶ�USART_DR��������������
*************************************************************************/
void USART3_IRQHandler(void) 
{
	/******�����ж� ����¼�������ݽ��յ���λ��*****/
	if(USART_GetITStatus(USART3,USART_IT_IDLE)!= RESET)                     //�ж��Ƿ�Ϊ���߿����ж� ����ʾ���յ�һ������
	{	
		USART_ReceiveData(USART3);                                            //������߿����жϱ�־λ ( ֻ��ͨ���ȶ�USART_SR ,�ٶ�USART_DR��������������) 
		USART3_ReceiveFront = USART3_ReceiveSizeMax - DMA_GetCurrDataCounter(DMA1_Stream1);  //�������ڽ��յ����ݵ�ַ
	}
}






/*************************************************************************
  * @brief  DMA1_Channel4�жϷ�����(����DMA���ݷ��� ������������ʱ�����ж�)
  * @param  ��
  * @retval ��
  * @notice USART3_SendRear ��������ݿ�ʼ����ʱ���´η������ݵĵ�ַ
            USART3_SendNextStart��������ݷ�����ɽ�USART3_SendRear����´����ݷ��͵�ַ������
*************************************************************************/
void DMA1_Stream4_IRQHandler(void)
{
	/******һ�����ݷ����� �������´����ݵķ���****/
	if(DMA_GetITStatus(DMA1_Stream4 , DMA_IT_TCIF4)!= RESET)                //�ж��Ƿ�Ϊ��������ж�
  { 
    DMA_ClearITPendingBit(DMA1_Stream4 , DMA_IT_TCIF4);                   //���DMA�жϱ�־λ 
		DMA_Cmd(DMA1_Stream4,DISABLE);                                        //һ֡���ݷ������ ���ر�DMA���͵����ݴ���
	  USART3_SendNextStart = USART3_SendRear ;                              //���ڴ��͵�һ֡���ݷ�����ɣ�����һ�����ݷ��͵ĵ�ַ �� USART3_SendNextStart
	  USART3_SendBusyFlag  = 0 ;                                            //USART3 DMA ���ڲ�����æµ״̬ �����Է�����һ֡����
	  USART3_DataSendTrigger() ;                                            //����Ƿ�����Ҫ���͵����ݣ������������ٴη��� 
	}
}




/*************************************************************************
  * @brief  DMA1_Channe1 �жϷ�����(����DMA���ݽ��� �����ظ�ģʽ�£�����ж�λ���ٴο�ʼ��������
  * @param  ��
  * @retval ��
  * @notice ��Ҫ��Ŀ��Ϊ ������жϱ�־λ ��ȷ�����յ�����һ���ֽڽ�������USART3_ReceiveData[0]��
*************************************************************************/
void DMA1_Stream1_IRQHandler(void)
{
		/******���жϱ�־���´����ݴ�ͷ����****/
	if(DMA_GetITStatus(DMA1_Stream1 , DMA_IT_TCIF1)!= RESET)                //�ж��Ƿ�Ϊ����DMA�Ƿ����
  { 
    DMA_ClearITPendingBit(DMA1_Stream1 , DMA_IT_TCIF1);                   //���DMA�жϱ�־λ �� ���ظ�ģʽ�� ����һ���ֽڽ��ٴδ�USART3_ReceiveData[0]��ʼ
	}
}









/***************************�Ի����ڴ�Ĳ���*******************************/

/*************************************************************************
  * @brief  USART3�����ڴ��ʼ��
  * @param  ��
  * @retval ��
  * @notice USART3_SendBusyFlag��ʼ����Ϊ 0 ����ʾ��ʼ�ܹ���������
*************************************************************************/
static void USART3_MemoryInit( void )
{
	USART3_SendBusyFlag = 0 ;                                               // һ��ʼ���ڿ��� �����Է�������
	USART3_SendFront    = 0 ;                                               // �������ݻ��������ͷ��ַ��ʼ��
	USART3_SendRear     = 0 ;                                               // �������ݻ��������β��ַ��ʼ��
	USART3_SendNextStart= 0 ;                                               // ��һ֡���ݷ��͵Ŀ�ʼ��ַ��ʼ��
	USART3_ReceiveFront = 0 ;                                               // �������ݻ��������ͷ��ַ��ʼ��
	USART3_ReceiveRear  = 0 ;                                               // �������ݻ��������β��ַ��ʼ��
}



/*************************************************************************
  * @brief  �������ݷ��ʹ��� ������DMA�Ƿ����ڷ��ͣ��� �ڴ����Ƿ�������������һ�����ݵķ���
  * @param  ��
  * @retval ��
  * @notice ���жϺ�����������ʹ��
*************************************************************************/
static void USART3_DataSendTrigger( void )
{
	uint16_t SendNum ;                                                      //��Ҫ���͵������� 
	uint16_t SendNextRear;                                                  //��һ�����ݴ���û�г���dmaһ������ܹ�������ֽ���ʱ����һ֡���ݵĿ�ʼλ��
	
	/******�ж��Ƿ���Ҫ����DMA�����ݷ��� **********/
	if(USART3_SendBusyFlag  != 0 ) return ;                                 //�����ϴ����ݻ�δ������� ��������æµ״̬ ��
	if( USART3_SendFront == USART3_SendRear ) return ;                      //����������Ϊ�� ��û��������Ҫ����
	
	/***����Ҫ���͵�����£�������Ҫ���͵�������***/
	if(USART3_SendFront > USART3_SendRear)                                  //��������ͷ������β��λ��ȷ�����ݷ��͵���
	{
		SendNum      =  USART3_SendFront -  USART3_SendRear ;                 //֡ͷ��֡β���һ���Է������ 	
		SendNextRear =  USART3_SendFront ;                                    //��һ�����ݵķ��ʹ�USART3_SendFront ��ʼ
	}
	else                                                                    //֡ͷ��֡βС�ķ��������ݷ��� ���ȷ���  USART3_SendRear ��  USART3_SendSizeMax ������
	{                                                                       //�ٷ��ʹ�0��ʼ��USART3_SendFront ������
		SendNum      = USART3_SendSizeMax -  USART3_SendRear ;                //�����һ����Ҫ���͵�������
	  SendNextRear = 0 ;                                                    //��һ�����ݵķ��ʹ� 0 ��ʼ
	}
	

	/**����һ��DMA�������������������ڴ��ͷ�**/
	if( SendNum > USART3_DMASendOneSizeMax )	                              //�Ƚ�������Ҫ������ֽ���������ֽ����Ĵ�С                           
	{
		SendNum = USART3_DMASendOneSizeMax ;                                  //��������һ��DMA���ݴ��� ��ʹ�����һ�����ݴ���
		USART3_SendRear = USART3_SendRear + USART3_DMASendOneSizeMax ;        //��һ�����ݵķ��ʹ�USART3_SendRear + USART3_DMASendOneSizeMax  ��ʼ
	}
	else
	{
		USART3_SendRear = SendNextRear ;                                      //��һ�����ݵķ��ʹ�SendNextRear��ʼ
	} 

	
	/******��λ����æµ��־λ ���������ݷ���******/
	USART3_SendBusyFlag  = 1 ;                                              //æµ״̬
	DMA_Cmd(DMA1_Stream4,DISABLE);                                          //DMA_CNDTR��ҪDMA_CCRx�е�EN=0ʱ����д�� 
  DMA1_Stream4->M0AR = (uint32_t)&USART3_SendData[USART3_SendNextStart];  //�������ݷ��͵���ʼ��ַ
  DMA1_Stream4->NDTR = SendNum;                                           //�������ݷ��͵������� ����λ ���ֽ�
	DMA_Cmd(DMA1_Stream4,ENABLE);                                           //ʹ��DMA���ݷ���

}




/*************************************************************************
  * @brief  �������ݷ��� �����ݴ����Ƿ�æµ�ٴ���DMA���ݵķ���
  * @param  Data  �� �跢���������׵�ַ
					  Num   �� �跢�͵�������
  * @retval ��
  * @notice if( Num >= IdleNum ) return ERROR ;���е��ںţ�����һ���ֽڲ��� ��
  *         �������������� �� ���ݿ�
*************************************************************************/
ErrorStatus USART3_WriteDMASendMulData(uint8_t *Data , uint16_t Num)
{
	uint16_t  i ,IdleNum ;                                                  //���������е��ڴ���                                               
	
	/******��������ڴ� ���ж��Ƿ񻹹��� ***********/
	if(USART3_SendFront >= USART3_SendNextStart )                           //����������㻹�ж����ڴ湻�� ��ע����ںš� = �� 
		IdleNum = USART3_SendSizeMax-USART3_SendFront + USART3_SendNextStart ;                   
	else
		IdleNum = USART3_SendNextStart - USART3_SendFront ;
	if( Num >= IdleNum ) return ERROR ;                                     //�������ڴ治�� ������д���������һ���ֽڲ��ã�ע����ںš�=��
	
	/*****���ڴ湻�õ����������д�뻺���ڴ���******/
	for( i = 0 ; i < Num ; i++ )                                            //д���ڴ����
	{
		USART3_SendData[USART3_SendFront] = *Data++ ;                         //������д�뻺���ڴ���
		if(USART3_SendFront == USART3_SendSizeMax - 1)                        //�Ƿ�д���ڴ���β��
			USART3_SendFront  = 0 ;                                             //д���ڴ������һ���ֽ� ����һ���ֽڴ�0���¿�ʼ
		else
			USART3_SendFront++ ;                                                //�������������һ���ֽڵı���λ��
	}
	
	/******************�������ݵķ���***************/
	USART3_DataSendTrigger();                                               //�������ݵķ��� 
	return SUCCESS;                                                         //���ݴ���ɹ� 
}






/*************************************************************************
  * @brief  ���ڶ�ȡһ֡���� 
  * @param  Data  ����ȡ���ݱ���λ�� 
  *         Num   ����ȡ����֡���ݵĳ���
  * @retval ��
  * @notice USART3_ReceiveFinishFlag ��ʾ һ֡���ݶ�ȡ��� ��û�д����������� 
*************************************************************************/
ErrorStatus USART3_ReadDMAReceiveMulData(uint8_t *Data , uint16_t Num)
{
	uint16_t  i , num ;                                                     //���ջ���������Ч�ֽ���
	
	/******�жϻ������Ƿ���Num������******/
	if( USART3_ReceiveFront >= USART3_ReceiveRear)                          //�������������յ����ֽ���
		num = USART3_ReceiveFront - USART3_ReceiveRear ;                      
	else                                                                    
		num = USART3_ReceiveSizeMax - USART3_ReceiveRear + USART3_ReceiveFront ; 
	if( num < Num ) return ERROR ;                                          //���ݲ��� �����ض�ȡʧ�� ��ERROR
	
	/******�����ݹ�������¶�ȡ����******/
	for( i = 0 ; i < Num ; i++ )                                            //�������ݼ���
	{
			*Data++ = USART3_ReceiveData[USART3_ReceiveRear];                   //�ӽ��ջ����ڴ��ȡ���ݱ��浽Data��
	    if(USART3_ReceiveRear == USART3_ReceiveSizeMax - 1 )                //�Ƿ��ȡ���ڴ���β��
				USART3_ReceiveRear = 0 ;                                          //��ȡ���ڴ������һ���ֽ� ����һ���ֽڴ�0���¿�ʼ
			else                          
				USART3_ReceiveRear++ ;                                            //��������¼����ȡ����һ���ֽ�λ��
	}
	return SUCCESS;                                                         //���ݶ�ȡ�ɹ�
}







///************************************************************************
//  * @brief  ����ʹ�� �����ڽ���ʲô���ݾ͸����ڻ�ʲô���ݣ�DMA��ʽ��
//  * @param  ��
//  * @retval ��
//************************************************************************/
void USART3_Debug(void)
{
	uint8_t USART3_Dat[16];
	uint8_t USART3_Num = 16;

	if(USART3_ReadDMAReceiveMulData(USART3_Dat , USART3_Num ) == SUCCESS)            //��ȡ���յ�һ֡����
		USART3_WriteDMASendMulData(USART3_Dat , USART3_Num ) ;                         //���ͽ��ܵ���һ֡����
}


/*********************************************END OF FILE**********************/
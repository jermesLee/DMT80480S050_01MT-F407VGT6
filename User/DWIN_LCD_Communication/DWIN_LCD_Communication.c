#include "DWIN_LCD_Communication.h"


/************************** DWIN_LCD���ܺ�������*************************/ 
void DWIN_LCD_DisplayInit(void);                                          //�ϵ��ʼ��DWIN_LCD��ʾ����
void DWIN_LCD_AndMCUCommunication(void);                                  //��Ҫ�ӻ�����д��	DWIN_LCD������ �� �� �����յ��ĵ��������ݴ���д��DWIN_LCD������ 
uint16_t DWIN_LCD_DisplaySetLineVoltage(void);                            //����DWIN_LCD����ʾ�����ߵ�ѹ ���ͽ����õ��ߵ�ѹд��DWIN_LCD������
uint16_t DWIN_LCD_DisplaySetFrequency(void);                              //����DWIN_LCD����ʾ�����ź�Ƶ�ʣ��ͽ����õ�Ƶ��д��DWIN_LCD������


/************************************************************************
  * @brief  �ϵ��ʼ��DWIN_LCD��ʾ����
  * @param  ��  
  * @retval �� 
	* @notice Dat�������Ϊ�ϵ��ʼ��ʱDWIN_LCD����ʾ������ 
*************************************************************************/
void DWIN_LCD_DisplayInit(void)
{
	uint16_t Dat[]={11,22,33,44,55,66,1000,0,0,10,11,12,13,14,15,16};       //��Ҫ��DWIN_LCD��ʾ������ʾ������ 
  DWIN_LCD_AddrMapDataWrite(Dat , 16 , 0X0000);                           //����Ҫ��ʾ������д��DWIN_LCD������
}





/************************************************************************
  * @brief  ��Ҫ�ӻ�����д��	DWIN_LCD������ �� �� �����յ��ĵ��������ݴ���д��DWIN_LCD������ 
  * @param  ��  
  * @retval �� 
	* @notice ����ʵ��Ӧ�ý����޸�
*************************************************************************/
void  DWIN_LCD_AndMCUCommunication(void) 
{		
	DWIN_LCD_ReceiveHandleFunction();                                       //�����ڽ��յ����������ݽ������浽���������ݻ�����
  DWIN_LCD_SendHandleFunction(DWIN_LCD_WriteDataMemory,0X0000,11);	      //�ѻ���������д�������
}



/************************************************************************
  * @brief  ����DWIN_LCD����ʾ�����ߵ�ѹ ���ͽ����õ��ߵ�ѹд��DWIN_LCD������
  * @param  ��  
  * @retval ��
	* @notice ������������ı�־�� 0X7FFF ������������޸� ��������ֱ�����ʵ��Ӧ�ò��������� 
*************************************************************************/
uint16_t  DWIN_LCD_DisplaySetLineVoltage(void) 
{ 
	static uint16_t DWIN_LCD_InputLineVoltage ;                             //ʵ�ʴ��������ߵ�ѹ 
	uint16_t InputLineVoltage;                                              //��ȡ���������ߵ�ѹ��ʱ����λ�� ����Ϊ0��ʾû�д����������� ��ʹ���ϴεĴ������� 
  int32_t  DisplaySetLineVoltageValue ;                                   //��ʾ�ߵ�ѹ�ļ���
  
	/************��ȡ���������ߵ�ѹ***************/
	DWIN_LCD_AddrMapDataRead(&InputLineVoltage , 1 ,DWIN_LCD_InputLineVoltageAddress  ) ;      //��ȡ����������������ߵ�ѹֵ 
	
	/*************�д����ߵ�ѹ*����***************/
	if(InputLineVoltage != 0X7FFF )                                         //�д����������� �� ������ݲ���O0X7FFF,����Ļʵ�������룩 
	{	
     TIM5_Encoder_SetData(0);                                             //���������ֵ 
		 DWIN_LCD_InputLineVoltage = InputLineVoltage ;                       //����������������������ߵ�ѹ����
		
		 InputLineVoltage= 0X7FFF;                                            //���ݶ�ȡ������ ( 0x7fff��һ��ʵ���ò��������� ��û��ʵ������ ������һ����־����)
		 DWIN_LCD_AddrMapDataWrite(&InputLineVoltage , 1 ,DWIN_LCD_InputLineVoltageAddress  ) ;  //����־���֣�0x7fff��д�뻺���� �������� DWIN_LCD_AndMCUCommunication() �﷢��������
	}
	
	/**��ʾ�ߵ�ѹ = �����������ߵ�ѹ + ����������**/
	DisplaySetLineVoltageValue = TIM5_Encoder_GetData() + DWIN_LCD_InputLineVoltage ;          //��ʾ�ߵ�ѹ = �����������ߵ�ѹ + ����������
	
	/**************������ֵ�����ߵ�ѹ**************/	
	if(DisplaySetLineVoltageValue < DWIN_LCD_DisplaySetLineVoltageMin  )    //�ߵ�ѹС��DWIN_LCD_DisplaySetFrequencyMin
	{
		DisplaySetLineVoltageValue  = DWIN_LCD_DisplaySetLineVoltageMin ;     //���õ��ߵ�ѹ��С ��ǿ��ת��Ϊ��С��ѹֵ �� DWIN_LCD_DisplaySetFrequencyMin                         
    TIM5_Encoder_SetData( DWIN_LCD_DisplaySetLineVoltageMin - DWIN_LCD_InputLineVoltage);  //���ñ�����ֵ ��ʹ�´�   (int16_t)TIM4_Encoder_GetData() + DWIN_LCD_InputFrequency ����ʾֵΪDWIN_LCD_DisplaySetFrequencyMin
  }
  else if(DisplaySetLineVoltageValue > DWIN_LCD_DisplaySetLineVoltageMax )//�ߵ�ѹ����DWIN_LCD_DisplaySetFrequencyMax
	{
		DisplaySetLineVoltageValue = DWIN_LCD_DisplaySetLineVoltageMax ;      //���õ��ߵ�ѹ���� ��ǿ��ת��Ϊ����ѹֵ �� DWIN_LCD_DisplaySetFrequencyMax                         
    TIM5_Encoder_SetData( DWIN_LCD_DisplaySetLineVoltageMax - DWIN_LCD_InputLineVoltage);  //���ñ�����ֵ ��ʹ�´�   (int16_t)TIM4_Encoder_GetData() + DWIN_LCD_InputFrequency ����ʾֵΪDWIN_LCD_DisplaySetFrequencyMax	
	}
	
	/************�������ߵ�ѹд�뻺����**********/	
	DWIN_LCD_AddrMapDataWrite( (uint16_t*)&DisplaySetLineVoltageValue , 1 ,DWIN_LCD_DisplaySetLineVoltageAddress ) ; //����ʾ�ߵ�ѹ�����ڻ����� 
  return   (uint16_t)DisplaySetLineVoltageValue ;                        //��������ʵ�����õ��ߵ�ѹֵ
}






/************************************************************************
  * @brief  ����DWIN_LCD����ʾ�����ź�Ƶ�ʣ��ͽ����õ�Ƶ��д��DWIN_LCD������
  * @param  ��  
  * @retval ��
	* @notice ������������ı�־�� 0X7FFF ������������޸� ��������ֱ�����ʵ��Ӧ�ò��������� 
*************************************************************************/
uint16_t  DWIN_LCD_DisplaySetFrequency(void) 
{ 
	static uint16_t DWIN_LCD_InputFrequency ;                               //ʵ�ʴ�������Ƶ��
	uint16_t InputFrequency;                                                //��ȡ��������Ƶ����ʱ����λ�� ����Ϊ0X7FFF��ʾû�д����������� ��ʹ���ϴεĴ������� 
  int32_t  DisplaySetFrequencyValue ;                                     //��ʾƵ�ʵļ���
  
	/************��ȡ����Ƶ������*****************/
	DWIN_LCD_AddrMapDataRead(&InputFrequency , 1 ,DWIN_LCD_InputFrequencyAddress  ) ;      //��ȡ����������������ߵ�ѹֵ 
	
	/*************�д���Ƶ������******************/
	if(InputFrequency != 0X7FFF )                                           //�д����������� �� ������ݲ���0X7FFF,����Ļʵ�������� ,����һ����־���֣� 
	{	
     TIM3_Encoder_SetData(0);                                             //���������ֵ 
		 DWIN_LCD_InputFrequency= InputFrequency ;                            //���������������������Ƶ������
		
		 InputFrequency= 0X7FFF ;                                             //���ݶ�ȡ������ ( 0x7fff��һ��ʵ���ò��������� ��û��ʵ������ ������һ����־����)
		 DWIN_LCD_AddrMapDataWrite(&InputFrequency , 1 ,DWIN_LCD_InputFrequencyAddress  ) ;  //����־���֣�0x7fff��д�뻺���� �������� DWIN_LCD_AndMCUCommunication() �﷢��������
	}
	
	/**��ʾ����Ƶ�� = ����������Ƶ�� + ����������**/
	DisplaySetFrequencyValue = TIM3_Encoder_GetData() + DWIN_LCD_InputFrequency ; //��ʾ����Ƶ��= ����������Ƶ��+ ����������
	
	/**************������ֵ�����ź�Ƶ��************/	
	if(DisplaySetFrequencyValue < DWIN_LCD_DisplaySetFrequencyMin  )        //�ߵ�ѹС��DWIN_LCD_DisplaySetFrequencyMin
	{
		DisplaySetFrequencyValue = DWIN_LCD_DisplaySetFrequencyMin ;          //���õ��ߵ�ѹ��С ��ǿ��ת��Ϊ��С��ѹֵ �� DWIN_LCD_DisplaySetFrequencyMin                         
    TIM3_Encoder_SetData( DWIN_LCD_DisplaySetFrequencyMin - DWIN_LCD_InputFrequency);  //���ñ�����ֵ ��ʹ�´�   (int16_t)TIM4_Encoder_GetData() + DWIN_LCD_InputFrequency ����ʾֵΪDWIN_LCD_DisplaySetFrequencyMin
  }
  else if(DisplaySetFrequencyValue > DWIN_LCD_DisplaySetFrequencyMax )    //�ߵ�ѹ����DWIN_LCD_DisplaySetFrequencyMax
	{
		DisplaySetFrequencyValue = DWIN_LCD_DisplaySetFrequencyMax ;          //���õ��ߵ�ѹ���� ��ǿ��ת��Ϊ����ѹֵ �� DWIN_LCD_DisplaySetFrequencyMax                         
    TIM3_Encoder_SetData( DWIN_LCD_DisplaySetFrequencyMax - DWIN_LCD_InputFrequency);  //���ñ�����ֵ ��ʹ�´�   (int16_t)TIM4_Encoder_GetData() + DWIN_LCD_InputFrequency ����ʾֵΪDWIN_LCD_DisplaySetFrequencyMax	
	}
	
	/************�������ź�Ƶ��д�뻺����**********/	
	DWIN_LCD_AddrMapDataWrite( (uint16_t*)&DisplaySetFrequencyValue , 1 ,DWIN_LCD_DisplaySetFrequencyAddress ) ; //����ʾƵ�ʱ����ڻ����� 
  return   (uint16_t)DisplaySetFrequencyValue ;                          //��������ʵ�����õ��ź�Ƶ��ֵ
}

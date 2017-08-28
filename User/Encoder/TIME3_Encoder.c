#include "TIME3_Encoder.h" 

#define TIM3_Encoder_InitialValue        (uint16_t)32000                  // �ڲ������������һ�����ϻ������ܹ��ǵ�����   TIM3_Encoder_InitialValue ���Ϊ 32767                
#define TIM3_Encoder_ARRValue            (uint16_t)(TIM3_Encoder_InitialValue * 2)     // �˴�����Ϊ * 2                 

/*************************TIM3 �������ڴ����**************************/
int16_t TIM3_Encoder_Overflown = 0 ;                                      //�ڴ��������


/*************************TIM3 �������ڲ�����**************************/
static void TIM3_GPIO_Config(void);                                       //����TIM3 ������ģʽ������ PC6��PC7
static void TIM3_Mode_Config(void);                                       //����TIM3������ ������ģʽ
static void TIM3_NVIC_Configuration(void);                                //����TIM3�жϼ������ȼ�


/*************************TIM3 �������û�����**************************/
void TIM3_Encoder_Init(void);                                             //TIM3����Ϊ������ģʽ                                      
void TIM3_Encoder_SetData(int32_t EncoderValue);                          //���ñ��������� ��-2^31-1  ~  2^31 - 1��
int32_t TIM3_Encoder_GetData(void);                                       //��ȡ���������� ��-2^31-1  ~  2^31 - 1��





/*************************************************************************
  * @brief  ����TIM3 ������ģʽ������ PC6��PC7
  * @param  None
  * @retval None
  * @notice 
*************************************************************************/
static void TIM3_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);                 //���Ÿ��ù���ӳ��
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM3);
	
	/* TIM3 channel 1 and 2 pin (PC6 and PC7 ) configuration */
	GPIO_InitStructure.GPIO_Pin    = GPIO_Pin_6 | GPIO_Pin_7 ;	            //ѡ��Ҫ���Ƶ�GPIOB����			
	GPIO_InitStructure.GPIO_Mode   = GPIO_Mode_AF;                          //���÷�ʽ
	GPIO_InitStructure.GPIO_OType  = GPIO_OType_PP;                         //������� 
	GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_100MHz;                     //������������Ϊ100MHz 
	GPIO_InitStructure.GPIO_PuPd   = GPIO_PuPd_UP ;                         //����
	GPIO_Init(GPIOC, &GPIO_InitStructure);                                  //���ÿ⺯������ʼ��GPIOC6,7

} 





/*************************************************************************
  * @brief  ����TIM3������ ������ģʽ
  * @param  ��
  * @retval ��
  * @notice 
*************************************************************************/
static void TIM3_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
  TIM_ICInitTypeDef TIM_ICInitStructure; 
	 
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);                         //Ĭ��ȱʡֵ
  TIM_TimeBaseStructure.TIM_Prescaler     = 0x0;                          //Ԥ��Ƶ�� 
  TIM_TimeBaseStructure.TIM_Period        = TIM3_Encoder_ARRValue ;       //�趨�������Զ���װֵ ARR
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;                 //ѡ��ʱ�ӷ�Ƶ������Ƶ
  TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_CenterAligned3;//TIM����/���¼�����������жϣ����� ���ж�ʹ�ܣ�  
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);                         //����
	 
  TIM_ICInitStructure.TIM_Channel         = TIM_Channel_1;                //IC1
  TIM_ICInitStructure.TIM_ICPolarity      = TIM_ICPolarity_Rising;        //������
  TIM_ICInitStructure.TIM_ICSelection     = TIM_ICSelection_DirectTI;     //TI1FP1 -> IC1 , TI2FP2 -> IC2
  TIM_ICInitStructure.TIM_ICPrescaler     = TIM_ICPSC_DIV1;               //�ⲿ�����źŲ���Ƶ
  TIM_ICInitStructure.TIM_ICFilter        = 15 ;                          //ѡ������Ƚ��˲���
  TIM_ICInit(TIM3, &TIM_ICInitStructure);                                 //���� 
	
	TIM_ICInitStructure.TIM_Channel         = TIM_Channel_2;                //IC2
  TIM_ICInit(TIM3, &TIM_ICInitStructure);                                 //���� 

  TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI2, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//ʹ�ñ�����ģʽ2 ,����IC2��ƽ�� IC1���ؽ��м���
	
  TIM_ARRPreloadConfig(TIM3,DISABLE);                                     //Ԥװ�ز�ʹ��
  TIM_ClearFlag(TIM3, TIM_FLAG_Update);                                   //���TIM�ĸ��±�־λ
  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);                              //ʹ�ܸ����ж�

  TIM_SetCounter(TIM3,TIM3_Encoder_InitialValue);                         //��ʼ����������ֵ
  TIM_Cmd(TIM3, ENABLE);                                                  //ʹ�ܼ���������

}







/*************************************************************************
  * @brief  ����TIM3�жϼ������ȼ�
  * @param  ��
  * @retval ��
  * @notice 
*************************************************************************/
static void TIM3_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    
    NVIC_InitStructure.NVIC_IRQChannel                   = TIM3_IRQn;	    // ��򿪵��ж�Դ  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = TIM3_IRQPreemptionPrio; //��ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = TIM3_IRQSubPrio ;       //�μ����ȼ�	
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;        //TIM2_IRQn�ж�ʹ��
    NVIC_Init(&NVIC_InitStructure);                                       //����
}






/*************************************************************************
  * @brief  TIM3��ʼ���ܺ��� ��������ģʽ  ��
  * @param  ��
  * @retval ��
  * @notice 
*************************************************************************/
void TIM3_Encoder_Init(void)                                        
{
	/**************ʱ������*********************/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);                    //ʹ�ܶ�ʱ��3��ʱ��
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);                   //ʹ��Pc�˿�ʱ��

	/**************Ӳ������*********************/
	TIM3_NVIC_Configuration();                                              // ����TIM3�жϼ������ȼ�
	TIM3_Mode_Config();	                                                    // ��ʱ��TIM3����ģʽ����   
	TIM3_GPIO_Config();                                                     // I/0������������
}




/*************************************************************************
  * @brief  ��ʼ��TIM3 ������ֵ
  * @param  ��
  * @retval ��
  * @notice ������ǰ����رն�ʱ��TIM5
*************************************************************************/
void TIM3_Encoder_SetData(int32_t EncoderValue)                           //���ñ��������� ��-2^31-1  ~  2^31 - 1��
{	
	 int16_t Encoder_InitialValue ;

	 TIM_Cmd(TIM3, DISABLE);                                                //�ض�ʱ��  
	
   if( ( EncoderValue < TIM3_Encoder_InitialValue)&&( EncoderValue > -TIM3_Encoder_InitialValue) )	  //EncoderValueû����� ����С������
	 {	
     /*********û�����*********/		 
		 TIM3_Encoder_Overflown = 0 ;                                         //�����Ϊ0
	   Encoder_InitialValue   = EncoderValue + TIM3_Encoder_InitialValue ;  //����д����ʱ��ARR��ֵ
	 }
	 else                                                                   //����������ȷ���� ����û�������������뿪 ����С����                                                   
	 {
     /***********�����*********/		 
		 TIM3_Encoder_Overflown = EncoderValue / TIM3_Encoder_InitialValue ;  //������������Ĵ��� �� -13 /5 = -2 ....-3��
		 Encoder_InitialValue   = EncoderValue % TIM3_Encoder_InitialValue ;  //��Ƭ���︺���ĳ�����ȡ�����㣺�� -13 /5 = -2 ....-3�� 
		 Encoder_InitialValue   = Encoder_InitialValue + TIM3_Encoder_InitialValue ;  //����д����ʱ��ARR��ֵ
	 }
	//�ڴ��������	
	 TIM_SetCounter(TIM3,Encoder_InitialValue);                             //��ʼ����������ֵ ����Encoder_InitialValueд�� ARR�Ĵ���
	 TIM_Cmd(TIM3, ENABLE);                                                 //ʹ�ܶ�ʱ��
}







/*************************************************************************
  * @brief  ��ȡ���������� �� -2^31 -1 ~ 2^31 -1��
  * @param  ��
  * @retval ���������� �� -2^31 -1 ~ 2^31 -1��
  * @notice 
*************************************************************************/
int32_t TIM3_Encoder_GetData(void)
{
	int32_t Encoder_Data;   
	int32_t Time_CounterData;   
		
	Time_CounterData = TIM_GetCounter(TIM3) - TIM3_Encoder_InitialValue ;   //��ȡ��ǰ������ֵ ����ʱ��ֵ - ��ʼֵ  	
	
	/*�������TIM3_Encoder_OverflownΪ0*/
	if( TIM3_Encoder_Overflown == 0)                                        // ����ʹ�� �����������Ϻ��������                                   
	{                              
		Encoder_Data = Time_CounterData ;                                     // ��ǰ������ֵ������ת��������������ֵ
		return Encoder_Data ;                                                 // ���ر���ֵ
	}
	
	/*��һ��������ת TIM3_Encoder_InitialValue ��32000�������ϵ����*/
	Encoder_Data =  TIM3_Encoder_InitialValue * TIM3_Encoder_Overflown  + Time_CounterData ;  //�������ֵ����
	return Encoder_Data ;                                                   // ���ر���ֵ                           
}







/*************************************************************************
  * @brief  TIM3����Ϊ������ģʽ   
  * @param  ��
  * @retval ��
  * @notice 
*************************************************************************/
void  TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)                       //�����ж�
	{
		if((TIM3->CR1)&0X0010)                                                //���¼�1���
		{				  
       TIM3_Encoder_Overflown--;                                          //���¼�1��������������1
		} 
		else                                                                  //���ϼ�1���
		{				 
			 TIM3_Encoder_Overflown++;                                          //���ϼ�1��������������1
		}
		TIM_SetCounter(TIM3,TIM3_Encoder_InitialValue);                       //��ʼ����������ֵ
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);                           //���жϱ�־λ
	}
}







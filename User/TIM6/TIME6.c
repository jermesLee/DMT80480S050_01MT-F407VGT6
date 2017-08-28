#include "TIME6.h" 

/*******************************TIM6 ��������****************************/ 
uint32_t TIM6_TickCount = 0 ;                                             //�жϴ���������ÿ����һ���жϼ�һ,ע����32λ���ݣ����������ݿ��һ����                            
                                                                          //�����ݲ���32Ϊ����ʱ ����TIM6_xxMsFinish����������Ķ�ʱ�Ƿ��ѵ����жϷ�����һ������ 
																																					//���� ����32λ��ϵͳ�8��16��λ���� - 8��16��λ���� ������ 8��16��λ���� �����ǵ���Ĭ�ϵ�32λ����
/*******************************TIM6 �û�����****************************/ 
void TIM6_Init(void);                                                     //TIME6��ʼ��   
FlagStatus TIM6_50MsFinish(void);                                         //��ʱ50ms �� ��ʱʱ�䵽 ����SET ,���򷵻�RESET
FlagStatus TIM6_100MsFinish(void);                                        //��ʱ100ms , ��ʱʱ�䵽 ����SET ,���򷵻�RESET


/*******************************TIM6 �ڲ����� ***************************/ 
static void TIM6_MODE_Config(void);                                       //TIM6����ģʽ���� 
static void TIM6_NVIC_Config(void);                                       //TIM6�ж�����������
  
	
	
	
/*************************************************************************
  * @brief  TIM6 �ж����ȼ�����
  * @param  ��
  * @retval ��
  * @notice TIME6_IRQPreemptionPrio �� TIME6_IRQSubPrio�ڡ�TIM6.h������#define ���к궨��
**************************************************************************/
static void TIM6_NVIC_Config(void)                                        //�ж�����������
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    
		/*********TIM6 �ж����ȼ�����************/ 
    NVIC_InitStructure.NVIC_IRQChannel    = TIM6_DAC_IRQn ;	              //ָ���ж���  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = TIM6_IRQPreemptionPrio;  //������ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = TIM6_IRQSubPrio;	              //���ô����ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                       //�ж�������ʹ��
    NVIC_Init(&NVIC_InitStructure);                                       //����
}






/*************************************************************************
 * TIM_Period / Auto Reload Register(ARR) = 49    TIM_Prescaler--1679
 * �ж�����Ϊ = 1/(84MHZ /1680) * 50 = 1ms
 *
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIM_Period(ARR) --> �ж� ��TIMxCNT����Ϊ0���¼��� 
**************************************************************************/
static void TIM6_MODE_Config(void)                                        //TIM6����ģʽ���ã� ��λ ��ms ��
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
   
		/*********TIM6 ��ʱʱ������************/ 
   	TIM_TimeBaseStructure.TIM_Period    = TIM6_TimeTick * 50 - 1;         //�Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) */
    TIM_TimeBaseStructure.TIM_Prescaler = 1679;                           //����ϵ�� 1679
	
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		              //���ⲿʱ�ӽ��в�����ʱ�ӷ�Ƶ,����û���õ� 
	  TIM_TimeBaseStructure.TIM_RepetitionCounter=0;                        //ûʹ��
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;             //���ϼ���
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);                       //����
	
  	/**************ʹ���ж�****************/
    TIM_ClearFlag(TIM6, TIM_FLAG_Update);	                                //���жϱ�־ 
    TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);                              //ʹ���ж�		 
    TIM_Cmd(TIM6, ENABLE);                                                //ʹ�ܶ�ʱ��
}






/*************************************************************************
  * @brief  �򿪶�ʱ��TIM6
  * @param  MsTime ��  ��ʱʱ�� ����λ ��ms
  * @retval ��
  * @notice MsTime ���Ϊ 120ms ���ڴ˺��������޷�
*************************************************************************/
void TIM6_Init(void)
{
	/***********ʱ��ʹ��***************/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6 , ENABLE);                   //ʹ��TIME6��ʱ��

	/**********Ӳ������****************/
	TIM6_NVIC_Config();                                                     //��ʱ���жϴ� �������ж����ȼ�
	TIM6_MODE_Config();                                                     //��ʱ����ʱʱ������
}




/*************************************************************************
  * @brief  ��ʱ100ms
  * @param  ��
  * @retval ��ʱ��ɷ��� SET ,��ʱʱ�仹û������ RESET
  * @notice TIM6_FinishFlag�ڶ�ʱ�ж��ﱻ��λ
*************************************************************************/
FlagStatus TIM6_100MsFinish(void)
{
	static uint16_t  TimeTickNum = 100/TIM6_TimeTick ;                      // 100MS��ʱ���� �� ��Ҫ�Զ�ʱ��6�����жϼ���
	static uint32_t  TimeLastCount   = 0 ;                                  // ��ʼֵΪ 0 ��Ӧ��Ϊ32λ���ݣ� �����߿��һ�� ��

	if( TIM6_TickCount - TimeLastCount >= TimeTickNum )                     // �Ƚ�����TIM6_TickCount��ֵ����һ�ν��붨ʱ��ɵ�ֵ
	{
	  TimeLastCount	= TIM6_TickCount ;                                      //��ʱʱ�䵽 ������ζ�ʱ������ֵ��TimeLastCount �������´ζ�ʱ�ļ���
		return SET ;                                                          //��ʱʱ�䵽 ������ SET
	}		
	else  return RESET;                                                     //��ʱʱ�仹û�� ������ RESET
}




/*************************************************************************
  * @brief  ��ʱ50ms
  * @param  ��
  * @retval ��ʱ��ɷ��� SET ,��ʱʱ�仹û������ RESET
  * @notice TIM6_FinishFlag�ڶ�ʱ�ж��ﱻ��λ
*************************************************************************/
FlagStatus TIM6_50MsFinish(void)
{
	static uint16_t  TimeTickNum = 50/TIM6_TimeTick ;                       // 50MS��ʱ���� �� ��Ҫ�Զ�ʱ��6�����жϼ���
	static uint32_t  TimeLastCount   = 0 ;                                  // ��ʼֵΪ 0 ��Ӧ��Ϊ32λ���ݣ� �����߿��һ�� ��
	if( TIM6_TickCount - TimeLastCount >= TimeTickNum )                     // �Ƚ�����TIM6_TickCount��ֵ����һ�ν��붨ʱ��ɵ�ֵ
	{
	  TimeLastCount	= TIM6_TickCount ;                                      //��ʱʱ�䵽 ������ζ�ʱ������ֵ��TimeLastCount �������´ζ�ʱ�ļ���
		return SET ;                                                          //��ʱʱ�䵽 ������ SET
	}		
	else  return RESET;                                                     //��ʱʱ�仹û�� ������ RESET
}



/*************************************************************************
  * @brief  ��ʱ��TIM6 ISR������
  * @param  ��
  * @retval ��
  * @notice 
*************************************************************************/
void TIM6_DAC_IRQHandler(void)                                            //TIM6�жϷ�����
{
	if ( TIM_GetITStatus(TIM6 , TIM_IT_Update) != RESET )                   //����жϱ�־λ
	{
		TIM_ClearITPendingBit(TIM6 , TIM_FLAG_Update);                        //���жϱ�־λ	
		TIM6_TickCount++  ;                                                   //��ʱ��TIM6����
	}	
	LED1_TOGGLE();
}






/*********************************************END OF FILE**********************/



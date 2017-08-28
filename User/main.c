#include "includes.h"

typedef struct DisplayParametersType
{
	uint16_t SetFrequency ;                                                // ��������ź�Ƶ��
	uint32_t SetVoltageAmplitude;                                          // ���������ѹ��ֵ
	uint16_t ActualFrequency ;                                             // ʵ�������ѹƵ��
	uint32_t ActualVoltageAmplitude;                                       // ʵ�������ѹ��ֵ

}DisplayParametersType ;

DisplayParametersType DWIN_LCD_DisplayParameters ;

/************************************************************************
  * @brief  ������
  * @param  ��  
  * @retval �� 
	* @notice 
*************************************************************************/
int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);                         //����ϵͳ�ж����ȼ�����2
    LED_Init();                                                             //LED�������ų�ʼ������ ( ����ʱʹ�� )
	TIM6_Init();                                                            //��ʱ����ʱ��ʼ��
	DWIN_LCD_Init(115200);                                                  //������ͨ��Ӳ����ʼ��
	DWIN_LCD_DisplayInit();                                                 //���ϵ�ʱ��Ļ����ʾ��Ϣ��ʼ��
	TIM3_Encoder_Init();                                                    //Ƶ��  ��������ʱ����ʱ��TIM3������ģʽ��ʼ��
	TIM5_Encoder_Init();                                                    //�ߵ�ѹ��������ʱ����ʱ��TIM5������ģʽ��ʼ��
	FSMC_Init();                                                            //FSMC ���߳�ʼ��
	while(1)                                                                                                                                                                              
	{
		LED1_TOGGLE();
       // LED1_ON();
        

//         USART3_Debug();
       
        /***ÿ��100ms����Ҫ��ĵ�����д��LCD������**/ 	
		if( TIM6_100MsFinish() == SET )                                       //100MS��ʱʱ�䵽 
		{	
			LED1_TOGGLE();                                                      //����ʹ��
			DWIN_LCD_DisplayParameters.SetVoltageAmplitude = DWIN_LCD_DisplaySetLineVoltage(); //��ʾ�����ߵ�ѹֵ ������  �����ߵ�ѹֵ
			DWIN_LCD_DisplayParameters.SetFrequency        = DWIN_LCD_DisplaySetFrequency();	 //��ʾ����Ƶ��	    ������  ����Ƶ��

			FPGA_SetOutputFrequency (DWIN_LCD_DisplayParameters.SetFrequency);                 // �����õ������ѹƵ�ʷ��͸�FPGA
			FPGA_SetOutputVoltageAmplitude (DWIN_LCD_DisplayParameters.SetVoltageAmplitude);   //	�����õ������ѹ��ֵ���͸�FPGA
		}
		
   			
		/*********ÿ��50ms�������ͨ��һ��**********/ 
		if( TIM6_50MsFinish() == SET )                                        //50MS��ʱʱ�䵽 
		{	
			LED2_TOGGLE();                                                      //����ʹ��
			DWIN_LCD_AndMCUCommunication();                                     //���ڴ������ݷ��͸�������,�������յ�������
		}
	}
}















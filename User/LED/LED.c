#include "LED.h" 

/****************************�û�����************************************/
void LED_Init(void);                                                      //LED��ʼ������ ������MCU IO�� �ͳ�ʼ��IO�ڳ�ʼ����ƽ

/****************************�ڲ�����************************************/
static void LED_GPIO_Config(void);


/*************************************************************************
  * @brief  LED IO ������ 
  * @param  ��
  * @retval ��
  * @notice ��
*************************************************************************/
void LED_GPIO_Config(void)
{		
		GPIO_InitTypeDef GPIO_InitStructure;		                              //����һ��GPIO_InitTypeDef���͵Ľṹ��
		                                                   
	  /* Configure PG6 and PG8 in output pushpull mode */
		GPIO_InitStructure.GPIO_Pin    = GPIO_Pin_0;	                        //ѡ��Ҫ���Ƶ�GPIOH����			
		GPIO_InitStructure.GPIO_Mode   = GPIO_Mode_OUT;                       //�����ʽ
		GPIO_InitStructure.GPIO_OType  = GPIO_OType_PP;                       //������� 
		GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_100MHz;                   //������������Ϊ100MHz 
		GPIO_InitStructure.GPIO_PuPd   = GPIO_PuPd_UP;                        //����
		GPIO_Init(GPIOE, &GPIO_InitStructure);                                //���ÿ⺯������ʼ��GPIOH5
     	
	 	GPIO_InitStructure.GPIO_Pin    = GPIO_Pin_1;			                    //ѡ��Ҫ���Ƶ�GPIOA����														   	
		GPIO_Init(GPIOE, &GPIO_InitStructure);                                //���ÿ⺯������ʼ��GPIOA4

		GPIO_InitStructure.GPIO_Pin    = GPIO_Pin_2;			                    //ѡ��Ҫ���Ƶ�GPIOA����														   	
		GPIO_Init(GPIOE, &GPIO_InitStructure);          	                    //���ÿ⺯������ʼ��GPIOA6

		GPIO_SetBits(GPIOE, GPIO_Pin_0);	                                    //��ʼ������
		GPIO_SetBits(GPIOE, GPIO_Pin_1);	                                    //��ʼ������
		GPIO_SetBits(GPIOE, GPIO_Pin_2);	                                    //��ʼ������
}




/*************************************************************************
  * @brief  LED ��ʼ������ 
  * @param  ��
  * @retval ��
  * @notice ����ʱ�� ����Cortex-M3��ͬ ��IO���������AHB1 ������
*************************************************************************/
void LED_Init(void)
{
	 /**********����GPIOH��GPIOA������ʱ��******/  
//   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH |RCC_AHB1Periph_GPIOA, ENABLE);	 
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH , ENABLE);	   
	 /**********LED�õ���io������***************/
 	 LED_GPIO_Config(); 
}



/*********************************************END OF FILE**********************/

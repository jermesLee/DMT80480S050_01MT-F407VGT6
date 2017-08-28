#include "FSMC.h"

static void FSMC_GPIO_Config(void);                                       //��ʼ��FSMC I/O ��������
static void FSMC_MODE_Config(void);                                       //FSMC����ģʽ��ʱ��ʱ������
void FSMC_Init(void);                                                     //fsmc ���߳�ʼ���ܺ���




/*************************************************************************
  * @brief  ��ʼ��FSMC I/O ��������
  * @param  ��
  * @retval ��
  * @notice ����ʵ���õ������ſ��Խ��вü� �����ڲ��õ�FSMC���ſ��Ե���ͨ��I/O��ʹ��
*************************************************************************/
static void FSMC_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;  		   

	
/******************************������ FSMC_D[15..0] ********************** 
	     FSMC_D0   --->  PD14     // PORT D
	     FSMC_D1   --->  PD15     //
	     FSMC_D2   --->  PD0      //
	     FSMC_D3   --->  PD1      // 
	     FSMC_D4   --->  PE7      // PORT E
	     FSMC_D5   --->  PE8      //
	     FSMC_D6   --->  PE9      //
	     FSMC_D7   --->  PE10     //
	     FSMC_D8   --->  PE11     //
	     FSMC_D9   --->  PE12     //
	     FSMC_D10  --->  PE13     //
	     FSMC_D11  --->  PE14     //
	     FSMC_D12  --->  PE15     //
	     FSMC_D13  --->  PD8      // PORT D
	     FSMC_D14  --->  PD9      //
	     FSMC_D15  --->  PD10     //
***************************************************************************/
    /* ����FSMC���Ӧ��������,FSMC D0~D15*/	
		GPIO_InitStructure.GPIO_Mode   = GPIO_Mode_AF;                        //���÷�ʽ
		GPIO_InitStructure.GPIO_OType  = GPIO_OType_PP;                       //������� 
		GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_100MHz;                   //������������Ϊ100MHz 
		GPIO_InitStructure.GPIO_PuPd   = GPIO_PuPd_UP ;                       //����
 		
		GPIO_PinAFConfig(GPIOD,GPIO_PinSource14,GPIO_AF_FSMC);                //FSMC_D0   --->  PD14
		GPIO_PinAFConfig(GPIOD,GPIO_PinSource15,GPIO_AF_FSMC);                //FSMC_D1   --->  PD15
		GPIO_PinAFConfig(GPIOD,GPIO_PinSource0 ,GPIO_AF_FSMC);                //FSMC_D2   --->  PD0
		GPIO_PinAFConfig(GPIOD,GPIO_PinSource1 ,GPIO_AF_FSMC);                //FSMC_D3   --->  PD1
		GPIO_PinAFConfig(GPIOE,GPIO_PinSource7 ,GPIO_AF_FSMC);                //FSMC_D4   --->  PE7
		GPIO_PinAFConfig(GPIOE,GPIO_PinSource8 ,GPIO_AF_FSMC);                //FSMC_D5   --->  PE8
		GPIO_PinAFConfig(GPIOE,GPIO_PinSource9 ,GPIO_AF_FSMC);                //FSMC_D6   --->  PE9
		GPIO_PinAFConfig(GPIOE,GPIO_PinSource10,GPIO_AF_FSMC);                //FSMC_D7   --->  PE10
		GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_FSMC);                //FSMC_D8   --->  PE11
		GPIO_PinAFConfig(GPIOE,GPIO_PinSource12,GPIO_AF_FSMC);                //FSMC_D9   --->  PE12
		GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_FSMC);                //FSMC_D10  --->  PE13
		GPIO_PinAFConfig(GPIOE,GPIO_PinSource14,GPIO_AF_FSMC);                //FSMC_D11  --->  PE14
		GPIO_PinAFConfig(GPIOE,GPIO_PinSource15,GPIO_AF_FSMC);                //FSMC_D12  --->  PE15
		GPIO_PinAFConfig(GPIOD,GPIO_PinSource8 ,GPIO_AF_FSMC);                //FSMC_D13  --->  PD8
		GPIO_PinAFConfig(GPIOD,GPIO_PinSource9 ,GPIO_AF_FSMC);                //FSMC_D14  --->  PD9
		GPIO_PinAFConfig(GPIOD,GPIO_PinSource10,GPIO_AF_FSMC);                //FSMC_D15  --->  PD10


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9 |              
                                  GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15;                                           
    GPIO_Init(GPIOD, &GPIO_InitStructure);                                // PORT D  ��FSMC ���ŵ�����                                                    
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7  | GPIO_Pin_8  | GPIO_Pin_9  | GPIO_Pin_10 | 
                                  GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | 
                                  GPIO_Pin_15;
    GPIO_Init(GPIOE, &GPIO_InitStructure);                                // PORT E  ��FSMC ���ŵ�����     
 
 
 
 
 
/******************************��ַ�� FSMC_A[25..0] ********************** 
	     FSMC_A0   --->  PF0     // PORT F 
	     FSMC_A1   --->  PF1     // 
	     FSMC_A2   --->  PF2     // 
	     FSMC_A3   --->  PF3     // 
	     FSMC_A4   --->  PF4     // 
	     FSMC_A5   --->  PF5     // 
	     FSMC_A6   --->  PF12    // 
	     FSMC_A7   --->  PF13    // 
	     FSMC_A8   --->  PF14    // 
	     FSMC_A9   --->  PF15    // 
	     FSMC_A10  --->  PG0     // PORT G
	     FSMC_A11  --->  PG1     // 
	     FSMC_A12  --->  PG2     // 
	     FSMC_A13  --->  PG3     // 
	     FSMC_A14  --->  PG4     // 
	     FSMC_A15  --->  PG5     // 
	     FSMC_A16  --->  PD11    // PORT D 
	     FSMC_A17  --->  PD12    // 
	     FSMC_A18  --->  PD13    // 
	     FSMC_A19  --->  PE3     // PORT E
	     FSMC_A20  --->  PE4     // 
	     FSMC_A21  --->  PE5     // 
	     FSMC_A22  --->  PE6     // 
	     FSMC_A23  --->  PE2     // 
	     FSMC_A24  --->  PG13    // PORT G
	     FSMC_A25  --->  PG14    // 
*************************************************************************/

  /* ����FSMC���Ӧ��������,FSMC A0~A25 */
		
//		GPIO_PinAFConfig(GPIOF,GPIO_PinSource0 ,GPIO_AF_FSMC);              //FSMC_A0   --->  PF0
//		GPIO_PinAFConfig(GPIOF,GPIO_PinSource1 ,GPIO_AF_FSMC);              //FSMC_A1   --->  PF1
//		GPIO_PinAFConfig(GPIOF,GPIO_PinSource2 ,GPIO_AF_FSMC);              //FSMC_A2   --->  PF2
//		GPIO_PinAFConfig(GPIOF,GPIO_PinSource3 ,GPIO_AF_FSMC);              //FSMC_A3   --->  PF3
//		GPIO_PinAFConfig(GPIOF,GPIO_PinSource4 ,GPIO_AF_FSMC);              //FSMC_A4   --->  PF4
//		GPIO_PinAFConfig(GPIOF,GPIO_PinSource5 ,GPIO_AF_FSMC);              //FSMC_A5   --->  PF5
//		GPIO_PinAFConfig(GPIOF,GPIO_PinSource12,GPIO_AF_FSMC);              //FSMC_A6   --->  PF6
//		GPIO_PinAFConfig(GPIOF,GPIO_PinSource13,GPIO_AF_FSMC);              //FSMC_A7   --->  PF7
//		GPIO_PinAFConfig(GPIOF,GPIO_PinSource14,GPIO_AF_FSMC);              //FSMC_A8   --->  PF8
//		GPIO_PinAFConfig(GPIOF,GPIO_PinSource15,GPIO_AF_FSMC);              //FSMC_A9   --->  PF9
//		GPIO_PinAFConfig(GPIOF,GPIO_PinSource0 ,GPIO_AF_FSMC);              //FSMC_A10  --->  PG0
//		GPIO_PinAFConfig(GPIOG,GPIO_PinSource1 ,GPIO_AF_FSMC);              //FSMC_A11  --->  PG1
//		GPIO_PinAFConfig(GPIOG,GPIO_PinSource2 ,GPIO_AF_FSMC);              //FSMC_A12  --->  PG2
//		GPIO_PinAFConfig(GPIOG,GPIO_PinSource3 ,GPIO_AF_FSMC);              //FSMC_A13  --->  PG3
//		GPIO_PinAFConfig(GPIOG,GPIO_PinSource4 ,GPIO_AF_FSMC);              //FSMC_A14  --->  PG4
//		GPIO_PinAFConfig(GPIOG,GPIO_PinSource5 ,GPIO_AF_FSMC);              //FSMC_A15  --->  PG5
		GPIO_PinAFConfig(GPIOD,GPIO_PinSource11,GPIO_AF_FSMC);                //FSMC_A16  --->  PD11
		GPIO_PinAFConfig(GPIOD,GPIO_PinSource12,GPIO_AF_FSMC);                //FSMC_A17  --->  PD12
		GPIO_PinAFConfig(GPIOD,GPIO_PinSource13,GPIO_AF_FSMC);                //FSMC_A18  --->  PD13
		GPIO_PinAFConfig(GPIOE,GPIO_PinSource3 ,GPIO_AF_FSMC);                //FSMC_A19  --->  PE3
		GPIO_PinAFConfig(GPIOE,GPIO_PinSource4 ,GPIO_AF_FSMC);                //FSMC_A20  --->  PE4
		GPIO_PinAFConfig(GPIOE,GPIO_PinSource5 ,GPIO_AF_FSMC);                //FSMC_A21  --->  PE5	
		GPIO_PinAFConfig(GPIOE,GPIO_PinSource6 ,GPIO_AF_FSMC);                //FSMC_A22  --->  PE6
		GPIO_PinAFConfig(GPIOE,GPIO_PinSource2 ,GPIO_AF_FSMC);                //FSMC_A23  --->  PE2
		GPIO_PinAFConfig(GPIOG,GPIO_PinSource13,GPIO_AF_FSMC);                //FSMC_A24  --->  PG13
		GPIO_PinAFConfig(GPIOG,GPIO_PinSource14,GPIO_AF_FSMC);                //FSMC_A25  --->  PG14
		

//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2  | GPIO_Pin_3  | 
//                                GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_12 | GPIO_Pin_13 | 
//                                GPIO_Pin_14 | GPIO_Pin_15;
//  GPIO_Init(GPIOF, &GPIO_InitStructure);                                // PORT F  ��FSMC ���ŵ�����   
  
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | 
//                                GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_13| GPIO_Pin_14;                               
//  GPIO_Init(GPIOG, &GPIO_InitStructure);                                // PORT G  ��FSMC ���ŵ�����   
  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13| GPIO_Pin_14;                 // PORT G  ��FSMC ���ŵ�����                             
  GPIO_Init(GPIOG, &GPIO_InitStructure); 
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 |
                                GPIO_Pin_6 ; 
  GPIO_Init(GPIOE, &GPIO_InitStructure);                                  // PORT E  ��FSMC ���ŵ�����   
    
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13; 
  GPIO_Init(GPIOD, &GPIO_InitStructure);                                  // PORT D  ��FSMC ���ŵ�����   
   
	 	



/********************������ FSMC_NOE , FSMC_NWE ,FSMC_NWAIT ***************** 
	     FSMC_NOE   --->  PD4     // PORT D 
	     FSMC_NWE   --->  PD5     //  
	     FSMC_NWAIT --->  PD6     //  
*****************************************************************************/	  

		GPIO_PinAFConfig(GPIOD,GPIO_PinSource4 ,GPIO_AF_FSMC);                //FSMC_NOE   --->  PD4 
		GPIO_PinAFConfig(GPIOD,GPIO_PinSource5 ,GPIO_AF_FSMC);                //FSMC_NWE   --->  PD5
//		GPIO_PinAFConfig(GPIOD,GPIO_PinSource6 ,GPIO_AF_FSMC);              //FSMC_NWAIT --->  PD6 

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 |GPIO_Pin_5;                 // NOE and NWE configuration 
		GPIO_Init(GPIOD, &GPIO_InitStructure);
  
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;                             // NWAIT configuration 
//  GPIO_Init(GPIOD, &GPIO_InitStructure);





/****************************************************************************
2^26 =0X0400 0000 = 64MB,ÿ�� BANK ��4*64MB = 256MB
64MB:FSMC_Bank1_NORSRAM1:0X6000 0000 ~ 0X63FF FFFF
64MB:FSMC_Bank1_NORSRAM2:0X6400 0000 ~ 0X67FF FFFF
64MB:FSMC_Bank1_NORSRAM3:0X6800 0000 ~ 0X6BFF FFFF
64MB:FSMC_Bank1_NORSRAM4:0X6C00 0000 ~ 0X6FFF FFFF
*****************************************************************************/
/*****************************NOR/PSRAM �洢������****************************
	     FSMC_NE1 / FSMC_NCE2     --->  PD7     // PORT D 
	     FSMC_NE2 / FSMC_NCE3     --->  PG9     // PORT G
	     FSMC_NE3 / FSMC_NCE4_1   --->  PG10    // 
	     FSMC_NE4                 --->  PG12    // 
	     FSMC_CLK                 --->  PD3     // PORT D
	     FSMC_NBL1                --->  PE1     // PORT E 
	     FSMC_NBL0                --->  PE0     // 
	     FSMC_NCE4_2              --->  PG11    // PORT G 
	     FSMC_NL(NADV)            --->  PB7     // PORT B 
******************************************************************************/   
		GPIO_PinAFConfig(GPIOD,GPIO_PinSource7 ,GPIO_AF_FSMC);                  //FSMC_NE1 / FSMC_NCE2     --->  PD7 
//		GPIO_PinAFConfig(GPIOG,GPIO_PinSource9 ,GPIO_AF_FSMC);                //FSMC_NE2 / FSMC_NCE3     --->  PG9
//		GPIO_PinAFConfig(GPIOG,GPIO_PinSource10,GPIO_AF_FSMC);                //FSMC_NE3 / FSMC_NCE4_1   --->  PG10 
//		GPIO_PinAFConfig(GPIOG,GPIO_PinSource12,GPIO_AF_FSMC);                //FSMC_NE4                 --->  PG12
//		GPIO_PinAFConfig(GPIOD,GPIO_PinSource3 ,GPIO_AF_FSMC);                //FSMC_CLK                 --->  PD3
//    GPIO_PinAFConfig(GPIOE,GPIO_PinSource1 ,GPIO_AF_FSMC);                //FSMC_NBL1                --->  PE1
//    GPIO_PinAFConfig(GPIOE,GPIO_PinSource0 ,GPIO_AF_FSMC);                //FSMC_NBL0                --->  PE0  
//    GPIO_PinAFConfig(GPIOG,GPIO_PinSource11,GPIO_AF_FSMC);                //FSMC_NCE4_2              --->  PG11 
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource7 ,GPIO_AF_FSMC);                  //FSMC_NL(NADV)            --->  PB7  

  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; 
		GPIO_Init(GPIOD, &GPIO_InitStructure);                                  // NE1 configuration 
    
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 
//  GPIO_Init(GPIOG, &GPIO_InitStructure);                                  // NE2 configuration 
//    
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
//  GPIO_Init(GPIOG, &GPIO_InitStructure);                                  // NE3 configuration 
//    
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; 
//  GPIO_Init(GPIOG, &GPIO_InitStructure);                                  // NE4 configuration 
// 
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; 
//  GPIO_Init(GPIOD, &GPIO_InitStructure);                                  // CLK configuration 
//  
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1; 
//  GPIO_Init(GPIOE, &GPIO_InitStructure);                                  // NBL0, NBL1 configuration 
//  
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 ; 
//  GPIO_Init(GPIOG, &GPIO_InitStructure);                                  //  FSMC_NCE4_2 configuration 
//  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 ; 
  GPIO_Init(GPIOB, &GPIO_InitStructure);                                  //  FSMC_NL     configuration 
  
	
}





/*************************************************************************
  * @brief  FSMC����ģʽ������ʱ��ʱ�������
  * @param  ��
  * @retval ��
  * @notice ����ʱ��ʱ���ɱ��ش洢��ʱ�����ó�
*************************************************************************/
static void FSMC_MODE_Config(void)
{
    FSMC_NORSRAMInitTypeDef        FSMC_NORSRAMInitStructure;                                      // FSMC����ģʽ����
    FSMC_NORSRAMTimingInitTypeDef  FSMC_NORSRAMReadTimingInitStructure;                            // FSMC����ʱ��ʱ����� 
    FSMC_NORSRAMTimingInitTypeDef  FSMC_NORSRAMWriteTimingInitStructure;                           // FSMC����ʱ��ʱ����� 
    
	  /*******************дʱ��*****************/
  	FSMC_NORSRAMWriteTimingInitStructure.FSMC_AddressSetupTime      = 0x01;	                       //SetTimeValue(0-15)  + 1 :��ַ����ʱ�� ( �첽 )   
    FSMC_NORSRAMWriteTimingInitStructure.FSMC_AddressHoldTime       = 0x01;	                       //SetTimeValue(0-15)  + 1 :��ַ����ʱ�� ( �첽 ����IO )
    FSMC_NORSRAMWriteTimingInitStructure.FSMC_DataSetupTime         = 0x02;		                     //SetTimeValue(1-255) + 1 :���ݽ���ʱ�� ( �첽 )
    FSMC_NORSRAMWriteTimingInitStructure.FSMC_BusTurnAroundDuration = 0x0F;                        //SetTimeValue(0-15)  + 1 :���߻ָ�ʱ�� ( �첽��ͬ���� ����һ�ζ�����֮���������Ҫ������������Ϊ�´β����ͳ���ַ �������ʱ����Ϊ�˷�ֹ���߳�ͻ
    FSMC_NORSRAMWriteTimingInitStructure.FSMC_CLKDivision           = 0x00;                        //SetTimeValue(1-15)  + 1 :ʱ�ӷ�Ƶ����� ͬ��ͨ��ʹ�� �� ���� HCLK ����Ϊ��λ
    FSMC_NORSRAMWriteTimingInitStructure.FSMC_DataLatency           = 0x00;                        //SetTimeValue(0-15)  + 2 :�����ӳ�ʱ�䣨 ͬ��ͨ��ʹ�� ��������ͬ������ģʽ��NOR���� ����Ҫ�����ڶ�ȡ��һ������֮ǰ�ȴ��Ĵ洢��������Ŀ ����λ��CLK ,����HCLK �������첽�����в�������
  	FSMC_NORSRAMWriteTimingInitStructure.FSMC_AccessMode            = FSMC_AccessMode_D;	         //����ģʽ D
   
	 /********************��ʱ��*****************/
  	FSMC_NORSRAMReadTimingInitStructure.FSMC_AddressSetupTime       = 0x01;	                       //SetTimeValue(0-15)  + 1 :��ַ����ʱ�� ( �첽 )   
    FSMC_NORSRAMReadTimingInitStructure.FSMC_AddressHoldTime        = 0x01;	                       //SetTimeValue(0-15)  + 1 :��ַ����ʱ�� ( �첽 ����IO )
    FSMC_NORSRAMReadTimingInitStructure.FSMC_DataSetupTime          = 0x03;		                     //SetTimeValue(1-255) + 1 :���ݽ���ʱ�� ( �첽 )
    FSMC_NORSRAMReadTimingInitStructure.FSMC_BusTurnAroundDuration  = 0x00;                        //SetTimeValue(0-15)  + 1 :���߻ָ�ʱ�� ( �첽��ͬ���� ����һ�ζ�����֮���������Ҫ������������Ϊ�´β����ͳ���ַ �������ʱ����Ϊ�˷�ֹ���߳�ͻ
    FSMC_NORSRAMReadTimingInitStructure.FSMC_CLKDivision            = 0x00;                        //SetTimeValue(1-15)  + 1 :ʱ�ӷ�Ƶ����� ͬ��ͨ��ʹ�� �� ���� HCLK ����Ϊ��λ
    FSMC_NORSRAMReadTimingInitStructure.FSMC_DataLatency            = 0x00;                        //SetTimeValue(0-15)  + 2 :�����ӳ�ʱ�䣨 ͬ��ͨ��ʹ�� ��������ͬ������ģʽ��NOR���� ����Ҫ�����ڶ�ȡ��һ������֮ǰ�ȴ��Ĵ洢��������Ŀ ����λ��CLK ,����HCLK �������첽�����в�������
  	FSMC_NORSRAMReadTimingInitStructure.FSMC_AccessMode             = FSMC_AccessMode_A;	         //����ģʽ A
 
    /***************�Ĵ�������****************/ 
    FSMC_NORSRAMInitStructure.FSMC_Bank                  = FSMC_Bank1_NORSRAM1;                    //ʹ��FSMC_BANK1 ���Ӱ�� 1                  
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux        = FSMC_DataAddressMux_Disable;            //���õ�ַ���ݸ���
    FSMC_NORSRAMInitStructure.FSMC_MemoryType            = FSMC_MemoryType_SRAM;                   //�洢������Ϊ �� SRAM
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth       = FSMC_MemoryDataWidth_16b;               //�洢�����ݿ��Ϊ �� 16λ
    FSMC_NORSRAMInitStructure.FSMC_WriteOperation        = FSMC_WriteOperation_Enable;             //�Ƿ�����Դ洢������д���� 
	  FSMC_NORSRAMInitStructure.FSMC_ExtendedMode          = FSMC_ExtendedMode_Enable;               //�Ƿ�����ʹ����չ���� �� ��дʹ�ò�ͬ��ʱ�� ���� ������ʹ��FSMC_SWTR�Ĵ�����
    FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait      = FSMC_AsynchronousWait_Disable ;         //�첽ͨ�ŵȴ� ��ʹ��  
		FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode       = FSMC_BurstAccessMode_Disable;           //�ر�FSMCͻ���������ģʽ �� �ڳ���ģʽ��ʹ�� ��
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity    = FSMC_WaitSignalPolarity_Low;            //�ȴ��ź���Ч���� �� �ڳ���ģʽ��ʹ�� ��
    FSMC_NORSRAMInitStructure.FSMC_WrapMode              = FSMC_WrapMode_Disable;                  //�����������Ƿ�֧�ְɷǶ����AHB��������ָ�Ϊ2�����Բ����� �ڳ���ģʽ��ʹ�� ��
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive      = FSMC_WaitSignalActive_BeforeWaitState;  //�����洢���ڵȴ�״̬ǰ��һ��ʱ�����ڲ���NWAIT�ź� �����ǵȴ�״̬�ڼ����NWAIT�ź� �� �ڳ���ģʽ��ʹ�� ��
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal            = FSMC_WaitSignal_Disable;                //�Ƿ�����ͨ��NWAIT�źŲ���ȴ�״̬ �� ���鴫��ģʽ��ʹ�� ��
    FSMC_NORSRAMInitStructure.FSMC_WriteBurst            = FSMC_WriteBurst_Disable;                //�Ƿ�����ͨ��NWAIT�źŲ���ȴ�״̬ �� ��������ͬ�����鴫��Э��ʹ��λ�� ��FSMC_BCRx�Ĵ�����BURETENλ �� ���鴫��ģʽ��ʹ�� ��
    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &FSMC_NORSRAMReadTimingInitStructure;   //��ʱ�����
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct     = &FSMC_NORSRAMWriteTimingInitStructure;  //дʱ�����  
    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);                                                  //������Ϣд����Ӧ�Ĵ���
    
    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);                                                  //ʹ�� FSMC_Bank1 �Ӱ�� 1 
                                        
}





/*************************************************************************
  * @brief  FSMC��ʼ���ܺ���
  * @param  ��
  * @retval ��
  * @notice ʱ��ʹ�� �� FSMC_GPIO_Config()�е�������Ҫ����ʵ�������������
*************************************************************************/
	
void FSMC_Init(void)
{
    RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE);                                           // ʹ��FSMCʱ��
    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE |                          // ʹ��FSMC��Ӧ��Ӧ�ܽ�ʱ��
	                          RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOG |
	                          RCC_AHB1Periph_GPIOB , ENABLE);
    FSMC_GPIO_Config();                                                                            //��ʼ��FSMC I/O ��������
    FSMC_MODE_Config();                                                                            //FSMC����ģʽ��ʱ��ʱ������
}





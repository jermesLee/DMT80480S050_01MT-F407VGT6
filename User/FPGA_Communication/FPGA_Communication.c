#include "FPGA_Communication.h"

/************************** FPGA���ܺ�������*************************/ 
void FPGA_SetOutputFrequency (uint32_t OutputFrequency);                  //����FPGA ���Ƶ��ֵ ,��λ ��   1 LSB = 1 Hz 
void FPGA_SetOutputVoltageAmplitude (uint32_t OutputAmplitude );          //����FPGA �����ѹ�ķ�ֵ ,��λ��1 LSB = 0.01v 


/************************************************************************
  * @brief  ����FPGA ���Ƶ��ֵ
  * @param  ����źŵ�Ƶ��ֵ ,��λ ��1 LSB = 1 Hz 
  * @retval �� 
	* @notice ʵ���Ǽ���������˷�Ƶϵ�� 
*************************************************************************/
void FPGA_SetOutputFrequency (uint32_t OutputFrequency)
{
	uint32_t FrequencyDivsion;                                              // ��Ƶϵ���� 32 λ ��
	uint16_t FrequencyDivsionH;                                             // ��Ƶϵ���ĸ�16λ 
	uint16_t FrequencyDivsionL;                                             // ��Ƶϵ���ĵ�16λ
	
	/********************���õķ�Ƶϵ�������Ƶ�ʵĹ�ϵ********************/
	OutputFrequency   = OutputFrequency * 6 ;                               // ��Ƶϵ�������Ƶ�ʵĹ�ϵ�� FPGA �ĳ�����ƾ��� 
	FrequencyDivsion  = (FPGA_StandardInputFrequency + OutputFrequency )/( OutputFrequency << 1 )-1; //��OutputFrequency �� �������� ��Ч��
	FrequencyDivsionH = (uint16_t)(FrequencyDivsion >> 16 );                // ���� ��16λ        
	FrequencyDivsionL = (uint16_t)(FrequencyDivsion & 0X0000FFFF ) ;        // ���� ��16λ
	
	/**************************����FSMC���͸�FPGA**************************/
	FSMC_BANK1_FPGAWriteHalfWord(FPGA_SetFrequencyDivsionAddressH , FrequencyDivsionH);  // ���ͷ�Ƶϵ���ĸ� 8 λ 
	FSMC_BANK1_FPGAWriteHalfWord(FPGA_SetFrequencyDivsionAddressL , FrequencyDivsionL);  // ���ͷ�Ƶϵ���ĵ� 8 λ 
	FSMC_BANK1_FPGAWriteHalfWord(FPGA_SetDataLatchAddress , 1);             //�������������ź� ��ʵ��������һ����ַ�ߵ������� ������û��ʵ������                     
}






/************************************************************************
  * @brief  ����FPGA �����ѹ�ķ�ֵ
  * @param  ���������ѹ�ķ�ֵ ����λ��1LSB = 0.01v 
  * @retval �� 
	* @notice ʵ����Ŀ��ͨ��16λ��DAC��� ������ʵ�����õ���DACֵ
*************************************************************************/
void FPGA_SetOutputVoltageAmplitude (uint32_t OutputAmplitude )
{
	uint16_t DAC16BitValue;                                                 //16λDACֵ
	
	/******************���õ������ѹ��ֵ��DAC16ֵ�Ĺ�ϵ ******************/
	DAC16BitValue = OutputAmplitude * 65536 / 250 ;                         //DAC16BitValue = (OutputAmplitude / 100 ) * 65536 / 2.5 ;100������ 1LSB = 0.01v 
	                                                                        // 2.5 Ϊ �ο���ѹ 2.5V ,65536 Ϊ  16λ DAC��������
	/**************************����FSMC���͸�FPGA**************************/
	FSMC_BANK1_FPGAWriteHalfWord(FPGA_SetDAC16BitValueAddress , DAC16BitValue);  // ����DAC16ֵ 
	FSMC_BANK1_FPGAWriteHalfWord(FPGA_SetDataLatchAddress , 1);             //�������������ź� ��ʵ��������һ����ַ�ߵ������� ������û��ʵ������                     
}




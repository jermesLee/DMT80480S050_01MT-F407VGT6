#ifndef _FPGA_COMMUNICATION_H_
#define	_FPGA_COMMUNICATION_H_

#include "includes.h"                                                     // ���е�ͷ�ļ�
#include "FSMC.h"


/***********************FPGA��ַ����˵��********************************/ 
#define FPGA_StandardInputFrequency             ((uint32_t)90000000)      //FPGA ��׼����Ƶ�� 90M

/*************д���ݵ�ַ************/
#define FPGA_SetDataLatchAddress                (1 << 24)                 //�������������浽�Ĵ������ź��ߵ�ַ ��ʵ����������һ����ַ�ߵı���������

#define FPGA_SetFrequencyDivsionAddressH        (1 << 20)                 //���� FPGA ���η�Ƶϵ���ĸ�16λ���ݵ�ַ 
#define FPGA_SetFrequencyDivsionAddressL        (2 << 20)                 //���� FPGA ���η�Ƶϵ���ĵ�16λ���ݵ�ַ     
#define FPGA_SetDAC16BitValueAddress            (3 << 20)                 //���� FPGA ������εķ�ֵ ��16λDAC��Ӧ����ֵ ����ַ      


/*************�����ݵ�ַ************/




/************************** FPGA���ܺ�������*************************/ 
extern void FPGA_SetOutputFrequency (uint32_t OutputFrequency);           //����FPGA ���Ƶ��ֵ ,��λ ��   1 LSB = 1 Hz 
extern void FPGA_SetOutputVoltageAmplitude (uint32_t OutputAmplitude );   //����FPGA �����ѹ�ķ�ֵ ,��λ��1 LSB = 0.01v 




#endif /* __DWIN_LCD_H */



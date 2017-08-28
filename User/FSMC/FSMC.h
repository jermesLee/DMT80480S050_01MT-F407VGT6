#ifndef _FSMC_H_
#define _FSMC_H_

#include "includes.h"

/******************************���ݷ��ʿ�Ⱥ궨��************************/       //����궨������˲�ͬ���ʿ���µĵ�ַ���ߵ�ӳ���ϵ 
#define FSMC_BANK1_DataWidth                         FSMC_BANK1_DataWidth16Bit   //���ݷ��ʿ�Ⱥ궨��  ��FSMC_BANK1_DataWidth16Bit  ��ʾ16λ�ķ��� ��
                                                                                 //FSMC_BANK1_DataWidth8Bit  ��ʾ8λ�ķ��� ��


/*********************���ݷ��ʿ������ѡ�񣨲������޸ģ�*****************/
#define FSMC_BANK1_DataWidth16Bit                     1                   // 1 ��ʾFSMC��������Ϊ16λ , ���� �����ھ�����ַ�ߵ�ӳ���ϵ
#define FSMC_BANK1_DataWidth8Bit                      0                   // 0 ��ʾFSMC��������Ϊ8λ �� ���� �����ھ�����ַ�ߵ�ӳ���ϵ

#ifdef  FSMC_BANK1_DataWidth                                              // BANK1 ��ַ������  ,���ʷ�ʽ ��16λ     
#define FSMC_BANK1_AddressGenerate(Addr)             ((uint32_t)( ( ( Addr ) <<1 ) & 0X03FFFFFF | 0X60000000 ))                             
#else                                                                     // BANK1 ��ַ������  �����ʷ�ʽ��8λ     
#define FSMC_BANK1_AddressGenerate(Addr)             ((uint32_t)( ( Addr ) & 0X03FFFFFF | 0X60000000 ))                                   
#endif


/********************************���ݷ��ʶ�д*****************************/
#define FSMC_BANK1_FPGAWriteHalfWord(Addr , Value)   (*(volatile uint16_t *)FSMC_BANK1_AddressGenerate(Addr))=((uint16_t)Value)          // ��BANK1��д����
#define FSMC_BANK1_FPGAReadHalfWord(Addr)            (*(volatile uint16_t *)FSMC_BANK1_AddressGenerate(Addr))                            //��ȡBANK1�������� 

extern void FSMC_Init(void);                                              //FSMC ���߳�ʼ���ܺ���

#endif



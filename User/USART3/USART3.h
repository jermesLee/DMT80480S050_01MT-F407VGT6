#ifndef _USART3_H_
#define	_USART3_H_

#include "includes.h"

/************************USART3  �ڴ�����С�궨��**********************/
#define USART3_SendSizeMax                 256                            //���ڷ���һ�η��͵�����ֽ���(DMA��ʽ)
#define USART3_ReceiveSizeMax              256                            //���ڽ���һ���Խ��յ�����ֽ�����DMA��ʽ��
#define USART3_DMASendOneSizeMax            32                            //DMAһ���������͵�����ֽ��� �����ڷ��͵�һ֡�������ڴ������ܱ����ģ�������д���� �� �������Ѿ������˵������ڴ��޷��õ��ͷ� ��
                                                                          //Ϊ��������⣬����һ�η��͵�����ֽ�������һ֡���ݱȽϳ�ʱ��Ӱ���ر����������һ��һ��DMA�ܹ���������һ�����ݣ�������ͨ��ʱ�ڴ���ͷ�
/************************USART3  DMA���ͽ����ж����ȼ�*******************/
#define USART3_IRQPreemptionPrio            2                             //USART3��ռʽ���ȼ� ( ���ڽ��� ) ,������ռʽ���ȼ���DMA1_Channel5_IRQPreemptionPrio�� 
#define USART3_IRQSubPrio                   2                             //USART3�����ȼ�

#define DMA1_Stream4_IRQPreemptionPrio      1                             //USART3 DMA1_Stream4 ������ռʽ���ȼ� �� ���ڷ��� ��
#define DMA1_Stream4_IRQSubPrio             2                             //USART3 DMA1_Stream4 ��������ȼ�                   

#define DMA1_Stream1_IRQPreemptionPrio      1                             //USART3 DMA1_Stream1 ������ռʽ���ȼ� �� ���ڽ��� ����������ռʽ���ȼ���USART3_IRQPreemptionPrio ��
#define DMA1_Stream1_IRQSubPrio             1                             //USART3 DMA1_Stream1 ��������ȼ�                   




/**************************USART3  �û�����*******************************/
extern void USART3_Init(uint32_t Baudrate);                               //USART3���� �� Baudrate ��������
extern ErrorStatus USART3_WriteDMASendMulData(uint8_t *Data   , uint16_t  Num);  //���ڷ�������  ��Num ����Ҫ���͵������� 
extern ErrorStatus USART3_ReadDMAReceiveMulData(uint8_t *Data , uint16_t  Num);  //���ڶ�ȡ����  ��Num �����ؽ��յ���������

extern void USART3_Debug(void) ;


#endif /* __USART3_H */

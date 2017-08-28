#ifndef __TIM3_ENCODER_H
#define	__TIM3_ENCODER_H

#include "includes.h"


/****************************��ʱ����ʱ�ж����ȼ�*************************/
#define TIM3_IRQPreemptionPrio   1                                         //TIM4����ռʽ���ȼ�      
#define TIM3_IRQSubPrio          3                                         //TIM4�Ĵ����ȼ�

/*************************TIM3 �������û�����**************************/
extern void TIM3_Encoder_Init(void);                                       //TIME4����Ϊ������ģʽ                                      
extern void TIM3_Encoder_SetData(int32_t EncoderValue);                    //���ñ��������� ��-2^31-1  ~  2^31 - 1��
extern int32_t TIM3_Encoder_GetData(void);                                 //��ȡ���������� ��-2^31-1  ~  2^31 - 1��



#endif /* __PWM_OUTPUT_H */


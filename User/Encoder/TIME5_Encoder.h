#ifndef __TIM5_ENCODER_H
#define	__TIM5_ENCODER_H

#include "includes.h"


/****************************��ʱ����ʱ�ж����ȼ�*************************/
#define TIM5_IRQPreemptionPrio   2                                        //TIM5����ռʽ���ȼ�      
#define TIM5_IRQSubPrio          2                                         //TIM5�Ĵ����ȼ�

/*************************TIME5 �������û�����**************************/
extern void TIM5_Encoder_Init(void);                                       //TIME5����Ϊ������ģʽ                                      
extern void TIM5_Encoder_SetData(int32_t EncoderValue);                    //���ñ��������� ��-2^31-1  ~  2^31 - 1��
extern int32_t TIM5_Encoder_GetData(void);                                 //��ȡ���������� ��-2^31-1  ~  2^31 - 1��



#endif /* __PWM_OUTPUT_H */


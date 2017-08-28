#ifndef __TIME6_H_
#define	__TIME6_H_

#include "includes.h"

/****************************��ʱ����ʱ�ж����ȼ�*************************/
#define TIM6_TimeTick            5                                       //TIM6��ʱʱ�� ,���ڶ�ʱ�Ļ�׼

/****************************��ʱ����ʱ�ж����ȼ�*************************/
#define TIM6_IRQPreemptionPrio   1                                        //TIM6����ռʽ���ȼ�      
#define TIM6_IRQSubPrio          3                                        //TIM6�Ĵ����ȼ�


/*****************************��ʱ���û�����******************************/
extern void TIM6_Init(void);                                              //TIME6��ʼ�� ,��λ �� ms ����С1ms �����1200ms
extern FlagStatus TIM6_50MsFinish(void);                                  //��ʱ50ms �� ��ʱʱ�䵽 ����SET ,���򷵻�RESET
extern FlagStatus TIM6_100MsFinish(void);                                 //��ʱ100ms , ��ʱʱ�䵽 ����SET ,���򷵻�RESET


#endif 




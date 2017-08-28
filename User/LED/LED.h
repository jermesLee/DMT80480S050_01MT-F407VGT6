#ifndef __LED_H_
#define	__LED_H_

#include "includes.h"

/* �������IO�ĺ� */
#define LED1_TOGGLE()		GPIO_ToggleBits(GPIOE,GPIO_Pin_0)                 // ȡ��
#define LED1_OFF()			GPIO_SetBits(GPIOE,GPIO_Pin_0)                    // ��λ
#define LED1_ON()		   	GPIO_ResetBits(GPIOE,GPIO_Pin_0)                  // ����

#define LED2_TOGGLE()		GPIO_ToggleBits(GPIOE,GPIO_Pin_1)                 // ȡ��
#define LED2_OFF()			GPIO_SetBits(GPIOE,GPIO_Pin_1)                    // ��λ
#define LED2_ON()		   	GPIO_ResetBits(GPIOE,GPIO_Pin_1)                  // ����

#define LED3_TOGGLE()		GPIO_ToggleBits(GPIOE,GPIO_Pin_2)                 // ȡ��
#define LED3_OFF()			GPIO_SetBits(GPIOE,GPIO_Pin_2)                    // ��λ
#define LED3_ON()		   	GPIO_ResetBits(GPIOE,GPIO_Pin_2)                  // ����

extern void LED_Init(void);


#endif /* LED.H*/



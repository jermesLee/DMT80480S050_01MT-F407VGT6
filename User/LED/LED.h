#ifndef __LED_H_
#define	__LED_H_

#include "includes.h"

/* 定义控制IO的宏 */
#define LED1_TOGGLE()		GPIO_ToggleBits(GPIOE,GPIO_Pin_0)                 // 取反
#define LED1_OFF()			GPIO_SetBits(GPIOE,GPIO_Pin_0)                    // 置位
#define LED1_ON()		   	GPIO_ResetBits(GPIOE,GPIO_Pin_0)                  // 清零

#define LED2_TOGGLE()		GPIO_ToggleBits(GPIOE,GPIO_Pin_1)                 // 取反
#define LED2_OFF()			GPIO_SetBits(GPIOE,GPIO_Pin_1)                    // 置位
#define LED2_ON()		   	GPIO_ResetBits(GPIOE,GPIO_Pin_1)                  // 清零

#define LED3_TOGGLE()		GPIO_ToggleBits(GPIOE,GPIO_Pin_2)                 // 取反
#define LED3_OFF()			GPIO_SetBits(GPIOE,GPIO_Pin_2)                    // 置位
#define LED3_ON()		   	GPIO_ResetBits(GPIOE,GPIO_Pin_2)                  // 清零

extern void LED_Init(void);


#endif /* LED.H*/



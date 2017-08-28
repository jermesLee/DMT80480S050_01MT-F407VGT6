#ifndef _USART3_H_
#define	_USART3_H_

#include "includes.h"

/************************USART3  内存分配大小宏定义**********************/
#define USART3_SendSizeMax                 256                            //串口发送一次发送的最大字节数(DMA方式)
#define USART3_ReceiveSizeMax              256                            //串口接收一次性接收的最大字节数（DMA方式）
#define USART3_DMASendOneSizeMax            32                            //DMA一次性允许发送的最大字节数 ，正在发送的一帧数据在内存里是受保护的，不允许写操作 ， 而其中已经发送了的数据内存无法得到释放 ，
                                                                          //为解决此问题，限制一次发送的最大字节数。当一帧数据比较长时，影响特别大，所有限制一下一次DMA能够传输最大的一次数据，，便于通信时内存的释放
/************************USART3  DMA发送接收中断优先级*******************/
#define USART3_IRQPreemptionPrio            2                             //USART3抢占式优先级 ( 用于接收 ) ,建议抢占式优先级比DMA1_Channel5_IRQPreemptionPrio低 
#define USART3_IRQSubPrio                   2                             //USART3次优先级

#define DMA1_Stream4_IRQPreemptionPrio      1                             //USART3 DMA1_Stream4 传输抢占式优先级 （ 用于发送 ）
#define DMA1_Stream4_IRQSubPrio             2                             //USART3 DMA1_Stream4 传输次优先级                   

#define DMA1_Stream1_IRQPreemptionPrio      1                             //USART3 DMA1_Stream1 传输抢占式优先级 （ 用于接收 ），建议抢占式优先级比USART3_IRQPreemptionPrio 高
#define DMA1_Stream1_IRQSubPrio             1                             //USART3 DMA1_Stream1 传输次优先级                   




/**************************USART3  用户函数*******************************/
extern void USART3_Init(uint32_t Baudrate);                               //USART3配置 ， Baudrate ：波特率
extern ErrorStatus USART3_WriteDMASendMulData(uint8_t *Data   , uint16_t  Num);  //串口发送数据  ，Num ：需要发送的数据量 
extern ErrorStatus USART3_ReadDMAReceiveMulData(uint8_t *Data , uint16_t  Num);  //串口读取数据  ，Num ：返回接收到的数据量

extern void USART3_Debug(void) ;


#endif /* __USART3_H */

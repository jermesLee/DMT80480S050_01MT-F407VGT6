#ifndef _DWIN_LCD_COMMUNICATION_H_
#define	_DWIN_LCD_COMMUNICATION_H_

#include "includes.h"
#include "DWIN_LCD.h"
#include "TIME3_Encoder.h" 
#include "TIME5_Encoder.h" 


/************************DWIN_LCD��ʾ��ַ�궨��**************************/ 
#define DWIN_LCD_InputLineVoltageAddress         (uint16_t)0X000E         //���������ѹ��ַ
#define DWIN_LCD_DisplaySetLineVoltageAddress    (uint16_t)0X000A         //ʵ����ʾ�����ߵ�ѹ�ĵ�ַ
 
#define DWIN_LCD_InputFrequencyAddress           (uint16_t)0X000C         //��������Ƶ�ʵĵ�ַ
#define DWIN_LCD_DisplaySetFrequencyAddress      (uint16_t)0X0008         //ʵ����ʾ����Ƶ�ʵĵ�ַ


/*********************�����ߵ�ѹ Ƶ�� ��ֵ�궨��*************************/ 
#define DWIN_LCD_DisplaySetLineVoltageMin        (uint16_t)1             //ʵ����ʾ���õ��ߵ�ѹ��Сֵ
#define DWIN_LCD_DisplaySetLineVoltageMax        (uint16_t)2000          //ʵ����ʾ���õ��ߵ�ѹ���ֵ ����λ ��0.01 LSB (V)

#define DWIN_LCD_DisplaySetFrequencyMin          (uint16_t)1             //ʵ����ʾ���õ��ź�Ƶ�ʵ���Сֵ
#define DWIN_LCD_DisplaySetFrequencyMax          (uint16_t)2000          //ʵ����ʾ���õ��ź�Ƶ�ʵ����ֵ ����λ ��1 LSB(Hz)


/************************** DWIN_LCD���ܺ�������*************************/ 
extern void DWIN_LCD_DisplayInit(void);                                   //�ϵ��ʼ��DWIN_LCD��ʾ����
extern void DWIN_LCD_AndMCUCommunication(void);                           //��Ҫ�ӻ�����д��	DWIN_LCD������ �� �� �����յ��ĵ��������ݴ���д��DWIN_LCD������ 
extern uint16_t DWIN_LCD_DisplaySetLineVoltage(void);                     //����DWIN_LCD����ʾ�����ߵ�ѹ ���ͽ����õ��ߵ�ѹд��DWIN_LCD������
extern uint16_t DWIN_LCD_DisplaySetFrequency(void);                       //����DWIN_LCD����ʾ�����ź�Ƶ�ʣ��ͽ����õ�Ƶ��д��DWIN_LCD������


#endif /* __DWIN_LCD_H */



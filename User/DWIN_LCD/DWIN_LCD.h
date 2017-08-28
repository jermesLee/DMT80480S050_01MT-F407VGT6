#ifndef _DWIN_LCD_H_
#define	_DWIN_LCD_H_

#include "includes.h"                                                                              // ���е�ͷ�ļ�
#include "USART3.h"                                                                                // ���� ��������DWIN_LCD��������ͨ��
#include "CRC16.h"                                                                                 // CRC16У�� ��ȷ��ͨ����������


/******************************DWIN_LCD������ ָ�ö�ٶ���*************************************/
typedef enum DWIN_LCD_CommandType 
{
	DWIN_LCD_WriteControlRegiter = 0X80 ,                                                            //д���ƼĴ���ָ��
	DWIN_LCD_ReadControlRegiter  = 0X81 ,                                                            //�����ƼĴ���ָ��
	DWIN_LCD_WriteDataMemory     = 0X82 ,                                                            //д�����ڴ�ָ��
	DWIN_LCD_ReadDataMemory      = 0X83                                                              //�������ڴ�ָ��
}DWIN_LCD_CommandType ;                                                                            //DWIN_LCD������ ָ�����


/**************************DWIN_LCD������ͨ���ڴ�����С�궨�� *********************************/
#define DWIN_LCD_FrameSizeMax                         128                                          // ��DWIN_LCD ͨ�����������һ֡���ݵĴ�С ����λ���ֽ� ������ʹ��Ĭ��ֵ
                                                                                                   // ��û��ʹ������궨��ʱ ����ʹ��Ĭ��ֵ ������Ĭ��ֵΪ  DWIN_LCD_NumSize * 2 + 10 
#define DWIN_LCD_FrameHeadH                          0X5A                                          // ͨ��֡ͷ���ֽ�             
#define DWIN_LCD_FrameHeadL                          0XA5                                          // ͨ��֡ͷ���ֽ�
#define DWIN_LCD_StartAddr                         0X0000                                          // DWIN_LCD����������ʹ�õĵ�ַ�׵�ַ
#define DWIN_LCD_NumSize                               32                                          // DWIN_LCD����������ʹ�õĵ�ַ����


/**************************DWIN_LCD������ͨ���õ����ⲿ����***************************************/
#define DWIN_LCD_Init(Baudrate)                          USART3_Init(Baudrate)                     // DWIN_LCD��������ʼ�� ��ʵ�ʾ��Ǵ��ڳ�ʼ��
#define DWIN_LCD_WriteSend(Data   ,Num)                  USART3_WriteDMASendMulData(Data   ,Num)   // ���������ڵ�����  ��dataΪ��Ҫ���͵����ݣ�numΪ��Ҫ���͵��ֽ���
#define DWIN_LCD_ReadReceive(Data ,Num)                  USART3_ReadDMAReceiveMulData(Data ,Num)   // ��ȡ���յ��ĵ�����������data�� �����յ����ֽ���������Num
#define DWIN_LCD_CRC16_CheckCompute(DatHead,DatLen,ReturnDat)  CRC16_CheckCompute(DatHead,DatLen,ReturnDat) // crc16����,������������ReturnDat��
#define DWIN_LCD_CRC16_Check(DataHeader1,DataHeader2)    CRC16_Check(DataHeader1,DataHeader2)      // �Ƚ�����crcֵ�Ƿ���� ������򷵻�SUCCESS ,����Ϊ ERROR 


/******************************DWIN_LCD�������û�����*********************************************/ 
// DWIN_LCD_Init(Baudrate)                                                                         //�궨���ʾ ��ʵ�ʵ��õ��Ǵ��ڳ�ʼ������
extern void DWIN_LCD_ReceiveHandleFunction(void);                                                  //mcu���յ�����������Ӧ���� �����ݽ������ݵ��������Ӧ������д����Ӧλ��
extern void DWIN_LCD_SendHandleFunction(DWIN_LCD_CommandType FunSel ,uint16_t StartAddr , uint16_t Num);    //�������ݸ������� �����������ݣ����ȡ������
extern ErrorStatus DWIN_LCD_AddrMapDataRead(uint16_t *Data , uint16_t Num , uint16_t StartAddr );  //��ȡ�ڴ�ά���������ݱ��浽�����У�����ʵ��Ӧ��       
extern ErrorStatus DWIN_LCD_AddrMapDataWrite(uint16_t *Data , uint16_t Num , uint16_t StartAddr);  //����Ӧ����д�� mcu ά�����ڴ��� �������ڴ�ά��������     


#endif /* __DWIN_LCD_H */



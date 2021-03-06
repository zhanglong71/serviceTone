#include  "stdio.h"
#include  "string.h"
#include  "intrins.h"
#include "reg51.h"
#include "SJA1000.h"

#ifdef  MAIN_GLOBALS
#define MAIN_EXT
#else
#define MAIN_EXT  extern
#endif
//************************** 8051中断源定义 ***********************
#define		INT_EX0		0		//外部中断0源,P3.2
#define		INT_EX1		2		//外部中断1源,P3.3
#define		INT_T0		1		//定时器0中断源,Timer0
#define		INT_T1		3		//定时器1中断源,Timer1
#define		INT_SER		4		//串口中断源,P3.0,P3.1
//****************************************************************
sfr WDT_CONTR = 0xe1; //Watch-Dog-Timer Control register
MAIN_EXT unsigned char xdata *SJA_BCANAdr;
MAIN_EXT unsigned char key_temp;
//MAIN_EXT bit flag_ComRxFinish;
MAIN_EXT bit flag_CanRxFinish;
//MAIN_EXT bit flag_ComRxFirst;
//MAIN_EXT bit flag_ComTxAddr;
//MAIN_EXT unsigned char ComRxLength;
//MAIN_EXT unsigned char CntComRxData;
//MAIN_EXT unsigned char CntComRxTimout;
//MAIN_EXT bit flag_CntComRxTimeout;





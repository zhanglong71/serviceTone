#define KEY_GLOBALS
#include <includes.h>

void KeyCLr(void)
{
	INT8U i;
	KeyWrPoint=0;
	KeyRdPoint=0;
	for(i=0;i<10;i++)KeyBuf[i]=0x00;
}


INT8U KeyRd(void)                     //键盘译码程序
{
	INT8U KeyTemp;
	if(KeyWrPoint!=KeyRdPoint)          //缓冲环中有按键
    {
    	switch(KeyBuf[KeyRdPoint])
    	{
    		
    		case KEY0:
    		     KeyTemp=0x01;
    		     break;
    		case KEY1:
    		     KeyTemp=0x02;
    		     break;
    		case KEY2:
    		     KeyTemp=0x03;
    		     break;
    		case KEY3:
    		     KeyTemp=0x04;
    		     break;
    		case KEY4:
    		     KeyTemp=0x05;
    		     break;
    		case KEY5:
    		     KeyTemp=0x06;
    		     break;
    		case KEY6:
    		     KeyTemp=0x07;
    		     break;
    		case KEY7:
    		     KeyTemp=0x08;
    		     break;
    		case KEY8:
    		     KeyTemp=0x09;
    		     break;
    		case KEY9:
    		     KeyTemp=0x0a;
    		     break;
    		case KEYEX:
    		     KeyTemp=0x00;
    		     break;
    		case KEYEJ:
    		     KeyTemp=0x0b;
    		     break;
    		case KEYF1:
    		     KeyTemp=0x0c;
    		     break;
    		case KEYF2:
    		     KeyTemp=0x0d;
    		     break;
    		case KEYF3:
    		     KeyTemp=0x0e;
    		     break;
    		case KEYF4:
    		     KeyTemp=0x0f;
    		     break;
    		default : 
    			   KeyTemp=0xff;
    			   break;
    	}
    	KeyRdPoint++;
    	KeyRdPoint%=10;
    }
    else KeyTemp=0xff;
    return(KeyTemp);
}

/*******************************************************************************/
//蜂鸣器控制程序
/**********************************************************************************************/
//void BuzzerOn()
//{
//  BUZ=BZ_ON;
//}
/*******************************************************************/
//void BuzzerOff()
//{
//  BUZ=BZ_OFF;
//}
/*******************************************************************/
//void Buzzer_delay_time(unsigned int  Delaytime)
//{
// int i,j;
// for (i=0;i<Delaytime;i++)
// {
// 	for (j=0;j<1500;j++)
// 	{
// 		WDT_CONTR=0x3d;  //喂狗 1.13s;
// 	}
// }
//}
/*******************************************************************/
//void BuzzerCTL(unsigned char Bell_Number,unsigned int DelayTime)
//{
// int i;
// for (i=0;i<Bell_Number;i++)
// {
//     BuzzerOn();
//     Buzzer_delay_time(DelayTime);
//     BuzzerOff();
//     Buzzer_delay_time(DelayTime);
// }
//}

//unsigned char ArrCompare(unsigned char *p1,unsigned char *p2,unsigned char Len)                //比较两数组是否相等
//{
//	int i;
//	unsigned char fEQU=1;
//	unsigned char a1,a2;
//	for(i=0;i<Len;i++)
//	{
//		a1=*(p1+i);
//		a2=*(p2+i);
//		if(a1!=a2)fEQU=0;
//	}
//	return(fEQU);
//}
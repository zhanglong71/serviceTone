#ifdef  KEY_GLOBALS
#define KEY_EXT
#else
#define KEY_EXT  extern
#endif


//**************************定义按键端口***************************
sbit k1=P0^0;
sbit k2=P0^1;
sbit k3=P0^2;
sbit k4=P0^3;
sbit k5=P0^4;
sbit k6=P0^5;
sbit k7=P0^6;
sbit k8=P0^7;
#define KEYPORT P0



//***************************定义按键键值**************************

#define KEY0  0x88                 //数字0
#define KEY1  0x48                 //数字1
#define KEY2  0x28                 //数字2
#define KEY3  0x18                 //数字3
#define KEY4  0x84                 //数字4
#define KEY5  0x44                 //数字5
#define KEY6  0x24                 //数字6
#define KEY7  0x14                 //数字7
#define KEY8  0x82                 //数字8
#define KEY9  0x42                 //数字9
#define KEYEX 0x22                 //功能键*
#define KEYEJ 0x12                 //功能键#
#define KEYF1 0x81                 //功能键F1
#define KEYF2 0x41                 //功能键F2
#define KEYF3 0x21                 //功能键F3
#define KEYF4 0x11                 //功能键F4

#define FUN1  0x0c
#define FUN2  0x0d
#define FUN3  0x0e
#define FUN4  0x0f
#define FUN5  0x04
#define FUN6  0x06
#define FUN7  0x05
#define FUN8  0x00
#define FUN9	0x07
#define FUN10	0x08
#define FUN11	0x01
#define FUN12 0x02
#define FUN13 0x03
#define FUN14 0x09

#define STAT1		1+48
#define STAT2		2+48
#define STAT3		3+48
#define STAT4		4+48
#define STAT5		5+48
#define STAT6		6+48
#define STAT7		7+48
#define STAT8		8+48
#define STAT9		9+48
#define STAT0		0+48

//*****************************************************************


KEY_EXT unsigned char KeyPoint;           //键盘防抖动指针
KEY_EXT unsigned char KeyCir[5];          //键盘防抖动缓冲环
KEY_EXT unsigned char KeyBuf[10];         //键值存贮缓冲环
KEY_EXT unsigned char KeyWrPoint;         //键盘写指针
KEY_EXT unsigned char KeyRdPoint;         //键盘读指针
KEY_EXT unsigned char KeyRptBuf;          //长按缓存

KEY_EXT unsigned char KeyRd(void);        //键盘译码程序
KEY_EXT void KeyCLr(void);
//KEY_EXT void BuzzerOn();
//KEY_EXT void BuzzerOff();
//KEY_EXT void Buzzer_delay_time(unsigned int  Delaytime);
//KEY_EXT void BuzzerCTL(unsigned char Bell_Number,unsigned int DelayTime);
//KEY_EXT unsigned char ArrCompare(unsigned char *p1,unsigned char *p2,unsigned char Len);


//KEY_EXT INT8U ArrCompare(INT8U *p1,INT8U *p2,INT8U Len);                //比较两数组是否相等
//KEY_EXT  void ArrCopy(INT8U *p1,INT8U *p2,INT8U Len);                   //将P1复制到P2,<P2=P1>
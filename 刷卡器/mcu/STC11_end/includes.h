#include <stdio.h>
#include <STC11F60XE.H>
#include <intrins.h>
#include <OS.h>
#include <stdlib.h>
//#include <menu.h>
#include <key.h>
#include <IAP.h>
#include <wifii.h>
#include <Lcd.h>
#include <rfid.H>
#include <AP170.H>
#include <string.h>
#include <math.h>
#define INT8U unsigned char        //无符号8位数
#define INT8S signed char          //有符号8位数
#define INT16U unsigned int        //无符号16位数
#define INT16S signed int          //有符号16位数
#define INT32U unsigned long       //无符号32位数
#define INT32S signed long         //有符号32位数
#define FP32 float                 //单精度浮点数
#define FP64 double                //双精度浮点数

#define QQR 58
#define QXZ 59
#define	QSK 60
#define QAJ 61

//************************** 8051中断源定义 ***********************
#define		INT_EX0		0		//外部中断0源,P3.2
#define		INT_EX1		2		//外部中断1源,P3.3
#define		INT_T0		1		//定时器0中断源,Timer0
#define		INT_T1		3		//定时器1中断源,Timer1
#define		INT_SER		4		//串口中断源,P3.0,P3.1
//****************************************************************
//***********************定义LCD控制与I/O复用控制端口***************

//sfr WDT_CONTR   = 0xe1;    //看门狗

#define x1    0x80
#define x2    0x88
#define y     0x80
#define comm  0
#define dat   1
#define DISP_BUF_MAX		40		//显示缓存数量
#define ADDR_PROSEL 0x0009    //程序选择存储地址
#define ADDR_CHNSEL 0x000a    //频道选择存储地址
#define ADDR_ADDRESSL 0x000b   //房间号储地址
#define ADDR_ADDRESSH 0x000c   //房间号储地址



sbit CS  = P4^4;   //H=data; L=command;
sbit SID = P4^5;   //H=read; L=write;
sbit SCK = P4^1;   //input enable;
sbit LCD_PWR=P4^0;
sbit KeyLed=P4^6;

sbit CS_AP170=P2^0;					//芯片片选
sbit SCK_AP170=P2^1;				//串行时钟
sbit DI_AP170=P2^2;					//串行数据
sbit BUSY_AP170=P2^3;				//忙信号
#if	1
/** cooked code **/
	sbit PinRff=P3^3;
	sbit PinRfs=P3^2;
#else	
/** raw code **/
	sbit PinRfs=P3^3;
	sbit PinRff=P3^2;
#endif
sbit PinRfa1=P3^4;
sbit PinRfa2=P3^5;
sbit PinSet=P4^2;
sbit PinSet_ZigBee=P3^7;
//sbit PinSet_ZigBee=P4^2;
//sbit PinSet=P3^7;
sbit busy=P2^7;   //lcd busy bit
//sbit PinMpwr=P4^3;
//sbit testPin=P4^2;
#define LED_ON 1
#define LED_OFF 0
#define ALARM_ON  1
#define ALARM_OFF 0
#define LCD_ON  1
#define LCD_OFF 0

//***************************************************************
#ifdef  MAIN_GLOBALS
#define MAIN_EXT
#else
#define MAIN_EXT  extern
#endif

#define CNT_LCD_ON 40    //30S关闭LCD背光
#define LCD_LEFT_CNT 160	//500ms左移一次

MAIN_EXT INT8U fProSelect;  //桑拿与沐足程序选择
MAIN_EXT INT8U CardId[4];
MAIN_EXT INT8U idbuf[10];

MAIN_EXT INT8U fSoundOn;
MAIN_EXT fCancel;
MAIN_EXT fOvTime;



MAIN_EXT INT16U  CntLcdOn;        //关闭LCD背光计数器
MAIN_EXT INT8U   Cnt5ms;
MAIN_EXT INT8U   fLcdPwr;
MAIN_EXT INT8U  CntOv1s;
MAIN_EXT INT8U  LcdLeftCnt;        //显示左移计时器
MAIN_EXT INT8U  LCD_disp_starting;	//显示起点
MAIN_EXT INT8U  LcddispCnt;					//LCD显示计数
MAIN_EXT INT8U  disp_buf[DISP_BUF_MAX];				//显示缓存
MAIN_EXT INT8U  LCD_disp_img_flag;
MAIN_EXT INT8U  Notice_opt;					//通知选项
MAIN_EXT INT8U	Notice_operat_flag;			//标志当前为通知操作
MAIN_EXT INT8U	Notice_again_ENflag;			//在通知使能标志

#define RX_OVERTIME 4    //接收数据等待时间
#define ID_OVERTIME 15   //刷卡等待时间
#define KEY_OVERTIME 60   //等待按键时间
#define CNT_RETRY   3    //接收超时重试次数

//#define CNT_WORKUSE 6    //上钟技师缓存数量
//MAIN_EXT INT8U Cnt1s;
//MAIN_EXT INT16U Cnt1min[CNT_WORKUSE];
//MAIN_EXT INT8U TRDataBuf[100];
//MAIN_EXT INT8U TRDataLen;
//MAIN_EXT INT8U TRDataBuf_buf[100];
//MAIN_EXT INT8U TXDataLen_buf;		//通知发送数据缓冲
MAIN_EXT INT8U IndexSend;       //发送数据流水号
MAIN_EXT INT8U RoomId[7];       //房间号

MAIN_EXT INT8U RoomLen;
MAIN_EXT INT8U NewRoomId[7];       //房间号
MAIN_EXT INT8U NewRoomLen;
MAIN_EXT INT8U CostId[7];          //消费代码
MAIN_EXT INT8U CostLen;
MAIN_EXT INT8U WorkId[7];          //技师工号
MAIN_EXT INT8U CntWorkId;          //上钟技师工号存取指针
MAIN_EXT INT8U WorkLen;
MAIN_EXT INT8U CustId[7];          //锁牌号
MAIN_EXT INT8U CustLen;
MAIN_EXT INT8U Xiaofei[7];					//小费
MAIN_EXT INT8U Xiaofeilen;
MAIN_EXT INT8U Num[3];							//需要技师的个数
MAIN_EXT INT8U Numlen;
MAIN_EXT INT8U init_comp_flag;
MAIN_EXT INT8U male_female;						//男女技师性别
MAIN_EXT INT8U Type_clock;						//技师上钟类型
MAIN_EXT INT8U check_flag;						//能否结账标志
MAIN_EXT INT8U Service_type;					//服务类型
MAIN_EXT INT8U Inte[7];								//积分
MAIN_EXT INT8U Intelen;
MAIN_EXT INT8U Resi[7];								//余额
MAIN_EXT INT8U Resilen;
MAIN_EXT INT8U Cost[7];								//消费额
MAIN_EXT INT8U Cost_len;
MAIN_EXT INT8U Query;									//查询的内容
MAIN_EXT INT8U QueryID[3][7];					//查询队列返回的3个头牌的工号
MAIN_EXT INT8U QueryIDlen[3];
MAIN_EXT INT8U Stat;									//当前查询技师，所返回的状态
MAIN_EXT INT8U Time[4];								//技师被预约的时间
MAIN_EXT INT8U Locat[7];							//当前查询技师的位置
MAIN_EXT INT8U Locatlen;

MAIN_EXT INT8U AddTime[2];
MAIN_EXT INT8U CostNum[2];

MAIN_EXT INT8U player_Cut_stop;				//计数停止
MAIN_EXT INT8U player_Cut;					//播放完计数
MAIN_EXT INT8U player_stop;					//当前播放被停止
MAIN_EXT INT8U player_wait_time;			//等待播放时间
MAIN_EXT INT8U player_retry_cnt;			//播放未成功，重试次数

MAIN_EXT INT8U comRxConfirmRdy;
MAIN_EXT INT8U comRxDataRdy;
MAIN_EXT INT8U MyChn;
MAIN_EXT INT8U MyAddres[2];
//MAIN_EXT INT8U txConfirmData[80];


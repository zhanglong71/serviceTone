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
#define INT8U unsigned char        //�޷���8λ��
#define INT8S signed char          //�з���8λ��
#define INT16U unsigned int        //�޷���16λ��
#define INT16S signed int          //�з���16λ��
#define INT32U unsigned long       //�޷���32λ��
#define INT32S signed long         //�з���32λ��
#define FP32 float                 //�����ȸ�����
#define FP64 double                //˫���ȸ�����

#define QQR 58
#define QXZ 59
#define	QSK 60
#define QAJ 61

//************************** 8051�ж�Դ���� ***********************
#define		INT_EX0		0		//�ⲿ�ж�0Դ,P3.2
#define		INT_EX1		2		//�ⲿ�ж�1Դ,P3.3
#define		INT_T0		1		//��ʱ��0�ж�Դ,Timer0
#define		INT_T1		3		//��ʱ��1�ж�Դ,Timer1
#define		INT_SER		4		//�����ж�Դ,P3.0,P3.1
//****************************************************************
//***********************����LCD������I/O���ÿ��ƶ˿�***************

//sfr WDT_CONTR   = 0xe1;    //���Ź�

#define x1    0x80
#define x2    0x88
#define y     0x80
#define comm  0
#define dat   1
#define DISP_BUF_MAX		40		//��ʾ��������
#define ADDR_PROSEL 0x0009    //����ѡ��洢��ַ
#define ADDR_CHNSEL 0x000a    //Ƶ��ѡ��洢��ַ
#define ADDR_ADDRESSL 0x000b   //����Ŵ���ַ
#define ADDR_ADDRESSH 0x000c   //����Ŵ���ַ



sbit CS  = P4^4;   //H=data; L=command;
sbit SID = P4^5;   //H=read; L=write;
sbit SCK = P4^1;   //input enable;
sbit LCD_PWR=P4^0;
sbit KeyLed=P4^6;

sbit CS_AP170=P2^0;					//оƬƬѡ
sbit SCK_AP170=P2^1;				//����ʱ��
sbit DI_AP170=P2^2;					//��������
sbit BUSY_AP170=P2^3;				//æ�ź�
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

#define CNT_LCD_ON 40    //30S�ر�LCD����
#define LCD_LEFT_CNT 160	//500ms����һ��

MAIN_EXT INT8U fProSelect;  //ɣ�����������ѡ��
MAIN_EXT INT8U CardId[4];
MAIN_EXT INT8U idbuf[10];

MAIN_EXT INT8U fSoundOn;
MAIN_EXT fCancel;
MAIN_EXT fOvTime;



MAIN_EXT INT16U  CntLcdOn;        //�ر�LCD���������
MAIN_EXT INT8U   Cnt5ms;
MAIN_EXT INT8U   fLcdPwr;
MAIN_EXT INT8U  CntOv1s;
MAIN_EXT INT8U  LcdLeftCnt;        //��ʾ���Ƽ�ʱ��
MAIN_EXT INT8U  LCD_disp_starting;	//��ʾ���
MAIN_EXT INT8U  LcddispCnt;					//LCD��ʾ����
MAIN_EXT INT8U  disp_buf[DISP_BUF_MAX];				//��ʾ����
MAIN_EXT INT8U  LCD_disp_img_flag;
MAIN_EXT INT8U  Notice_opt;					//֪ͨѡ��
MAIN_EXT INT8U	Notice_operat_flag;			//��־��ǰΪ֪ͨ����
MAIN_EXT INT8U	Notice_again_ENflag;			//��֪ͨʹ�ܱ�־

#define RX_OVERTIME 4    //�������ݵȴ�ʱ��
#define ID_OVERTIME 15   //ˢ���ȴ�ʱ��
#define KEY_OVERTIME 60   //�ȴ�����ʱ��
#define CNT_RETRY   3    //���ճ�ʱ���Դ���

//#define CNT_WORKUSE 6    //���Ӽ�ʦ��������
//MAIN_EXT INT8U Cnt1s;
//MAIN_EXT INT16U Cnt1min[CNT_WORKUSE];
//MAIN_EXT INT8U TRDataBuf[100];
//MAIN_EXT INT8U TRDataLen;
//MAIN_EXT INT8U TRDataBuf_buf[100];
//MAIN_EXT INT8U TXDataLen_buf;		//֪ͨ�������ݻ���
MAIN_EXT INT8U IndexSend;       //����������ˮ��
MAIN_EXT INT8U RoomId[7];       //�����

MAIN_EXT INT8U RoomLen;
MAIN_EXT INT8U NewRoomId[7];       //�����
MAIN_EXT INT8U NewRoomLen;
MAIN_EXT INT8U CostId[7];          //���Ѵ���
MAIN_EXT INT8U CostLen;
MAIN_EXT INT8U WorkId[7];          //��ʦ����
MAIN_EXT INT8U CntWorkId;          //���Ӽ�ʦ���Ŵ�ȡָ��
MAIN_EXT INT8U WorkLen;
MAIN_EXT INT8U CustId[7];          //���ƺ�
MAIN_EXT INT8U CustLen;
MAIN_EXT INT8U Xiaofei[7];					//С��
MAIN_EXT INT8U Xiaofeilen;
MAIN_EXT INT8U Num[3];							//��Ҫ��ʦ�ĸ���
MAIN_EXT INT8U Numlen;
MAIN_EXT INT8U init_comp_flag;
MAIN_EXT INT8U male_female;						//��Ů��ʦ�Ա�
MAIN_EXT INT8U Type_clock;						//��ʦ��������
MAIN_EXT INT8U check_flag;						//�ܷ���˱�־
MAIN_EXT INT8U Service_type;					//��������
MAIN_EXT INT8U Inte[7];								//����
MAIN_EXT INT8U Intelen;
MAIN_EXT INT8U Resi[7];								//���
MAIN_EXT INT8U Resilen;
MAIN_EXT INT8U Cost[7];								//���Ѷ�
MAIN_EXT INT8U Cost_len;
MAIN_EXT INT8U Query;									//��ѯ������
MAIN_EXT INT8U QueryID[3][7];					//��ѯ���з��ص�3��ͷ�ƵĹ���
MAIN_EXT INT8U QueryIDlen[3];
MAIN_EXT INT8U Stat;									//��ǰ��ѯ��ʦ�������ص�״̬
MAIN_EXT INT8U Time[4];								//��ʦ��ԤԼ��ʱ��
MAIN_EXT INT8U Locat[7];							//��ǰ��ѯ��ʦ��λ��
MAIN_EXT INT8U Locatlen;

MAIN_EXT INT8U AddTime[2];
MAIN_EXT INT8U CostNum[2];

MAIN_EXT INT8U player_Cut_stop;				//����ֹͣ
MAIN_EXT INT8U player_Cut;					//���������
MAIN_EXT INT8U player_stop;					//��ǰ���ű�ֹͣ
MAIN_EXT INT8U player_wait_time;			//�ȴ�����ʱ��
MAIN_EXT INT8U player_retry_cnt;			//����δ�ɹ������Դ���

MAIN_EXT INT8U comRxConfirmRdy;
MAIN_EXT INT8U comRxDataRdy;
MAIN_EXT INT8U MyChn;
MAIN_EXT INT8U MyAddres[2];
//MAIN_EXT INT8U txConfirmData[80];


//--------------------------------------------------
//PeliCAN模式，标准帧
//RS232波特率：57600
//CAN波特率：125kbps
//设置ID在18行
//--------------------------------------------------
#define MAIN_GLOBALS
#include <includes.h>
#define MAX_BUF_NUM     256
#define COM_RX_NONE     0x00 //空数据
#define COM_RX_DATA     0x01 //接收到数据
#define COM_RX_DCONFIRM 0x02 //接收到数据确认
#define COM_RX_ASKCHN   0x03 //接收到数据确认

#define	CTIMER_MS	(10)

#define CSEND_BUF_NUM	8	//16	 //缓冲区个数
#define CSEND_BUF_SIZE	128	//64	 //缓冲区大小
//#define INBUF_LEN 10   //数据长度
//sfr WDT_CONTR = 0xC1; //Watch-Dog-Timer Control register
unsigned char comSendDataBuf[MAX_BUF_NUM];
unsigned char Can2ComBuf[CSEND_BUF_NUM][CSEND_BUF_SIZE];		//暂存来自canbus转发给com串口的数据(CANbus ==> com)
unsigned char Com2CanBuf[CSEND_BUF_NUM][CSEND_BUF_SIZE];		//暂存来自com串口转发给CAN的数据(com ==> CANbus)

#define	CWAITACK_ID	1
#define	CLENGTH_ID	3
#define	CSTART_ID	(CLENGTH_ID + 1)
//unsigned char cntComSendIndex;
unsigned int comSendLength;
unsigned char comRxDataBuf[MAX_BUF_NUM];
unsigned int comRxLength;
unsigned int cntComRxIndex;
unsigned int comSnd_timer;
unsigned int canSnd_timer;
unsigned char cntComRxTimeout;
unsigned char flag_com_rx_finish;
unsigned char staComRx;
unsigned char flag_canRxRoll;

unsigned char data Tmod_data;
unsigned char run_lamp_flush_count = 0;
unsigned char run_lamp_flush_time = 10;
sbit  run_lamp = P1^0;
unsigned char CanRevceData[MAX_BUF_NUM],RxData_Buf[10];
//unsigned int data rxbuffer[10]={REG_RXBuffer1,REG_RXBuffer2,REG_RXBuffer3,REG_RXBuffer4,REG_RXBuffer5,REG_RXBuffer6,REG_RXBuffer7,REG_RXBuffer8,REG_RXBuffer9,REG_RXBuffer10};
//unsigned int data txbuffer[10]={REG_TXBuffer1,REG_TXBuffer2,REG_TXBuffer3,REG_TXBuffer4,REG_TXBuffer5,REG_TXBuffer6,REG_TXBuffer7,REG_TXBuffer8,REG_TXBuffer9,REG_TXBuffer10};
//unsigned char data Send_CAN_Info_ID[5]={0x08,0x55,0xe0,0x55,0x55};
//unsigned char data Send_CAN_Info_ID[5]={0x01,0x55,0xe0,0x55,0x55};
//unsigned char data Com_RecBuff[8]={0x01,0x02,0x03,0x4,0x05,0x06,0x07,0x08};
unsigned char data Com_RecBuff[8];
unsigned char bdata flag_init;
//*************************
#define INBUF_LEN 8   //数据长度

unsigned char inbuf1[80];
unsigned char checksum,count3=0;
bit  overtimer_flag=0;read_flag=0;
//***************************
sbit rcv_flag=flag_init^0;
sbit err_flag=flag_init^0;
sbit CS=P2^7;
sbit RSTCAN=P1^6;
sbit LED1=P3^5;
sbit LED2=P3^4;
sbit Pin_test=P3^3;
sbit k1=P1^1;
sbit k2=P1^2;
sbit k3=P1^3;
sbit k4=P1^4;
unsigned char RECOK;
unsigned int msg;
unsigned int CntMainTimeout;
bit flag_CanMainTimeout;
bit flag_CanReset;
#include <bytemapOp.c>
void delay(unsigned int us)   //delay time
{
  	while(us--)
  	{
  		_nop_();
  		WDT_CONTR=0x3b;  //喂狗 1.13s;
  	}
}
void delay1(unsigned int ms)
{
  	unsigned int i,j;
  	for(i=0;i<ms;i++)
  	for(j=0;j<15;j++)
  	delay(1);
}

/****************************************************
 * 
 * sendBuf_clean
 *
 *
 ************************************************/
void sendBuf_clean(void)
{
	unsigned int i, j;
	
	for(i = 0; i < CSEND_BUF_NUM; i++)
	{
		for(j = 0; j < CSEND_BUF_SIZE; j++)
		{
			Com2CanBuf[i][j] = 0;
			Can2ComBuf[i][j] = 0;
		}
	}
}
/****************************************************
 * 找一个空的项，作为存入数据的地点 
 * function name: findEmptyIndex
 * input: 
 * output:
 * return: index
 * No change buffer
 *
 ****************************************************/
unsigned int findEmptyIndex(unsigned char buf[CSEND_BUF_NUM][CSEND_BUF_SIZE])
{
	unsigned int i;

	/** find empty one **/
	for(i = 0; i < CSEND_BUF_NUM; i++)
	{
		if(buf[i][0] == 0)
		{
			buf[i][0] = 1;
			return	i;
		}
	}
	/** 超过范围，出错 **/
	return	0xff;
}

/****************************************************
 * 找一个有数据的项，准备发送数据
 * sendCanBuf_send
 *
 ****************************************************/
unsigned int findBusyIndex(unsigned char buf[][CSEND_BUF_SIZE])
{
	unsigned int i;
	
    for(i = 0; i < CSEND_BUF_NUM; i++)
    {
    	if(buf[i][0] != 0)
    	{
    		buf[i][0] = 0;
    		return	i;
   		}
   	}

	/** 超过范围，出错 **/
	return	0xff;
}
#if	0
void setEmptyIndex(unsigned char buf[CSEND_BUF_NUM][CSEND_BUF_SIZE], unsigned int index)
{
	EA = 0;
	buf[index][0] = 0;
	EA = 1;
}
#endif
/*******************************************************************************/
void setTimer(unsigned char TimerNum, unsigned int TickCount)
{
	unsigned char	_EA;

	_EA = EA;
	EA = 0;

	if(TimerNum == 0)		comSnd_timer = TickCount;
	else if(TimerNum == 1)	canSnd_timer = TickCount;
	//else if(TimerNum == 2)	UserTimer2 = TickCount;
	//else if(TimerNum == 3)	UserTimer3 = TickCount;

	EA = _EA;
}
unsigned int getTimer(unsigned char TimerNum)
{
	unsigned int	ReturnTick;
	unsigned char	_EA;

	_EA = EA;
	EA = 0;

	if(TimerNum == 0)		ReturnTick = comSnd_timer;
	else if(TimerNum == 1)	ReturnTick = canSnd_timer;
	//else if(TimerNum == 2)	ReturnTick = UserTimer2;
	//else if(TimerNum == 3)	ReturnTick = UserTimer3;
	else						ReturnTick = 0;

	EA = _EA;

	return	ReturnTick;
}
///--------------------------------------------------
//函数原型: void init_serialcomm(void)
//参数说明: 串口初始化
//说明:     设值单片机的定时器1的方式选择波特率 。该子程序只能用于复位模式                      
//--------------------------------------------------
void init_serialcomm(void)
{
    SCON = 0x50;		   			//串口工作在模式1,速率57600bps
	  PCON |= 0x80;       //SMOD=1; 
 	  TMOD = 0x21;
	  TH1  = 0xff;
	  TL1  = 0xfa;
 	  TR1  = 1;
    //IE   |= 0x90;       //Enable Serial Interrupt 
    TR1   = 1;          // timer 1 run
    ES=1;
}
//定时器0初始化
void	T0_Ini() reentrant using 0		//定时器0初始化,10ms中断一次
{
	TR0	= 0;
	TF0	= 0;
	TH0	= 0xdc;
	TL0	= 0x31;
	TR0	= 1;
	ET0	= 1;
}
//向串口发送一个字符 
void send_char_com(unsigned char ch)  
{
    LED1=0;
	SBUF=ch;
    while(!TI);
    TI=0;
	LED1=1;
}

//向串口发送一个字符串，strlen为该字符串长度 
void send_string_com(unsigned char *str,unsigned int strlen)
{
    unsigned int k=0;
    do 
    {
        send_char_com(*(str + k));
        k++;
        WDT_CONTR=0x3b;  //喂狗 1.13s;
    } while(k < strlen);
}
//定时器0中断,不够8个就在此发送
void T0_int(void) interrupt INT_T0		//10ms进入中断一次.
{
	//INT8U cntKeyBuf[5];          //键盘缓冲环计数数组.
	//INT8U key_temp;
	//INT8U i,j;
	TH0	= 0xdc;
	TL0	= 0x31;
	//INT8U i,j;
	Pin_test=!Pin_test;
	if(cntComRxTimeout>=5)
	{
		cntComRxTimeout=5;
		if(cntComRxIndex!=0)
		{
			comRxLength=cntComRxIndex;
			if((comRxDataBuf[0]==comRxLength-1)&&(comRxDataBuf[comRxLength-1]==0xff))   //帧长度和帧结束标志正确
			{
				if((comRxDataBuf[1]==0x00)&&(comRxDataBuf[6]==0x02)&&(comRxDataBuf[7]==0x00))  //接收到查询chn,myadress
				{
					staComRx=COM_RX_ASKCHN;
					flag_com_rx_finish=1;
				}
				if((comRxDataBuf[1]==0x02)&&(comRxDataBuf[4]==0x00)&&(comRxDataBuf[5]==0x00))  //接收到数据
				{
					staComRx=COM_RX_DATA;
					flag_com_rx_finish=1;
				}
				if((comRxDataBuf[1]==0x03)&&(comRxDataBuf[4]==0x00)&&(comRxDataBuf[5]==0x00)&&(comRxDataBuf[6]==0x52))  //接收到数据确认帧
				{
					staComRx=COM_RX_DCONFIRM;
					flag_com_rx_finish=1;
				}
			}
		}
		cntComRxIndex=0;
	}
	cntComRxTimeout++;
	CntCanTxTimeout++;
	if(CntCanTxTimeout>=50)          //CAN数据发送500ms超时
	{
		CntCanTxTimeout=51;
		flag_CanTxTimeout=1;
	}
	CntCanRxTimeout++;
	if(CntCanRxTimeout>=50)          //CAN数据接收500ms超时
	{
		//CntCanRxTimeout=51;
		CntCanRxTimeout=0;
		flag_CanRxTimeout=1;
	}
	CntMainTimeout++;
	if(CntMainTimeout>=500)          //CAN5s没有数据接收时复位
	{
		CntMainTimeout=501;
		flag_CanMainTimeout=1;
	}
	
	if(comSnd_timer) comSnd_timer--;
	if(canSnd_timer) canSnd_timer--;
}
//串口接收中断函数 
void serial () interrupt 4 using 1 
{
	unsigned char ch;
  if(RI)
  {
  	RI = 0;
  	ET0=1;TR0=1;
    ch=SBUF;
    LED1=0;
  	comRxDataBuf[cntComRxIndex++] = ch;
  	cntComRxTimeout=0;
    LED1=1;
  }
}
//--------------------------------------------------
//函数原型：  void ex0_int(void) interrupt 0 //using 1
//功    能：  中断接收函数
//入口参数:   无 
//出口参数:   RevceData[]数组   
//说    明:   当sja1000 收到正确的报文时,会产生int中断           
//--------------------------------------------------
void ex0_int(void) interrupt 0 using 1
{
	unsigned char tt,tt1,length,i;
	i=0;
	msg++;
	CntMainTimeout=0;
	flag_CanMainTimeout=0;
	if(msg==5)
	{
		msg=0;
    //P34=0;
  }
  LED2=0;
  SJA_BCANAdr=REG_INTERRUPT;
  if((*SJA_BCANAdr)&0x01)//产生了接收中断,并且CAN接收完成已经清零
  {
  	SJA_BCANAdr=REG_RXBuffer1;
    tt=*SJA_BCANAdr;
    tt1=*SJA_BCANAdr;
    length=tt1&0x0F;
    if ((tt&0x40)!=0x40)                   //数据帧   假= 为远程帧
    {
    	SJA_BCANAdr =REG_RXBuffer1;          //宏定义的变量不能memcpy(CanRevceData,REG_RXBuffer4,8);
    	memcpy(RxData_Buf,SJA_BCANAdr,length+5);//功能：由src所指内存区域复制count个字节到dest所指内存区域
    	//for(i=0;i<length+2;i++)send_char_com(CanRevceData[i]);//发送到串口
    	//CanRevceData[0]=RxData_Buf[1];
    	//CanRevceData[1]=RxData_Buf[2];
    	tt=(RxData_Buf[3]>>4)&0x0f;
    	tt1=RxData_Buf[3]&0x0f;
    	memcpy(&CanRevceData[tt1*8],&RxData_Buf[5],length);
    	if(0x0 != bytemap_test_set((unsigned int)tt1))
		{
			;/** warning: the frame received, but now received again **/
		}
    	//if(tt==tt1)  //判断数据包结束
    	if(bytemap_isfull(tt + 1))    /** 判断数据包结束. tt从0开始的序号， 加1后就是的1的个数  **/
    	{
    		flag_CanRxFinish=1;
    		CntCanRxLength=length+tt*8;
    		bytemap_clean_interrupt();
    	}
    	
    	CntCanRxTimeout = 0; /** CAN数据接收超时计数清0 **/
    	//send_string_com(CanRevceData,CntCanRxLength);
    }
    else                                    //远程帧
    {
    	flag_canRxRoll=1;
    }
    BCAN_CMD_PRG(RRB_CMD);                  //释放SJA1000接收缓冲区，****已经修改
  }
  LED2=1;
}
//--------------------------------------------------

//初始化cpu

//--------------------------------------------------
void Init_Cpu(void)                                  //单片机初始化,开放外部中断0
{
     PX0=1;
     EX0=1;
     IT0=0;
    // EA=1;
}

//--------------------------------------------------
//函数原型：   void main(void)

void main(void)
{
  unsigned char ss;
  unsigned int i;
  unsigned char temptt;
  unsigned char i_buf;
  //unsigned char dstAdress[2];
//_reset_line: 
  i_buf=0;
	i=0;
	CS=1;                //片选择引脚
  EA=0;
  k1=1;
  k2=1;
  k3=1;
  k4=1;
  CntMainTimeout=0;
	flag_CanMainTimeout=0;
  key_temp=0;
  _nop_();
	_nop_();
	_nop_();
	_nop_();
	if(k1)key_temp|=0x01;else key_temp&=0xfe;
	if(k2)key_temp|=0x02;else key_temp&=0xfd;
	if(k3)key_temp|=0x04;else key_temp&=0xfb;
	if(k4)key_temp|=0x08;else key_temp&=0xf7;
  RSTCAN=0;
  delay1(500);
  RSTCAN=1;
  bytemap_clean_interrupt();		/** Don't change interrupt status**/
  sendBuf_clean();
  Init_Cpu();
  init_serialcomm();     //初始化串口 
  T0_Ini();       //定时器0初始化
  //初始化SJA1000
  ss=Sja_1000_Init(1);
  WDT_CONTR=0x3b;  //喂狗 1.13s;
  if(ss!=0)             //初始化失败
  {
  	ss=Sja_1000_Init(1);
  	LED1=0;
	  LED2=0;
	  WDT_CONTR=0x3b;  //喂狗 1.13s;
	}
	LED2=1;
	LED1=1;
	
	flag_com_rx_finish=0;
  comRxLength=0;
  cntComRxIndex=0;
  cntComRxTimeout=0;
  staComRx=COM_RX_NONE;
  
  //CntComRxTimout=0;
  //flag_ComTxAddr=0;
  flag_CanRxFinish=0;
  flag_CanReset=0;
  setTimer(0, 0/CTIMER_MS);
  setTimer(1, 0/CTIMER_MS);
  
  EA=1; //初始化成功，开总中断
  while(1)
  {
   	WDT_CONTR=0x3b;  //喂狗 1.13s;
    //--------------------------------------------------
    if(flag_CanReset==1)
    {
    	//LED1=0;
    	//LED2=0;
    	//while(1);
    	//goto _reset_line;
    }
    if(flag_CanMainTimeout==1)
    {
    	//LED1=0;
    	//LED2=0;
    	//while(1);
    	//goto _reset_line;
    }
/** 
 * done something about receive com data  
 *
 *
 **/
    if(flag_com_rx_finish == 1)
    {
    	flag_com_rx_finish = 0;
    	
    	switch(staComRx)
    	{
    		case COM_RX_DATA:		//如果是数据, 先存下
    			i_buf = findEmptyIndex(Com2CanBuf);
    			if(i_buf == 0xff)	/** error, all buffer busy **/
    			{
    				break;
    			}
    			for(i = 0; i < CSEND_BUF_SIZE - CSTART_ID; i++)
    			{		   
    				Com2CanBuf[i_buf][CSTART_ID + i] = comRxDataBuf[i];
    			}
    			Com2CanBuf[i_buf][CLENGTH_ID] = comRxLength;
				/** send Ack first **/
    			comSendDataBuf[0] = 0x07;
          		comSendDataBuf[1] = 0x03;
          		comSendDataBuf[2] = 0x00;
          		comSendDataBuf[3] = 0x00;
    			comSendDataBuf[4] = Com2CanBuf[i_buf][CSTART_ID + 2];                 //发送数据确认帧
    			comSendDataBuf[5] = Com2CanBuf[i_buf][CSTART_ID + 3];
          		comSendDataBuf[6] = 0x52;
          		comSendDataBuf[7] = 0xff;
          		send_string_com(comSendDataBuf,comSendDataBuf[0]+1);
          		
    			break;
    		case COM_RX_DCONFIRM:
    			break;
    		case COM_RX_ASKCHN:
    			comSendDataBuf[0]=12;
	  			comSendDataBuf[1]=0x01;
	  			comSendDataBuf[2]=0x00;
	  			comSendDataBuf[3]=0x00;
	  			comSendDataBuf[4]=0x00;
	  			comSendDataBuf[5]=0x00;
	  			comSendDataBuf[6]=0x02;  //注意
	  			comSendDataBuf[7]=0x0b+key_temp;//  //chn
	  			comSendDataBuf[8]=0x00;
	  			comSendDataBuf[9]=0x00;
	  			comSendDataBuf[10]=0x00;  //addressL
	  			comSendDataBuf[11]=0x00;  //addressH
	  			comSendDataBuf[12]=0xff;
	  			send_string_com(comSendDataBuf,comSendDataBuf[0]+1);
	  			break;
    		default: break;	
    	}
    }

    if(flag_canRxRoll == 1)      //如果取数标志已置位，就查看是否有数据待发，将读到的数经canbus发出
    {
    	flag_canRxRoll = 0;
    	
    	i_buf = findBusyIndex(Com2CanBuf);
     	if(i_buf != 0xff)	/** all buffer empty ? **/
   		{
   			for(i = 0; i < Com2CanBuf[i_buf][CLENGTH_ID]; i++)
   			{
   				comSendDataBuf[i] = Com2CanBuf[i_buf][CSTART_ID + i];
   			}
   			comSendLength = Com2CanBuf[i_buf][CLENGTH_ID];
   			//setEmptyIndex(Com2CanBuf, i_buf);
   			packAcr0=0x12;  //server ID
    		packAcr1=key_temp;
    		CAN_Send_Large(comSendDataBuf,comSendLength);
   		}
    }
   	if(flag_CanRxTimeout == 1)
	{
		flag_CanRxTimeout=0;
		bytemap_clean();
	}
	/** 连续发往com串口的时间到 **/
	if(getTimer(0) == 0)
	{
		i_buf = findBusyIndex(Can2ComBuf);	
    	if(i_buf < CSEND_BUF_NUM)
    	{
    		comSendLength = Can2ComBuf[i_buf][CLENGTH_ID];
    		for(i = 0; i < CSEND_BUF_SIZE; i++)
    		{
    			comSendDataBuf[i] = Can2ComBuf[i_buf][CSTART_ID + i];
    		}
    		//setEmptyIndex(Can2ComBuf, i_buf);
    		send_string_com(comSendDataBuf, comSendLength);
    		setTimer(0, 70/CTIMER_MS);
    	}
	}
    if(flag_CanRxFinish == 1)		/** 查是否有收到canbus数据待转发给刷卡器 **/
    {
    	flag_CanRxFinish = 0;
    	flag_CanRxHead=0;			/** 有什么用? **/
    	#if	0
    		comSendLength=CntCanRxLength;   //comRxLength = comRxDataBuf[0]+1
    		for(i=0;i<MAX_BUF_NUM;i++)comSendDataBuf[i]=CanRevceData[i];
    		send_string_com(comSendDataBuf, comSendLength);
    	#else
    	    /** 找一可用的缓冲，并将数据存入 **/	
    		i_buf = findEmptyIndex(Can2ComBuf);
    		if(i_buf < CSEND_BUF_NUM)
    		{
    			Can2ComBuf[i_buf][CLENGTH_ID] = CntCanRxLength;
    			for(i = 0; i < CSEND_BUF_SIZE - CSTART_ID; i++)
    			{
    				Can2ComBuf[i_buf][CSTART_ID + i] = CanRevceData[i];
    			}
    		}
    	#endif
    	/** 收到服务器的数据确认，若有数据继续发 **/
    	#if	1
    	i_buf = findBusyIndex(Com2CanBuf);
     	if(i_buf != 0xff)
   		{
   			for(i = 0; i < Com2CanBuf[i_buf][CLENGTH_ID]; i++)
   			{
   				comSendDataBuf[i] = Com2CanBuf[i_buf][CSTART_ID + i];
   			}
   			comSendLength = Com2CanBuf[i_buf][CLENGTH_ID];
   			//setEmptyIndex(Com2CanBuf, i_buf);
   			
   			packAcr0=0x12;  //server ID
    		packAcr1=key_temp;
    		CAN_Send_Large(comSendDataBuf,comSendLength);
   		}
   		#endif
    }
    //--------------------------------------------------
    SJA_BCANAdr = REG_STATUS;    
    temptt=*SJA_BCANAdr;
    if ((temptt&0x40)==0x40)                     //读错误状态有错误时复位
    {
    	WDT_CONTR=0x3b;  //喂狗 1.13s;
    	LED1=0;
    	LED2=0;
    	delay1(500);
    	Sja_1000_Init(0);
    	LED1=1;
    	LED2=1;
    	delay1(500);
    }
  }
}



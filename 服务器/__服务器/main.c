//--------------------------------------------------
//PeliCAN模式，标准帧
//RS232波特率：57600
//CAN波特率：125kbps
//设置ID在18行
//--------------------------------------------------
#define MAIN_GLOBALS
#include <includes.h>
#include <CodeCrc.h>
#define MAX_BUF_NUM     128
#define ROOM_NUM 40			//一个协调器里存放房间号的个数
#define COORD_NUM 16		//CAN里连协调器的个数

#define COM_RX_NONE     0x00 //空数据
#define COM_RX_DATA     0x01 //接收到数据
#define COM_RX_DCONFIRM 0x02 //接收到数据确认
#define COM_RX_ASKCHN   0x03 //接收到数据确认

#define CSEND_BUF_NUM	8		//canbus接收(同时也是com发送)缓冲区个数
#define CSEND_BUF_SIZE	128		//单个canbus接收(同时也是com发送)缓冲区大小
unsigned char Can2ComBuf[CSEND_BUF_NUM][CSEND_BUF_SIZE];		//Don't write in it in main loop
#define	CLENGTH_ID	1
#define	CCOORD_ID	(CLENGTH_ID + 1)
#define	CSTART_ID	(CCOORD_ID + 1)

#define	CTIMER_MS	10
#define	CROLLCYCLE	(100)		/** 轮询周期可设为100ms **/

//#define INBUF_LEN 10   //数据长度
//sfr WDT_CONTR = 0xC1; //Watch-Dog-Timer Control register
unsigned char RoomTable[COORD_NUM][ROOM_NUM][2];
unsigned char comSendDataBuf[MAX_BUF_NUM];
//unsigned char cntComSendIndex;
unsigned int comSendLength;
unsigned char comRxDataBuf[MAX_BUF_NUM];
unsigned char comRxDataCirl[6][MAX_BUF_NUM];
unsigned char comWrPoint;
unsigned char comRdPoint;
unsigned int comRxLength;
unsigned int cntComRxIndex;
unsigned char cntComRxTimeout;
unsigned char flag_com_rx_finish;
unsigned char staComRx;
unsigned char canRxCoorNum;
unsigned char comSendCoorNum;

unsigned char cntRollTmr;
unsigned char cntComSndTmr;	/** 连续发送多组数据给串口的组间延时计数器 **/
unsigned char cntReqComTmr;


unsigned char rollCnt;
unsigned char rollDelayCnt;		/** 本协调器延迟次数 **/
unsigned char rollCycleCnt;		/** 本次16个协调器中已有多少个延迟**/

//unsigned char data Tmod_data;
unsigned char run_lamp_flush_count = 0;
unsigned char run_lamp_flush_time = 10;
sbit  run_lamp = P1^0;
unsigned char CanRevceData[MAX_BUF_NUM];
//unsigned int data rxbuffer[10]={REG_RXBuffer1,REG_RXBuffer2,REG_RXBuffer3,REG_RXBuffer4,REG_RXBuffer5,REG_RXBuffer6,REG_RXBuffer7,REG_RXBuffer8,REG_RXBuffer9,REG_RXBuffer10};
//unsigned int data txbuffer[10]={REG_TXBuffer1,REG_TXBuffer2,REG_TXBuffer3,REG_TXBuffer4,REG_TXBuffer5,REG_TXBuffer6,REG_TXBuffer7,REG_TXBuffer8,REG_TXBuffer9,REG_TXBuffer10};
//unsigned char data Send_CAN_Info_ID[5]={0x08,0x55,0xe0,0x55,0x55};
//unsigned char data Send_CAN_Info_ID[5]={0x01,0x55,0xe0,0x55,0x55};
//unsigned char data Com_RecBuff[8]={0x01,0x02,0x03,0x4,0x05,0x06,0x07,0x08};
unsigned char bdata flag_init;
//*************************
#define INBUF_LEN 8   //数据长度

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
unsigned char RECOK;
unsigned int msg;
unsigned int CntMainTimeout;
bit flag_CanMainTimeout;
bit flag_CanReset;
bit flag_Roll;

#include <bytemapOp.c>

unsigned char GetCrc8(unsigned char *ptr, unsigned char len)
{
	unsigned char crc8,dataa;
	crc8=0;
	while(len--!=0)
	{
		dataa = *ptr++;
		crc8=crc8_tbl[crc8^dataa];
	}
	return crc8;
}
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
 ************************************************/
void sendBuf_clean(void)
{
	unsigned int i, j;
	
	for(i = 0; i < CSEND_BUF_NUM; i++)
	{
		for(j = 0; j < CSEND_BUF_SIZE; j++)
		{
			Can2ComBuf[i][j] = 0;
		}
	}
}

/****************************************************
 * 找一个空的项，作为存入数据的地点 
 * function name: findEmptyIndex_Interrupt
 * input: 
 * output:
 * return: index
 * No change buffer, No change interrupt flag
 *
 ****************************************************/
unsigned int findEmptyIndex_Interrupt(void)
{
	unsigned int i;

	/** find empty one **/
	for(i = 0; i < CSEND_BUF_NUM; i++)
	{
		if(Can2ComBuf[i][0] == 0)
		{
			Can2ComBuf[i][0] = 1;
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
 ************************************************/
unsigned int findBusyIndex(void)
{
	unsigned int i;
	
    for(i = 0; i < CSEND_BUF_NUM; i++)
    {
    	if(Can2ComBuf[i][0] != 0)
    	{
    		Can2ComBuf[i][0] = 0;
    		return	i;
   		}
   	}

	/** 超过范围，出错 **/
	return	0xff;
}

#if	0
void setIndexEmpty(unsigned int index)
{

	unsigned char	_EA;
	_EA = EA;
	
	EA = 0;
	Can2ComBuf[index][0] = 0;
	EA = _EA;
}
#endif
/*******************************************************************************
 * timer operation
 *
 * setTimer/addTimer/getTimer
 *
 *******************************************************************************/
void setTimer(unsigned char TimerNum, unsigned int TickCount)
{
	unsigned char	_EA;

	_EA = EA;
	EA = 0;

	if(TimerNum == 0)		cntComSndTmr = TickCount;		/** 设定串口发送延迟 **/
	else if(TimerNum == 1)	cntReqComTmr = TickCount;
	else if(TimerNum == 2)	cntRollTmr = TickCount;			

	EA = _EA;
}
#if	0
void addTimer(unsigned char TimerNum, unsigned int TickCount)
{
	unsigned char	_EA;

	_EA = EA;
	EA = 0;

	if(TimerNum == 0)		cntComSndTmr += TickCount;		/** 设定串口发送延迟 **/
	else if(TimerNum == 1)	cntReqComTmr += TickCount;
	else if(TimerNum == 2)	cntRollTmr += TickCount;			

	EA = _EA;
}
#endif
unsigned int getTimer(unsigned char TimerNum)
{
	unsigned int	ReturnTick;
	unsigned char	_EA;

	_EA = EA;
	EA = 0;

	if(TimerNum == 0)		ReturnTick = cntComSndTmr;
	else if(TimerNum == 1)	ReturnTick = cntReqComTmr;
	else if(TimerNum == 2)	ReturnTick = cntRollTmr;
	else					ReturnTick = 0;

	EA = _EA;

	return	ReturnTick;
}
/*********************************************************************/
void updateRDPoint(void)
{	
	unsigned char	_EA;

	_EA = EA;
	EA = 0;
	comRdPoint++;			/** The only position that change RdPoint **/
	comRdPoint%=6;
	EA = _EA;	
}
void updateWRPoint(void)
{	
	unsigned char	_EA;

	_EA = EA;
	EA = 0;
	comWrPoint++;			/** The only position change WrPoint **/
	comWrPoint %= 6;
	EA = _EA;	
}
/********************************************************************* 
 *
 * 如果收到canbus数据，表明协调器上可能有更多的数据，需要延时待
 * 但考虑到如果每一台协调器上都有数据，则最长需要16秒才能将所有协调器轮询一遍，这显然是不合理的。 
 *
 * 限制如下：
 * 1、每一轮询周期内(16个协调器全部查一遍)，最多只能有3个协调器对应的时间片可以延时
 * 2、每个协调器对应的时间片最多可以延长2次。
 *
 * 实现：
 * a、当发起轮询时，rollDelayCnt = 0，若协调器号是0， rollCycleCnt=0。
 * b、当收到canbus数据时，若rollDelayCnt 为0，则rollCycleCnt加1。
 * c、当收到canbus数据时，rollDelayCnt加1并调整不超出上限。
 * d、查rollDelayCnt与rollCycleCnt是否超出上限，如果没有超出上限，就延长时间片，否则不延长时间片。
 *
 *********************************************************************/
void rollDelayRestart(void)
{
	unsigned char	_EA;

	_EA = EA;
	EA = 0;
	
	if(rollCnt == 0)
	{
		rollCycleCnt = 0;
	}
	rollDelayCnt = 0;

	EA = _EA;
}
/*
 *
 *return: allow add timeslice or not
 */
unsigned char rollDelayUpdate(void)
{
	unsigned char ret = 0;
	/** 先更新 **/
	if(rollDelayCnt == 0)	/** 一定是第一次收到新协调器的数据 **/
	{
		rollCycleCnt++;
	}
	rollDelayCnt++;
	
	/** 再修正 **/
	if(rollCycleCnt > 3)
	{
		rollCycleCnt = 4;
		return 0;
	}
	if(rollDelayCnt > 4)
	{
		rollDelayCnt = 5;
		return 0;
	}
	return	1;
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
	//INT8U i,j;
	unsigned char i;
	unsigned int startIndex;
	TH0	= 0xdc;
	TL0	= 0x31;
	Pin_test=!Pin_test;
	if(cntComRxTimeout>=3)
	{
		cntComRxTimeout=3;
		if(cntComRxIndex!=0)
		{
			if(comRxDataBuf[0] >= cntComRxIndex - 1)	
			{		
				setTimer(0, 0/CTIMER_MS);		/** 收到一组串口数据，可以发下一组串口数据，而不必等待超时 **/
			}
			startIndex = 0;
			comRxLength=cntComRxIndex;
			
			while((comRxDataBuf[startIndex] <= comRxLength - 1) && (comRxDataBuf[comRxDataBuf[startIndex]] == 0xff))	/** 多组数据挤在一起了，需要自己解析出来 **/
			{
				staComRx = COM_RX_DATA;
				flag_com_rx_finish = 1;

				for(i = startIndex; i < (startIndex + comRxDataBuf[startIndex] + 1); i++)
				{
					comRxDataCirl[comWrPoint][i - startIndex] = comRxDataBuf[i];
				}
				if(comRdPoint != ((comWrPoint + 1) % 6))   /** 还有缓冲环可用, 更新缓冲指针。如果缓冲环满，这里会覆盖上次收到的数据 **/
				{
					#if	0
					comWrPoint++;						/** The only position change WrPoint **/
					comWrPoint %= 6;
					#else
					updateWRPoint();
					#endif
				}
				
				startIndex = startIndex + comRxDataBuf[startIndex] + 1;
				comRxLength = comRxLength - comRxDataBuf[startIndex] - 1;
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

	if(cntRollTmr) cntRollTmr--;		/** 轮询发远程帧时间, 暂定70ms(原为180ms) **/
	if(cntReqComTmr) cntReqComTmr--;	/** canbus发送延迟，查串口数据, 暂定60ms **/
	if(cntComSndTmr) cntComSndTmr--;	/** 串口发送延迟，暂定1000ms **/

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
	unsigned char tt,tt1,length,i,RxData_Buf[10];
	i=0;
	msg++;
	
	//cntRollTmr = 0;			/** 此处似无需要 **/
	//addTimer(2, (CROLLCYCLE * 9 / CTIMER_MS));	/** 一旦收到数据，就延长本一轮时间槽时长 **/
	if(rollDelayUpdate())
	{
		setTimer(2, (CROLLCYCLE * 9 / CTIMER_MS));	/** 收到数据，可能要延长本一轮时间槽的时长 **/
	}
	//CntMainTimeout=0;
	//flag_CanMainTimeout=0;
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
    	canRxCoorNum=RxData_Buf[2];
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
		#if	0
    	else if(bytemap_isfull(tt + 1) == 0)
    	{
    		//send_string_com(byteMap,16);	/** ??????????????????? **/
    	}
		#endif
    	
    	CntCanRxTimeout = 0; /** CAN数据接收超时计数清0 **/
    	//send_string_com(CanRevceData,CntCanRxLength);
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
	unsigned char ss,i,j;
	unsigned char temptt;
	unsigned char i_buf;
	unsigned int i_16u;
	unsigned int bufindex;
	//unsigned char dstAdress[2];
	unsigned char tabBuf[ROOM_NUM][2];
	//_reset_line:
	i_buf=0;
	rollCnt=0;
	i=0;
	CS=1;                //片选择引脚
	EA=0;
	CntMainTimeout=0;
	flag_CanMainTimeout=0;
	key_temp=0xff;
	RSTCAN=0;
	delay1(500);
	RSTCAN=1;
	bytemap_clean_interrupt();		/** Don't change interrupt status**/
	Init_Cpu();
	init_serialcomm();     //初始化串口
	sendBuf_clean();
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
	cntRollTmr = 0;
	staComRx=COM_RX_NONE;

	//CntComRxTimout=0;
	//flag_ComTxAddr=0;
	flag_CanRxFinish=0;
	flag_CanReset=0;
	flag_Roll = 0;

	for(i=0;i<COORD_NUM;i++)
	{
		for(j=0;j<ROOM_NUM;j++)
		{
			RoomTable[i][j][0]=0;
			RoomTable[i][j][1]=0;
		}
	}
	comWrPoint=0;
	comRdPoint=0;
	for(i=0;i<6;i++)
	{
		for(j=0;j<MAX_BUF_NUM;j++)
		{
			comRxDataCirl[i][j]=0;
		}
	}
	EA=1; //初始化成功，开总中
	while(1)
	{
		WDT_CONTR=0x3b;  //喂狗 1.13s;
		//--------------------------------------------------

		//if((comWrPoint!=comRdPoint) && (CntMainTimeout >= 18))     //串口缓冲环中有数据
        if(getTimer(1) == 0)					/** 向canbus发数据的时间到了 **/
        {
        	setTimer(1, 30/CTIMER_MS);
        	//flag_ReqComPort = 0;
        	if(comWrPoint!=comRdPoint)          //串口缓冲环中有数据
        	{
        
        		flag_com_rx_finish=0;	/** 似没有用到? **/
        		//comSendLength=comRxLength+1;   //comRxLength = comRxDataBuf[0]+1
        		comSendLength=comRxDataCirl[comRdPoint][0]+2;
        		comSendDataBuf[0]=comRxDataCirl[comRdPoint][0]+1;
        		comSendDataBuf[1]=0x02;
        		comSendDataBuf[2]=0;
        		comSendDataBuf[3]=0;
        		comSendDataBuf[4]=comRxDataCirl[comRdPoint][2];
        		comSendDataBuf[5]=comRxDataCirl[comRdPoint][3];
        		for(i_16u=6;i_16u<comSendLength-1;i_16u++)
        		{
        			comSendDataBuf[i_16u]=comRxDataCirl[comRdPoint][i_16u-2];
        		}
        		comSendDataBuf[comSendLength-1]=0xff;
        		//加入寻址功能
        		for(j=0;j<COORD_NUM;j++)
        		{
        			for(i=0;i<ROOM_NUM;i++)
        			{
        				if((RoomTable[j][i][0]==comSendDataBuf[4])&&(RoomTable[j][i][1]==comSendDataBuf[5]))break;
        			}
        			if(i<ROOM_NUM)//break;  //接收到的在表格内
        			{
        				packAcr0=j;
        				packAcr1=0x34;
        				CAN_Send_Large(comSendDataBuf,comSendLength);
        			}
        		}
        		staComRx=COM_RX_NONE;
        		#if	0
        		comRdPoint++;			/** The only position that change RdPoint **/
        		comRdPoint%=6;
        		#else
        		updateRDPoint();
        		#endif
        
        		//cntRollTmr = 0; //可设其它数据,如10	/** 将发送远程帧的计时器清0, 推迟对下一协调器的轮询，等待可能的下一组数据 **/
        	}
        }
		//if(CntMainTimeout >= 18)	/** 轮询发远程帧(收can数据及发can数据时不能轮询，需要计时清0) **/
		//if(flag_Roll == 1)
		if(getTimer(2) == 0)		/** 轮询发远程帧 **/
		{
			setTimer(2, CROLLCYCLE/CTIMER_MS);	/** 下一时间槽计时开始 **/
			
			//cntRollTmr = 0;
			//flag_Roll = 0;	
			
			canSendRtr(rollCnt,0x34);
			rollCnt++;
			rollCnt%=16;
			
			rollDelayRestart();
		}
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
		//if(flag_com_rx_finish==1)      //如果取数标志已置位，就将读到的数从串口发出
		if(flag_CanRxTimeout == 1)
		{
			flag_CanRxTimeout=0;
			bytemap_clean();
		}
		
		if(flag_CanRxFinish==1)
		{
			bufindex = findEmptyIndex_Interrupt();
			if(bufindex < CSEND_BUF_NUM)
			{
				Can2ComBuf[bufindex][CLENGTH_ID] = CntCanRxLength;
				Can2ComBuf[bufindex][CCOORD_ID] = canRxCoorNum;
				for(i = 0; i < CntCanRxLength; i++)
				{
					Can2ComBuf[bufindex][CSTART_ID + i] = CanRevceData[i];
				}
			}
		}
		//if(flag_CanRxFinish==1)
		bufindex = findBusyIndex();		/** 收到 **/
		//if((bufindex < CSEND_BUF_NUM) && (getTimer(0) == 0))
		if(bufindex < CSEND_BUF_NUM)
		{
			comSendLength = Can2ComBuf[bufindex][CLENGTH_ID] - 1;
			comSendCoorNum = Can2ComBuf[bufindex][CCOORD_ID];
			comSendDataBuf[0] = Can2ComBuf[bufindex][CSTART_ID + 0] - 1;
			comSendDataBuf[1] = '0';
			comSendDataBuf[2] = Can2ComBuf[bufindex][CSTART_ID + 2];
			comSendDataBuf[3] = Can2ComBuf[bufindex][CSTART_ID + 3];
			for(i_16u = 4; i_16u < comSendLength - 2; i_16u++)
			{
				comSendDataBuf[i_16u] = Can2ComBuf[bufindex][i_16u + CSTART_ID + 2];;
			}
			comSendDataBuf[comSendLength - 2] = GetCrc8(comSendDataBuf, comSendLength - 2);
			comSendDataBuf[comSendLength - 1] = 0xff;
			
			send_string_com(comSendDataBuf, comSendLength);
			
			flag_CanRxFinish=0;
			flag_CanRxHead=0;
			setTimer(0, 100/CTIMER_MS);			/** init cntComSndTmr 100ms,  **/
			for(i=0;i<ROOM_NUM;i++)
			{
				if((RoomTable[comSendCoorNum][i][0]==comSendDataBuf[2])&&(RoomTable[comSendCoorNum][i][1]==comSendDataBuf[3]))break;
			}
			if(i>=ROOM_NUM)  //接收到的不在表格内
			{
				for(i=0;i<ROOM_NUM;i++)
				{
					if((RoomTable[comSendCoorNum][i][0]==0x00)&&(RoomTable[comSendCoorNum][i][1]==0x00))break;
				}
				if(i>=ROOM_NUM)     //表格已满,则将最先进入的删除。将此地址加到最后一个
				{
					for(j=0;j<ROOM_NUM-1;j++)
					{
						tabBuf[j][0]=RoomTable[comSendCoorNum][j+1][0];
						tabBuf[j][1]=RoomTable[comSendCoorNum][j+1][1];
					}
					tabBuf[ROOM_NUM-1][0]=comSendDataBuf[2];
					tabBuf[ROOM_NUM-1][1]=comSendDataBuf[3];
					for(j=0;j<ROOM_NUM;j++)
					{
						RoomTable[comSendCoorNum][j][0]=tabBuf[j][0];
						RoomTable[comSendCoorNum][j][1]=tabBuf[j][1];
					}
				}
				else
				{
					RoomTable[comSendCoorNum][i][0]=comSendDataBuf[2];
					RoomTable[comSendCoorNum][i][1]=comSendDataBuf[3];
				}
			}
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



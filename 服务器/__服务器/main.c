//--------------------------------------------------
//PeliCANģʽ����׼֡
//RS232�����ʣ�57600
//CAN�����ʣ�125kbps
//����ID��18��
//--------------------------------------------------
#define MAIN_GLOBALS
#include <includes.h>
#include <CodeCrc.h>
#define MAX_BUF_NUM     128
#define ROOM_NUM 40			//һ��Э�������ŷ���ŵĸ���
#define COORD_NUM 16		//CAN����Э�����ĸ���

#define COM_RX_NONE     0x00 //������
#define COM_RX_DATA     0x01 //���յ�����
#define COM_RX_DCONFIRM 0x02 //���յ�����ȷ��
#define COM_RX_ASKCHN   0x03 //���յ�����ȷ��

#define CSEND_BUF_NUM	8		//canbus����(ͬʱҲ��com����)����������
#define CSEND_BUF_SIZE	128		//����canbus����(ͬʱҲ��com����)��������С
unsigned char Can2ComBuf[CSEND_BUF_NUM][CSEND_BUF_SIZE];		//Don't write in it in main loop
#define	CLENGTH_ID	1
#define	CCOORD_ID	(CLENGTH_ID + 1)
#define	CSTART_ID	(CCOORD_ID + 1)

#define	CTIMER_MS	10
#define	CROLLCYCLE	(100)		/** ��ѯ���ڿ���Ϊ100ms **/

//#define INBUF_LEN 10   //���ݳ���
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
unsigned char cntComSndTmr;	/** �������Ͷ������ݸ����ڵ������ʱ������ **/
unsigned char cntReqComTmr;


unsigned char rollCnt;
unsigned char rollDelayCnt;		/** ��Э�����ӳٴ��� **/
unsigned char rollCycleCnt;		/** ����16��Э���������ж��ٸ��ӳ�**/

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
#define INBUF_LEN 8   //���ݳ���

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
  		WDT_CONTR=0x3b;  //ι�� 1.13s;
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
 * ��һ���յ����Ϊ�������ݵĵص� 
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
	/** ������Χ������ **/
	return	0xff;
}
/****************************************************
 * ��һ�������ݵ��׼����������
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

	/** ������Χ������ **/
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

	if(TimerNum == 0)		cntComSndTmr = TickCount;		/** �趨���ڷ����ӳ� **/
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

	if(TimerNum == 0)		cntComSndTmr += TickCount;		/** �趨���ڷ����ӳ� **/
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
 * ����յ�canbus���ݣ�����Э�����Ͽ����и�������ݣ���Ҫ��ʱ��
 * �����ǵ����ÿһ̨Э�����϶������ݣ������Ҫ16����ܽ�����Э������ѯһ�飬����Ȼ�ǲ�����ġ� 
 *
 * �������£�
 * 1��ÿһ��ѯ������(16��Э����ȫ����һ��)�����ֻ����3��Э������Ӧ��ʱ��Ƭ������ʱ
 * 2��ÿ��Э������Ӧ��ʱ��Ƭ�������ӳ�2�Ρ�
 *
 * ʵ�֣�
 * a����������ѯʱ��rollDelayCnt = 0����Э��������0�� rollCycleCnt=0��
 * b�����յ�canbus����ʱ����rollDelayCnt Ϊ0����rollCycleCnt��1��
 * c�����յ�canbus����ʱ��rollDelayCnt��1���������������ޡ�
 * d����rollDelayCnt��rollCycleCnt�Ƿ񳬳����ޣ����û�г������ޣ����ӳ�ʱ��Ƭ�������ӳ�ʱ��Ƭ��
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
	/** �ȸ��� **/
	if(rollDelayCnt == 0)	/** һ���ǵ�һ���յ���Э���������� **/
	{
		rollCycleCnt++;
	}
	rollDelayCnt++;
	
	/** ������ **/
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
//����ԭ��: void init_serialcomm(void)
//����˵��: ���ڳ�ʼ��
//˵��:     ��ֵ��Ƭ���Ķ�ʱ��1�ķ�ʽѡ������ �����ӳ���ֻ�����ڸ�λģʽ                      
//--------------------------------------------------
void init_serialcomm(void)
{
    SCON = 0x50;		   			//���ڹ�����ģʽ1,����57600bps
	  PCON |= 0x80;       //SMOD=1; 
 	  TMOD = 0x21;
	  TH1  = 0xff;
	  TL1  = 0xfa;
 	  TR1  = 1;
    //IE   |= 0x90;       //Enable Serial Interrupt 
    TR1   = 1;          // timer 1 run
    ES=1;
}
//��ʱ��0��ʼ��
void	T0_Ini() reentrant using 0		//��ʱ��0��ʼ��,10ms�ж�һ��
{
	TR0	= 0;
	TF0	= 0;
	TH0	= 0xdc;
	TL0	= 0x31;
	TR0	= 1;
	ET0	= 1;
}
//�򴮿ڷ���һ���ַ� 
void send_char_com(unsigned char ch)  
{
    LED1=0;
	SBUF=ch;
    while(!TI);
    TI=0;
	LED1=1;
}

//�򴮿ڷ���һ���ַ�����strlenΪ���ַ������� 
void send_string_com(unsigned char *str,unsigned int strlen)
{
    unsigned int k=0;
    do 
    {
        send_char_com(*(str + k));
        k++;
        WDT_CONTR=0x3b;  //ι�� 1.13s;
    } while(k < strlen);
}

//��ʱ��0�ж�,����8�����ڴ˷���
void T0_int(void) interrupt INT_T0		//10ms�����ж�һ��.
{
	//INT8U cntKeyBuf[5];          //���̻��廷��������.
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
				setTimer(0, 0/CTIMER_MS);		/** �յ�һ�鴮�����ݣ����Է���һ�鴮�����ݣ������صȴ���ʱ **/
			}
			startIndex = 0;
			comRxLength=cntComRxIndex;
			
			while((comRxDataBuf[startIndex] <= comRxLength - 1) && (comRxDataBuf[comRxDataBuf[startIndex]] == 0xff))	/** �������ݼ���һ���ˣ���Ҫ�Լ��������� **/
			{
				staComRx = COM_RX_DATA;
				flag_com_rx_finish = 1;

				for(i = startIndex; i < (startIndex + comRxDataBuf[startIndex] + 1); i++)
				{
					comRxDataCirl[comWrPoint][i - startIndex] = comRxDataBuf[i];
				}
				if(comRdPoint != ((comWrPoint + 1) % 6))   /** ���л��廷����, ���»���ָ�롣������廷��������Ḳ���ϴ��յ������� **/
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
	if(CntCanTxTimeout>=50)          //CAN���ݷ���500ms��ʱ
	{
		CntCanTxTimeout=51;
		flag_CanTxTimeout=1;
	}
	CntCanRxTimeout++;
	if(CntCanRxTimeout>=50)          //CAN���ݽ���500ms��ʱ
	{
		//CntCanRxTimeout=51;
		CntCanRxTimeout=0;
		flag_CanRxTimeout=1;
	}
	CntMainTimeout++;
	if(CntMainTimeout>=500)          //CAN5sû�����ݽ���ʱ��λ
	{
		CntMainTimeout=501;
		flag_CanMainTimeout=1;
	}

	if(cntRollTmr) cntRollTmr--;		/** ��ѯ��Զ��֡ʱ��, �ݶ�70ms(ԭΪ180ms) **/
	if(cntReqComTmr) cntReqComTmr--;	/** canbus�����ӳ٣��鴮������, �ݶ�60ms **/
	if(cntComSndTmr) cntComSndTmr--;	/** ���ڷ����ӳ٣��ݶ�1000ms **/

}
//���ڽ����жϺ��� 
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
//����ԭ�ͣ�  void ex0_int(void) interrupt 0 //using 1
//��    �ܣ�  �жϽ��պ���
//��ڲ���:   �� 
//���ڲ���:   RevceData[]����   
//˵    ��:   ��sja1000 �յ���ȷ�ı���ʱ,�����int�ж�           
//--------------------------------------------------
void ex0_int(void) interrupt 0 using 1
{
	unsigned char tt,tt1,length,i,RxData_Buf[10];
	i=0;
	msg++;
	
	//cntRollTmr = 0;			/** �˴�������Ҫ **/
	//addTimer(2, (CROLLCYCLE * 9 / CTIMER_MS));	/** һ���յ����ݣ����ӳ���һ��ʱ���ʱ�� **/
	if(rollDelayUpdate())
	{
		setTimer(2, (CROLLCYCLE * 9 / CTIMER_MS));	/** �յ����ݣ�����Ҫ�ӳ���һ��ʱ��۵�ʱ�� **/
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
  if((*SJA_BCANAdr)&0x01)//�����˽����ж�,����CAN��������Ѿ�����
  {
  	SJA_BCANAdr=REG_RXBuffer1;
    tt=*SJA_BCANAdr;
    tt1=*SJA_BCANAdr;
    length=tt1&0x0F;
    if ((tt&0x40)!=0x40)                   //����֡   ��= ΪԶ��֡
    {
    	SJA_BCANAdr =REG_RXBuffer1;          //�궨��ı�������memcpy(CanRevceData,REG_RXBuffer4,8);
    	memcpy(RxData_Buf,SJA_BCANAdr,length+5);//���ܣ���src��ָ�ڴ�������count���ֽڵ�dest��ָ�ڴ�����
    	//for(i=0;i<length+2;i++)send_char_com(CanRevceData[i]);//���͵�����
    	canRxCoorNum=RxData_Buf[2];
    	tt=(RxData_Buf[3]>>4)&0x0f;
    	tt1=RxData_Buf[3]&0x0f;
    	memcpy(&CanRevceData[tt1*8],&RxData_Buf[5],length);
    	if(0x0 != bytemap_test_set((unsigned int)tt1))
		{
			;/** warning: the frame received, but now received again **/
		}
    	//if(tt==tt1)  //�ж����ݰ�����
		if(bytemap_isfull(tt + 1))    /** �ж����ݰ�����. tt��0��ʼ����ţ� ��1����ǵ�1�ĸ���  **/
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
    	
    	CntCanRxTimeout = 0; /** CAN���ݽ��ճ�ʱ������0 **/
    	//send_string_com(CanRevceData,CntCanRxLength);
    }
    BCAN_CMD_PRG(RRB_CMD);                  //�ͷ�SJA1000���ջ�������****�Ѿ��޸�
  }
  LED2=1;
}
//--------------------------------------------------

//��ʼ��cpu

//--------------------------------------------------
void Init_Cpu(void)                                  //��Ƭ����ʼ��,�����ⲿ�ж�0
{
     PX0=1;
     EX0=1;
     IT0=0;
    // EA=1;
}

//--------------------------------------------------
//����ԭ�ͣ�   void main(void)

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
	CS=1;                //Ƭѡ������
	EA=0;
	CntMainTimeout=0;
	flag_CanMainTimeout=0;
	key_temp=0xff;
	RSTCAN=0;
	delay1(500);
	RSTCAN=1;
	bytemap_clean_interrupt();		/** Don't change interrupt status**/
	Init_Cpu();
	init_serialcomm();     //��ʼ������
	sendBuf_clean();
	T0_Ini();       //��ʱ��0��ʼ��
	//��ʼ��SJA1000
	ss=Sja_1000_Init(1);
	WDT_CONTR=0x3b;  //ι�� 1.13s;
	if(ss!=0)             //��ʼ��ʧ��
	{
		ss=Sja_1000_Init(1);
		LED1=0;
		LED2=0;
		WDT_CONTR=0x3b;  //ι�� 1.13s;
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
	EA=1; //��ʼ���ɹ���������
	while(1)
	{
		WDT_CONTR=0x3b;  //ι�� 1.13s;
		//--------------------------------------------------

		//if((comWrPoint!=comRdPoint) && (CntMainTimeout >= 18))     //���ڻ��廷��������
        if(getTimer(1) == 0)					/** ��canbus�����ݵ�ʱ�䵽�� **/
        {
        	setTimer(1, 30/CTIMER_MS);
        	//flag_ReqComPort = 0;
        	if(comWrPoint!=comRdPoint)          //���ڻ��廷��������
        	{
        
        		flag_com_rx_finish=0;	/** ��û���õ�? **/
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
        		//����Ѱַ����
        		for(j=0;j<COORD_NUM;j++)
        		{
        			for(i=0;i<ROOM_NUM;i++)
        			{
        				if((RoomTable[j][i][0]==comSendDataBuf[4])&&(RoomTable[j][i][1]==comSendDataBuf[5]))break;
        			}
        			if(i<ROOM_NUM)//break;  //���յ����ڱ����
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
        
        		//cntRollTmr = 0; //������������,��10	/** ������Զ��֡�ļ�ʱ����0, �Ƴٶ���һЭ��������ѯ���ȴ����ܵ���һ������ **/
        	}
        }
		//if(CntMainTimeout >= 18)	/** ��ѯ��Զ��֡(��can���ݼ���can����ʱ������ѯ����Ҫ��ʱ��0) **/
		//if(flag_Roll == 1)
		if(getTimer(2) == 0)		/** ��ѯ��Զ��֡ **/
		{
			setTimer(2, CROLLCYCLE/CTIMER_MS);	/** ��һʱ��ۼ�ʱ��ʼ **/
			
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
		//if(flag_com_rx_finish==1)      //���ȡ����־����λ���ͽ����������Ӵ��ڷ���
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
		bufindex = findBusyIndex();		/** �յ� **/
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
			if(i>=ROOM_NUM)  //���յ��Ĳ��ڱ����
			{
				for(i=0;i<ROOM_NUM;i++)
				{
					if((RoomTable[comSendCoorNum][i][0]==0x00)&&(RoomTable[comSendCoorNum][i][1]==0x00))break;
				}
				if(i>=ROOM_NUM)     //�������,�����Ƚ����ɾ�������˵�ַ�ӵ����һ��
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
		if ((temptt&0x40)==0x40)                     //������״̬�д���ʱ��λ
		{
			WDT_CONTR=0x3b;  //ι�� 1.13s;
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



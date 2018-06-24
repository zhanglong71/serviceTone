//--------------------------------------------------
//PeliCANģʽ����׼֡
//RS232�����ʣ�57600
//CAN�����ʣ�125kbps
//����ID��18��
//--------------------------------------------------
#define MAIN_GLOBALS
#include <includes.h>
#define MAX_BUF_NUM     256
#define COM_RX_NONE     0x00 //������
#define COM_RX_DATA     0x01 //���յ�����
#define COM_RX_DCONFIRM 0x02 //���յ�����ȷ��
#define COM_RX_ASKCHN   0x03 //���յ�����ȷ��

#define	CTIMER_MS	(10)

#define CSEND_BUF_NUM	8	//16	 //����������
#define CSEND_BUF_SIZE	128	//64	 //��������С
//#define INBUF_LEN 10   //���ݳ���
//sfr WDT_CONTR = 0xC1; //Watch-Dog-Timer Control register
unsigned char comSendDataBuf[MAX_BUF_NUM];
unsigned char Can2ComBuf[CSEND_BUF_NUM][CSEND_BUF_SIZE];		//�ݴ�����canbusת����com���ڵ�����(CANbus ==> com)
unsigned char Com2CanBuf[CSEND_BUF_NUM][CSEND_BUF_SIZE];		//�ݴ�����com����ת����CAN������(com ==> CANbus)

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
#define INBUF_LEN 8   //���ݳ���

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
 * ��һ���յ����Ϊ�������ݵĵص� 
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
	/** ������Χ������ **/
	return	0xff;
}

/****************************************************
 * ��һ�������ݵ��׼����������
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

	/** ������Χ������ **/
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
			if((comRxDataBuf[0]==comRxLength-1)&&(comRxDataBuf[comRxLength-1]==0xff))   //֡���Ⱥ�֡������־��ȷ
			{
				if((comRxDataBuf[1]==0x00)&&(comRxDataBuf[6]==0x02)&&(comRxDataBuf[7]==0x00))  //���յ���ѯchn,myadress
				{
					staComRx=COM_RX_ASKCHN;
					flag_com_rx_finish=1;
				}
				if((comRxDataBuf[1]==0x02)&&(comRxDataBuf[4]==0x00)&&(comRxDataBuf[5]==0x00))  //���յ�����
				{
					staComRx=COM_RX_DATA;
					flag_com_rx_finish=1;
				}
				if((comRxDataBuf[1]==0x03)&&(comRxDataBuf[4]==0x00)&&(comRxDataBuf[5]==0x00)&&(comRxDataBuf[6]==0x52))  //���յ�����ȷ��֡
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
	
	if(comSnd_timer) comSnd_timer--;
	if(canSnd_timer) canSnd_timer--;
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
    	//CanRevceData[0]=RxData_Buf[1];
    	//CanRevceData[1]=RxData_Buf[2];
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
    	
    	CntCanRxTimeout = 0; /** CAN���ݽ��ճ�ʱ������0 **/
    	//send_string_com(CanRevceData,CntCanRxLength);
    }
    else                                    //Զ��֡
    {
    	flag_canRxRoll=1;
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
  unsigned char ss;
  unsigned int i;
  unsigned char temptt;
  unsigned char i_buf;
  //unsigned char dstAdress[2];
//_reset_line: 
  i_buf=0;
	i=0;
	CS=1;                //Ƭѡ������
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
  init_serialcomm();     //��ʼ������ 
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
  staComRx=COM_RX_NONE;
  
  //CntComRxTimout=0;
  //flag_ComTxAddr=0;
  flag_CanRxFinish=0;
  flag_CanReset=0;
  setTimer(0, 0/CTIMER_MS);
  setTimer(1, 0/CTIMER_MS);
  
  EA=1; //��ʼ���ɹ��������ж�
  while(1)
  {
   	WDT_CONTR=0x3b;  //ι�� 1.13s;
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
    		case COM_RX_DATA:		//���������, �ȴ���
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
    			comSendDataBuf[4] = Com2CanBuf[i_buf][CSTART_ID + 2];                 //��������ȷ��֡
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
	  			comSendDataBuf[6]=0x02;  //ע��
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

    if(flag_canRxRoll == 1)      //���ȡ����־����λ���Ͳ鿴�Ƿ������ݴ�����������������canbus����
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
	/** ��������com���ڵ�ʱ�䵽 **/
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
    if(flag_CanRxFinish == 1)		/** ���Ƿ����յ�canbus���ݴ�ת����ˢ���� **/
    {
    	flag_CanRxFinish = 0;
    	flag_CanRxHead=0;			/** ��ʲô��? **/
    	#if	0
    		comSendLength=CntCanRxLength;   //comRxLength = comRxDataBuf[0]+1
    		for(i=0;i<MAX_BUF_NUM;i++)comSendDataBuf[i]=CanRevceData[i];
    		send_string_com(comSendDataBuf, comSendLength);
    	#else
    	    /** ��һ���õĻ��壬�������ݴ��� **/	
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
    	/** �յ�������������ȷ�ϣ��������ݼ����� **/
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



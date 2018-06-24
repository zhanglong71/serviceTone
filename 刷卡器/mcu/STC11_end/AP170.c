#define PM50_GLOBALS
#include <includes.H>
//#include <CodeMenu.h>
//void delay(unsigned char us)   //delay time
//{
//  	while(us--);
//}
//void delay1(unsigned char ms)
//{
//  	unsigned char i,j;
//  	for(i=0;i<ms;i++)
//  	for(j=0;j<15;j++)
//  	delay(1);
//}
void player_init(void)
{
		CS_AP170=0;
		SCK_AP170=0;
		DI_AP170=0;
		player_PDN();
		//player_PUP();
		//WR_Byte(PDN1);//µôµçÖ¸Áî
		//WR_Byte(PUP2);//ÊÊºÏCOUT
		//WR_Byte(PDN2);//µôµçÖ¸Áî
}
void player_Num(unsigned char addr)
{
	//if(addr>=56)
	//	delay1(2000);
	unsigned char i;
	player_stop=0;
  while(player_busy()==1)
  {
  	WDT_CONTR=0x3d;  //Î¹¹· 1.13s;
  	i=0x03;

  }
	EA = 0;
	BUSY_AP170=1;
	_nop_();
	_nop_();
	_nop_();
	player_wait_time=0;
	player_retry_cnt=10;
	while((player_stop==0)&&(BUSY_AP170==0)&&(player_retry_cnt>0))
	{
		//i=0x04;
    //UART_SendData(&i,1);
		WDT_CONTR=0x3d;  //Î¹¹· 1.13s;
		if(player_wait_time==0)
		{
			player_Cut=0;
			player_Cut_stop=1;
			player_PUP();
			CS_AP170=1;
			delay(350);
			WR_Byte(PLAY);
			WR_Byte(addr);
			CS_AP170=0;
			delay1(30);
			player_Cut_stop=0;
			player_Cut=0;
			player_wait_time=2;
			player_retry_cnt--;
		}
	}
	EA = 1;
	//if(addr>=56)
	//	delay1(6000);
}
void WR_Byte(unsigned char byte1)
{
	char i;
	EA=0;
	for(i=7;i>=0;i--)
	{
		DI_AP170=(bit)((byte1<<i)&0x80);
		delay(20);
		SCK_AP170=1;
		delay(30);
		SCK_AP170=0;
		delay(10);
	}
	DI_AP170=0;
	delay(20);
	SCK_AP170=0;
	EA=1;
}
unsigned char player_busy(void)//µÈ´ýÓïÒôÐ¾Æ¬±äÏÐ
{
	BUSY_AP170=1;
	_nop_();
	_nop_();
	_nop_();
	if(BUSY_AP170==0)
	{
		delay1(100);
		if(BUSY_AP170==0)
			return (0);
	}
	return (1);
}
void player_PUP(void)
{
	CS_AP170=1;
	delay(350);
	WR_Byte(PUP1);//ÉÏµç£¬ÊÊºÏVOUT
	CS_AP170=0;
	delay1(10);
}
void player_PDN(void)
{
	CS_AP170=1;
	delay(350);
	WR_Byte(PDN1);//µôµçÖ¸Áî
	CS_AP170=0;
	delay1(10);
}
void LCD_Chk(unsigned char *chn,unsigned char NumLine,unsigned char chn_size,unsigned char NUM)
{
	if(LcdLeftCnt==0)
	{
		if((NUM==0)||(NUM==1))
			chn_left_disp1(chn,NumLine,chn_size,LCD_disp_starting,NUM);
		else
			chn_left_disp(chn,NumLine,chn_size,LCD_disp_starting);
		LCD_disp_starting++;
		LCD_disp_starting++;
		LcddispCnt++;
		if(LCD_disp_starting>=(chn_size+4))
			LCD_disp_starting=0;
		LcdLeftCnt=LCD_LEFT_CNT;
	}
}
void player_Time(unsigned char *p,unsigned char *chn1,unsigned char NumLine1,unsigned char chn_size1,unsigned char NUM)
{
	if(player_stop==0)
	{
		if(p[0]!=48)
		{
			while(player_busy()==1)
			{
				LCD_Chk(chn1,NumLine1,chn_size1,NUM);
				WDT_CONTR=0x3d;  //Î¹¹· 1.13s;
			}
			player_Num(p[0]-48);
			while(player_busy()==1)
			{
				LCD_Chk(chn1,NumLine1,chn_size1,NUM);
				WDT_CONTR=0x3d;  //Î¹¹· 1.13s;
			}
			player_Num(48);
		}
		if(p[1]!=48)
		{
			while(player_busy()==1)
			{
				LCD_Chk(chn1,NumLine1,chn_size1,NUM);
				WDT_CONTR=0x3d;  //Î¹¹· 1.13s;
			}
			player_Num(p[1]-48);
		}
		else
		{
			if(p[0]==48)
			{
				while(player_busy()==1)
				{
					LCD_Chk(chn1,NumLine1,chn_size1,NUM);
					WDT_CONTR=0x3d;  //Î¹¹· 1.13s;
				}
				player_Num(p[1]-48);
			}
		}
		while(player_busy()==1)
		{
			LCD_Chk(chn1,NumLine1,chn_size1,NUM);
			WDT_CONTR=0x3d;  //Î¹¹· 1.13s;
		}
		player_Num(46);
		if(p[2]!=48)
		{
			while(player_busy()==1)
			{
				LCD_Chk(chn1,NumLine1,chn_size1,NUM);
				WDT_CONTR=0x3d;  //Î¹¹· 1.13s;
			}
			player_Num(p[2]-48);
			while(player_busy()==1)
			{
				LCD_Chk(chn1,NumLine1,chn_size1,NUM);
				WDT_CONTR=0x3d;  //Î¹¹· 1.13s;
			}
			player_Num(48);
		}
		if(p[3]!=48)
		{
			if(p[2]==48)
			{
				while(player_busy()==1)
				{
					LCD_Chk(chn1,NumLine1,chn_size1,NUM);
					WDT_CONTR=0x3d;  //Î¹¹· 1.13s;
				}
				player_Num(p[2]-48);
			}
			while(player_busy()==1)
			{
				LCD_Chk(chn1,NumLine1,chn_size1,NUM);
				WDT_CONTR=0x3d;  //Î¹¹· 1.13s;
			}
			player_Num(p[3]-48);
			while(player_busy()==1)
			{
				LCD_Chk(chn1,NumLine1,chn_size1,NUM);
				WDT_CONTR=0x3d;  //Î¹¹· 1.13s;
			}
			player_Num(47);
		}
		else
		{
			if(p[2]!=48)
			{
				while(player_busy()==1)
				{
					LCD_Chk(chn1,NumLine1,chn_size1,NUM);
					WDT_CONTR=0x3d;  //Î¹¹· 1.13s;
				}
				player_Num(47);
			}
		}
	}
}
void player_Sev(unsigned char *p,unsigned char len,unsigned char *chn1,unsigned char NumLine1,unsigned char chn_size1,unsigned char NUM1)
{
	if(player_stop==0)
	{
		unsigned char j;
		j=0;
		switch(len)
		{
			case 6:
				while(player_busy()==1)
				{
					LCD_Chk(chn1,NumLine1,chn_size1,NUM1);
					WDT_CONTR=0x3d;  //Î¹¹· 1.13s;
				}
				player_Num(p[j]-48);
				j++;
				while(player_busy()==1)
				{
					LCD_Chk(chn1,NumLine1,chn_size1,NUM1);
					WDT_CONTR=0x3d;  //Î¹¹· 1.13s;
				}
				player_Num(48);
				if(p[j]!=48)
				{
					while(player_busy()==1)
					{
						LCD_Chk(chn1,NumLine1,chn_size1,NUM1);
						WDT_CONTR=0x3d;  //Î¹¹· 1.13s;
					}
					player_Num(p[j]-48);
				}
				j++;
				while(player_busy()==1)
				{
					LCD_Chk(chn1,NumLine1,chn_size1,NUM1);
					WDT_CONTR=0x3d;  //Î¹¹· 1.13s;
				}
				player_Num(49);
				if(p[j]!=48)
				{
					while(player_busy()==1)
					{
						LCD_Chk(chn1,NumLine1,chn_size1,NUM1);
						WDT_CONTR=0x3d;  //Î¹¹· 1.13s;
					}
					player_Num(p[j]-48);
					while(player_busy()==1)
					{
						LCD_Chk(chn1,NumLine1,chn_size1,NUM1);
						WDT_CONTR=0x3d;  //Î¹¹· 1.13s;
					}
					player_Num(50);
				}
				j++;
				if(p[j]!=48)
				{
					if(p[j-1]==48)
					{
						while(player_busy()==1)
						{
							LCD_Chk(chn1,NumLine1,chn_size1,NUM1);
							WDT_CONTR=0x3d;  //Î¹¹· 1.13s;
						}
						player_Num(0);
					}
					while(player_busy()==1)
					{
						LCD_Chk(chn1,NumLine1,chn_size1,NUM1);
						WDT_CONTR=0x3d;  //Î¹¹· 1.13s;
					}
					player_Num(p[j]-48);
					while(player_busy()==1)
					{
						LCD_Chk(chn1,NumLine1,chn_size1,NUM1);
						WDT_CONTR=0x3d;  //Î¹¹· 1.13s;
					}
					player_Num(51);
				}
				j++;
				if(p[j]!=48)
				{
					if(p[j-1]==48)
					{
						while(player_busy()==1)
						{
							LCD_Chk(chn1,NumLine1,chn_size1,NUM1);
							WDT_CONTR=0x3d;  //Î¹¹· 1.13s;
						}
						player_Num(0);
					}
					while(player_busy()==1)
					{
						LCD_Chk(chn1,NumLine1,chn_size1,NUM1);
						WDT_CONTR=0x3d;  //Î¹¹· 1.13s;
					}
					player_Num(p[j]-48);
					while(player_busy()==1)
					{
						LCD_Chk(chn1,NumLine1,chn_size1,NUM1);
						WDT_CONTR=0x3d;  //Î¹¹· 1.13s;
					}
					player_Num(48);
				}
				j++;
				if(p[j]!=48)
				{
					if(p[j-1]==48)
					{
						while(player_busy()==1)
						{
							LCD_Chk(chn1,NumLine1,chn_size1,NUM1);
							WDT_CONTR=0x3d;  //Î¹¹· 1.13s;
						}
						player_Num(0);
					}
					while(player_busy()==1)
					{
						LCD_Chk(chn1,NumLine1,chn_size1,NUM1);
						WDT_CONTR=0x3d;  //Î¹¹· 1.13s;
					}
					player_Num(p[j]-48);
				}
				break;
			case 5:
				while(player_busy()==1)
				{
					LCD_Chk(chn1,NumLine1,chn_size1,NUM1);
					WDT_CONTR=0x3d;  //Î¹¹· 1.13s;
				}
				player_Num(p[j]-48);
				j++;
				while(player_busy()==1)
				{
					LCD_Chk(chn1,NumLine1,chn_size1,NUM1);
					WDT_CONTR=0x3d;  //Î¹¹· 1.13s;
				}
				player_Num(49);
				if(p[j]!=48)
				{
					while(player_busy()==1)
					{
						LCD_Chk(chn1,NumLine1,chn_size1,NUM1);
						WDT_CONTR=0x3d;  //Î¹¹· 1.13s;
					}
					player_Num(p[j]-48);
					while(player_busy()==1)
					{
						LCD_Chk(chn1,NumLine1,chn_size1,NUM1);
						WDT_CONTR=0x3d;  //Î¹¹· 1.13s;
					}
					player_Num(50);
				}
				j++;
				if(p[j]!=48)
				{
					if(p[j-1]==48)
					{
						while(player_busy()==1)
						{
							LCD_Chk(chn1,NumLine1,chn_size1,NUM1);
							WDT_CONTR=0x3d;  //Î¹¹· 1.13s;
						}
						player_Num(0);
					}
					while(player_busy()==1)
					{
						LCD_Chk(chn1,NumLine1,chn_size1,NUM1);
						WDT_CONTR=0x3d;  //Î¹¹· 1.13s;
					}
					player_Num(p[j]-48);
					while(player_busy()==1)
					{
						LCD_Chk(chn1,NumLine1,chn_size1,NUM1);
						WDT_CONTR=0x3d;  //Î¹¹· 1.13s;
					}
					player_Num(51);
				}
				j++;
				if(p[j]!=48)
				{
					if(p[j-1]==48)
					{
						while(player_busy()==1)
						{
							LCD_Chk(chn1,NumLine1,chn_size1,NUM1);
							WDT_CONTR=0x3d;  //Î¹¹· 1.13s;
						}
						player_Num(0);
					}
					while(player_busy()==1)
					{
						LCD_Chk(chn1,NumLine1,chn_size1,NUM1);
						WDT_CONTR=0x3d;  //Î¹¹· 1.13s;
					}
					player_Num(p[j]-48);
					while(player_busy()==1)
					{
						LCD_Chk(chn1,NumLine1,chn_size1,NUM1);
						WDT_CONTR=0x3d;  //Î¹¹· 1.13s;
					}
					player_Num(48);
				}
				j++;
				if(p[j]!=48)
				{
					if(p[j-1]==48)
					{
						while(player_busy()==1)
						{
							LCD_Chk(chn1,NumLine1,chn_size1,NUM1);
							WDT_CONTR=0x3d;  //Î¹¹· 1.13s;
						}
						player_Num(0);
					}
					while(player_busy()==1)
					{
						LCD_Chk(chn1,NumLine1,chn_size1,NUM1);
						WDT_CONTR=0x3d;  //Î¹¹· 1.13s;
					}
					player_Num(p[j]-48);
				}
				break;
			case 4:
				while(player_busy()==1)
				{
					LCD_Chk(chn1,NumLine1,chn_size1,NUM1);
					WDT_CONTR=0x3d;  //Î¹¹· 1.13s;
				}
				player_Num(p[j]-48);
				j++;
				while(player_busy()==1)
				{
					LCD_Chk(chn1,NumLine1,chn_size1,NUM1);
					WDT_CONTR=0x3d;  //Î¹¹· 1.13s;
				}
				player_Num(50);
				if(p[j]!=48)
				{
					while(player_busy()==1)
					{
						LCD_Chk(chn1,NumLine1,chn_size1,NUM1);
						WDT_CONTR=0x3d;  //Î¹¹· 1.13s;
					}
					player_Num(p[j]-48);
					while(player_busy()==1)
					{
						LCD_Chk(chn1,NumLine1,chn_size1,NUM1);
						WDT_CONTR=0x3d;  //Î¹¹· 1.13s;
					}
					player_Num(51);
				}
				j++;
				if(p[j]!=48)
				{
					if(p[j-1]==48)
					{
						while(player_busy()==1)
						{
							LCD_Chk(chn1,NumLine1,chn_size1,NUM1);
							WDT_CONTR=0x3d;  //Î¹¹· 1.13s;
						}
						player_Num(0);
					}
					while(player_busy()==1)
					{
						LCD_Chk(chn1,NumLine1,chn_size1,NUM1);
						WDT_CONTR=0x3d;  //Î¹¹· 1.13s;
					}
					player_Num(p[j]-48);
					while(player_busy()==1)
					{
						LCD_Chk(chn1,NumLine1,chn_size1,NUM1);
						WDT_CONTR=0x3d;  //Î¹¹· 1.13s;
					}
					player_Num(48);
				}
				j++;
				if(p[j]!=48)
				{
					if(p[j-1]==48)
					{
						while(player_busy()==1)
						{
							LCD_Chk(chn1,NumLine1,chn_size1,NUM1);
							WDT_CONTR=0x3d;  //Î¹¹· 1.13s;
						}
						player_Num(0);
					}
					while(player_busy()==1)
					{
						LCD_Chk(chn1,NumLine1,chn_size1,NUM1);
						WDT_CONTR=0x3d;  //Î¹¹· 1.13s;
					}
					player_Num(p[j]-48);
				}
				break;
			case 3:
				while(player_busy()==1)
				{
					LCD_Chk(chn1,NumLine1,chn_size1,NUM1);
					WDT_CONTR=0x3d;  //Î¹¹· 1.13s;
				}
				player_Num(p[j]-48);
				j++;
				while(player_busy()==1)
				{
					LCD_Chk(chn1,NumLine1,chn_size1,NUM1);
					WDT_CONTR=0x3d;  //Î¹¹· 1.13s;
				}
				player_Num(51);
				if(p[j]!=48)
				{
					while(player_busy()==1)
					{
						LCD_Chk(chn1,NumLine1,chn_size1,NUM1);
						WDT_CONTR=0x3d;  //Î¹¹· 1.13s;
					}
					player_Num(p[j]-48);
					while(player_busy()==1)
					{
						LCD_Chk(chn1,NumLine1,chn_size1,NUM1);
						WDT_CONTR=0x3d;  //Î¹¹· 1.13s;
					}
					player_Num(48);
				}
				j++;
				if(p[j]!=48)
				{
					if(p[j-1]==48)
					{
						while(player_busy()==1)
						{
							LCD_Chk(chn1,NumLine1,chn_size1,NUM1);
							WDT_CONTR=0x3d;  //Î¹¹· 1.13s;
						}
						player_Num(0);
					}
					while(player_busy()==1)
					{
						LCD_Chk(chn1,NumLine1,chn_size1,NUM1);
						WDT_CONTR=0x3d;  //Î¹¹· 1.13s;
					}
					player_Num(p[j]-48);
				}
				break;
			case 2:
				while(player_busy()==1)
				{
					LCD_Chk(chn1,NumLine1,chn_size1,NUM1);
					WDT_CONTR=0x3d;  //Î¹¹· 1.13s;
				}
				player_Num(p[j]-48);
				j++;
				while(player_busy()==1)
				{
					LCD_Chk(chn1,NumLine1,chn_size1,NUM1);
					WDT_CONTR=0x3d;  //Î¹¹· 1.13s;
				}
				player_Num(48);
				if(p[j]!=48)
				{
					while(player_busy()==1)
					{
						LCD_Chk(chn1,NumLine1,chn_size1,NUM1);
						WDT_CONTR=0x3d;  //Î¹¹· 1.13s;
					}
					player_Num(p[j]-48);
				}
				break;
			case 1:
				while(player_busy()==1)
				{
					LCD_Chk(chn1,NumLine1,chn_size1,NUM1);
					WDT_CONTR=0x3d;  //Î¹¹· 1.13s;
				}
				player_Num(p[j]-48);
				break;
			default:
				break;
		}
	}
}
void player_INT(unsigned char addr)//´Ëº¯ÊýÎªÁË·ÀÖ¹±àÒëÊ±£¬³öÏÖ¾¯¸æ
{
	unsigned char byte1;
	char i;
	//if(BUSY_AP170==0);
	//{
	//	delay1_INT(100);
	//	if(BUSY_AP170==0)
	//	{
	player_Cut=0;
	player_Cut_stop=1;
	player_PUP_INT();
	CS_AP170=1;
	delay_INT(350);
	byte1=PLAY;
	for(i=7;i>=0;i--)
	{
		DI_AP170=(bit)((byte1<<i)&0x80);
		delay_INT(20);
		SCK_AP170=1;
		delay_INT(30);
		SCK_AP170=0;
		delay_INT(10);
	}
	DI_AP170=0;
	delay_INT(20);
	SCK_AP170=0;
	byte1=addr;
	for(i=7;i>=0;i--)
	{
		DI_AP170=(bit)((byte1<<i)&0x80);
		delay_INT(20);
		SCK_AP170=1;
		delay_INT(30);
		SCK_AP170=0;
		delay_INT(10);
	}
	DI_AP170=0;
	delay_INT(20);
	SCK_AP170=0;
	CS_AP170=0;
	delay1_INT(30);
	player_Cut_stop=0;
	player_Cut=0;
	player_stop=1;
	//	}
	//}
}
void delay_INT(unsigned int us)   //delay time
{
  	while(us--)
  	{
  		WDT_CONTR=0x3d;  //Î¹¹· 1.13s;
  	}
}
void delay1_INT(unsigned int ms)
{
  	INT16U i,j;
  	for(i=0;i<ms;i++)
  	for(j=0;j<15;j++)
  	delay_INT(1);
}
void player_PDN_INT(void)
{
	CS_AP170=1;
	delay_INT(350);
	WR_Byte_INT(PDN1);//µôµçÖ¸Áî
	CS_AP170=0;
	delay1_INT(10);
}
void player_PUP_INT(void)
{
	CS_AP170=1;
	delay_INT(350);
	WR_Byte_INT(PUP1);//ÉÏµç£¬ÊÊºÏVOUT
	CS_AP170=0;
	delay1_INT(10);
}
void WR_Byte_INT(unsigned char byte1)
{
	char i;
	EA=0;
	for(i=7;i>=0;i--)
	{
		DI_AP170=(bit)((byte1<<i)&0x80);
		delay_INT(20);
		SCK_AP170=1;
		delay_INT(30);
		SCK_AP170=0;
		delay_INT(10);
	}
	DI_AP170=0;
	delay_INT(20);
	SCK_AP170=0;
	EA=1;
}
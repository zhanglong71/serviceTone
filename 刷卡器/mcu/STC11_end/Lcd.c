#define LCD_GLOBALS
#include <includes.H>
#include <Codeimg.H>
//******************************************************************************************
//* 函数名称    ：init_lc
//* 功能描述    ： 初始化
//* 参数        ：  参数名称：	输入/输出？	类型		描述
//* 
//* 返回值      ：
//* 作者        ：
//* 创建日期    ：2006-12-19
//* 全局变量    ：
//* 全局静态变量：
//* 局部静态变量：
//*----------------------------------------修改历史------------------------------------------
//* 当前版本    ： 			 修改人：                  修改日期：
//* 修改说明    ：
//******************************************************************************************/
void init_lcd(void)
{
//  	rst=1;
//  	psb=1;
//  	psb=0;
 	wr_lcd(comm,0x30);  /*30---基本指令动作*/   
	wr_lcd(comm,0x03);
  	wr_lcd(comm,0x0c);  /*清屏，地址指针指向00H*/
//  	delay(100);
  	wr_lcd(comm,0x01);  /*光标的移动方向*/
  	wr_lcd(comm,0x06);  /*开显示，关游标*/
//		WriteCommand(0x30);      //8BitMCU,基本指令集合
//    WriteCommand(0x03);      //AC归0,不改变DDRAM内容
//    WriteCommand(0x0C);      //显示ON,游标OFF,游标位反白OFF
//    WriteCommand(0x01);      //清屏,AC归0
//    WriteCommand(0x06);      //写入时,游标右移动
}

/******************************************************************************************
* 函数名称    ：chn_disp
* 功能描述    ： 显示汉字或字符
* 参数        ：  参数名称：	输入/输出？	类型		描述
* 
* 返回值      ：
* 作者        ：
* 创建日期    ：2006-12-19
* 全局变量    ：
* 全局静态变量：
* 局部静态变量：
*----------------------------------------修改历史------------------------------------------
* 当前版本    ： 			 修改人：                  修改日期：
* 修改说明    ：
******************************************************************************************/
void chn_disp(INT8U code *chn,INT8U NumLine)
{
  	INT8U i;
  	wr_lcd(comm,0x30);
  	if(NumLine==0)
  	{
			wr_lcd(comm,0x80);
  		for(i=0;i<16;i++)
  	  {
				delay(1000);
  	  	wr_lcd(dat,chn[i]);
  	  }
  	}
  	else
  	{
		
	  	wr_lcd(comm,0x90);
  		for(i=0;i<16;i++)
  	  {
				delay(1000);
  	  	wr_lcd(dat,chn[i]);
  	  }
  	}
}
void chn_disp_to_left(INT8U *chn,INT8U NumLine)
{
  	INT8U i;
  	wr_lcd(comm,0x30);
  	if(NumLine==0)
  	{
			wr_lcd(comm,0x80);
  		for(i=0;i<16;i++)
  	  {
				delay(1000);
  	  	wr_lcd(dat,chn[i]);
  	  }
  	}
  	else
  	{
		
	  	wr_lcd(comm,0x90);
  		for(i=0;i<16;i++)
  	  {
				delay(1000);
  	  	wr_lcd(dat,chn[i]);
  	  }
  	}
}
void chn_left_disp(INT8U *chn,INT8U NumLine,INT8U chn_size,INT8U display_pos)
{
	INT8U i;
	INT8U chn_buf[16];
	if(chn_size>15)
	{
		for(i=0;i<16;i++)
		{
			if(display_pos>=chn_size)
			{
				chn_buf[i]=' ';
				display_pos++;
				if(display_pos>=(chn_size+4))
					display_pos=0;
			}
			else
			{
				chn_buf[i]=chn[display_pos];
				display_pos++;
			}
		}
		chn_disp_to_left(chn_buf,NumLine);
	}
	else
	{
		chn_disp_to_left(chn,NumLine);
	}
}
void chn_left_disp1(INT8U *chn,INT8U NumLine,INT8U chn_size,INT8U display_pos,INT8U Num)
{
	INT8U i,x;//这里加个x是为了绘制“积”这个字
	INT8U chn_buf[16];
	x=0xFF;
	if(chn_size>15)
	{
		for(i=0;i<16;i++)
		{
			if(display_pos==0)
			{
				if(i==0)
					x=i;
				else
				{
					x=i-1;
					x=x/2+1;
				}
			}
			if(display_pos>=chn_size)
			{
				chn_buf[i]=' ';
				display_pos++;
				if(display_pos>=(chn_size+4))
					display_pos=0;
			}
			else
			{
				chn_buf[i]=chn[display_pos];
				display_pos++;
			}
		}
		if(x!=0xFF)
		{
			if(Num==1)
			{
				img_disp(BmpNum5,1,(0x90+x));
				LCD_disp_img_flag=1;
			}
			if(Num==0)
			{
				img_disp(BmpNum4,1,(0x90+x));
				LCD_disp_img_flag=1;
			}
		}
		else
		{
			if(LCD_disp_img_flag==1)
			{
				img_disp(BmpNull1,1,0x90);
				LCD_disp_img_flag=0;
			}	
		}
		chn_disp_to_left(chn_buf,NumLine);
	}
	else
	{
		chn_disp_to_left(chn,NumLine);
	}
}

void Lcd_overtime(INT8U lov,INT8U disline)
{
	INT8U i,j;
	CntLcdOn=0;
	if(lov>=99)lov=99;
	i=lov/10+48;
	j=lov%10+48;
	if(disline==0)
	{
		wr_lcd(comm,0x30);
	  wr_lcd(comm,0x86);
	  if(i==48)
	  {
	  	wr_lcd(dat,' ');
	  }
	  else
	  {
	  	wr_lcd(dat,i);
	  }
	  wr_lcd(dat,j);
	}
	else
	{
		wr_lcd(comm,0x30);
	  wr_lcd(comm,0x96);
	  if(i==48)
	  {
	  	wr_lcd(dat,' ');
	  }
	  else
	  {
	  	wr_lcd(dat,i);
	  }
	  wr_lcd(dat,j);
	}
}
/******************************************************************************************
* 函数名称    ：img_disp
* 功能描述    ： 显示图形
* 参数        ：  参数名称：	输入/输出？	类型		描述
* 
* 返回值      ：
* 作者        ：
* 创建日期    ：2006-12-19
* 全局变量    ：
* 全局静态变量：
* 局部静态变量：
*----------------------------------------修改历史------------------------------------------
* 当前版本    ： 			 修改人：                  修改日期：
* 修改说明    ：
******************************************************************************************/
void img_disp(INT8U code *img,INT8U disline,INT8U addr)
{
  	INT8U i,j;
  	j=0;
  	for(j=0;j<32;j++)
  	{
  		for(i=0;i<10;i++)
  	  {
      	wr_lcd(comm,0x34);
      	wr_lcd(comm,0x80+j);
      	wr_lcd(comm,0x80+i);
      	wr_lcd(comm,0x30);
      	wr_lcd(dat,0x00);
      	wr_lcd(dat,0x00);
  	  }
  	}
  	//wr_lcd(comm,0x36);
	if(disline==0)
	{
	  	for(j=0;j<16;j++)
	  	{
	      	wr_lcd(comm,0x34);
	      	wr_lcd(comm,0x80+j);
	      	wr_lcd(comm,addr);
	      	wr_lcd(comm,0x30);
	      	wr_lcd(dat,img[j*2]);
	      	wr_lcd(dat,img[j*2+1]);
	  	}
	}
	if(disline==1)
	{
		for(j=0;j<16;j++)
	  	{
	      	wr_lcd(comm,0x34);
	      	wr_lcd(comm,0x90+j);
	      	wr_lcd(comm,addr);
	      	wr_lcd(comm,0x30);
	      	wr_lcd(dat,img[j*2]);
	      	wr_lcd(dat,img[j*2+1]);
	  	}
	}
  wr_lcd(comm,0x36);
}


/******************************************************************************************
* 函数名称    ：clrram
* 功能描述    ： 清DDRAM
* 参数        ：  参数名称：	输入/输出？	类型		描述
* 
* 返回值      ：
* 作者        ：
* 创建日期    ：2006-12-19
* 全局变量    ：
* 全局静态变量：
* 局部静态变量：
*----------------------------------------修改历史------------------------------------------
* 当前版本    ： 			 修改人：                  修改日期：
* 修改说明    ：
******************************************************************************************/
void clrram(void)
{
  	wr_lcd(comm,0x30);
  	wr_lcd(comm,0x01);
  	delay(180);
}

/******************************************************************************************
* 函数名称    ：wr_lcd
* 功能描述    ： 
* 参数        ：  参数名称：	输入/输出？	类型		描述
* 
* 返回值      ：
* 作者        ：
* 创建日期    ：2006-12-19
* 全局变量    ：
* 全局静态变量：
* 局部静态变量：
*----------------------------------------修改历史------------------------------------------
* 当前版本    ： 			 修改人：                  修改日期：
* 修改说明    ：
******************************************************************************************/
void wr_lcd(INT8U dat_comm,INT8U content)
{
	if(dat_comm==comm)
	{
		WriteCommand(content);
	}
	if(dat_comm==dat)
	{
		WriteData(content);
	}
}
/******************************************************************************************
* 函数名称    ：SendByte
* 功能描述    ：串口发送一个字节
* 参数        ：  参数名称：	输入/输出？	类型		描述
* 
* 返回值      ：
* 作者        ：
* 创建日期    ：2006-12-19
* 全局变量    ：
* 全局静态变量：
* 局部静态变量：
*----------------------------------------修改历史------------------------------------------
* 当前版本    ： 			 修改人：                  修改日期：
* 修改说明    ：
******************************************************************************************/
void SendByte(INT8U Dbyte)
{
     INT8U i;
     for(i=0;i<8;i++)
     {
           SCK = 0;
           Dbyte=Dbyte<<1;      //左移一位
           SID = CY;            //移出的位给SID
           SCK = 1;
           SCK = 0;
     }
}
void CheckBusy( void )
{
     do   SendByte(0xfc);      //11111,RW(1),RS(0),0
     while(0x80&ReceiveByte());      //BF(.7)=1 Busy
}
/******************************************************************************************
* 函数名称    ：WriteCommand
* 功能描述    ：
* 参数        ：  参数名称：	输入/输出？	类型		描述
* 
* 返回值      ：
* 作者        ：
* 创建日期    ：2006-12-19
* 全局变量    ：
* 全局静态变量：
* 局部静态变量：
*----------------------------------------修改历史------------------------------------------
* 当前版本    ： 			 修改人：                  修改日期：
* 修改说明    ：
******************************************************************************************/
void WriteCommand(INT8U Cbyte )
{
     CS = 1;
     CheckBusy();
     SendByte(0xf8);            //11111,RW(0),RS(0),0
     SendByte(0xf0&Cbyte);      //高四位
     SendByte(0xf0&Cbyte<<4);//低四位(先执行<<)
     CS = 0;
}
/******************************************************************************************
* 函数名称    ：WriteData
* 功能描述    ：
* 参数        ：  参数名称：	输入/输出？	类型		描述
* 
* 返回值      ：
* 作者        ：
* 创建日期    ：2006-12-19
* 全局变量    ：
* 全局静态变量：
* 局部静态变量：
*----------------------------------------修改历史------------------------------------------
* 当前版本    ： 			 修改人：                  修改日期：
* 修改说明    ：
******************************************************************************************/
void WriteData(INT8U Dbyte )
{
     CS = 1;
     CheckBusy();
     SendByte(0xfa);            //11111,RW(0),RS(1),0
     SendByte(0xf0&Dbyte);      //高四位
     SendByte(0xf0&Dbyte<<4);//低四位(先执行<<)
     CS = 0;
}
/******************************************************************************************
* 函数名称    ：ReceiveByte
* 功能描述    ：串口接收一个字节
* 参数        ：  参数名称：	输入/输出？	类型		描述
* 仅在读取数据的时候用到
* 而读出的数据是一次只能读出4bit的
* 返回值      ：
* 作者        ：
* 创建日期    ：2006-12-19
* 全局变量    ：
* 全局静态变量：
* 局部静态变量：
*----------------------------------------修改历史------------------------------------------
* 当前版本    ： 			 修改人：                  修改日期：
* 修改说明    ：
******************************************************************************************/
INT8U ReceiveByte(void)
{
     INT8U i,temp1,temp2;
     temp1 = 0;
     temp2 = 0;
     for(i=0;i<8;i++)
     {
           temp1=temp1<<1;
           SCK = 0;
           SCK = 1;            
           SCK = 0;
           if(SID) temp1++;        //先接收高四位
     }
     for(i=0;i<8;i++)
     {
           temp2=temp2<<1;
           SCK = 0;
           SCK = 1;
           SCK = 0;
           if(SID) temp2++;        //后接收低四位
     }
     return ((0xf0&temp1)+(0x0f&temp2));   //
}
/******************************************************************************************
* 函数名称    ：chk_busy
* 功能描述    ： 
* 参数        ：  参数名称：	输入/输出？	类型		描述
* 
* 返回值      ：
* 作者        ：
* 创建日期    ：2006-12-19
* 全局变量    ：
* 全局静态变量：
* 局部静态变量：
*----------------------------------------修改历史------------------------------------------
* 当前版本    ： 			 修改人：                  修改日期：
* 修改说明    ：
******************************************************************************************/
//void chk_busy(void)
//{
//  	P2=0xff;
//  	rs=0;
//  	rw=1;
//  	e =1;
//  	while(busy==1);
//  	e =0;
//}
/******************************************************************************************
* 函数名称    ：
* 功能描述    ： 
* 参数        ：  参数名称：	输入/输出？	类型		描述
* 
* 返回值      ：
* 作者        ：
* 创建日期    ：2006-12-19
* 全局变量    ：
* 全局静态变量：
* 局部静态变量：
*----------------------------------------修改历史------------------------------------------
* 当前版本    ： 			 修改人：                  修改日期：
* 修改说明    ：
******************************************************************************************/
void delay(INT16U us)   //delay time
{
  	while(us--)
  	{
  		WDT_CONTR=0x3d;  //喂狗 1.13s;
  	}
}
/******************************************************************************************
* 函数名称    ：
* 功能描述    ： 
* 参数        ：  参数名称：	输入/输出？	类型		描述
* 
* 返回值      ：
* 作者        ：
* 创建日期    ：2006-12-19
* 全局变量    ：
* 全局静态变量：
* 局部静态变量：
*----------------------------------------修改历史------------------------------------------
* 当前版本    ： 			 修改人：                  修改日期：
* 修改说明    ：
******************************************************************************************/
void delay1(INT16U ms)
{
  	INT16U i,j;
  	for(i=0;i<ms;i++)
  	for(j=0;j<15;j++)
  	delay(1);
}

//******************************************************************************************
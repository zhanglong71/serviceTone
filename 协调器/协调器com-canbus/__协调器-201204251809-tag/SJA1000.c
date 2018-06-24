#define SJA1000_GLOBALS
#include <includes.h>

//void HR_memcpy(unsigned char *p1,unsigned char *p2,unsigned char length1)
//{
//	unsigned char i;
//	for(i=0;i<length1;i++)
//	{
//		*p1=*p2;
//		p1++;
//		p2++;
//	}
//}
//***********************************************************************
/************************************************************************
*函数原型: bit BCAN_SET_OUTCLK( unsigned char Clock_Out)               *
*参数说明:                                                             *
*          Clock_Out:存放时钟分频寄存器(CDR)的参数设置                 *
*返回值:                                                               *
*           0 ;设置成功                                                *
*           1 ;设置失败                                                *
*说明:设置SJA1000的时钟分频 。该子程序只能用于复位模式                      
************************************************************************/
bit BCAN_SET_OUTCLK(unsigned char Clock_Out)
{
  SJA_BCANAdr=REG_CDR;           

  *SJA_BCANAdr=Clock_Out;        
  if(*SJA_BCANAdr != Clock_Out)
     return 1; 
   else 
     return 0;
} 

/************************************************************************
*函数原型: bit BCAN_SET_OBJECT(unsigned char  BCAN_ACR0,BCAN_ACR1,BCAN_ACR2,BCAN_ACR3     
                                              BCAN_AMR0, BCAN_AMR1, BCAN_AMR2, BCAN_AMR3     
*参数说明:                                                              *
*    BCAN_ACR(0-3):存放验收代码寄存器（ACR）的参数设置                  *
*    BCAN_AMR(0-3):存放接收屏蔽寄存器（AMR）的参数设置                  *
*返回值:                                                                *
*           0 ;设置成功                                                 *
*           1 ;设置失败                                                 *
*说明:设置CAN节点的通讯对象，允许接收的报文,是由AMR和ACR共同决定的.     *
*************************************************************************/ 
bit BCAN_SET_OBJECT(unsigned char  BCAN_ACR0,BCAN_ACR1,BCAN_ACR2,BCAN_ACR3,BCAN_AMR0,BCAN_AMR1,BCAN_AMR2,BCAN_AMR3)
{
  SJA_BCANAdr=REG_TXBuffer1;     

  *SJA_BCANAdr=BCAN_ACR0;        //写入参数
   
  SJA_BCANAdr=REG_TXBuffer2;
  *SJA_BCANAdr=BCAN_ACR1;

  SJA_BCANAdr=REG_TXBuffer3;  
  *SJA_BCANAdr=BCAN_ACR2;
   
  SJA_BCANAdr=REG_TXBuffer4; 
  *SJA_BCANAdr=BCAN_ACR3;  
 
  //校验写入值
  if(*SJA_BCANAdr != BCAN_ACR3)  return 1; 
  
  SJA_BCANAdr=REG_TXBuffer5;      

  *SJA_BCANAdr=BCAN_AMR0;     //写入参数
  
  SJA_BCANAdr=REG_TXBuffer6;         
  *SJA_BCANAdr=BCAN_AMR1;

  SJA_BCANAdr=REG_TXBuffer7; 
  *SJA_BCANAdr=BCAN_AMR2;

  SJA_BCANAdr=REG_TXBuffer8; 
  *SJA_BCANAdr=BCAN_AMR3;
  //校验写入值
  if(*SJA_BCANAdr != BCAN_AMR3) return 1; 

  return 0;
} 

/************************************************************************
;*函数原型:  bit BCAN_SET_BANDRATE(unsigned char CAN_ByteRate)          *
;*返回值:                                                               *
;*           0 ;波特率设置成功                                          *
;*           1 ;波特率设置失败                                          *
;*                                                                      * 
;*说明:设置CAN控制器SJA1000通讯波特率.SJA1000的晶振必须为16MHZ,         *
;*     其它晶体的频率的值的波特率，需自己计算 。该子程序只能用于        *
;*     复位模式                                                         *  
;************************************************************************/ 
bit BCAN_SET_BANDRATE(unsigned char CAN_ByteRate)         //波特率选择
{
     unsigned char BR_Num= CAN_ByteRate,BTR0_num,BTR1_num;
       switch (BR_Num)
        {
          case ByteRate_5k:
               BTR0_num=0xBF;
               BTR1_num=0xFF;
               break;
          case ByteRate_10k:
               BTR0_num=0x31;
               BTR1_num=0x1C;
               break;
          case ByteRate_20k:
               BTR0_num=0x18;
               BTR1_num=0x1C;
               break;
          case ByteRate_40k  :
               BTR0_num=0x87;
               BTR1_num=0xff;
               break;
          case ByteRate_50k:
               //BTR0_num=0x47;
               //BTR1_num=0x2f;
               BTR0_num=0x0e;
               BTR1_num=0x1c;
               break;
          case ByteRate_80k  :
               //BTR0_num=0x83;
               //BTR1_num=0xff;
               BTR0_num=0x49;                      //ok
               BTR1_num=0x1b;
               break;
          case ByteRate_100k  :
               //BTR0_num=0x43;
               //BTR1_num=0x2f;
               BTR0_num=0x04;                       //ok
               BTR1_num=0x1C;
               break;
          case ByteRate_125k  :                      //SJA1000的晶振为必须为16MHZ,波特率设置为125kpbs
               BTR0_num=0x03;
               BTR1_num=0x1c;
               //BTR0_num=0x44;                        //ok
               //BTR1_num=0x1f;
               break;
          case ByteRate_200k  ://24MHZ
            //   BTR0_num=0xc5;  //
             //  BTR1_num=0xa5;
               BTR0_num=0x43;                        //ok
               BTR1_num=0x1b;
               break;
          /* case ByteRate_200k  ://24MHZ
               BTR0_num=0x81;
               BTR1_num=0xFA;
               break;*/
          case ByteRate_400k  :
               BTR0_num=0x80;
               BTR1_num=0xfa;
               break;
          case ByteRate_500k  :
               BTR0_num=0x01;
               BTR1_num=0x1c;
               break;
          case ByteRate_800k  :
               BTR0_num=0x00;
               BTR1_num=0x16;
               break;
          case ByteRate_1000k  :
               BTR0_num=0x00;
               BTR1_num=0x14;
               break;
          default :
               return 1;
               break;
        }

    SJA_BCANAdr=REG_BTR0;
    *SJA_BCANAdr=BTR0_num;
    if(*SJA_BCANAdr!=BTR0_num)
      {return 1;}
    SJA_BCANAdr=REG_BTR1;
    *SJA_BCANAdr=BTR1_num;
    if(*SJA_BCANAdr!=BTR1_num)
      {return 1;}
    return 0;
}
/************************************************************************
 *函数原型: bit BCAN_SET_CONTROL(unsigend char CMD)                     *
 *参数说明: 设置控制寄存器                                              *
 ************************************************************************/
//bit BCAN_SET_CONTROL(unsigned char CMD)
//{  unsigned char TempData;
// 
//  SJA_BCANAdr=REG_CONTROL;   //SJA_BaseAdr+0x00  控制寄存器
//  TempData=  *SJA_BCANAdr; 
// 
//  *SJA_BCANAdr=CMD;
// 
//  if (*SJA_BCANAdr == CMD)
//    return 0;
//  else
//    return 1;
//
//}
/************************************************************************
 *函数原型:  bit   BCAN_CREATE_COMMUNATION(void)                        * 
 *参数说明:  无                                                         *
 *返回值:                                                               *
 *           0 ; 表示SJA1000接口正常                                    *
 *           1 ; 表示SJA1000与处理器接口不正常                          *
 *说明:该函数用于检测CAN控制器的接口是否正常                            *
 ************************************************************************/
 bit BCAN_CREATE_COMMUNATION(void)
 {
    SJA_BCANAdr=REG_TEST;      
    *SJA_BCANAdr=0xaa;       //写入测试值
    if(*SJA_BCANAdr == 0xaa)
       return 0;            //读测试正确
    else
       return 1;
     
 }

 /************************************************************************
 *函数原型:      bit   BCAN_ENTER_RETMODEL(void)                        *
 *参数说明:  无                                                         *
 *返回值:                                                               *
 *           0 ; 表示成功进入复位工作模式                               *
 *           1 ; 表示不能进入复位工作模式                               *
 *                                                                      * 
 *说明:      CAN控制器进入复位工作模式                                  *
 ************************************************************************/ 
 bit   BCAN_ENTER_RETMODEL(void)     //置位复位请求
 {
    unsigned   char   TempData;
    SJA_BCANAdr  = REG_CONTROL;   

  TempData=  *SJA_BCANAdr;       
    *SJA_BCANAdr=0x09;                 //置位复位请求 和单滤波模式
    if((*SJA_BCANAdr&0x01) == 1)
     return   0;
    else
      return   1;   
 } 
/************************************************************************
 *函数原型:   BCAN_CMD_PRG(unsigned char cmd)                           *
 *参数说明:  unsigned char cmd                                          *
 *返回值:                                                               *
 *           0 ; 请求成功                                               *
 *           1 ; 请求失败                                               *
 *                                                                      * 
 *说明:      启动命令字                                                 *
 ************************************************************************/
bit  BCAN_CMD_PRG(unsigned char cmd)
 {
   SJA_BCANAdr=REG_COMMAND;            //访问地址指向命令寄存器
   *SJA_BCANAdr=cmd;                   //启动命令字
 
   switch(cmd)
   {    case  TR_CMD:                    

           return    0;
           break;

  case  SRR_CMD:      

      return 0;
     break;
    
  case  AT_CMD:                  

            SJA_BCANAdr = REG_STATUS;   //访问地址指向状态寄存器   
           if((*SJA_BCANAdr & 0x20)==0) //判断是否正在发送
             return  0;
           else
             return  1;              
           break; 
     case  RRB_CMD:                   
           SJA_BCANAdr = REG_STATUS;   //访问地址指向状态寄存器   
           if((*SJA_BCANAdr & 0x01)==1)
              return  1;
           else           
              return  0;               
           break;  
     case  COS_CMD:                  

           SJA_BCANAdr = REG_STATUS;   
           if((*SJA_BCANAdr & 0x02)==0)//判断清除超载是否成功
             return  0; 
           else
             return  1;             
           break; 
     default:
             return  1;
             break; 
   }
}

//unsigned char BCAN_DATA_WRITE(unsigned char *SendDataBuf)
// {  unsigned char temp;
//   
//    SJA_BCANAdr = REG_STATUS;    
//    temp=*SJA_BCANAdr;
//
//    if ((temp&0x08)==0) return  1;    //上次发送未完成
//    if ((temp&0x04)==0) return  2;    //发送缓冲区是否锁定 
//    if ((temp&0x10)==0x10) return 3;  //判断是否正在接收   
//
//    SJA_BCANAdr = REG_RXBuffer1;      //访问地址指向发送缓冲区1,修改成头文件
//
//    memcpy(SJA_BCANAdr,SendDataBuf,4);   
//    BCAN_CMD_PRG(TR_CMD);             //请求发送         
//    return 0;
//}
//CAN发送任意长度字节
void CAN_Send_Small(unsigned char *CAN_TX_Buf,unsigned char length1,unsigned char packLength,unsigned char packNum)   //小于或者等于8字节数据发送
{
	unsigned char temptt;
	CntCanTxTimeout=0;
	flag_CanTxTimeout=0;
	do
	{
		if(flag_CanTxTimeout)return;
		SJA_BCANAdr = REG_STATUS;
    temptt=*SJA_BCANAdr;
  }while((temptt&TBS_Bit)!=TBS_Bit);               //循环检测等待                       
	//可以向发送缓冲器写数据
    SJA_BCANAdr = REG_TXBuffer1;     //访问地址指向发送缓冲区1,修改成头文件
    *SJA_BCANAdr=length1|0x80;       //扩展帧
    
    SJA_BCANAdr = REG_TXBuffer2;     //ID28-21
    *SJA_BCANAdr=packAcr0;
    SJA_BCANAdr = REG_TXBuffer3;     //ID20-13
    *SJA_BCANAdr=packAcr1;
    SJA_BCANAdr = REG_TXBuffer4;     //ID12-5
    *SJA_BCANAdr=((packLength<<4)&0xf0)+(packNum&0x0f);            //包序列号
    SJA_BCANAdr = REG_TXBuffer5;     //ID12-5
    *SJA_BCANAdr=0xf0;
    
    SJA_BCANAdr = REG_TXBuffer6;
    //*SJA_BCANAdr=PackNum;
    //SJA_BCANAdr = REG_TXBuffer7;
    memcpy(SJA_BCANAdr,CAN_TX_Buf,length1);   
	//数据发送请求
    BCAN_CMD_PRG(TR_CMD);            //请求发送  
}

void CAN_Send_Large(unsigned char *CAN_TX_Buf,unsigned char length1)  //大于8字节发送
{
	unsigned char bufi,i,j;
	unsigned char bufj[8];
	for(i=0;i<8;i++)
	{
		bufj[i]=0;
	}
	i=length1/8;
	j=length1%8;
	if(j==0)
	{
		i--;
		for(bufi=0;bufi<=i;bufi++)
	  {
	  	CAN_Send_Small(&CAN_TX_Buf[8*bufi],8,i,bufi);
	  }
	}
	else
	{
		for(bufi=0;bufi<i;bufi++)CAN_Send_Small(&CAN_TX_Buf[8*bufi],8,i,bufi);
		CAN_Send_Small(&CAN_TX_Buf[8*i],j,i,i);
	}
	//CAN_Send_Small(bufj,1,0xff);
}

/****************************************************
**函数原型：   bit Sja_1000_Init(void)
**功    能：   初始化SJA10000
**入口参数:    无 
**返 回 值:     
      0： 初始化成功 
      1： 复位失败 
      2:  测试sja1000失败 
      3： 设置失败 
      4： 设置验收滤波器失败
      5： 设置波特率失败     
*****************************************************/

unsigned char Sja_1000_Init(unsigned char intMode)
{
 bit s; 
 //EA=0;                                 //关总中断
 //WDT_CONTR=0x3d;  //喂狗 1.13s;
   s=BCAN_ENTER_RETMODEL();
 if (s==1) return 1; 
 if(intMode==1)
 {
    s=BCAN_CREATE_COMMUNATION();       //建立通信
    if (s==1) return 2;
 }
 s=BCAN_SET_OUTCLK(0x88);             //Pelican，关闭clkout
 if (s==1) return 3;

 s=BCAN_SET_OBJECT(key_temp,0x34,0x16,0x00,0x00,0xff,0xff,0xf0);//屏蔽寄存器，都设为无关，接收所有报文 
                                                            //当屏蔽位为1，不滤波，0就滤波必须相等
 //s=BCAN_SET_OBJECT(0x55,0xe0,0xaa,0xa1,0x00,0x00,0xff,0xff);//验收码&屏蔽码

 if (s==1) return 4;

 s=BCAN_SET_BANDRATE(ByteRate_100k);    //设置波特率125K 16MHZ
    if (s==1) return 5;
    
    SJA_BCANAdr=REG_OCR ;                  //输出控制寄存器  
    *SJA_BCANAdr=0x1a;                     // 0001 1010 正常输出模式，TX1悬空，TX0上拉，正常输出模式
    
    SJA_BCANAdr=REG_INTENABLE;      
   *SJA_BCANAdr=0x1D;                  //设置中断，接收和发送中断
    
    // s=BCAN_SET_CONTROL(0x08);
    SJA_BCANAdr=REG_CONTROL;            //退出 复位模式
    *SJA_BCANAdr=*SJA_BCANAdr&0xfe;
     if(*SJA_BCANAdr!=0x00)
     return 6;

 //if (s==1) return 6;

    //EA=1;
 return 0;   
}
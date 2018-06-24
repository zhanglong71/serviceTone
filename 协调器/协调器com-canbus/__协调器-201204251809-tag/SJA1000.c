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
*����ԭ��: bit BCAN_SET_OUTCLK( unsigned char Clock_Out)               *
*����˵��:                                                             *
*          Clock_Out:���ʱ�ӷ�Ƶ�Ĵ���(CDR)�Ĳ�������                 *
*����ֵ:                                                               *
*           0 ;���óɹ�                                                *
*           1 ;����ʧ��                                                *
*˵��:����SJA1000��ʱ�ӷ�Ƶ �����ӳ���ֻ�����ڸ�λģʽ                      
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
*����ԭ��: bit BCAN_SET_OBJECT(unsigned char  BCAN_ACR0,BCAN_ACR1,BCAN_ACR2,BCAN_ACR3     
                                              BCAN_AMR0, BCAN_AMR1, BCAN_AMR2, BCAN_AMR3     
*����˵��:                                                              *
*    BCAN_ACR(0-3):������մ���Ĵ�����ACR���Ĳ�������                  *
*    BCAN_AMR(0-3):��Ž������μĴ�����AMR���Ĳ�������                  *
*����ֵ:                                                                *
*           0 ;���óɹ�                                                 *
*           1 ;����ʧ��                                                 *
*˵��:����CAN�ڵ��ͨѶ����������յı���,����AMR��ACR��ͬ������.     *
*************************************************************************/ 
bit BCAN_SET_OBJECT(unsigned char  BCAN_ACR0,BCAN_ACR1,BCAN_ACR2,BCAN_ACR3,BCAN_AMR0,BCAN_AMR1,BCAN_AMR2,BCAN_AMR3)
{
  SJA_BCANAdr=REG_TXBuffer1;     

  *SJA_BCANAdr=BCAN_ACR0;        //д�����
   
  SJA_BCANAdr=REG_TXBuffer2;
  *SJA_BCANAdr=BCAN_ACR1;

  SJA_BCANAdr=REG_TXBuffer3;  
  *SJA_BCANAdr=BCAN_ACR2;
   
  SJA_BCANAdr=REG_TXBuffer4; 
  *SJA_BCANAdr=BCAN_ACR3;  
 
  //У��д��ֵ
  if(*SJA_BCANAdr != BCAN_ACR3)  return 1; 
  
  SJA_BCANAdr=REG_TXBuffer5;      

  *SJA_BCANAdr=BCAN_AMR0;     //д�����
  
  SJA_BCANAdr=REG_TXBuffer6;         
  *SJA_BCANAdr=BCAN_AMR1;

  SJA_BCANAdr=REG_TXBuffer7; 
  *SJA_BCANAdr=BCAN_AMR2;

  SJA_BCANAdr=REG_TXBuffer8; 
  *SJA_BCANAdr=BCAN_AMR3;
  //У��д��ֵ
  if(*SJA_BCANAdr != BCAN_AMR3) return 1; 

  return 0;
} 

/************************************************************************
;*����ԭ��:  bit BCAN_SET_BANDRATE(unsigned char CAN_ByteRate)          *
;*����ֵ:                                                               *
;*           0 ;���������óɹ�                                          *
;*           1 ;����������ʧ��                                          *
;*                                                                      * 
;*˵��:����CAN������SJA1000ͨѶ������.SJA1000�ľ������Ϊ16MHZ,         *
;*     ���������Ƶ�ʵ�ֵ�Ĳ����ʣ����Լ����� �����ӳ���ֻ������        *
;*     ��λģʽ                                                         *  
;************************************************************************/ 
bit BCAN_SET_BANDRATE(unsigned char CAN_ByteRate)         //������ѡ��
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
          case ByteRate_125k  :                      //SJA1000�ľ���Ϊ����Ϊ16MHZ,����������Ϊ125kpbs
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
 *����ԭ��: bit BCAN_SET_CONTROL(unsigend char CMD)                     *
 *����˵��: ���ÿ��ƼĴ���                                              *
 ************************************************************************/
//bit BCAN_SET_CONTROL(unsigned char CMD)
//{  unsigned char TempData;
// 
//  SJA_BCANAdr=REG_CONTROL;   //SJA_BaseAdr+0x00  ���ƼĴ���
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
 *����ԭ��:  bit   BCAN_CREATE_COMMUNATION(void)                        * 
 *����˵��:  ��                                                         *
 *����ֵ:                                                               *
 *           0 ; ��ʾSJA1000�ӿ�����                                    *
 *           1 ; ��ʾSJA1000�봦�����ӿڲ�����                          *
 *˵��:�ú������ڼ��CAN�������Ľӿ��Ƿ�����                            *
 ************************************************************************/
 bit BCAN_CREATE_COMMUNATION(void)
 {
    SJA_BCANAdr=REG_TEST;      
    *SJA_BCANAdr=0xaa;       //д�����ֵ
    if(*SJA_BCANAdr == 0xaa)
       return 0;            //��������ȷ
    else
       return 1;
     
 }

 /************************************************************************
 *����ԭ��:      bit   BCAN_ENTER_RETMODEL(void)                        *
 *����˵��:  ��                                                         *
 *����ֵ:                                                               *
 *           0 ; ��ʾ�ɹ����븴λ����ģʽ                               *
 *           1 ; ��ʾ���ܽ��븴λ����ģʽ                               *
 *                                                                      * 
 *˵��:      CAN���������븴λ����ģʽ                                  *
 ************************************************************************/ 
 bit   BCAN_ENTER_RETMODEL(void)     //��λ��λ����
 {
    unsigned   char   TempData;
    SJA_BCANAdr  = REG_CONTROL;   

  TempData=  *SJA_BCANAdr;       
    *SJA_BCANAdr=0x09;                 //��λ��λ���� �͵��˲�ģʽ
    if((*SJA_BCANAdr&0x01) == 1)
     return   0;
    else
      return   1;   
 } 
/************************************************************************
 *����ԭ��:   BCAN_CMD_PRG(unsigned char cmd)                           *
 *����˵��:  unsigned char cmd                                          *
 *����ֵ:                                                               *
 *           0 ; ����ɹ�                                               *
 *           1 ; ����ʧ��                                               *
 *                                                                      * 
 *˵��:      ����������                                                 *
 ************************************************************************/
bit  BCAN_CMD_PRG(unsigned char cmd)
 {
   SJA_BCANAdr=REG_COMMAND;            //���ʵ�ַָ������Ĵ���
   *SJA_BCANAdr=cmd;                   //����������
 
   switch(cmd)
   {    case  TR_CMD:                    

           return    0;
           break;

  case  SRR_CMD:      

      return 0;
     break;
    
  case  AT_CMD:                  

            SJA_BCANAdr = REG_STATUS;   //���ʵ�ַָ��״̬�Ĵ���   
           if((*SJA_BCANAdr & 0x20)==0) //�ж��Ƿ����ڷ���
             return  0;
           else
             return  1;              
           break; 
     case  RRB_CMD:                   
           SJA_BCANAdr = REG_STATUS;   //���ʵ�ַָ��״̬�Ĵ���   
           if((*SJA_BCANAdr & 0x01)==1)
              return  1;
           else           
              return  0;               
           break;  
     case  COS_CMD:                  

           SJA_BCANAdr = REG_STATUS;   
           if((*SJA_BCANAdr & 0x02)==0)//�ж���������Ƿ�ɹ�
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
//    if ((temp&0x08)==0) return  1;    //�ϴη���δ���
//    if ((temp&0x04)==0) return  2;    //���ͻ������Ƿ����� 
//    if ((temp&0x10)==0x10) return 3;  //�ж��Ƿ����ڽ���   
//
//    SJA_BCANAdr = REG_RXBuffer1;      //���ʵ�ַָ���ͻ�����1,�޸ĳ�ͷ�ļ�
//
//    memcpy(SJA_BCANAdr,SendDataBuf,4);   
//    BCAN_CMD_PRG(TR_CMD);             //������         
//    return 0;
//}
//CAN�������ⳤ���ֽ�
void CAN_Send_Small(unsigned char *CAN_TX_Buf,unsigned char length1,unsigned char packLength,unsigned char packNum)   //С�ڻ��ߵ���8�ֽ����ݷ���
{
	unsigned char temptt;
	CntCanTxTimeout=0;
	flag_CanTxTimeout=0;
	do
	{
		if(flag_CanTxTimeout)return;
		SJA_BCANAdr = REG_STATUS;
    temptt=*SJA_BCANAdr;
  }while((temptt&TBS_Bit)!=TBS_Bit);               //ѭ�����ȴ�                       
	//�������ͻ�����д����
    SJA_BCANAdr = REG_TXBuffer1;     //���ʵ�ַָ���ͻ�����1,�޸ĳ�ͷ�ļ�
    *SJA_BCANAdr=length1|0x80;       //��չ֡
    
    SJA_BCANAdr = REG_TXBuffer2;     //ID28-21
    *SJA_BCANAdr=packAcr0;
    SJA_BCANAdr = REG_TXBuffer3;     //ID20-13
    *SJA_BCANAdr=packAcr1;
    SJA_BCANAdr = REG_TXBuffer4;     //ID12-5
    *SJA_BCANAdr=((packLength<<4)&0xf0)+(packNum&0x0f);            //�����к�
    SJA_BCANAdr = REG_TXBuffer5;     //ID12-5
    *SJA_BCANAdr=0xf0;
    
    SJA_BCANAdr = REG_TXBuffer6;
    //*SJA_BCANAdr=PackNum;
    //SJA_BCANAdr = REG_TXBuffer7;
    memcpy(SJA_BCANAdr,CAN_TX_Buf,length1);   
	//���ݷ�������
    BCAN_CMD_PRG(TR_CMD);            //������  
}

void CAN_Send_Large(unsigned char *CAN_TX_Buf,unsigned char length1)  //����8�ֽڷ���
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
**����ԭ�ͣ�   bit Sja_1000_Init(void)
**��    �ܣ�   ��ʼ��SJA10000
**��ڲ���:    �� 
**�� �� ֵ:     
      0�� ��ʼ���ɹ� 
      1�� ��λʧ�� 
      2:  ����sja1000ʧ�� 
      3�� ����ʧ�� 
      4�� ���������˲���ʧ��
      5�� ���ò�����ʧ��     
*****************************************************/

unsigned char Sja_1000_Init(unsigned char intMode)
{
 bit s; 
 //EA=0;                                 //�����ж�
 //WDT_CONTR=0x3d;  //ι�� 1.13s;
   s=BCAN_ENTER_RETMODEL();
 if (s==1) return 1; 
 if(intMode==1)
 {
    s=BCAN_CREATE_COMMUNATION();       //����ͨ��
    if (s==1) return 2;
 }
 s=BCAN_SET_OUTCLK(0x88);             //Pelican���ر�clkout
 if (s==1) return 3;

 s=BCAN_SET_OBJECT(key_temp,0x34,0x16,0x00,0x00,0xff,0xff,0xf0);//���μĴ���������Ϊ�޹أ��������б��� 
                                                            //������λΪ1�����˲���0���˲��������
 //s=BCAN_SET_OBJECT(0x55,0xe0,0xaa,0xa1,0x00,0x00,0xff,0xff);//������&������

 if (s==1) return 4;

 s=BCAN_SET_BANDRATE(ByteRate_100k);    //���ò�����125K 16MHZ
    if (s==1) return 5;
    
    SJA_BCANAdr=REG_OCR ;                  //������ƼĴ���  
    *SJA_BCANAdr=0x1a;                     // 0001 1010 �������ģʽ��TX1���գ�TX0�������������ģʽ
    
    SJA_BCANAdr=REG_INTENABLE;      
   *SJA_BCANAdr=0x1D;                  //�����жϣ����պͷ����ж�
    
    // s=BCAN_SET_CONTROL(0x08);
    SJA_BCANAdr=REG_CONTROL;            //�˳� ��λģʽ
    *SJA_BCANAdr=*SJA_BCANAdr&0xfe;
     if(*SJA_BCANAdr!=0x00)
     return 6;

 //if (s==1) return 6;

    //EA=1;
 return 0;   
}
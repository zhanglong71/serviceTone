#define WIFII_GLOBALS
#include <includes.H>
#include <CodeCrc.h>

INT8U comTxDataConfirm(INT8U *p)
{
	INT8U j;
	INT16U i_16;
	INT8U err;
	
	for(j=0;j<4;j++)
  {
  	i_16=(TH0<<8)|TL0;
  	srand(i_16);
  	i_16=rand();
  	i_16%=1000;
  	comSenData(p[0]+1,p);
  	OSSemPend(&comRxConfirmRdy,3000+i_16,&err);
  	if((err==OS_NO_ERR)&&(staComRx==COM_RX_DCONFIRM))break;
  }
  if((staComRx==COM_RX_DCONFIRM)&&(err==OS_NO_ERR))
  {
  	staComRx=COM_RX_NONE;
  	flag_com_confirm_finish=0;
    return(COM_NO_ERR);
  }
  else
  {
  	staComRx=COM_RX_NONE;
    return(COM_ERR_COR);
  }
}
INT8U comTrCommand(char *p,INT8U lenght)
{
	INT8U i,k;
	INT8U err;
	INT16U i_16;
	unsigned char myDataBuf[128];
	myDataBuf[0]=0x06+lenght;
  myDataBuf[1]=0x02;
  myDataBuf[2]=MyAddres[0];
  myDataBuf[3]=MyAddres[1];
  myDataBuf[4]=0x00;
  myDataBuf[5]=0x00;
  for(i=0;i<lenght;i++)myDataBuf[6+i]=(unsigned char)p[i];
  myDataBuf[6+lenght]=0xff;
	for(i=0;i<6;i++)
  {
  	i_16=(TH0<<8)|TL0;
  	//srand(i_16);
  	//i_16=rand();
  	//i_16%=3000;
  	i_16 &= 0x7ff;
  	k=comTxDataConfirm(myDataBuf);
  	if(k==COM_ERR_COR)return(COM_ERR_COR);
  	//OSSemPend(&comRxDataRdy,10000+i_16,&err);
  	OSSemPend(&comRxDataRdy,3000+i_16,&err);
  	if((err==OS_NO_ERR)&&(staComRx==COM_RX_DATA))break;
  }
	if((err==OS_NO_ERR)&&(staComRx==COM_RX_DATA))
  {
  	myDataBuf[0]=0x07;
  	myDataBuf[1]=0x03;
  	myDataBuf[2]=MyAddres[0];
  	myDataBuf[3]=MyAddres[1];
  	myDataBuf[4]=0x00;
  	myDataBuf[5]=0x00;
  	myDataBuf[6]=0x52;
  	myDataBuf[7]=0xff;
  	comSenData(myDataBuf[0]+1,myDataBuf);
  	staComRx=COM_RX_NONE;
  	flag_com_data_finish=0;
  	return(COM_NO_ERR);
  }
  else return(COM_ERR_SER);
}

void comSenData(INT8U Len,INT8U *p)					//从串口发送数据
{
	  INT8U i;
		for(i=0;i<Len;i++)
		{
			ACC=*(p+i);
		  TB8=!P;
		  SBUF=*(p+i);
		  while(!TI);
      TI=0;
		  //return(1);
		}
}


#define RFID_GLOBALS
#include <includes.h>


#if	0
void serSpeed_set()
{
	ES=0;
	TH1 = 0xfe;
	TL1 = 0xfa;
	delay(200);
	ES=1;
}

void serSpeed_restore()
{
	ES=0;
	TH1 = 0xff;
	TL1 = 0xfa;
	delay(200);
	ES=1;
}
#endif

void TrCardId()
{
	INT32U ComIdbuf=0;
	INT8U i=0;
	INT32U j32u=1;
	//INT32U i32u=1000000000
	ComIdbuf=CardId[3];
	ComIdbuf=(ComIdbuf<<8)+CardId[2];
  ComIdbuf=(ComIdbuf<<8)+CardId[1];
  ComIdbuf=(ComIdbuf<<8)+CardId[0];
  for(i=0;i<10;i++)
  {
  	idbuf[i]=ComIdbuf/(1000000000/j32u)+48;
  	ComIdbuf=ComIdbuf%(1000000000/j32u);
  	j32u*=10;
  }
}
#if	0
unsigned char GetRfid()
{
	if(!PinRff)return(0);
	else
	{
		PinRfa1=0;
    PinRfa2=0;
    P1=0xff;
    delay1(200);
    CardId[0]=P1;
    PinRfa1=1;
    PinRfa2=0;
    P1=0xff;
    delay1(200);
    CardId[1]=P1;
    
    PinRfa1=0;
    PinRfa2=1;
    P1=0xff;
    delay1(200);
    CardId[2]=P1;
    PinRfa1=1;
    PinRfa2=1;
    P1=0xff;
    delay1(200);
    CardId[3]=P1;
    //fKeyBuz=BZ_ON;
    //Id_disp();
    TrCardId();
    return(1);
  }
}
#else
unsigned char GetRfid()
{
	//serSpeed_set();			/** seral set **/
	PinRfs=0;
	if((flag_com_confirm_finish == 1) && (staComRx == COM_RX_CARD))
	{
		flag_com_confirm_finish = 0;
		staComRx=COM_RX_NONE;

		CardId[3] = comRxDataBuf[4];
		CardId[2] = comRxDataBuf[5];
		CardId[1] = comRxDataBuf[6];
		CardId[0] = comRxDataBuf[7];
		
		#if	0
		/*** debug start ***/
 		PinRff=0;
		delay(200);
 		PinRff=1;
		/*** debug end ***/
		#endif
	}
	else
	{
		//serSpeed_restore();		/** seral speed restore **/
		return(0);
	}
	
	TrCardId();
	//serSpeed_restore();			/** seral set restore **/
	
	return(1);
}
#endif

void RFID_GetID()
{
	unsigned char i;
	CntOv1s=0;
  fOvTime=0;
  fCancel=0;
  while(!GetRfid())//µÈ´ýË¢¿¨
  {
  	WDT_CONTR=0x3d;  //Î¹¹· 1.13s;
  	i=KeyRd();
  	if(i==0x0a)
  	{
  		fCancel=1;
  		return;
  	}
  	if(CntOv1s<=ID_OVERTIME)
    {
    	Lcd_overtime(ID_OVERTIME-CntOv1s,0);
    }
    else
    {
    	fOvTime=1;
			player_stop=0;
    	while(player_busy()==1)WDT_CONTR=0x3d;  //Î¹¹· 1.13s;
			player_Num(54);
			while(player_busy()==1)WDT_CONTR=0x3d;  //Î¹¹· 1.13s;
			player_Num(55);
    	return;
    }
  }
}

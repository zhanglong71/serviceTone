/*******************************************************************************
	- Chip		: MG2450/2455
	- Vendor		: RadioPulse Inc, 2007.
	- Date		: 2010-08-13
	- Version		: VER 1.80

	[2010-08-13] VER 1.80
	- Modified to support the new version chip whose CHIP ID is 0x96 in ZHAL_3V_LOGIC_INIT().

	[2009-07-15] VER 1.76
	- More stable lock detector in ZHAL_MODEM_INIT_CHIP92().
		+ xPLLLD 0x0A --> 0x1C
	- When a 500K/1Mbps-mode device receives a 250Kbps packet, 
	   it should transmits a ACK packet(250Kbps). Then, 290us is too 
	   short. So, the ACK transmission is failed.
		+ @16MHz Mode	: 300(290us) --> 400(400us)
		+ @8MHz Mode	: 130(290us) --> 180(400us) 
	- TxPower Registers for low current consumption @-40dBm or -50dBm in ZHAL_TXPOWER_SET_CHIP92().
		+ @-40dBm, 
			xTXPA=0x9F --> 11
			xTXDA=0xFF --> E1
			xTXMIX=0x1C --> 6D
		+ @-50dBm, 
			xTXPA=0x18 --> 11
			xTXDA=0xE7 --> E0
			xTXMIX=0x1C --> 6D

	[2009-02-23]	VER 1.75
	- <#if ENABLE_PHY_UNCALLED ~ #endif> is added	
	- ZHAL_MODEM_INIT_CHIP92() is updated.
	- ZHAL_MODEM_INIT() is updated.
	- ZHAL_SEND_TXFIFO() is updated.
	- ZHAL_SEND_ACK() is updated.
	- ZHAL_DATARATE_SET_ISR() is added.
	- Mode_19MHz has default value as 0.	
	- ZHAL_RF_INT_CLEAR() is deleted.
	- xxxx_CHIP91() function is deleted.
	- ZHAL_RECEIVER_ON/OFF() is deleted.

	[2008-07-28] VER 1.71
	- ZHAL_SEND_TXFIFO() : When returning 0xE1, xMTFCRP is also cleared to 0.
	- ZHAL_SEND_TXFIFO() : When transmitting, TxOperating is set to 1
	- ZHAL_SEND_ACK() : When TxOperating is 1, Ack is not transmitted
	- ZHAL_MODEM_INIT_CHIP92() : 17 registers are updated
		+ xCORCNF1
		+ xAGCCNF7/8/9/10/11/12/15/16/17/18/19/22
		+ xPLLLF1/2/3
		+ xTXLPF	
	- ZHAL_COORDINATOR_SET() : The operated bit was wrong. So, It is modified.
	
	[2008-04-01] VER 1.70
	- ZHAL_SEND_TXFIFO() : Check Modem Status before transmitting
	- ZHAL_SEND_ACK() : Check Modem Status before transmitting
	- ZHAL_DATARATE_SET() : xVRXLPF is changed from 0xE0 to 0xED
	- ZHAL_MODEM_INIT_CHIP92() : xVRXLPF is changed from 0xE0 to 0xED

	[2008-03-06] VER 1.63
	- Brown-Out-Dector(BOD) is enabled
	- 1M parameter(xRXRSSI, xVRXLPF) is changed in ZHAL_DATARATE_SET()
	- RxStart Interrupt is disabled(xINTCON = 0xFA)

	[2008-01-10] VER 1.62
	- Register Version V1.5 is adopted
		xAGCCNF19, xRXBBAMP, xVRXLPF, xAGCCNF22 are updated
		xINTCON : RxStart Interrupt is enabled

	[2007-12-20] VER 1.61
	- AckWait for() counter is changed for stability in ZHAL_SEND_TXFIFO()
		1000 ==> 1200	: @16MHz, MAX 2.0ms
		500  ==> 600		: @8MHz, MAX 2.0ms
	
	[2007-12-11] VER 1.60
	- for() counter is changed in ZHAL_SEND_TXFIFO()	
		5000 ==> 2500
		4999 ==> 2499
		1000 ==> 500
		(Because, Core Clock is changed from 16MHz to 8MHz)
	- for() counter is changed in ZHAL_SEND_ACK()
	 	1000 ==> 500
	 	999 ==> 499
	 	(Because, Core Clock is changed from 16MHz to 8MHz)
	- xTST0 is changed when Modulation with IFS mode in ZHAL_TeST_TXOUT()
		0x7F ==> 0x77
	
	[2007-11-07] VER 1.50
	- Initial Version	
*******************************************************************************/

#include "INCLUDE_TOP.h"

//
//	xPDM
//	bit[7]	: Analog Regulator. 1=Voltage is changed (1.5V <-> 1.8V)
//	bit[6]	: Digital Regulator. 1=Voltage is changed (1.5V <-> 1.8V)
//
//	When MSV pin of MG245X is HIGH(It means 1.8V input)
//	===> if bit[7]=1, Regulator Output Voltage is 1.5V.(1.5V is used in core block).
//	When MSV pin of MG245X is LOW(It means 1.5V input)
//	===> if bit[7]=1, Regulator Output Voltage is 1.8V.(1.8V is used in core block).
//
void ZHAL_REGULATOR_SET(UINT8 ChangeAnalog, UINT8 ChangeDigital)
{
	if(ChangeAnalog)	xPDM |= 0x80;		// bit[7]=1
	else					xPDM &= ~0x80;		// bit[7]=0

	if(ChangeDigital)		xPDM |= 0x40;		// bit[6]=1
	else					xPDM &= ~0x40;		// bit[6]=0
}


#if (ENABLE_PHY_UNCALLED)
void ZHAL_CLOCK_BLOCK_SET(UINT8 EnaCodec, UINT8 EnaAES)
{
	//--------------------------------------------------------------------
	//	BIT[7]	BIT[6]	BIT[5]	BIT[4]	BIT[3]	BIT[2]	BIT[1]	BIT[0]
	//--------------------------------------------------------------------
	//	xCLKON0 ( 0xff)
	//	CCLK	CCCLK	CPCLK	AESCLK	MTCLK	MRCLK	REGCLK	ECHOCLK
	//					(PERI)									(CODEC)
	//--------------------------------------------------------------------
	
	if(EnaCodec)
			xCLKON0 |=  (1<<0);
  	else		xCLKON0 &= ~(1<<0);
	
	if(EnaAES)
			xCLKON0 |= (1<<4);
   	else		xCLKON0 &= ~(1<<4);
}
#endif


//-- EnaCLKPLL
//	1 	: Enable CLKPLL to use 19.2MHz XTAL CLOCK.
//	0	: Disable CLKPLL to use default XTAL CLOCK.
void ZHAL_CLOCK_PLL_SET(UINT8 EnaCLKPLL)
{
	//-------------------------------------------------------------------------------------
	//	xCKPLL
	//-------------------------------------------------------------------------------------
	//	BIT[7]	BIT[6]		BIT[5]		BIT[4]		BIT[3]		BIT[2:1]			BIT[0]
	//	Rsvd	CKPLL_CH	CKPLL_OK	CKPLLEN	CKPLLRSTB		XTALSEL[1:0]		RCOSCDIV2
	//-------------------------------------------------------------------------------------
	
	UINT8	_CKPLL;

	_CKPLL = xCKPLL;
	
	if(EnaCLKPLL)
	{
		_CKPLL |= ( 1 << 4); 	// CKPLLEN ON
		_CKPLL |= ( 1 << 3);	// Release RESET# 

		xCKPLL = _CKPLL;

		while(1)	// 
		{
			if(xCKPLL & 0x20) break;	// PLL OK CONFIRM
		}

		xCKPLL |= (1<<2);			// CLOCK SWITCHING TO PLLOUT
	}
	else
	{
		_CKPLL &= ~(1 <<2);	// CLK = XTAL	
		_CKPLL &= ~(1 <<4); 	// CKPLLEN OFF
		_CKPLL &= ~(1 <<3);	// Initialte RESET#
		
		xCKPLL = _CKPLL;		// CLOCK SWITCHING TO 16MHZ
	}
}


//-- Ena16MHz
//	1 	: System Clock 16MHz
//	0	: System Clock 8MHz
/*void ZHAL_CLOCK_16MHz_SET(UINT8 Ena16MHz)
{
	if(Ena16MHz)	xCLKDIV0 = 0x00;
	else				xCLKDIV0 = 0xFF;
}*/

	UINT8	Mode_19MHz = 0;	// 19.2MHz CLK PLL
void ZHAL_HW_INIT(UINT8 Ena19MHz, UINT8 VoltChange)
{	
	Mode_19MHz = 0;	
	if(Ena19MHz)
	{	
		ZHAL_CLOCK_PLL_SET(1);
		Mode_19MHz = 1;
	}

	if(VoltChange)
	{
		ZHAL_REGULATOR_SET(1, 1);
	}
}

void ZHAL_CHANNEL_SET_CHIP92_XTAL19(UINT8 CH)
{
	UINT16		Counter;
	UINT8		STA;
	UINT16		i;
	UINT8		F3, F4;

	xRXRFPD	= 0x00;
	xRXRFPU	= 0xFF;
	xTXRFPD	= 0x00;
	xTXRFPU	= 0xFF;

	switch(CH) 
	{
		case 11 	: F3 = 0x05; F4 = 0x30;	 xPLLADIV = (xPLLADIV & 0x30) | 0x01; break;
		case 12 	: F3 = 0x0A; F4 = 0x70;	 xPLLADIV = (xPLLADIV & 0x30) | 0x01; break;
		case 13 	: F3 = 0x0F; F4 = 0xA0;	 xPLLADIV = (xPLLADIV & 0x30) | 0x01; break;
		case 14 	: F3 = 0x14; F4 = 0xD0;	 xPLLADIV = (xPLLADIV & 0x30) | 0x01; break;
		case 15 	: F3 = 0xB6; F4 = 0x10;	 xPLLADIV = (xPLLADIV & 0x30) | 0x02; break;
		case 16 	: F3 = 0xBB; F4 = 0x40;	 xPLLADIV = (xPLLADIV & 0x30) | 0x02; break;
		case 17 	: F3 = 0xC0; F4 = 0x70;	 xPLLADIV = (xPLLADIV & 0x30) | 0x02; break;
		case 18 	: F3 = 0x29; F4 = 0xB0;	 xPLLADIV = (xPLLADIV & 0x30) | 0x01; break;
		case 19 	: F3 = 0x2E; F4 = 0xE0;	 xPLLADIV = (xPLLADIV & 0x30) | 0x01; break;
		case 20 	: F3 = 0x34; F4 = 0x10;	 xPLLADIV = (xPLLADIV & 0x30) | 0x01; break;
		case 21 	: F3 = 0x39; F4 = 0x50;	 xPLLADIV = (xPLLADIV & 0x30) | 0x01; break;
		case 22 	: F3 = 0x3E; F4 = 0x80;	 xPLLADIV = (xPLLADIV & 0x30) | 0x01; break;
		case 23 	: F3 = 0xDF; F4 = 0xB0;	 xPLLADIV = (xPLLADIV & 0x30) | 0x02; break;
		case 24 	: F3 = 0xE4; F4 = 0xF0;	 xPLLADIV = (xPLLADIV & 0x30) | 0x02; break;
		case 25 	: F3 = 0xEA; F4 = 0x20;	 xPLLADIV = (xPLLADIV & 0x30) | 0x02; break;
		case 26 	: F3 = 0x53; F4 = 0x50;	 xPLLADIV = (xPLLADIV & 0x30) | 0x01; break;
		default 	: F3 = 0x2E; F4 = 0xE0;	 xPLLADIV = (xPLLADIV & 0x30) | 0x01; break;
	}

	xPLLBDIV = 0x06;
	xPLLCP = 0x32;
	xPLLFRAC3	= F3;
	xPLLFRAC4	= F4;

	Counter = 0;
	while(1)
	{	
		Counter += 100;
		xPLLADFC	= 0xEF;
		for(i=0 ; i<Counter ; i++);
		xPLLADFC	= 0xEF;

		STA = 0;
		for(i=0 ; i<0x1000 ; i++)
		{
			if(xPLLLD & 0x40)
			{
				STA = 1;
				break;
			}
		}

		if(STA)	break;
	}

	xRXRFPD	= 0xF7;
	xRXRFPU	= 0xFF;
	xTXRFPD	= 0xFF;
	xTXRFPU	= 0xFF;
}



void ZHAL_CHANNEL_SET_CHIP92_XTAL16(UINT8 CH)
{
	UINT16		Counter;
	UINT8		STA;
	UINT16		i;
	UINT8		F3, F4;

	xRXRFPD	= 0x00;
	xRXRFPU	= 0xFF;
	xTXRFPD	= 0x00;
	xTXRFPU	= 0xFF;

	switch(CH) 
	{
		case 11 	: F3 = 0x06; F4 = 0x40;	 xPLLADIV = (xPLLADIV & 0x30) | 0x02; break;
		case 12 	: F3 = 0x0C; F4 = 0x80;	 xPLLADIV = (xPLLADIV & 0x30) | 0x02; break;
		case 13 	: F3 = 0x12; F4 = 0xC0;	 xPLLADIV = (xPLLADIV & 0x30) | 0x02; break;
		case 14 	: F3 = 0x19; F4 = 0x00;	 xPLLADIV = (xPLLADIV & 0x30) | 0x02; break;
		case 15 	: F3 = 0xBB; F4 = 0x40;	 xPLLADIV = (xPLLADIV & 0x30) | 0x03; break;
		case 16 	: F3 = 0xC1; F4 = 0x80;	 xPLLADIV = (xPLLADIV & 0x30) | 0x03; break;
		case 17 	: F3 = 0xC7; F4 = 0xC0;	 xPLLADIV = (xPLLADIV & 0x30) | 0x03; break;
		case 18 	: F3 = 0x32; F4 = 0x00;	 xPLLADIV = (xPLLADIV & 0x30) | 0x02; break;
		case 19 	: F3 = 0x38; F4 = 0x40;	 xPLLADIV = (xPLLADIV & 0x30) | 0x02; break;
		case 20 	: F3 = 0x3E; F4 = 0x80;	 xPLLADIV = (xPLLADIV & 0x30) | 0x02; break;
		case 21 	: F3 = 0x44; F4 = 0xC0;	 xPLLADIV = (xPLLADIV & 0x30) | 0x02; break;
		case 22 	: F3 = 0x4B; F4 = 0x00;	 xPLLADIV = (xPLLADIV & 0x30) | 0x02; break;
		case 23 	: F3 = 0xED; F4 = 0x40;	 xPLLADIV = (xPLLADIV & 0x30) | 0x03; break;
		case 24 	: F3 = 0xF3; F4 = 0x80;	 xPLLADIV = (xPLLADIV & 0x30) | 0x03; break;
		case 25 	: F3 = 0xF9; F4 = 0xC0;	 xPLLADIV = (xPLLADIV & 0x30) | 0x03; break;
		case 26 	: F3 = 0x00; F4 = 0x00;	 xPLLADIV = (xPLLADIV & 0x30) | 0x03; break;
		default	: F3 = 0x38; F4 = 0x40;	 xPLLADIV = (xPLLADIV & 0x30) | 0x02; break;
	}

	switch (CH) 
	{
		case 26 	: xPLLCP	= 0x3B;		break;
		default 	: xPLLCP 	= 0x32;		break;
	}
	
	xPLLRDIV	= (xPLLRDIV & 0x01);
	xPLLFRAC3	= F3;
	xPLLFRAC4	= F4;

	Counter = 0;
	while(1)
	{	
		Counter += 100;
		xPLLADFC	= 0xEF;
		for(i=0 ; i<Counter ; i++);
		xPLLADFC	= 0xEF;

		STA = 0;
		for(i=0 ; i<0x1000 ; i++)
		{
			if(xPLLLD & 0x40)
			{
				STA = 1;
				break;
			}
		}

		if(STA)	break;
	}

	xRXRFPD	= 0xF7;
	xRXRFPU	= 0xFF;
	xTXRFPD	= 0xFF;
	xTXRFPU	= 0xFF;
}


void ZHAL_CHANNEL_SET(UINT8 CH)
{
	if(Mode_19MHz)
			ZHAL_CHANNEL_SET_CHIP92_XTAL19(CH);
	else		ZHAL_CHANNEL_SET_CHIP92_XTAL16(CH);
}



void ZHAL_3V_LOGIC_INIT()
{	
	xPDM	= 0x00;
	//
	// 	[2008-03-06] VER 1.63
	//
	//xPDCON	= 0xE8;	// 1110_1000. BOD is NOT enabled. EVK V1.5
	if(xCHIPID >= 0x96)
	{
		xPDCON = 0xE8;	// 1110_1000. BOD(Bit2)=0(Enabled, reversed polarity)
	}
	else
	{
		xPDCON = 0xEC;	// 1110_1100. BOD(Bit2)=1(Enabled)
	}
	//
	xRCOSC1= 0x3F;
	xRCOSC2= 0xF4;
	xRTDLY	= 0x02;
	xRTCON	= 0x80;
	xRTINT1	= 0x00;
	xRTINT0	= 0x08;
	xCKPLL	= 0xC2;
}

//- RETURN	: 1=Fail, 0=OK
UINT8 ZHAL_MODEM_INIT_CHIP92(UINT8 XTAL19, UINT16 WaitLoopCounter)
{
	UINT8	ModemOn_INT_STS;
	UINT8	ModemOn_INT_IDX;
	UINT8	ModemOn_RFIE;
	UINT16	iw;
	UINT8	InitError;	
	
	xRFCONCNF 	= 0xFD;
	xCORCNF0	= 0xC0;
	xCORCNF1	= 0x85;	// VER1.71 of PHY.C(Newly added)
	xTDCNF3 	= 0x7F;

	xTDCNF2		= 0xFF;

	xAGCCNF0 = 0xC2;
	xAGCCNF1 = 0x81;
	xAGCCNF3 = 0xFF;
	xAGCCNF5 = 0xDE;
	xAGCCNF6 = 0xD9;
	
//	xAGCCNF7 = 0x83;
//	xAGCCNF8 = 0x8B;	
//	xAGCCNF10 = 0xE0;
//	xAGCCNF11 = 0x20;	
//	xAGCCNF15 = 0xCE;
//	xAGCCNF16 = 0xD8;
//	xAGCCNF17 = 0xE7;
//	xAGCCNF18 = 0xE7;
//	xAGCCNF19 = 0xEF;
	xAGCCNF7 = 0xF8;	// VER1.71 of PHY.C
	xAGCCNF8 = 0xF8;	// VER1.71 of PHY.C
	xAGCCNF9 = 0xE0;	// VER1.71 of PHY.C
	xAGCCNF10 = 0xF8;	// VER1.71 of PHY.C
	xAGCCNF11 = 0xFB;	// VER1.71 of PHY.C
	xAGCCNF12 = 0xE0;	// VER1.71 of PHY.C
	xAGCCNF15 = 0x07;	// VER1.71 of PHY.C
	xAGCCNF16 = 0x0F;	// VER1.71 of PHY.C
	xAGCCNF17 = 0x21;	// VER1.71 of PHY.C
	xAGCCNF18 = 0x1D;	// VER1.71 of PHY.C
	xAGCCNF19 = 0xEC;	// VER1.71 of PHY.C
	
//	xAGCCNF22 = 0xF6;
	xAGCCNF22 = 0xF5;	// VER1.71 of PHY.C
	xAGCCNF23 = 0x77;
	xAGCCNF24 = 0x88;
	xAGCCNF26 = 0x3F;
	
	// RF Setting
	xPLLADFC = 0x6F;
	//xPLLLD 	= 0x0A;	// PHY v1.75
	xPLLLD 	= 0x1C;		// PHY v1.76
	xPLLCP 	= 0x32;
	xPLLVC 	= 0xE8;
	xPLLDIV 	= 0xFA;	
	xPLLBUF 	= 0x7F;
	xRXLNA 	= 0x12;
	xRXMIX 	= 0x70;
	xRXIP2I 	= 0x12;
	xRXIP2Q 	= 0x2A;
	xRXBBAMP = 0xF8;
	xRXRSSI = 0x4F;
	xCTLVGA = 0x00;	
//	xPLLLF1 	= 0x01;
//	xPLLLF2 	= 0x9F;
//	xPLLLF3 	= 0x78;
	xPLLLF1 	= 0x04;		// VER1.71 of PHY.C	
	xPLLLF2 	= 0x4F;		// VER1.71 of PHY.C	
	xPLLLF3 	= 0x59;		// VER1.71 of PHY.C
//	xVRXLPF = 0xE0;
	xVRXLPF = 0xED;		// VER1.70 of PHY.C
	xTXPA 	= 0x9F;
	xTXDA 	= 0xFF;
	xTXMIX	= 0x6F;
//	xTXLPF	= 0x12;		
	xTXLPF	= 0x3A;		// VER1.71 of PHY.C
	xBIAS 	= 0x11; 		//
	xSADCCON	= 0x30; 	//

	xRXRFPD = 0xF7;
	xRXRFPU = 0xFF;	// Release RF RX-path Power-up
	xPCMD0 &= 0x7F;

	if(XTAL19)		// 19.2 MHz PLL
	{
		xPLLADIV = 0x01;
		xPLLBDIV = 0x06;
		xPLLFRAC3 = 0x2E;
		xPLLFRAC4 = 0xE0;
	}	

	xPLLPD  	= 0xFF;	// Release PLL Power-down
	xPLLPU	= 0xFF;	// Release PLL Power-up
	xRXRFPD = 0xF7;	// Release RF RX-path Power-down
	xRXRFPU = 0xFF;	// Release RF RX-path Power-up
	xTXRFPD	= 0xFF;	// Release RF TX-path Power-down
	xTXRFPU 	= 0xFF;	// Release RF TX-path Power-up
	xPCMD1  = 0x84;	// Release PHY TX/RX-path off		

	// Digital DCC	
	xCLKON0 = 0xC2;	
	xRXRFPD = 0xF7;
	xRXRFPU = 0xFF;		
	xDCCCNF = 0x1C;
	xRXVGA = 0x06;
	xCTLVGA = 0x00;		// Analog DCC disable
	xDCCCON = 0xC5;		// Full Cal

	InitError = 1;
	for(iw=0 ; iw<WaitLoopCounter ; iw++)
	{
		if( (xDCCCON & 0x04) == 0)
		{
			InitError = 0;
			break;
		}
	}	
	xCLKON0 = 0xFF;
	xDCCCNF = 0x08;
	xDCCCON = 0xC9;		// Tracking ON
	xRXRFPU = 0x00;
	//

	if(InitError == 0)
	{
		ModemOn_RFIE = RFIE;
		RFIE = 0;
		xINTCON = 0xF1;	// 1111_0001
		xPCMD0 &= 0x7F;	// bit[7]=0
		xPCMD0 &= 0xBF;	// bit[6]=0	

		InitError = 1;
		for(iw=0 ; iw<WaitLoopCounter ; iw++)
		{
			ModemOn_INT_STS = xINTSTS & 0x0F;
			if(ModemOn_INT_STS != 0x0F)
			{
				ModemOn_INT_IDX = xINTIDX;
				EXIF &= 0xEF;
				if(ModemOn_INT_STS == 0x0E)
				{
					InitError = 0;
					break;	// 0000_1110. Modem-On Interrupt
				}
			}
		}

		// [2008-03-06] VER 1.63
		//xINTCON = 0xFE;	// 1111_1110. RxEnd=1, RxStart=1, TxEnd=1, Modem-On=0. EVK V1.5
		xINTCON = 0xFA;		// 1111_1010. RxEnd=1, RxStart=0, TxEnd=1, Modem-On=0. EVK V1.6
		xTST2 = 0xFF;
		xTST13 = 0x00;
		xTST15 = 0x00;
		xCLKON1 = 0x7B;			// TSTCLK is OFF	
		RFIE = ModemOn_RFIE;
	}

	return InitError;
	
}


UINT8 ZHAL_MODEM_INIT(UINT16 WaitLoopCounter)
{
	UINT8	InitError;
	
	InitError = ZHAL_MODEM_INIT_CHIP92(Mode_19MHz, WaitLoopCounter);

	return InitError;
}

//-- RETURN
//	255	: Max Energy (High)
//	0	: Min Energy (Low)
UINT8 ZHAL_ED_LEVEL_GET()
{
	INT8		dBm;
	UINT16	Tmp16;
	UINT8	Level;

	dBm = xAGCSTS2;

	// RSSI : LQI type, 0~255, 255=high power
	if(dBm > 0)
	{
		Level = 255;
	}
	else if(dBm > -100)
	{
		Tmp16 = 100 + dBm;
		Tmp16 *= 255;
		Tmp16 /= 100;
		Level = Tmp16;
	}
	else
	{
		Level = 0;
	}

	return	Level;

}

#if (ENABLE_PHY_UNCALLED)
INT8 ZHAL_ED_dBm_GET()
{
	INT8		dBm;

	dBm = xAGCSTS2;
	return	dBm;
}
#endif

	extern	UINT8	FlagRxAck;
	extern	UINT8	FlagTxDone;
	UINT8	TxOperating = 0;

//-	RETURN
//	0xE1	: Channel Access Failure
//	0xE9	: No Ack
//	0x00	: SUCCESS
UINT8 ZHAL_SEND_TXFIFO()
{
	UINT16	iw;
	UINT8	STA;
	UINT8	_RXSTS;

	xMTFCRP = 0;
	xMACDSN = xMTxFIFO(0+3);
	FlagTxDone = 0;
	FlagRxAck = 0;

	//
	// Check Modem Status before transmitting. Added in VER1.70 of PHY.C
	//
	_RXSTS = xPHYSTS0 & 0x70;
	if( (_RXSTS != 0x00) && (_RXSTS != 0x10) )
	{
		return	0xE1;
	}
	//

	TxOperating = 1;
	FlagTxDone = 0;
	xPCMD0 &= 0xFB;	
	#if (OPERATE_AT_16MHz)
	for(iw=0 ; iw<5000 ; iw++)		// MAX 4.84 ms
	#else
	for(iw=0 ; iw<2500 ; iw++)		// MAX 4.84 ms
	#endif
	{
		if(FlagTxDone)
		{
			break;
		}

		#if (OPERATE_AT_16MHz)
		if(iw == 4999)
		#else
		if(iw == 2499)
		#endif
		{		
			xPCMD0 |= 0x04;
			xSWRST &= 0xFE;
			xSWRST |= 0x01;
			xSWRST &= 0xFD;
			xSWRST |= 0x02;
			xMTFCRP = 0;
			TxOperating = 0;
			STA = 0xE1;
			return	STA;
		}
	}
	TxOperating = 0;

	STA = 0;
	if(xMTxFIFO(0+1) & 0x20)
	{		
		STA = 0xE9;
		#if (OPERATE_AT_16MHz)
		for(iw=0 ; iw<1200 ; iw++)		// MAX 2.0 ms
		#else
		for(iw=0 ; iw<600 ; iw++)		// MAX 2.0 ms
		#endif
		{
			if(FlagRxAck)
			{
				STA = 0;
				break;
			}
		}			
	}

	xMTFCRP = 0;
	
	return	STA;
}

void ZHAL_SEND_ACK(UINT8 FP, UINT8 DSN)
{
	UINT16	iw;
	UINT8	_RXSTS;

	if(TxOperating)	return;

	xMTxFIFO(0x80) = 0x05;
	xMTxFIFO(0x81) = 0x02;
	if(FP)	xMTxFIFO(0x81) |= 0x10;
	xMTxFIFO(0x82) = 0x00;
	xMTxFIFO(0x83) = DSN;		

	xCCA0 = 0x88;			// Disable CCA
	xMTFCRP = 0x80;		

	//
	// Check Modem Status before transmitting. Added in VER1.70 of PHY.C
	//
	_RXSTS = xPHYSTS0 & 0x70;
	if( (_RXSTS != 0x00) && (_RXSTS != 0x10) )
	{
		goto JP_SEND_ACK_RETURN;
	}
	//
	
	xPCMD0 &= 0xFB;
	#if (OPERATE_AT_16MHz)
	for(iw=0 ; iw<400 ; iw++);		// LoopCounter=400(400us)
	#else
	for(iw=0 ; iw<180 ; iw++);		// LoopCounter=180(400us)
	#endif
	xPCMD0 |= 0x04;

	JP_SEND_ACK_RETURN:

	xMTFCRP = 0x00;
	xCCA0 = 0xC4;			// Enable CCA	
}

#if (ENABLE_PHY_UNCALLED)
//-	Enable : Test Mode Enable
// 0	: Normal Operation Mode
// 1	: Continuous RF Signal Generation Mode
//-	ModuleationEn : Modulated Signal Generation Enable
// 0	: Carrier Signal Generation
// 1	: Modulated Signal Generation with IFS(Inter-Frame-Space)
// 2	: Modulated Signal Generation without IFS(Inter-Frame-Space)
void ZHAL_TEST_TXOUT(UINT8 Ena, UINT8 ModulationEn)
{
	if(Ena)
	{
		//
		// Common Paramaters
		//	
		xPLLPD  = 0xFF;
		xPLLPU  = 0x00;
		xPCMD0	= 0x7F;		// 0111_1111
		xPCMD0	= 0xFE;		// 1111_1110
		xPCMD1	= 0xC4;		// 1100_0100
		xTXRFPD = 0xFF;
		xTXRFPU = 0x00;

		xRXRFPD = 0x00;
		xCLKON1 = 0x47;		// 0100_0111		

		if(ModulationEn == 1)			// Modulation with IFS
		{
			xTST0	= 0xFF;
			xTST2  	= 0x7F;
			xTST13 	= 0x7F;
			xTST0  	= 0xFF;
			xTST0  	= 0x77;
			xTST15 	= 0x00;
			xTST0	= 0x77;
		}
		else if(ModulationEn == 2)	// Modulation without IFS
		{
			xTST2	= 0x00;
			xTST13 	= 0x7F;
			xTST0  	= 0xFF;
			xTST0  	= 0x22;
		}
		else
		{
			xTST14	= 0x00;	// 0x5A = 700K, 0x40 = 500K
			xTST0 	= 0x20;
		}
	}
	else
	{
		xTST0   = 0x80;
		xCLKON1 = 0x7B;
		xRXRFPD = 0xF7;
		xRXRFPU = 0xFF;
		xTXRFPD = 0xFF;
		xTXRFPU = 0xFF;
		xPCMD0  = 0xBF;
	}
	
}
#endif

#if (ENABLE_PHY_UNCALLED)
// Rate = 0	:	250 Kbps
// Rate = 1	:	500 Kbps
// Rate = 2	:	1 Mbps
// Other		:	250 Kbps
 void ZHAL_DATARATE_SET(UINT8 Rate)
 {	
 	xRXFRM1 &= 0x0F;	
 	
 	if(Rate == 1)			xRXFRM1 |= 0x50;	// bit[4]=1, bit[6]=1
 	else if(Rate == 2)	xRXFRM1 |= 0xA0;	// bit[5]=1, bit[7]=1
 
 	if(Rate==2)
 	{
 		xRXRSSI = 0x4F;
 		xVRXLPF = 0xEA;
 	}
 	else
 	{	
 		xRXRSSI = 0x4F;
 		xVRXLPF = 0xED;
 	}
 }
#endif

// Rate = 0	:	250 Kbps
// Rate = 1	:	500 Kbps
// Rate = 2	:	1 Mbps
// Other		:	250 Kbps
 void ZHAL_DATARATE_SET_ISR(UINT8 Rate)
 {	
 	xRXFRM1 &= 0x0F;	
 	
 	if(Rate == 1)			xRXFRM1 |= 0x50;	// bit[4]=1, bit[6]=1
 	else if(Rate == 2)	xRXFRM1 |= 0xA0;	// bit[5]=1, bit[7]=1
 
 	if(Rate==2)
 	{
 		xRXRSSI = 0x4F;
 		xVRXLPF = 0xEA;
 	}
 	else
 	{	
 		xRXRSSI = 0x4F;
 		xVRXLPF = 0xED;
 	}
 }


//-----------------------------
// MAC_CTRL
//-----------------------------
// bit[7:5] : rsv				:: default=0
// bit[4] : prevent_ack_packet	:: default=0
// bit[3] : pan_coordinator		:: default=0
// bit[2] ; addr_decode			:: default=1
// bit[1] : auto_crc				:: default=1
// bit[0] : auto_ack			:: default=0
void ZHAL_MAC_CTRL_SET(UINT8 MacCtrl)
{
	xMACCTRL = MacCtrl;
}

#if (ENABLE_PHY_UNCALLED)
void ZHAL_AUTO_CRC_SET(UINT8 Ena)
{
	if(Ena)	xMACCTRL |= 0x02;		// bit[1]=1
	else		xMACCTRL &= ~0x02;		// bit[1]=0
}
#endif

#if (ENABLE_PHY_UNCALLED)
void ZHAL_ADDR_DECODE_SET(UINT8 Ena)
{
	if(Ena)	xMACCTRL |= 0x04;		// bit[2]=1
	else		xMACCTRL &= ~0x04;		// bit[2]=0
}
#endif

void ZHAL_COORDINATOR_SET(UINT8 Ena)
{	
//	if(Ena)	xMACCTRL |= 0x10;
//	else		xMACCTRL &= ~0x10;
	if(Ena)	xMACCTRL |= 0x08;		// bit[3]=1. VER1.71 of PHY.C
	else		xMACCTRL &= ~0x08;		// bit[3]=0. VER1.71 of PHY.C
}

void ZHAL_PAN_ID_SET(UINT16 ID) {
	xPANID(0) = (UINT8) (ID) ;
	xPANID(1) = (UINT8) (ID >> 8) ;
}

void ZHAL_SHORT_ADDR_SET(UINT16 Addr) {
	xSHORTADDR(0) = (UINT8) (Addr) ;
	xSHORTADDR(1) = (UINT8) (Addr >> 8) ;
}

void ZHAL_IEEE_ADDR_SET(UINT8 *pAddr) {
	memcpy( &xEXTADDR(0), pAddr, 8);
}

void ZHAL_MAC_INIT()
{
	xMACCTRL = 0x16;
	xMRFCRP = 0;
	xMRFCWP = 0;
	xMTFCRP = 0;
	xMTFCWP = 0;
}

//-----------------------------------
//--	PowerLevel
//-----------------------------------
//	0 	: 8		dBm
//	1	: 8		dBm
//	2	: 8		dBm
//	3	: 7		dBm
//	4	: 6		dBm
//	5	: 5		dBm
//	6	: 4		dBm
//	7	: 3		dBm
//	8	: 2		dBm
//	9	: 1		dBm
//	10	: 0		dBm
//	11	: -5		dBm
//	12	: -7		dBm
//	13	: -10	dBm
//	14	: -15	dBm
//	15	: -20	dBm
//	16	: -30 	dBm
//	17	: -40	dBm
//	18	: -50	dBm
//-----------------------------------

void ZHAL_TXPOWER_SET_CHIP92(UINT8 PowerLevel) 
{
	switch(PowerLevel)
	{
		case 0	: xTXPA=0x9F; xTXDA=0xFF; xTXMIX=0x6F;	break;
		case 1	: xTXPA=0x9F; xTXDA=0xFF; xTXMIX=0x6F;	break;
		case 2	: xTXPA=0x9F; xTXDA=0xFF; xTXMIX=0x6F;	break;
		case 3	: xTXPA=0x9F; xTXDA=0xF5; xTXMIX=0x6F;	break;
		case 4	: xTXPA=0x9D; xTXDA=0xF0; xTXMIX=0x6F;	break;
		case 5	: xTXPA=0x9F; xTXDA=0xED; xTXMIX=0x6F;	break;
		case 6	: xTXPA=0x95; xTXDA=0xED; xTXMIX=0x6F;	break;
		case 7	: xTXPA=0x1F; xTXDA=0xF3; xTXMIX=0x6F;	break;
		case 8	: xTXPA=0x1F; xTXDA=0xEC; xTXMIX=0x6F;	break;
		case 9	: xTXPA=0x1E; xTXDA=0xEA; xTXMIX=0x6F;	break;
		case 10	: xTXPA=0x1C; xTXDA=0xE9; xTXMIX=0x6F;	break;
		case 11	: xTXPA=0x1E; xTXDA=0xE3; xTXMIX=0x6F;	break;
		case 12	: xTXPA=0x18; xTXDA=0xE3; xTXMIX=0x6F;	break;
		case 13	: xTXPA=0x18; xTXDA=0xE2; xTXMIX=0x6F;	break;		
		case 14	: xTXPA=0x13; xTXDA=0xE2; xTXMIX=0x6F;	break;		
		case 15	: xTXPA=0x12; xTXDA=0xE2; xTXMIX=0x6E;	break;
		case 16	: xTXPA=0x11; xTXDA=0xE2; xTXMIX=0x6D;	break;
	//	case 17	: xTXPA=0x9F; xTXDA=0xFF; xTXMIX=0x1C;	break;	// PHY v1.75
	//	case 18	: xTXPA=0x18; xTXDA=0xE7; xTXMIX=0x1C;	break;	// PHY v1.75
		case 17	: xTXPA=0x11; xTXDA=0xE1; xTXMIX=0x6D;	break;	// PHY v1.76
		case 18	: xTXPA=0x11; xTXDA=0xE0; xTXMIX=0x6D;	break;	// PHY v1.76
		default	: xTXPA=0x9F; xTXDA=0xFF; xTXMIX=0x6F;	break;
		
	}

}

void ZHAL_TXPOWER_SET(UINT8 PowerLevel)
{
	ZHAL_TXPOWER_SET_CHIP92(PowerLevel);
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

void ZHAL_EXT0_INT_SET(UINT8 IntEna, UINT8 Priority, UINT8 TypeEdge) 
{
	UINT8	_EA;

	_EA = EA;
	EA = 0;	

	IT0 = TypeEdge;			// Edge=1, Level = 0
	PX0 = Priority;

	if(IntEna)	EX0 = 1;
	else 		EX0 = 0;

	EA = _EA;
}


void ZHAL_EXT1_INT_SET(UINT8 IntEna, UINT8 Priority, UINT8 TypeEdge) 
{
	UINT8	_EA;

	_EA = EA;
	EA = 0;	

	IT1 = TypeEdge;			// Edge=1, Level = 0
	PX1 = Priority;

	if(IntEna)	EX1 = 1;
	else 		EX1 = 0;

	EA = _EA;	
}

void ZHAL_RF_INT_SET(UINT8 IntEna, UINT8 Priority)
{
	UINT8	_EA;

	_EA = EA;
	EA = 0;

	RFIP = Priority;

	if(IntEna)	RFIE = 1;
	else			RFIE = 0;

	EA = _EA;
}

void ZHAL_SYSTEM_INTERRUPT(UINT8 IntEna)
{
	EA = IntEna;
}


/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////


#if (ENABLE_PHY_UNCALLED)
//-------------------------------------------------------
// Ena
//-------------------------------------------------------
// bit[1] : 1=Enable TRSWB(P1.6).	TxState=0, RxState=1
// bit[0] : 1=Enable TRSW(P1.7).		TxState=1, RxState=0
//-------------------------------------------------------
void ZHAL_TRSW_SET(UINT8 Ena)
{
	// TRSW = P1.7
	// TRSWB = P1.6

	xGPCNF4 &= 0x3F;
	if(Ena & 0x01)
	{	
		xGPCNF4 |= 0x40;
		xMONCON0 &= 0x0F;
	}

	xGPCNF4 &= 0xCF;
	if(Ena & 0x02)
	{
		xGPCNF4 |= 0x10;
	}
	
}
#endif

void ZHAL_PORT0_INOUT_SET(UINT8 Port, UINT8 OutEnable)
{
	UINT8	MASK_OR;
	UINT8	MASK_AND;
	UINT8	MASK_ENA;

	MASK_ENA = 1;
	switch(Port)
	{
		case 0x0	: MASK_OR = 0x01;	MASK_AND = 0xFE;	break;
		case 0x1	: MASK_OR = 0x02;	MASK_AND = 0xFD;	break;
		case 0x2	: MASK_OR = 0x04;	MASK_AND = 0xFB;	break;
		case 0x3	: MASK_OR = 0x08;	MASK_AND = 0xF7;	break;
		case 0x4	: MASK_OR = 0x10;	MASK_AND = 0xEF;	break;
		case 0x5	: MASK_OR = 0x20;	MASK_AND = 0xDF;	break;
		case 0x6	: MASK_OR = 0x40;	MASK_AND = 0xBF;	break;
		case 0x7	: MASK_OR = 0x80;	MASK_AND = 0x7F;	break;
		case 0xA	: MASK_OR = 0xFF;	MASK_AND = 0x00;	break;
		default	: MASK_ENA = 0;							break;
	}

	if(MASK_ENA)
	{
		if(Port == 0x0A)	{	P0REN = 0;	P0OEN = 0;	}
	
		if(OutEnable)	{	P0REN |= MASK_OR;	P0OEN &= MASK_AND;	}
		else				{	P0REN &= MASK_AND;	P0OEN |= MASK_OR;	}
	}
}


/*void ZHAL_PORT1_INOUT_SET(UINT8 Port, UINT8 OutEnable)
{
	UINT8	MASK_OR;
	UINT8	MASK_AND;
	UINT8	MASK_ENA;

	MASK_ENA = 1;
	switch(Port)
	{
		case 0x0	: MASK_OR = 0x01;	MASK_AND = 0xFE;	break;
		case 0x1	: MASK_OR = 0x02;	MASK_AND = 0xFD;	break;
		case 0x2	: MASK_OR = 0x04;	MASK_AND = 0xFB;	break;
		case 0x3	: MASK_OR = 0x08;	MASK_AND = 0xF7;	break;
		case 0x4	: MASK_OR = 0x10;	MASK_AND = 0xEF;	break;
		case 0x5	: MASK_OR = 0x20;	MASK_AND = 0xDF;	break;
		case 0x6	: MASK_OR = 0x40;	MASK_AND = 0xBF;	break;
		case 0x7	: MASK_OR = 0x80;	MASK_AND = 0x7F;	break;
		case 0xA	: MASK_OR = 0xFF;	MASK_AND = 0x00;	break;
		default	: MASK_ENA = 0;							break;
	}

	if(MASK_ENA)
	{
		if(Port == 0x0A)	{	P1REN = 0;	P1OEN = 0;	}
		
		if(OutEnable)	{	P1REN |= MASK_OR;	P1OEN &= MASK_AND;	}
		else				{	P1REN &= MASK_AND;	P1OEN |= MASK_OR;	}
	}

}*/

/*void ZHAL_PORT3_INOUT_SET(UINT8 Port, UINT8 OutEnable)
{
	UINT8	MASK_OR;
	UINT8	MASK_AND;
	UINT8	MASK_ENA;

	MASK_ENA = 1;
	switch(Port)
	{
		case 0x0	: MASK_OR = 0x01;	MASK_AND = 0xFE;	break;
		case 0x1	: MASK_OR = 0x02;	MASK_AND = 0xFD;	break;
		case 0x2	: MASK_OR = 0x04;	MASK_AND = 0xFB;	break;
		case 0x3	: MASK_OR = 0x08;	MASK_AND = 0xF7;	break;
		case 0x4	: MASK_OR = 0x10;	MASK_AND = 0xEF;	break;
		case 0x5	: MASK_OR = 0x20;	MASK_AND = 0xDF;	break;
		case 0x6	: MASK_OR = 0x40;	MASK_AND = 0xBF;	break;
		case 0x7	: MASK_OR = 0x80;	MASK_AND = 0x7F;	break;
		case 0xA	: MASK_OR = 0xFF;	MASK_AND = 0x00;	break;		
		default	: MASK_ENA = 0;							break;
	}

	if(MASK_ENA)
	{
		if(Port == 0x0A)	{	P3REN = 0;	P3OEN = 0;	}
	
		if(OutEnable)	{	P3REN |= MASK_OR;	P3OEN &= MASK_AND;	}
		else				{	P3REN &= MASK_AND;	P3OEN |= MASK_OR;	}
	}	
	
}  */

#if (ENABLE_PHY_UNCALLED)
//-- VFifoSrc
//	0	: I2S
//	1	: SPI
//	2	: UART0
//	3	: UART1
void ZHAL_VFIFO_MUX_SET(UINT8 VFifoSrc)
{
	xSRCCTL &= 0x9F;	// bit[6:5]=0
	xSRCCTL |= (VFifoSrc & 0x03) << 5;

//	xVTF_CTL |= 0x01;
//	xVTF_CTL |= 0x02;
//	xVRF_CTL |= 0x01;
//	xVRF_CTL |= 0x02;
//	while(xVTF_STS);
}
#endif



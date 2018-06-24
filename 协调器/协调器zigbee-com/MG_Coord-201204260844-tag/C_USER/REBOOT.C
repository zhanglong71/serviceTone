
/*******************************************************************************
	- Chip		: MG2450/2455
	- Vendor		: RadioPulse Inc, 2007.
	- Date		: 2010-05-28
	- Version		: VER 2.10

	[2010-05-28]	VER 2.10
	- Modified for V1.70 of FLASH.c	
*******************************************************************************/


//
//
//
//	*.A51 must configured to have 4-banks
//	*.H03 contains reboot program which copy code from ext-flash to int-flash
//	0x1200,0x1400,0x1600 has code-banking code, which have same image
//	
#include "INCLUDE_TOP.h"

void 	ZOTA_SPI_ON();
void  	ZOTA_SPI_OFF();
void 	ZOTA_SPI_SET(UINT8 Ena, UINT8 Mode, UINT8 Speed);
UINT8 	ZOTA_SPI_MASTER_POLLING(UINT8 SendByte);

UINT16 tmpadd(UINT16 a, UINT16 b)
{
	return(a+b);
}




void ZOTA_REBOOT()
{
	UINT16	T_Addr;
	UINT16	Over64K;

	UINT16	Tmp16;	

	UINT16 	i;
	UINT8	len;
	UINT8	dst;
	UINT8	op_int_flash_erase;
	UINT8	op_ext_flash_read;
	UINT8	op_int_flash_write;
	UINT8	op_reboot;
	UINT8	Tmp8;
	UINT16	L_Addr;
	UINT16	Var1;
	UINT8	_P0OEN;
	UINT8	_EA;

	char 	PageBuf[512];

	WDT |= 0x10;	// set bit[4]. WDT Write Enable
	WDT &= 0xF7;	// clear bit[3]. WDT Disable

	_EA = EA;
	_P0OEN = P0OEN;	

	EA=0;
	P0OEN &= ~(0x01<<0);
	P0OEN &= ~(0x01<<1);
	P0OEN &= ~(0x01<<2);
	P0OEN &= ~(0x01<<3);

	L_Addr  = 0;
	
	op_int_flash_erase=1;
	op_ext_flash_read=1;
	op_int_flash_write=1;
	op_reboot=1;

	//
	//
#if 	(OPERATE_AT_16MHz)
	/* 16MHz Parameters */
	xFCN_TERS1	=0x04; 
	xFCN_TERS0	=0xE2; 
	xFCN_TME1	=0x04; 
	xFCN_TME0	=0xE2; 
	xFCN_TPRG	=0x14;
	xFCN_TRCV	=0x10;
	xFCN_THV1	=0x00; 
	xFCN_THV0	=0xFA; 
	xFCN_TNVS	=0x50;
	xFCN_TNVH	=0x50;
	xFCN_TPGS	=0xA0;
	xFCN_TNVH1	=0x64; 
#else
	/* 08MHz Parameters */
	xFCN_TERS1	=0x02;
	xFCN_TERS0	=0x71;
	xFCN_TME1	=0x02;
	xFCN_TME0	=0x71;
	xFCN_TPRG	=0x0A;
	xFCN_TRCV	=0x08;
	xFCN_THV1	=0x00;
	xFCN_THV0	=0x7C;
	xFCN_TNVS	=0x28;
	xFCN_TNVH	=0x28;
	xFCN_TPGS	=0x50;
	xFCN_TNVH1 	=0x32;
#endif

	if( (xPDCON & 0x04) == 0)	goto JP_REBOOT_RET;
	if(xFCN_CMD != 0x80)		goto JP_REBOOT_RET;
	if(xFCN_LEN1 != 0x00)		goto JP_REBOOT_RET;
	if(xFCN_LEN0 != 0x00)		goto JP_REBOOT_RET;
	if(xFCN_ADR1 != 0x70)	goto JP_REBOOT_RET;
	if(xFCN_ADR0 != 0x00)	goto JP_REBOOT_RET;
	//
	//

	//
	//	Bank & library code @ 0x1200 ~ 0x17ff
	Var1 = 128;
	for(L_Addr=0x0000;L_Addr<0x6000;L_Addr=tmpadd(L_Addr,Var1))	// 96KB ( 4B * 0x6000 )
	{ 
	//	if(F_Addr == 0x1200) continue;
	//	if(F_Addr == 0x1400) continue;
	//	if(F_Addr == 0x1600) continue;
		//
		//	int-flash erase
		//
		if(op_int_flash_erase)
		{
			P0_0 = ~P0_0;
			P0_1 = 0;
			P0_2 = 0;

			xFCN_NOWAIT = 0x00;
		
			while(xFCN_STS0 & 0x01);
		
			_nop_();
			_nop_();
			_nop_();
			_nop_();
		
			//pTmp = (UINT8 *)&L_Addr;

			xFCN_ADR0 = L_Addr;		// LOW
			xFCN_ADR1 = L_Addr>>8;	// HIGH
			xFCN_LEN1 = 0;
			xFCN_LEN0 = 1;

			xFCN_CMD  = 0x02;	// Page Erase
		
			_nop_();
			_nop_();
			_nop_();
			_nop_();
		
			while(xFCN_STS0 & 0x01) ;
		}
		//
		//	ext-flash read
		//



		if(op_ext_flash_read)
		{
			P0_0 = 0;
			P0_1 = ~P0_1;
			P0_2 = 0;
			// Read Page Data : cmd-addr/3-data/1
			#define spiREAD           	0x03 
			//

			T_Addr = L_Addr + L_Addr + L_Addr + L_Addr;
			Over64K=(L_Addr >= 0x4000)? 1 : 0; 
			//pTmp = (UINT8 *)&T_Addr;

			ZOTA_SPI_ON();
			ZOTA_SPI_MASTER_POLLING(spiREAD);
			ZOTA_SPI_MASTER_POLLING(Over64K);
			ZOTA_SPI_MASTER_POLLING(T_Addr>>8);
			ZOTA_SPI_MASTER_POLLING(T_Addr);
			
			Tmp8=0;
			while(1)
			{
				Tmp16 = ZOTA_SPI_MASTER_POLLING(0);
				PageBuf[Tmp8] = Tmp16;
				Tmp8=Tmp8+1;
				if(Tmp8==0) break;	
			}
			ZOTA_SPI_OFF();
	
			T_Addr = L_Addr + L_Addr + L_Addr + L_Addr + 256;
			Over64K=(L_Addr >= 0x4000)? 1 : 0; 
			//pTmp = (UINT8 *)&T_Addr;

			ZOTA_SPI_ON();
			ZOTA_SPI_MASTER_POLLING(spiREAD);
			ZOTA_SPI_MASTER_POLLING(Over64K);
			ZOTA_SPI_MASTER_POLLING(T_Addr>>8);
			ZOTA_SPI_MASTER_POLLING(T_Addr);
		
			Tmp8=0;
			while(1)
			{
				Tmp16 = ZOTA_SPI_MASTER_POLLING(0);
				PageBuf[256+Tmp8] = Tmp16;
				Tmp8=Tmp8+1;
				if(Tmp8==0) break;	
			}
			ZOTA_SPI_OFF();

		}
		//
		//	int-flash write
		//
		if(op_int_flash_write)
		{
			P0_0 = 0;
			P0_1 = 0;
			P0_2 = ~P0_2;

			while(xFCN_STS0&0x01);

			i = 0;
			dst = L_Addr;
			len = 128;
			
			while(len)
			{
				//pTmp = (UINT8 *)&dst;

				xFCN_ADR0 = dst;
				xFCN_ADR1 = L_Addr>>8;

				xFCN_DAT3 = PageBuf[i+3];
				xFCN_DAT2 = PageBuf[i+2];
				xFCN_DAT1 = PageBuf[i+1];
				xFCN_DAT0 = PageBuf[i+0];											
				
				xFCN_CMD  = 0x01;
				
				_nop_();
				_nop_();
				_nop_();
				_nop_();
		
				i   += 4; 		/* byte addr */
				dst += 1; 		/* word addr */
				len -= 1; 		/* word wlen */
			}
			while(xFCN_STS0 & 0x01);
		}
		//
		//
	}//for loop

	//
	//
	xFCN_CMD	=0x80;		/* protect bit set */
	xFCN_ADR1	=0x70;		/* address register parking at 0x6000-0x7FFF */
	xFCN_ADR0	=0x00;
	xFCN_LEN1	=0x00;		/* length register parking with 0x0000 */
	xFCN_LEN0	=0x00;
	xFCN_TERS1	=0x00;		/* flash timing parameters parking with all zero */
	xFCN_TERS0	=0x00;
	xFCN_TME1	=0x00;
	xFCN_TME0	=0x00;
	xFCN_TPRG	=0x00;
	xFCN_TRCV	=0x00;
	xFCN_THV1	=0x00;
	xFCN_THV0	=0x00;
	xFCN_TNVS	=0x00;
	xFCN_TNVH	=0x00;
	xFCN_TPGS	=0x00;
	xFCN_TNVH1	=0x00;
	//
	//

	if(op_reboot)
	{
		EA=0;
		WDT = 0x01;
		WDT |= 0x10;
		WDT |= 0x08;
		while(1) ;
	}

	JP_REBOOT_RET:
	P0OEN = _P0OEN;
	EA = _EA;

	WDT |= 0x10;	// set bit[4]. WDT Write Enable
	WDT |= 0x08;	// set bit[3]. WDT Enable
	
}


	


	

UINT8 ZOTA_SPI_MASTER_POLLING(UINT8 SendByte)
{
UINT8	Tmp8;
UINT8	_EA;

_EA=EA;
EA=0;

	xSPCR &= ~(0x01<<7);	// Clear SPI Interrupt
	xSPDR = SendByte;
	// Polling
	while(1) if(xSPSR & 0x80) break;

	Tmp8 = xSPDR;
	xSPSR &= ~(0x01<<7);			// Int Clear, bit[7]=0
	if(xSPSR & 0x40) xSPSR &= 0xBF;	// Write-Collision Clear, bit[6]=0

EA=_EA;


	return	Tmp8;
}
	

#define	SPI_CSN		P3_7
#define	SPI_CLK		P3_6
#define	SPI_DO		P3_5
#define	SPI_DI		P3_4

void ZOTA_SPI_SET(UINT8 Ena, UINT8 Mode, UINT8 Speed)
{
	UINT8	Div;

	if(Speed == 1)		Div = 0x05;
	else if(Speed == 2)	Div = 0x03;
	else if(Speed == 3)	Div = 0x04;
	else if(Speed > 9)	Div = 0x0B;
	else				Div = Speed + 2;

	xSPCR = 0x00;
	xSPER = 0x00;

	xSPCR |= Div & 0x03 ;
	xSPER |= (Div & 0x0C) >> 2;	
	
	if(Ena)
	{
		if(Mode & 0x10)	// Master
		{
			SPI_DI = 1;		
			SPI_DO = (Mode & 0x08)? 1 : 0;
			SPI_CLK = (Mode & 0x08)? 1 : 0;
			SPI_CSN = 0;

			P3REN &= 0x0F;		// P3REN = 0000_xxxx
			P3REN |= 0xE0;		// P3REN = 111x_xxxx	--> P3REN = 1110_xxxx
			P3OEN |= 0xF0;		// P3OEN = 1111_xxxx
			P3OEN &= 0x1F;		// P3OEN = 000x_xxxx	--> P3OEN = 0001_xxxx
		}
		else		// Slave Mode
		{
			SPI_DI = 1;		
			SPI_DO = (Mode & 0x08)? 1 : 0;
			SPI_CLK = 1;
			SPI_CSN = 1;

			P3REN &= 0x0F;		// P3REN = 0000_xxxx
			P3REN |= 0x20;		// P3REN = xx1x_xxxx	--> P3REN = 0010_xxxx
			P3OEN |= 0xF0;		// P3OEN = 1111_xxxx
			P3OEN &= 0xDF;		// P3OEN = xx0x_xxxx	--> P3OEN = 1101_xxxx			
		}
	
		xSPCR |= (Mode & 0x3C) ;	
		xSPCR |= 0xC0;		// bit[7;6] = 2'b11, if bit[6] is 1 from 0, RD/WR pointer is reset		
		SPIIE = 1;

		if( (Mode & 0x10) == 0)	// Slave Mode
		{
			xSPDR = 0x55;		// 1st Dummy Write
		}		
		
	}
	else
	{
		xSPCR = 0x00;
		SPIIE = 0;
	}
}

void ZOTA_SPI_ON()
{
	ZOTA_SPI_SET(1,0x10,0x01);
}

void  ZOTA_SPI_OFF()
{
	ZOTA_SPI_SET(0,0x00,0x00);
	// input with pullup
	//
	P3OEN = (1<<7);
	P3REN &= ~(1<<7);
}



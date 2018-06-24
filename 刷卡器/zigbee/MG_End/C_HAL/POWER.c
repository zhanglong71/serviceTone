/*******************************************************************************
	- Chip		: MG2450/2455
	- Vendor		: RadioPulse Inc, 2007.
	- Date		: 2010-08-13
	- Version		: VER 1.90

	[2010-08-13] VER 1.90
	- Modified to support the new version chip whose CHIP ID is 0x96 in ZHAL_PD_SET().

	[2010-02-22] VER 1.80
	- Update ZSYS_PowerDown(), ZHAL_PD_SET() for MG245XA(xCHIPID is 0x94)
	For details, search "xCHIPID" in this file.
	- When Mode is 1, RTDLY 0x20 --> 0x71 in ZHAL_PD_SET()
	- Remove unused routines in ZHAL_PD_SET()
		xPDM &= ~(0x03<<4);			// clear bit[5:4]
		INT_LVL = (IntLevel & 0x03) << 4;
		xPDM |= INT_LVL;
	- Remove unused routines in ZHAL_PD_SET()
		RTCIP = 1
		RTCIE = 1
	- Modified
		if(WakeIntSrc & 0x01)		if(P3_2 == 0)	goto JP_PD_SET_RETURN;
		if(WakeIntSrc & 0x02)		if(P3_3 == 0)	goto JP_PD_SET_RETURN;
		==>
		if(P3_2 == 0)	goto JP_PD_SET_RETURN;
		if(P3_3 == 0)	goto JP_PD_SET_RETURN;

	[2009-03-09] VER 1.76
	- TimeoutTick30us = TimeoutSecond * 32768;
		==>
	   TimeoutTick30us = 32768;
	   TimeoutTick30us *= TimeoutSecond;

	[2009-02-23] VER 1.75
	- PLL_LOCK_EXPIRE_COUNTER : 0x0200 --> 0x0400
	- ZHAL_PD_SET() is updated.
	- <#if ENABLE_UART0 ~ #endif> is added	
	- <#if ENABLE_UART1 ~ #endif> is added

	[2008-07-28] VER 1.70
	- ZHAL_IDLE_MODE() : ZHAL_PWRMNGT_SET() is not called to support Wakeup-By-RF-Interrupt.
	- ZHAL_PD_SET() : modified for more stable power-down operation.

	[2008-04-16] VER 1.61
	- Parameter 'BatteryMonitoring' is renamed as 'ProtectFlashErase'
	- ZHAL_3V_LOGIC_INIT() is not called in ZSYS_PowerDown().
	- "xPDCON |= 0x04" --> "xPDCON = 0xEC" in ZHAL_PD_SET()

	[2008-03-06] VER 1.60
	- ZHAL_PWRMNGT_SET() is modified(xBIASPD/PU). ADC is not power-off.
	- ZSYS_PowerDown() is modifield to support BatteryMonitoring

	[2007-12-04] VER 1.51
	- Function Comments are changed in ZHAL_PD_SET() and ZSYS_PowerDown()
	
	[2007-11-07] VER 1.50
	- Initial Version	
*******************************************************************************/

#include "INCLUDE_TOP.h"

#include "C_HAL/PHY.h"
#include "C_HAL/WDT.h"
#include "C_HAL/UART.h"

	#define	PLL_LOCK_EXPIRE_COUNTER	0x0400

// 'AdcOff' with value 1 is not permitted. It is permitted to only ZSYS_PowerDown()
void ZHAL_PWRMNGT_SET(UINT8 RF_PowerOff)
{
	UINT16	ExpireCounter;

	if(RF_PowerOff) 
	{		
		xRXRFPU	= 0xFF;
		xTXRFPU	= 0xFF;
		xPLLPU	= 0xFF;
		xBIASPU	= 0xF9;
		
		xRXRFPD	= 0x00;
		xTXRFPD	= 0x00;
		xPLLPD	= 0x00;
		xBIASPD = 0x06;

		// Disable Unused Clock
		xCLKON0 &= ~(1<<2);		// bit[2]=0
		xCLKON1 = 0x00;			// bit[7:0]=0
		xDCCCON &= ~(0x01);		// bit[0]=0

		xPLLADFC &= ~(1<<6);		// bit[6]=0, AFC Disable
	}
	else 
	{		
		// Enable Used Clock
		xCLKON0 |= (1<<2);		// bit[2]=1
		xCLKON1 = 0x7B;			// bit[7:0]=0x7B
		xDCCCON |= (0x01);  		// bit[0]=1
		
		xBIASPD	= 0xFF;
		xBIASPU	= 0x00;
		xPLLPD	= 0xFF;	
		xRXRFPD	= 0x00;
		xTXRFPD	= 0x00;
		xPLLPU	= 0x00;	
		xPLLADFC=0xEF;		

		// Wait for PLL-Locking
		xPLLLD |= 0x80;
		ExpireCounter = PLL_LOCK_EXPIRE_COUNTER;
		while(1)
		{
			if(xPLLLD & 0x40)	break;
			ExpireCounter--;
			if(ExpireCounter == 0)
			{
				xPLLADFC = 0xEF;
				ExpireCounter = PLL_LOCK_EXPIRE_COUNTER;
			}
		}
		xRXRFPD= 0xFF;
		xTXRFPD	= 0xFF;
	}	

}




// Wakeup Source : UART0, UART1, RF, VOICE, RTC Interrupt
void ZHAL_IDLE_MODE()
{
	UINT8	_IE;
	UINT8	_EIE;

	ZSYS_WDT_SET(0);

	//ZHAL_PWRMNGT_SET(1);		// deleted in VER1.70 of POWER.C

	_IE = IE;
	_EIE = EIE;
	
	EIE = 0xFF;
	IE = 0xFF;

	P0OEN |= ( 1 << 2);
	P0OEN |= ( 1 << 3);

	PCON = 0x01;
	_nop_();_nop_();

	//ZHAL_PWRMNGT_SET(0);		// deleted in VER1.70 of POWER.C
	
	EA = 0;
	EIE = _EIE;
	IE = _IE;	
}

//--	TimeoutTick30us	: 0x0 ~ 0x007FFFFF

//--	IntLevel
//	bit[0]	: EXT0_INT Wakeup Edge. 0=Rising Edge. 1=Falling Edge 
//	bit[1]	: EXT1_INT Wakeup Edge. 0=Rising Edge. 1=Falling Edge

//--	WakeIntSrc
//	bit[0]	: EXT0_INT
//	bit[1]	: EXT1_INT
//	bit[7]	: RTC Interrupt

//--	Block State in each Mode
//	0	: OSCBUFF=ON	AVREG=ON	DVREG=ON	SleepTimer=ON
//	1	: OSCBUFF=OFF	AVREG=OFF	DVREG=ON	SleepTimer=ON
//	2	: OSCBUFF=OFF	AVREG=OFF	DVREG=OFF	SleepTimer=ON
//	3	: OSCBUFF=OFF	AVREG=OFF	DVREG=OFF	SleepTimer=OFF

//--	Available Wakeup Source in each Mode
//	0	: ---
//	1	: Wakeup Source : H/W Reset, EXT0/1 Interrupt, SleepTimer
//	2	: Wakeup Source : H/W Reset, EXT0/1 Interrupt, SleepTimer
//	3	: Wakeup Source : H/W Reset, EXT0/1 Interrupt
void ZHAL_PD_SET(UINT32 TimeoutTick30us, UINT8 IntLevel, UINT8 WakeIntSrc, UINT8 Mode)
{
//	UINT8	INT_LVL;

	UINT8	_IE;
	UINT8	_EIE;
	UINT8	_TCON;
	UINT8	_EIP;
	UINT8	SleepTimerOff;
	UINT8	_xCLKDIV0;

	IntLevel = IntLevel;				// to remove warning when compiled
	WakeIntSrc = WakeIntSrc;		// to remove warning when compiled

	_EIP = EIP;
	_IE = IE;
	_EIE = EIE;
	_TCON = TCON;

	if(TimeoutTick30us)	SleepTimerOff = 0;
	else					SleepTimerOff = 1;

	EIP = 0;
	EIE = 0;
	IE = 0x80;		// EA=1

	xRTINT0 = (UINT8) TimeoutTick30us;	TimeoutTick30us >>= 8;
	xRTINT1 = (UINT8) TimeoutTick30us;	TimeoutTick30us >>= 8;
	xRTCON = (UINT8) (TimeoutTick30us & 0x7F);

	//
	// VER1.70 of POWER.C
	if( (Mode == 2) || (Mode == 3) )
	{
		xRTDLY = 0x20;		// xRTCON[7]=0 --> WakeupSource=RTDLY. 
		xPLLPD = 0xFF;
		P0SEL |= 0x02;
	}
	else
	{
		xRTDLY = 0x71;		// xRTCON[7]=0 --> WakeupSource=RTDLY. Min=0x20
		P0SEL |= 0x02;	
	}

	if(Mode == 1)
	{
		if(xCHIPID >= 0x96)
		{
			// 0x08 : SleepTimer(bit4)=0(Off)	RCOSCEN(bit3)=1(On)	BOD(bit2)=0(On, reversed polarity)
			// 0x18 : SleepTimer(bit4)=1(On)	RCOSCEN(bit3)=1(On)	BOD(bit2)=0(On, reversed polarity)
			xPDCON = (SleepTimerOff)? 0x08 : 0x18;
		}
		else if(xCHIPID >= 0x94)
		{
			// 0x0C : SleepTimer(bit4)=0(Off)	RCOSCEN(bit3)=1(On)	BOD(bit2)=1(On)
			// 0x1C : SleepTimer(bit4)=1(On)	RCOSCEN(bit3)=1(On)	BOD(bit2)=1(On)
			xPDCON = (SleepTimerOff)? 0x0C : 0x1C;
		}
		else
		{
			// 0x08 : SleepTimer(bit4)=0(Off)	RCOSCEN(bit3)=1(On)	BOD(bit2)=0(Off)
			// 0x18 : SleepTimer(bit4)=1(On)	RCOSCEN(bit3)=1(On)	BOD(bit2)=0(Off)
			xPDCON = (SleepTimerOff)? 0x08 : 0x18;
		}
	}
	else if(Mode == 2)
	{
		if(xCHIPID >= 0x96)
		{
			// 0x08 : SleepTimer(bit4)=0(Off)	RCOSCEN(bit3)=1(On)	BOD(bit2)=0(On, reversed polarity)
			// 0x18 : SleepTimer(bit4)=1(On)	RCOSCEN(bit3)=1(On)	BOD(bit2)=0(On, reversed polarity)
			xPDCON = (SleepTimerOff)? 0x08 : 0x18;		
		}
		else
		{
			// 0x0C : SleepTimer(bit4)=0(Off)	RCOSCEN(bit3)=1(On)	BOD(bit2)=1(On)
			// 0x1C : SleepTimer(bit4)=1(On)	RCOSCEN(bit3)=1(On)	BOD(bit2)=1(On)
			xPDCON = (SleepTimerOff)? 0x0C : 0x1C;
		}		
	}	
	else if(Mode == 3)
	{
		if(xCHIPID >= 0x96)
		{
			// 0x08 : SleepTimer(bit4)=0(Off)	RCOSCEN(bit3)=1(On)	BOD(bit2)=0(On, reversed polarity)
			xPDCON = 0x08;
		}
		else
		{
			// 0x0C : SleepTimer(bit4)=0(Off)	RCOSCEN(bit3)=1(On)	BOD(bit2)=1(On)
			xPDCON = 0x0C;
		}
	}
	else					return;				// Mode0	
	
	xPDCON |= (Mode & 0x03);		// PowerDown Mode=1, 2, 3


	if(P3_2 == 0)	goto JP_PD_SET_RETURN;
	if(P3_3 == 0)	goto JP_PD_SET_RETURN;

	if(xCHIPID >= 0x94)
	{
		_xCLKDIV0 = xCLKDIV0;
		xCLKDIV0 = 0xFF;			// @8MHz Mode
		_nop_();	_nop_();	_nop_();	_nop_();	_nop_();		
	}

	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	#pragma asm
	MOV	R0, #02H
	MOV	DPTR, #022F0H
	MOV	A, #01H
	MOVX	@DPTR, A
	MOV		87H, R0
	#pragma endasm
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	
	if(xCHIPID >= 0x94)
	{
		xCLKDIV0 = _xCLKDIV0;
		_nop_();	_nop_();	_nop_();	_nop_();	_nop_();
	}

	EA = 0;
	// Forced Watchdog Reset
	if( (Mode == 2) || (Mode == 3) )
	{
		WDT |= 0x10; 	// set bit[4]. WDT Write Enable
		WDT |= 0x08; 	// set bit[3]. WDT Enable
		while(1);		// Watchdog Reset
	}

	JP_PD_SET_RETURN:
	P0SEL &= ~0x02;
	TCON = _TCON;		// IT0, IT1
	EIE = _EIE;
	IE = _IE;
	EIP = _EIP;

	//xPDCON |= 0x04;		// by YJH. DBODEN(Brown Out Detector is enabled)
	if(xCHIPID >= 0x96)
	{
		xPDCON = 0xE8;		// 1110_1000. BOD(Bit2)=0(Enabled, reversed polarity)
	}
	else
	{
		xPDCON = 0xEC;		// 1110_1100. BOD(Bit2)=1(Enabled)
	}

}

//--	TimeoutSecond	: 0x0000 ~ 0xFFFF for MG245XA(xCHIPID is 0x94). For lower xCHIPID, 0x0000 ~ 0x00FF

//--	Block State in each Mode
//	0	: OSCBUFF=ON	AVREG=ON	DVREG=ON	SleepTimer=ON
//	1	: OSCBUFF=OFF	AVREG=OFF	DVREG=ON	SleepTimer=ON
//	2	: OSCBUFF=OFF	AVREG=OFF	DVREG=OFF	SleepTimer=ON
//	3	: OSCBUFF=OFF	AVREG=OFF	DVREG=OFF	SleepTimer=OFF

//--	Available Wakeup Source in each Mode
//	0	: ---
//	1	: Wakeup Source : H/W Reset, EXT0/1 Interrupt, SleepTimer
//	2	: Wakeup Source : H/W Reset, EXT0/1 Interrupt, SleepTimer
//	3	: Wakeup Source : H/W Reset, EXT0/1 Interrupt
	extern	UINT8	ProtectFlashErase;
void ZSYS_PowerDown(UINT16 TimeoutSecond, UINT8 Mode)
{	
	UINT8	_P0OEN;
	UINT8	_P1OEN;
	UINT8	_P3OEN;
	UINT8	_P0REN;
	UINT8	_P1REN;
	UINT8	_P3REN;
	UINT8	_CLKON0;
	UINT8	_SADCCON;
	
	UINT32	TimeoutTick30us;

	//TimeoutTick30us = TimeoutSecond * 32768;		// Cast Operation Error in some case
	if(xCHIPID >= 0x94)
			TimeoutTick30us = 128;		// 1 tick of MG245XA is 7808 us. So, 128 * 7808 us is about 1 second.
	else		TimeoutTick30us = 32768;	// 1 tick of MG245X is 30.5 us. So, 32768 * 30.5 us is about 1 second.
	TimeoutTick30us *= TimeoutSecond;
	
	// RC Calibration 
	xPDCON |= 0x08;			// <== RC Oscillation Enable
	xRCOSC1 |= 0x80;		// <== RCCEN enable

	//-----------------------------------------------------------
	// External RTC Clock is used, this line should be added.
	//-----------------------------------------------------------
	//xRCOSC1 |= 0x40;		// Enable External RTC Clock
	//-----------------------------------------------------------

	// AES BLOCK OFF
	_CLKON0 = xCLKON0;
	xCLKON0 &= ~(1<<4);		// bit[4]=0, AESCLK=0	

	 // GPIOs Input : Pull-Up
	_P0OEN = P0OEN;	_P1OEN = P1OEN;	_P3OEN = P3OEN;
	_P0REN = P0REN;	_P1REN = P1REN;	_P3REN = P3REN;	 
 	P0OEN = 0xFF;	P1OEN = 0xFF; 	P3OEN = 0xFF;
 	P0REN =0x00;	P1REN =0x00;	P3REN =0x00	;

#if (ENABLE_UART0)
	while(ZSYS_UART0_TX_WORKING());
	while(1) if(xU0_IIR & 0x01) break;	// IF 1, PENDING INTERRUPT	
#endif

#if (ENABLE_UART1)
	while(ZSYS_UART1_TX_WORKING());	
	while(1) if(xU1_IIR & 0x01) break;	// IF 1, PENDING INTERRUPT	
#endif

	//BatteryMonitoring = 0;	// Deleted in VER1.61 of POWER.C	
	ProtectFlashErase = 0;	// Added in VER1.70 of POWER.C

	// SDAC OFF
	_SADCCON = xSADCCON;
	xSADCCON = 0x00;		// bit[7:0] = 0

	ZHAL_PWRMNGT_SET(1);
	xBIASPU	= 0xFF;
	xBIASPD	= 0x00;

	ZHAL_PD_SET(TimeoutTick30us, 0x00, 0xFF, Mode);

	ZHAL_PWRMNGT_SET(0);

	// RESTORE : SADC
	xSADCCON = _SADCCON;

	//BatteryMonitoring = 1;	// Deleted in VER1.61 of POWER.C
	ProtectFlashErase = 1;	// Added in VER1.70 of POWER.C

	// RESTONE : AES BLOCK
	xCLKON0 = _CLKON0;

	// RESTORE : GPIOs
	P0OEN = _P0OEN;	P1OEN = _P1OEN;	P3OEN = _P3OEN;
	P0REN = _P0REN;	P1REN = _P1REN;	P3REN = _P3REN;

	//if(Mode == 1)	ZHAL_3V_LOGIC_INIT();	// Deleted in VER1.61 of POWER.C
}



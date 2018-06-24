/*******************************************************************************
	- Chip		: MG2450/2455
	- Vendor		: RadioPulse Inc, 2007.
	- Date		: 2009-02-23
	- Version		: VER 1.70

	[2009-02-23]	VER 1.70
	- <#if ENABLE_COUNTER_UNCALLED ~ #endif> is added	
	- In ZHAL_TIMER3_SET()
		+ Delete Timer3_Reload_TH3, Timer3_Reload_TL3
		+ Timer3_Reload_TH3 = Reload32 >> 8; 	==> TH3 = Reload32 >> 8;
		+ Timer3_Reload_TL3 = Reload32; 		==> TL3 = Reload32;
		+ Bug : if(us > 0x2FFF)	Reload32 = 0x2FFF; ==> Reload32 = 0x2FFF << 1;
	- In ZHAL_TIMER2_SET()
		+ Delete Timer2_Reload_TH2, Timer2_Reload_TL2
		+ Timer2_Reload_TH2 = Reload16 >> 8; 	==> TH2 = Reload16 >> 8;
		+ Timer2_Reload_TL2 = Reload16; 		==> TL2 = Reload16;
		+ Bug : overflow
			if(us == 0)	Reload16 = 0;
			else			Reload16 = 2*us - 1;
				==>
			if(us == 0)			Reload16 = 0;
			else if(us > 0x7FFF)	Reload16 = (0x7FFF << 1) - 1;		
			else					Reload16 = (us << 1) - 1;

	[2007-07-28] VER 1.61
	- ZHAL_COUNTER0_SET() : Only Comment is changed(Source is NOT changed)
		+ Counter Input : P0.6 ==> P3.4
	- ZHAL_COUNTER1_SET() : Only Comment is changed(Source is NOT changed)
		+ Counter Input : P0.7 ==> P3.5

	[2007-12-11] VER 1.60
	- followings in ZHAL_TIMER0_SET() : (Because, Core Clock is changed from 16MHz to 8MHz)
		if(ms > 98)		_MS = 98;
		else				_MS = ms;
	- followings in ZHAL_TIMER1_SET() : (Because, Core Clock is changed from 16MHz to 8MHz)
		if(ms > 98)		_MS = 98;
		else				_MS = ms;		
	- followings in ZHAL_TIMER2_SET() : (Because, Core Clock is changed from 16MHz to 8MHz)
		if(us == 0)	Reload16 = 0;
		else			Reload16 = us - 1;
	- followings in ZHAL_TIMER3_SET() : (Because, Core Clock is changed from 16MHz to 8MHz)
		if(us > 0x5FFF)	Reload32 = 0x5FFF;
		else				Reload32 = us;
	
	[2007-11-07] VER 1.50
	- Initial Version	
*******************************************************************************/

#include "INCLUDE_TOP.h"

	extern 	UINT8 Timer0_Reload_TL0;
	extern 	UINT8 Timer0_Reload_TH0;

//-------------------------------------------------
// MODE0/1/2/3 is supported
//-------------------------------------------------
// ms(milli-second) @ 8MHz 		: 1 ~ 98
// ms(milli-second) @ 16MHz	: 1 ~ 49
//-------------------------------------------------
void ZHAL_TIMER0_SET(UINT8 IntEna, UINT8 Priority, UINT8 Run, UINT8 ms) 
{
	UINT32	Reload32;
	UINT8	_EA;
	UINT8	_MS;

	_EA = EA;
	EA = 0;

	#if (OPERATE_AT_16MHz)
		if(ms > 49)		_MS = 49;
		else				_MS = ms << 1;
	#else
		if(ms > 98)		_MS = 98;
		else				_MS = ms;
	#endif

	Reload32 = 2000;
	Reload32 = Reload32 * _MS;		
	Reload32 = (Reload32 / 3) & 0x0000FFFF;
	Reload32 = 0xFFFF - Reload32 + 1;

	Timer0_Reload_TH0 = Reload32 >> 8;
	Timer0_Reload_TL0 = Reload32;

	TH0 = Timer0_Reload_TH0;
	TL0 = Timer0_Reload_TL0;	

	TMOD &= 0xF0;
	TMOD |= 0x01;
	
	PT0 = Priority;

	if(Run)		TR0 = 1;
	else			TR0 = 0;

	if(IntEna)	ET0 = 1;
	else			ET0 = 0;

	EA = _EA;
	

}


#if (ENABLE_COUNTER_UNCALLED)
// Counter Input : P3.4
void ZHAL_COUNTER0_SET(UINT8 IntEna, UINT8 Priority, UINT8 Run, UINT16 CountNum)
{
	UINT16	Reload16;
	UINT8	_EA;

	_EA = EA;
	EA = 0;

	Reload16 = 0xFFFF - CountNum + 1;
	Timer0_Reload_TH0 = Reload16 >> 8;
	Timer0_Reload_TL0 = Reload16;

	TH0 = Timer0_Reload_TH0;
	TL0 = Timer0_Reload_TL0;

	TMOD &= 0xF0;		// Clear bit[3:0], 12 bit counter
	TMOD |= 0x04;		// C/T0# = 1 : Counter Mode Operation

	PT0 = Priority;

	if(Run)		TR0 = 1;
	else			TR0 = 0;

	if(IntEna)	ET0 = 1;
	else			ET0 = 0;

	EA = _EA;
}
#endif


	extern 	UINT8 Timer1_Reload_TL1;
	extern 	UINT8 Timer1_Reload_TH1;
	
//-------------------------------------------------
// MODE0/1/2/3 is supported
//-------------------------------------------------
// ms(milli-second) @ 8MHz 		: 1 ~ 98
// ms(milli-second) @ 16MHz	: 1 ~ 49
//-------------------------------------------------
void ZHAL_TIMER1_SET(UINT8 IntEna, UINT8 Priority, UINT8 Run, UINT8 ms) 
{

	UINT32	Reload32;
	UINT8	_EA;
	UINT8	_MS;

	_EA = EA;
	EA = 0;	

	#if (OPERATE_AT_16MHz)
		if(ms > 49)		_MS = 49;
		else				_MS = ms << 1;	
	#else
		if(ms > 98)		_MS = 98;
		else				_MS = ms;
	#endif
	
	Reload32 = 2000;
	Reload32 = Reload32 * _MS;		
	Reload32 = (Reload32 / 3) & 0x0000FFFF;
	Reload32 = 0xFFFF - Reload32 + 1;

	Timer1_Reload_TH1 = Reload32 >> 8;
	Timer1_Reload_TL1 = Reload32;

	TH1 = Timer1_Reload_TH1;
	TL1 = Timer1_Reload_TL1;
	
	TMOD &= 0x0F;		// clear bit[7:4]
	TMOD |= 0x10;		// Mode=1(16 bit timer)	
	
	PT1 = Priority;

	if(Run)		TR1 = 1;
	else			TR1 = 0;

	if(IntEna)		ET1 = 1;
	else			ET1 = 0;

	EA = _EA;

}


#if (ENABLE_COUNTER_UNCALLED)
// Counter1 Input : P3.5
void ZHAL_COUNTER1_SET(UINT8 IntEna, UINT8 Priority, UINT8 Run, UINT16 CountNum)
{
	UINT16	Reload16;
	UINT8	_EA;

	_EA = EA;
	EA = 0;

	Reload16 = 0xFFFF - CountNum + 1;
	Timer1_Reload_TH1 = Reload16 >> 8;
	Timer1_Reload_TL1 = Reload16;

	TH1 = Timer1_Reload_TH1;
	TL1 = Timer1_Reload_TL1;

	TMOD &= 0x0F;		// Clear bit[7:4]
	TMOD |= 0x50;		// 16bit Counter Mode and C/T1# = 1 : Counter Mode Operation

	PT1 = Priority;

	if(Run)		TR1 = 1;
	else			TR1 = 0;

	if(IntEna)	ET1 = 1;
	else			ET1 = 0;

	EA = _EA;	

}
#endif

//-------------------------------------------------
// us(micro-second) @ 8MHz 	: 1 ~ 0xFFFF(65535)
// us(micro-second) @ 16MHz	: 1 ~ 0x7FFF(32767)
//-------------------------------------------------
void ZHAL_TIMER2_SET(UINT8 IntEna, UINT8 Priority, UINT8 Run, UINT16 us) 
{
	UINT16	Reload16;
	UINT8	_EA;

	_EA = EA;
	EA = 0;

	#if (OPERATE_AT_16MHz)
	//	if(us == 0)	Reload16 = 0;
	//	else			Reload16 = 2*us - 1;
		if(us == 0)			Reload16 = 0;
		else if(us > 0x7FFF)	Reload16 = (0x7FFF << 1) - 1;		
		else					Reload16 = (us << 1) - 1;
		
	#else
		if(us == 0)	Reload16 = 0;
		else			Reload16 = us - 1;		
	#endif
		
	TH2 = Reload16 >> 8;
	TL2 = Reload16;

	T2IP = Priority;

	if(Run)	{	T23CON |= 0x02;	}
	else		{	T23CON &= 0xFD;	}

	if(IntEna)	T2IE = 1;
	else			T2IE = 0;

	EA = _EA;
}

//-------------------------------------------------
// us(micro-second) @ 8MHz 	: 1 ~ 0x5FFF(24575)
// us(micro-second) @ 16MHz	: 1 ~ 0x2FFF(12287)
//-------------------------------------------------	
/*void ZHAL_TIMER3_SET(UINT8 IntEna, UINT8 Priority, UINT8 Run, UINT16 us)
{
	UINT32	Reload32;
	UINT8	_EA;

	_EA = EA;
	EA = 0;

	#if (OPERATE_AT_16MHz)
		if(us > 0x2FFF)	Reload32 = 0x2FFF << 1;
		else				Reload32 = us << 1;
	#else
		if(us > 0x5FFF)	Reload32 = 0x5FFF;
		else				Reload32 = us;
	#endif
	
	Reload32 = Reload32 << 3;
	Reload32 = Reload32 / 3;
	if(Reload32 == 0)		Reload32 = 0;
	else					Reload32 = Reload32 - 1;

	TH3 = Reload32 >> 8;		// bit[15:8]
	TL3 = Reload32;			// bit[7:0]	
	
	T3IP = Priority;

	if(Run)	{	T23CON |=  0x08; }
	else		{	T23CON &= 0xF7; }

	if(IntEna)	T3IE = 1;
	else			T3IE = 0;

	EA = _EA;
}*/


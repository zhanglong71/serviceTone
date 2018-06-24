/*******************************************************************************
	- Chip		: MG2450/2455
	- Vendor		: RadioPulse Inc, 2007.
	- Date		: 2009-02-23
	- Version		: VER 1.60

	[2009-02-23] VER 1.60
	- ZSYS_WDT_RESET() is added.
	
	[2007-11-07] VER 1.50
	- Initial Version	
*******************************************************************************/

#include "INCLUDE_TOP.h"

//------------------------------------------------
//-- @8MHz System Clock
// Timeout = 0 : 62.5 ms
// Timeout = 1 : 125 ms
// Timeout = 2 : 250 ms
// Timeout = 3 : 500 ms
//------------------------------------------------
//-- @16MHz System Clock
// Timeout = 0 : 31.2 ms
// Timeout = 1 : 62.5 ms
// Timeout = 2 : 125 ms
// Timeout = 3 : 250 ms
//------------------------------------------------

void ZHAL_WDT_SET(UINT8 Ena, UINT8 Timeout) 
{
	UINT8	LocalPre;

	LocalPre = Timeout & 0x03;	
	WDT = LocalPre;
	
	WDT |= 0x10;			// set bit[4]. WDT Write Enable
	if(Ena)	WDT |= 0x08;	// set bit[3]. WDT Enable
	else		WDT &= 0xF7;	// clear bit[3]. Disable	
}

void ZHAL_WDT_RESTART() {
	WDT |= 0x04;			// set bit[2]
}

//
//	Main Timer must have this variable
//
	extern UINT16		WatchDogCount;
	extern UINT8		WatchDogCountEnable;
//
void ZSYS_WDT_SET(UINT16 Timer0Period_Cnt)
{
	UINT8 _ET0;

	_ET0 = ET0;
	ET0 = 0;

	if(Timer0Period_Cnt)		// WDT Enable
	{
		WatchDogCount = Timer0Period_Cnt;
		WatchDogCountEnable = 1;
		ZHAL_WDT_SET(1, 3);
	} 
	else				// WDT Disable
	{
		WatchDogCountEnable = 0;
		ZHAL_WDT_SET(0, 3);
	}

	ET0 = _ET0;
}

//------------------------------------------------
//-- @8MHz System Clock
// Timeout = 0 : 62.5 ms
// Timeout = 1 : 125 ms
// Timeout = 2 : 250 ms
// Timeout = 3 : 500 ms
//------------------------------------------------
//-- @16MHz System Clock
// Timeout = 0 : 31.2 ms
// Timeout = 1 : 62.5 ms
// Timeout = 2 : 125 ms
// Timeout = 3 : 250 ms
//------------------------------------------------
void ZSYS_WDT_RESET(UINT8 Timeout)
{
	EA = 0;
	WDT = Timeout & 0x03;
	WDT |= 0x10; 	// set bit[4]. WDT Write Enable
	WDT |= 0x08; 	// set bit[3]. WDT Enable
	while(1);		// Watchdog Reset
}

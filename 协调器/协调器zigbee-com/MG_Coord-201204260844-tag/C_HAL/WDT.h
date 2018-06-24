/*******************************************************************************
	- Chip		: MG2450/2455
	- Vendor		: RadioPulse Inc, 2007.
	- Date		: 2007-11-07
	- Version		: VER 1.5	
*******************************************************************************/

	extern void ZHAL_WDT_SET(UINT8 Ena, UINT8 Timeout);
	extern void ZHAL_WDT_RESTART();
	extern void ZSYS_WDT_SET(UINT16 Timer0Period_Cnt);
	extern void ZSYS_WDT_RESET(UINT8 Timeout);


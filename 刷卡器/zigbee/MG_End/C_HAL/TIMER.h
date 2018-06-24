/*******************************************************************************
	- Chip		: MG2450/2455
	- Vendor		: RadioPulse Inc, 2007.
	- Date		: 2007-12-11
	- Version		: VER 1.60
*******************************************************************************/

	extern void ZHAL_TIMER0_SET(UINT8 IntEna, UINT8 Priority, UINT8 Run, UINT8 ms) ;
	extern void ZHAL_TIMER1_SET(UINT8 IntEna, UINT8 Priority, UINT8 Run, UINT8 ms);
	extern void ZHAL_TIMER2_SET(UINT8 IntEna, UINT8 Priority, UINT8 Run, UINT16 us); 
	extern void ZHAL_TIMER3_SET(UINT8 IntEna, UINT8 Priority, UINT8 Run, UINT16 us);
	extern void ZHAL_COUNTER0_SET(UINT8 IntEna, UINT8 Priority, UINT8 Run, UINT16 CountNum);
	extern void ZHAL_COUNTER1_SET(UINT8 IntEna, UINT8 Priority, UINT8 Run, UINT16 CountNum);


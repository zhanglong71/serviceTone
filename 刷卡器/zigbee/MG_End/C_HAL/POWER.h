/*******************************************************************************
	- Chip		: MG2450/2455
	- Vendor		: RadioPulse Inc, 2007.
	- Date		: 2008-03-06
	- Version		: VER 1.60
*******************************************************************************/

	extern	void ZHAL_PWRMNGT_SET(UINT8 RF_PowerOff);
	extern	void ZHAL_IDLE_MODE();
	extern	void ZHAL_PD_SET(UINT32 TimeoutTick30us, UINT8 IntLevel, UINT8 WakeIntSrc, UINT8 Mode);
	extern	void ZSYS_PowerDown(UINT16 TimeoutSecond, UINT8 Mode);


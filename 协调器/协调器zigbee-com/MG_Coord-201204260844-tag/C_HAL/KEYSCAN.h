/*******************************************************************************
	- Chip		: MG2450/2455
	- Vendor		: RadioPulse Inc, 2007.
	- Date		: 2007-11-07
	- Version		: VER 1.5	
*******************************************************************************/


	extern 	void ZHAL_KEYSCAN_SET(UINT8 ScanEN, UINT8 P0andEN, UINT8 P0mask);
			//ScanEN
			//	EXT0/1 Interrupt invoked during Low state not edge
			//PoandEN
			//	INPUT : P0[0~7]
			//	FUNCTION : P1.7 = &(P0|P0mask)
			//	 

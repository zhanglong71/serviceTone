
/*******************************************************************************
	- Chip		: MG2450/2455
	- Vendor		: RadioPulse Inc, 2007.
	- Date		: 2010-03-02
	- Version		: VER 2.10	

	[2010-03-02] VER 2.10
	- For ZigBeePRO Development Kit V2.10
*******************************************************************************/

	extern	void Swap_Buffer_4B(UINT8 *pBuf, UINT8 Idx);
	extern	void Swap_Buffer_2B(UINT8 *pBuf, UINT8 Idx);
	extern	UINT8 Check_8Byte_Null(UINT8 *pArray);
	extern	UINT8 Check_16Byte_Null(UINT8 *pArray);
	extern	UINT8 Check_8Byte_Broadcast(UINT8 *pArray);
	extern	UINT8 ZSYS_LQI_TO_COST(UINT8 LQI);
	extern	void ZSYS_RANDOM_GEN_SEED(UINT16 RandomSeed);
	extern	UINT16 ZSYS_RANDOM_GEN_GET(UINT16 MaskAnd);
	extern	void ZSYS_SET_CHANNEL(UINT8 Channel);
	extern	void ZSYS_SET_SHORT_ADDR(UINT16 ShortAddr);
	extern	void ZSYS_SET_PAN_ID(UINT16 PanID);
	extern	void ZSYS_SET_EPID(UINT8 *pEPID);
	extern	UINT16 ZSYS_GET_SHORT_ADDR();
	extern	UINT16 ZSYS_GET_PAN_ID();
	extern	UINT8 ZSYS_GET_CHANNEL();

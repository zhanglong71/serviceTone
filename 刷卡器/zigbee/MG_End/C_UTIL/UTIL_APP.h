
/*******************************************************************************
	- Chip		: MG2450/2455
	- Vendor		: RadioPulse Inc, 2007.
	- Date		: 2010-03-02
	- Version		: VER 2.10	

	[2010-03-02] VER 2.10
	- For ZigBeePRO Development Kit V2.10
*******************************************************************************/

	extern 	void zPrintf(UINT8 Port, char *fmt, ...);
	extern 	void zScanf(UINT8 Port, UINT16 *pValue);
	extern	UINT8 zScanf_Decimal(UINT8 Port, UINT16 *pValue);	
	extern	void Display_Buffer(UINT8 *pBuf, UINT8 Len, UINT8 Linefeed);	
	extern	void Display_NT(UINT8 DispOpt);
	extern	void Display_RT(UINT8 DispOpt);
	extern	void Display_NAM(UINT8 DispOpt);	
	extern	void Display_RRECT(UINT8 DispOpt);
	extern	void Display_GT(UINT8 DispOpt);
	extern	void Display_MyInfo();

	extern	UINT8 ZSYS_HIB_COPY(UINT8 *pBuf);
	extern	INT8 ZSYS_LQI_TO_dBm(UINT8 LQI);
	extern	UINT8 ZSYS_dBm_TO_LQI(INT8 dBmValue);


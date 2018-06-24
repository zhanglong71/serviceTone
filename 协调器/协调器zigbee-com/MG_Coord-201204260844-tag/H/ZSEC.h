
/*******************************************************************************
	- Chip		: MG2450/2455
	- Vendor		: RadioPulse Inc, 2007.
	- Date		: 2010-11-15
	- Version		: VER 2.2

	[2010-03-02] VER 2.2
	- For ZigBeePRO Development Kit V2.2
	
	[2010-03-02] VER 2.10
	- For ZigBeePRO Development Kit V2.10
*******************************************************************************/

	extern	SS_PIB		SIB;
	extern	SS_MATERIAL_OUT	OutgoMaterial;
	extern	SS_MATERIAL_IN		IncomeMaterial;
	extern	UINT8				NumNwkSecMat;	
	extern	UINT8				NumApsKeyPair;		

	extern	UINT8 ZSEC_FindNwkSecMaterial_WithKeySeq(UINT8 KeySeqNum, UINT8 FindOption);
	extern	UINT8 ZSEC_SetNwkSecMaterial(SS_NwkSecMaterial *pMaterial);

	extern	UINT8 ZSEC_FindApsKeyPair_WithIEEE(UINT8 *pInIEEE);
	extern	UINT8 ZSEC_SetApsKeyPair(SS_ApsDevKeyPair *pKeyPair);


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

#define	secSTA_SUCCESS			0x00
#define	secSTA_INVALID_PARAMETER	0x01
#define	secSTA_NO_MASTER_KEY		0x02
#define	secSTA_INVALID_CHALLENGE	0x03
#define	secSTA_INVALID_SKG		0x04
#define	secSTA_INVALID_MAC		0x05
#define	secSTA_INVALID_KEY		0x06
#define	secSTA_TIMEOUT			0x07
#define	secSTA_BAD_FRAME			0x08


#define	MG245X_SUITE_NONE	0
#define	MG245X_SUITE_CBC		1
#define	MG245X_SUITE_CTR		2
#define	MG245X_SUITE_CCM		3

//	ZigBee Security : Suite

#define	ZBSEC_SUITE_NONE		0
#define	ZBSEC_SUITE_MIC32		1
#define	ZBSEC_SUITE_MIC64		2
#define	ZBSEC_SUITE_MIC128	3
#define	ZBSEC_SUITE_ENC		4
#define	ZBSEC_SUITE_CCM32		5
#define	ZBSEC_SUITE_CCM64		6
#define	ZBSEC_SUITE_CCM128	7

//	ZigBee Security : Key ID
#define	ZBSEC_KEYID_LINK			0
#define	ZBSEC_KEYID_NWK			1
#define	ZBSEC_KEYID_KEY_TRANS	2
#define	ZBSEC_KEYID_KEY_LOAD		3

typedef struct
{
	// Input
	UINT8	Suite;			// 0 ~ 7
	UINT8	KeyID;
	UINT8	ExtNonce;
	UINT8	FrameCnt[4];	
	UINT8	KeySeq;			// If KeyID=1
	UINT8	MLen;			// Message(Payload) Length
	UINT8	HLen;			// Header Length
	UINT8	*pSenderIEEE;
	UINT8	*pKey;
	UINT8	*pInText;
	UINT8	*pOutText;

	// Output
	UINT8	OutLen;
} SS_MATERIAL_OUT;

typedef struct
{
	// Input	
	UINT8	Suite;			// 0 ~ 7
	UINT8	MLen;			// Message(Payload) Length
	UINT8	HLen;			// Header Length
	UINT8	*pSenderIEEE;
	UINT8	*pKey;
	UINT8	*pInText;	
	UINT8	*pOutText;

	// Output
	UINT8	OutLen;
} SS_MATERIAL_IN;

typedef struct
{
	UINT8	KeySeqNum;
	UINT8	OutFrameCnt[4];
	UINT8	Key[16];
	UINT8	KeyType;	// 1=standard NWK key, 5=high-security NWK key
//	UINT8	SenderIEEE_0[8];
//	UINT8	InFrameCnt_0[4];
//	UINT8	SenderIEEE_1[8];
//	UINT8	InFrameCnt_1[4];
	UINT8	EnaSM;
} SS_NwkSecMaterial;

typedef struct
{
	UINT8	DevIEEE[8];
	UINT8	MasterKey[16];
	UINT8	LinkKey[16];
	UINT8	OutFCounter[4];
	UINT8	InFCounter[4];
	UINT8	EnaKP;
} SS_ApsDevKeyPair;

typedef struct
{
	// NWK Layer
	UINT8				nwkSecurityLevel;		// 0xA0 
	SS_NwkSecMaterial	*pnwkSecMaterial;		// 0xA1
	UINT8				nwkActKeySeqNum;		// 0xA2
	UINT8				nwkAllFresh;			// 0xA3
	UINT8				nwkSecureAllFrame;		// 0xA4

	// APS Layer
	SS_ApsDevKeyPair		*papsKeyPairSet;		// 0xAA
	UINT8				apsTrustCenIEEE[8];		// 0xAB
	UINT32				apsTimeoutPeriod;		// 0xAC

	// By RadioPulse	
	UINT8	SIZE_NwkMaterial;		// Entry Size for pnwkSecMaterial
	UINT8	SIZE_ApsKeyPair;		// Entry Size for papsKeyPairSet	
	UINT8	CurrentNwkKeyType;		// 1=Standard, 5=High-Security
	//-	PreconfigMode
	//	0	: None
	//	1	: Secured Network : Preconfigured NWK key. ZCP test cases.
	//	2	: Secured Network : No-preconfigured NWK key. ZCP test cases.
	//	3	: Secured Network : Preconfigured TC link Key. SEP test cases.
	UINT8	PreconfigMode;						// 0xB0
	UINT8	KeyTransportKey[16];				// 0xB1
	UINT8	KeyLoadKey[16];						// 0xB2
	UINT8	PreconfigTCLinkKey[16];				// 0xB3
	UINT8	PreconfigNWKKey[16];				// 0xB4
} SS_PIB;		// Security Information Base


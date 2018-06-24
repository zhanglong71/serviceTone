/*******************************************************************************
	- Chip		: MG2450/2455
	- Vendor		: RadioPulse Inc, 2007.
	- Date		: 2010-01-20
	- Version		: VER 2.10	

	[2010-01-20] VER 2.10
	- New Define : #define	VTimer_MacTimer16	0
	- New Define : #define	VTimer_Timestamp_L	1
	- New Define : #define	VTimer_Timestamp_H	2
	- Modify : #define	maccResponseWaitTime_ms		(26 --> 492)
	- Add parameter in MS_PendingPacket : UINT8 EnaPP

	[2007-12-11] VER 1.61
	- New Define : #define	mFFFE			0xFFFE
	- New Define : #define	mBROADCAST	0xFFFF
	- New Define : #define	mSTA_RP_NO_IMPLEMENT	0xFB
	- New Define : #define	mAssoStatus_SameIEEE	0x11
	- structure MAC_AD --> xxxIEEE in a few primitive
	- mSTA_RP_INDIRECT_FAKE_RETURN --> mSTA_RP_PENDING_OK

	[2007-11-30] VER 1.60
	- All MAC source is rebuilded

	[2007-11-07] VER 1.50
	- Initial Version
*******************************************************************************/


#define	MAC_VERSION_ZIGBEE			1

#define	IMPLEMENT_ORPHAN							1
#define	IMPLEMENT_DISASSOCIATE					0
#define	IMPLEMENT_CONFLICT_AND_REALIGNMENT		0
#define	IMPLEMENT_MCPS_PURGE						0
#define	IMPLEMENT_MLME_RX_ENA					0

//--------------------------------------------------------
//	Virtual Timer
//--------------------------------------------------------
#define	VTimer_MacTimer16		0
#define	VTimer_Timestamp_L	1
#define	VTimer_Timestamp_H	2

//--------------------------------------------------------
//	MAC Constants
//--------------------------------------------------------
#define	maccMaxPHYPacketSize			127
#define	maccTurnaroundTime			12		 	// 192 us
#define	maccBaseSlotDuration			60
#define	maccBaseSuperframeDuration	960			// 60*16
#define	maccMaxBE						5
#define	maccMaxBeaconOverhead		75
#define	maccMaxBeaconPayloadLength	(maccMaxPHYPacketSize - maccMaxBeaconOverhead)
#define	maccGTSDescPersistenceTime	4
#define	maccMaxFrameOverhead			25
#define	maccMaxFrameResponseTime	1220
#define	maccMaxFrameRetries			3
#define	maccMaxLostBeacons			4
#define	maccMaxMACFrameSize			(maccMaxPHYPacketSize - maccMaxFrameOverhead)
#define	maccMaxSIFSFrameSize			18
#define	maccMinCAPLength				440			// 7040 us
#define	maccMinLIFSPeriod				40			// 640 us
#define	maccMinSIFSPeriod				12			// 192 us
#define	maccNumSuperframeSlots		16
#define	maccResponseWaitTime_ms		492			// 
#define	maccUnitBackoffPeriod			20			// 320 us
//--------------------------------------------------------
//- PHY/MAC PIB Attribute Code
//--------------------------------------------------------
#define	mPPIB_Channel				0x00
#define	mPPIB_ChannelSupport		0x01
#define	mPPIB_TransmitPower		0x02
#define	mPPIB_CCAMode				0x03
//--------------------------------------------------------
#define	mMPIB_AckDuration			0x40
#define	mMPIB_AssoPermit			0x41
#define	mMPIB_AutoRequest			0x42
#define	mMPIB_BattLifeExt			0x43
#define	mMPIB_BattLifeExtPeriod		0x44
#define	mMPIB_BeaconPayload		0x45
#define	mMPIB_BeaconPayloadLength	0x46
#define	mMPIB_BeaconOrder			0x47
#define	mMPIB_BeaconTxTime		0x48
#define	mMPIB_BSN					0x49
#define	mMPIB_CoordExtendAddr		0x4A
#define	mMPIB_CoordShortAddr		0x4B
#define	mMPIB_DSN					0x4C
#define	mMPIB_GTSPermit			0x4D
#define	mMPIB_MaxCSMABAckoff		0x4E
#define	mMPIB_MinBE				0x4F
#define	mMPIB_PANId				0x50
#define	mMPIB_PromiscuousMode	0x51
#define	mMPIB_RxOnWhenIdle		0x52
#define	mMPIB_ShortAddr			0x53
#define	mMPIB_SuperframeOrder		0x54
#define	mMPIB_TranPersistTIme		0x55
#define	mMPIB_IEEEAddr			0x7F
//--------------------------------------------------------
//- MAC Status
//--------------------------------------------------------
#define	mSTA_SUCCESS 						0x00
#define	mSTA_BEACON_LOSS 				0xE0  
#define	mSTA_CHANNEL_ACCESS_FAILURE 	0xE1  
#define	mSTA_DENIED 						0xE2
#define	mSTA_DISABLE_TRX_FAILURE 		0xE3  
#define	mSTA_FAILED_SECURITY_CHECK 		0xE4  
#define	mSTA_FRAME_TOO_LONG 			0xE5  
#define	mSTA_INVALID_GTS 					0xE6  
#define	mSTA_INVALID_HANDLE 				0xE7  
#define	mSTA_INVALID_PARAMETER 			0xE8  
#define	mSTA_NO_ACK 						0xE9  
#define	mSTA_NO_BEACON 					0xEA  
#define	mSTA_NO_DATA 						0xEB  
#define	mSTA_NO_SHORT_ADDRESS 			0xEC  
#define	mSTA_OUT_OF_CAP 					0xED  
#define	mSTA_PAN_ID_CONFLICT 			0xEE 
#define	mSTA_REALIGNMENT 				0xEF  
#define	mSTA_TRANSACTION_EXPIRED 		0xF0  
#define	mSTA_TRANSACTION_OVERFLOW 		0xF1  
#define	mSTA_TX_ACTIVE 					0xF2  
#define	mSTA_UNAVAILABLE_KEY 			0xF3 
#define	mSTA_UNSUPPORTED_ATTRIBUTE 		0xF4  
//-------------------------------------------------------- 
#define	mSTA_RP_SEC_START_FAIL			0xF9	// by RadioPulse
#define	mSTA_RP_SEC_FINISH_FAIL			0xFA	// by RadioPulse
#define	mSTA_RP_NO_IMPLEMENT			0xFB	// by RadioPulse
#define	mSTA_RP_IEEE_ERROR				0xFC	// by RadioPulse
#define	mSTA_RP_PENDING_OK				0xFD	// by RadioPulse
#define	mSTA_RP_ASSO_RSP_TIMEOUT		0xFE	// by RadioPulse
#define	mSTA_RP_UNKNOWN					0xFF	// by RadioPulse
//--------------------------------------------------------
//	Security Mode by IEEE 802.15.4
//--------------------------------------------------------
#define	mSecMode_UNSECURE	0x00
#define	mSecMode_ACL			0x01
#define	mSecMode_SECURE		0x02
#define	mSecMode_NO_ACL		0x08
#define	mSecMode_USER0		0x10		// by RadioPulse
//--------------------------------------------------------
//	Security Suite by IEEE 802.15.4
//--------------------------------------------------------
#define	mSecSuite_NONE		0x00
#define	mSecSuite_CTR		0x01
#define	mSecSuite_CCM128	0x02
#define	mSecSuite_CCM64	0x03
#define	mSecSuite_CCM32	0x04
#define	mSecSuite_CBC128	0x05
#define	mSecSuite_CBC64	0x06
#define	mSecSuite_CBC32	0x07
//--------------------------------------------------------
//	Security Level by ZigBee Specification
//--------------------------------------------------------
#define	mSecLevel_NONE		0x00
#define	mSecLevel_MIC32		0x01
#define	mSecLevel_MIC64		0x02
#define	mSecLevel_MIC128		0x03
#define	mSecLevel_ENC			0x04
#define	mSecLevel_ENCMIC32	0x05
#define	mSecLevel_ENCMIC64	0x06
#define	mSecLevel_ENCMIC128	0x07
//--------------------------------------------------------
#define	mFFFE			0xFFFE
#define	mBROADCAST	0xFFFF
//--------------------------------------------------------
//	Address Mode
//--------------------------------------------------------
#define	mAM_NO		0
#define	mAM_16			2
#define	mAM_64			3
//--------------------------------------------------------
//	Frame Type
//--------------------------------------------------------
#define	mFT_BEACON	0
#define	mFT_DATA		1
#define	mFT_ACK		2
#define	mFT_MACCMD	3
//--------------------------------------------------------
//	FrameControl Subfield
//--------------------------------------------------------
#define	mFC_FT		0x07
#define	mFC_SE		0x08
#define	mFC_FP		0x10
#define	mFC_AR		0x20
#define	mFC_IP		0x40
#define	mFC_DAM	0x0C
#define	mFC_SAM	0xC0
//--------------------------------------------------------
//	TxOptions of MCPS-DATA-REQ
//--------------------------------------------------------
#define	mTxOptions_ACK			0x01	// bit[0]
#define	mTxOptions_GTS			0x02	// bit[1]
#define	mTxOptions_INDIRECT	0x04	// bit[2]
#define	mTxOptions_SECURITY	0x08	// bit[3]
//--------------------------------------------------------
//	Scan Type of MLME-SCAN-REQ
//--------------------------------------------------------
#define	mScanType_EnergyDetect	0x00
#define	mScanType_Active			0x01
#define	mScanType_Passive			0x02
#define	mScanType_Orphan			0x03
//--------------------------------------------------------
//	MAC Command ID
//--------------------------------------------------------
#define	mCMD_ASSO_REQ	0x01
#define	mCMD_ASSO_RES	0x02
#define	mCMD_DISA_NOTI	0x03
#define	mCMD_DATA_REQ	0x04
#define	mCMD_PANID_CON	0x05
#define	mCMD_ORPH_NOTI	0x06
#define	mCMD_BEA_REQ		0x07
#define	mCMD_COOR_REAL	0x08
#define	mCMD_GTS_REQ		0x09
#define	mDATA_TRANSFER	0x10
//--------------------------------------------------------
//	COMM-STA-IND ID
//--------------------------------------------------------
#define	mCommStaID_AssoRspExpire	0x10
#define	mCommStaID_AssoRspPend	0x20
#define	mCommStaID_AssoRspOK		0x30
#define	mCommStaID_OrphanRsp		0x40
//--------------------------------------------------------
//	Association Response Status
//--------------------------------------------------------
#define	mAssoStatus_OK			0x00
#define	mAssoStatus_Capacity	0x01
#define	mAssoStatus_Denied		0x02
#define	mAssoStatus_Timeout	0x10	// by RadioPulse
#define	mAssoStatus_SameIEEE	0x11	// by RadioPulse
#define	mAssoStatus_NotPermit	0x12	// by RadioPulse
//--------------------------------------------------------
//	Disassociation Reason of MLME-DISA-REQ
//--------------------------------------------------------
#define	mDISA_BY_COORDI	0x01
#define	mDISA_BY_DEVICE	0x02
//--------------------------------------------------------
//	GTS Direction
//--------------------------------------------------------
#define	mGTS_C2D			0x01	// Coordinator To Device
#define	mGTS_D2C			0x00	// Device To Coordinator
//--------------------------------------------------------
//	MAC Parameters
//--------------------------------------------------------
//#define	mMAX_NUM_PendAddr		4
#define	mMAX_NUM_GTS_List			7
#define	mMAX_LEN_BeaconPayload	16
#define	mMAX_LEN_IndirectBuffer	96
#define	mMAX_LEN_GTS_Buffer		(30+8)
#define	mMAX_LEN_PhyPacket		128
#define	mMAX_ExpireTime_MS		5000	// 5000 ms
#define	mMAX_AssoRsp_Hold_MS		10000	// 1000 ms
#define	mMAX_FilterPersistTime_1ms	48		// 48 ms

//--------------------------------------------------------
//	Packet Structure
//--------------------------------------------------------
typedef struct
{
	UINT8	TLen;			// Total Length
	UINT8	HLen;			// Header Length
	UINT8	MLen;			// MSDU Length

	// Header
	UINT8	FC_L;
	UINT8	FC_H;
	UINT8	DSN;
	UINT8	RxOOB;
	
	UINT8	DstMode;		// 0=No, 2=Short, 3=Extended
	UINT16	DstPanID;	
	UINT16	DstShortAddr;
	UINT8	DstExtAddr[8];

	UINT8	SrcMode;
	UINT16	SrcPanID;	
	UINT16	SrcShortAddr;
	UINT8	SrcExtAddr[8];
	
	// Payload
	UINT8	MSDU[maccMaxPHYPacketSize];

	// Footer
	UINT8	FCS[2];
	UINT8	RSSI_LQI;
	INT8		RSSI_dBm;
	
} MAC_PKT;

typedef union
{
	UINT8	Ext[8];
	UINT16	Short;
} MAC_AD;

typedef struct {
	UINT8	AddrMode;
	UINT8	Index;
	MAC_AD	Addr_U;
	UINT8	Handle;
	UINT8	TxPacket[mMAX_LEN_IndirectBuffer];
	UINT16	ExpireTimer;
	UINT8	HLen;				// For Security
	UINT8	EnaPP;
} MS_PendingPacket;

//--------------------------------------------------------
//	PHY & MAC Information Base
//--------------------------------------------------------
typedef struct {
	// Identifier : 0x00 ~ 0x03
	UINT8	phyCurrentChannel;
	UINT32	phyChannelsSupported;
	UINT8	phyTransmitPower;
	UINT8	phyCCAMode;
} PHY_INFORMATION_BASE;

typedef struct {		// stored in FLASH

	// Identifier : 0x40 ~ 0x4F
	UINT16	macAckWaitDuration;			// 54 ot 120
	UINT8	macAssociationPermit;			// TF
	UINT8	macAutoRequest;				// TF
	UINT8	macBattLifeExt;					// TF
	UINT8	macBattLifeExtPeriods;
	UINT8	macBeaconPayload[16];			// mMAX_LEN_BeaconPayload
	UINT8	macBeaconPayloadLength;
	UINT8	macBeaconOrder;
	UINT32	macBeaconTxTime;
	UINT8	macBSN;
	UINT8	macCoordExtendAddr[8];
	UINT16	macCoordShortAddr;
	UINT8	macDSN;
	UINT8	macGTSPermit;
	UINT8	macMaxCSMABackoffs;
	UINT8	macMinBE;	
	
	// Identifier : 0x50 ~ 0x55
	UINT16	macPANId;
	UINT8	macPromiscuousMode;
	UINT8	macRxOnWhenIdle;	
	UINT16	macShortAddr;
	UINT8	macSuperframeOrder;
	UINT16	macTransactionPersistenceTime;	

	UINT8	IEEE_ExtendAddr[8];		// 0x7F

	// Identifier : 0x70 ~ 0x76
	
	//- macSecurityMode
	// mSecMode_UNSECURE	0x00
	// mSecMode_ACL			0x01
	// mSecMode_SECURE		0x02
	// mSecMode_NO_ACL		0x08
	// mSecMode_USER0		0x10	
	UINT8	macSecurityMode;	// 0x76

	// by RadioPulse
	//- secSecuritySuite
	// mSecSuite_NONE	0x00
	// mSecSuite_CTR		0x01
	// mSecSuite_CCM128	0x02
	// mSecSuite_CCM64	0x03
	// mSecSuite_CCM32	0x04
	// mSecSuite_CBC128	0x05
	// mSecSuite_CBC64	0x06
	// mSecSuite_CBC32	0x07		
	UINT8	secSecuritySuite;	// 0x77	
	UINT32	secNonceFCnt;		// 0x78	
	UINT8	secNonceKeySeq;	// 0x79
	UINT8	secNonceIEEE[8];	// 0x7A
	UINT8	secKey[16];			// 0x7B
	
	UINT16	rsv7C;		// 0x7C
	UINT16	rsv7D;		// 0x7D
	UINT16	rsv7E;		// 0x7E
	
	UINT16	rsv6X[16];	// 0x60 ~ 0x6F	

	// RadioPulse Specific
	UINT8	EnableFFD;
	UINT8	EnablePanCoordi;
	UINT8	JoinedToPanCoordi;
	UINT8	TrackingBO;

	//-- UpdateReport	: If following PPIB/MPIB is changed, corresponding bit is set to 1
	//	bit[15:8]	: reserved
	//	bit[7]	: macRxOnWhenIdle
	//	bit[6]	: macCoordShortAddr
	//	bit[5]	: macCoordExtendAddr
	//	bit[4]	: macAssociationPermit
	//	bit[3]	: macPANId
	//	bit[2]	: macShortAddr
	//	bit[1] 	: phyTransmitPower
	//	bit[0] 	: phyCurrentChannel
	UINT16	UpdateReport;

	UINT8		SIZE_RXQ;
	MAC_PKT		*pRXQ;
	UINT8				SIZE_PendTxQ;
	MS_PendingPacket		*pPendTxQ;
} MAC_INFORMATION_BASE;

//--------------------------------------------------------
//	Structure Definitions for MAC Data
//--------------------------------------------------------

typedef struct {
	UINT8	ExpCounter;	// Counter for expiring when not used
	UINT8	IncCounter;	// Counter for including in Beacon Frame
	UINT8	GtsCtrl;		// GTS Control bits. bit[7]=Enable/Disable, bit[1]=RemoveFlag, bit[0]=Dir		
	UINT16	DevAddr;	// GTS device's short address
	UINT8	StartSlot;
	UINT8	Length;
} MS_GTSList;

typedef struct {
	UINT8		DescCnt;
	MS_GTSList	rsList[mMAX_NUM_GTS_List];
} MS_GTSInfo;

typedef struct {
	// [7]	: Enable/Disable
	// [0]	: Direction(D2C=1, C2D=0)
	UINT8		Ctrl;
	UINT8		SlotInfo;	// [7:4]=Length, [3:0]=StartSlot
	UINT8		BeaconWait;
} MS_DevGTSDesc;

typedef struct {
	MS_DevGTSDesc	DevC2D;
	MS_DevGTSDesc	DevD2C;
} MS_DeviceGTS;

typedef struct {
	UINT8		ListNum;
	UINT8		FlagMode;	// bit[7]=Enable/Disable
} MS_TxGTS;

typedef struct {
	UINT8	ModeFlag;			// bit[7]=Enable/Disable
	UINT16	TimerTickBI;
	UINT16	TimerTickSD;
	UINT16	TimerTickSlot;
	UINT8	CurrSlotNum;
	UINT16	BeaconTick;
} MS_TxBeaManager;

typedef struct {
	UINT8	SyncTiming;			
	UINT8	ModeFlag;			// bit[7]=Ena/Disable, bit[6]=no-tracking sync req, bit[0]=Sync Ok
	UINT8	CurrSlotNum;
	UINT8	LostCnt;
	UINT16	TimerTickBI;
	UINT16	TimerTickSlot;		
	UINT16	BeaconTick;
	UINT32	TimeStamp;
} MS_RxBeaManager;

typedef struct 
{
	UINT16	BO 			: 4 ;
	UINT16 	SO 			: 4 ;
	UINT16 	FinalCap 	: 4 ;
	UINT16 	BattExt 		: 1 ;
	UINT16 	rsv0 		: 1 ;
	UINT16 	PANCoord	: 1 ;
	UINT16 	AssoPermit	: 1 ;
} MS_SuperSpec_Sub;

typedef union 
{
	UINT16				All;
	MS_SuperSpec_Sub	b;
} MS_SuperSpec;

typedef struct 
{
	UINT8	Ena;	
	UINT8	CoordAddrMode;
	UINT16	CoordPANId;
	UINT8	CoordAddr[8];
	UINT8	LogicalChannel;
	MS_SuperSpec	SuperframeSpec;
	UINT8	GTSPermit;
	UINT8	LinkQuality;
	UINT32	TimeStamp;
	UINT8	SecurityUse;
	UINT8	ACLEntry;
	UINT8	SecurityFail;
	UINT8	BeaconPayload[mMAX_LEN_BeaconPayload];	// Beacon Payload
} MS_PanDes;		// PAN Descriptor

typedef struct 
{
	UINT8	Hlen;
	UINT8	Tlen;
	UINT8	*pPay;
} MS_TxPKT;

typedef struct
{
	UINT8	AssoReqExtAddr[8];
	UINT8	TxPacket[25];
	UINT16	ExpireTimer;	
	UINT8	AssoStatus;
	UINT8	EnaAssoBuff;	
} MS_AssoBuff;

typedef struct
{
	UINT8	ProtoID;
	UINT8	Ver_Stack;
	//-- B2
	//	bit[7]	: EndDevCapa. 1=Accept Joining from ZED
	//	bit[6:3]	: DevDepth
	//	bit[2]	: RouterCapa;
	//	bit[1:0]	: reserve	
	UINT8	B2;
	UINT8	EPID[8];
	UINT8	TxOffset[3];
	UINT8	UpdateID;
} S_ZigBee2007;

typedef union
{
	S_ZigBee2007		Z2007;
	UINT8			Buff[mMAX_LEN_BeaconPayload];
} U_BEA_PAY;

typedef struct
{
	UINT16	PanID;
	UINT8	SrcMode;
	MAC_AD	SrcAddr;
	UINT8	BeaPayLen;	
	UINT8	*pBeaPay;
} MS_BeaconInfo;

typedef struct
{
	UINT8	Chan;
	UINT16	PanID;
	UINT8	AddrMode;
	UINT16	ShortAddr;
	UINT8	ExtAddr[8];
	UINT8	SO_BO;			// SuperSpec_L, bit[7:4]=SO, bit[3:0]=BO
	UINT8	SuperSpec_H;	// bit[3:0]=FinalCap, bit[4]=BattExt, bit[5]=rsv, bit[6]=PANCoord, bit[7]=PermitJoin

	UINT8	GTSPermit;
	UINT8	LinkQuality;
	UINT32	TimeStamp;
	UINT8	SecurityUse;
	UINT8	ACLEntry;
	UINT8	SecurityFail;

	UINT8		BeaPayLen;
	U_BEA_PAY	BeaPayload;

	UINT8	EnaZN;			// by RadioPusle
} S_ZIGBEE_NET;

//--------------------------------------------------------
//	MAC Primitives
//--------------------------------------------------------
typedef struct 
{
	UINT8	msduHandle;
} MCPS_PURGE_REQ;

typedef struct 
{
	UINT8	msduHandle;
	UINT8	status;
} MCPS_PURGE_CON;

typedef struct 
{
	UINT8	SrcAddrMode;
	UINT16	SrcPanId;
	MAC_AD	SrcAddr_U;
	UINT8	DstAddrMode;
	UINT16	DstPanId;
	MAC_AD	DstAddr_U;
	UINT8	msduLength;
	UINT8	msduHandle;
	//-- TxOptions
	// bit[7:4]	: OOB. by RadioPulse
	// bit[3]		: Security
	// bit[2]		: Indirect Transmission
	// bit[1] 		: GTS Transfer
	// bit[0]		: Ack Request
	UINT8	TxOptions;
	UINT8	*pmsdu;
} MCPS_DATA_REQ;

typedef struct 
{
	UINT8	msduHandle;
	UINT8	status;
} MCPS_DATA_CON;

typedef struct 
{
	UINT8	SrcAddrMode;
	UINT16	SrcPanId;
	MAC_AD	SrcAddr_U;
	UINT8	DstAddrMode;
	UINT16	DstPanId;
	MAC_AD	DstAddr_U;
	UINT8	msduLength;
	UINT8	msduLinkQuality;
	UINT8	SecurityUse;
	UINT8	ACLEntry;
	UINT8	*pmsdu;
} MCPS_DATA_IND;

typedef struct 
{
	UINT16	PANId;
	UINT8	LogicalChannel;
	UINT8	BeaconOrder;
	UINT8	SuperframeOrder;
	UINT8	PANCoordinator;
	UINT8	BatteryLifeExt;
	UINT8	CoordRealignment;
	UINT8	SecurityEnable;
} MLME_START_REQ;

typedef struct 
{
	UINT8	status;
} MLME_START_CON;

typedef struct 
{
	UINT8	ScanType;
	UINT32	ScanChannels;
	UINT8	ScanDuration;
} MLME_SCAN_REQ;

typedef struct 
{
	UINT8	status;
	UINT8	ScanType;
	UINT32	UnscanChannel;
	UINT8	ResultSize;
	UINT8	*pEnergyList;
	S_ZIGBEE_NET	*pZNET;
} MLME_SCAN_CON;

typedef struct 
{
	UINT8	LogicalChannel;
	UINT8	CoordAddrMode;
	UINT16	CoordPanId;
	MAC_AD	CoordAddr_U;
	//-- CapaInfo
	//	bit[7]	: 1=Allocate Addr. Always 1 in ZigBee 2006
	//	bit[6]	: 1=MAC Security enabled
	//	bit[5:4]	: rsv
	//	bit[3]	: 1=RxOnWhenIdle
	//	bit[2]	: 1=Mains-powered
	//	bit[1]	: 1=(Joining Device is ZR) && (JoinAsRouter=1) ==> 1=FFD
	//	bit[0]	: 1=Alternative Coordinator	
	UINT8	Capability;
	UINT8	SecurityEnable;
} MLME_ASSO_REQ;

typedef struct 
{
	UINT16	AssoShortAddr;
	UINT8	status;
} MLME_ASSO_CON;

typedef struct 
{
	UINT8	AssoIndIEEE[8];
	//-- CapaInfo
	//	bit[7]	: 1=Allocate Addr. Always 1 in ZigBee 2006
	//	bit[6]	: 1=MAC Security enabled
	//	bit[5:4]	: rsv
	//	bit[3]	: 1=RxOnWhenIdle
	//	bit[2]	: 1=Mains-powered
	//	bit[1]	: 1=(Joining Device is ZR) && (JoinAsRouter=1) ==> 1=FFD
	//	bit[0]	: 1=Alternative Coordinator	
	UINT8	CapaInfo;
	UINT8	SecurityUse;
	UINT8	ACLEntry;
} MLME_ASSO_IND;

typedef struct 
{
	UINT8	AssoRspIEEE[8];
	UINT16	AssoShortAddr;
	UINT8	status;
	UINT8	SecurityEnable;
} MLME_ASSO_RSP;

typedef struct 
{
	UINT8	SetDefaultPIB;
} MLME_RESET_REQ;

typedef struct 
{
	UINT8	status;
} MLME_RESET_CON;

typedef struct 
{
	UINT8	PIBAttribute;
	UINT8	PIBAttributeValue[16];
} MLME_SET_REQ;

typedef struct 
{
	UINT8	status;
	UINT8	PIBAttribute;
} MLME_SET_CON;


typedef struct 
{
	UINT8	PIBAttribute;
} MLME_GET_REQ;

typedef struct 
{
	UINT8	status;
	UINT8	PIBAttribute;
	UINT8	PIBAttributeValue[16];
	UINT8	PIBAttributeLength;
} MLME_GET_CON;

typedef struct 
{
	MAC_AD	DevAddr_U;
	UINT8	DisaReason;
	UINT8	SecurityEnable;	
	UINT8	DevAddrMode;
	UINT16	DevPANId;
} MLME_DISA_REQ;

typedef struct 
{
	UINT8	status;		
} MLME_DISA_CON;

typedef struct 
{
	UINT8	DisaIndIEEE[8];
	UINT8	DisaReason;
	UINT8	SecurityUse;
	UINT8	ACLEntry;
} MLME_DISA_IND;

typedef struct 
{
	//-- GTS Character
	// bit[5] : 1=allocate, 0=deallocate
	// bit[4] : 1=C2D, 0=D2C
	// bit[3:0] : Slot length
	UINT8	GTSCharacter;		
	UINT8	SecurityEnable;
} MLME_GTS_REQ;

typedef struct 
{
	UINT8	GTSCharacter;
	UINT8	status;
} MLME_GTS_CON;

typedef struct 
{
	UINT16	DevAddr;
	UINT8	GTSCharacter;
	UINT8	SecurityUse;
	UINT8	ACLEntry;
} MLME_GTS_IND;

typedef struct 
{
	UINT8	CoordAddrMode;
	UINT16	CoordPanId;
	MAC_AD	CoordAddr_U;
	UINT8	SecurityEnable;
} MLME_POLL_REQ;

typedef struct 
{
	UINT8	status;	
} MLME_POLL_CON;

typedef struct 
{
	UINT8	DeferPermit;
	UINT32	RxOnTime;
	UINT32	RxOnDuration;
} MLME_RX_ENA_REQ;

typedef struct 
{
	UINT8	status;	
} MLME_RX_ENA_CON;

typedef struct 
{
	UINT8	OrpIndIEEE[8];
	UINT8	SecurityUse;
	UINT8	ACLEntry;
} MLME_ORPH_IND;

typedef struct 
{
	UINT8	OrpRspIEEE[8];
	UINT16	ShortAddr;
	UINT8	AssoMember;
	UINT8	SecurityEnable;
} MLME_ORPH_RSP;

typedef struct 
{
	UINT8	LogicalChannel;
	UINT8	TrackBeacon;
} MLME_SYNC_REQ;

typedef struct 
{
	UINT8	LossReason;
} MLME_SYNC_LOSS_IND;

typedef struct 
{
	UINT16	PANId;
	UINT8	SrcAddrMode;
	MAC_AD	SrcAddr_U;
	UINT8	DstAddrMode;
	MAC_AD	DstAddr_U;
	UINT8	status;
	UINT8	ID;			// out of spec. by RadioPulse
} MLME_COMM_STA_IND;

typedef struct {
	UINT8	BSN;
	MS_PanDes	*pPANDes;
	UINT8	PendAddrSpec;
	UINT8	*pAddrList;
	UINT8	sduLength;
	UINT8	*psdu;
} MLME_BEA_NOTI_IND;

typedef union 
{
	MCPS_PURGE_REQ		PURGE_REQ;
	MCPS_PURGE_CON	PURGE_CON;		
	MCPS_DATA_REQ		DATA_REQ;
	MCPS_DATA_CON		DATA_CON;
	MCPS_DATA_IND		DATA_IND;	
	MLME_START_REQ		START_REQ;
	MLME_START_CON		START_CON;	
	MLME_SCAN_REQ		SCAN_REQ;
	MLME_SCAN_CON		SCAN_CON;	
	MLME_ASSO_REQ		ASSO_REQ;
	MLME_ASSO_CON		ASSO_CON;
	MLME_ASSO_IND		ASSO_IND;	
	MLME_ASSO_RSP		ASSO_RSP;	
	MLME_RESET_REQ		RESET_REQ;
	MLME_RESET_CON		RESET_CON;	
	MLME_SET_REQ		SET_REQ;
	MLME_SET_CON		SET_CON;	
	MLME_GET_REQ		GET_REQ;
	MLME_GET_CON		GET_CON;	
	MLME_DISA_REQ		DISA_REQ;
	MLME_DISA_CON		DISA_CON;	
	MLME_DISA_IND		DISA_IND;	
	MLME_GTS_REQ		GTS_REQ;
	MLME_GTS_CON		GTS_CON;
	MLME_GTS_IND		GTS_IND;	
	MLME_POLL_REQ		POLL_REQ;
	MLME_POLL_CON		POLL_CON;	
	MLME_RX_ENA_REQ	RX_ENA_REQ;
	MLME_RX_ENA_CON	RX_ENA_CON;	
	MLME_ORPH_IND		ORPH_IND;	
	MLME_ORPH_RSP		ORPH_RSP;
	MLME_SYNC_REQ		SYNC_REQ;
	MLME_SYNC_LOSS_IND	SYNC_LOSS_IND;		
	MLME_COMM_STA_IND	COMM_STA_IND;	
	MLME_BEA_NOTI_IND	BEA_NOTI_IND;
} MLDME_PACKET;




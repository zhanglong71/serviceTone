/*******************************************************************************
	- Chip		: MG2450/2455
	- Vendor		: RadioPulse Inc, 2007.
	- Date		: 2010-11-15
	- Version		: VER 2.2

	[2010-11-15] VER 2.2
	- For ZigBeePRO Development Kit V2.2
	
	[2010-03-02] VER 2.10
	- For ZigBeePRO Development Kit V2.10
*******************************************************************************/

////////////////////////////////////////////////////////////////
/////////////////////// APS Primitive ID Code /////////////////////
////////////////////////////////////////////////////////////////

	#define	aID_REQ		0x00
	#define	aID_RSP			0x01
	#define	aID_CON		0x02
	#define	aID_IND			0x03

	#define	aID_DATA		0x00
	#define	aID_BIND		0x10
	#define	aID_UNBIND		0x20
	#define	aID_SET			0x30
	#define	aID_GET		0x40
	#define	aID_ADD_GRP	0x50
	#define	aID_RM_GRP		0x60
	#define	aID_RM_ALL_GRP	0x70

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

	//---------------------------------------------------------------
	// APS Frame Type
	//---------------------------------------------------------------
	#define	aFragPS_TX_IDLE		0
	#define	aFragPS_TX_WINDOW	1
	#define	aFragPS_WAIT_ACK		2
	#define	aFragPS_ROUTE_DISC	3

	#define	aFragPS_RX_IDLE		0
	#define	aFragPS_RX_WINDOW	1
	
	//---------------------------------------------------------------
	// APS Frame Type
	//---------------------------------------------------------------
	#define	aFT_DATA		0
	#define	aFT_CMD		1
	#define	aFT_ACK		2

	//---------------------------------------------------------------
	// APS Frame Control Field
	//---------------------------------------------------------------
	#define	aFC_FrameType		0x03
	#define	aFC_DeliverMode	0x0C
	#define	aFC_AckFormat		0x10
	#define	aFC_Security		0x20
	#define	aFC_AckReq			0x40
	#define	aFC_ExtHead		0x80

	//---------------------------------------------------------------
	// APS Constants
	//---------------------------------------------------------------
	#define	apscMaxDescSize		64
	#define	apscMaxFrameRetry		3
	#define	apscAckWaitDuration		1600	// 0.05 * (2*nwkcMaxDepth) + (security encrypt delay) ms.
	#define	apscMinRejectTableSize	1
	#define	apscMaxWindowSize		8		// Fragmentation parameter. set by stack profile
	#define	apscInterframeDelay		10		// Fragmentation parameter. set by stack profile. milli-second
	#define	apscMinHeadOverhead	12		// minimum APS header size.	
	#define	apscMaxASDU			92		// 78 --> 92

	//---------------------------------------------------------------
	// APS Status Code
	//---------------------------------------------------------------
	#define	aSTA_SUCCESS				0x00
	#define	aSTA_ASDU_TOO_LONG		0xA0
	#define	aSTA_DEFRAG_DEFERED		0xA1
	#define	aSTA_DEFRAG_UNSUPPORT	0xA2
	#define	aSTA_ILLEGAL_REQUEST		0xA3
	#define	aSTA_INVALID_BINDING		0xA4
	#define	aSTA_INVALID_GROUP		0xA5
	#define	aSTA_INVALID_PARAMETER	0xA6
	#define	aSTA_NO_ACK				0xA7
	#define	aSTA_NO_BOUND_DEVICE		0xA8
	#define	aSTA_NO_SHORT_ADDR		0xA9
	#define	aSTA_NOT_SUPPORTED		0xAA
	#define	aSTA_SECURED_LINK_KEY	0xAB
	#define	aSTA_SECURED_NWK_KEY	0xAC
	#define	aSTA_SECURITY_FAIL		0xAD
	#define	aSTA_TABLE_FULL			0xAE
	#define	aSTA_UNSECURED			0xAF
	#define	aSTA_UNSUPPORT_ATTRI		0xB0
	
	#define	aSTA_RP_AQ_BUSY			0xB1	// by RadioPulse
	#define	aSTA_RP_FRAGMENT_START	0xB2	// by RadioPulse
	#define	aSTA_RP_FRAGMENT_BUSY	0xB3	// by RadioPulse
	#define	aSTA_RP_FRAGMENT_FINISH	0xB4	// by RadioPulse	
	#define	aSTA_RP_ROUTE_DISC_FAIL	0xB5	// by RadioPulse
	#define	aSTA_RP_NO_IEEE_ADDR		0xB6	// by RadioPulse

	//---------------------------------------------------------------
	// APS Security Key Type
	//---------------------------------------------------------------
	#define	aKT_TcMaster	0x00
	#define	aKT_StandNwk	0x01
	#define	aKT_AppMaster	0x02
	#define	aKT_AppLink	0x03
	#define	aKT_TcLink		0x04
	#define	aKT_HighNwk	0x05
	//---------------------------------------------------------------
	// APS Command Code
	//---------------------------------------------------------------
	#define	aCMD_SKKE_1			0x01
	#define	aCMD_SKKE_2			0x02
	#define	aCMD_SKKE_3			0x03
	#define	aCMD_SKKE_4			0x04
	#define	aCMD_TRANSPORT_KEY	0x05
	#define	aCMD_UPDATE_DEVICE	0x06
	#define	aCMD_REMOVE_DEVICE	0x07
	#define	aCMD_REQUEST_KEY		0x08
	#define	aCMD_SWITCH_KEY		0x09
	#define	aCMD_EA_INIT_CHLNG	0x0A
	#define	aCMD_EA_RSP_CHLNG	0x0B
	#define	aCMD_EA_INIT_MAC_DATA	0x0C
	#define	aCMD_EA_RSP_MAC_DATA	0x0D
	#define	aCMD_TUNNEL			0x0E

	//---------------------------------------------------------------
	// ZDP Cluster ID
	//---------------------------------------------------------------
	#define	aCID_Nwk_Addr_Req			0x0000
	#define	aCID_IEEE_Addr_Req		0x0001
	#define	aCID_Node_Desc_Req		0x0002
	#define	aCID_Power_Desc_Req		0x0003
	#define	aCID_Simp_Desc_Req		0x0004
	#define	aCID_Active_EP_Req			0x0005
	#define	aCID_Match_Desc_Req		0x0006
	#define	aCID_Comp_Desc_Req		0x0010
	#define	aCID_User_Desc_Req		0x0011
	#define	aCID_Disc_Cache_Req		0x0012	
	#define	aCID_Device_Announce		0x0013
	#define	aCID_User_Desc_Set			0x0014
	#define	aCID_Sys_Serv_Disc_Req		0x0015
	#define	aCID_Disc_Store_Req		0x0016
	#define	aCID_Node_Desc_Store_Req	0x0017
	#define	aCID_Power_Desc_Store_Req	0x0018
	#define	aCID_Active_EP_Store_Req	0x0019
	#define	aCID_Simp_Desc_Store_Req	0x001A
	#define	aCID_Rm_Node_Cache_Req	0x001B
	#define	aCID_Find_Node_Cache_Req	0x001C
	
	#define	aCID_End_Device_Bind_Req	0x0020
	#define	aCID_Bind_Req				0x0021
	#define	aCID_Unbind_Req			0x0022
	#define	aCID_Bind_Register_Req		0x0023
	#define	aCID_Replace_Device_Req	0x0024
	#define	aCID_Store_Back_Bind_Entry_Req	0x0025
	#define	aCID_Rm_Back_Bind_Entry_Req		0x0026
	#define	aCID_Back_Bind_Table_Req	0x0027
	#define	aCID_Reco_Bind_Table_Req	0x0028
	#define	aCID_Back_Src_Bind_Req		0x0029
	#define	aCID_Reco_Src_Bind_Req		0x002A

	#define	aCID_Mgmt_Nwk_Disc_Req	0x0030
	#define	aCID_Mgmt_LQI_Req			0x0031
	#define	aCID_Mgmt_Rtg_Req			0x0032
	#define	aCID_Mgmt_Bind_Req		0x0033
	#define	aCID_Mgmt_Leave_Req		0x0034
	#define	aCID_Mgmt_Direct_Join_Req	0x0035
	#define	aCID_Mgmt_Permit_Join_Req	0x0036
	#define	aCID_Mgmt_Cache_Req		0x0037
	#define	aCID_Mgmt_Nwk_Update_Req	0x0038

	#define	aCID_Nwk_Addr_Rsp			0x8000
	#define	aCID_IEEE_Addr_Rsp			0x8001
	#define	aCID_Node_Desc_Rsp		0x8002
	#define	aCID_Power_Desc_Rsp		0x8003
	#define	aCID_Simp_Desc_Rsp		0x8004
	#define	aCID_Active_EP_Rsp			0x8005
	#define	aCID_Match_Desc_Rsp		0x8006
	#define	aCID_Comp_Desc_Rsp		0x8010
	#define	aCID_User_Desc_Rsp			0x8011
	#define	aCID_Disc_Cache_Rsp		0x8012
	#define	aCID_User_Desc_Conf		0x8014
	#define	aCID_Sys_Serv_Disc_Rsp		0x8015
	#define	aCID_Disc_Store_Rsp		0x8016
	#define	aCID_Node_Desc_Store_Rsp	0x8017
	#define	aCID_Power_Desc_Store_Rsp	0x8018
	#define	aCID_Active_EP_Store_Rsp	0x8019
	#define	aCID_Simp_Desc_Store_Rsp	0x801A
	#define	aCID_Rm_Node_Cache_Rsp	0x801B
	#define	aCID_Find_Node_Cache_Rsp	0x801C

	#define	aCID_End_Device_Bind_Rsp	0x8020
	#define	aCID_Bind_Rsp				0x8021
	#define	aCID_Unbind_Rsp			0x8022
	#define	aCID_Bind_Register_Rsp		0x8023
	#define	aCID_Replace_Device_Rsp	0x8024
	#define	aCID_Store_Back_Bind_Entry_Rsp		0x8025
	#define	aCID_Rm_Back_Bind_Entry_Rsp		0x8026
	#define	aCID_Back_Bind_Table_Rsp	0x8027
	#define	aCID_Reco_Bind_Table_Rsp	0x8028
	#define	aCID_Back_Src_Bind_Rsp		0x8029
	#define	aCID_Reco_Src_Bind_Rsp		0x802A

	#define	aCID_Mgmt_Nwk_Disc_Rsp	0x8030
	#define	aCID_Mgmt_LQI_Rsp			0x8031
	#define	aCID_Mgmt_Rtg_Rsp			0x8032
	#define	aCID_Mgmt_Bind_Rsp		0x8033
	#define	aCID_Mgmt_Leave_Rsp		0x8034
	#define	aCID_Mgmt_Direct_Join_Rsp	0x8035
	#define	aCID_Mgmt_Permit_Join_Rsp	0x8036
	#define	aCID_Mgmt_Cache_Rsp		0x8037
	#define	aCID_Mgmt_Nwk_Update_Noti	0x8038

	
typedef union
{
	UINT8	Ext[8];
	UINT16	Short;
} APS_AD;

typedef struct
{
	UINT8	SrcIEEE[8];
	UINT8	SrcEP;
	UINT16	ClusterID;
	//-- DstMode
	//	1	: Group Address
	//	3	: 64bit Ext Address
	UINT8	DstMode;
	APS_AD	DstAdd_U;
	UINT8	DstEP;			// Present only if DstMode=3
	UINT8	EnaBT;
} AS_BIND_TABLE;

typedef struct
{
	UINT16	GroupAddr;
	UINT8	GroupEP;
	UINT8	EnaGT;
} AS_GROUP_TABLE;

typedef struct
{
	AS_BIND_TABLE	*papsBindTable;		// 0xC1. NULL
	UINT8			apsDesignCoord;		// 0xC2. 0(0~1)
	UINT32			apsChannelMask;	// 0xC3. All Channel	
	UINT8			apsUseExtPanID[8];	// 0xC4. 0x00..0
	AS_GROUP_TABLE	*papsGroupTable;	// 0xC5. NULL
	UINT8			apsNonmemRadius;	// 0xC6. 2(1~7)
	UINT8			apsPermitConfig;	// 0xC7. NULL(???)
	UINT8			apsUseInsecureJoin;	// 0xC8. 1(0~1)
	UINT8			apsInterframeDelay;	// 0xC9. set by stack profile. milli-second
	UINT8			apsLastChanEnergy;	// 0xCA. NULL
	UINT8			apsLastChanFailRate;// 0xCB. NULL(0~100)
	UINT8			apsChanTimer;		// 0xCC. NULL(1~24)

	UINT8			DSN;
	UINT8			SIZE_BT;
	UINT8			SIZE_GT;
	UINT16			BroadcastAddr;

	//	bit[0]	: 1=APS Filtering Enable, 0=Disable
	UINT16			Option;		// specific option	

	UINT16			SIZE_FRAG_TX_BUFF;
	UINT16			SIZE_FRAG_RX_BUFF;
	UINT8			apsMaxWindowSize;
	UINT8			*pFragmentTxBuff;
	UINT8			*pFragmentRxBuff;
	UINT8			MaxAsduSizeForTest;	
} APS_PIB;

typedef struct
{
	UINT8	CmdID;
	UINT8	InitIEEE[8];
	UINT8	RspIEEE[8];
	UINT8	Data[16];
} AS_SKKE_CMD;

typedef struct
{
	UINT8	Key[16];
	UINT8	DstIEEE[8];
	UINT8	SrcIEEE[8];
} AS_TRANSPORT_KEY_CMD_DESCRIPTOR_TC;

typedef struct
{
	UINT8	Key[16];
	UINT8	Seq;
	UINT8	DstIEEE[8];
	UINT8	SrcIEEE[8];
} AS_TRANSPORT_KEY_CMD_DESCRIPTOR_NWK;

typedef struct
{
	UINT8	Key[16];
	UINT8	PartnerIEEE[8];
	UINT8	InitiatorFlag;
} AS_TRANSPORT_KEY_CMD_DESCRIPTOR_APP;

typedef union
{
	AS_TRANSPORT_KEY_CMD_DESCRIPTOR_TC	TC;
	AS_TRANSPORT_KEY_CMD_DESCRIPTOR_NWK	NWK;
	AS_TRANSPORT_KEY_CMD_DESCRIPTOR_APP	APP;
} AS_TRANSPORT_KEY_CMD_DESCRIPTOR;

typedef struct
{
	UINT8	CmdID;
	UINT8	KeyType;
	AS_TRANSPORT_KEY_CMD_DESCRIPTOR	Desc;
} AS_TRANSPORT_KEY_CMD;

typedef struct
{
	UINT8	CmdID;
	UINT8	DevIEEE[8];
	UINT8	ShortAddr[2];
	UINT8	Status;
} AS_UPDATE_DEVICE_CMD;

typedef struct
{
	UINT8	CmdID;
	UINT8	ChildIEEE[8];
} AS_REMOVE_DEVICE_CMD;

typedef struct
{
	UINT8	CmdID;
	UINT8	KeyType;
	UINT8	PartnerIEEE[8];		// 0 or 8
} AS_REQUEST_KEY_CMD;

typedef struct
{
	UINT8	CmdID;
	UINT8	KeySeq;
} AS_SWITCH_KEY_CMD;

typedef struct
{
	UINT8	CmdID;
	UINT8	*pKeyInfo;	// variable
	UINT8	InitIEEE[8];
	UINT8	RspIEEE[8];
	UINT8	Challenge[16];
} AS_AUTHEN_CHALLENGE;

typedef struct
{
	UINT8	CmdID;
	UINT8	MAC[16];
	UINT8	DataType;
	UINT8	Data[4];
} AS_AUTHEN_MAC_AND_DATA;

typedef struct
{
	UINT8	CmdID;
	UINT8	DstIEEE[8];
	UINT8	TunApsHead[2];
	UINT8	TunAuxFrame[13];
	UINT8	*pTunCommand;		// variable
	UINT8	TunApsMIC[4];
} AS_TUNNEL_CMD;

typedef union
{
	AS_SKKE_CMD				Skke;
	AS_TRANSPORT_KEY_CMD		TrKey;
	AS_UPDATE_DEVICE_CMD		UpDev;
	AS_REMOVE_DEVICE_CMD		RmDev;
	AS_REQUEST_KEY_CMD		ReqKey;
	AS_SWITCH_KEY_CMD			SwKey;
	AS_AUTHEN_CHALLENGE		AuChall;
	AS_AUTHEN_MAC_AND_DATA	AuMac;
	AS_TUNNEL_CMD				Tunnel;
} AS_PAY;

typedef struct
{	
	//-- DstMode
	//	0	: DstAddr = not present. 		DstEP not present
	// 	1	: DstAddr = 16 bit Group Addr. 	DstEP not present
	//	2	: DstAddr = 16 bit Addr. 		DstEP present
	//	3	: DstAddr = 64 bit Addr. 		DstEP present
	UINT8	DstMode;
	APS_AD	DstAdd_U;
	UINT8	DstEP;
	UINT16	ProfileID;		// Profile ID
	UINT16	ClusID;			// Cluster ID
	UINT8	SrcEP;		
	UINT16	asduLen;
	//--	TxOption
	// bit[7]		: Direct
	// bit[6:4]	: rsv
	// bit[3]		: Fragmentation permitted
	// bit[2]		: AckReq 
	// bit[1]		: Use NWK Key
	// bit[0]		: Security
	UINT8	TxOption;
	UINT8	Radius;
	UINT8	*pasdu;
} APSDE_DATA_req;

typedef struct
{
	//-- DstMode
	//	0	: DstAddr = not present. 		DstEP not present
	// 	1	: DstAddr = 16 bit Group Addr. 	DstEP not present
	//	2	: DstAddr = 16 bit Addr. 		DstEP present
	//	3	: DstAddr = 64 bit Addr. 		DstEP present
	UINT8	DstMode;
	APS_AD	DstAdd_U;
	UINT8	DstEP;
	UINT8	SrcEP;
	UINT8	Sta;
	UINT16	TxTime;			// Z2007
} APSDE_DATA_con;

typedef struct
{
	//-- DstMode
	//	0	: rsv
	// 	1	: DstAddr = 16 bit Group Addr. 	DstEP not present
	//	2	: DstAddr = 16 bit Addr. 		DstEP present
	//	3	: rsv
	UINT8	DstMode;
	APS_AD	DstAdd_U;
	UINT8	DstEP;
	//-- SrcMode
	//	0	: rsv
	// 	1	: rsv
	//	2	: SrcAddr = 16 bit SrcAddr.		SrcEP present
	//	3	: SrcAddr = 64 bit SrcAddr.		SrcEP present
	UINT8	SrcMode;
	APS_AD	SrcAdd_U;
	UINT8	SrcEP;	
	UINT16	ProfileID;	// Profile ID
	UINT16	ClusID;		// Cluster ID
	UINT16	asduLen;
	UINT8	Sta;
	UINT8	SecSta;
	UINT8	LQI;
	UINT16	RxTime;
	UINT8	*pasdu;
} APSDE_DATA_ind;

typedef struct
{
	UINT8	SrcIEEE[8];
	UINT8	SrcEP;
	UINT16	ClusID;
	UINT8	DstMode;		// 1=16bit Group Addr, 3=Dst IEEE
	APS_AD	DstAdd_U;
	UINT8	DstEP;			// Present only if DstMode=3
} APSME_BIND_req;

typedef struct
{
	UINT8	Sta;
	UINT8	SrcIEEE[8];
	UINT8	SrcEP;
	UINT16	ClusID;
	UINT8	DstMode;
	APS_AD	DstAdd_U;		// 1=16bit Group Addr, 3=Dst IEEE
	UINT8	DstEP;			// Present only if DstMode=3
} APSME_BIND_con;

typedef struct
{
	UINT8	SrcIEEE[8];
	UINT8	SrcEP;
	UINT16	ClusID;
	UINT8	DstMode;		// 1=16bit Group Addr, 3=Dst IEEE
	APS_AD	DstAdd_U;
	UINT8	DstEP;			// Present only if DstMode=3
} APSME_UNBIND_req;

typedef struct
{	
	UINT8	Sta;
	UINT8	SrcIEEE[8];
	UINT8	SrcEP;
	UINT16	ClusID;
	UINT8	DstMode;		// 1=16bit Group Addr, 3=Dst IEEE
	APS_AD	DstAdd_U;
	UINT8	DstEP;			// Present only if DstMode=3
} APSME_UNBIND_con;

typedef struct
{
	UINT8	Attribute;
} APSME_GET_req;

typedef struct
{
	UINT8	Sta;
	UINT8	Attribute;
	UINT16	AttributeLength;
	UINT8	AttributeValue[16];
} APSME_GET_con;


typedef struct
{
	UINT8	Attribute;
	UINT16	AttributeLength;
	UINT8	AttributeValue[16];
} APSME_SET_req;

typedef struct
{
	UINT8	Sta;
	UINT8	Attribute;
} APSME_SET_con;

typedef struct
{
	UINT8	ExtendAtt;
	UINT8	ExtendAttLen;
	UINT8	ExtendEntry;		// Entry Number
	UINT8	*pEntry;
} APSME_SET_TABLE_req;

typedef struct
{
	UINT8	ExtendSta;	
	UINT8	ExtendAtt;
} APSME_SET_TABLE_con;

typedef struct
{
	UINT8	ExtendAtt;
} APSME_GET_TABLE_req;

typedef struct
{
	UINT8	ExtendSta;
	UINT8	ExtendAtt;
	UINT8	ExtendEntryLen;
	UINT8	*pEntry;
} APSME_GET_TABLE_con;

typedef struct
{
	UINT16	GroupAddr;
	UINT8	EndPoint;
} APSME_ADD_GROUP_req;

typedef struct
{
	UINT8	Sta;
	UINT16	GroupAddr;
	UINT8	EndPoint;
} APSME_ADD_GROUP_con;

typedef struct
{
	UINT16	GroupAddr;
	UINT8	EndPoint;
} APSME_REMOVE_GROUP_req;

typedef struct
{
	UINT8	Sta;
	UINT16	GroupAddr;
	UINT8	EndPoint;
} APSME_REMOVE_GROUP_con;

typedef struct
{
	UINT8	EndPoint;
} APSME_REMOVE_ALL_GROUP_req;

typedef struct
{
	UINT8	Sta;
	UINT8	EndPoint;
} APSME_REMOVE_ALL_GROUP_con;

typedef struct
{
	UINT8	ParentIEEE[8];
	UINT8	Key[16];
} AS_KEYDATA_REQ_TC;

typedef struct
{
	UINT8	KeySeqNum;
	UINT8	NwkKey[16];
	UINT8	UseParent;
	UINT8	ParentIEEE[8];	// If UseParent=1, present
} AS_KEYDATA_REQ_NWK;

typedef struct
{
	UINT8	PartnerIEEE[8];
	UINT8	Initiator;
	UINT8	Key[16];	
} AS_KEYDATA_REQ_APP;			// Application Master Key or Link Key

typedef union
{
	AS_KEYDATA_REQ_TC		TC;
	AS_KEYDATA_REQ_NWK	NWK;
	AS_KEYDATA_REQ_APP		APP;
} AS_KEYDATA_REQ;

typedef struct
{
	UINT8	DstIEEE[8];
	//-- KeyType
	//	0	: Trust-Center Master key
	//	1	: Standard Network Key
	//	2	: Application Master Key
	//	3	: Application Link Key
	//	4	: Trust-Center Link Key
	//	5	: High-Security Network Key		
	UINT8	KeyType;
	AS_KEYDATA_REQ	KeyData;
} APSME_TRANSPORT_KEY_req;

typedef struct
{
	UINT8	TC_MasterKey[16];
} AS_KEYDATA_IND_TC;

typedef struct
{
	UINT8	KeySeqNum;
	UINT8	NwkKey[16];
} AS_KEYDATA_IND_NWK;

typedef struct
{
	UINT8	PartnerIEEE[8];
	UINT8	Key[16];
	UINT8	InitiatorFlag;
} AS_KEYDATA_IND_APP;			// Application Master Key or Link Key

typedef union
{
	AS_KEYDATA_IND_TC		TC;
	AS_KEYDATA_IND_NWK	NWK;
	AS_KEYDATA_IND_APP		APP;
} AS_KEYDATA_IND;

typedef struct
{
	UINT8	SrcIEEE[8];
	//-- KeyType
	//	0	: Trust-Center Master key
	//	1	: Standard Network Key
	//	2	: Application Master Key
	//	3	: Application Link Key
	//	4	: Trust-Center Link Key
	//	5	: High-Security Network Key	
	UINT8	KeyType;
	AS_KEYDATA_IND	KeyData;
} APSME_TRANSPORT_KEY_ind;

typedef struct
{
	UINT8	RspIEEE[8];
	UINT8	UseParent;
	UINT8	RspParentIEEE[8];	// if UseParent=1
	UINT8	EstablishMethod;		// 0=SKKE, 1~3=reserved

	UINT8	SkkeIdx;
	UINT8	*pSkkeData;			// 16 Bytes
} APSME_ESTABLISH_KEY_req;

typedef struct
{
	UINT8	InitIEEE[8];
	UINT8	EstablishMethod;	

	UINT8	SkkeIdx;
	UINT8	*pSkkeData;			// 16 Bytes
} APSME_ESTABLISH_KEY_ind;

typedef struct
{
	UINT8	DstIEEE[8];
	UINT8	DevIEEE[8];			// device being updated
	//- Sta
	//	0	: Standard Device Secured Join
	//	1	: Standard Device Unsecured Join
	//	2	: Device Left
	//	3	: Standard Device Unsecured Rejoin
	//	4	: High-Security Device Secured Join
	//	5	: High-Security Device Unsecured Join
	//	6	: reserved
	//	7	: High-Security Device Unsecured Rejoin
	UINT8	Status;
	UINT16	DevShortAddr;
} APSME_UPDATE_DEV_req;

typedef struct
{
	UINT8	SrcIEEE[8];
	UINT8	DevIEEE[8];			// device being updated
	//- Sta
	//	0	: Standard Device Secured Join
	//	1	: Standard Device Unsecured Join
	//	2	: Device Left
	//	3	: Standard Device Unsecured Rejoin
	//	4	: High-Security Device Secured Join
	//	5	: High-Security Device Unsecured Join
	//	6	: reserved
	//	7	: High-Security Device Unsecured Rejoin
	UINT8	Status;
	UINT16	DevShortAddr;
} APSME_UPDATE_DEV_ind;

typedef struct
{
	UINT8	ParentIEEE[8];
	UINT8	ChildIEEE[8];
} APSME_REMOVE_DEVICE_req;

typedef struct
{
	UINT8	SrcIEEE[8];
	UINT8	ChildIEEE[8];
} APSME_REMOVE_DEVICE_ind;


typedef struct
{
	UINT8	DstIEEE[8];
	//-- KeyType
	//	1	: Network Key
	//	2	: Application Key
	UINT8	KeyType;
	UINT8	PartnerIEEE[8];		// present if KeyType=2
} APSME_REQUEST_KEY_req;

typedef struct
{
	UINT8	SrcIEEE[8];
	//-- KeyType
	//	1	: Network Key
	//	2	: Application Key
	UINT8	KeyType;
	UINT8	PartnerIEEE[8];
} APSME_REQUEST_KEY_ind;


typedef struct
{
	UINT8	DstIEEE[8];
	UINT8	KeySeqNum;	
} APSME_SWITCH_KEY_req;

typedef struct
{
	UINT8	SrcIEEE[8];
	UINT8	KeySeqNum;
} APSME_SWITCH_KEY_ind;

typedef struct
{
	UINT8	PartnerIEEE[8];
	//- Action
	//	0	: INITIATE
	//	1	: RESPOND_ACCEPT
	//	2	: RESPOND_REJECT
	UINT8	Action;
	UINT8	*pRandomChallenge;		// 16 Bytes
} APSME_AUTHENTICATE_req;

typedef struct
{
	UINT8	DevIEEE[8];
	UINT8	Sta;
} APSME_AUTHENTICATE_con;

typedef struct
{
	UINT8	InitIEEE[8];
	UINT8	*pRandomChallenge;		// 16 Bytes
} APSME_AUTHENTICATE_ind;

typedef struct
{
	UINT8	DstEP;		// If 0xFF, DstEP is not present in the ACK frame
	UINT16	ClusID;
	UINT16	ProfID;
	UINT8	SrcEP;		// If 0xFF, SrcEP is not present in the ACK frame
	UINT8	DSN;
} APSDME_ACK_ind;

typedef struct
{
	UINT8	FrameControl;
	UINT8	DstEP;
	UINT16	ClusID;
	UINT16	ProfileID;
	UINT8	SrcEP;
	UINT8	DSN;
	UINT16	DstAddr;
	UINT8	SendStatus;
} APSDME_ACK_Status;

typedef union
{
	APSDE_DATA_req			DATA_REQ;
	APSME_BIND_req			BIND_REQ;
	APSME_UNBIND_req		UNBIND_REQ;
	APSME_GET_req			GET_REQ;
	APSME_SET_req			SET_REQ;
	APSME_ADD_GROUP_req	ADD_GROUP_REQ;
	APSME_REMOVE_GROUP_req		REMOVE_GROUP_REQ;
	APSME_REMOVE_ALL_GROUP_req		REMOVE_ALL_GROUP_REQ;
	APSME_ESTABLISH_KEY_req		ESTABL_KEY_REQ;	
	APSME_TRANSPORT_KEY_req	TRANS_KEY_REQ;
	APSME_UPDATE_DEV_req		UPDATE_DEV_REQ;
	APSME_REQUEST_KEY_req		REQUEST_KEY_REQ;	
	APSME_SWITCH_KEY_req		SWITCH_KEY_REQ;
	APSME_REMOVE_DEVICE_req	REMOVE_DEV_REQ;
	APSME_AUTHENTICATE_req		AUTHEN_REQ;	
	APSME_SET_TABLE_req			SET_TABLE_REQ;		// by RadioPulse
	APSME_GET_TABLE_req			GET_TABLE_REQ;		// by RadioPulse
} APSDME_REQ_PKT;

typedef union
{
	APSDE_DATA_con			DATA_CON;
	APSME_BIND_con			BIND_CON;
	APSME_UNBIND_con		UNBIND_CON;
	APSME_GET_con			GET_CON;
	APSME_SET_con			SET_CON;
	APSME_ADD_GROUP_con	ADD_GROUP_CON;
	APSME_REMOVE_GROUP_con		REMOVE_GROUP_CON;
	APSME_REMOVE_ALL_GROUP_con	REMOVE_ALL_GROUP_CON;
	APSME_AUTHENTICATE_con		AUTHEN_CON;
	APSME_SET_TABLE_con			SET_TABLE_CON;		// by RadioPulse
	APSME_GET_TABLE_con			GET_TABLE_CON;	// by RadioPulse
} APSDME_CON_PKT;

typedef union
{
	APSDE_DATA_ind				DATA_IND;
	APSME_ESTABLISH_KEY_ind		ESTABL_KEY_IND;
	APSME_TRANSPORT_KEY_ind	TRANS_KEY_IND;	
	APSME_UPDATE_DEV_ind		UPDATE_DEV_IND;
	APSME_REQUEST_KEY_ind		REQUEST_KEY_IND;
	APSME_SWITCH_KEY_ind		SWITCH_KEY_IND;
	APSME_REMOVE_DEVICE_ind	REMOVE_DEV_IND;
	APSME_AUTHENTICATE_ind		AUTHEN_IND;
	APSDME_ACK_ind				ACK_IND;		// by RadioPulse
	APSDME_ACK_Status			ACK_STA;		// by RadioPulse
} APSDME_IND_PKT;

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

typedef struct
{
	UINT8	FT	: 2 	;	// Frame Type. 0=Data, 1=Command, 2=Ack
	UINT8	DM	: 2 	;	// Delivery Mode. 0=Normal, 1=Indirect, 2=Broadcast, 3=Group
	UINT8	AF	: 1 	;	// Ack Format
	UINT8	Sec	: 1 	;	// Security
	UINT8	Ack	: 1 	;	// Ack Request
	UINT8	Ext	: 1 	;	// Extended Header
} AS_FC_Sub;

typedef union
{
	UINT8		All;
	AS_FC_Sub	b;	
} AS_FC;

	//-----------------------------------------------------
	//	ExtFC.Frag	Description		BlockNum
	//-----------------------------------------------------
	//		0		Not fragmented	Not present
	//		1		1st fragment		Number of All Blocks
	//		2		2nd~last fragment	Number of current block
	//		3		rsv				---
	//-----------------------------------------------------
typedef struct
{
	// bit[7:2] 	: rsv
	// bit[1:0]	: Fragmentation
	UINT8	ExtFC;
	UINT8	BlockNum;
	UINT8	AckBitfield;
} AS_ExtHEAD;

typedef struct
{
	//--Frame Control --
	//	bit[7]	: 1=Extended Header Present
	// 	bit[6]	: AckReq
	//	bit[5]	: Security
	// 	bit[4]	: AckFormat. 0=APS Data Ack. 1=APS Cmd Ack(DstEP, ClusID, ProfID, SrcEP are present)
	//	bit[3:2]	: Delivery Mode. 0=Normal, 1=Indirect, 2=Broadcast, 3=Group
	//	bit[1:0]	: Frame Type. 0=Data, 1=Command, 2=Ack
	AS_FC	FC;
	UINT8	DstEP;		// optional
	UINT16	GroupAddr;	// optional
	UINT16	ClusID;		// optional
	UINT16	ProfileID;	// optional
	UINT8	SrcEP;		// optional
	UINT8	DSN;

	AS_ExtHEAD	ExtHead;

	// bit[7]	: ExtHead.AckBitField
	// bit[6]	: ExtHead.BlockNum
	// bit[5]	: ExtHead.ExtFC
	// bit[4]	: SrcEP is present
	// bit[3]	: Profile ID is present		
	// bit[2] 	: ClusID is present
	// bit[1] 	: GroupAddress is present
	// bit[0] 	: DstEP is present
	UINT8	FieldPresent;
} AS_HEAD;

typedef struct
{
	//--Frame Control --
	//	bit[7]	: 1=Extended Header Present
	// 	bit[6]	: AckReq
	//	bit[5]	: Security
	// 	bit[4]	: AckFormat. 0=APS Data Ack. 1=APS Cmd Ack(DstEP, ClusID, ProfID, SrcEP are present)
	//	bit[3:2]	: Delivery Mode. 0=Normal, 1=Indirect, 2=Broadcast, 3=Group
	//	bit[1:0]	: Frame Type. 0=Data, 1=Command, 2=Ack
	AS_FC	FC;
	UINT8	DstEP;			// optional
	UINT16	GroupAddr;		// optional
	UINT16	ClusID;
	UINT16	ProfileID;
	UINT8	SrcEP;
	UINT8	DSN;

	AS_ExtHEAD	ExtHead;	// optional

	// bit[4]	: SrcEP is present
	// bit[3]	: Profile ID is present		
	// bit[2] 	: ClusID is present
	// bit[1] 	: GroupAddress is present
	// bit[0] 	: DstEP is present
	UINT8	FieldPresent;
} AS_HEAD_DATA;


typedef struct
{
	//--Frame Control --
	//	bit[7]	: 1=Extended Header Present
	// 	bit[6]	: AckReq
	//	bit[5]	: Security
	// 	bit[4]	: AckFormat. 0=APS Data Ack. 1=APS Cmd Ack(DstEP, ClusID, ProfID, SrcEP are present)
	//	bit[3:2]	: Delivery Mode. 0=Normal, 1=Indirect, 2=Broadcast, 3=Group
	//	bit[1:0]	: Frame Type. 0=Data, 1=Command, 2=Ack
	AS_FC	FC;
	UINT8	DSN;

	// bit[4]	: SrcEP is present
	// bit[3]	: Profile ID is present		
	// bit[2] 	: ClusID is present
	// bit[1] 	: GroupAddress is present
	// bit[0] 	: DstEP is present
	UINT8	FieldPresent;	
} AS_HEAD_CMD;


typedef struct
{
	//--Frame Control --
	//	bit[7]	: 1=Extended Header Present
	// 	bit[6]	: AckReq
	//	bit[5]	: Security
	// 	bit[4]	: AckFormat. 0=APS Data Ack. 1=APS Cmd Ack(DstEP, ClusID, ProfID, SrcEP are present)
	//	bit[3:2]	: Delivery Mode. 0=Normal, 1=Indirect, 2=Broadcast, 3=Group
	//	bit[1:0]	: Frame Type. 0=Data, 1=Command, 2=Ack
	AS_FC	FC;
	UINT8	DstEP;			// optional
	UINT16	ClusID;			// optional
	UINT16	ProfileID;		// optional
	UINT8	SrcEP;			// optional
	UINT8	DSN;

	AS_ExtHEAD	ExtHead;	// optional

	// bit[4]	: SrcEP is present
	// bit[3]	: Profile ID is present		
	// bit[2] 	: ClusID is present
	// bit[1] 	: GroupAddress is present
	// bit[0] 	: DstEP is present
	UINT8	FieldPresent;	
} AS_HEAD_ACK;


typedef union
{
	AS_HEAD_DATA	DH;		// Data Header
	AS_HEAD_CMD	CH;		// Command Header
	AS_HEAD_ACK	AH;		// Ack Header
} AS_HEAD_TOTAL;

typedef struct
{
	UINT8		PS;
	UINT8		Radius;
	UINT16		NwkDstAddr;
	UINT16		AsduLen;
	AS_HEAD		ApsHead;
	UINT8		KeyID;
	UINT8		SecurityIEEE[8];

	UINT8		WindowBitField;
	UINT8		RetryCount;	
	UINT8		LastBlockNum;
	UINT8		LastVolumeNum;
	UINT8		CurrVolumeNum;

	UINT8		AckOK;
	UINT8		AckBitField;
	UINT8		AckBlockNum;
	UINT8		PrevAckBitField;
} AS_FRAG_TX_PARA;

typedef struct
{
	UINT8		PS;
	UINT16		NwkSrcAddr;
	UINT8		ApsDSN;
	UINT8		WindowAsduSize;
	
	UINT8		WindowBitField;
	UINT8		LastBlockNum;	
	UINT8		LastVolumeNum;
	UINT8		ExpectVolumeNum;
	UINT16		TotalAsduLen;
	UINT16		Timeout;		// ms	
} AS_FRAG_RX_PARA;



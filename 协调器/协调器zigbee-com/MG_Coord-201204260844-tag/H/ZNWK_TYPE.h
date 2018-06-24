
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
/////////////////////// NWK TIMER  ////// ////////////////////
////////////////////////////////////////////////////////////////

#define	TIMER3_UNIT		4		// 2~6. T3_UNIT=4 --> 16ms
#define	NWKQUE_DEPTH		2
#define	DISC_NT_DEPTH		8

#define	VirtualTimer_NwkQue0		0
#define	VirtualTimer_NwkQue1		1
#define	VirtualTimer_ApsQue0		8
#define	VirtualTimer_UpdateChan		11
#define	VirtualTimer_UpdatePan		12
#define	VirtualTimer_PermitJoin		13
#define	VirtualTimer_Rejoin2			14
#define	VirtualTimer_LinkStatus		15
#define	VirtualTimer_Conflict			16
#define	VirtualTimer_FragTx			17
#define	VirtualTimer_FragRx			18
#define	VirtualTimer_PendBind		19
#define	VirtualTimer_ApsFilter		20

////////////////////////////////////////////////////////////////
/////////////////////// NIB Attribute ID ////// ////////////////////
////////////////////////////////////////////////////////////////

#define	nATT_NeighTable	0x87
#define	nATT_RouteTable	0x8B
#define	nATT_BroadTable	0x98
#define	nATT_GroupID		0x99
#define	nATT_RouteRecTable	0x9C
#define	nATT_NwkAddrMap	0xA9

////////////////////////////////////////////////////////////////
/////////////////////// NWK Define Parameters ////////////////////
////////////////////////////////////////////////////////////////

#define	nFT_DATA		0x00
#define	nFT_CMD		0x01

#define	nBROAD_ALL			0xFFFF
#define	nBROAD_RSV_FE		0xFFFE
#define	nBROAD_RXON		0xFFFD
#define	nBROAD_ZCZR		0xFFFC
#define	nBROAD_ZR_LowPwr	0xFFFB
#define	nBROAD_RSV_FA		0xFFFA
#define	nBROAD_RSV_F9		0xFFF9
#define	nBROAD_RSV_F8		0xFFF8
#define	nBROAD_LOWEST		0xFFF8	// same to nBROAD_RSV_F8

////////////////////////////////////////////////////////////////
/////////////////////// NWK Primitive ID Code /////////////////////
////////////////////////////////////////////////////////////////

#define	nID_REQ			0x04
#define	nID_RSP				0x05
#define	nID_CON			0x06
#define	nID_IND				0x07

#define	nID_DATA			0x00
#define	nID_NET_DIS		0x10
#define	nID_NET_FOR		0x20
#define	nID_PERMIT_JOIN	0x30
#define	nID_START_ROU		0x40
#define	nID_JOIN			0x50
#define	nID_DIRECT_JOIN	0x60
#define	nID_LEAVE			0x70
#define	nID_RESET			0x80
#define	nID_SYNC			0x90
#define	nID_SET				0xA0
#define	nID_GET			0xB0
#define	nID_ROU_DIS		0xC0
#define	nID_ROU_ERR		0xD0
#define	nID_FORCE_JOIN		0xE0
#define	nID_GET_TABLE		0xF0

////////////////////////////////////////////////////////////////
/////////////////////// NWK Status Code /////////////////////////
////////////////////////////////////////////////////////////////

#define	nSTA_SUCCESS			0x00
#define	nSTA_INVALID_PARA		0xC1
#define	nSTA_INVALID_REQ		0xC2
#define	nSTA_NOT_PERMIT		0xC3
#define	nSTA_STARTUP_FAIL		0xC4
#define	nSTA_ALREADY_PRESENT	0xC5
#define	nSTA_SYNC_FAIL		0xC6
#define	nSTA_NEIGH_TAB_FULL	0xC7
#define	nSTA_UNKNOWN_DEVICE	0xC8
#define	nSTA_UNSUPPORT_ATTRI	0xC9
#define	nSTA_NO_NETWORKS		0xCA
#define	nSTA_LEAVE_UNCONFIRM	0xCB
#define	nSTA_MAX_FRM_CNTR	0xCC
#define	nSTA_NO_KEY			0xCD
#define	nSTA_BAD_CCM_OUTPUT	0xCE
#define	nSTA_NO_ROUTE_CAPA	0xCF
#define	nSTA_ROUTE_DISC_FAIL	0xD0
#define	nSTA_ROUTE_ERROR		0xD1
#define	nSTA_BT_TABLE_FULL	0xD2
#define	nSTA_FRAME_NO_BUFFER	0xD3

#define	nSTA_RP_LONG_MSDU	0xD5
#define	nSTA_RP_NO_RESPONSE	0xD6	// by RadioPulse
#define	nSTA_RP_NOT_DEFINED	0xD7	// by RadioPulse
#define	nSTA_RP_NQ_BUSY		0xDB	// by RadioPulse
#define	nSTA_RP_NO_IMPLEMENT	0xDC	// by RadioPulse
#define	nSTA_RP_REJOINING		0xDD	// by RadioPulse
#define	nSTA_RP_ROUTE_FIRST	0xDE	// by RadioPulse
#define	nSTA_RP_DISCOVERING	0xDF	// by RadioPulse

////////////////////////////////////////////////////////////////
/////////////////////// RT Entry Status ///////////////////////////
////////////////////////////////////////////////////////////////

#define	nRTSTA_ACTIVE			0x00
#define	nRTSTA_DISC_UNDER		0x01
#define	nRTSTA_DISC_FAIL		0x02
#define	nRTSTA_INACTIVE		0x03
#define	nRTSTA_VALID_UNDER	0x04

////////////////////////////////////////////////////////////////
/////////////////////// NWK Command Code //////////////////////
////////////////////////////////////////////////////////////////

#define	nCMD_ROU_REQUEST	0x01
#define	nCMD_ROU_REPLY	0x02
#define	nCMD_NWK_STATUS	0x03
#define	nCMD_LEAVE			0x04
#define	nCMD_ROU_RECORD	0x05
#define	nCMD_REJOIN_REQ	0x06
#define	nCMD_REJOIN_RSP	0x07
#define	nCMD_LINK_STATUS	0x08
#define	nCMD_NWK_REPORT	0x09
#define	nCMD_NWK_UPDATE	0x0A
#define	nCMD_RP_FIND_PARENT	0x80

////////////////////////////////////////////////////////////////
/////////////// NWK Status Command's Status Code  /////////////////
////////////////////////////////////////////////////////////////

#define	nNwkStaCmd_NoRoute			0x00
#define	nNwkStaCmd_TreeLinkFail		0x01
#define	nNwkStaCmd_NonTreeLinkFail	0x02
#define	nNwkStaCmd_LowBattery			0x03
#define	nNwkStaCmd_NoRouteCapa		0x04
#define	nNwkStaCmd_NoIndirectCapa		0x05
#define	nNwkStaCmd_IndirectExpire		0x06
#define	nNwkStaCmd_TargetUnavail		0x07
#define	nNwkStaCmd_TargetUnallocate	0x08
#define	nNwkStaCmd_ParentLinkFail		0x09
#define	nNwkStaCmd_ValidateRoute		0x0A
#define	nNwkStaCmd_SrcRouteFail		0x0B
#define	nNwkStaCmd_M2ORouteFail		0x0C
#define	nNwkStaCmd_AddrConflict		0x0D
#define	nNwkStaCmd_VerifyAddress		0x0E
#define	nNwkStaCmd_PanIdUpdate		0x0F
#define	nNwkStaCmd_NwkAddrUpdate	0x10
#define	nNwkStaCmd_BadFrameCounter	0x11
#define	nNwkStaCmd_BadKeySeqNum		0x12
#define	nNwkStaCmd_RadiusExpire		0x13

////////////////////////////////////////////////////////////////
/////////////////////// NWK Constants ///////////////////////////
////////////////////////////////////////////////////////////////

#define	nwkcCoordCapa			1			// 0 or 1, pre-configured
#define	nwkcDefSecLevel		0x03
#define	nwkcDiscRetryLimit		0x03
#define	nwkcMaxDepth			0x0F
#define	nwkcMinHeadOverhead	0x08
#define	nwkcProtocolVersion		0x02		// 2=ZigBee 2006/2007/PRO
#define	nwkcWaitBeforeValid		0x500		// 1280 ms
#define	nwkcRepairThreshold	0x03
#define	nwkcRouteDiscTime		0x2710		// 10000 ms
#define	nwkcMaxBroadJitter		0x40		// 64 ms
#define	nwkcInitRREQRetry		0x03
#define	nwkcRREQRetry			0x02
#define	nwkcRREQRetryInterval	0xFF		// 255 ms
#define	nwkcMinRREQJitter		0x01		// in 2 millisecond slots
#define	nwkcMaxRREQJitter		0x40		// in 2 millisecond slots
#define	nwkcMaxNSDU			105			// by RadioPulse. 96 --> 105. APS_UPDATE_ExtendMaxAsdu.
#define	nwkcMAXMSDU			116			// by RadioPulse. Full size = 116 or 114
#define	nwkcMACFrameOverhead	0x0B		// MAC Header + MAC Footer=11Byte

#define	nNQ_IDLE	0x00
#define	nNQ_TRANS	0x01
#define	nNQ_WAIT	0x02

#define	nNQ_IDX_RREQ_M2O		0x80
#define	nNQ_IDX_RREQ_DST		0x81
#define	nNQ_IDX_RREQ_REBROAD	0x82
#define	nNQ_IDX_DATA_REBROAD	0x83
#define	nNQ_IDX_DATA_BROAD	0x84
#define	nNQ_IDX_DATA_MULTI	0x85
#define	nNQ_IDX_ADDR_CONFL	0x86
#define	nNQ_IDX_NWK_UPDATE	0x87
#define	nNQ_IDX_NWK_STATUS	0x88

////////////////////////////////////////////////////////////////
/////////////////////// NWK Table ///////////////////////////////
////////////////////////////////////////////////////////////////

typedef struct
{
	UINT16	PanID;
	UINT8	EPID[8];
	UINT8	Chan;
	UINT8	Ver_Stack;
	UINT8	SO_BO;
	UINT8	PermitJoin;
	
	UINT16	NwkAddr;		// out of spec. defined by RadioPulse
	UINT8	ProtoID;			// out of spec. defined by RadioPulse
	UINT8	Depth;			// out of spec. defined by RadioPulse
	UINT8	EnaDESC;		// out of spec. defined by RadioPulse
} NS_DESC;

typedef struct
{
	UINT16	FT		: 2 	;	// bit[1:0]
	UINT16	ProVer	: 4	;	// bit[5:2]
	UINT16	DiscRou	: 2	;	// bit[7:6]
	UINT16	Multi	: 1	;	// bit[8]
	UINT16	Secure	: 1	;	// bit[9]
	UINT16	SrcRou	: 1	;	// bit[10]
	UINT16	DstIEEE	: 1	;	// bit[11]
	UINT16	SrcIEEE	: 1	;	// bit[12]
	UINT16	rsv		: 3	;	// bit[15:13]
} NS_FC_Sub;

typedef union
{
	UINT16		All;		// full word
	NS_FC_Sub	b;
} NS_FC;

typedef struct
{
	UINT8	MultiMode	: 2 	;	// 0=NonMember Mode, 1=Member Mode
	UINT8	NonMemRad	: 3 	;
	UINT8	MaxNonMem	: 3 	;	
} NS_MC_Sub;

typedef union
{
	UINT8		All;		// Full Byte
	NS_MC_Sub	b;
} NS_MC;

typedef struct
{
	UINT8	Cnt;
	UINT8	Index;
	UINT16	List[12];
} NS_SR;

typedef struct 
{
	NS_FC	FC;
	UINT16	DstAddr;
	UINT16	SrcAddr;
	UINT8	Radius;
	UINT8	SN;
	UINT8	DstIEEEAddr[8];	// if FC.DstIEEE=1, present
	UINT8	SrcIEEEAddr[8];	// if FC.SrcIEEE=1, present
	NS_MC	MultiCtrl;		// if FC.Multi=1, present
//	NS_SR	SrcRoute;		// if FC.SrcRoute=1, present
} NS_HEAD;

typedef struct 
{
	UINT8	CID;		// Command ID=0x01
	// bit[7] 		: rsv
	// bit[6]		: 1=Multicast
	// bit[5]		: DstIEEE.
	// bit[4:3] 	: Many-To-One.
	//			0 = No M2O
	//			1 = Sender Supports a route record table
	//			2 = Sender does not support a route record table
	//			3 = rsv
	// bit[2:0]	: rsv
	UINT8	Opt;
	UINT8	RID;		// Request ID
	UINT16	DstAddr;
	UINT8	PathCost;
	UINT8	DstIEEE[8];	// present if Opt.DstIEEE=1
} NS_RREQ_CMD;

typedef struct 
{
	UINT8	CID;		// Command ID=0x02
	// bit[7] 		: rsv
	// bit[6]		: 1=Multicast
	// bit[5]		: RspIEEE
	// bit[4]		: OriIEEE
	// bit[3:0]	: rsv
	UINT8	Opt;
	UINT8	RID;		// Request ID
	UINT16	OrgAddr;	// RREQ's source
	UINT16	RspAddr;	// RREQ's destination
	UINT8	PathCost;
	UINT8	OriIEEE[8];
	UINT8	RspIEEE[8];
} NS_RREP_CMD;

typedef struct {
	UINT8	CID;		// Command ID=0x03
	UINT8	StaCode;
	UINT16	DstAddr;
} NS_NwkSta_CMD;

typedef struct {
	UINT8	CID;		// Command ID=0x03
	UINT8	ERR;		// Error Code
	UINT16	DstAddr;
} NS_RERR_CMD;

typedef struct {
	UINT8	CID;		// Command ID=0x04
	// bit[7]		: Rm Child. 	1=Remove Children
	// bit[6]		: Request.	1=Rm Device, 0=I will leave
	// bit[5]		: Rejoin.		1=Rejoin after leaving
	// bit[4:0]	: rsv
	UINT8	Opt;
} NS_LEAVE_CMD;

typedef struct {
	UINT8	CID;		// Command ID=0x05
	UINT8	RelayCnt;	
//	UINT16	RelayList[2];
} NS_RREC_CMD;

typedef struct {
	UINT8	CID;		// Command ID=0x06
	//-- CapaInfo
	//	bit[7]	: 1=Allocate Addr. Always 1 in ZigBee 2006
	//	bit[6]	: 1=MAC Security enabled
	//	bit[5:4]	: rsv
	//	bit[3]	: 1=RxOnWhenIdle
	//	bit[2]	: 1=Mains-powered
	//	bit[1]	: 1=(Joining Device is ZR) && (JoinAsRouter=1)
	//	bit[0]	: 1=Alternative Coordinator		
	UINT8	CapaInfo;	
} NS_RejREQ_CMD;

typedef struct {
	UINT8	CID;		// Command ID=0x07
	UINT16	SrcAddr;
	UINT8	RejSta;
} NS_RejRSP_CMD;

typedef struct {
	UINT16	NeighborNwkAddr;
	// bit[7]		: rsv
	// bit[6:4]	: Outgoing cost
	// bit[3]		: rsv
	// bit[2:0]	: Incoming cost
	UINT8	LinkStatus;
} NS_LinkStatusList;

typedef struct {
	UINT8	CID;		// Command ID=0x08
	//	bit[7]	: rsv
	//	bit[6]	: last frame
	//	bit[5]	: first frame
	//	bit[4:0]	: Entry Count
	UINT8	Opt;
	//NS_LinkStatusList	LinkStaList[N];
} NS_LinkSta_CMD;

typedef struct {
	UINT8	CID;		// Command ID=0x09
	// bit[7:5]	: Report Cmd ID. 0x00=PAN ID Conflict. 0x01~0x07=rsv
	// bit[4:0]	: Report Info Count
	UINT8	Opt;
	UINT8	EPID[8];
	//UINT16	ReportInfo[N];
} NS_NwkRep_CMD;

typedef struct {
	UINT8	CID;		// Command ID=0x0A
	// bit[7:5]	: Update Cmd ID. 0x00=PAN ID Update. 0x01~0x07=rsv
	// bit[4:0]	: Update Info Count
	UINT8	Opt;
	UINT8	EPID[8];
	UINT8	UpdateID;
	UINT16	NewPanID;
} NS_NwkUp_CMD;

typedef union
{
	NS_RREQ_CMD 	RouREQ;		// Command ID=0x01
	NS_RREP_CMD	RouREP;		// Command ID=0x02
	NS_NwkSta_CMD	NwkSta;		// Command ID=0x03
	NS_LEAVE_CMD	Leave;		// Command ID=0x04
	NS_RREC_CMD	RouREC;		// Command ID=0x05
	NS_RejREQ_CMD	RejREQ;		// Command ID=0x06
	NS_RejRSP_CMD	RejRSP;		// Command ID=0x07
	NS_LinkSta_CMD	LinkSta;		// Command ID=0x08
	NS_NwkRep_CMD	NwkReport;	// Command ID=0x09
	NS_NwkUp_CMD	NwkUpdate;	// Command ID=0x0A
} NS_PAY;

// Routing Table
typedef struct
{
	UINT16	DstAddr;		
	//-- RtOption
	// bit[7]		: rsv
	// bit[6]		: No Route Cache
	// bit[5]		: GroupID Flag
	// bit[4]		: Route Record Required
	// bit[3] 		: Many-To-One
	// bit[2:0]	: rsv
	UINT8	RtOption;
	UINT8	Sta;			// nRTSTA_ACTIVE ....
	UINT16	NextHop;
	UINT8	EnaRT;
} NS_RT;

typedef struct
{
	UINT16	GroupID;
	UINT8	EnaGID;
} NS_GID;

typedef struct
{
	UINT16	NwkAddr;
	UINT16	RelayCount;
	UINT16	Path[12];
	UINT8	EnaRRECT;	
} NS_RRECT;		// Route Record Table

typedef struct
{
	UINT8	IEEEAddr[8];
	UINT16	NwkAddr;
	UINT8	EnaNAM;
} NS_NAM;

#define	AND_RT_OPT_GID	0x20
#define	AND_RT_OPT_RouRec	0x10
#define	AND_RT_OPT_M2O	0x08
#define	AND_RT_OPT_Sta	0x07

// Route Discovery Table
typedef struct
{
	UINT8	RouID;
	UINT16	SrcAddr;
	UINT16	SenderAddr;
	UINT8	ForCost;		// cost from Route Request Source To Me
	UINT8	ResCost;	// cost from Me to Route Destination
	UINT16	RdtExpTime;	// if ExpTime != 0, valid entry
	UINT16	Option;
} NS_RDT;

typedef struct
{
	UINT8	IEEEAddr[8];
	UINT16	NwkAddr;
	UINT8	DevType;
	UINT8	RxOnWhenIdle;
	UINT8	Relation;	
	UINT8	TxFailure;
	UINT8	NtLQI;
	UINT8	OutCost;
	UINT8	Age;
	//UINT32	BeaconRxTime;
	//UINT32	BeaconTxOffset;
	UINT8	EnaNT;
} NS_NT;


typedef struct
{
	UINT8	VerStack;
	//-- Option
	//	bit[7]	: EndDevCapa. 1=Accept Joining from ZED
	//	bit[6:3]	: DevDepth
	//	bit[2]	: RouterCapa;
	//	bit[1]	: reserved
	//	bit[0] 	: 1=Association Permit
	UINT8	Option;
	UINT8	EPID[8];
	UINT8	Chan;
	UINT8	BoSo;
	UINT16	PanID;
	UINT8	UpdateID;
	UINT8	EnaOptNT;
} NS_OptionalNT;


#define	nNT_RELATION_PARENT		0x00
#define	nNT_RELATION_CHILD		0x01
#define	nNT_RELATION_SIBLING		0x02
#define	nNT_RELATION_NO			0x03
#define	nNT_RELATION_PREV_CHILD	0x04
#define	nNT_RELATION_UNAUTHEN	0x05
#define	nNT_DEVTYPE_ZC	0x00
#define	nNT_DEVTYPE_ZR	0x01
#define	nNT_DEVTYPE_ZED	0x02

// Broadcast Transaction Table
typedef struct
{
	UINT16	SrcAddr;
	UINT8	SeqNum;
	UINT16	BttExpTime;		// Max 10000 ms
} NS_BTT;

// MAC Beacon Payload
typedef struct
{
	UINT8	ProtocolID;
	// bit[7:4]	: ProtoclVer
	// bit[3:0]	: StackProfile
	UINT8	OPT1;
	// bit[7]		: 1=Can have children
	// bit[6:3]	: Max Device Depth
	// bit[2] 		: 1=Router Capable
	// bit[1:0]	: rsv
	UINT8	OPT2;
	UINT8	EPID[8];
	UINT8	TxOffset[3];	// optional field. present in multi-hop beacon network
} NS_BeaPay;

typedef struct
{
	UINT16	In_NwkAddr;	
	UINT8	Out_Depth;
	UINT16	Out_ParentAddr;
	UINT8	Out_ZED;
} NS_AddrAssign;

typedef struct
{
	UINT8	PS;
	UINT8	MaxMask;
	UINT8	Interval;
	UINT8	Retry;
	UINT8	Idx;
	MCPS_DATA_REQ		MAC_DataReq;
	MCPS_DATA_CON 		MAC_DataCon;
	UINT8	MSDU[nwkcMAXMSDU];
} NS_StateMachine;

typedef union
{
	NS_NT		*pNT;
	NS_RT		*pRT; 
 	NS_BTT		*pBTT; 		
	NS_GID		*pGID;
	NS_RRECT	*pRRECT; 
	NS_NAM		*pNAM; 
} NS_EntryPointer;

typedef struct
{	
	// ZigBee2007 Standard
	UINT16	nwkPanID;					// 0x80. Z2007	
	UINT8	nwkDSN;					// 0x81
	UINT8	nwkPassiveAckTimeout;		// 0x82	// in sec
	UINT8	nwkMaxBroadcastRetry;		// 0x83
	UINT8	nwkMaxChildren;				// 0x84
	UINT8	nwkMaxDepth;				// 0x85
	UINT8	nwkMaxRouter;				// 0x86
//	NS_NT	*pnwkNT;					// 0x87
	UINT16	nwkNetBroadDeliverTime;	// 0x88	// in ms
	UINT8	nwkReportConstantCost;		// 0x89	// 0=Link Cost by LQI, 1=constant Link Cost
	UINT8	nwkRouDiscRetryPermit;		// 0x8A
//	NS_RT	*pRT;						// 0x8B
	UINT8	nwkTimeStamp;				// 0x8C. Z2007
	UINT16	nwkTxTotal;					// 0x8D. Z2007
	UINT8	nwkSymLink;				// 0x8E
	//-- nwkCapaInfo
	//	bit[7]	: AllocAddr	
	//	bit[6]	: SecureCapa 
	//	bit[5:4]	: rsv			- 
	//	bit[3]	: RxOnWhenIdle 	- 
	//	bit[2]	: PowerSrc 	- 1=Mains-powered
	//	bit[1]	: DevType 	- 1=(Joining Device is ZR) && (JoinAsRouter=1). RouteCapa.
	//	bit[0]	: AlterCoord	- 
	UINT8	nwkCapaInfo;				// 0x8F	
	UINT8	nwkAddrAlloc;				// 0x90. 0=Distributed, 2=Stochastic. Z2007
	UINT8	nwkUseTreeRoute;			// 0x91
	UINT16	nwkManagerAddr;			// 0x92. Z2007
	UINT8	nwkMaxSrcRoute;			// 0x93. Z2007
	UINT8	nwkUpdateId;				// 0x94. Z2007
	UINT16	nwkTransPersisTime;		// 0x95	// in superframe periods(macTransactionTime)
	UINT16	nwkNetworkAddr;			// 0x96	
	UINT8	nwkStackProfile;				// 0x97
// 	NS_BTT	*pnwkBTT;					// 0x98. Z2007
//	NS_GID	*pGID;						// 0x99
	UINT8	nwkEPID[8];					// 0x9A

	UINT8	nwkUseMulticast;			// 0x9B
//	NS_RRECT	*pRRECT;				// 0x9C
	UINT8	nwkIsConcentrator;			// 0x9D
	UINT8	nwkConcentratorRadius;		// 0x9E
	UINT8	nwkConcentratorDiscTime;	// 0x9F
	
	UINT8	nwkLinkStatusPeriod;		// 0xA6, in second
	UINT8	nwkRouterAgeLimit;			// 0xA7
	UINT8	nwkUniqueAddr;				// 0xA8
//	NS_NAM		*pnwkAddrMap;			// 0xA9

	// RadioPulse Specific
	UINT8	DEVTYPE;		// 'C'=Coordinator, 'R'=Router, 'E'=End Device, 'N'=Not Joined

	UINT8	Child_AssignZR;		// Address Allocation.
	UINT8	Child_CurrZR;		// Associated ZR's number
	UINT8	Child_ReuseZR;		// Reuse Address Number
	
	UINT8	Child_AssignZED;
	UINT8	Child_CurrZED;
	UINT8	Child_ReuseZED;

	//-- UpdateReport	: If following NIB is changed, corresponding bit is set to 1
	//	bit[15]	: reserved
	//	bit[14]	: A RT Entry is deleted.
	//	bit[13]	: A NT Entry is deleted.
	//	bit[12]	: A NAM Entry is deleted.
	// 	bit[11]	: reserved
	//	bit[10]	: RT(Routing Table) Entry is added or changed.
	//	bit[9]	: NT(Neighbor Table) Entry is added or changed.
	//	bit[8]	: NAM(Network Address Map Table) Entry is added or changed.
	//	bit[7:5]	: reserved
	//	bit[4]	: Link Status Command is transmitted.
	//	bit[3]	: reserved
	//	bit[2]	: PPIB.phyCurrentChannel
	//	bit[1]	: NIB.nwkNetworkAddr;
	//	bit[0] 	: DEVTYPE
	UINT16	UpdateReport;

	UINT8	SIZE_NT;
	UINT8	SIZE_RT;
	UINT8	SIZE_GID;
	UINT8	SIZE_RDT;
	UINT8	SIZE_BTT;
	UINT8	SIZE_RRECT;
	UINT8	SIZE_NAM;
	UINT8	SIZE_ReuseZRAddr;
	UINT8	SIZE_ReuseZEDAddr;	

	NS_RDT		*pRDT;
	NS_BTT		*pBTT;

	NS_NT		*pNT;
	NS_RT		*pRT;			// 0x8B
	NS_GID		*pGID;
	NS_RRECT	*pRRECT;		// Route Record Table
	NS_NAM		*pNAM;			// Network Address Map
	UINT16		*pReuseZRAddr;
	UINT16		*pReuseZEDAddr;

	UINT8		UpdateChan;			// Frequency Agility

	UINT8		RREQNum;			// Route Request Sequence Number	
	UINT8		InitRREQRetry;		// Initial Route Request Retry Number
	UINT8		RREQRetry;			// Route Request Retry Number
	UINT16		RouteDiscTime;		// Route Discovery Time in ms
	UINT8		TxRetry;			// NWK Transmit Retry Number
	UINT16		WdtPeriod;			// auto watchdog period in ms

	UINT8		MinValidLQI;			
	UINT8		MaxLinkFail;			// 0=Disable
	UINT8		JoinPriority;			// 0=Depth, 1=LQI
	UINT8		MinJoinLQI;
	UINT8		MinNtRelayLQI;
	UINT8		ProtocolVersion;
	UINT8		RejoinOption;		// AttID = 0x99
} NS_PIB;

typedef struct
{
	NS_StateMachine	NQ[NWKQUE_DEPTH];
	UINT16		UpdatePanID;
} NS_OPTION;

////////////////////////////////////////////////////////////////
/////////////////////// NWK REQUEST primitive ////////////////////
////////////////////////////////////////////////////////////////

typedef struct
{
	UINT8	DstMode;		// 1=Multicast GroupID, 2=Device Addr or Broadcast
	UINT16	DstAddr;
	UINT8	nsduLen;
	UINT8	nsduHandle;
	UINT8	Radius;
	UINT8	NoMemRadius;		// 0x00 ~ 0x07		
	UINT8	DiscRoute;
	UINT8	Security;
	UINT8	*pnsdu;
} NLDE_DATA_req;

typedef struct
{
	UINT8	Sta;
	UINT8	nsduHandle;
	UINT16	TxTime;		// valid if nwkTimeStamp=1
} NLDE_DATA_con;
  
typedef struct
{
	UINT8	DstMode;	// 1=Multicast GroupID, 2=Device Addr or Broadcast
	UINT16	DstAddr;
	UINT16	SrcAddr;
	UINT8	nsduLen;
	UINT8	LQI;
	UINT16	RxTime;	
	UINT8	SecureUse;
	UINT8	*pnsdu;
 } NLDE_DATA_ind;

// By ZC or ZR
typedef struct
{
	UINT8	DstMode;		// 0=No Dst(Many-To-One mode), 1=Multicast GroupID, 2=Device Addr
	UINT16	DstAddr;
	UINT8	Radius;
	UINT8	NoRouteCache;
} NLME_ROUTE_DISC_req;

typedef struct
{
	UINT8	Sta;
	UINT8	NwkStaCode;
} NLME_ROUTE_DISC_con;


typedef struct
{
	UINT32	ScanChan;
	UINT8	ScanDur;
} NLME_NET_DISC_req;

typedef struct
{
	UINT8	EPID[8];
	UINT8	Chan;
	UINT8	Stack;
	UINT8	Version;
	UINT8	BO;
	UINT8	SO;
	UINT8	PermitJoin;
	UINT8	RouterCapa;
	UINT8	EndDevCapa;
} NS_NwkDesc;

typedef struct
{
	UINT8		Sta;	
	UINT8		NwkCount;
	NS_NwkDesc	*pNwkDesc;		
} NLME_NET_DISC_con;

typedef struct
{
	UINT32	ScanChan;
	UINT8	ScanDur;
	UINT8	BO;
	UINT8	SO;
	UINT8	BattExtend;
} NLME_NET_FORM_req;

typedef struct
{
	UINT8	Sta;
} NLME_NET_FORM_con;

typedef struct
{
	UINT8	PermitDur;
} NLME_PERMIT_JOIN_req;

typedef struct
{
	UINT8	Sta;
} NLME_PERMIT_JOIN_con;

typedef struct
{
	UINT8	BO;
	UINT8	SO;
	UINT8	BattExtend;
} NLME_START_ROU_req;

typedef struct
{
	UINT8	Sta;
} NLME_START_ROU_con;

typedef struct
{
	UINT8	EPID[8];
	//--	Rejoin
	//	0	: Joining through association
	// 	1	: Joining directly or rejoining through orphaning
	//	2	: Joining through NWK rejoining
	//	3	: Changing network channel
	UINT8	Rejoin;
	UINT32	ScanChan;
	UINT8	ScanDur;
	//-- CapaInfo
	//	bit[7]	: 1=Allocate Addr. Always 1 in ZigBee 2006
	//	bit[6]	: 1=MAC Security enabled
	//	bit[5:4]	: rsv
	//	bit[3]	: 1=RxOnWhenIdle
	//	bit[2]	: 1=Mains-powered
	//	bit[1]	: 1=(Joining Device is ZR) && (JoinAsRouter=1) ==> 1=FFD
	//	bit[0]	: 1=Alternative Coordinator	
	UINT8	CapaInfo;
	UINT8	SecureEna;
} NLME_JOIN_req;

typedef struct
{
	//- ForceMode
	//	bit[7:3]	: reserved
	//	bit[2]	: 1=EPID is valid
	//	bit[1]	: 1=PanID is valid
	//	bit[0] 	: 1=NwkAddr is valid
	UINT8	ForceMode;
	UINT16	NwkAddr;
	UINT16	PanID;
	UINT8	EPID[8];
//	UINT8	Chan;
	UINT32	ScanChan;
	//UINT8	JoinAsRou;
	//-- CapaInfo
	//	bit[7]	: 1=Allocate Addr. Always 1 in ZigBee 2006
	//	bit[6]	: 1=MAC Security enabled
	//	bit[5:4]	: rsv
	//	bit[3]	: 1=RxOnWhenIdle
	//	bit[2]	: 1=Mains-powered
	//	bit[1]	: 1=(Joining Device is ZR) && (JoinAsRouter=1) ==> 1=FFD
	//	bit[0]	: 1=Alternative Coordinator		
	UINT8	CapaInfo;
	UINT8	SecureEna;
} NLME_FORCE_JOIN_req;

typedef struct
{	
	UINT8	Sta;
	UINT16	NwkAddr;
	UINT8	EPID[8];
	UINT8	Chan;
} NLME_JOIN_con;

typedef struct
{
	UINT8	Sta;
	UINT16	NwkAddr;
	UINT8	EPID[8];
	UINT8	Chan;
} NLME_FORCE_JOIN_con;


typedef struct
{
	UINT16	ShortAddr;
 	UINT8	ExtendAddr[8];
	//-- 	CapaInfo
	// bit[7]		: Allocate Addr. 1=Can be a PAN Coordinator
	// bit[6]		: Security
	// bit[5:4]	: rsv
	// bit[3]		: RxOnWhenIdle
	// bit[2]		: Power Source. 1=Mains Power
	// bit[1]		: Device Type. 1=FFD, 0=RFD
	// bit[0]		: Alternative PAN Coordinator
	UINT8	CapaInfo;
	//--	RejonNwk
	//	0	: through association
	//	1	: directly or orphaning
	//	2	: NWK rejoning
	UINT8	RejoinNwk;
	UINT8	SecRejoin;
} NLME_JOIN_ind;


typedef struct
{
	UINT8	DevAddr[8];
	//-- 	CapaInfo
	// bit[7]		: Allocate Addr. 1=Can be a PAN Coordinator
	// bit[6]		: Security
	// bit[5:4]	: rsv
	// bit[3]		: RxOnWhenIdle
	// bit[2]		: Power Source. 1=Mains Power
	// bit[1]		: Device Type. 1=FFD, 0=RFD
	// bit[0]		: Alternative PAN Coordinator
	UINT8	CapaInfo;
} NLME_DIRECT_JOIN_req;

typedef struct
{
	UINT8	DevAddr[8];
	UINT8	Sta;
} NLME_DIRECT_JOIN_con;


typedef struct
{
	UINT8	DevAddr[8];
	UINT8	RmChild;
	UINT8	Rejoin;
} NLME_LEAVE_req;

typedef struct
{
	UINT8	Sta;
	UINT8	DevAddr[8];
} NLME_LEAVE_con;

typedef struct
{
	UINT8	DevAddr[8];
	UINT8	Rejoin;
} NLME_LEAVE_ind;


typedef struct
{
	UINT8	WarmStart;
} NLME_RESET_req;


typedef struct
{
	UINT8	Sta;	
} NLME_RESET_con;


typedef struct
{
	UINT8	Track;
} NLME_SYNC_req;

typedef struct
{
	UINT8	Sta;
} NLME_SYNC_con;

typedef struct
{
	UINT8	NoParameter;
} NLME_SYNC_ind;


typedef struct
{
	UINT8	AttID;
} NLME_GET_req;

typedef struct
{
	UINT8	Sta;
	UINT8	AttID;
	UINT16	AttLen;
	UINT8	AttValue[16];
} NLME_GET_con;

typedef struct
{
	UINT8	AttID;
	UINT16	AttLen;
	UINT8	AttValue[16];
} NLME_SET_req;

typedef struct
{
	UINT8	Sta;
	UINT8	AttID;
} NLME_SET_con;

typedef struct
{
	UINT16	ShortAddr;		// address of failed routing destination
	UINT8	Sta;
} NLME_ROUTE_ERR_ind;

typedef struct
{
	UINT8	Sta;
	UINT16	NwkAddr;
} NLME_NWK_STATUS_ind;

typedef struct
{
	UINT8	AttID;
	UINT8	AttLen;
	UINT8	AttEntry;
	UINT8	*pEntry;
} NLME_SET_TABLE_req;

typedef struct
{
	UINT8	Sta;
	UINT8	AttID;
} NLME_SET_TABLE_con;

typedef struct
{
	UINT8	AttID;
} NLME_GET_TABLE_req;

typedef struct
{
 	UINT8	Sta;
 	UINT8	AttID;
 	UINT8	EntryLen;
 	UINT8	*pEntry;
} NLME_GET_TABLE_con;

typedef struct
{
	UINT32	ScanChan;
	UINT8	ScanDur;
} NLME_ED_SCAN_req;

typedef struct
{
	UINT8	Sta;
	UINT32	UnscanChan;
	UINT8	EnergyDetectList[16];
} NLME_ED_SCAN_con;

////////////////////////////////////////////////////////////////
/////////////////////// NWK primitive ////////////////////////////
////////////////////////////////////////////////////////////////

typedef union 
{
	NLDE_DATA_req			DATA_REQ;
	NLME_RESET_req			RESET_REQ;
	NLME_NET_FORM_req		NET_FORM_REQ;
	NLME_NET_DISC_req		NET_DISC_REQ;
	NLME_JOIN_req			JOIN_REQ;
	NLME_START_ROU_req		START_ROU_REQ;
	NLME_PERMIT_JOIN_req	PERMIT_JOIN_REQ;
	NLME_ED_SCAN_req		ED_SCAN_REQ;
	NLME_LEAVE_req			LEAVE_REQ;	
	NLME_DIRECT_JOIN_req	DIRECT_JOIN_REQ;
	NLME_GET_req			GET_REQ;
	NLME_SET_req			SET_REQ;
	NLME_SYNC_req			SYNC_REQ;
	NLME_ROUTE_DISC_req	ROU_DISC_REQ;	
	NLME_FORCE_JOIN_req		FORCE_JOIN_REQ;	// out of spec. by RadioPulse
	NLME_GET_TABLE_req		GET_TABLE_REQ;		// out of spec. by RadioPulse
	NLME_SET_TABLE_req		SET_TABLE_REQ;		// out of spec. by RadioPulse
} NLDME_REQ_PKT;

typedef union 
{
	NLDE_DATA_con			DATA_CON;
	NLME_RESET_con			RESET_CON;
	NLME_NET_FORM_con		NET_FORM_CON;
	NLME_NET_DISC_con		NET_DISC_CON;
	NLME_JOIN_con			JOIN_CON;
	NLME_START_ROU_con		START_ROU_CON;
	NLME_PERMIT_JOIN_con	PERMIT_JOIN_CON;
	NLME_ED_SCAN_con		ED_SCAN_CON;
	NLME_LEAVE_con			LEAVE_CON;
	NLME_DIRECT_JOIN_con	DIRECT_JOIN_CON;
	NLME_GET_con			GET_CON;
	NLME_SET_con			SET_CON;
	NLME_SYNC_con			SYNC_CON;
	NLME_ROUTE_DISC_con	ROU_DISC_CON;
	NLME_FORCE_JOIN_con		FORCE_JOIN_CON;	// out of spec. by RadioPulse
	NLME_GET_TABLE_con		GET_TABLE_CON;	// out of spec. by RadioPulse
	NLME_SET_TABLE_con		SET_TABLE_CON;		// out of spec. by RadioPulse
} NLDME_CON_PKT;

typedef union 
{
	NLDE_DATA_ind			DATA_IND;
	NLME_JOIN_ind			JOIN_IND;
	NLME_LEAVE_ind			LEAVE_IND;	
	NLME_SYNC_ind			SYNC_IND;	
	NLME_NWK_STATUS_ind	NWK_STATUS_IND;
} NLDME_IND_PKT;

	#define	nNewDevType	0x0001
	#define	nNewAddress 	0x0002
	#define	nNewChannel	0x0004
	#define	nTxLinkStatus	0x0010
	#define	nNewNAM		0x0100
	#define	nNewNT			0x0200
	#define	nNewRT			0x0400
	#define	nDeleteNAM		0x1000
	#define	nDeleteNT		0x2000
	#define	nDeleteRT		0x4000
	#define	UpdateNIB(UpdateValue)		(NIB.UpdateReport |= UpdateValue)


	#define	nAQ_IDLE	0x00
	#define	nAQ_PEND	0x01
	
	#define	nAQ_IDX_DATA	0
	#define	nAQ_IDX_CMD	1	
	#define	nAQ_IDX_ACK	2
	#define	nAQ_IDX_DATA_RELAY_FromZED	3
	#define	nAQ_IDX_FRAG_TX_TIMEOUT		4

typedef struct
{
	UINT8	PS;
	UINT8	Idx;
	NLDE_DATA_req	Nwk_DataReq;
	NLDE_DATA_con	Nwk_DataCon;
	UINT8	NSDU[nwkcMaxNSDU];	
} NS_AQ_State;

	 

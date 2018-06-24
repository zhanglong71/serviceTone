
/*******************************************************************************
	- Chip		: MG2450/2455
	- Vendor		: RadioPulse Inc, 2007.
	- Date		: 2010-11-15
	- Version		: VER 2.2

	[2010-11-15] VER 2.2
	- For ZigBeePRO Development Kit V2.2
		+ ZAPS_SET_CONFIG_REQ() is added.
*******************************************************************************/

	//----------------------------------------------------------------------- 
	//
	// 	Profile ID and ZDO Endpoint
	//
	//----------------------------------------------------------------------- 	
 	#define	ProfileHA		0x0104
	#define	ProfileZDP		0x0000
	#define	EndpointZDO		0x00 

 
	//----------------------------------------------------------------------- 
	//
	// 	ZCL	:	STATUS 
	//
	//----------------------------------------------------------------------- 
	#define	zclSTA_SUCCESS				0x00	// Success 
	#define	zclSTA_FAIL					0x01	// Fail 
	#define	zclSTA_MALFORM_CMD		0x80	// Malformed Cmd 
	#define	zclSTA_UNSUP_CCMD			0x81	// Unsupported Cluster Command 
	#define	zclSTA_UNSUP_GCMD			0x82	// Unsupported General Command 
	#define	zclSTA_UNSUP_MANU_CID	0x83	// Unsupported Manufacturer Cluster Command 
	#define	zclSTA_UNSUP_MANU_GCMD	0x84	// Unsupported Manufacturer General Command 
	#define	zclSTA_INV_FIELD			0x85	// Invalid Field 
	#define	zclSTA_UNSUP_ATT			0x86	// Unsupported Attribute 
	#define	zclSTA_INV_VALUE			0x87	// Invalid Attribute Value 
	#define	zclSTA_READ_ONLY			0x88	// Read Only Attribute 
	#define	zclSTA_INSUFF_SPACE		0x89	// Insufficient Space 
	#define	zclSTA_DUPLE_EXISTS		0x8A	// Entry is not created due to duplication(entry is already present) 
	#define	zclSTA_NOT_FOUND			0x8B	// Requested information is not found 
	#define	zclSTA_UNREPORT_ATT		0x8C	// Unreportable Attribute 
	#define	zclSTA_INV_DTYPE			0x8D	// Invalid Data Type 
	#define	zclSTA_HW_FAIL				0xC0	// HW Fail 
	#define	zclSTA_SW_FAIL				0xC1	// SW Fail 
	#define	zclSTA_CALIB_ERROR			0xC2	// Calibration Error 
 
 
	//----------------------------------------------------------------------- 
	//
	// 	ZCL	:	DATA TYPE 
	//
	//----------------------------------------------------------------------- 
	#define	ZCL_DTYPE_Null		0x00 
	#define	ZCL_DTYPE_1Byte	0x08 
	#define	ZCL_DTYPE_2Byte	0x09 
	#define	ZCL_DTYPE_3Byte	0x0A 
	#define	ZCL_DTYPE_4Byte	0x0B 
	#define	ZCL_DTYPE_Bool		0x10 
	#define	ZCL_DTYPE_8bit		0x18 
	#define	ZCL_DTYPE_16bit	0x19 
	#define	ZCL_DTYPE_24bit	0x1A 
	#define	ZCL_DTYPE_32bit	0x1B 
	#define	ZCL_DTYPE_UINT8	0x20 
	#define	ZCL_DTYPE_UINT16	0x21 
	#define	ZCL_DTYPE_UINT24	0x22 
	#define	ZCL_DTYPE_UINT32	0x23 
	#define	ZCL_DTYPE_INT8		0x28 
	#define	ZCL_DTYPE_INT16	0x29 
	#define	ZCL_DTYPE_INT24	0x2A 
	#define	ZCL_DTYPE_INT32	0x2B 
	#define	ZCL_DTYPE_Enum1B	0x30 
	#define	ZCL_DTYPE_Enum2B	0x31 
	#define	ZCL_DTYPE_Float2B	0x38 
	#define	ZCL_DTYPE_Float4B	0x39 
	#define	ZCL_DTYPE_Float8B	0x3A 
	#define	ZCL_DTYPE_StrOctet	0x41	// length is defined in 1st octet 
	#define	ZCL_DTYPE_StrChar	0x42	// length is defined in 1st octet 
	#define	ZCL_DTYPE_Time	0xE0	// Len = 4Byte 
	#define	ZCL_DTYPE_Date		0xE1	// Len = 4Byte 
	#define	ZCL_DTYPE_CID		0xE8	// Len = 2Byte 
	#define	ZCL_DTYPE_AttID	0xE9	// Len = 2Byte 
	#define	ZCL_DTYPE_BAC		0xEA	// Len = 4Byte, BACnet OID 
	#define	ZCL_DTYPE_IEEE		0xF0	// Len = 8Byte 
	#define	ZCL_DTYPE_Unknown	0xFF	// Len = 0Byte 
 
	//----------------------------------------------------------------------- 
	//
	// 	ZCL	:	Command 
	//
	//----------------------------------------------------------------------- 
	#define	ZCL_CMD_READ						0x00 
	#define	ZCL_CMD_READ_RESPONSE			0x01 
	#define	ZCL_CMD_WRITE						0x02 
	#define	ZCL_CMD_WRITE_UNDIVIDE			0x03	 
	#define	ZCL_CMD_WRITE_RESPONSE			0x04 
	#define	ZCL_CMD_WRITE_NORESPONSE		0x05 
	#define	ZCL_CMD_CONFIG_REPORT			0x06 
	#define	ZCL_CMD_CONFIG_REPORT_RSP		0x07 
	#define	ZCL_CMD_READ_REPORT_CONFIG		0x08 
	#define	ZCL_CMD_READ_REPORT_CONFIG_RSP	0x09 
	#define	ZCL_CMD_REPORT					0x0A 
	#define	ZCL_CMD_DEFAULT_RSP				0x0B 
	#define	ZCL_CMD_DISCOVER_ATTRIBUTE		0x0C 
	#define	ZCL_CMD_DISCOVER_ATTRIBUTE_RSP	0x0D 
 
	//----------------------------------------------------------------------- 
	//
	// 	ZCL	:	Cluster 
	//
	//-----------------------------------------------------------------------	 
	// Function Domain : General 
	#define	ZCL_ClusBasic		0x0000 
	#define	ZCL_ClusPwrCfg		0x0001 
	#define	ZCL_ClusTempCfg	0x0002 
	#define	ZCL_ClusIdentify		0x0003 
	#define	ZCL_ClusGroups		0x0004	 
	#define	ZCL_ClusScenes		0x0005 
	#define	ZCL_ClusOnOff		0x0006 
		#define	ZCL_ClusOnOff_AttOnOff	0x0000 
			#define	ZCL_ClusOnOff_AttOnOff_DTYPE			ZCL_DTYPE_Bool	 
		#define	ZCL_ClusOnOff_CmdOff	0x00 
		#define	ZCL_ClusOnOff_CmdOn	0x01 
		#define	ZCL_ClusOnOff_CmdTogg	0x02 
	#define	ZCL_ClusOnOffSwCfg	0x0007 
		#define	ZCL_ClusOnOffSwCfg_AttSwTyp	0x0000 
			#define	ZCL_ClusOnOffSwCfg_AttSwTyp_DTYPE		ZCL_DTYPE_Enum1B 
			#define	ZCL_ClusOnOffSwCfg_AttSwTyp_Togg		0x00 
			#define	ZCL_ClusOnOffSwCfg_AttSwTyp_Momen	0x01 
		#define	ZCL_ClusOnOffSwCfg_AttSwAct	0x0010 
			#define	ZCL_ClusOnOffSwCfg_AttSwAct_DTYPE		ZCL_DTYPE_Enum1B 
			#define	ZCL_ClusOnOffSwCfg_AttSwAct_On2Off		0x00 
			#define	ZCL_ClusOnOffSwCfg_AttSwAct_Off2On		0x01 
			#define	ZCL_ClusOnOffSwCfg_AttSwAct_Togg		0x02 
	#define	ZCL_ClusLevelCtrl	0x0008 
	#define	ZCL_ClusAlarms		0x0009 
	#define	ZCL_ClusTime		0x000A 
	#define	ZCL_ClusRssiLocate	0x000B 
 
	// Function Domain : Closures 
	#define	ZCL_ClusShadeCfg	0x0100 
	 
	// Function Domain : HVAC 
	#define	ZCL_ClusPumpCtrl	0x0200 
	#define	ZCL_ClusThermostat	0x0201 
	#define	ZCL_ClusFanCtrl		0x0202 
	#define	ZCL_ClusDehumiCtrl	0x0203 
	#define	ZCL_ClusTherUsrCtrl	0x0204 
	 
	// Function Domain : Lighting 
	#define	ZCL_ClusColorCtrl	0x0300 
	#define	ZCL_ClusBallastCtrl	0x0301 
 
	// Function Domain : Measurement and Sensing 
	#define	ZCL_ClusIlluMeas	0x0400 
	#define	ZCL_ClusIlluLvlSen	0x0401 
	#define	ZCL_ClusTempMeas	0x0402 
	#define	ZCL_ClusPressMeas	0x0403 
	#define	ZCL_ClusFlowMeas	0x0404 
	#define	ZCL_ClusReHumMeas	0x0405 
	#define	ZCL_ClusOccuSen	0x0406 
 
	// Function Domain : Security and Safety 
	#define	ZCL_ClusIASZone	0x0500 
	#define	ZCL_ClusIASAce		0x0501 
	#define	ZCL_ClusIASWD		0x0502 
 
	//-----------------------------------------------------------------------  
	// 
	// 	DEVICEs defined in Home-Automation(HA) Profile 
	// 
	//-----------------------------------------------------------------------  
	//	Home-Automation	: 	Generic 
	#define	ProfileHA_DeviceID_OnOffSwitch		0x0000 
	#define	ProfileHA_DeviceID_LevelCtrlSwitch	0x0001 
	#define	ProfileHA_DeviceID_OnOffOutput		0x0002 
	#define	ProfileHA_DeviceID_LevelCtrlOutput	0x0003 
	#define	ProfileHA_DeviceID_SceneSelector	0x0004 
	#define	ProfileHA_DeviceID_ConfigureTool	0x0005 
	#define	ProfileHA_DeviceID_RemoteCtrl		0x0006 
	#define	ProfileHA_DeviceID_CombInterface	0x0007 
	#define	ProfileHA_DeviceID_RangeExtender	0x0008 
	#define	ProfileHA_DeviceID_MainsPwrOutlet	0x0009 
	 
	//	Home-Automation	: 	Lighting 
	#define	ProfileHA_DeviceID_OnOffLight		0x0100 
	#define	ProfileHA_DeviceID_DimmLight		0x0101 
	#define	ProfileHA_DeviceID_ColorDimmLight	0x0102 
	#define	ProfileHA_DeviceID_OnOffLightSwitch	0x0103 
	#define	ProfileHA_DeviceID_DimmSwitch		0x0104 
	#define	ProfileHA_DeviceID_ColorDimmSwitch	0x0105 
	#define	ProfileHA_DeviceID_LightSensor		0x0106 
	#define	ProfileHA_DeviceID_OccupySensor	0x0107 
 
	//	Home-Automation	: 	Closure 
	#define	ProfileHA_DeviceID_Shade			0x0200 
	#define	ProfileHA_DeviceID_ShadeController	0x0201 
 
	//	Home-Automation	: 	HVAC 
	#define	ProfileHA_DeviceID_HeatCoolUnit		0x0300 
	#define	ProfileHA_DeviceID_Thermostat		0x0301 
	#define	ProfileHA_DeviceID_TempSensor		0x0302 
	#define	ProfileHA_DeviceID_Pump			0x0303 
	#define	ProfileHA_DeviceID_PumpController	0x0304 
	#define	ProfileHA_DeviceID_PressureSensor	0x0305 
	#define	ProfileHA_DeviceID_FlowSensor		0x0306 
 
	//	Home-Automation	: 	Intruder Alarm System 
	#define	ProfileHA_DeviceID_IAS_Ctrl_Equip	0x0400 
	#define	ProfileHA_DeviceID_IAS_AncilCtrller	0x0401 
	#define	ProfileHA_DeviceID_IAS_Zone		0x0402 
	#define	ProfileHA_DeviceID_IAS_WarnDevice	0x0403 
	// 




 
#ifndef __KEIL_DATA_TYPE__ 
 
	#define	__KEIL_DATA_TYPE__ 
 
	typedef 	unsigned long		UINT32; 
	typedef 	unsigned short 	UINT16; 
	typedef 	unsigned char		UINT8; 
	typedef 	unsigned char		UINT1; 
	typedef	long				INT32; 
	typedef 	short			INT16; 
	typedef 	char				INT8; 
	 
#endif 

  
//----------------------------------------------------------------------- 
//
// 	ZDO Descriptor Structure
//
//----------------------------------------------------------------------- 
	
typedef struct 
{ 
	//--	Avail_Type 
	// bit[7:5]	: rsv 
	// bit[4]		: User Descriptor available 
	// bit[3]		: Complex Descriptor available 
	// bit[2:0]	: Logical Type. 0=ZC, 1=ZR, 2=ZED 
	UINT8	Avail_Type; 
	//--	Freq_ApsFla	g 
	// bit[7]		: Freq Band. reserved 
	// bit[6]		: Freq Band. 1=Support 2.4GHz 
	// bit[5]		: Freq Band. 1=Support 900MHz 
	// bit[4]		: Freq Band. reserved. 
	// bit[3]		: Freq Band. 1=Support 800MHz 
	// bit[2:0]	: APS Flags. Always 0 in ZigBee2006/2007/PRO.
	UINT8	Freq_ApsFlag; 
	//-- 	MacCapa 
	// bit[7]		: Allocate Addr. 1=Can be a PAN Coordinator 
	// bit[6]		: Security 
	// bit[5:4]	: rsv 
	// bit[3]		: RxOnWhenIdle 
	// bit[2]		: Power Source. 1=Mains Power 
	// bit[1]		: Device Type. 1=FFD, 0=RFD 
	// bit[0]		: Alternative PAN Coordinator 
	UINT8	MacCapa; 
	UINT16	Manufacture; 
	UINT8	MaxBufSize;		// Max asdu Size(before fragmentation or re-assembly). 0x00~0x7F 
	UINT16	MaxInTranSize;	// 0x0000 ~ 0x7FFF 
	//-- ServerMask 
	// bit[15:7]	: reserved 
	// bit[6]		: 1=Support Network Manager
	// bit[5]		: 1=Support Backup Discovery Cache 
	// bit[4] 		: 1=Support Primary Discovery Cache 
	// bit[3]  	: 1=Support Backup Binding Table Cache 
	// bit[2]  	: 1=Support Primary Binding Table Cache 
	// bit[1]  	: 1=Support Backup Trust Center 
	// bit[0]  	: 1=Support Primary Trust Center 
	UINT16	ServerMask; 
	UINT16	MaxOutTransSize;
	UINT8	DescCapa;
} ZS_NodeDesc; 
 
typedef struct 
{ 
	//-- AvailPwrSrc_PwrMode 
	//	bit[7]	: Available Power Source. rsv 
	//	bit[6]	: Available Power Source. 1=Support Disposable battery. 
	//	bit[5]	: Available Power Source. 1=Support Rechargeable battery. 
	//	bit[4]	: Available Power Source. 1=Support Mains Power.	 
	//	bit[3:0]	: Current Power Mode. 0 or 1 or 2.	 
	//				0=Sync with RxOnWhenIdle 
	//				1=Receiver comes on periodically as defined by the node power descriptor 
	//				2=Receiver comes on when stimulated 
	UINT8	AvailPwrSrc_CurrPwrMode; 
	 
	//-- CurrPwrLevel_CurrPwrSrc 
	//	bit[7:4]	: Current Power Level. 0=Critical, 1=33, 2=66, 3=100
	//	bit[3]	: Current Power Source. rsv 
	//	bit[2]	: Current Power Source. 1=Support Disposable battery. 
	//	bit[1]	: Current Power Source. 1=Support Rechargeable battery. 
	//	bit[0]	: Current Power Source. 1=Support Mains Power. 
	UINT8	CurrPwrLevel_CurrPwrSrc; 
	 
} ZS_PowerDesc; 
 
	#define	zPM_SYNC_RxOnIdle		0x00	// sync with RxOnWhenIdle of node descriptor 
	#define	zPM_PERIODIC			0x01	// periodically as defined by the node power descriptor 
	#define	zPM_USER_STIMULATE	0x02	// RX is enabled when stimulated.(ex:user pressing a button) 
 
typedef struct 
{	 
	UINT8	EP; 
	UINT16	AppProID; 
	UINT16	AppDevID; 
	UINT8	DevVersion;		// bit[7:4]=rsv, bit[3:0]=DevVersion 
	UINT8	NumInClus; 
	UINT16	InClusList[9];	// Max=9
	UINT8	NumOutClus; 
	UINT16	OutClusList[8]; 
 
	UINT8	EnaSD;			// out of spec. by RadioPulse 
} ZS_SimpDesc;			// Simple Descriptor 
 
typedef struct 
{ 
	UINT32	ChanList; 
	UINT8	EPID[8]; 
	UINT8	ProVer;		// Protocol Version 
	UINT8	StackProf; 
	UINT8	BO; 
	UINT8	SO; 
	UINT8	BattExt; 
} ZS_NWK_Mode_Param; 
 
typedef struct 
{	 
	UINT8	EnaJD; 
	UINT8	IEEEAddr[8]; 
	UINT8	CapaInfo; 
	UINT8	MasterKey[16];		// if security is enabled 
	UINT8	JoinStatus;			// 0=SUCCESS, Others=Fail 
} ZS_JoinDirect; 
 
typedef struct 
{ 
	UINT8	ZR; 
	UINT8	ZED; 
	UINT8	Depth; 
	// 
} ZS_MaxAsso; 
 
typedef struct 
{ 
	// Mandotary 
	ZS_NodeDesc			NodeDesc; 
	ZS_PowerDesc		PowerDesc; 
	ZS_SimpDesc			SimpDesc;
	ZS_NWK_Mode_Param	NwkMode;	 
	UINT8				NwkScanAttempt;		// default=5, 1 ~ 255 
	UINT8				NwkTimeBtwnScan;		// default=1, 1 ~ 255 (second)

	// Optional 
	UINT8				PermitJoinDuration; 
	ZS_MaxAsso			MaxAsso;				// established in the Stack Profile 
	 
	UINT8				NwkBroadDeliverTime;	// established in the Stack Profile. Max = 63 
	UINT16				NwkTransPersistTime;	// established in the Stack Profile 

	UINT8				UserDescriptor[16];	 
} ZS_CONF;


	 //----------------------------------------------------------------------- 
	//
	// 	ZDP Status Code
	//
	//-----------------------------------------------------------------------
	#define	zdpSTA_SUCCESS		0x00 
	#define	zdpSTA_INV_REQTYPE	0x80
	#define	zdpSTA_DEV_NOT_FOU	0x81 
	#define	zdpSTA_INV_EP			0x82 
	#define	zdpSTA_NOT_ACTIVE		0x83 
	#define	zdpSTA_NOT_SUPPORT	0x84 
	#define	zdpSTA_TIMEOUT		0x85 
	#define	zdpSTA_NO_MATCH		0x86 
	#define	zdpSTA_RSV_0x87		0x87
	#define	zdpSTA_NO_ENTRY		0x88 
	#define	zdpSTA_NO_DESCRIPT	0x89 
	#define	zdpSTA_INSUFF_SPACE	0x8A 
	#define	zdpSTA_NOT_PERMIT		0x8B 
	#define	zdpSTA_TABLE_FULL		0x8C 
	#define	zdpSTA_NOT_AUTHOR	0x8D 
 
	//----------------------------------------------------------------------- 
	//
	// 	ZDP Constants
	//
	//-----------------------------------------------------------------------
	#define	REQTYPE_SINGLE		0
	#define	REQTYPE_EXTEND		1

 //----------------------------------------------------------------------- 
//
// 	ZDP Command Structure/Union
//
//-----------------------------------------------------------------------
typedef union 
{ 
	UINT8	Ext[8]; 
	UINT16	Short; 
} ZS_ADD; 
	 
typedef struct 
{	 
	UINT8	IEEEAddr[8]; 
	UINT8	ReqType;		// 0=Single, 1=Extended 
	UINT8	StartIdx;		// if ReqType=1, present 
} ZDPS_NWK_ADDR_req; 
 
typedef struct 
{ 
	UINT8	Sta; 
	UINT8	IEEEAddr[8]; 
	UINT16	NwkAddr;	 
	UINT8	NumAssoDev;	// If ReqType=Single, not present 
	UINT8	StartIdx;		// If ReqType=Single, not present 
	UINT16	NwkAddrAssoDevList[(apscMaxASDU-14)/2];
} ZDPS_NWK_ADDR_rsp; 
 
typedef struct 
{ 
	UINT16	NwkAddr; 
	UINT8	ReqType;		// 0=Single, 1=Extended 
	UINT8	StartIdx;		// if ReqType=1, present 
} ZDPS_IEEE_ADDR_req; 
 
typedef struct 
{ 
	UINT8	Sta; 
	UINT8	IEEEAddr[8]; 
	UINT16	NwkAddr; 
	UINT8	NumAssoDev; 
	UINT8	StartIdx; 
	UINT16	NwkAddrAssoDevList[(apscMaxASDU-14)/2];	
} ZDPS_IEEE_ADDR_rsp;
   
typedef struct 
{ 
	UINT8	SrcIEEE[8];
	UINT8	SrcEP; 
	UINT16	ClusID; 
	UINT8	DstMode; 		// 1=Group Address, 3=IEEE Address
	ZS_ADD	DstAdd_U;
	UINT8	DstEP; 			// Present only if DstMode=3
} ZDPS_BIND_req; 
 
typedef struct 
{ 
	UINT8	Sta; 
} ZDPS_BIND_rsp; 

typedef struct
{
	UINT8	SrcIEEE[8];
	UINT8	SrcEP;
	UINT16	ClusID;
	UINT8	DstMode;
	ZS_ADD	DstAdd_U;
	UINT8	DstEP;			// Present only if DstMode=3
} ZDPS_UNBIND_req;

typedef struct
{
	UINT8	Sta;
} ZDPS_UNBIND_rsp;
 
typedef struct 
{ 
	UINT16	NwkAddr; 
	UINT8	IEEEAddr[8]; 
	UINT8	Capa; 
} ZDPS_DEVICE_annce; 
 
typedef struct 
{ 
	UINT16	NwkAddr; 
} ZDPS_ACTIVE_EP_req; 
 
typedef struct 
{ 
	UINT8	Sta; 
	UINT16	NwkAddr; 
	UINT8	ActiveEpCnt; 
	UINT8	ActiveEpList[apscMaxASDU-5]; 
} ZDPS_ACTIVE_EP_rsp; 
 
typedef struct 
{ 
	UINT16	NwkAddr; 
	UINT8	EP; 
} ZDPS_SIMP_DESC_req; 
	 
typedef struct 
{ 
	UINT8	Sta; 
	UINT16	NwkAddr; 
	UINT8	Length; 
	UINT8	DescBuf[apscMaxASDU-5]; 
} ZDPS_SIMP_DESC_rsp; 
 
typedef struct 
{ 
	UINT16	NwkAddr; 
} ZDPS_NODE_DESC_req; 
 
typedef struct 
{ 
	UINT8	Sta; 
	UINT16	NwkAddr; 
	UINT8	DescBuf[apscMaxASDU-4]; 
} ZDPS_NODE_DESC_rsp; 
 
typedef struct 
{ 
	UINT16	NwkAddr; 
} ZDPS_POWER_DESC_req; 
 
typedef struct 
{ 
	UINT8	Sta; 
	UINT16	NwkAddr; 
	UINT8	DescBuf[apscMaxASDU-4]; 
} ZDPS_POWER_DESC_rsp; 
 
typedef struct 
{ 
	UINT16	NwkAddr; 
	UINT16	ProfileID; 
	UINT8	NumInClus; 
	UINT16	InClusList[8];	// variable length. depend on NumInClus
	UINT8	NumOutClus; 
	UINT16	OutClusList[8]; 	// variable length. depend on NumOutClus
} ZDPS_MATCH_DESC_req; 
 
typedef struct 
{ 
	UINT8	Sta; 
	UINT16	NwkAddr; 
	UINT8	MatchLen; 
	UINT8	MatchList[apscMaxASDU-5]; 
} ZDPS_MATCH_DESC_rsp; 
 
typedef struct 
{ 
	UINT16	ServerMask; 
} ZDPS_SYS_SERV_DISC_req; 
 
typedef struct 
{ 
	UINT8	Sta; 
	UINT16	ServerMask; 
} ZDPS_SYS_SERV_DISC_rsp; 
 
typedef struct
{
	UINT16	NwkAddr;
} ZDPS_COMP_DESC_req;

typedef struct
{
	UINT8	Sta;
	UINT16	NwkAddr;
	UINT8	Len;
	UINT8	CompDesc[apscMaxASDU-5];
} ZDPS_COMP_DESC_rsp;

typedef struct
{
	UINT16	NwkAddr;
	UINT8	IEEEAddr[8];
} ZDPS_DISC_CACHE_req;

typedef struct
{
	UINT8	Sta;
} ZDPS_DISC_CACHE_rsp;

typedef struct
{
	UINT16	NwkAddr;
} ZDPS_USER_DESC_req;

typedef struct
{
	UINT8	Sta;
	UINT16	NwkAddr;
	UINT8	Len;
	UINT8	UserDesc[16];
} ZDPS_USER_DESC_rsp;

typedef struct
{
	UINT16	NwkAddr;
	UINT8	Len;
	UINT8	UserDesc[16];
} ZDPS_USER_DESC_set;

typedef struct
{
	UINT8	Sta;
	UINT16	NwkAddr;
} ZDPS_USER_DESC_conf;

typedef struct
{
	UINT32	ChanMask;
	//-	ScanDur
	//-	0 ~ 5	: ED Scan
	//-	6 ~ 0xFD	: reserved
	//-	0xFE		: change Channel
	//-	0xFF		: change AIB.apsChannelMask, NIB.nwkUpdateId, NIB.nwkManagerAddr
	UINT8	ScanDur;
	UINT8	ScanCount;		// present if ScanDur is 0~5
	UINT8	NwkUpdateID;	// present if ScanDur is 0xFE or 0xFF
	UINT16	NwkManagerAddr;	// present if ScanDur=0xFF
} ZDPS_MgmtNwkUpdateReq;

typedef struct
{
	UINT8	Sta;
	UINT32	ScannedChan;
	UINT16	TotalTrans;
	UINT16	TransFail;
	UINT8	ScannedChanCount;
	UINT8	EnergyValue[16];
} ZDPS_MgmtNwkUpdateNoti;

typedef struct
{
	UINT8	StartIndex;	
} ZDPS_MgmtLQI_req;

typedef struct
{
	UINT8	Sta;
	UINT8	NTEntries;
	UINT8	StartIndex;
	UINT8	NTListCnt;
//			NTList[2];
} ZDPS_MgmtLQI_rsp;

typedef struct
{
	UINT8	StartIndex;	
} ZDPS_MgmtRtg_req;

typedef struct
{
	UINT8	Sta;
	UINT8	RTEntries;
	UINT8	StartIndex;
	UINT8	RTListCnt;
//			RTList[2];
} ZDPS_MgmtRtg_rsp;

typedef struct
{
	UINT8	DevIEEEAddr[8];
	//-	LeaveOption
	//	bit[7:2]	: rsv
	//	bit[1]	: Remove Children
	//	bit[0]	: Rejoin
	UINT8	LeaveOption;
} ZDPS_MgmtLeave_req;

typedef struct
{
	UINT8	Sta;
} ZDPS_MgmtLeave_rsp;

typedef struct
{
	UINT8	PermitDur;
	UINT8	TC_Sign;
} ZDPS_MgmtPermitJoin_req;

typedef struct
{
	UINT8	Sta;
} ZDPS_MgmtPermitJoin_rsp;
 
typedef union 
{ 
	ZDPS_DEVICE_annce			DEV_ANNCE; 
 
	ZDPS_NWK_ADDR_req			NWK_ADDR_REQ; 
	ZDPS_NWK_ADDR_rsp			NWK_ADDR_RSP;	 
	 
	ZDPS_IEEE_ADDR_req			IEEE_ADDR_REQ; 
	ZDPS_IEEE_ADDR_rsp			IEEE_ADDR_RSP;	 
 
	ZDPS_NODE_DESC_req			NODE_DESC_REQ; 
	ZDPS_NODE_DESC_rsp			NODE_DESC_RSP; 
 
	ZDPS_POWER_DESC_req		POWER_DESC_REQ; 
	ZDPS_POWER_DESC_rsp		POWER_DESC_RSP; 
 
	ZDPS_SIMP_DESC_req			SIMP_DESC_REQ; 
	ZDPS_SIMP_DESC_rsp			SIMP_DESC_RSP; 
 
	ZDPS_ACTIVE_EP_req			ACTIVE_EP_REQ; 
	ZDPS_ACTIVE_EP_rsp			ACTIVE_EP_RSP; 
 
	ZDPS_MATCH_DESC_req		MATCH_DESC_REQ; 
	ZDPS_MATCH_DESC_rsp		MATCH_DESC_RSP;	 
 
	ZDPS_SYS_SERV_DISC_req		SYS_SERV_DISC_REQ; 
	ZDPS_SYS_SERV_DISC_rsp		SYS_SERV_DISC_RSP;	 
 
	ZDPS_BIND_req				BIND_REQ; 
	ZDPS_BIND_rsp				BIND_RSP; 

	ZDPS_UNBIND_req				UNBIND_REQ;
	ZDPS_UNBIND_rsp				UNBIND_RSP;
 
	ZDPS_COMP_DESC_req			COMP_DESC_REQ;
	ZDPS_COMP_DESC_rsp			COMP_DESC_RSP;
	
	ZDPS_DISC_CACHE_req		DISC_CACHE_REQ;
	ZDPS_DISC_CACHE_rsp		DISC_CACHE_RSP;
	
	ZDPS_USER_DESC_req			USER_DESC_REQ;
	ZDPS_USER_DESC_rsp			USER_DESC_RSP;
	
	ZDPS_USER_DESC_set			USER_DESC_SET;
	ZDPS_USER_DESC_conf		USER_DESC_CONF;

	ZDPS_MgmtNwkUpdateReq		Mgmt_NwkUpdate_Req;
	ZDPS_MgmtNwkUpdateNoti		Mgmt_NwkUpdate_Noti;

	ZDPS_MgmtLQI_req			Mgmt_LQI_REQ;
	ZDPS_MgmtLQI_rsp			Mgmt_LQI_RSP;

	ZDPS_MgmtRtg_req			Mgmt_Rtg_REQ;
	ZDPS_MgmtRtg_rsp			Mgmt_Rtg_RSP;

	ZDPS_MgmtLeave_req			Mgmt_Leave_REQ;
	ZDPS_MgmtLeave_rsp			Mgmt_Leave_RSP;

	ZDPS_MgmtPermitJoin_req		Mgmt_PermitJoin_REQ;
	ZDPS_MgmtPermitJoin_rsp		Mgmt_PermitJoin_RSP;	
} ZDPS_MSG; 
 
typedef struct 
{ 
	UINT8		Seq; 
	ZDPS_MSG	Msg; 
} ZDPS_PAY; 
 

	//----------------------------------------------------------------------- 
	//
	// 	ZCL Constants
	//
	//-----------------------------------------------------------------------	
	#define	ZCL_FT_EntireCmd		0 
	#define	ZCL_FT_SpecificCmd		1 
	 
	#define	ZCL_DIR_Server2Client	1 
	#define	ZCL_DIR_Cleint2Server	0
 
	#define	ZCL_DefaultRsp_Disable	1 
	#define	ZCL_DefaultRsp_Enable	0 

//----------------------------------------------------------------------- 
//
// 	ZCL Structure
//
//-----------------------------------------------------------------------	 
typedef struct 
{ 
	// 	0	: Command acts across the entire profile 
	//	1	: Command acts to a specific cluster 
	UINT8	FrameType	: 2; 
	UINT8	Manufact	: 1; 
	UINT8	Direction	: 1;		// 1=Server To Client, 0=Client To Server 
	UINT8	DisableRsp	: 1;		// 1=No Response Command. 
	UINT8	rsv			: 3; 
} ZCLS_FC_Sub; 
 
typedef union 
{ 
	UINT8			All; 
	ZCLS_FC_Sub		b; 
} ZCLS_FC; 
 
typedef struct 
{ 
	ZCLS_FC		FC; 
	UINT16		ManuCode;	 
	UINT8		SeqNum; 
	UINT8		CmdID; 
} ZCLS_HEAD; 
 
typedef struct 
{	 
	ZCLS_FC		FC; 
	UINT16		ManuCode;	 
	UINT8		SeqNum; 
	UINT8		CmdID;
	UINT8		Payload[apscMaxASDU-5]; 
	 
	UINT8		PayLen;		// out of spec. by RadioPulse 
} ZCLS_PAY_M;		// ZCL Manufacturer : Including ManuCode 
 
typedef struct 
{	 
	ZCLS_FC		FC; 
	UINT8		SeqNum; 
	UINT8		CmdID; 
	UINT8		Payload[apscMaxASDU-3];
	 
	UINT8		PayLen;		// out of spec. by RadioPulse 
} ZCLS_PAY_G;		// ZCL General : Not including ManuCode 
 

//----------------------------------------------------------------------- 
//
//	ASDU Structure/Union
//
//-----------------------------------------------------------------------	 
typedef union 
{ 
	ZDPS_PAY	ZDP; 
	UINT8		Buff[apscMaxASDU]; 
	ZCLS_PAY_G	ZCL; 
	ZCLS_PAY_M	ZCL_MANU; 
} S_ASDU; 




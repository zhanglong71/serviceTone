/*******************************************************************************
	- Chip		: MG2450/2455
	- Vendor		: RadioPulse Inc, 2007.
	- Date		: 2008-01-10
	- Version		: VER 1.62

	[2008-01-10] VER 1.62
	- UpdateReport is added in MPIB and NIB

	[2007-12-03] VER 1.61
	- reserved MPIB is added in MAC_INFORMATION_BASE(rsv7C, rsv7D, rsv7E, rsv6X[])

	[2007-11-30] VER 1.60
	- MAC_INFORMATION_BASE : added new MPIB.secSecuritySuite
	- MAC_INFORMATION_BASE : added MPIB.secNonceFCnt
	- MAC_INFORMATION_BASE : added MPIB.secNonceKeySeq
	- MAC_INFORMATION_BASE : added MPIB.secNonceIEEE[8]
	- MAC_INFORMATION_BASE : added MPIB.secKey[16]

	[2007-11-07] VER 1.5
	- Initial Version
*******************************************************************************/


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


#define	HIB_ADDRESS		0x1000	// If HPIB is used, this value should not be modified.
#define	HIB_LENGTH			64
#define	INFO_ADDRESS		0x1200

typedef struct
{
	UINT8	IEEE_ADDR[8];		// IEEE Address
	UINT8	ChipID;				// 1=MG2400-F48, 2=MG2450/55
	UINT8	PowerID;			// 0x15=1.5V, 0x18=1.8V
	UINT8	ModemID;			// 0=250K, 1=500K, 2=1M
	UINT8	StackVersion;		// 0x10=ZigBee 2004, 0x12=2006, 0x13=2007
	UINT8	Channel;			// 0x0B~0x1A
	UINT8	PanID[2];			// PanID[0]=[7:0], PanID[1]=[15:8]
	UINT8	NwkAddr[2];			// NwkAddr[0]=[7:0], NwkAddr[1]=[15:8]
	UINT8	SecurityLevel;		// 0=No, 1=MIC32, 2=MIC64, 3=MIC128, 4=ENC, 5=ENCMIC32, 6=ENCMIC64, 7=ENCMIC128
	UINT8	PreConfigMode;		// 0x00 ~ 0xFF
	UINT8	NwkKey[16];			// Nwk Security Key
	UINT8	Reserved_0[8];		// reserved
	UINT8	EPID[8];				// Extended Pan ID	
	UINT8	Reserved_1[12];		// reserved
	UINT8	CSUM;				// SUM(IEEE_ADDR + Reserved_1) + CSUM = 0x00
	UINT8	Space[448];			// For fitting to 512Byte(1Page) size
} HW_INFORMATION;

typedef union
{
	UINT8	BUFF[512];
	HW_INFORMATION		DataHIB;
} FLASH_AREA;

#define	sSTA_SUCCESS			0x00
#define	sSTA_INV_PARAMETER	0x01
#define	sSTA_NO_MASTER_KEY	0x02
#define	sSTA_INV_CHALLENGE	0x03
#define	sSTA_INVALID_SKG		0x04
#define	sSTA_INVALID_MAC		0x05
#define	sSTA_INVALID_KEY		0x06
#define	sSTA_TIMEOUT			0x07
#define	sSTA_BAD_FRAME		0x08

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//		UART API Definition
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

#define MAX_API_PACKET      249


typedef struct _API_PACKET
{
    UINT8 layer;
    UINT8 Opcode;
    UINT8 Type;
    UINT8 Size;
    UINT8 Arguments[MAX_API_PACKET];
} API_PACKET;

typedef struct _API_SERIAL_PACKET
{
    UINT8 STX;      			/*	0xAA										*/
    UINT8 LEN;      			/* 	Size of PKT									*/
    API_PACKET PKT; 		/* 	PACKET BUFFER								*/
    UINT8 SUM;      			/* 	SUM = SUM(PKT)	, IF SUM == 0, SUM is Ignored	*/
    UINT8 ETX;      			/* 	0x55										*/
} API_SERIAL_PACKET;



	#define 	CFG_TOP_LAYER			0x00
	#define 	CFG_NWK_INFO			0x01
	#define 	CFG_SYS_VERSION		0x7F
	#define 	CFG_SYS_BACKUP		0x0F
	#define 	CFG_TOP_LAYER_GET		0x80
	#define 	CFG_NWK_INFO_GET		0x81

	#define	ZIGBEE_VERSION			0x02	// 0x30=ZigBee2006, 0x02=ZigBee2007

	#define 	cCOMMON_LENGTH_ERROR 	0xF0
	#define 	cCOMMON_CRC_ERROR      		0xF1

	//	Layer
	#define 	COMM_LAYER			0x01
	#define 	APS_LAYER				0x10
	#define 	NWK_LAYER				0x20
	#define 	MAC_LAYER				0x30
	#define 	PHY_LAYER				0x40
	#define 	OOB_LAYER				0x50
	#define 	APP_LAYER				0x80
	
	//	Type
	#define 	REQ_TYPE				0x10
	#define 	CON_TYPE				0x11
	#define 	IND_TYPE				0x12
	#define 	RSP_TYPE				0x13
	#define 	COMM_TYPE				0x01
	
	//	Common Layer
	#define 	COMM_OPCODE			0x01

	// 	MAC Layer	
	#define	M_RESET_OPC			0x01
	#define	M_START_OPC			0x02
	#define	M_SCAN_OPC			0x03
	#define	M_ASSO_OPC			0x04
	#define	M_DISA_OPC				0x05
	#define	M_GTS_OPC				0x06
	#define	M_SYNC_OPC			0x07
	#define	M_POLL_OPC				0x08
	#define	M_SET_OPC				0x09
	#define	M_GET_OPC				0x0A
	#define	M_RX_ENA_OPC			0x0B
	#define	M_DATA_OPC			0x0C
	#define	M_PURGE_OPC			0x0D
	#define	M_BEA_NOTI_OPC		0x0E
	#define	M_ORPH_OPC			0x0F
	#define	M_SYNC_LOSS_OPC		0x10
	#define	M_COMM_STA_OPC		0x11	

	//	NWK Layer
	#define 	N_DATA_OPC			0x10
	#define 	N_NET_DIS_OPC         	0x11
	#define 	N_NET_FOR_OPC          	0x12
	#define 	N_PERMIT_JOIN_OPC		0x13
	#define 	N_START_ROUTE_OPC	0x14
	#define 	N_JOIN_OPC				0x15
	#define 	N_DIRECT_JOIN_OPC		0x16
	#define 	N_LEAVE_OPC			0x17
	#define 	N_RESET_OPC			0x18
	#define 	N_SYNC_OPC				0x19
	#define	N_NWK_STATUS_OPC		0x1A
	#define	N_ED_SCAN_OPC			0x1B
	#define 	N_GET_OPC				0x20
	#define 	N_SET_OPC				0x21
	#define	N_ROUTE_DISC_OPC		0x30
	#define 	N_ROUTE_ERR_OPC		0x31
	#define	N_FORCE_JOIN_OPC		0x32
	#define	N_GET_TABLE_OPC		0x33
	#define	N_SET_TABLE_OPC		0x34
	#define 	N_DEBUG_OPC			0xD0

	//	APS Layer
	#define 	A_DATA_OPC 			0x10
	#define 	A_BIND_OPC				0x11
	#define 	A_UNBIND_OPC			0x12	
	#define 	A_GET_OPC				0x13
	#define 	A_SET_OPC				0x14
	#define 	A_ESTABLISH_KEY_OPC	0x15
	#define 	A_TRANSPORT_KEY_OPC	0x16
	#define 	A_UPDATE_DEVICE_OPC	0x17
	#define 	A_REMOVE_DEVICE_OPC	0x18
	#define 	A_REQUEST_KEY_OPC	0x19
	#define 	A_SWITCH_KEY_OPC		0x1A
	#define 	A_ADD_GRP_OPC			0x1B
	#define 	A_RM_GRP_OPC			0x1C
	#define	A_RM_ALL_GRP_OPC		0x1D	
	#define	A_SET_CONFIG_OPC		0x1E
	#define	A_REQUEST_IEEE_OPC	0x1F

	#define 	A_SKKE_OPC				0x20
	#define	A_SET_TABLE_OPC		0x30
	#define	A_GET_TABLE_OPC		0x31
	#define	A_ACK_IND_OPC			0x40
	#define	A_ACK_STA_OPC			0x41
	#define	A_NOT_DEFINED			0x80

	// 	APP Layer	
 	#define	APP_MGR_OPC			0x80
	#define	APP_EVT_OPC			0xE0


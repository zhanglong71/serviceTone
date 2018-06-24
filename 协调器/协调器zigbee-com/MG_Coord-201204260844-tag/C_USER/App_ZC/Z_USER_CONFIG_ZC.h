
/*******************************************************************************
	- Chip		: MG2450/2455
	- Vendor		: RadioPulse Inc, 2007.
	- Date		: 2010-11-15
	- Version		: VER 2.2

	[2010-11-15] VER 2.2
	- Definitions for ZDP are added.
		+ UserConfig_Enable_ZDP_LQIReq
		+ UserConfig_Enable_ZDP_RtgReq
		+ UserConfig_Enable_ZDP_LeaveReq
		+ UserConfig_Enable_ZDP_PermitJoinReq

	[2010-03-02] VER 2.10
	- For ZigBeePRO Development Kit V2.10
*******************************************************************************/

////////////////////////////////////////////////////////////////////////////////////
//	Type to Operate
////////////////////////////////////////////////////////////////////////////////////

	//-- DEVICE_OPERATING_MODE
	//	0	: ZigBee Coordinator. FFD library should be used.
	//	1	: ZigBee Router. FFD library should be used.
	//	2	: ZigBee End Device with Mains Power. FFD or RFD library can be used.
	//	3	: ZigBee End Device with Battery Power. FFD or RFD library can be used.
	#define	DEVICE_OPERATING_MODE		0
	
////////////////////////////////////////////////////////////////////////////////////
//	Memory Allocation for ZigBee Tables
////////////////////////////////////////////////////////////////////////////////////

	//----------------------------------------------------------------------------
	// Memory for RF Receiver Buffer
	//----------------------------------------------------------------------------
	#define	USER_SIZE_RF_RXQ		4		// 2 ~ 8
	
	//----------------------------------------------------------------------------
	// Memory for MAC Buffer to transmit to RX-OFF devices
	//----------------------------------------------------------------------------	
	#if( (DEVICE_OPERATING_MODE == 0) || (DEVICE_OPERATING_MODE == 1) )
		#define	USER_SIZE_Pend_TXQ	2	// 0x00 ~ 0xFE
	#else
		#define	USER_SIZE_Pend_TXQ	0	// If ZED, not needed.
	#endif

	//----------------------------------------------------------------------------
	// Memory for Security
	//----------------------------------------------------------------------------
	#define	USER_SIZE_SEC_NwkMaterial		1	// NWK Security Material. 0x00 ~ 0xFE
	#define	USER_SIZE_SEC_ApsKeyPair		0	// APS Device Key Pair Set. 0x00 ~ 0xFE

	//----------------------------------------------------------------------------
	// Memory for APS Layer
	//----------------------------------------------------------------------------
	#define	USER_SIZE_APS_BT		2		// Bind Table Entry Size. 0x00 ~ 0xFE
	#define	USER_SIZE_APS_GT		4		// Group Table Entry Size. 0x00 ~ 0xFE
	//----------------------------------------------------------------------------

	//----------------------------------------------------------------------------
	// Memory for NWK Layer
	//----------------------------------------------------------------------------
	#define	USER_SIZE_NWK_GID	4		// GroupID Entry Size. 0x00 ~ 0xFE
	#define	USER_SIZE_NWK_BTT	8		// Broadcast Transaction Table Entry Size. 0x00 ~ 0xFE		
	#define	USER_SIZE_NWK_NAM	32		// Address Map. 0x00 ~ 0xFE
	
	#if( (DEVICE_OPERATING_MODE == 0) || (DEVICE_OPERATING_MODE == 1) )	
		#define	USER_SIZE_NWK_NT		32		// Neighbor Table Entry Size. 0x04 ~ 0xFE
		#define	USER_SIZE_NWK_RT		32		// Route Table Entry Size. 0x02 ~ 0xFE
		#define	USER_SIZE_NWK_RDT	3		// Routing Discovery Table Entry Size. 0x01 ~ 0xFE
		#define	USER_SIZE_NWK_RRECT	2		// Route Record Table. 0x00 ~ 0xFE
	#else
		#define	USER_SIZE_NWK_NT		4		// If ZED, minimum NT entries are only needed.
		#define	USER_SIZE_NWK_RT		0		// If ZED, not needed.
		#define	USER_SIZE_NWK_RDT	0		// If ZED, not needed.
		#define	USER_SIZE_NWK_RRECT	0		// If ZED, not needed.
	#endif	
	//----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////////
//	ZDP Command Configuration
////////////////////////////////////////////////////////////////////////////////////
	
	#define	UserConfig_Enable_ZDP_NwkAddrReq		1		// Mandotary
	#define	UserConfig_Enable_ZDP_IEEEAddrReq		1		// Mandotary
	#define	UserConfig_Enable_ZDP_SimpDescReq		1		// Mandotary
	#define	UserConfig_Enable_ZDP_ActiveEPReq		1		// Mandotary
	#define	UserConfig_Enable_ZDP_NodeDescReq	1		// Mandotary
	#define	UserConfig_Enable_ZDP_PowerDescReq	0		// Mandotary
	#define	UserConfig_Enable_ZDP_MatchDescReq	0		// Mandotary
	#define	UserConfig_Enable_ZDP_BindReq			0		// Optional
	#define	UserConfig_Enable_ZDP_UserDescriptor	0		// Optional
	#define	UserConfig_Enable_ZDP_SysServerReq	0		// Optional
	#define	UserConfig_Enable_ZDP_CompDescReq	0		// Optional
	#define	UserConfig_Enable_ZDP_NwkUpdateReq	1		// Optional
	#define	UserConfig_Enable_ZDP_LQIReq			0		// Optional
	#define	UserConfig_Enable_ZDP_RtgReq			0		// Optional
	#define	UserConfig_Enable_ZDP_LeaveReq		0		// Optional
	#define	UserConfig_Enable_ZDP_PermitJoinReq	0		// Optional

////////////////////////////////////////////////////////////////////////////////////
//	OTA Configuration
////////////////////////////////////////////////////////////////////////////////////

	#define	UserConfig_Enable_OTA		1


////////////////////////////////////////////////////////////////////////////////////
//	ZDO Configuration
////////////////////////////////////////////////////////////////////////////////////

	//-- UserConfig_Capability 
	// bit[7]		: Allocate Addr
	// bit[6]		: Security 
	// bit[5:4]	: rsv 
	// bit[3]		: RxOnWhenIdle 
	// bit[2]		: Power Source. 1=Mains Power 
	// bit[1]		: Device Type. 1=FFD, 0=RFD 
	// bit[0]		: Alternative PAN Coordinator			

	#if (DEVICE_OPERATING_MODE == 0)
		#define	UserConfig_Capability	0x0E
		#define	UserConfig_DevType		0		// 0=ZC, 1=ZR, 2=ZED
	#elif (DEVICE_OPERATING_MODE == 1)
		#define	UserConfig_Capability	0x8E
		#define	UserConfig_DevType		1		// 0=ZC, 1=ZR, 2=ZED
	#elif (DEVICE_OPERATING_MODE == 2)
		#define	UserConfig_Capability	0x8C
		#define	UserConfig_DevType		2		// 0=ZC, 1=ZR, 2=ZED
	#elif (DEVICE_OPERATING_MODE == 3)
		#define	UserConfig_Capability	0x80
		#define	UserConfig_DevType		2		// 0=ZC, 1=ZR, 2=ZED
	#else
		"Wrong Definition"
	#endif	
	
	//-- UserConfig_ServerMask 
	// bit[15:7]	: reserved 
	// bit[6]		: 1=Support Network Manager
	// bit[5]		: 1=Support Backup Discovery Cache 
	// bit[4] 		: 1=Support Primary Discovery Cache 
	// bit[3]  	: 1=Support Backup Binding Table Cache 
	// bit[2]  	: 1=Support Primary Binding Table Cache 
	// bit[1]  	: 1=Support Backup Trust Center 
	// bit[0]  	: 1=Support Primary Trust Center	
	#if (DEVICE_OPERATING_MODE == 0)	
		#define	UserConfig_ServerMask	0x0044
	#else
		#define	UserConfig_ServerMask	0x0000
	#endif
	
	#define	UserConfig_ManufacturerCode	0x1551
	#define	UserConfig_BO					0x0F
	#define	UserConfig_SO					0x0F
	#define	UserConfig_PermitDuration		0xFF	// in seconds, 0=Not Permit, 0xFF=Permit always.

	#define	UserConfig_EPID7		0x00
	#define	UserConfig_EPID6		0x00
	#define	UserConfig_EPID5		0x00
	#define	UserConfig_EPID4		0x00
	#define	UserConfig_EPID3		0x00
	#define	UserConfig_EPID2		0x00
	#define	UserConfig_EPID1		0x00
	#define	UserConfig_EPID0		0x00

	#define	UserConfig_ProtocolVersion	0x02	// 0=Zig2004, 2=Zig2006 or Zig2007 or ZigBeePRO
	#define	UserConfig_StackProfile		0x02	// 1=ZigBee2007, 2=ZigBeePRO

////////////////////////////////////////////////////////////////////////////////////
//	Network Characteristics Configuration
////////////////////////////////////////////////////////////////////////////////////

	#define	UserConfig_MaxDepth			0x0F	// 1 ~ 15
	#define	UserConfig_MaxBroadcastRetry	1		// 3 in ZigBee Standard
	#define	UserConfig_InitRREQRetry		1		// 3 in ZigBee Standard
	#define	UserConfig_RREQREtry			1		// 2 in ZigBee Standard
	#define	UserConfig_NwkTxRetry			3		// 3 in ZigBee Standard

	#define	UserConfig_BroadDeliverTime	3000	// in milli-seconds. 9000 in ZigBee Standard
	#define	UserConfig_LinkStatusPeriod		15		// in seconds. 15 in ZigBee Standard
	#define	UserConfig_TransPersistTime		5000	// in milli-seconds
	#define	UserConfig_RouteDiscTime		3000	// in milli-seconds. 10000 in ZigBee Standard
	#define	UserConfig_WdtPeriod			10000	// in milli-seconds

	#define	UserConfig_JoinPriority			0		// 0=Depth(ZigBee Standard), 1=Link Quality(Out of Standard)
	#define	UserConfig_MaxLinkFail			3		// 0=Disable

	//-- UserConfig_MinValidLQI 
	// 255	:    0 dBm
	// 178	: -30 dBm
	// 153	: -40 dBm
	// 127	: -50 dBm
	// 102	: -60 dBm
	// 76	: -70 dBm
	// 51	: -80 dBm
	// 25	: -90 dBm
	// 0		: disabled
	#define	UserConfig_MinValidLQI		 0//	51		// 0 ~ 255
	
////////////////////////////////////////////////////////////////////////////////////
//	Profile For Application
////////////////////////////////////////////////////////////////////////////////////
	 
	#define	ProfileHA_DevSensorADC_EP_14		0x14	// defined by user 
	#define	ProfileHA_DevSensorADC_ID		0x1001	// defined by HA Profile 
			#define		ProfileHA_DevSensorADC_ClusCtrlADC		0x0410 
				#define		ProfileHA_DevSensorADC_ClusCtrlADC_AttOpeMode		0x0000 
					#define		ProfileHA_DevSensorADC_ClusCtrlADC_AttOpeMode_DTYPE	ZCL_DTYPE_UINT8 
			#define		ProfileHA_DevSensorADC_ClusCurrentADC		0x0411 
				#define		ProfileHA_DevSensorADC_ClusCurrentADC_AttRead		0x0000 
					#define		ProfileHA_DevSensorADC_ClusCurrentADC_AttRead_DTYPE	ZCL_DTYPE_UINT16 

	#define	ProfileHA_DevOnOffLight_EP_01		0x01	// defined by user 
	#define	ProfileHA_DevOnOffLight_ID		0x0100	// defined by HA Profile 
		#define		ProfileHA_DevOnOffLight_ClusOnOff		0x0006 
			#define		ProfileHA_DevOnOffLight_ClusOnOff_AttOnOff		0x0000 
				#define		ProfileHA_DevOnOffLight_ClusOnOff_AttOnOff_DTYPE	ZCL_DTYPE_Bool 
		#define		ProfileHA_DevOnOffLight_ClusScenes		0x0005 
			#define		ProfileHA_DevOnOffLight_ClusScenes_AttSceneCount		0x0000 
				#define		ProfileHA_DevOnOffLight_ClusScenes_AttSceneCount_DTYPE	ZCL_DTYPE_UINT8 
			#define		ProfileHA_DevOnOffLight_ClusScenes_AttCurScene		0x0001 
				#define		ProfileHA_DevOnOffLight_ClusScenes_AttCurScene_DTYPE	ZCL_DTYPE_UINT8 
			#define		ProfileHA_DevOnOffLight_ClusScenes_AttCurGroup		0x0002 
				#define		ProfileHA_DevOnOffLight_ClusScenes_AttCurGroup_DTYPE	ZCL_DTYPE_UINT16 
			#define		ProfileHA_DevOnOffLight_ClusScenes_AttSceneValid		0x0003 
				#define		ProfileHA_DevOnOffLight_ClusScenes_AttSceneValid_DTYPE	ZCL_DTYPE_Bool 
			#define		ProfileHA_DevOnOffLight_ClusScenes_AttNameSupport		0x0004 
				#define		ProfileHA_DevOnOffLight_ClusScenes_AttNameSupport_DTYPE	ZCL_DTYPE_8bit 

	#define	ProfileHA_DevSerialComm_EP_0A		0x0A	// defined by user 
	#define	ProfileHA_DevSerialComm_ID		0x1000	// defined by HA Profile 
			#define		ProfileHA_DevSerialComm_ClusComCtrl		0x0800 
				#define		ProfileHA_DevSerialComm_ClusComCtrl_AttLen		0x0000 
					#define		ProfileHA_DevSerialComm_ClusComCtrl_AttLen_DTYPE	ZCL_DTYPE_UINT8 
			#define		ProfileHA_DevSerialComm_ClusComStr		0x0801 
				#define		ProfileHA_DevSerialComm_ClusComStr_AttContent		0x0000 
					#define		ProfileHA_DevSerialComm_ClusComStr_AttContent_DTYPE	ZCL_DTYPE_StrChar 	
	
////////////////////////////////////////////////////////////////////////////////////
//	Application Option
////////////////////////////////////////////////////////////////////////////////////
	
	#define	AppOption_TIMER1_ms		10		// 10ms
	#define	AppOption_Enable_FlashFunc	0
	#define	AppOption_Enable_FlashRecall	0
	#define	AppOption_BoardVersion		0x12
	#define	AppOption_ProfileSimulator	0
	#define	AppOption_Enable_OOB		1 // Bryan
	#define	AppOption_JoinProcessFunc	0
	#define	AppOption_Orphan_AfterJoin	0
	
////////////////////////////////////////////////////////////////////////////////////
//	Security Mode Definition
////////////////////////////////////////////////////////////////////////////////////	

//#define	__SECURED_WITH_PRECONFIGURED_NWK_KEY__
#ifdef __SECURED_WITH_PRECONFIGURED_NWK_KEY__
	#define	PRECONFIG_NWK_KEY_15		0xAB
	#define	PRECONFIG_NWK_KEY_14		0xCD
	#define	PRECONFIG_NWK_KEY_13		0xEF
	#define	PRECONFIG_NWK_KEY_12		0x01
	#define	PRECONFIG_NWK_KEY_11		0x23
	#define	PRECONFIG_NWK_KEY_10		0x45
	#define	PRECONFIG_NWK_KEY_9		0x67
	#define	PRECONFIG_NWK_KEY_8		0x89
	#define	PRECONFIG_NWK_KEY_7		0x00
	#define	PRECONFIG_NWK_KEY_6		0x00
	#define	PRECONFIG_NWK_KEY_5		0x00
	#define	PRECONFIG_NWK_KEY_4		0x00
	#define	PRECONFIG_NWK_KEY_3		0x00
	#define	PRECONFIG_NWK_KEY_2		0x00
	#define	PRECONFIG_NWK_KEY_1		0x00
	#define	PRECONFIG_NWK_KEY_0		0x00
#endif

//#define	__SECURED_WITHOUT_PRECONFIGURED_NWK_KEY__
#ifdef __SECURED_WITHOUT_PRECONFIGURED_NWK_KEY__
	#define	TRUSTCENTER_NWK_KEY_15		0xAB
	#define	TRUSTCENTER_NWK_KEY_14		0xCD
	#define	TRUSTCENTER_NWK_KEY_13		0xEF
	#define	TRUSTCENTER_NWK_KEY_12		0x01
	#define	TRUSTCENTER_NWK_KEY_11		0x23
	#define	TRUSTCENTER_NWK_KEY_10		0x45
	#define	TRUSTCENTER_NWK_KEY_9		0x67
	#define	TRUSTCENTER_NWK_KEY_8		0x89
	#define	TRUSTCENTER_NWK_KEY_7		0x00
	#define	TRUSTCENTER_NWK_KEY_6		0x00
	#define	TRUSTCENTER_NWK_KEY_5		0x00
	#define	TRUSTCENTER_NWK_KEY_4		0x00
	#define	TRUSTCENTER_NWK_KEY_3		0x00
	#define	TRUSTCENTER_NWK_KEY_2		0x00
	#define	TRUSTCENTER_NWK_KEY_1		0x00
	#define	TRUSTCENTER_NWK_KEY_0		0x00
#endif


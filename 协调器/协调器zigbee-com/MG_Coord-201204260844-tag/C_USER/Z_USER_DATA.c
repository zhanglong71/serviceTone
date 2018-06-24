
/*******************************************************************************
	- Chip		: MG2450/2455
	- Vendor		: RadioPulse Inc, 2007.
	- Date		: 2010-11-15
	- Version		: VER 2.2
	
	[2010-11-15] VER 2.2
	- If SIB.PreconfigMode is not equal to 0, some steps are skipped in ZSYS_JOIN_PROCESS().

	[2010-03-02] VER 2.10
	- For ZigBeePRO Development Kit V2.10
*******************************************************************************/

	//----------------------------------------------------------------------------
	// Memory for RF Receiver Buffer
	//----------------------------------------------------------------------------
	#if (USER_SIZE_RF_RXQ)
		MAC_PKT		MEMORY_RF_RXQ[USER_SIZE_RF_RXQ];
	#endif

	//----------------------------------------------------------------------------
	// Memory for MAC Buffer to transmit to RX-OFF devices
	//----------------------------------------------------------------------------
	#if (USER_SIZE_Pend_TXQ)
		MS_PendingPacket	MEMORY_Pend_TXQ[USER_SIZE_Pend_TXQ];
	#endif

	//----------------------------------------------------------------------------
	// Memory for Security
	//----------------------------------------------------------------------------
	#if (USER_SIZE_SEC_NwkMaterial)
		SS_NwkSecMaterial	MEMORY_SEC_NwkMaterial[USER_SIZE_SEC_NwkMaterial];
	#endif

	#if (USER_SIZE_SEC_ApsKeyPair)
		SS_ApsDevKeyPair		MEMORY_SEC_ApsKeyPair[USER_SIZE_SEC_ApsKeyPair];
	#endif


	//----------------------------------------------------------------------------
	// Memory for APS Layer
	//----------------------------------------------------------------------------
	#if (USER_SIZE_APS_BT)	
		AS_BIND_TABLE	MEMORY_BT[USER_SIZE_APS_BT];	
	#endif

	#if (USER_SIZE_APS_GT)		
		AS_GROUP_TABLE	MEMORY_GT[USER_SIZE_APS_GT];
	#endif
	//----------------------------------------------------------------------------


	//----------------------------------------------------------------------------
	// Memory for NWK Layer
	//----------------------------------------------------------------------------
	#if (USER_SIZE_NWK_NT)	
		NS_NT		MEMORY_NT[USER_SIZE_NWK_NT];
	#endif

	#if (USER_SIZE_NWK_RT)	
		NS_RT		MEMORY_RT[USER_SIZE_NWK_RT];
	#endif

	#if (USER_SIZE_NWK_GID)	
		NS_GID		MEMORY_GID[USER_SIZE_NWK_GID];
	#endif

	#if (USER_SIZE_NWK_RDT)	
		NS_RDT		MEMORY_RDT[USER_SIZE_NWK_RDT];
	#endif

	#if (USER_SIZE_NWK_BTT)	
		NS_BTT		MEMORY_BTT[USER_SIZE_NWK_BTT];	
	#endif

	#if (USER_SIZE_NWK_RRECT)
		NS_RRECT	MEMORY_RRECT[USER_SIZE_NWK_RRECT];
	#endif

	#if (USER_SIZE_NWK_NAM)
		NS_NAM		MEMORY_NAM[USER_SIZE_NWK_NAM];
	#endif

/***********************************************************************************
 *
 *	NAME		: ZSYS_ALLOCATE_MEMORY()
 *
 *	DESCRIPTION	: Callback function called by the stack when ZNWK_RESET_REQ() is called.
 *		The routines is as follows.
 *			ZNWK_RESET_REQ()                                                                                                                                                                                                               
 *			{
 *				// Initializing NWK Layer
 *				ZSYS_ALLOCATE_MEMORY();
 * 				ZNWK_RESET_NIB();
 *			} 			
 *		In this function,  data memory for tables, buffers and Queue of the stack is assigned
 *		by configurations.(e.g. USER_SIZE_RF_RXQ, ...)
 *
 * 	PARAMETER	: None
 *
 * 	RETURN		: None
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
void ZSYS_ALLOCATE_MEMORY()
{
	UINT8	i;

	//----------------------------------------------------------------------------
	// Pointer to RF Receiver Buffer Memory
	//----------------------------------------------------------------------------
	#if (USER_SIZE_RF_RXQ)
		MPIB.pRXQ = MEMORY_RF_RXQ;	
		MPIB.SIZE_RXQ = USER_SIZE_RF_RXQ;
	#endif

	//----------------------------------------------------------------------------
	// Pointer to MAC Buffer Memory
	//----------------------------------------------------------------------------
	#if (USER_SIZE_Pend_TXQ)
		MPIB.pPendTxQ = MEMORY_Pend_TXQ;
		MPIB.SIZE_PendTxQ = USER_SIZE_Pend_TXQ;
		for(i=0 ; i<USER_SIZE_Pend_TXQ ; i++)
		{
			MPIB.pPendTxQ[i].EnaPP = 0;
		}
	#endif

	//----------------------------------------------------------------------------
	// Pointer to Security Memory
	//----------------------------------------------------------------------------
	#if (USER_SIZE_SEC_ApsKeyPair)
		SIB.papsKeyPairSet = MEMORY_SEC_ApsKeyPair;	
		SIB.SIZE_ApsKeyPair = USER_SIZE_SEC_ApsKeyPair;
		for(i=0 ; i<USER_SIZE_SEC_ApsKeyPair ; i++)		SIB.papsKeyPairSet[i].EnaKP = 0;
	#endif	

	#if (USER_SIZE_SEC_NwkMaterial)	
		SIB.pnwkSecMaterial = MEMORY_SEC_NwkMaterial;
		SIB.SIZE_NwkMaterial = USER_SIZE_SEC_NwkMaterial;
		for(i=0 ; i<USER_SIZE_SEC_NwkMaterial ; i++)	SIB.pnwkSecMaterial[i].EnaSM = 0;
	#endif

	//---------------------------------------------------------------
	// Pointer to APS Memory
	//---------------------------------------------------------------
	#if (USER_SIZE_APS_BT)
		// Initialize Binding Table(BT)
		AIB.papsBindTable = MEMORY_BT;
		AIB.SIZE_BT = USER_SIZE_APS_BT;
		for(i=0 ; i<USER_SIZE_APS_BT ; i++)
		{
			AIB.papsBindTable[i].EnaBT = 0;
		}	
	#endif

	#if (	USER_SIZE_APS_GT)
		// Initialize Group Table(GT)
		AIB.papsGroupTable = MEMORY_GT;
		AIB.SIZE_GT = USER_SIZE_APS_GT;
		for(i=0 ; i<USER_SIZE_APS_GT ; i++)
		{
			AIB.papsGroupTable[i].EnaGT = 0;
		}
	#endif

	//---------------------------------------------------------------
	// Pointer to NWK Memory
	//---------------------------------------------------------------	

	#if (USER_SIZE_NWK_NT)	
		// Initialize Neighbor Table(NT)
		NIB.pNT = MEMORY_NT;
		NIB.SIZE_NT = USER_SIZE_NWK_NT;;
		for(i=0 ; i<USER_SIZE_NWK_NT ; i++)	NIB.pNT[i].EnaNT = 0;
	#endif

	#if (USER_SIZE_NWK_RT)
		// Initialize Routing Table(RT)
		NIB.pRT = MEMORY_RT;
		NIB.SIZE_RT = USER_SIZE_NWK_RT;
		for(i=0 ; i<USER_SIZE_NWK_RT ; i++)	NIB.pRT[i].EnaRT = 0;	
	#endif

	#if (USER_SIZE_NWK_GID)	
		// Initialize GroupID Table(GID)
		NIB.pGID = MEMORY_GID;
		NIB.SIZE_GID = USER_SIZE_NWK_GID;
		for(i=0 ; i<USER_SIZE_NWK_GID ; i++)	NIB.pGID[i].EnaGID = 0;
	#endif

	#if (USER_SIZE_NWK_RDT)	
		// Initialize Routing Discovery Table(RDT)
		NIB.pRDT = MEMORY_RDT;		
		NIB.SIZE_RDT = USER_SIZE_NWK_RDT;
		for(i=0 ; i<USER_SIZE_NWK_RDT ; i++)	NIB.pRDT[i].RdtExpTime = 0;
	#endif

	#if (USER_SIZE_NWK_BTT)	
		// Initialize Broadcast Table(BTT)	
		NIB.pBTT = MEMORY_BTT;
		NIB.SIZE_BTT = USER_SIZE_NWK_BTT;
		for(i=0 ; i<USER_SIZE_NWK_BTT ; i++)	NIB.pBTT[i].BttExpTime = 0;
	#endif

	#if (USER_SIZE_NWK_RRECT)
		// Initialize Route Record Table
		NIB.pRRECT = MEMORY_RRECT;
		NIB.SIZE_RRECT = USER_SIZE_NWK_RRECT;
		for(i=0 ; i<USER_SIZE_NWK_RRECT ; i++)	NIB.pRRECT[i].EnaRRECT = 0;
	#endif

	#if (USER_SIZE_NWK_NAM)
		// Initialize NWK Address Map
		NIB.pNAM = MEMORY_NAM;
		NIB.SIZE_NAM = USER_SIZE_NWK_NAM;
		for(i=0 ; i<USER_SIZE_NWK_NAM ; i++)		NIB.pNAM[i].EnaNAM = 0;
	#endif
	
}

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#if (AppOption_Enable_FlashFunc)

/***********************************************************************************
 *
 *	NAME		: ZSYS_READ_FLASH()
 *
 *	DESCRIPTION	: Reads FLASH(code memory) data and copies it to data memory
 *
 * 	PARAMETER	: 
 *		FlashCodeAddr - address of code memory to read
 *		pDstBuf - pointer to data memory to which code data is copied
 *
 * 	RETURN		: None
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
void ZSYS_READ_FLASH(UINT16 FlashCodeAddr, UINT8 *pDstBuf)
{
	UINT16	i;
	UINT8	code *pCodeBuf;

	pCodeBuf = FlashCodeAddr;
	for(i=0 ; i<512 ; i++)	pDstBuf[i] = pCodeBuf[i];
}

/***********************************************************************************
 *
 *	NAME		: ZSYS_WRITE_FLASH()
 *
 *	DESCRIPTION	: Writes data memory into FLASH(code memory)
 *
 * 	PARAMETER	: 
 *		FlashCodeAddr - address of code memory to write
 *		pSrctBuf - pointer to data memory that is written into code memory
 *
 * 	RETURN		: None
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
void ZSYS_WRITE_FLASH(UINT16 FlashCodeAddr, UINT8 *pSrcBuf)
{
	UINT16	i;
	UINT8	SUM;

	if(FlashCodeAddr == HIB_ADDRESS)
	{	
		SUM = 0;
		for(i=0 ; i<(HIB_LENGTH-1) ; i++)
		{
			SUM += pSrcBuf[i];
		}

		pSrcBuf[HIB_LENGTH-1] = 0x00 - SUM;
	}

	ZHAL_FLASH_WRITE_UNPROTECTED();
	ZHAL_FLASH_PAGE_WRITE_EXTEND(0, FlashCodeAddr, pSrcBuf);
	ZHAL_FLASH_WRITE_PROTECTED();
	
}

/***********************************************************************************
 *
 *	NAME		: ZSYS_DATA_BACKUP()
 *
 *	DESCRIPTION	: Writes some information base which indicates characteristics of the device
 *		into FLASH(code memory)
 *		Additionally, application-specific variables can be written into FLASH using reserved fields.
 *		But, the total length of written data can not be more than 512 bytes.
 *
 * 	PARAMETER	: None
 *
 * 	RETURN		: None
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
void ZSYS_DATA_BACKUP()
{
	//----------------------------------------------------------------
	//	HIB Area
	//----------------------------------------------------------------	
	DATA_COPY.DataHIB.Channel = PPIB.phyCurrentChannel;
	rpmemcpy(DATA_COPY.DataHIB.EPID, NIB.nwkEPID, 8);
	rpmemcpy(DATA_COPY.DataHIB.IEEE_ADDR, MPIB.IEEE_ExtendAddr, 8);
	DATA_COPY.DataHIB.NwkAddr[0] = NIB.nwkNetworkAddr;
	DATA_COPY.DataHIB.NwkAddr[1] = NIB.nwkNetworkAddr >> 8;
	DATA_COPY.DataHIB.PanID[0] = NIB.nwkPanID;
	DATA_COPY.DataHIB.PanID[1] = NIB.nwkPanID >> 8;
	//----------------------------------------------------------------

	//----------------------------------------------------------------
	//	Address Map of DATA_COPY.DataHIB.Space[448]	
	//----------------------------------------------------------------

	//----------------------------------------------------------------
	// 0x00 ~ 0x0F	: reserved. User specific
	//----------------------------------------------------------------

	//----------------------------------------------------------------
	// 0x10 ~ 0x1F	: NWK Layer Info
	//----------------------------------------------------------------
	DATA_COPY.DataHIB.Space[0x10] = NIB.DEVTYPE;
	DATA_COPY.DataHIB.Space[0x11] = NIB.nwkCapaInfo;
	DATA_COPY.DataHIB.Space[0x12] = NIB.nwkUpdateId;
	// 0x13 ~ 0x1F : reserved
	//----------------------------------------------------------------

	//----------------------------------------------------------------
	// 0x20 ~ 0x4F	: MAC & PHY
	//----------------------------------------------------------------
	DATA_COPY.DataHIB.Space[0x20] = PPIB.phyTransmitPower;
	DATA_COPY.DataHIB.Space[0x21] = MPIB.macAssociationPermit;
	DATA_COPY.DataHIB.Space[0x22] = MPIB.macBeaconOrder;
	DATA_COPY.DataHIB.Space[0x23] = MPIB.macSuperframeOrder;
	DATA_COPY.DataHIB.Space[0x24] = MPIB.macRxOnWhenIdle;
	DATA_COPY.DataHIB.Space[0x25] = MPIB.macPANId;
	DATA_COPY.DataHIB.Space[0x26] = MPIB.macPANId >> 8;
	DATA_COPY.DataHIB.Space[0x27] = MPIB.macShortAddr;
	DATA_COPY.DataHIB.Space[0x28] = MPIB.macShortAddr >> 8;
	DATA_COPY.DataHIB.Space[0x29] = MPIB.EnableFFD;
	DATA_COPY.DataHIB.Space[0x2A] = MPIB.EnablePanCoordi;
	DATA_COPY.DataHIB.Space[0x2B] = MPIB.macCoordShortAddr;
	DATA_COPY.DataHIB.Space[0x2C] = MPIB.macCoordShortAddr >> 8;
	// 0x2D ~ 0x2E	: reserved
	DATA_COPY.DataHIB.Space[0x2F] = MPIB.macBeaconPayloadLength;
	rpmemcpy(&DATA_COPY.DataHIB.Space[0x30], MPIB.macBeaconPayload, 16);	// 0x30 ~ 0x3F
	rpmemcpy(&DATA_COPY.DataHIB.Space[0x40], MPIB.macCoordExtendAddr, 8);	// 0x40 ~ 0x47
	// 0x48 ~ 0x4F	: reserved
	//----------------------------------------------------------------

	//----------------------------------------------------------------
	// 0x50 ~ 0x5F	: APS
	//----------------------------------------------------------------
	rpmemcpy(&DATA_COPY.DataHIB.Space[0x50], (UINT8 *)&AIB.apsChannelMask, 4);		// 0x50 ~ 0x53
	DATA_COPY.DataHIB.Space[0x54] = AIB.apsDesignCoord;
	DATA_COPY.DataHIB.Space[0x55] = AIB.apsUseInsecureJoin;
	// 0x56 ~ 0x57	: reserved
	rpmemcpy(&DATA_COPY.DataHIB.Space[0x58], AIB.apsUseExtPanID, 8);			// 0x58 ~ 0x5F
	//----------------------------------------------------------------

	//----------------------------------------------------------------
	// 0x60 ~ 0x7F	: Security
	//----------------------------------------------------------------
	DATA_COPY.DataHIB.Space[0x60] = SIB.nwkSecureAllFrame;
	DATA_COPY.DataHIB.Space[0x61] = SIB.PreconfigMode;
	DATA_COPY.DataHIB.Space[0x62] = SIB.nwkSecurityLevel;	
	// 0x63 ~ 0x6C	: reserved	
	#if (USER_SIZE_SEC_NwkMaterial)
	DATA_COPY.DataHIB.Space[0x6D] = SIB.pnwkSecMaterial[0].EnaSM;
	DATA_COPY.DataHIB.Space[0x6E] = SIB.pnwkSecMaterial[0].KeySeqNum;
	DATA_COPY.DataHIB.Space[0x6F] = SIB.pnwkSecMaterial[0].KeyType;
	rpmemcpy(&DATA_COPY.DataHIB.Space[0x70], SIB.pnwkSecMaterial[0].Key, 16);	// 0x70 ~ 0x7F
	#endif
	//----------------------------------------------------------------

	//----------------------------------------------------------------
	// 0x80 ~ 0x01C0 	: Reserved
	//----------------------------------------------------------------
	// A user can add additional data to be saved in flash using DATA_COPY.DataHIB.Space[128~447]
	//
	// (ex) 
	//	DATA_COPY.DataHIB.Space[128] = UserDefineSaveData[0]
	//   ....
	//  	DATA_COPY.DataHIB.Space[447] = UserDefineSaveData[319]
	//----------------------------------------------------------------

	ZSYS_WRITE_FLASH(HIB_ADDRESS, (UINT8 *)&DATA_COPY);
}

/***********************************************************************************
 *
 *	NAME		: ZSYS_DATA_RECALL()
 *
 *	DESCRIPTION	: Reads the preserved FLASH(code memory) and writes it to corresponding 
 *		information base. 
 *
 * 	PARAMETER	: None
 *
 * 	RETURN		: None
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
void ZSYS_DATA_RECALL()
{
	ZSYS_READ_FLASH(HIB_ADDRESS, (UINT8 *)&DATA_COPY);

	//----------------------------------------------------------------
	//	HIB Area
	//----------------------------------------------------------------
	PPIB.phyCurrentChannel = DATA_COPY.DataHIB.Channel;	
	rpmemcpy(NIB.nwkEPID, DATA_COPY.DataHIB.EPID, 8);
	rpmemcpy(MPIB.IEEE_ExtendAddr, DATA_COPY.DataHIB.IEEE_ADDR, 8);
	NIB.nwkNetworkAddr = (DATA_COPY.DataHIB.NwkAddr[1] << 8) | DATA_COPY.DataHIB.NwkAddr[0];
	NIB.nwkPanID = (DATA_COPY.DataHIB.PanID[1] << 8) | DATA_COPY.DataHIB.PanID[0];

	//----------------------------------------------------------------
	//	Address Map of DATA_COPY.DataHIB.Space[448]	
	//----------------------------------------------------------------

	//----------------------------------------------------------------
	// 0x00 ~ 0x0F	: reserved. User specific
	//----------------------------------------------------------------

	//----------------------------------------------------------------
	// 0x10 ~ 0x1F	: NWK Layer Info
	//----------------------------------------------------------------
	NIB.DEVTYPE = DATA_COPY.DataHIB.Space[0x10];
	NIB.nwkCapaInfo = DATA_COPY.DataHIB.Space[0x11];
	NIB.nwkUpdateId = DATA_COPY.DataHIB.Space[0x12];	
	// 0x13 ~ 0x1F : reserved
	//----------------------------------------------------------------

	//----------------------------------------------------------------
	// 0x20 ~ 0x4F	: MAC & PHY
	//----------------------------------------------------------------
	PPIB.phyTransmitPower = DATA_COPY.DataHIB.Space[0x20];
	MPIB.macAssociationPermit = DATA_COPY.DataHIB.Space[0x21];
	MPIB.macBeaconOrder = DATA_COPY.DataHIB.Space[0x22];
	MPIB.macSuperframeOrder = DATA_COPY.DataHIB.Space[0x23];
	MPIB.macRxOnWhenIdle = DATA_COPY.DataHIB.Space[0x24];
	MPIB.macPANId = (DATA_COPY.DataHIB.Space[0x26] << 8) | DATA_COPY.DataHIB.Space[0x25];
	MPIB.macShortAddr = (DATA_COPY.DataHIB.Space[0x28] << 8) | DATA_COPY.DataHIB.Space[0x27];
	MPIB.EnableFFD = DATA_COPY.DataHIB.Space[0x29];
	MPIB.EnablePanCoordi = DATA_COPY.DataHIB.Space[0x2A];
	MPIB.macCoordShortAddr = (DATA_COPY.DataHIB.Space[0x2C] << 8) | DATA_COPY.DataHIB.Space[0x2B];	
	// 0x2D ~ 0x2E	: reserved
	MPIB.macBeaconPayloadLength = DATA_COPY.DataHIB.Space[0x2F];
	rpmemcpy(MPIB.macBeaconPayload, &DATA_COPY.DataHIB.Space[0x30], 16);	// 0x30 ~ 0x3F
	rpmemcpy(MPIB.macCoordExtendAddr, &DATA_COPY.DataHIB.Space[0x40], 8);	// 0x40 ~ 0x47	
	// 0x48 ~ 0x4F	: reserved
	//----------------------------------------------------------------

	//----------------------------------------------------------------
	// 0x50 ~ 0x5F	: APS
	//----------------------------------------------------------------
	rpmemcpy( (UINT8 *)&AIB.apsChannelMask, &DATA_COPY.DataHIB.Space[0x50], 4);		// 0x50 ~ 0x53
	AIB.apsDesignCoord = DATA_COPY.DataHIB.Space[0x54];	
	AIB.apsUseInsecureJoin = DATA_COPY.DataHIB.Space[0x55];	
	// 0x56 ~ 0x57	: reserved
	rpmemcpy(AIB.apsUseExtPanID, &DATA_COPY.DataHIB.Space[0x58], 8);			// 0x58 ~ 0x5F
	//----------------------------------------------------------------

	//----------------------------------------------------------------
	// 0x60 ~ 0x7F	: Security
	//----------------------------------------------------------------
	SIB.nwkSecureAllFrame = DATA_COPY.DataHIB.Space[0x60];
	SIB.PreconfigMode = DATA_COPY.DataHIB.Space[0x61];
	SIB.nwkSecurityLevel = DATA_COPY.DataHIB.Space[0x62];
	// 0x63 ~ 0x6C	: reserved	
	#if (USER_SIZE_SEC_NwkMaterial)
	SIB.pnwkSecMaterial[0].EnaSM = DATA_COPY.DataHIB.Space[0x6D];
	SIB.pnwkSecMaterial[0].KeySeqNum = DATA_COPY.DataHIB.Space[0x6E];
	SIB.pnwkSecMaterial[0].KeyType = DATA_COPY.DataHIB.Space[0x6F];
	rpmemcpy(SIB.pnwkSecMaterial[0].Key, &DATA_COPY.DataHIB.Space[0x70], 16);	// 0x70 ~ 0x7F
	#endif	
	//----------------------------------------------------------------

	//----------------------------------------------------------------
	// 0x80 ~ 0x01C0 	: Reserved
	//----------------------------------------------------------------

	//----------------------------------------------------------------
	// Network Initializing
	//----------------------------------------------------------------
	if(MPIB.EnablePanCoordi)	ZHAL_MAC_CTRL_SET(0x1E);
	ZHAL_CHANNEL_SET(PPIB.phyCurrentChannel);
	ZHAL_TXPOWER_SET(PPIB.phyTransmitPower);
	ZHAL_PAN_ID_SET(MPIB.macPANId);	
	ZHAL_SHORT_ADDR_SET(MPIB.macShortAddr);
	ZHAL_IEEE_ADDR_SET(MPIB.IEEE_ExtendAddr);
	//----------------------------------------------------------------
}

/***********************************************************************************
 *
 *	NAME		: ZSYS_DATA_DELETE()
 *
 *	DESCRIPTION	: Writes 0xFF to the preserved FLASH(code memory) except HIB area. 
 *		It causes to delete information base.
 *
 * 	PARAMETER	: None
 *
 * 	RETURN		: None
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
void ZSYS_DATA_DELETE()
{
	//----------------------------------------------------------------
	//	HIB Area
	//----------------------------------------------------------------	
	DATA_COPY.DataHIB.Channel = PPIB.phyCurrentChannel;
	rpmemcpy(DATA_COPY.DataHIB.EPID, NIB.nwkEPID, 8);
	rpmemcpy(DATA_COPY.DataHIB.IEEE_ADDR, MPIB.IEEE_ExtendAddr, 8);
	DATA_COPY.DataHIB.NwkAddr[0] = NIB.nwkNetworkAddr;
	DATA_COPY.DataHIB.NwkAddr[1] = NIB.nwkNetworkAddr >> 8;
	DATA_COPY.DataHIB.PanID[0] = NIB.nwkPanID;
	DATA_COPY.DataHIB.PanID[1] = NIB.nwkPanID >> 8;

	memset(&DATA_COPY.DataHIB.Space[0], 0xFF, 448);	// Delete "Space" Area
	ZSYS_WRITE_FLASH(HIB_ADDRESS, (UINT8 *)&DATA_COPY);
}
#endif

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

/***********************************************************************************
 *
 *	NAME		: ZSYS_SEND_DEVICE_ANNOUNCE()
 *
 *	DESCRIPTION	: Transmits a ZDP Device_annce packet.
 *
 * 	PARAMETER	: None
 *
 * 	RETURN		: Status of ZAPS_DATA_REQ()
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
UINT8 ZSYS_SEND_DEVICE_ANNOUNCE()
{
	// DEVICE-ANNOUNCE
	APP_ASDU.Buff[0] = ++APP_DSN;
	APP_ASDU.Buff[1] = NIB.nwkNetworkAddr;
	APP_ASDU.Buff[2] = NIB.nwkNetworkAddr >> 8;
	rpmemcpy(&APP_ASDU.Buff[3], MPIB.IEEE_ExtendAddr, 8);
	APP_ASDU.Buff[11] = NIB.nwkCapaInfo;

	APP2APS.DATA_REQ.asduLen = 12;
	APP2APS.DATA_REQ.pasdu = APP_ASDU.Buff;
	APP2APS.DATA_REQ.ClusID = aCID_Device_Announce;
	APP2APS.DATA_REQ.DstAdd_U.Short = nBROAD_RXON;
	APP2APS.DATA_REQ.DstEP = EndpointZDO;
	APP2APS.DATA_REQ.DstMode = 0x02;
	APP2APS.DATA_REQ.ProfileID = ProfileZDP;
	APP2APS.DATA_REQ.Radius = NIB.nwkMaxDepth << 1;
	APP2APS.DATA_REQ.SrcEP = EndpointZDO;
	APP2APS.DATA_REQ.TxOption = 0;

	ZAPS_DATA_REQ(&APP2APS.DATA_REQ, &APS2APP.DATA_CON);
	return	APS2APP.DATA_CON.Sta;
}

/***********************************************************************************
 *
 *	NAME		: ZSYS_JOIN_PROCESS()
 *
 *	DESCRIPTION	: Transmits a ZDP Device_annce packet.
 *
 * 	PARAMETER	: ChanList - bitmap of operating RF channels
 *			- bit[31:27] : reserved
 *			- bit[26] : 1=Ch26 is enabled
 *			   ......
 *			- bit[26] : 1=Ch11 is enabled
 *			- bit[10:0] : reserved
 *
 * 	RETURN		: Stauts of joining. 1=Successful. 0=Failed.
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
 #if (AppOption_JoinProcessFunc)
UINT8 ZSYS_JOIN_PROCESS(UINT32 ChanList)
{
	UINT8	ib;
	UINT8	MatchEPID;
	UINT8	MatchCapa;
	UINT8	Status;
	
	UINT8	ParentChannel;
	UINT8	ParentEPID[8];

	// 1. Network Discovering
	zPrintf(1, "\n [USER] Discovery");
	APS2NWK.NET_DISC_REQ.ScanChan = ChanList;
	APS2NWK.NET_DISC_REQ.ScanDur = 0x04;
	ZNWK_NET_DISC_REQ(&APS2NWK.NET_DISC_REQ, &NWK2APS.NET_DISC_CON);
	zPrintf(1, " => STA=%02x",(short)NWK2APS.NET_DISC_CON.Sta);
	zPrintf(1, " => NwkCount=%02x",(short)NWK2APS.NET_DISC_CON.NwkCount);	

	if( (NWK2APS.NET_DISC_CON.Sta != nSTA_SUCCESS) 
	|| (NWK2APS.NET_DISC_CON.NwkCount == 0) )
	{
		zPrintf(1, "\n Join Error : Discovery is failed");
		return 0;
	}

	// 2. Display Discovered Network Information	
	for(ib=0 ; ib<NWK2APS.NET_DISC_CON.NwkCount ; ib++)
	{
		zPrintf(1, "\n NwkNum=0x%02x",(short)ib);
		zPrintf(1, " :CH=0x%02x",(short)NWK2APS.NET_DISC_CON.pNwkDesc[ib].Chan);
		zPrintf(1, " :Permit=%x",(short)NWK2APS.NET_DISC_CON.pNwkDesc[ib].PermitJoin);
		zPrintf(1, " :RoutreCapa=%x",(short)NWK2APS.NET_DISC_CON.pNwkDesc[ib].RouterCapa);
		zPrintf(1, " :EndCapa=%x",(short)NWK2APS.NET_DISC_CON.pNwkDesc[ib].EndDevCapa);
		zPrintf(1, " :Stack=%x",(short)NWK2APS.NET_DISC_CON.pNwkDesc[ib].Stack);
		zPrintf(1, " :Version=%x",(short)NWK2APS.NET_DISC_CON.pNwkDesc[ib].Version);
		zPrintf(1, " :EPID(0~7)=");
		Display_Buffer(NWK2APS.NET_DISC_CON.pNwkDesc[ib].EPID, 8, 0);
	}	

	// 3. Select the Network to Join
	ParentChannel = 0;
	for(ib=0 ; ib<NWK2APS.NET_DISC_CON.NwkCount ; ib++)
	{		
		// Compare EPID
		if(Check_8Byte_Null(NIB.nwkEPID))		MatchEPID = 1;
		else
		{
			if(memcmp(NIB.nwkEPID, NWK2APS.NET_DISC_CON.pNwkDesc[ib].EPID, 8))
					MatchEPID = 0;
			else		MatchEPID = 1;
		}

		// Compare RouterCapa/EndDevCapa
		#if (UserConfig_DevType == 0x01)	// ZR
			MatchCapa = NWK2APS.NET_DISC_CON.pNwkDesc[ib].RouterCapa;
		#else		// ZED
			MatchCapa = NWK2APS.NET_DISC_CON.pNwkDesc[ib].EndDevCapa;
		#endif

		if( 	(MatchEPID) && (MatchCapa)
		&&	(NWK2APS.NET_DISC_CON.pNwkDesc[ib].PermitJoin)
		&&	(NWK2APS.NET_DISC_CON.pNwkDesc[ib].Stack == UserConfig_StackProfile)
		&&	(NWK2APS.NET_DISC_CON.pNwkDesc[ib].Version == UserConfig_ProtocolVersion)
		&&	(NWK2APS.NET_DISC_CON.pNwkDesc[ib].BO == UserConfig_BO)
		&&	(NWK2APS.NET_DISC_CON.pNwkDesc[ib].SO == UserConfig_SO) )
		{
			ParentChannel = NWK2APS.NET_DISC_CON.pNwkDesc[ib].Chan;
			rpmemcpy(ParentEPID, NWK2APS.NET_DISC_CON.pNwkDesc[ib].EPID, 8);
			break;
		}	
	}
	if(ParentChannel == 0)
	{
		zPrintf(1, "\n Join Error : No matched Network");
		return 0;
	}

	// 4. Joining by Association	
	zPrintf(1, "\n [USER] Joining");
	APS2NWK.JOIN_REQ.CapaInfo = UserConfig_Capability;
	rpmemcpy(APS2NWK.JOIN_REQ.EPID, ParentEPID, 8);
	APS2NWK.JOIN_REQ.Rejoin = 0;		// Association
	APS2NWK.JOIN_REQ.ScanChan = 1;
	APS2NWK.JOIN_REQ.ScanChan <<= ParentChannel;
	APS2NWK.JOIN_REQ.ScanDur = 0x04;
	APS2NWK.JOIN_REQ.SecureEna = 0;
	ZNWK_JOIN_REQ(&APS2NWK.JOIN_REQ, &NWK2APS.JOIN_CON);
	zPrintf(1, " => STA=%02x",(short)NWK2APS.JOIN_CON.Sta);

	if(NWK2APS.JOIN_CON.Sta == nSTA_SUCCESS)
	{
		zPrintf(1, " =>CH=0x%02x",(short)NWK2APS.JOIN_CON.Chan);
		zPrintf(1, " =>NwkAddr=%04x",(short)NWK2APS.JOIN_CON.NwkAddr);

		if(SIB.PreconfigMode == 0)
		{
		#if (UserConfig_DevType == 1)		// ZR
			zPrintf(1, "\n [USER] Start Router");
			APS2NWK.START_ROU_REQ.BattExtend = 0;
			APS2NWK.START_ROU_REQ.BO = UserConfig_BO;
			APS2NWK.START_ROU_REQ.SO = UserConfig_SO;
			ZNWK_START_ROUTE_REQ(&APS2NWK.START_ROU_REQ, &NWK2APS.START_ROU_CON);
			zPrintf(1, " => STA=%02x",(short)NWK2APS.START_ROU_CON.Sta);

			zPrintf(1, "\n [USER] Permit Joining");
			APS2NWK.PERMIT_JOIN_REQ.PermitDur = UserConfig_PermitDuration;
			ZNWK_PERMIT_JOIN_REQ(&APS2NWK.PERMIT_JOIN_REQ, &NWK2APS.PERMIT_JOIN_CON);
			zPrintf(1, " => STA=%02x",(short)NWK2APS.PERMIT_JOIN_CON.Sta);
		
			zPrintf(1, "\n => ZR-JOIN Completed");
		#else
			zPrintf(1, "\n => ZED-JOIN Completed");
		#endif
		}
		else
		{
			zPrintf(1, "\n => Waiting for receiving the NWK key");
		}
	}	
	else
	{
		zPrintf(1, "\n Join Error : Joining is failed");
		return 0;
	}

	// 5. Orphan Joining for checking join status
#if (AppOption_Orphan_AfterJoin)
	zPrintf(1, "\n [USER] Orphaning");	
	APS2NWK.JOIN_REQ.Rejoin = 1;		// Orphaning
	APS2NWK.JOIN_REQ.ScanChan = 1;
	APS2NWK.JOIN_REQ.ScanChan <<= ParentChannel;
	ZNWK_JOIN_REQ(&APS2NWK.JOIN_REQ, &NWK2APS.JOIN_CON);
	zPrintf(1, " => STA=%02x",(short)NWK2APS.JOIN_CON.Sta);
	if(NWK2APS.JOIN_CON.Sta != nSTA_SUCCESS)
	{
		// Orphaning is failed ==> Reset Myself
		APS2NWK.RESET_REQ.WarmStart = 0;
		ZNWK_RESET_REQ(&APS2NWK.RESET_REQ, &NWK2APS.RESET_CON);
		zPrintf(1, "\n Join Error : Orphaning is failed");
		return 0;
	}
#endif

	// 6. Device Announce
	if(SIB.PreconfigMode == 0)
	{
		zPrintf(1, "\n [USER] Send DeviceAnnounce");
		Status = ZSYS_SEND_DEVICE_ANNOUNCE();
		zPrintf(1, " => STA=%02x",(short)Status);
	}

	return 1;
	
}
#endif

#if (	AppOption_Enable_OOB)
/***********************************************************************************
 *
 *	NAME		: ZSYS_SEND_OOB_PACKET()
 *
 *	DESCRIPTION	: Transmits a MAC OOB packet.
 *
 * 	PARAMETER	: 
 *		OobNum - OOB identifier(0x01 ~ 0x0E). 0=No OOB. 0x0F=OTA.
 *		DstShortAddr - network address of destination
 *		msduLen - length of MSDU to transmit
 *		pmsdu - pointer to MSDU
 *
 * 	RETURN		: Stauts of transmitting. 
 *					- 0x00=Success
 *					- 0xE1=Channel Access Failure
 *					- 0xE9=No Ack Failure
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
UINT8 ZSYS_SEND_OOB_PACKET(UINT8 OobNum, UINT16 DstShortAddr, UINT8 msduLen, UINT8 *pmsdu)
{
	NWK2MAC.DATA_REQ.DstAddrMode = 2;
	NWK2MAC.DATA_REQ.DstAddr_U.Short = DstShortAddr;
	NWK2MAC.DATA_REQ.DstPanId = MPIB.macPANId;
	NWK2MAC.DATA_REQ.msduHandle = 0;
	NWK2MAC.DATA_REQ.msduLength = msduLen;
	NWK2MAC.DATA_REQ.pmsdu = pmsdu;
	NWK2MAC.DATA_REQ.SrcAddrMode = 2;
	NWK2MAC.DATA_REQ.SrcAddr_U.Short = MPIB.macShortAddr;
	NWK2MAC.DATA_REQ.SrcPanId = MPIB.macPANId;	
	NWK2MAC.DATA_REQ.TxOptions = (DstShortAddr == 0xFFFF)? 0x00 : 0x01;
	NWK2MAC.DATA_REQ.TxOptions |= (OobNum << 4);
	
	ZMAC_DATA_REQ(&NWK2MAC.DATA_REQ, &MAC2NWK.DATA_CON);
	return	MAC2NWK.DATA_CON.status;
}
#endif



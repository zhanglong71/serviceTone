
/*******************************************************************************
	- Chip		: MG2450/2455
	- Vendor		: RadioPulse Inc, 2007.
	- Date		: 2010-11-15
	- Version		: VER 2.2
	
	[2010-11-15] VER 2.2
	- ZZDP_Send_BindReq() : added.
	- ZZDP_Callback_For_BindReq() : corrected and enhanced.
	- ZZDP_Callback_For_BindRsp() : added.
	- ZZDP_Callback_For_UnbindReq() : corrected.
	- ZZDP_Callback_For_UnbindRsp() : added.
	- ZZDP_Send_NwkAddrReq() : enhanced including arguments.
	- ZZDP_Callback_For_NwkAddrReq() : enhanced.
	- ZZDP_Callback_For_NwkAddrRsp() : added.
	- ZZDP_Send_IEEEAddrReq() : enhanced including arguments.
	- ZZDP_Callback_For_IEEEAddrReq() : enhanced.
	- ZZDP_Callback_For_IEEEAddrRsp() : added.
	- ZZDP_Callback_For_NodeDescReq() : corrected.
	- ZZDP_Callback_For_PowerDescReq() : enhanced.
	- ZZDP_Callback_For_MatchDescReq() : corrected.
	- ZZDP_Send_LQIReq() : added.
	- ZZDP_Callback_For_LQIReq() : added.
	- ZZDP_Send_RtgReq() : added.
	- ZZDP_Callback_For_RtgReq() : added.
	- ZZDP_Send_LeaveReq() : added.
	- ZZDP_Callback_For_LeaveReq() : added.
	- ZZDP_Send_PermitJoinReq() : added.
	- ZZDP_Callback_For_PermitJoinReq() : added.
	- ZZDP_APS_DATA_IND() : enhanced.
	
	[2010-03-02] VER 2.10
	- For ZigBeePRO Development Kit V2.10
*******************************************************************************/

	extern	void ZSYS_SET_Q_TIMER(UINT8 QNum, UINT16 msUnit, UINT8 RightShift);
	extern	UINT8		NumNT;

//============================================================
#if (UserConfig_Enable_ZDP_BindReq)
//============================================================

/***********************************************************************************
 *
 *	NAME		: ZZDP_Send_BindReq()
 *
 *	DESCRIPTION	: Transmits a ZDP Bind_req packet.
 *
 * 	PARAMETER	: pBindReq - Pointer to binding parameters
 				  TargetNwkAddr - Network address of the target device
 *
 * 	RETURN		: Status of ZAPS_DATA_REQ()
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
UINT8 ZZDP_Send_BindReq(ZDPS_BIND_req *pBindReq, UINT16 TargetNwkAddr)
{
	if( (pBindReq->DstMode != 1) && (pBindReq->DstMode != 3) )
	{
		return	aSTA_INVALID_PARAMETER;
	}

	APP_ASDU.Buff[0] = ++APP_DSN;						// APP_ASDU.ZDP.Seq
	rpmemcpy(&APP_ASDU.Buff[1], pBindReq->SrcIEEE, 8);	// APP_ASDU.ZDP.Msg.BIND_REQ.SrcIEEE
	APP_ASDU.Buff[9] = pBindReq->SrcEP;					// APP_ASDU.ZDP.Msg.BIND_REQ.SrcEP
	APP_ASDU.Buff[10] = pBindReq->ClusID;				// APP_ASDU.ZDP.Msg.BIND_REQ.ClusID
	APP_ASDU.Buff[11] = pBindReq->ClusID >> 8;
	APP_ASDU.Buff[12] = pBindReq->DstMode;				// APP_ASDU.ZDP.Msg.BIND_REQ.DstMode
	if(pBindReq->DstMode == 1)		// Group Address
	{
		APP_ASDU.Buff[13] = pBindReq->DstAdd_U.Short;			// APP_ASDU.ZDP.Msg.BIND_REQ.DstAdd_U.Short
		APP_ASDU.Buff[14] = pBindReq->DstAdd_U.Short >> 8;	// APP_ASDU.ZDP.Msg.BIND_REQ.DstEP
		APP2APS.DATA_REQ.asduLen = 15;
	}
	else			// IEEE Address
	{
		rpmemcpy(&APP_ASDU.Buff[13], pBindReq->DstAdd_U.Ext, 8);	// APP_ASDU.ZDP.Msg.BIND_REQ.DstAdd_U.Ext
		APP_ASDU.Buff[21] = pBindReq->DstEP;						// APP_ASDU.ZDP.Msg.BIND_REQ.DstEP
		APP2APS.DATA_REQ.asduLen = 22;
	}	

	APP2APS.DATA_REQ.ClusID = aCID_Bind_Req;
	APP2APS.DATA_REQ.DstAdd_U.Short = TargetNwkAddr;
	APP2APS.DATA_REQ.DstEP = EndpointZDO;
	APP2APS.DATA_REQ.DstMode = 2;
	APP2APS.DATA_REQ.pasdu = APP_ASDU.Buff;
	APP2APS.DATA_REQ.ProfileID = ProfileZDP;
	APP2APS.DATA_REQ.Radius = NIB.nwkMaxDepth << 1;
	APP2APS.DATA_REQ.SrcEP = EndpointZDO;
	APP2APS.DATA_REQ.TxOption = 0x00;

	ZAPS_DATA_REQ(&APP2APS.DATA_REQ, &APS2APP.DATA_CON);

	return APS2APP.DATA_CON.Sta;
}

/***********************************************************************************
 *
 *	NAME		: ZZDP_Callback_For_BindReq()
 *
 *	DESCRIPTION	: Callback function called by ZZDP_APS_DATA_IND() when a ZDP 
 *		Bind_req 	packet is received. 
 *		In this function, if needed, ASDU of response packet to be transmitted should be constructed.
 *
 * 	PARAMETER	:
 *		pReqAsdu - pointer to ASDU of received ZDP packet
 *		pRspAsdu - pointer to ASDU of response ZDP packet to be transmitted
 *
 * 	RETURN		: 
 *		0x00 - The response packet is not transmitted.
 *		Others - ASDU Length of response ZDP packet in bytes.
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
UINT8 ZZDP_Callback_For_BindReq(UINT8 *pReqAsdu, UINT8 *pRspAsdu)
{
	UINT8	DstModeOK;
	APSME_BIND_req	LocalBindReq;
	APSME_BIND_con	LocalBindCon;
	ZDPS_PAY	*pInZDP;
	ZDPS_PAY	*pOutZDP;
	UINT16	TempNwkAddr;

	// Input ZDP Command : Bind Request
	pInZDP = (ZDPS_PAY *)pReqAsdu;	
	Swap_Buffer_2B( (UINT8 *) &(pInZDP->Msg.BIND_REQ.ClusID), 0);
	if(pInZDP->Msg.BIND_REQ.DstMode == 1)	// Group Address
	{
		Swap_Buffer_2B( (UINT8 *)&(pInZDP->Msg.BIND_REQ.DstAdd_U.Short), 0);
		pInZDP->Msg.BIND_REQ.DstEP = pReqAsdu[15];
	}

	// Output ZDP Command : Bind Response
	pOutZDP = (ZDPS_PAY *)pRspAsdu;	
	pOutZDP->Seq = pInZDP->Seq;

	if( 	( (UserConfig_ServerMask & 0x0004) == 0)			// primary binding table cache = 0
	&&	(memcmp(pInZDP->Msg.BIND_REQ.SrcIEEE, MPIB.IEEE_ExtendAddr, 8) ) )	// SrcIEEE address is NOT My IEEE Address
	{
		pOutZDP->Msg.BIND_RSP.Sta = zdpSTA_NOT_SUPPORT;
		return 2;
	}
	
	LocalBindReq.ClusID = pInZDP->Msg.BIND_REQ.ClusID;
	LocalBindReq.DstEP = pInZDP->Msg.BIND_REQ.DstEP;
	LocalBindReq.DstMode = pInZDP->Msg.BIND_REQ.DstMode;
	LocalBindReq.SrcEP = pInZDP->Msg.BIND_REQ.SrcEP;
	rpmemcpy(LocalBindReq.SrcIEEE, pInZDP->Msg.BIND_REQ.SrcIEEE, 8);	
	DstModeOK = 1;
	if(pInZDP->Msg.BIND_REQ.DstMode == 1)	// Group Address
	{
		LocalBindReq.DstAdd_U.Short = pInZDP->Msg.BIND_REQ.DstAdd_U.Short;
	}
	else if(pInZDP->Msg.BIND_REQ.DstMode == 3)
	{
		rpmemcpy(LocalBindReq.DstAdd_U.Ext, pInZDP->Msg.BIND_REQ.DstAdd_U.Ext, 8);
	}
	else
	{
		DstModeOK = 0;
	}

	if(DstModeOK)
	{
		ZAPS_BIND_REQ(&LocalBindReq, &LocalBindCon);
		if(LocalBindCon.Sta == aSTA_TABLE_FULL)
				pOutZDP->Msg.BIND_RSP.Sta = zdpSTA_TABLE_FULL;
		else if(LocalBindCon.Sta == aSTA_ILLEGAL_REQUEST)
				pOutZDP->Msg.BIND_RSP.Sta = zdpSTA_INV_REQTYPE;
		else		pOutZDP->Msg.BIND_RSP.Sta = zdpSTA_SUCCESS;		

		if( (LocalBindCon.Sta == aSTA_SUCCESS) && (LocalBindReq.DstMode == 3) )
		{
			if(NWKLIB_DevMgmt_GetNWKAddr(LocalBindReq.DstAdd_U.Ext, &TempNwkAddr) == 0)
			{
				// TODO: Needed how to transmit NwkAddr_req command.
			}
		}
	}
	else
	{
		pOutZDP->Msg.BIND_RSP.Sta = zdpSTA_INV_REQTYPE;
	}

	return	2;
}

/***********************************************************************************
 *
 *	NAME		: ZZDP_Callback_For_BindRsp()
 *
 *	DESCRIPTION	: 
 *
 * 	PARAMETER	:
 *
 * 	RETURN		: 
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
void ZZDP_Callback_For_BindRsp(UINT8 *pRspAsdu)
{
	ZDPS_PAY	*pInZDP;

	// Input ZDP Command : Bind Response
	pInZDP = (ZDPS_PAY *)pRspAsdu;

//	zPrintf(1, "\n Bind_rsp.Sta = %02x",(short)pInZDP->Msg.BIND_RSP.Sta);
}

/***********************************************************************************
 *
 *	NAME		: ZZDP_Send_UnbindReq()
 *
 *	DESCRIPTION	: Transmits a ZDP Bind_req packet.
 *
 * 	PARAMETER	: pUnbindReq - Pointer to unbinding parameters
 				  TargetNwkAddr - Network address of the target device
 *
 * 	RETURN		: Status of ZAPS_DATA_REQ()
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
UINT8 ZZDP_Send_UnbindReq(ZDPS_UNBIND_req *pUnbindReq, UINT16 TargetNwkAddr)
{
	if( (pUnbindReq->DstMode != 1) && (pUnbindReq->DstMode != 3) )
	{
		return	aSTA_INVALID_PARAMETER;
	}

	APP_ASDU.Buff[0] = ++APP_DSN;						// APP_ASDU.ZDP.Seq
	rpmemcpy(&APP_ASDU.Buff[1], pUnbindReq->SrcIEEE, 8);	// APP_ASDU.ZDP.Msg.BIND_REQ.SrcIEEE
	APP_ASDU.Buff[9] = pUnbindReq->SrcEP;					// APP_ASDU.ZDP.Msg.BIND_REQ.SrcEP
	APP_ASDU.Buff[10] = pUnbindReq->ClusID;				// APP_ASDU.ZDP.Msg.BIND_REQ.ClusID
	APP_ASDU.Buff[11] = pUnbindReq->ClusID >> 8;
	APP_ASDU.Buff[12] = pUnbindReq->DstMode;				// APP_ASDU.ZDP.Msg.BIND_REQ.DstMode
	if(pUnbindReq->DstMode == 1)		// Group Address
	{
		APP_ASDU.Buff[13] = pUnbindReq->DstAdd_U.Short;			// APP_ASDU.ZDP.Msg.BIND_REQ.DstAdd_U.Short
		APP_ASDU.Buff[14] = pUnbindReq->DstAdd_U.Short >> 8;	// APP_ASDU.ZDP.Msg.BIND_REQ.DstEP
		APP2APS.DATA_REQ.asduLen = 15;
	}
	else			// IEEE Address
	{
		rpmemcpy(&APP_ASDU.Buff[13], pUnbindReq->DstAdd_U.Ext, 8);	// APP_ASDU.ZDP.Msg.BIND_REQ.DstAdd_U.Ext
		APP_ASDU.Buff[21] = pUnbindReq->DstEP;						// APP_ASDU.ZDP.Msg.BIND_REQ.DstEP
		APP2APS.DATA_REQ.asduLen = 22;
	}	

	APP2APS.DATA_REQ.ClusID = aCID_Unbind_Req;
	APP2APS.DATA_REQ.DstAdd_U.Short = TargetNwkAddr;
	APP2APS.DATA_REQ.DstEP = EndpointZDO;
	APP2APS.DATA_REQ.DstMode = 2;
	APP2APS.DATA_REQ.pasdu = APP_ASDU.Buff;
	APP2APS.DATA_REQ.ProfileID = ProfileZDP;
	APP2APS.DATA_REQ.Radius = NIB.nwkMaxDepth << 1;
	APP2APS.DATA_REQ.SrcEP = EndpointZDO;
	APP2APS.DATA_REQ.TxOption = 0x00;

	ZAPS_DATA_REQ(&APP2APS.DATA_REQ, &APS2APP.DATA_CON);

	return APS2APP.DATA_CON.Sta;
}

/***********************************************************************************
 *
 *	NAME		: ZZDP_Callback_For_UnbindReq()
 *
 *	DESCRIPTION	: Callback function called by ZZDP_APS_DATA_IND() when a ZDP 
 *		Unbind_req packet is received. 
 *		In this function, if needed, ASDU of response packet to be transmitted should be constructed.
 *
 * 	PARAMETER	:
 *		pReqAsdu - pointer to ASDU of received ZDP packet
 *		pRspAsdu - pointer to ASDU of response ZDP packet to be transmitted
 *
 * 	RETURN		: 
 *		0x00 - The response packet is not transmitted.
 *		Others - ASDU Length of response ZDP packet in bytes.
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
UINT8 ZZDP_Callback_For_UnbindReq(UINT8 *pReqAsdu, UINT8 *pRspAsdu)
{
	UINT8	DstModeOK;
	APSME_UNBIND_req	LocalUnbindReq;
	APSME_UNBIND_con	LocalUnbindCon;
	ZDPS_PAY	*pInZDP;
	ZDPS_PAY	*pOutZDP;	

	// Input ZDP Command : Unbind Request
	pInZDP = (ZDPS_PAY *)pReqAsdu;
	Swap_Buffer_2B( (UINT8 *) &(pInZDP->Msg.UNBIND_REQ.ClusID), 0);	
	if(pInZDP->Msg.UNBIND_REQ.DstMode == 1)		// Group Address
	{
		Swap_Buffer_2B( (UINT8 *)&(pInZDP->Msg.UNBIND_REQ.DstAdd_U.Short), 0);
		pInZDP->Msg.UNBIND_REQ.DstEP = pReqAsdu[15];
	}
	
	// Output ZDP Command : Unbind Response
	pOutZDP = (ZDPS_PAY *)pRspAsdu;
	pOutZDP->Seq = pInZDP->Seq;

	if(	( (UserConfig_ServerMask & 0x0004) == 0)		// primary binding table cache = 0
	&&	(memcmp(pInZDP->Msg.UNBIND_REQ.SrcIEEE, MPIB.IEEE_ExtendAddr, 8) ) )	// SrcIEEE address is NOT My IEEE Address
	{
		pOutZDP->Msg.UNBIND_RSP.Sta = zdpSTA_NOT_SUPPORT;
		return 2;
	}

	LocalUnbindReq.ClusID = pInZDP->Msg.UNBIND_REQ.ClusID;
	LocalUnbindReq.DstEP = pInZDP->Msg.UNBIND_REQ.DstEP;
	LocalUnbindReq.DstMode = pInZDP->Msg.UNBIND_REQ.DstMode;
	LocalUnbindReq.SrcEP = pInZDP->Msg.UNBIND_REQ.SrcEP;
	rpmemcpy(LocalUnbindReq.SrcIEEE, pInZDP->Msg.UNBIND_REQ.SrcIEEE, 8);	
	DstModeOK = 1;	
	if(pInZDP->Msg.UNBIND_REQ.DstMode == 1)		// Group Address
	{		
		LocalUnbindReq.DstAdd_U.Short = pInZDP->Msg.UNBIND_REQ.DstAdd_U.Short;
	}
	else if(pInZDP->Msg.UNBIND_REQ.DstMode == 3)
	{
		rpmemcpy(LocalUnbindReq.DstAdd_U.Ext, pInZDP->Msg.UNBIND_REQ.DstAdd_U.Ext, 8);
	}
	else
	{
		DstModeOK = 0;
	}

	if(DstModeOK)
	{
		ZAPS_UNBIND_REQ(&LocalUnbindReq, &LocalUnbindCon);
		if(LocalUnbindCon.Sta == aSTA_INVALID_BINDING)
				pOutZDP->Msg.UNBIND_RSP.Sta = zdpSTA_NO_ENTRY;
		else	 if(LocalUnbindCon.Sta == aSTA_ILLEGAL_REQUEST)
				pOutZDP->Msg.UNBIND_RSP.Sta = zdpSTA_INV_REQTYPE;
		else		pOutZDP->Msg.UNBIND_RSP.Sta = zdpSTA_SUCCESS;
	}
	else
	{
		pOutZDP->Msg.UNBIND_RSP.Sta = zdpSTA_INV_REQTYPE;
	}

	return 2;	
}

/***********************************************************************************
 *
 *	NAME		: ZZDP_Callback_For_UnbindRsp()
 *
 *	DESCRIPTION	: 
 *
 * 	PARAMETER	:
 *
 * 	RETURN		: 
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
void ZZDP_Callback_For_UnbindRsp(UINT8 *pRspAsdu)
{
	ZDPS_PAY	*pInZDP;

	// Input ZDP Command : Bind Response
	pInZDP = (ZDPS_PAY *)pRspAsdu;

//	zPrintf(1, "\n Unbind_rsp.Sta = %02x",(short)pInZDP->Msg.UNBIND_RSP.Sta);
}

#endif
//============================================================







//============================================================
//	Device Announce
//============================================================

/***********************************************************************************
 *
 *	NAME		: ZZDP_Callback_For_DeviceAnnounce()
 *
 *	DESCRIPTION	: Callback function called by ZZDP_APS_DATA_IND() when a ZDP Device_annce 
 *		packet is received. 
 *
 * 	PARAMETER	: pAnnAsdu - pointer to ASDU of received ZDP packet
 *
 * 	RETURN		: None
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
void ZZDP_Callback_For_DeviceAnnounce(UINT8 *pAnnAsdu)
{
	UINT8	CmpIEEE[8];
	ZDPS_PAY	*pInZDP;

	pInZDP = (ZDPS_PAY *)pAnnAsdu;
	Swap_Buffer_2B( (UINT8 *) &(pInZDP->Msg.DEV_ANNCE.NwkAddr), 0);

	if(pInZDP->Msg.DEV_ANNCE.NwkAddr == NIB.nwkNetworkAddr)
	{
		if(memcmp(MPIB.IEEE_ExtendAddr, pInZDP->Msg.DEV_ANNCE.IEEEAddr, 8))
		{
			NWKLIB_Set_ConflictAddr(NIB.nwkNetworkAddr);
		
			// Confliction
			if(NIB.DEVTYPE == 'R')
			{
				#if (UserConfig_Capability & 0x02)		// FFD Library
					NIB.nwkNetworkAddr = NWKLIB_Get_RandomNwkAddr(0xFF);
					UpdateNIB(nNewAddress);				
					ZSYS_SET_SHORT_ADDR(NIB.nwkNetworkAddr);
					ZSYS_SEND_DEVICE_ANNOUNCE();
					NWKLIB_SEND_AddrConflict(pInZDP->Msg.DEV_ANNCE.NwkAddr);
				#endif
			}
			NWKLIB_DevMgmt_DeleteNWKAddr(pInZDP->Msg.DEV_ANNCE.NwkAddr);
		}
	}
	else
	{
		if(NWKLIB_DevMgmt_GetIEEEAddr(pInZDP->Msg.DEV_ANNCE.NwkAddr, CmpIEEE))
		{
			if(memcmp(CmpIEEE, pInZDP->Msg.DEV_ANNCE.IEEEAddr, 8))
			{
				// Confliction
				NWKLIB_Set_ConflictAddr(pInZDP->Msg.DEV_ANNCE.NwkAddr);
				#if (UserConfig_Capability & 0x02)		// FFD Library
					NWKLIB_PROC_AddrConflict(pInZDP->Msg.DEV_ANNCE.NwkAddr, 0  );
				#endif
				NWKLIB_DevMgmt_DeleteIEEEAddr(pInZDP->Msg.DEV_ANNCE.IEEEAddr);
			}
			else
			{
				NWKLIB_DevMgmt_UpdateIEEEAddr(pInZDP->Msg.DEV_ANNCE.NwkAddr, pInZDP->Msg.DEV_ANNCE.IEEEAddr);
			}
		}
		else
		{
			NWKLIB_DevMgmt_UpdateIEEEAddr(pInZDP->Msg.DEV_ANNCE.NwkAddr, pInZDP->Msg.DEV_ANNCE.IEEEAddr);
		}
	}
}

//============================================================




//============================================================
#if (UserConfig_Enable_ZDP_NwkAddrReq)
//============================================================

/***********************************************************************************
 *
 *	NAME		: ZZDP_Send_NwkAddrReq()
 *
 *	DESCRIPTION	: Transmits a ZDP NWK_addr_req packet.
 *
 * 	PARAMETER	: pTargetIEEEAddr - pointer to IEEE address of the target device
 *
 * 	RETURN		: Status of ZAPS_DATA_REQ()
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
UINT8 ZZDP_Send_NwkAddrReq(UINT8 *pTargetIEEEAddr, UINT8 ExtMode, UINT8 StartIdx)
{
	APP_ASDU.Buff[0] = ++APP_DSN;						// APP_ASDU.ZDP.Seq
	rpmemcpy(&APP_ASDU.Buff[1], pTargetIEEEAddr, 8);	// APP_ASDU.ZDP.Msg.NWK_ADDR_REQ.IEEEAddr
	APP_ASDU.Buff[9] = ExtMode;							// APP_ASDU.ZDP.Msg.NWK_ADDR_REQ.ReqType=0(single)
	APP_ASDU.Buff[10] = StartIdx;							// APP_ASDU.ZDP.Msg.NWK_ADDR_REQ.StartIdx=0

	APP2APS.DATA_REQ.asduLen = 11;
	APP2APS.DATA_REQ.ClusID = aCID_Nwk_Addr_Req;
	APP2APS.DATA_REQ.DstAdd_U.Short = nBROAD_RXON;
	APP2APS.DATA_REQ.DstEP = EndpointZDO;
	APP2APS.DATA_REQ.DstMode = 2;
	APP2APS.DATA_REQ.pasdu = APP_ASDU.Buff;
	APP2APS.DATA_REQ.ProfileID = ProfileZDP;
	APP2APS.DATA_REQ.Radius = NIB.nwkMaxDepth << 1;
	APP2APS.DATA_REQ.SrcEP = EndpointZDO;
	APP2APS.DATA_REQ.TxOption = 0x00;

	ZAPS_DATA_REQ(&APP2APS.DATA_REQ, &APS2APP.DATA_CON);

	return APS2APP.DATA_CON.Sta;
}

/***********************************************************************************
 *
 *	NAME		: ZZDP_Callback_For_NwkAddrReq()
 *
 *	DESCRIPTION	: Callback function called by ZZDP_APS_DATA_IND() when a ZDP 
 *		NWK_addr_req packet is received. 
 *		In this function, if needed, ASDU of response packet to be transmitted should be constructed.
 *
 * 	PARAMETER	:
 *		pReqAsdu - pointer to ASDU of received ZDP packet
 *		pRspAsdu - pointer to ASDU of response ZDP packet to be transmitted
 *
 * 	RETURN		: 
 *		0x00 - The response packet is not transmitted.
 *		Others - ASDU Length of response ZDP packet in bytes.
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
UINT8 ZZDP_Callback_For_NwkAddrReq(UINT8 *pReqAsdu, UINT8 *pRspAsdu)
{
	UINT8	N;
	UINT8	i;
	UINT8	ZdpLen;
	ZDPS_PAY	*pInZDP;
	ZDPS_PAY	*pOutZDP;	

	// Input ZDP Command : NWK Address Request
	pInZDP = (ZDPS_PAY *)pReqAsdu;

	// Output ZDP Command : NWK Address Response
	pOutZDP = (ZDPS_PAY *)pRspAsdu;
	pOutZDP->Seq = pInZDP->Seq;
	pOutZDP->Msg.NWK_ADDR_RSP.Sta = zdpSTA_SUCCESS;
	rpmemcpy(pOutZDP->Msg.NWK_ADDR_RSP.IEEEAddr, pInZDP->Msg.NWK_ADDR_REQ.IEEEAddr, 8);
	pOutZDP->Msg.NWK_ADDR_RSP.NwkAddr = NIB.nwkNetworkAddr;
	Swap_Buffer_2B((UINT8 *)&(pOutZDP->Msg.NWK_ADDR_RSP.NwkAddr), 0);
		
	// If IEEE Address is not matched, Response is not transmitted
	if(memcmp(MPIB.IEEE_ExtendAddr, pInZDP->Msg.NWK_ADDR_REQ.IEEEAddr, 8))	// Different IEEE Addr
	{
		if(NWKLIB_FindNT_IEEEAddr(nNT_RELATION_CHILD, pInZDP->Msg.NWK_ADDR_REQ.IEEEAddr))
		{
			if(NIB.pNT[NumNT].RxOnWhenIdle)
			{
				return 0;
			}
			else
			{
				pOutZDP->Msg.NWK_ADDR_RSP.NwkAddr = NIB.pNT[NumNT].NwkAddr;
				return 12;
			}
		}
		else
		{
			return 0;
		}
	}	

	ZdpLen = 12;
	if(pInZDP->Msg.NWK_ADDR_REQ.ReqType == REQTYPE_SINGLE)
	{
	}
	else if(pInZDP->Msg.NWK_ADDR_REQ.ReqType == REQTYPE_EXTEND)
	{
		N = 0;
		
		if( (NIB.DEVTYPE == 'C') || (NIB.DEVTYPE == 'R') )
		{
			if(pInZDP->Msg.NWK_ADDR_REQ.StartIdx < NIB.SIZE_NT)
			{
				for(i=pInZDP->Msg.NWK_ADDR_REQ.StartIdx ; i<NIB.SIZE_NT ; i++)
				{
					if( (NIB.pNT[i].EnaNT) && (NIB.pNT[i].Relation == nNT_RELATION_CHILD) )
					{	
						pOutZDP->Msg.NWK_ADDR_RSP.NwkAddrAssoDevList[N] = NIB.pNT[i].NwkAddr;
						Swap_Buffer_2B((UINT8 *) &(pOutZDP->Msg.NWK_ADDR_RSP.NwkAddrAssoDevList[N]), 0);
						N++;
					}

					if( N == ((apscMaxASDU-14)/2) )	break;
				}
			}
		}

		if(N)
		{
			pOutZDP->Msg.NWK_ADDR_RSP.NumAssoDev = N;
			pOutZDP->Msg.NWK_ADDR_RSP.StartIdx = pInZDP->Msg.NWK_ADDR_REQ.StartIdx;
			ZdpLen += 2*N + 2;		// NumAsso, StartIdx Field
		}
	}
	else
	{
		pOutZDP->Msg.NWK_ADDR_RSP.Sta = zdpSTA_INV_REQTYPE;
	}
	
	return	ZdpLen;	
}

/***********************************************************************************
 *
 *	NAME		: ZZDP_Callback_For_NwkAddrRsp()
 *
 *	DESCRIPTION	: Callback function called by ZZDP_APS_DATA_IND() when a ZDP 
 *		NWK_addr_rsp packet is received. 
 *		In this function, if needed, a pair of short and extended address is registred in the device table.
 *
 * 	PARAMETER	: pRspAsdu - pointer to ASDU of received ZDP packet
 *
 * 	RETURN		: None
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
void ZZDP_Callback_For_NwkAddrRsp(UINT8 *pRspAsdu)
{
	ZDPS_PAY	*pInZDP;

	// Input ZDP Command : NWK Address Response
	pInZDP = (ZDPS_PAY *)pRspAsdu;

//	zPrintf(1, "\n NwkAddr_Rsp.Sta = %02x",(short)pInZDP->Msg.NWK_ADDR_RSP.Sta);
	if(pInZDP->Msg.NWK_ADDR_RSP.Sta == aSTA_SUCCESS)
	{
		Swap_Buffer_2B( (UINT8 *) &(pInZDP->Msg.NWK_ADDR_RSP.NwkAddr), 0);
		NWKLIB_DevMgmt_UpdateIEEEAddr(pInZDP->Msg.NWK_ADDR_RSP.NwkAddr, pInZDP->Msg.NWK_ADDR_RSP.IEEEAddr);
//		zPrintf(1, " :NwkAddr=%04x",pInZDP->Msg.NWK_ADDR_RSP.NwkAddr);
	}
}

#endif
//============================================================










//============================================================
#if (UserConfig_Enable_ZDP_IEEEAddrReq)
//============================================================

/***********************************************************************************
 *
 *	NAME		: ZZDP_Send_IEEEAddrReq()
 *
 *	DESCRIPTION	: Transmits a ZDP IEEE_addr_req packet.
 *
 * 	PARAMETER	: TargetNwkAddr - Network address of the target device
 *
 * 	RETURN		: Status of ZAPS_DATA_REQ()
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
UINT8 ZZDP_Send_IEEEAddrReq(UINT16 TargetNwkAddr, UINT8 ExtMode, UINT8 StartIdx)
{	
	APP_ASDU.Buff[0] = ++APP_DSN;			// APP_ASDU.ZDP.Seq
	APP_ASDU.Buff[1] = TargetNwkAddr;		// APP_ASDU.ZDP.Msg.IEEE_ADDR_REQ.NwkAddr
	APP_ASDU.Buff[2] = TargetNwkAddr >> 8;	// APP_ASDU.ZDP.Msg.IEEE_ADDR_REQ.NwkAddr
	APP_ASDU.Buff[3] = ExtMode;				// APP_ASDU.ZDP.Msg.IEEE_ADDR_REQ.ReqType=0(single)
	APP_ASDU.Buff[4] = StartIdx;				// APP_ASDU.ZDP.Msg.IEEE_ADDR_REQ.StartIdx=0

	APP2APS.DATA_REQ.asduLen = 5;
	APP2APS.DATA_REQ.ClusID = aCID_IEEE_Addr_Req;	
	APP2APS.DATA_REQ.DstAdd_U.Short = TargetNwkAddr;
	APP2APS.DATA_REQ.DstEP = EndpointZDO;
	APP2APS.DATA_REQ.DstMode = 2;
	APP2APS.DATA_REQ.pasdu = APP_ASDU.Buff;
	APP2APS.DATA_REQ.ProfileID = ProfileZDP;
	APP2APS.DATA_REQ.Radius = NIB.nwkMaxDepth << 1;
	APP2APS.DATA_REQ.SrcEP = EndpointZDO;
	APP2APS.DATA_REQ.TxOption = 0x00;

	ZAPS_DATA_REQ(&APP2APS.DATA_REQ, &APS2APP.DATA_CON);

	return APS2APP.DATA_CON.Sta;
}

/***********************************************************************************
 *
 *	NAME		: ZZDP_Callback_For_IEEEAddrReq()
 *
 *	DESCRIPTION	: Callback function called by ZZDP_APS_DATA_IND() when a ZDP 
 *		IEEE_addr_req packet is received. 
 *		In this function, if needed, ASDU of response packet to be transmitted should be constructed.
 *
 * 	PARAMETER	:
 *		pReqAsdu - pointer to ASDU of received ZDP packet
 *		pRspAsdu - pointer to ASDU of response ZDP packet to be transmitted
 *
 * 	RETURN		: 
 *		0x00 - The response packet is not transmitted.
 *		Others - ASDU Length of response ZDP packet in bytes.
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
UINT8 ZZDP_Callback_For_IEEEAddrReq(UINT8 *pReqAsdu, UINT8 *pRspAsdu)
{	
	UINT8	i;
	UINT8	N;
	UINT8	ZdpLen;
	ZDPS_PAY	*pInZDP;
	ZDPS_PAY	*pOutZDP;	

	// Input ZDP Command : IEEE Address Request
	pInZDP = (ZDPS_PAY *)pReqAsdu;
	Swap_Buffer_2B( (UINT8 *) &(pInZDP->Msg.IEEE_ADDR_REQ.NwkAddr), 0);

	// Output ZDP Command : IEEE Address Response
	pOutZDP = (ZDPS_PAY *)pRspAsdu;
	pOutZDP->Seq = pInZDP->Seq;
	pOutZDP->Msg.IEEE_ADDR_RSP.Sta = zdpSTA_SUCCESS;
	rpmemcpy(pOutZDP->Msg.IEEE_ADDR_RSP.IEEEAddr, MPIB.IEEE_ExtendAddr, 8);
	pOutZDP->Msg.IEEE_ADDR_RSP.NwkAddr = pInZDP->Msg.IEEE_ADDR_REQ.NwkAddr;
	Swap_Buffer_2B( (UINT8 *) &(pOutZDP->Msg.IEEE_ADDR_RSP.NwkAddr), 0);

	// If NwkAddr is not matched, Response is not transmitted.
	if(pInZDP->Msg.IEEE_ADDR_REQ.NwkAddr != NIB.nwkNetworkAddr)
	{
		if(NWKLIB_FindNT_NwkAddr(pInZDP->Msg.IEEE_ADDR_REQ.NwkAddr))
		{
			if(	(NIB.pNT[NumNT].Relation == nNT_RELATION_CHILD)
			&&	(NIB.pNT[NumNT].RxOnWhenIdle == 0) )
			{
				rpmemcpy(pOutZDP->Msg.IEEE_ADDR_RSP.IEEEAddr, NIB.pNT[NumNT].IEEEAddr, 8);
				return 12;
			}
			else
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
	}	

	ZdpLen = 12;
	if(pInZDP->Msg.IEEE_ADDR_REQ.ReqType == REQTYPE_SINGLE)
	{
	}
	else if(pInZDP->Msg.IEEE_ADDR_REQ.ReqType == REQTYPE_EXTEND)
	{
		N = 0;
		
		if( (NIB.DEVTYPE == 'C') || (NIB.DEVTYPE == 'R') )
		{
			if(pInZDP->Msg.IEEE_ADDR_REQ.StartIdx < NIB.SIZE_NT)
			{
				for(i=pInZDP->Msg.IEEE_ADDR_REQ.StartIdx ; i<NIB.SIZE_NT ; i++)
				{
					if( (NIB.pNT[i].EnaNT) && (NIB.pNT[i].Relation == nNT_RELATION_CHILD) )
					{
						pOutZDP->Msg.IEEE_ADDR_RSP.NwkAddrAssoDevList[N] = NIB.pNT[i].NwkAddr;
						Swap_Buffer_2B((UINT8 *) &(pOutZDP->Msg.IEEE_ADDR_RSP.NwkAddrAssoDevList[N]), 0);
						N++;
					}

					if( N == ((apscMaxASDU-14)/2) )	break;
				}
			}
		}

		if(N)
		{
			pOutZDP->Msg.IEEE_ADDR_RSP.NumAssoDev = N;
			pOutZDP->Msg.IEEE_ADDR_RSP.StartIdx = pInZDP->Msg.IEEE_ADDR_REQ.StartIdx;
			ZdpLen += 2*N + 2;		// NumAsso, StartIdx Field
		}		
	}
	else
	{
		pOutZDP->Msg.IEEE_ADDR_RSP.Sta = zdpSTA_INV_REQTYPE;
	}

	return	ZdpLen;
}

/***********************************************************************************
 *
 *	NAME		: ZZDP_Callback_For_IEEEAddrRsp()
 *
 *	DESCRIPTION	: Callback function called by ZZDP_APS_DATA_IND() when a ZDP 
 *		IEEE_addr_rsp packet is received. 
 *		In this function, if needed, a pair of short and extended address is registred in the device table.
 *
 * 	PARAMETER	: pRspAsdu - pointer to ASDU of received ZDP packet
 *
 * 	RETURN		: None
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
void ZZDP_Callback_For_IEEEAddrRsp(UINT8 *pRspAsdu)
{
	ZDPS_PAY	*pInZDP;

	// Input ZDP Command : NWK Address Response
	pInZDP = (ZDPS_PAY *)pRspAsdu;

	if(pInZDP->Msg.IEEE_ADDR_RSP.Sta == aSTA_SUCCESS)
	{
		Swap_Buffer_2B( (UINT8 *) &(pInZDP->Msg.IEEE_ADDR_RSP.NwkAddr), 0);
		NWKLIB_DevMgmt_UpdateIEEEAddr(pInZDP->Msg.IEEE_ADDR_RSP.NwkAddr, pInZDP->Msg.IEEE_ADDR_RSP.IEEEAddr);
	}
}

#endif
//============================================================









//============================================================
#if (UserConfig_Enable_ZDP_NodeDescReq)
//============================================================

/***********************************************************************************
 *
 *	NAME		: ZZDP_Send_NodeDescReq()
 *
 *	DESCRIPTION	: Transmits a ZDP Node_Desc_req packet.
 *
 * 	PARAMETER	: TargetNwkAddr - Network address of the target device
 *
 * 	RETURN		: Status of ZAPS_DATA_REQ()
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
UINT8 ZZDP_Send_NodeDescReq(UINT16 TargetNwkAddr)
{
	APP_ASDU.Buff[0] = ++APP_DSN;			// APP_ASDU.ZDP.Seq
	APP_ASDU.Buff[1] = TargetNwkAddr;		// APP_ASDU.ZDP.Msg.NODE_DESC_REQ.NwkAddr
	APP_ASDU.Buff[2] = TargetNwkAddr >> 8;	// APP_ASDU.ZDP.Msg.NODE_DESC_REQ.NwkAddr

	APP2APS.DATA_REQ.asduLen = 3;
	APP2APS.DATA_REQ.ClusID = aCID_Node_Desc_Req;
	APP2APS.DATA_REQ.DstAdd_U.Short = TargetNwkAddr;
	APP2APS.DATA_REQ.DstEP = EndpointZDO;
	APP2APS.DATA_REQ.DstMode = 2;
	APP2APS.DATA_REQ.pasdu = APP_ASDU.Buff;
	APP2APS.DATA_REQ.ProfileID = ProfileZDP;
	APP2APS.DATA_REQ.Radius = NIB.nwkMaxDepth << 1;
	APP2APS.DATA_REQ.SrcEP = EndpointZDO;
	APP2APS.DATA_REQ.TxOption = 0x00;

	ZAPS_DATA_REQ(&APP2APS.DATA_REQ, &APS2APP.DATA_CON);

	return APS2APP.DATA_CON.Sta;
}

/***********************************************************************************
 *
 *	NAME		: ZZDP_Callback_For_NodeDescReq()
 *
 *	DESCRIPTION	: Callback function called by ZZDP_APS_DATA_IND() when a ZDP 
 *		Node_Desc_req packet is received. 
 *		In this function, if needed, ASDU of response packet to be transmitted should be constructed.
 *
 * 	PARAMETER	:
 *		pReqAsdu - pointer to ASDU of received ZDP packet
 *		pRspAsdu - pointer to ASDU of response ZDP packet to be transmitted
 *
 * 	RETURN		: 
 *		0x00 - The response packet is not transmitted.
 *		Others - ASDU Length of response ZDP packet in bytes.
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
UINT8 ZZDP_Callback_For_NodeDescReq(UINT8 *pReqAsdu, UINT8 *pRspAsdu)
{
	UINT8	ZdpLen;
	UINT8	DevType;
	UINT8	MyChild;
	UINT8	DescAvail;
	UINT16	NwkAddrOfInterest;
	ZDPS_PAY	*pOutZDP;

	// Input ZDP Command : Node Descriptor Request
	NwkAddrOfInterest = (pReqAsdu[2] << 8) | pReqAsdu[1];

	// Output ZDP Command : Node Descriptor Response
	pOutZDP = (ZDPS_PAY *)pRspAsdu;	
	pOutZDP->Seq = pReqAsdu[0];
	pOutZDP->Msg.NODE_DESC_RSP.Sta = zdpSTA_SUCCESS;
	pOutZDP->Msg.NODE_DESC_RSP.NwkAddr = NwkAddrOfInterest;
	Swap_Buffer_2B( (UINT8 *) &(pOutZDP->Msg.NODE_DESC_RSP.NwkAddr), 0);

	MyChild = 0;
	DescAvail = 0;
	
	ZdpLen = 4;
	if(NwkAddrOfInterest == NIB.nwkNetworkAddr)
	{	
		pOutZDP->Msg.NODE_DESC_RSP.DescBuf[0] = AppNodeDesc.Avail_Type;
		pOutZDP->Msg.NODE_DESC_RSP.DescBuf[1] = AppNodeDesc.Freq_ApsFlag;
		pOutZDP->Msg.NODE_DESC_RSP.DescBuf[2] = AppNodeDesc.MacCapa;
		pOutZDP->Msg.NODE_DESC_RSP.DescBuf[3] = AppNodeDesc.Manufacture;
		pOutZDP->Msg.NODE_DESC_RSP.DescBuf[4] = AppNodeDesc.Manufacture >> 8;
		pOutZDP->Msg.NODE_DESC_RSP.DescBuf[5] = AppNodeDesc.MaxBufSize;
		pOutZDP->Msg.NODE_DESC_RSP.DescBuf[6] = AppNodeDesc.MaxInTranSize;
		pOutZDP->Msg.NODE_DESC_RSP.DescBuf[7] = AppNodeDesc.MaxInTranSize >> 8;
		pOutZDP->Msg.NODE_DESC_RSP.DescBuf[8] = AppNodeDesc.ServerMask;
		pOutZDP->Msg.NODE_DESC_RSP.DescBuf[9] = AppNodeDesc.ServerMask >> 8;
		pOutZDP->Msg.NODE_DESC_RSP.DescBuf[10] = AppNodeDesc.MaxOutTransSize;
		pOutZDP->Msg.NODE_DESC_RSP.DescBuf[11] = AppNodeDesc.MaxOutTransSize >> 8;
		pOutZDP->Msg.NODE_DESC_RSP.DescBuf[12] = AppNodeDesc.DescCapa >> 8;
		ZdpLen += 13;		// length of Node Descriptor
	}
	else
	{	
		DevType = UserConfig_DevType;
		if(DevType==0 || DevType==1)
		{
			// Check REQ.NwkAddr is MyChild ???			
			if(MyChild)
			{
				// Check Node Descriptor for MyChid is Available ???
				if(DescAvail)
				{
					// Copy Node Descriptor
				}
				else
				{
					pOutZDP->Msg.NODE_DESC_RSP.Sta = zdpSTA_NO_DESCRIPT;
				}
			}
			else
			{
				pOutZDP->Msg.NODE_DESC_RSP.Sta = zdpSTA_DEV_NOT_FOU;
			}
		}
		else
		{
			pOutZDP->Msg.NODE_DESC_RSP.Sta = zdpSTA_INV_REQTYPE;
		}
	}

	return	ZdpLen;
}

#endif
//============================================================













//============================================================
#if (UserConfig_Enable_ZDP_PowerDescReq)
//============================================================

/***********************************************************************************
 *
 *	NAME		: ZZDP_Send_PowerDescReq()
 *
 *	DESCRIPTION	: Transmits a ZDP Power_Desc_req packet.
 *
 * 	PARAMETER	: TargetNwkAddr - Network address of the target device
 *
 * 	RETURN		: Status of ZAPS_DATA_REQ()
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
UINT8 ZZDP_Send_PowerDescReq(UINT16 TargetNwkAddr)
{
	APP_ASDU.Buff[0] = ++APP_DSN;			// APP_ASDU.ZDP.Seq
	APP_ASDU.Buff[1] = TargetNwkAddr;		// APP_ASDU.ZDP.Msg.POWER_DESC_REQ.NwkAddr
	APP_ASDU.Buff[2] = TargetNwkAddr >> 8;	// APP_ASDU.ZDP.Msg.POWER_DESC_REQ.NwkAddr

	APP2APS.DATA_REQ.asduLen = 3;
	APP2APS.DATA_REQ.ClusID = aCID_Power_Desc_Req;
	APP2APS.DATA_REQ.DstAdd_U.Short = TargetNwkAddr;
	APP2APS.DATA_REQ.DstEP = EndpointZDO;
	APP2APS.DATA_REQ.DstMode = 2;
	APP2APS.DATA_REQ.pasdu = APP_ASDU.Buff;
	APP2APS.DATA_REQ.ProfileID = ProfileZDP;
	APP2APS.DATA_REQ.Radius = NIB.nwkMaxDepth << 1;
	APP2APS.DATA_REQ.SrcEP = EndpointZDO;
	APP2APS.DATA_REQ.TxOption = 0x00;

	ZAPS_DATA_REQ(&APP2APS.DATA_REQ, &APS2APP.DATA_CON);

	return APS2APP.DATA_CON.Sta;
}

/***********************************************************************************
 *
 *	NAME		: ZZDP_Callback_For_PowerDescReq()
 *
 *	DESCRIPTION	: Callback function called by ZZDP_APS_DATA_IND() when a ZDP 
 *		Power_Desc_req packet is received. 
 *		In this function, if needed, ASDU of response packet to be transmitted should be constructed.
 *
 * 	PARAMETER	:
 *		pReqAsdu - pointer to ASDU of received ZDP packet
 *		pRspAsdu - pointer to ASDU of response ZDP packet to be transmitted
 *
 * 	RETURN		: 
 *		0x00 - The response packet is not transmitted.
 *		Others - ASDU Length of response ZDP packet in bytes.
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
UINT8 ZZDP_Callback_For_PowerDescReq(UINT8 *pReqAsdu, UINT8 *pRspAsdu)
{
	UINT8	ZdpLen;
	UINT8	DevType;
	UINT8	MyChild;
	UINT8	DescAvail;
	UINT16	NwkAddrOfInterest;
	ZDPS_PAY	*pOutZDP;

	// Input ZDP Command : Power Descriptor Request	
	NwkAddrOfInterest = (pReqAsdu[2] << 8) | pReqAsdu[1];

	// Output ZDP Command : Power Descriptor Response	
	pOutZDP = (ZDPS_PAY *)pRspAsdu;
	pOutZDP->Seq = pReqAsdu[0];
	pOutZDP->Msg.POWER_DESC_RSP.Sta = zdpSTA_SUCCESS;
	pOutZDP->Msg.POWER_DESC_RSP.NwkAddr = NwkAddrOfInterest;
	Swap_Buffer_2B( (UINT8 *) &(pOutZDP->Msg.POWER_DESC_RSP.NwkAddr), 0);

	MyChild = 0;
	DescAvail = 0;
	
	ZdpLen = 4;
	if(NwkAddrOfInterest == NIB.nwkNetworkAddr)
	{
		pOutZDP->Msg.POWER_DESC_RSP.DescBuf[0] = 0x70;		// PowerDescriptor.AvailaPwrSrc_PwrMode
		pOutZDP->Msg.POWER_DESC_RSP.DescBuf[1] = 0xC1;		// PowerDescriptor.PwrLevel_CurrentPwrSrc
		ZdpLen += 2;
	}
	else
	{	
		DevType = UserConfig_DevType;
		if(DevType==0 || DevType==1)
		{
			// Check REQ.NwkAddr is MyChild ???			
			if(MyChild)
			{
				// Check Power Descriptor for MyChid is Available ???
				if(DescAvail)
				{
					// Copy Power Descriptor
				}
				else
				{
					pOutZDP->Msg.POWER_DESC_RSP.Sta = zdpSTA_NO_DESCRIPT;
				}
			}
			else
			{
				pOutZDP->Msg.POWER_DESC_RSP.Sta = zdpSTA_DEV_NOT_FOU;
			}
		}
		else
		{
			pOutZDP->Msg.POWER_DESC_RSP.Sta = zdpSTA_INV_REQTYPE;
		}
	}
	
	return	ZdpLen;
}

#endif
//============================================================









//============================================================
#if (UserConfig_Enable_ZDP_SimpDescReq)
//============================================================

/***********************************************************************************
 *
 *	NAME		: ZZDP_Send_SimpDescReq()
 *
 *	DESCRIPTION	: Transmits a ZDP Simple_Desc_req packet.
 *
 * 	PARAMETER	: 
 *		TargetNwkAddr - Network address of the target device
 *		TargetEP - Number of the target endpoint
 *
 * 	RETURN		: Status of ZAPS_DATA_REQ()
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/ 
UINT8 ZZDP_Send_SimpDescReq(UINT16 TargetNwkAddr, UINT8 TargetEP)
{
	APP_ASDU.Buff[0] = ++APP_DSN;			// APP_ASDU.ZDP.Seq
	APP_ASDU.Buff[1] = TargetNwkAddr;		// APP_ASDU.ZDP.Msg.SIMP_DESC_REQ.NwkAddr
	APP_ASDU.Buff[2] = TargetNwkAddr >> 8;	// APP_ASDU.ZDP.Msg.SIMP_DESC_REQ.NwkAddr
	APP_ASDU.Buff[3] = TargetEP;				// APP_ASDU.ZDP.Msg.SIMP_DESC_REQ.EP

	APP2APS.DATA_REQ.asduLen = 4;
	APP2APS.DATA_REQ.ClusID = aCID_Simp_Desc_Req;
	APP2APS.DATA_REQ.DstAdd_U.Short = TargetNwkAddr;
	APP2APS.DATA_REQ.DstEP = EndpointZDO;
	APP2APS.DATA_REQ.DstMode = 2;
	APP2APS.DATA_REQ.pasdu = APP_ASDU.Buff;
	APP2APS.DATA_REQ.ProfileID = ProfileZDP;
	APP2APS.DATA_REQ.Radius = NIB.nwkMaxDepth << 1;
	APP2APS.DATA_REQ.SrcEP = EndpointZDO;
	APP2APS.DATA_REQ.TxOption = 0x00;

	ZAPS_DATA_REQ(&APP2APS.DATA_REQ, &APS2APP.DATA_CON);

	return APS2APP.DATA_CON.Sta;
}

/***********************************************************************************
 *
 *	NAME		: ZZDP_Callback_For_SimpDescReq()
 *
 *	DESCRIPTION	: Callback function called by ZZDP_APS_DATA_IND() when a ZDP 
 *		Simple_Desc_req packet is received. 
 *		In this function, if needed, ASDU of response packet to be transmitted should be constructed.
 *
 * 	PARAMETER	:
 *		pReqAsdu - pointer to ASDU of received ZDP packet
 *		pRspAsdu - pointer to ASDU of response ZDP packet to be transmitted
 *
 * 	RETURN		: 
 *		0x00 - The response packet is not transmitted.
 *		Others - ASDU Length of response ZDP packet in bytes.
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
UINT8 ZZDP_Callback_For_SimpDescReq(UINT8 *pReqAsdu, UINT8 *pRspAsdu)
{
	UINT8	N;
	UINT8	k;
	ZDPS_PAY	*pInZDP;
	ZDPS_PAY	*pOutZDP;

	// Input ZDP Command : Simple Desc Request
	pInZDP = (ZDPS_PAY *)pReqAsdu;
	Swap_Buffer_2B( (UINT8 *) &(pInZDP->Msg.SIMP_DESC_REQ.NwkAddr), 0);

	// Output ZDP Command : Simple Desc Response
	pOutZDP = (ZDPS_PAY *)pRspAsdu;
	pOutZDP->Seq = pInZDP->Seq;
	pOutZDP->Msg.SIMP_DESC_RSP.Sta = zdpSTA_SUCCESS;
	pOutZDP->Msg.SIMP_DESC_RSP.NwkAddr = pInZDP->Msg.SIMP_DESC_REQ.NwkAddr;
	Swap_Buffer_2B( (UINT8 *) &(pOutZDP->Msg.SIMP_DESC_RSP.NwkAddr), 0);
	pOutZDP->Msg.SIMP_DESC_RSP.Length = 0;

	// Check ZDP Command
	if(pInZDP->Msg.SIMP_DESC_REQ.NwkAddr != NIB.nwkNetworkAddr)
	{
		pOutZDP->Msg.SIMP_DESC_RSP.Sta = zdpSTA_DEV_NOT_FOU;
		return	5;		
	}

	// Check Simple Descriptor
	if( (AppSimpDesc.EnaSD == 0) || (AppSimpDesc.EP != pInZDP->Msg.SIMP_DESC_REQ.EP) )
	{
		pOutZDP->Msg.SIMP_DESC_RSP.Sta = zdpSTA_INV_EP;
		return	5;		
	}

	// 
	// Descriptor of Response
	//
	N = 0;
	pOutZDP->Msg.SIMP_DESC_RSP.DescBuf[0] = AppSimpDesc.EP;
	pOutZDP->Msg.SIMP_DESC_RSP.DescBuf[1] = AppSimpDesc.AppProID;
	pOutZDP->Msg.SIMP_DESC_RSP.DescBuf[2] = AppSimpDesc.AppProID >> 8;
	pOutZDP->Msg.SIMP_DESC_RSP.DescBuf[3] = AppSimpDesc.AppDevID;
	pOutZDP->Msg.SIMP_DESC_RSP.DescBuf[4] = AppSimpDesc.AppDevID >> 8;
	pOutZDP->Msg.SIMP_DESC_RSP.DescBuf[5] = AppSimpDesc.DevVersion;
	N += 6;

	// Input Cluster
	pOutZDP->Msg.SIMP_DESC_RSP.DescBuf[N] = AppSimpDesc.NumInClus;
	N++;
	for(k=0 ; k<AppSimpDesc.NumInClus ; k++)
	{
		pOutZDP->Msg.SIMP_DESC_RSP.DescBuf[N+0] = AppSimpDesc.InClusList[k];
		pOutZDP->Msg.SIMP_DESC_RSP.DescBuf[N+1] = AppSimpDesc.InClusList[k] >> 8;
		N += 2;
	}

	// Output Cluster
	pOutZDP->Msg.SIMP_DESC_RSP.DescBuf[N] = AppSimpDesc.NumOutClus;
	N++;
	for(k=0 ; k<AppSimpDesc.NumOutClus ; k++)
	{
		pOutZDP->Msg.SIMP_DESC_RSP.DescBuf[N+0] = AppSimpDesc.OutClusList[k];
		pOutZDP->Msg.SIMP_DESC_RSP.DescBuf[N+1] = AppSimpDesc.OutClusList[k] >> 8;
		N += 2;
	}

	pOutZDP->Msg.SIMP_DESC_RSP.Length = N;
	
	return	(N+5);
}

#endif
//============================================================







//============================================================
#if (UserConfig_Enable_ZDP_ActiveEPReq)
//============================================================

/***********************************************************************************
 *
 *	NAME		: ZZDP_Send_ActiveEpReq()
 *
 *	DESCRIPTION	: Transmits a ZDP Active_EP_req packet.
 *
 * 	PARAMETER	: TargetNwkAddr - Network address of the target device
 *
 * 	RETURN		: Status of ZAPS_DATA_REQ()
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/ 
UINT8 ZZDP_Send_ActiveEpReq(UINT16 TargetNwkAddr)
{
	APP_ASDU.Buff[0] = ++APP_DSN;			// APP_ASDU.ZDP.Seq
	APP_ASDU.Buff[1] = TargetNwkAddr;		// APP_ASDU.ZDP.Msg.ACTIVE_EP_REQ.NwkAddr
	APP_ASDU.Buff[2] = TargetNwkAddr >> 8;	// APP_ASDU.ZDP.Msg.ACTIVE_EP_REQ.NwkAddr

	APP2APS.DATA_REQ.asduLen = 3;
	APP2APS.DATA_REQ.ClusID = aCID_Active_EP_Req;
	APP2APS.DATA_REQ.DstAdd_U.Short = TargetNwkAddr;
	APP2APS.DATA_REQ.DstEP = EndpointZDO;
	APP2APS.DATA_REQ.DstMode = 2;
	APP2APS.DATA_REQ.pasdu = APP_ASDU.Buff;
	APP2APS.DATA_REQ.ProfileID = ProfileZDP;
	APP2APS.DATA_REQ.Radius = NIB.nwkMaxDepth << 1;
	APP2APS.DATA_REQ.SrcEP = EndpointZDO;
	APP2APS.DATA_REQ.TxOption = 0x00;

	ZAPS_DATA_REQ(&APP2APS.DATA_REQ, &APS2APP.DATA_CON);

	return APS2APP.DATA_CON.Sta;
}

/***********************************************************************************
 *
 *	NAME		: ZZDP_Callback_For_ActiveEpReq()
 *
 *	DESCRIPTION	: Callback function called by ZZDP_APS_DATA_IND() when a ZDP 
 *		Active_EP_req packet is received. 
 *		In this function, if needed, ASDU of response packet to be transmitted should be constructed.
 *
 * 	PARAMETER	:
 *		pReqAsdu - pointer to ASDU of received ZDP packet
 *		pRspAsdu - pointer to ASDU of response ZDP packet to be transmitted
 *
 * 	RETURN		: 
 *		0x00 - The response packet is not transmitted.
 *		Others - ASDU Length of response ZDP packet in bytes.
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
UINT8 ZZDP_Callback_For_ActiveEpReq(UINT8 *pReqAsdu, UINT8 *pRspAsdu)
{
	UINT8	ZdpLen;
	UINT16	NwkAddrOfInterest;
	ZDPS_PAY	*pOutZDP;	

	// Input ZDP Command : Active EP Request
	NwkAddrOfInterest = (pReqAsdu[2] << 8) | pReqAsdu[1];

	// Output ZDP Command : Active EP Response	
	pOutZDP = (ZDPS_PAY *)pRspAsdu;	
	pOutZDP->Seq = pReqAsdu[0];
	pOutZDP->Msg.ACTIVE_EP_RSP.NwkAddr = NwkAddrOfInterest;
	Swap_Buffer_2B( (UINT8 *) &(pOutZDP->Msg.ACTIVE_EP_RSP.NwkAddr), 0);
	pOutZDP->Msg.ACTIVE_EP_RSP.ActiveEpCnt = 0;
	pOutZDP->Msg.ACTIVE_EP_RSP.Sta = zdpSTA_SUCCESS;	

	// (1) Check Cache list. In here, no cache and return ERROR
	if(NwkAddrOfInterest != NIB.nwkNetworkAddr)
	{		
		pOutZDP->Msg.ACTIVE_EP_RSP.Sta = zdpSTA_DEV_NOT_FOU;
		return	5;
	}

	if(AppSimpDesc.EnaSD)
	{
		pOutZDP->Msg.ACTIVE_EP_RSP.ActiveEpList[0] = AppSimpDesc.EP;
		pOutZDP->Msg.ACTIVE_EP_RSP.ActiveEpCnt = 1;
		ZdpLen = 6;
	}
	else
	{
		ZdpLen = 5;
	}
	
	return	ZdpLen;
}

#endif
//============================================================











//============================================================
#if (UserConfig_Enable_ZDP_MatchDescReq)
//============================================================

/***********************************************************************************
 *
 *	NAME		: ZZDP_Send_MatchDescReq()
 *
 *	DESCRIPTION	: Transmits a ZDP Match_Desc_req packet.
 *		This function supports only one input cluster and one output cluster, each. If clusters more 
 *		than 1 is needed, the routines should be modified.
 *
 * 	PARAMETER	: 
 *		TargetNwkAddr - Network address of the target device
 *		ProfID - Profile ID
 *		InClusID - Input Cluster ID 
 *		OutClusID - Output Cluster ID
 *
 * 	RETURN		: Status of ZAPS_DATA_REQ()
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/ 
UINT8 ZZDP_Send_MatchDescReq(UINT16 TargetNwkAddr, UINT16 ProfID, UINT16 InClusID, UINT16 OutClusID)
{
	UINT8	N;		// ASDU Length

	APP_ASDU.Buff[0] = ++APP_DSN;			// APP_ASDU.ZDP.Seq
	APP_ASDU.Buff[1] = TargetNwkAddr;		// APP_ASDU.ZDP.Msg.MATCH_DESC_REQ.NwkAddr
	APP_ASDU.Buff[2] = TargetNwkAddr >> 8;	// APP_ASDU.ZDP.Msg.MATCH_DESC_REQ.NwkAddr
	APP_ASDU.Buff[3] = ProfID;				// APP_ASDU.ZDP.Msg.MATCH_DESC_REQ.ProfileID
	APP_ASDU.Buff[4] = ProfID >> 8;			// APP_ASDU.ZDP.Msg.MATCH_DESC_REQ.ProfileID
	N = 5;
	if(InClusID == 0xFFFF)	// Invalid Input Cluster ID
	{
		APP_ASDU.Buff[N+0] = 0x00;			// APP_ASDU.ZDP.Msg.MATCH_DESC_REQ.NumInClus=0
		N += 1;
	}
	else
	{
		APP_ASDU.Buff[N+0] = 0x01;			// APP_ASDU.ZDP.Msg.MATCH_DESC_REQ.NumInClus=1
		APP_ASDU.Buff[N+1] = InClusID;		// APP_ASDU.ZDP.Msg.MATCH_DESC_REQ.InClusList[0]
		APP_ASDU.Buff[N+2] = InClusID >> 8;	// APP_ASDU.ZDP.Msg.MATCH_DESC_REQ.InClusList[0]		
		N += 3;
	}
	if(OutClusID == 0xFFFF)	// Invalid Output Cluster ID
	{
		APP_ASDU.Buff[N+0] = 0x00;			// APP_ASDU.ZDP.Msg.MATCH_DESC_REQ.NumOutClus=0
		N += 1;
	}
	else
	{
		APP_ASDU.Buff[N+0] = 0x01;			// APP_ASDU.ZDP.Msg.MATCH_DESC_REQ.NumOutClus=1
		APP_ASDU.Buff[N+1] = OutClusID;		// APP_ASDU.ZDP.Msg.MATCH_DESC_REQ.OutClusList[0]
		APP_ASDU.Buff[N+2] = OutClusID >> 8;	// APP_ASDU.ZDP.Msg.MATCH_DESC_REQ.OutClusList[0]		
		N += 3;
	}

	APP2APS.DATA_REQ.asduLen = N;
	APP2APS.DATA_REQ.ClusID = aCID_Match_Desc_Req;
	APP2APS.DATA_REQ.DstAdd_U.Short = TargetNwkAddr;
	APP2APS.DATA_REQ.DstEP = EndpointZDO;
	APP2APS.DATA_REQ.DstMode = 2;
	APP2APS.DATA_REQ.pasdu = APP_ASDU.Buff;
	APP2APS.DATA_REQ.ProfileID = ProfileZDP;
	APP2APS.DATA_REQ.Radius = NIB.nwkMaxDepth << 1;
	APP2APS.DATA_REQ.SrcEP = EndpointZDO;
	APP2APS.DATA_REQ.TxOption = 0x00;

	ZAPS_DATA_REQ(&APP2APS.DATA_REQ, &APS2APP.DATA_CON);

	return APS2APP.DATA_CON.Sta;
}

/***********************************************************************************
 *
 *	NAME		: ZZDP_Callback_For_MatchDescReq()
 *
 *	DESCRIPTION	: Callback function called by ZZDP_APS_DATA_IND() when a ZDP 
 *		Match_Desc_req packet is received. 
 *		In this function, if needed, ASDU of response packet to be transmitted should be constructed.
 *
 * 	PARAMETER	:
 *		pReqAsdu - pointer to ASDU of received ZDP packet
 *		pRspAsdu - pointer to ASDU of response ZDP packet to be transmitted
 *
 * 	RETURN		: 
 *		0x00 - The response packet is not transmitted.
 *		Others - ASDU Length of response ZDP packet in bytes.
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
UINT8 ZZDP_Callback_For_MatchDescReq(UINT8 *pReqAsdu, UINT8 *pRspAsdu)
{
	UINT8	ZdpLen;
	UINT16	ProfileID;
	UINT16	NwkAddrOfInterest;
	UINT8	InClusNum;
	UINT8	OutClusNum;
	UINT8	N1;
	UINT8	N2;
	UINT16	ClusterList;
	UINT8	IDX;

	UINT8	MatchOK;
	ZDPS_PAY	*pInZDP;
	ZDPS_PAY	*pOutZDP;	

	// Input ZDP Command : Match Desc Request
	pInZDP = (ZDPS_PAY *)pReqAsdu;
	NwkAddrOfInterest = (pReqAsdu[2] << 8) | pReqAsdu[1];
	ProfileID = (pReqAsdu[4] << 8) | pReqAsdu[3];
	InClusNum = pReqAsdu[5];
	OutClusNum = pReqAsdu[6+2*InClusNum];

	// Output ZDP Command : Match Desc Response	
	pOutZDP = (ZDPS_PAY *)pRspAsdu;
	pOutZDP->Seq = pInZDP->Seq;
	pOutZDP->Msg.MATCH_DESC_RSP.Sta = zdpSTA_SUCCESS;
	pOutZDP->Msg.MATCH_DESC_RSP.NwkAddr = NIB.nwkNetworkAddr;
	Swap_Buffer_2B( (UINT8 *) &(pOutZDP->Msg.MATCH_DESC_RSP.NwkAddr), 0);
	pOutZDP->Msg.MATCH_DESC_RSP.MatchLen = 0;
	ZdpLen = 5;

	if(	(NwkAddrOfInterest == NIB.nwkNetworkAddr) || (NwkAddrOfInterest == nBROAD_RXON) )
	{
		if(ProfileID == AppSimpDesc.AppProID)
		{		
			MatchOK = 0;

			// Check InClusterList
			InClusNum = pReqAsdu[5];
			IDX = 6;

			for(N1=0 ; N1<InClusNum ; N1++)
			{
				ClusterList = (pReqAsdu[IDX+1] << 8) | pReqAsdu[IDX];
				IDX += 2;
				for(N2=0 ; N2<AppSimpDesc.NumInClus ; N2++)
				{
					if(ClusterList == AppSimpDesc.InClusList[N2])		MatchOK = 1;
				}
			}			

			// Check OutClusterList
			OutClusNum = pReqAsdu[IDX];
			IDX++;

			for(N1=0 ; N1<OutClusNum ; N1++)
			{
				ClusterList = (pReqAsdu[IDX+1] << 8) | pReqAsdu[IDX];
				IDX += 2;
				for(N2=0 ; N2<AppSimpDesc.NumOutClus; N2++)
				{
					if(ClusterList == AppSimpDesc.OutClusList[N2])	MatchOK = 1;
				}
			}
						
			if(MatchOK)
			{
				pOutZDP->Msg.MATCH_DESC_RSP.MatchLen = 1;
				pOutZDP->Msg.MATCH_DESC_RSP.MatchList[0] = AppSimpDesc.EP;
				ZdpLen++;
			}			
		}
		else
		{
			pOutZDP->Msg.MATCH_DESC_RSP.Sta = zdpSTA_NO_DESCRIPT;
		}
	}
	else
	{
		pOutZDP->Msg.MATCH_DESC_RSP.Sta = zdpSTA_NOT_PERMIT;
	}
	
	return	ZdpLen;
}

#endif
//============================================================













//============================================================
#if (UserConfig_Enable_ZDP_SysServerReq)
//============================================================

/***********************************************************************************
 *
 *	NAME		: ZZDP_Send_SysServDiscReq()
 *
 *	DESCRIPTION	: Transmits a ZDP System_Server_Discovery_req packet.
 *
 * 	PARAMETER	: ServerMask Value of Node Descriptor
 *
 * 	RETURN		: Status of ZAPS_DATA_REQ()
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/ 
UINT8 ZZDP_Send_SysServDiscReq(UINT16 ServerMask)
{
	APP_ASDU.Buff[0] = ++APP_DSN;		// APP_ASDU.ZDP.Seq
	APP_ASDU.Buff[1] = ServerMask;		// APP_ASDU.ZDP.Msg.SYS_SERV_DISC_REQ.ServerMask
	APP_ASDU.Buff[2] = ServerMask >> 8;	// APP_ASDU.ZDP.Msg.SYS_SERV_DISC_REQ.ServerMask

	APP2APS.DATA_REQ.asduLen = 3;
	APP2APS.DATA_REQ.ClusID = aCID_Sys_Serv_Disc_Req;
	APP2APS.DATA_REQ.DstAdd_U.Short = nBROAD_RXON;;
	APP2APS.DATA_REQ.DstEP = EndpointZDO;
	APP2APS.DATA_REQ.DstMode = 2;
	APP2APS.DATA_REQ.pasdu = APP_ASDU.Buff;
	APP2APS.DATA_REQ.ProfileID = ProfileZDP;
	APP2APS.DATA_REQ.Radius = NIB.nwkMaxDepth << 1;
	APP2APS.DATA_REQ.SrcEP = EndpointZDO;
	APP2APS.DATA_REQ.TxOption = 0x00;

	ZAPS_DATA_REQ(&APP2APS.DATA_REQ, &APS2APP.DATA_CON);

	return APS2APP.DATA_CON.Sta;
}

/***********************************************************************************
 *
 *	NAME		: ZZDP_Callback_For_SysServDiscReq()
 *
 *	DESCRIPTION	: Callback function called by ZZDP_APS_DATA_IND() when a ZDP 
 *		System_Server_Discovery_req packet is received. 
 *		In this function, if needed, ASDU of response packet to be transmitted should be constructed.
 *
 * 	PARAMETER	:
 *		pReqAsdu - pointer to ASDU of received ZDP packet
 *		pRspAsdu - pointer to ASDU of response ZDP packet to be transmitted
 *
 * 	RETURN		: 
 *		0x00 - The response packet is not transmitted.
 *		Others - ASDU Length of response ZDP packet in bytes.
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
UINT8 ZZDP_Callback_For_SysServDiscReq(UINT8 *pReqAsdu, UINT8 *pRspAsdu)
{
	UINT16	CompareMask;
	UINT16	ServerMask;
	ZDPS_PAY	*pOutZDP;		

	// Input ZDP Command : System Server Discovery Request
	ServerMask = (pReqAsdu[2] << 8) | pReqAsdu[1];
	
	// Output ZDP Command : System Server Discovery Response
	pOutZDP = (ZDPS_PAY *)pRspAsdu;
	pOutZDP->Seq = pReqAsdu[0];

	CompareMask = UserConfig_ServerMask & ServerMask;
	if(CompareMask)
	{
		pOutZDP->Msg.SYS_SERV_DISC_RSP.Sta = zdpSTA_SUCCESS;
		pOutZDP->Msg.SYS_SERV_DISC_RSP.ServerMask = CompareMask;
		Swap_Buffer_2B( (UINT8 *) &(pOutZDP->Msg.SYS_SERV_DISC_RSP.ServerMask), 0);
		return	4;
	}
	else
	{
		return	0;
	}
}

#endif
//============================================================
















//============================================================
#if (UserConfig_Enable_ZDP_UserDescriptor)
//============================================================

UINT8 ZZDP_Send_UserDescReq(UINT16 TargetNwkAddr)
{
	APP_ASDU.Buff[0] = ++APP_DSN;			// APP_ASDU.ZDP.Seq
	APP_ASDU.Buff[1] = TargetNwkAddr;		// APP_ASDU.ZDP.Msg.USER_DESC_REQ.NwkAddr
	APP_ASDU.Buff[2] = TargetNwkAddr >> 8;	// APP_ASDU.ZDP.Msg.USER_DESC_REQ.NwkAddr

	APP2APS.DATA_REQ.asduLen = 3;
	APP2APS.DATA_REQ.ClusID = aCID_User_Desc_Req;
	APP2APS.DATA_REQ.DstAdd_U.Short = TargetNwkAddr;;
	APP2APS.DATA_REQ.DstEP = EndpointZDO;
	APP2APS.DATA_REQ.DstMode = 2;
	APP2APS.DATA_REQ.pasdu = APP_ASDU.Buff;
	APP2APS.DATA_REQ.ProfileID = ProfileZDP;
	APP2APS.DATA_REQ.Radius = NIB.nwkMaxDepth << 1;
	APP2APS.DATA_REQ.SrcEP = EndpointZDO;
	APP2APS.DATA_REQ.TxOption = 0x00;

	ZAPS_DATA_REQ(&APP2APS.DATA_REQ, &APS2APP.DATA_CON);

	return APS2APP.DATA_CON.Sta;
}


/***********************************************************************************
 *
 *	NAME		: ZZDP_Callback_For_UserDescReq()
 *
 *	DESCRIPTION	: Callback function called by ZZDP_APS_DATA_IND() when a ZDP 
 *		User_Desc_req packet is received. 
 *		In this function, if needed, ASDU of response packet to be transmitted should be constructed.
 *
 * 	PARAMETER	:
 *		pReqAsdu - pointer to ASDU of received ZDP packet
 *		pRspAsdu - pointer to ASDU of response ZDP packet to be transmitted
 *
 * 	RETURN		: 
 *		0x00 - The response packet is not transmitted.
 *		Others - ASDU Length of response ZDP packet in bytes.
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
UINT8 ZZDP_Callback_For_UserDescReq(UINT8 *pReqAsdu, UINT8 *pRspAsdu)
{
	UINT16	NwkAddrOfInterest;
	ZDPS_PAY	*pOutZDP;	

	// Input ZDP Command : User Desc Request
	NwkAddrOfInterest = (pReqAsdu[2] << 8) | pReqAsdu[1];

	// Output ZDP Command : User Desc Response
	pOutZDP = (ZDPS_PAY *)pRspAsdu;
	pOutZDP->Seq = pReqAsdu[0];
	pOutZDP->Msg.USER_DESC_RSP.Len = 16;
	pOutZDP->Msg.USER_DESC_RSP.NwkAddr = NwkAddrOfInterest;	
	Swap_Buffer_2B( (UINT8 *)&(pOutZDP->Msg.USER_DESC_RSP.NwkAddr), 0);	
	pOutZDP->Msg.USER_DESC_RSP.Sta = zdpSTA_SUCCESS;
	rpmemcpy(pOutZDP->Msg.USER_DESC_RSP.UserDesc, AppUserDescriptor, 16);

	return	21;
}


UINT8 ZZDP_Send_UserDescSet(UINT16 TargetNwkAddr, UINT8 *pUserDesc)
{
	APP_ASDU.Buff[0] = ++APP_DSN;				// APP_ASDU.ZDP.Seq
	APP_ASDU.Buff[1] = TargetNwkAddr;			// APP_ASDU.ZDP.Msg.USER_DESC_SET.NwkAddr
	APP_ASDU.Buff[2] = TargetNwkAddr >> 8;		// APP_ASDU.ZDP.Msg.USER_DESC_SET.NwkAddr
	APP_ASDU.Buff[3] = 16;						// APP_ASDU.ZDP.Msg.USER_DESC_SET.Len=16
	rpmemcpy(&APP_ASDU.Buff[4], pUserDesc, 16);	// APP_ASDU.ZDP.Msg.USER_DESC_SET.UserDesc

	APP2APS.DATA_REQ.asduLen = 20;
	APP2APS.DATA_REQ.ClusID = aCID_User_Desc_Set;
	APP2APS.DATA_REQ.DstAdd_U.Short = TargetNwkAddr;;
	APP2APS.DATA_REQ.DstEP = EndpointZDO;
	APP2APS.DATA_REQ.DstMode = 2;
	APP2APS.DATA_REQ.pasdu = APP_ASDU.Buff;
	APP2APS.DATA_REQ.ProfileID = ProfileZDP;
	APP2APS.DATA_REQ.Radius = NIB.nwkMaxDepth << 1;
	APP2APS.DATA_REQ.SrcEP = EndpointZDO;
	APP2APS.DATA_REQ.TxOption = 0x00;

	ZAPS_DATA_REQ(&APP2APS.DATA_REQ, &APS2APP.DATA_CON);

	return APS2APP.DATA_CON.Sta;
}


/***********************************************************************************
 *
 *	NAME		: ZZDP_Callback_For_UserDescSet()
 *
 *	DESCRIPTION	: Callback function called by ZZDP_APS_DATA_IND() when a ZDP 
 *		User_Desc_set packet is received. 
 *		In this function, if needed, ASDU of response packet to be transmitted should be constructed.
 *
 * 	PARAMETER	:
 *		pReqAsdu - pointer to ASDU of received ZDP packet
 *		pRspAsdu - pointer to ASDU of response ZDP packet to be transmitted
 *
 * 	RETURN		: 
 *		0x00 - The response packet is not transmitted.
 *		Others - ASDU Length of response ZDP packet in bytes.
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
UINT8 ZZDP_Callback_For_UserDescSet(UINT8 *pReqAsdu, UINT8 *pRspAsdu)
{
	ZDPS_PAY	*pInZDP;
	ZDPS_PAY	*pOutZDP;

	// Input ZDP Command : User Desc Set
	pInZDP = (ZDPS_PAY *)pReqAsdu;
	Swap_Buffer_2B( (UINT8 *)&(pInZDP->Msg.USER_DESC_SET.NwkAddr), 0);

	// Output ZDP Command : User Desc Configure
	pOutZDP = (ZDPS_PAY *)pRspAsdu;
	pOutZDP->Seq = pInZDP->Seq;
	pOutZDP->Msg.USER_DESC_CONF.NwkAddr = pInZDP->Msg.USER_DESC_SET.NwkAddr;
	Swap_Buffer_2B( (UINT8 *)&(pOutZDP->Msg.USER_DESC_CONF.NwkAddr), 0);
	
	if(pInZDP->Msg.USER_DESC_SET.Len > 16)
	{
		pOutZDP->Msg.USER_DESC_CONF.Sta = zdpSTA_INSUFF_SPACE;
	}
	else
	{
		pOutZDP->Msg.USER_DESC_CONF.Sta = zdpSTA_SUCCESS;
		rpmemcpy(AppUserDescriptor, &pReqAsdu[4], pInZDP->Msg.USER_DESC_SET.Len);
	}

	return 4;
}

#endif
//============================================================












//============================================================
#if (UserConfig_Enable_ZDP_CompDescReq)
//============================================================

/***********************************************************************************
 *
 *	NAME		: ZZDP_Send_CompDescReq()
 *
 *	DESCRIPTION	: Transmits a ZDP Comp_Desc_req packet.
 *
 * 	PARAMETER	: TargetNwkAddr - Network address of the target device
 *
 * 	RETURN		: Status of ZAPS_DATA_REQ()
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/ 
UINT8 ZZDP_Send_CompDescReq(UINT16 TargetNwkAddr)
{
	APP_ASDU.Buff[0] = ++APP_DSN;			// APP_ASDU.ZDP.Seq
	APP_ASDU.Buff[1] = TargetNwkAddr;		// APP_ASDU.ZDP.Msg.COMP_DESC_REQ.NwkAddr
	APP_ASDU.Buff[2] = TargetNwkAddr >> 8;	// APP_ASDU.ZDP.Msg.COMP_DESC_REQ.NwkAddr

	APP2APS.DATA_REQ.asduLen = 3;
	APP2APS.DATA_REQ.ClusID = aCID_Comp_Desc_Req;
	APP2APS.DATA_REQ.DstAdd_U.Short = TargetNwkAddr;;
	APP2APS.DATA_REQ.DstEP = EndpointZDO;
	APP2APS.DATA_REQ.DstMode = 2;
	APP2APS.DATA_REQ.pasdu = APP_ASDU.Buff;
	APP2APS.DATA_REQ.ProfileID = ProfileZDP;
	APP2APS.DATA_REQ.Radius = NIB.nwkMaxDepth << 1;
	APP2APS.DATA_REQ.SrcEP = EndpointZDO;
	APP2APS.DATA_REQ.TxOption = 0x00;

	ZAPS_DATA_REQ(&APP2APS.DATA_REQ, &APS2APP.DATA_CON);

	return APS2APP.DATA_CON.Sta;
}

/***********************************************************************************
 *
 *	NAME		: ZZDP_Callback_For_CompDescReq()
 *
 *	DESCRIPTION	: Callback function called by ZZDP_APS_DATA_IND() when a ZDP 
 *		Comp_Desc_req packet is received. 
 *		In this function, if needed, ASDU of response packet to be transmitted should be constructed.
 *
 * 	PARAMETER	:
 *		pReqAsdu - pointer to ASDU of received ZDP packet
 *		pRspAsdu - pointer to ASDU of response ZDP packet to be transmitted
 *
 * 	RETURN		: 
 *		0x00 - The response packet is not transmitted.
 *		Others - ASDU Length of response ZDP packet in bytes.
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/ 
UINT8 ZZDP_Callback_For_CompDescReq(UINT8 *pReqAsdu, UINT8 *pRspAsdu)
{
	UINT16	NwkAddrOfInterest;
	ZDPS_PAY	*pOutZDP;	

	// Input ZDP Command : Complex Descriptor Request
	NwkAddrOfInterest = (pReqAsdu[2] << 8) | pReqAsdu[1];

	// Input ZDP Command : Complex Descriptor Response
	pOutZDP = (ZDPS_PAY *)pRspAsdu;
	pOutZDP->Seq = pReqAsdu[0];
	pOutZDP->Msg.COMP_DESC_RSP.Sta = zdpSTA_NOT_SUPPORT;
	return 2;
}

#endif
//============================================================










//============================================================
#if (UserConfig_Enable_ZDP_NwkUpdateReq)
//============================================================

/***********************************************************************************
 *
 *	NAME		: ZZDP_Send_NwkUpdateReq_EDScan()
 *
 *	DESCRIPTION	: Transmits a ZDP Mgmt_Nwk_Update_req packet for ED Scan
 *
 * 	PARAMETER	: 
 *		ChanMask - bitmap of channels to be scanned
 *			+ bit[31:27] : reserved
 *			+ bit[26] : 1=Ch26 is scanned. 0=Not scanned.
 *			   ......
 *			+ bit[26] : 1=Ch11 is scanned. 0=Not scanned.
 *			+ bit[10:0] : reserved
 *		TargetNwkAddr - Network address of the target device
 *
 * 	RETURN		: Status of ZAPS_DATA_REQ()
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/ 
UINT8 ZZDP_Send_NwkUpdateReq_EDScan(UINT32 ChanMask, UINT16 TargetNwkAddr)
{
	UINT32	Tmp32;

	Tmp32 = ChanMask;

	APP_ASDU.Buff[0] = ++APP_DSN;			// APP_ASDU.ZDP.Seq	
	APP_ASDU.Buff[1] = Tmp32; Tmp32 >>= 8;	// APP_ASDU.ZDP.Msg.Mgmt_NwkUpdate_Req.ChanMask
	APP_ASDU.Buff[2] = Tmp32; Tmp32 >>= 8;	// APP_ASDU.ZDP.Msg.Mgmt_NwkUpdate_Req.ChanMask
	APP_ASDU.Buff[3] = Tmp32; Tmp32 >>= 8;	// APP_ASDU.ZDP.Msg.Mgmt_NwkUpdate_Req.ChanMask
	APP_ASDU.Buff[4] = Tmp32;				// APP_ASDU.ZDP.Msg.Mgmt_NwkUpdate_Req.ChanMask
	APP_ASDU.Buff[5] = 0x04;					// APP_ASDU.ZDP.Msg.Mgmt_NwkUpdate_Req.ScanDur=4
	APP_ASDU.Buff[6] = 0x01;					// APP_ASDU.ZDP.Msg.Mgmt_NwkUpdate_Req.ScanCount=1

	APP2APS.DATA_REQ.asduLen = 7;
	APP2APS.DATA_REQ.ClusID = aCID_Mgmt_Nwk_Update_Req;
	APP2APS.DATA_REQ.DstAdd_U.Short = TargetNwkAddr;;
	APP2APS.DATA_REQ.DstEP = EndpointZDO;
	APP2APS.DATA_REQ.DstMode = 2;
	APP2APS.DATA_REQ.pasdu = APP_ASDU.Buff;
	APP2APS.DATA_REQ.ProfileID = ProfileZDP;
	APP2APS.DATA_REQ.Radius = NIB.nwkMaxDepth << 1;
	APP2APS.DATA_REQ.SrcEP = EndpointZDO;
	APP2APS.DATA_REQ.TxOption = 0x00;

	ZAPS_DATA_REQ(&APP2APS.DATA_REQ, &APS2APP.DATA_CON);

	return APS2APP.DATA_CON.Sta;
}

/***********************************************************************************
 *
 *	NAME		: ZZDP_Send_NwkUpdateReq_ChangeChannel()
 *
 *	DESCRIPTION	: Transmits a ZDP Mgmt_Nwk_Update_req packet to change RF channel.
 *
 * 	PARAMETER	: NewChannel - New RF channel
 *
 * 	RETURN		: Status of ZAPS_DATA_REQ()
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
UINT8 ZZDP_Send_NwkUpdateReq_ChangeChannel(UINT8 NewChannel)
{
	UINT32	Tmp32;

	if(NewChannel > 0x1A)	return	0xFF;
	if(NewChannel < 0x0B)	return	0xFF;

	Tmp32 = 1;
	Tmp32 <<= NewChannel;

	APP_ASDU.Buff[0] = ++APP_DSN;			// APP_ASDU.ZDP.Seq	
	APP_ASDU.Buff[1] = Tmp32; Tmp32 >>= 8;	// APP_ASDU.ZDP.Msg.Mgmt_NwkUpdate_Req.ChanMask
	APP_ASDU.Buff[2] = Tmp32; Tmp32 >>= 8;	// APP_ASDU.ZDP.Msg.Mgmt_NwkUpdate_Req.ChanMask
	APP_ASDU.Buff[3] = Tmp32; Tmp32 >>= 8;	// APP_ASDU.ZDP.Msg.Mgmt_NwkUpdate_Req.ChanMask
	APP_ASDU.Buff[4] = Tmp32;				// APP_ASDU.ZDP.Msg.Mgmt_NwkUpdate_Req.ChanMask
	APP_ASDU.Buff[5] = 0xFE;					// APP_ASDU.ZDP.Msg.Mgmt_NwkUpdate_Req.ScanDur=0xFE
	APP_ASDU.Buff[6] = ++NIB.nwkUpdateId;		// APP_ASDU.ZDP.Msg.Mgmt_NwkUpdate_Req.NwkUpdateID

	APP2APS.DATA_REQ.asduLen = 7;
	APP2APS.DATA_REQ.ClusID = aCID_Mgmt_Nwk_Update_Req;
	APP2APS.DATA_REQ.DstAdd_U.Short = nBROAD_RXON;
	APP2APS.DATA_REQ.DstEP = EndpointZDO;
	APP2APS.DATA_REQ.DstMode = 2;
	APP2APS.DATA_REQ.pasdu = APP_ASDU.Buff;
	APP2APS.DATA_REQ.ProfileID = ProfileZDP;
	APP2APS.DATA_REQ.Radius = NIB.nwkMaxDepth << 1;
	APP2APS.DATA_REQ.SrcEP = EndpointZDO;
	APP2APS.DATA_REQ.TxOption = 0x00;

	ZAPS_DATA_REQ(&APP2APS.DATA_REQ, &APS2APP.DATA_CON);

	if(APS2APP.DATA_CON.Sta == aSTA_SUCCESS)
	{
		NIB.UpdateChan = NewChannel;
		ZSYS_SET_Q_TIMER(VirtualTimer_UpdateChan, NIB.nwkNetBroadDeliverTime, TIMER3_UNIT);
	}

	return APS2APP.DATA_CON.Sta;
}

/***********************************************************************************
 *
 *	NAME		: ZZDP_Send_NwkUpdateReq_NwkManager()
 *
 *	DESCRIPTION	: Transmits a ZDP Mgmt_Nwk_Update_req packet to change AIB.apsChannelMask,
 *		NIB.nwkUpateID and NIB.nwkManagerAddr of a remote device.
 *
 * 	PARAMETER	: 
 *		ChanMask - New AIB.apsChannelMask
 *		NwkManagerAddr - New NIB.nwkManagerAddr
 *
 * 	RETURN		: Status of ZAPS_DATA_REQ()
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
UINT8 ZZDP_Send_NwkUpdateReq_NwkManager(UINT32 ChanMask, UINT16 NwkManagerAddr)
{
	UINT32	Tmp32;

	Tmp32 = ChanMask;

	APP_ASDU.Buff[0] = ++APP_DSN;			// APP_ASDU.ZDP.Seq	
	APP_ASDU.Buff[1] = Tmp32; Tmp32 >>= 8;	// APP_ASDU.ZDP.Msg.Mgmt_NwkUpdate_Req.ChanMask
	APP_ASDU.Buff[2] = Tmp32; Tmp32 >>= 8;	// APP_ASDU.ZDP.Msg.Mgmt_NwkUpdate_Req.ChanMask
	APP_ASDU.Buff[3] = Tmp32; Tmp32 >>= 8;	// APP_ASDU.ZDP.Msg.Mgmt_NwkUpdate_Req.ChanMask
	APP_ASDU.Buff[4] = Tmp32;				// APP_ASDU.ZDP.Msg.Mgmt_NwkUpdate_Req.ChanMask
	APP_ASDU.Buff[5] = 0xFF;					// APP_ASDU.ZDP.Msg.Mgmt_NwkUpdate_Req.ScanDur=0xFF
	APP_ASDU.Buff[6] = ++NIB.nwkUpdateId;		// APP_ASDU.ZDP.Msg.Mgmt_NwkUpdate_Req.NwkUpdateID
	APP_ASDU.Buff[7] = NwkManagerAddr;		// APP_ASDU.ZDP.Msg.Mgmt_NwkUpdate_Req.NwkManagerAddr
	APP_ASDU.Buff[8] = NwkManagerAddr >> 8;	// APP_ASDU.ZDP.Msg.Mgmt_NwkUpdate_Req.NwkManagerAddr
	
	APP2APS.DATA_REQ.asduLen = 9;
	APP2APS.DATA_REQ.ClusID = aCID_Mgmt_Nwk_Update_Req;
	APP2APS.DATA_REQ.DstAdd_U.Short = nBROAD_RXON;
	APP2APS.DATA_REQ.DstEP = EndpointZDO;
	APP2APS.DATA_REQ.DstMode = 2;
	APP2APS.DATA_REQ.pasdu = APP_ASDU.Buff;
	APP2APS.DATA_REQ.ProfileID = ProfileZDP;
	APP2APS.DATA_REQ.Radius = NIB.nwkMaxDepth << 1;
	APP2APS.DATA_REQ.SrcEP = EndpointZDO;
	APP2APS.DATA_REQ.TxOption = 0x00;

	ZAPS_DATA_REQ(&APP2APS.DATA_REQ, &APS2APP.DATA_CON);

	return APS2APP.DATA_CON.Sta;
}

/***********************************************************************************
 *
 *	NAME		: ZZDP_Callback_For_NwkUpdateReq()
 *
 *	DESCRIPTION	: Callback function called by ZZDP_APS_DATA_IND() when a ZDP 
 *		Mgmt_Nwk_Update_req packet is received. 
 *		In this function, if needed, ASDU of response packet to be transmitted should be constructed.
 *
 * 	PARAMETER	:
 *		pReqAsdu - pointer to ASDU of received ZDP packet
 *		pRspAsdu - pointer to ASDU of response ZDP packet to be transmitted
 *
 * 	RETURN		: 
 *		0x00 - The response packet is not transmitted.
 *		Others - ASDU Length of response ZDP packet in bytes.
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
UINT8 ZZDP_Callback_For_NwkUpdateReq(APSDE_DATA_ind *pInd, UINT8 *pRspAsdu)
{
	UINT32	ChanMask;
	UINT32	Cmp32;
	UINT8	ChanNum;
	UINT8	N2;
	UINT8	UnicastRx;
	UINT8	ScanCount;
	UINT8	ScanDur;
	ZDPS_PAY	*pOutZDP;	
	
	UnicastRx = (pInd->DstAdd_U.Short < nBROAD_LOWEST)? 1 : 0;

	ChanMask = pInd->pasdu[4];	ChanMask <<= 8;
	ChanMask |= pInd->pasdu[3];	ChanMask <<= 8;
	ChanMask |= pInd->pasdu[2];	ChanMask <<= 8;
	ChanMask |= pInd->pasdu[1];
	ScanDur = pInd->pasdu[5];
	
	pOutZDP = (ZDPS_PAY *)pRspAsdu;
	pOutZDP->Seq = pInd->pasdu[0];
	pOutZDP->Msg.Mgmt_NwkUpdate_Noti.Sta = zdpSTA_SUCCESS;
	pOutZDP->Msg.Mgmt_NwkUpdate_Noti.ScannedChan = 0;
	pOutZDP->Msg.Mgmt_NwkUpdate_Noti.TotalTrans = 0;		// Swap_Buffer_2B
	pOutZDP->Msg.Mgmt_NwkUpdate_Noti.TransFail = 0;		// Swap_Buffer_2B
	pOutZDP->Msg.Mgmt_NwkUpdate_Noti.ScannedChanCount = 0;

	if(ScanDur < 6)		// ED Scan
	{
		ScanCount = pInd->pasdu[6];
		if(UnicastRx)
		{
			APS2NWK.ED_SCAN_REQ.ScanChan = ChanMask;
			APS2NWK.ED_SCAN_REQ.ScanDur = pInd->pasdu[5];

			ZNWK_ED_SCAN_REQ(&APS2NWK.ED_SCAN_REQ, &NWK2APS.ED_SCAN_CON);
			pOutZDP->Msg.Mgmt_NwkUpdate_Noti.ScannedChan = ChanMask;
			Swap_Buffer_4B( (UINT8 *)&(pOutZDP->Msg.Mgmt_NwkUpdate_Noti.ScannedChan), 0);
			pOutZDP->Msg.Mgmt_NwkUpdate_Noti.Sta = NWK2APS.ED_SCAN_CON.Sta;
			
			N2 = 0;
			for(ChanNum=11 ; ChanNum < 27 ; ChanNum++)
			{
				Cmp32 = 1;
				Cmp32 <<= ChanNum;
				if(Cmp32 & ChanMask)
				{
					pOutZDP->Msg.Mgmt_NwkUpdate_Noti.EnergyValue[N2] = NWK2APS.ED_SCAN_CON.EnergyDetectList[ChanNum-11];
					N2++;
				}
			}
			pOutZDP->Msg.Mgmt_NwkUpdate_Noti.ScannedChanCount = N2;

			return	(11 + N2);
		}
		else
		{
			return	0;
		}
	}
	
	else if(ScanDur == 0xFE)		// Changing Channel
	{	
		for(ChanNum=11 ; ChanNum<27 ; ChanNum++)
		{
			Cmp32 = 1;
			Cmp32 <<= ChanNum;
			if(Cmp32 & ChanMask)		break;
		}
		if(ChanNum == 27)	// Not matched ChanMask
		{
			if(UnicastRx)	return 0;
			else	
			{
				pOutZDP->Msg.Mgmt_NwkUpdate_Noti.Sta = zdpSTA_INV_REQTYPE;
				return 11;
			}
		}
		else
		{
			NIB.nwkUpdateId = pInd->pasdu[6];	// New nwkUpdateId	
			NIB.UpdateChan = ChanNum;		// New Channel

			// Set Timer for Changing Channel
			ZSYS_SET_Q_TIMER(VirtualTimer_UpdateChan, NIB.nwkNetBroadDeliverTime, TIMER3_UNIT);
			return	0;
		}		
	}

	
	else if(ScanDur == 0xFF)		// Update AIB.apsChannelMask, NIB.nwkUpdateID, NIB.nwkManagerAddr
	{
		AIB.apsChannelMask = ChanMask;
		NIB.nwkUpdateId = pInd->pasdu[6];
		NIB.nwkManagerAddr = (pInd->pasdu[8] << 8) | pInd->pasdu[7];
		return	0;
	}	
	else			// ScanDur = 6 ~ 0xFD
	{
		if(UnicastRx)
		{
			pOutZDP->Msg.Mgmt_NwkUpdate_Noti.Sta = zdpSTA_INV_REQTYPE;
			return 11;
		}
		else
		{
			return 0;
		}		
	}
}

#endif
//============================================================


//============================================================
#if (UserConfig_Enable_ZDP_LQIReq)
//============================================================

/***********************************************************************************
 *
 *	NAME		: ZZDP_Send_LQIReq()
 *
 *	DESCRIPTION	: Transmits a ZDP Mgmt_LQI_req packet to obtain a neighbor list for the remote 
 *		device along with associated LQI values to each neighbor.
 *
 * 	PARAMETER	: 
 *		StartIndex - Starting index for the requested elements of the neighbor table.
 *		TargetNwkAddr - Network address of the target device.
 *
 * 	RETURN		: Status of ZAPS_DATA_REQ()
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
UINT8 ZZDP_Send_LQIReq(UINT8 StartIndex, UINT16 TargetNwkAddr)
{
	APP_ASDU.Buff[0] = ++APP_DSN;			// APP_ASDU.ZDP.Seq
	APP_ASDU.Buff[1] = StartIndex;			// APP_ASDU.ZDP.Msg.Mgmt_LQI_REQ.StartIndex

	APP2APS.DATA_REQ.asduLen = 2;
	APP2APS.DATA_REQ.ClusID = aCID_Mgmt_LQI_Req;
	APP2APS.DATA_REQ.DstAdd_U.Short = TargetNwkAddr;
	APP2APS.DATA_REQ.DstEP = EndpointZDO;
	APP2APS.DATA_REQ.DstMode = 2;
	APP2APS.DATA_REQ.pasdu = APP_ASDU.Buff;
	APP2APS.DATA_REQ.ProfileID = ProfileZDP;
	APP2APS.DATA_REQ.Radius = NIB.nwkMaxDepth << 1;
	APP2APS.DATA_REQ.SrcEP = EndpointZDO;
	APP2APS.DATA_REQ.TxOption = 0x00;

	ZAPS_DATA_REQ(&APP2APS.DATA_REQ, &APS2APP.DATA_CON);

	return APS2APP.DATA_CON.Sta;
}

/***********************************************************************************
 *
 *	NAME		: ZZDP_Callback_For_LQIReq()
 *
 *	DESCRIPTION	: Callback function called by ZZDP_APS_DATA_IND() when a ZDP 
 *		Mgmt_LQI_req packet is received. 
 *		In this function, if needed, ASDU of response packet to be transmitted should be constructed.
 *
 * 	PARAMETER	:
 *		pReqAsdu - pointer to ASDU of received ZDP packet
 *		pRspAsdu - pointer to ASDU of response ZDP packet to be transmitted
 *
 * 	RETURN		: 
 *		0x00 - The response packet is not transmitted.
 *		Others - ASDU Length of response ZDP packet in bytes.
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
UINT8 ZZDP_Callback_For_LQIReq(UINT8 *pReqAsdu, UINT8 *pRspAsdu)
{
	ZDPS_PAY	*pOutZDP;
	UINT8		StartIdx;
	UINT8		N;
	UINT8		RspAsduLen;

	// Input ZDP Command : LQI Request
	StartIdx = pReqAsdu[1];

	// Output ZDP Command : LQI Response
	pOutZDP = (ZDPS_PAY *)pRspAsdu;
	pOutZDP->Seq = pReqAsdu[0];
	pOutZDP->Msg.Mgmt_LQI_RSP.Sta = zdpSTA_SUCCESS;	
	pOutZDP->Msg.Mgmt_LQI_RSP.StartIndex = StartIdx;	
	pOutZDP->Msg.Mgmt_LQI_RSP.NTEntries = 0;
	pOutZDP->Msg.Mgmt_LQI_RSP.NTListCnt = 0;
	RspAsduLen = 5;
	for(N=0 ; N<NIB.SIZE_NT ; N++)
	{
		if(NIB.pNT[N].EnaNT)
		{
			pOutZDP->Msg.Mgmt_LQI_RSP.NTEntries++;

			if(pOutZDP->Msg.Mgmt_LQI_RSP.NTListCnt < 2)
			{
				if(N >= StartIdx)
				{
					rpmemcpy(&pRspAsdu[RspAsduLen+0], NIB.nwkEPID, 8);
					rpmemcpy(&pRspAsdu[RspAsduLen+8], NIB.pNT[N].IEEEAddr, 8);
					pRspAsdu[RspAsduLen+16] = NIB.pNT[N].NwkAddr;
					pRspAsdu[RspAsduLen+17] = NIB.pNT[N].NwkAddr >> 8;
					pRspAsdu[RspAsduLen+18] = (NIB.pNT[N].DevType & 0x03);
					pRspAsdu[RspAsduLen+18] |= (NIB.pNT[N].RxOnWhenIdle & 0x01) << 2;
					pRspAsdu[RspAsduLen+18] |= (NIB.pNT[N].Relation & 0x07) << 4;					
					pRspAsdu[RspAsduLen+19] = 0x02;		// PermitJoining=Unknown
					pRspAsdu[RspAsduLen+20] = 0xFF;		// unknown
					pRspAsdu[RspAsduLen+21] = NIB.pNT[N].NtLQI;
					RspAsduLen += 22;
				
					pOutZDP->Msg.Mgmt_LQI_RSP.NTListCnt++;
				}
			}			
		}
		else
		{
			break;
		}
	}
	
	return	RspAsduLen;
}
#endif
//============================================================







//============================================================
#if (UserConfig_Enable_ZDP_RtgReq)
//============================================================

/***********************************************************************************
 *
 *	NAME		: ZZDP_Send_RtgReq()
 *
 *	DESCRIPTION	: Transmits a ZDP Mgmt_Rtg_req packet to get routing table from a remote device.
 *
 * 	PARAMETER	: 
 *		StartIndex - Starting index for the requested elements of the routing table.
 *		TargetNwkAddr - Network address of the target device.
 *
 * 	RETURN		: Status of ZAPS_DATA_REQ()
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
UINT8 ZZDP_Send_RtgReq(UINT8 StartIndex, UINT16 TargetNwkAddr)
{
	APP_ASDU.Buff[0] = ++APP_DSN;			// APP_ASDU.ZDP.Seq
	APP_ASDU.Buff[1] = StartIndex;			// APP_ASDU.ZDP.Msg.Mgmt_Rtg_REQ.StartIndex

	APP2APS.DATA_REQ.asduLen = 2;
	APP2APS.DATA_REQ.ClusID = aCID_Mgmt_Rtg_Req;
	APP2APS.DATA_REQ.DstAdd_U.Short = TargetNwkAddr;
	APP2APS.DATA_REQ.DstEP = EndpointZDO;
	APP2APS.DATA_REQ.DstMode = 2;
	APP2APS.DATA_REQ.pasdu = APP_ASDU.Buff;
	APP2APS.DATA_REQ.ProfileID = ProfileZDP;
	APP2APS.DATA_REQ.Radius = NIB.nwkMaxDepth << 1;
	APP2APS.DATA_REQ.SrcEP = EndpointZDO;
	APP2APS.DATA_REQ.TxOption = 0x00;

	ZAPS_DATA_REQ(&APP2APS.DATA_REQ, &APS2APP.DATA_CON);

	return APS2APP.DATA_CON.Sta;
}

/***********************************************************************************
 *
 *	NAME		: ZZDP_Callback_For_RtgReq()
 *
 *	DESCRIPTION	: Callback function called by ZZDP_APS_DATA_IND() when a ZDP 
 *		Mgmt_Rtg_req packet is received. 
 *		In this function, if needed, ASDU of response packet to be transmitted should be constructed.
 *
 * 	PARAMETER	:
 *		pReqAsdu - pointer to ASDU of received ZDP packet
 *		pRspAsdu - pointer to ASDU of response ZDP packet to be transmitted
 *
 * 	RETURN		: 
 *		0x00 - The response packet is not transmitted.
 *		Others - ASDU Length of response ZDP packet in bytes.
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
UINT8 ZZDP_Callback_For_RtgReq(UINT8 *pReqAsdu, UINT8 *pRspAsdu)
{
	ZDPS_PAY	*pOutZDP;
	UINT8		StartIdx;
	UINT8		N;
	UINT8		RspAsduLen;

	// Input ZDP Command : Rtg Request
	StartIdx = pReqAsdu[1];

	// Output ZDP Command : Rtg Response
	pOutZDP = (ZDPS_PAY *)pRspAsdu;
	pOutZDP->Seq = pReqAsdu[0];	
	pOutZDP->Msg.Mgmt_Rtg_RSP.Sta = zdpSTA_SUCCESS;
	pOutZDP->Msg.Mgmt_Rtg_RSP.RTEntries = 0;
	pOutZDP->Msg.Mgmt_Rtg_RSP.StartIndex = StartIdx;
	pOutZDP->Msg.Mgmt_Rtg_RSP.RTListCnt = 0;
	RspAsduLen = 5;

	for(N=0 ; N<NIB.SIZE_RT ; N++)
	{
		if(NIB.pRT[N].EnaRT)
		{
			pOutZDP->Msg.Mgmt_Rtg_RSP.RTEntries++;
			if(RspAsduLen <= (apscMaxASDU - 5) )
			{
				if(N >= StartIdx)
				{
					pOutZDP->Msg.Mgmt_Rtg_RSP.RTListCnt++;
					
					pRspAsdu[RspAsduLen+0] = NIB.pRT[N].DstAddr;
					pRspAsdu[RspAsduLen+1] = NIB.pRT[N].DstAddr >> 8;
					pRspAsdu[RspAsduLen+2] = NIB.pRT[N].Sta & 0x07;
					if(NIB.pRT[N].RtOption & 0x40)	pRspAsdu[RspAsduLen+2] |= 0x08;	// Memory Constrained
					if(NIB.pRT[N].RtOption & 0x08)	pRspAsdu[RspAsduLen+2] |= 0x10;	/// Many-To-One
					if(NIB.pRT[N].RtOption & 0x10)	pRspAsdu[RspAsduLen+2] |= 0x20;	// Route Record Required
					pRspAsdu[RspAsduLen+3] = NIB.pRT[N].NextHop;
					pRspAsdu[RspAsduLen+4] = NIB.pRT[N].NextHop >> 8;
					RspAsduLen += 5;
				}				
			}	
		}
	}
	
	return	RspAsduLen;
}

#endif
//============================================================


//============================================================
#if (UserConfig_Enable_ZDP_LeaveReq)
//============================================================

/***********************************************************************************
 *
 *	NAME		: ZZDP_Send_LeaveReq()
 *
 *	DESCRIPTION	: Transmits a ZDP Mgmt_Leave_req packet to request that a remote device leave
 *		the network or to request that that another device leave the network.
 *
 * 	PARAMETER	: 
 *		pDevIEEEAddr - Pointer to IEEE address of the device to be left.
 *		RmChild - 1=Remove child, 0=Not.
 *		Rejoin - 1=Rejoin, 0=Not.
 *		TargetNwkAddr - Network address of the target device.
 *
 * 	RETURN		: Status of ZAPS_DATA_REQ()
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
UINT8 ZZDP_Send_LeaveReq(UINT8 *pDevIEEEAddr, UINT8 RmChild, UINT8 Rejoin, UINT16 TargetNwkAddr)
{
	APP_ASDU.Buff[0] = ++APP_DSN;					// APP_ASDU.ZDP.Seq
	rpmemcpy(&APP_ASDU.Buff[1], pDevIEEEAddr, 8);	// APP_ASDU.ZDP.Msg.Mgmt_Leave_REQ.DevIEEEAddr	
	APP_ASDU.Buff[9] = 0x00;							// APP_ASDU.ZDP.Msg.Mgmt_Leave_REQ.LeaveOption
	if(RmChild)	APP_ASDU.Buff[9] |= 0x02;
	if(Rejoin)	APP_ASDU.Buff[9] |= 0x01;
	
	APP2APS.DATA_REQ.asduLen = 10;
	APP2APS.DATA_REQ.ClusID = aCID_Mgmt_Leave_Req;
	
	APP2APS.DATA_REQ.DstAdd_U.Short = TargetNwkAddr;
	APP2APS.DATA_REQ.DstEP = EndpointZDO;
	APP2APS.DATA_REQ.DstMode = 2;
	APP2APS.DATA_REQ.pasdu = APP_ASDU.Buff;
	APP2APS.DATA_REQ.ProfileID = ProfileZDP;
	APP2APS.DATA_REQ.Radius = NIB.nwkMaxDepth << 1;
	APP2APS.DATA_REQ.SrcEP = EndpointZDO;
	APP2APS.DATA_REQ.TxOption = 0x00;

	ZAPS_DATA_REQ(&APP2APS.DATA_REQ, &APS2APP.DATA_CON);

	return APS2APP.DATA_CON.Sta;
}

/***********************************************************************************
 *
 *	NAME		: ZZDP_Callback_For_LeaveReq()
 *
 *	DESCRIPTION	: Callback function called by ZZDP_APS_DATA_IND() when a ZDP 
 *		Mgmt_Leave_req packet is received. 
 *		In this function, if needed, ASDU of response packet to be transmitted should be constructed.
 *
 * 	PARAMETER	:
 *		pReqAsdu - pointer to ASDU of received ZDP packet
 *		pRspAsdu - pointer to ASDU of response ZDP packet to be transmitted
 *
 * 	RETURN		: 
 *		0x00 - The response packet is not transmitted.
 *		Others - ASDU Length of response ZDP packet in bytes.
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
UINT8 ZZDP_Callback_For_LeaveReq(UINT8 *pReqAsdu, UINT8 *pRspAsdu)
{
	ZDPS_PAY	*pInZDP;
	ZDPS_PAY	*pOutZDP;

	// Input ZDP Command : Leave Request
	pInZDP = (ZDPS_PAY *)pReqAsdu;

	rpmemcpy(APS2NWK.LEAVE_REQ.DevAddr, pInZDP->Msg.Mgmt_Leave_REQ.DevIEEEAddr, 8);
	APS2NWK.LEAVE_REQ.RmChild = (pInZDP->Msg.Mgmt_Leave_REQ.LeaveOption & 0x02)? 1 : 0;
	APS2NWK.LEAVE_REQ.Rejoin = (pInZDP->Msg.Mgmt_Leave_REQ.LeaveOption & 0x01)? 1 : 0;
	ZNWK_LEAVE_REQ(&APS2NWK.LEAVE_REQ, &NWK2APS.LEAVE_CON);

	// Output ZDP Command : Leave Response
	pOutZDP = (ZDPS_PAY *)pRspAsdu;
	pOutZDP->Seq = pInZDP->Seq;
	pOutZDP->Msg.Mgmt_Leave_RSP.Sta = NWK2APS.LEAVE_CON.Sta;

	return	2;
}

#endif
//============================================================




//============================================================
#if (UserConfig_Enable_ZDP_PermitJoinReq)
//============================================================

/***********************************************************************************
 *
 *	NAME		: ZZDP_Send_PermitJoinReq()
 *
 *	DESCRIPTION	: Transmits a ZDP Mgmt_Permit_Join_req packet to request that a remote device
 *		allow or disallow association.
 *
 * 	PARAMETER	: 
 *		PermitDur - Permit Duration.
 *		TC_Sign - 1=Trust Center, 0=Not.
 *		TargetNwkAddr - Network address of the target device.
 *
 * 	RETURN		: Status of ZAPS_DATA_REQ()
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
UINT8 ZZDP_Send_PermitJoinReq(UINT8 PermitDur, UINT8 TC_Sign, UINT16 TargetNwkAddr)
{	
	APP_ASDU.Buff[0] = ++APP_DSN;	// APP_ASDU.ZDP.Seq
	APP_ASDU.Buff[1] = PermitDur;		// APP_ASDU.ZDP.Msg.Mgmt_PermitJoin_REQ.PermitDur
	APP_ASDU.Buff[2] = TC_Sign;		// APP_ASDU.ZDP.Msg.Mgmt_PermitJoin_REQ.TC_Sign
	
	APP2APS.DATA_REQ.asduLen = 3;
	APP2APS.DATA_REQ.ClusID = aCID_Mgmt_Permit_Join_Req;
	
	APP2APS.DATA_REQ.DstAdd_U.Short = TargetNwkAddr;
	APP2APS.DATA_REQ.DstEP = EndpointZDO;
	APP2APS.DATA_REQ.DstMode = 2;
	APP2APS.DATA_REQ.pasdu = APP_ASDU.Buff;
	APP2APS.DATA_REQ.ProfileID = ProfileZDP;
	APP2APS.DATA_REQ.Radius = NIB.nwkMaxDepth << 1;
	APP2APS.DATA_REQ.SrcEP = EndpointZDO;
	APP2APS.DATA_REQ.TxOption = 0x00;

	ZAPS_DATA_REQ(&APP2APS.DATA_REQ, &APS2APP.DATA_CON);

	return APS2APP.DATA_CON.Sta;
}

/***********************************************************************************
 *
 *	NAME		: ZZDP_Callback_For_PermitJoinReq()
 *
 *	DESCRIPTION	: Callback function called by ZZDP_APS_DATA_IND() when a ZDP 
 *		Mgmt_Permit_Join_req packet is received. 
 *		In this function, if needed, ASDU of response packet to be transmitted should be constructed.
 *
 * 	PARAMETER	:
 *		pReqAsdu - pointer to ASDU of received ZDP packet
 *		pRspAsdu - pointer to ASDU of response ZDP packet to be transmitted
 *
 * 	RETURN		: 
 *		0x00 - The response packet is not transmitted.
 *		Others - ASDU Length of response ZDP packet in bytes.
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
UINT8 ZZDP_Callback_For_PermitJoinReq(UINT8 *pReqAsdu, UINT8 *pRspAsdu)
{
	ZDPS_PAY	*pInZDP;
	ZDPS_PAY	*pOutZDP;

	// Input ZDP Command : Permit Join Request
	pInZDP = (ZDPS_PAY *)pReqAsdu;	

	APS2NWK.PERMIT_JOIN_REQ.PermitDur = pInZDP->Msg.Mgmt_PermitJoin_REQ.PermitDur;
	ZNWK_PERMIT_JOIN_REQ(&APS2NWK.PERMIT_JOIN_REQ, &NWK2APS.PERMIT_JOIN_CON);

	// Output ZDP Command : Permit Join Response
	pOutZDP = (ZDPS_PAY *)pRspAsdu;
	pOutZDP->Seq = pInZDP->Seq;
	pOutZDP->Msg.Mgmt_PermitJoin_RSP.Sta = NWK2APS.PERMIT_JOIN_CON.Sta;

	return	2;
}

#endif
//============================================================


/***********************************************************************************
 *
 *	NAME		: ZZDP_APS_DATA_IND()
 *
 *	DESCRIPTION	: Callback function called by the stack when an APS Data packet, whose
 *		destination Endpoint is 0x00, is received.
 *
 * 	PARAMETER	: pInd - pointer to APS Data packet(APSDE_DATA_ind)
 *
 * 	RETURN		: None
 *
 * 	NOTES		: If the destination Endpoint of the received packet is not 0x00, 
 *		ZUSER_APS_DATA_IND() is called by the stack.
 *	
 ***********************************************************************************/
void ZZDP_APS_DATA_IND(APSDE_DATA_ind *pInd)
{
	UINT8	EnableRsp;

	#if (AppOption_ProfileSimulator)
		UartOut.PKT.Arguments[0] = pInd->DstMode;
		UartOut.PKT.Arguments[1] = pInd->DstAdd_U.Short;
		UartOut.PKT.Arguments[2] = pInd->DstAdd_U.Short >> 8;
		UartOut.PKT.Arguments[3] = 0x00;
		UartOut.PKT.Arguments[4] = 0x00;
		UartOut.PKT.Arguments[5] = 0x00;
		UartOut.PKT.Arguments[6] = 0x00;
		UartOut.PKT.Arguments[7] = 0x00;
		UartOut.PKT.Arguments[8] = 0x00;
		UartOut.PKT.Arguments[9] = pInd->DstEP;
		UartOut.PKT.Arguments[10] = pInd->SrcMode;
		if(pInd->SrcMode == 2)
		{
			UartOut.PKT.Arguments[11] = pInd->SrcAdd_U.Short;
			UartOut.PKT.Arguments[12] = pInd->SrcAdd_U.Short >> 8;
			UartOut.PKT.Arguments[13] = 0x00;
			UartOut.PKT.Arguments[14] = 0x00;
			UartOut.PKT.Arguments[15] = 0x00;
			UartOut.PKT.Arguments[16] = 0x00;
			UartOut.PKT.Arguments[17] = 0x00;
			UartOut.PKT.Arguments[18] = 0x00;
		}
		else
		{
			rpmemcpy(&UartOut.PKT.Arguments[11], pInd->SrcAdd_U.Ext, 8);
		}		
		UartOut.PKT.Arguments[19] = pInd->SrcEP;
		UartOut.PKT.Arguments[20] = pInd->ProfileID;
		UartOut.PKT.Arguments[21] = pInd->ProfileID >> 8;
		UartOut.PKT.Arguments[22] = pInd->ClusID;
		UartOut.PKT.Arguments[23] = pInd->ClusID >> 8;
		UartOut.PKT.Arguments[24] = pInd->asduLen & 0x007F;
		UartOut.PKT.Arguments[25] = pInd->Sta;
		UartOut.PKT.Arguments[26] = pInd->SecSta;
		UartOut.PKT.Arguments[27] = pInd->LQI;
		UartOut.PKT.Arguments[28] = pInd->RxTime;
		UartOut.PKT.Arguments[29] = pInd->RxTime >> 8;		
		rpmemcpy(&(UartOut.PKT.Arguments[30]), pInd->pasdu, (pInd->asduLen & 0x007F) );

		UartOut.PKT.Size = (pInd->asduLen & 0x007F) + 30;
		UartOut.LEN = UartOut.PKT.Size + 4;
		UartOut.PKT.layer = APS_LAYER;
		UartOut.PKT.Opcode = A_DATA_OPC;
		UartOut.PKT.Type = IND_TYPE;
		ZSYS_UART_API_SEND();
	#endif

	if(pInd->ClusID & 0x8000)	// Case : Rx-Response
			EnableRsp = 0;				
	else		EnableRsp = 1;				// Case : Rx-Request
	
	APP2APS.DATA_REQ.DstEP = pInd->SrcEP;
	APP2APS.DATA_REQ.DstMode = pInd->SrcMode;	
	APP2APS.DATA_REQ.DstAdd_U.Short = pInd->SrcAdd_U.Short;
	APP2APS.DATA_REQ.pasdu = APP_ASDU.Buff;
	APP2APS.DATA_REQ.ProfileID = pInd->ProfileID;
	//APP2APS.DATA_REQ.Radius = 0;
	APP2APS.DATA_REQ.Radius = NIB.nwkMaxDepth << 1;
	APP2APS.DATA_REQ.SrcEP = pInd->DstEP;
	APP2APS.DATA_REQ.TxOption = 0x04;			// AckReq=1
	APP2APS.DATA_REQ.asduLen = 0;
		
	switch(pInd->ClusID)
	{
		//======================================================
		case aCID_Nwk_Addr_Req :
			#if (UserConfig_Enable_ZDP_NwkAddrReq)
				APP2APS.DATA_REQ.ClusID = aCID_Nwk_Addr_Rsp;			
				APP2APS.DATA_REQ.asduLen = ZZDP_Callback_For_NwkAddrReq(pInd->pasdu, APP_ASDU.Buff);
			#endif
			
			break;
		case aCID_Nwk_Addr_Rsp :	
			// TODO : Software Routine that processes this ZDP response can be added by Application	
			#if (UserConfig_Enable_ZDP_NwkAddrReq)
				ZZDP_Callback_For_NwkAddrRsp(pInd->pasdu);
			#endif			
			break;	
		//======================================================
		case aCID_IEEE_Addr_Req :
			#if (UserConfig_Enable_ZDP_IEEEAddrReq)
				APP2APS.DATA_REQ.ClusID = aCID_IEEE_Addr_Rsp;
				APP2APS.DATA_REQ.asduLen = ZZDP_Callback_For_IEEEAddrReq(pInd->pasdu, APP_ASDU.Buff);
			#endif
			
			break;			
		case aCID_IEEE_Addr_Rsp :
			// TODO : Software Routine that processes this ZDP response can be added by Application
			#if (UserConfig_Enable_ZDP_IEEEAddrReq)
				ZZDP_Callback_For_IEEEAddrRsp(pInd->pasdu);
			#endif			
			break;
		//======================================================
		case aCID_Node_Desc_Req :
			#if (UserConfig_Enable_ZDP_NodeDescReq)
				APP2APS.DATA_REQ.ClusID = aCID_Node_Desc_Rsp;
				APP2APS.DATA_REQ.asduLen = ZZDP_Callback_For_NodeDescReq(pInd->pasdu, APP_ASDU.Buff);			
			#endif
			
			break;
		case aCID_Node_Desc_Rsp :
			// TODO : Software Routine that processes this ZDP response can be added by Application
			break;
		//======================================================
		case aCID_Power_Desc_Req :
			#if (UserConfig_Enable_ZDP_PowerDescReq)
				APP2APS.DATA_REQ.ClusID = aCID_Power_Desc_Rsp;
				APP2APS.DATA_REQ.asduLen = ZZDP_Callback_For_PowerDescReq(pInd->pasdu, APP_ASDU.Buff);
			#endif
			
			break;
		case aCID_Power_Desc_Rsp :
			// TODO : Software Routine that processes this ZDP response can be added by Application
			break;
		//======================================================		
		case aCID_Simp_Desc_Req :
			#if (UserConfig_Enable_ZDP_SimpDescReq)
				APP2APS.DATA_REQ.ClusID = aCID_Simp_Desc_Rsp;
				APP2APS.DATA_REQ.asduLen = ZZDP_Callback_For_SimpDescReq(pInd->pasdu, APP_ASDU.Buff);
			#endif
			
			break;
		case aCID_Simp_Desc_Rsp :
			// TODO : Software Routine that processes this ZDP response can be added by Application
			break;
		//======================================================
		case aCID_Active_EP_Req :
			#if (UserConfig_Enable_ZDP_ActiveEPReq)
				APP2APS.DATA_REQ.ClusID = aCID_Active_EP_Rsp;			
				APP2APS.DATA_REQ.asduLen = ZZDP_Callback_For_ActiveEpReq(pInd->pasdu, APP_ASDU.Buff);
			#endif
			
			break;
		case aCID_Active_EP_Rsp :
			// TODO : Software Routine that processes this ZDP response can be added by Application
			break;
		//======================================================
		case aCID_Match_Desc_Req :
			#if (UserConfig_Enable_ZDP_MatchDescReq)
				APP2APS.DATA_REQ.ClusID = aCID_Match_Desc_Rsp;
				APP2APS.DATA_REQ.asduLen = ZZDP_Callback_For_MatchDescReq(pInd->pasdu, APP_ASDU.Buff);
			#endif
			
			break;
		case aCID_Match_Desc_Rsp :
			// TODO : Software Routine that processes this ZDP response can be added by Application
			break;
		//======================================================
		case aCID_Sys_Serv_Disc_Req :
			#if (UserConfig_Enable_ZDP_SysServerReq)
				APP2APS.DATA_REQ.TxOption = 0x04;		// AckReq=1
				APP2APS.DATA_REQ.ClusID = aCID_Sys_Serv_Disc_Rsp;
				APP2APS.DATA_REQ.asduLen = ZZDP_Callback_For_SysServDiscReq(pInd->pasdu, APP_ASDU.Buff);
			#endif
			
			break;
		case aCID_Sys_Serv_Disc_Rsp :
			// TODO : Software Routine that processes this ZDP response can be added by Application
			break;
		//======================================================
		case aCID_Device_Announce :
			ZZDP_Callback_For_DeviceAnnounce(pInd->pasdu);
			EnableRsp = 0;
			break;			
		//======================================================
		case aCID_Comp_Desc_Req :
			#if (UserConfig_Enable_ZDP_CompDescReq)
				APP2APS.DATA_REQ.ClusID = aCID_Comp_Desc_Rsp;			
				APP2APS.DATA_REQ.asduLen = ZZDP_Callback_For_CompDescReq(pInd->pasdu, APP_ASDU.Buff);
			#endif
			
			break;	
		case aCID_Comp_Desc_Rsp :
			// TODO : Software Routine that processes this ZDP response can be added by Application
			break;
		//======================================================
		case aCID_User_Desc_Req	:
			#if (UserConfig_Enable_ZDP_UserDescriptor)
				APP2APS.DATA_REQ.ClusID = aCID_User_Desc_Rsp;
				APP2APS.DATA_REQ.asduLen = ZZDP_Callback_For_UserDescReq(pInd->pasdu, APP_ASDU.Buff);
			#endif
			
			break;
		case aCID_User_Desc_Rsp	:
			// TODO : Software Routine that processes this ZDP response can be added by Application
			break;
		//======================================================
		case aCID_User_Desc_Set	:
			#if (UserConfig_Enable_ZDP_UserDescriptor)		
				APP2APS.DATA_REQ.ClusID = aCID_User_Desc_Conf;
				APP2APS.DATA_REQ.asduLen = ZZDP_Callback_For_UserDescSet(pInd->pasdu, APP_ASDU.Buff);
			#endif

			break;
		case aCID_User_Desc_Conf	:
			// TODO : Software Routine that processes this ZDP response can be added by Application
			break;
		//======================================================
		case aCID_Mgmt_Nwk_Update_Req	:
			#if (UserConfig_Enable_ZDP_NwkUpdateReq)
				APP2APS.DATA_REQ.ClusID = aCID_Mgmt_Nwk_Update_Noti;
				APP2APS.DATA_REQ.asduLen = ZZDP_Callback_For_NwkUpdateReq(pInd, APP_ASDU.Buff);
			#endif
			
			break;
		case aCID_Mgmt_Nwk_Update_Noti	:
			// TODO : Software Routine that processes this ZDP response can be added by Application
			break;			
		//======================================================
		case aCID_Bind_Req :
			#if (UserConfig_Enable_ZDP_BindReq)
				APP2APS.DATA_REQ.ClusID = aCID_Bind_Rsp;
				APP2APS.DATA_REQ.asduLen = ZZDP_Callback_For_BindReq(pInd->pasdu, APP_ASDU.Buff);
			#endif
			
			break;
		case aCID_Bind_Rsp :
			// TODO : Software Routine that processes this ZDP response can be added by Application					
			#if (UserConfig_Enable_ZDP_BindReq)
				ZZDP_Callback_For_BindRsp(pInd->pasdu);
			#endif
			break;
		//======================================================
		case aCID_Unbind_Req	 :
			#if (UserConfig_Enable_ZDP_BindReq)
				APP2APS.DATA_REQ.ClusID = aCID_Unbind_Rsp;
				APP2APS.DATA_REQ.asduLen = ZZDP_Callback_For_UnbindReq(pInd->pasdu, APP_ASDU.Buff);
			#endif
			
			break;
		case aCID_Unbind_Rsp :
			// TODO : Software Routine that processes this ZDP response can be added by Application
			#if (UserConfig_Enable_ZDP_BindReq)
				ZZDP_Callback_For_UnbindRsp(pInd->pasdu);
			#endif		
			break;
		//======================================================
		case aCID_Mgmt_LQI_Req :
			#if (UserConfig_Enable_ZDP_LQIReq)
				APP2APS.DATA_REQ.ClusID = aCID_Mgmt_LQI_Rsp;
				APP2APS.DATA_REQ.asduLen = ZZDP_Callback_For_LQIReq(pInd->pasdu, APP_ASDU.Buff);
			#endif

			break;
		case aCID_Mgmt_LQI_Rsp :
			// TODO : Software Routine that processes this ZDP response can be added by Application
			break;
		//======================================================		
		case aCID_Mgmt_Rtg_Req :
			#if (UserConfig_Enable_ZDP_RtgReq)
				APP2APS.DATA_REQ.ClusID = aCID_Mgmt_Rtg_Rsp;
				APP2APS.DATA_REQ.asduLen = ZZDP_Callback_For_RtgReq(pInd->pasdu, APP_ASDU.Buff);
			#endif

			break;
		case aCID_Mgmt_Rtg_Rsp :
			// TODO : Software Routine that processes this ZDP response can be added by Application
			break;		
		//======================================================		
		case aCID_Mgmt_Leave_Req :
			#if (UserConfig_Enable_ZDP_LeaveReq)
				APP2APS.DATA_REQ.ClusID = aCID_Mgmt_Leave_Rsp;
				APP2APS.DATA_REQ.asduLen = ZZDP_Callback_For_LeaveReq(pInd->pasdu, APP_ASDU.Buff);
			#endif
		
			break;
		case aCID_Mgmt_Leave_Rsp :
			// TODO : Software Routine that processes this ZDP response can be added by Application
			break;
		//======================================================		
		case aCID_Mgmt_Permit_Join_Req :
			#if (UserConfig_Enable_ZDP_PermitJoinReq)
				APP2APS.DATA_REQ.ClusID = aCID_Mgmt_Permit_Join_Rsp;
				APP2APS.DATA_REQ.asduLen = ZZDP_Callback_For_PermitJoinReq(pInd->pasdu, APP_ASDU.Buff);
			#endif
			break;

		case aCID_Mgmt_Permit_Join_Rsp :
			// TODO : Software Routine that processes this ZDP response can be added by Application		
			break;		
		//======================================================
		default	:	EnableRsp = 0;	break;
		//======================================================
		
	}

	if(EnableRsp && APP2APS.DATA_REQ.asduLen)
	{
		ZAPS_DATA_REQ(&APP2APS.DATA_REQ, &APS2APP.DATA_CON);
	}
	
}


/***********************************************************************************
 *
 *	NAME		: RemoveWarning_ZDP_Send()
 *
 *	DESCRIPTION	: Pretends to call ZZDP_Send_xxxx() functions to remove warnings of Keil Compiler.
 *		In fact, the functions are not called because "Call" is 0.
 *
 * 	PARAMETER	: None
 *
 * 	RETURN		: None
 *
 * 	NOTES		: A warning that indicates UNCALLED_SEGMENT about RemoveWarning_ZDP_Send()
 *		is generated when compiled. This warning is ignored.
 *	
 ***********************************************************************************/
void RemoveWarning_ZDP_Send()
{
	UINT8	Call;

	Call = 0;

	if(Call)
	{
		#if(UserConfig_Enable_ZDP_BindReq)
			ZZDP_Send_BindReq(0,0);
			ZZDP_Send_UnbindReq(0,0);
		#endif	
		#if (UserConfig_Enable_ZDP_NwkAddrReq)
			ZZDP_Send_NwkAddrReq(0,0,0);
		#endif
		#if (UserConfig_Enable_ZDP_IEEEAddrReq)
			ZZDP_Send_IEEEAddrReq(0,0,0);
		#endif
		#if (UserConfig_Enable_ZDP_SimpDescReq)
			ZZDP_Send_SimpDescReq(0,0);
		#endif
		#if (UserConfig_Enable_ZDP_ActiveEPReq)
			ZZDP_Send_ActiveEpReq(0);
		#endif
		#if (UserConfig_Enable_ZDP_NodeDescReq)
			ZZDP_Send_NodeDescReq(0);
		#endif
		#if (UserConfig_Enable_ZDP_PowerDescReq)
			ZZDP_Send_PowerDescReq(0);
		#endif
		#if (UserConfig_Enable_ZDP_MatchDescReq)
			ZZDP_Send_MatchDescReq(0,0,0,0);
		#endif
		#if (UserConfig_Enable_ZDP_UserDescriptor)
			ZZDP_Send_UserDescReq(0);
			ZZDP_Send_UserDescSet(0,0);
		#endif
		#if (UserConfig_Enable_ZDP_SysServerReq)
			ZZDP_Send_SysServDiscReq(0);
		#endif
		#if (UserConfig_Enable_ZDP_CompDescReq)
			ZZDP_Send_CompDescReq(0);
		#endif
		#if (UserConfig_Enable_ZDP_NwkUpdateReq)
			ZZDP_Send_NwkUpdateReq_ChangeChannel(0);
			ZZDP_Send_NwkUpdateReq_EDScan(0,0);
			ZZDP_Send_NwkUpdateReq_NwkManager(0,0);
		#endif
		#if (UserConfig_Enable_ZDP_LQIReq)
			ZZDP_Send_LQIReq(0,0);
		#endif
		#if (UserConfig_Enable_ZDP_RtgReq)
			ZZDP_Send_RtgReq(0,0);
		#endif
		#if (UserConfig_Enable_ZDP_LeaveReq)
			ZZDP_Send_LeaveReq(0,0,0,0);
		#endif
		#if (UserConfig_Enable_ZDP_PermitJoinReq)
			ZZDP_Send_PermitJoinReq(0,0,0);
		#endif

	}
}


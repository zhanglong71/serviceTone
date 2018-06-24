
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

	extern	NLDME_REQ_PKT	APS2NWK;
	extern	NLDME_CON_PKT	NWK2APS;

	extern	NS_PIB		NIB;

	extern	void ZNWK_RESET_REQ(NLME_RESET_req *pReq, NLME_RESET_con *pCon);
	extern	void ZNWK_NET_FORM_REQ(NLME_NET_FORM_req *pReq, NLME_NET_FORM_con *pCon);
	extern	void ZNWK_NET_DISC_REQ(NLME_NET_DISC_req *pReq, NLME_NET_DISC_con *pCon);
	extern	void ZNWK_JOIN_REQ(NLME_JOIN_req *pReq, NLME_JOIN_con *pCon);
	extern	void ZNWK_DATA_REQ(NLDE_DATA_req *pReq, NLDE_DATA_con *pCon);
	extern	void ZNWK_DIRECT_JOIN_REQ(NLME_DIRECT_JOIN_req *pReq, NLME_DIRECT_JOIN_con *pCon);
	extern	void ZNWK_START_ROUTE_REQ(NLME_START_ROU_req *pReq, NLME_START_ROU_con *pCon);
	extern	void ZNWK_PERMIT_JOIN_REQ(NLME_PERMIT_JOIN_req *pReq, NLME_PERMIT_JOIN_con *pCon);
	extern	void ZNWK_SET_REQ(NLME_SET_req *pReq, NLME_SET_con *pCon);
	extern	void ZNWK_GET_REQ(NLME_GET_req *pReq, NLME_GET_con *pCon);
	extern	void ZNWK_SET_TABLE_REQ(NLME_SET_TABLE_req *pReq, NLME_SET_TABLE_con *pCon);
	extern	void ZNWK_GET_TABLE_REQ(NLME_GET_TABLE_req *pReq, NLME_GET_TABLE_con *pCon);
	extern	void ZNWK_LEAVE_REQ(NLME_LEAVE_req *pReq, NLME_LEAVE_con *pCon);
	extern	void ZNWK_SYNC_REQ(NLME_SYNC_req *pReq, NLME_SYNC_con *pCon);
	extern	void ZNWK_ROUTE_DISC_REQ(NLME_ROUTE_DISC_req *pReq, NLME_ROUTE_DISC_con *pCon);
	extern	void ZNWK_ED_SCAN_REQ(NLME_ED_SCAN_req *pReq, NLME_ED_SCAN_con *pCon);

	extern	UINT8 NWKLIB_SEND_AddrConflict(UINT16 ConflictNwkAddr);
	extern	void NWKLIB_PROC_AddrConflict(UINT16 ConflictNwkAddr, UINT8 ProcMode);
	extern	void NWKLIB_Set_ConflictAddr(UINT16 ConflictNwkAddr);	
	extern	UINT16 NWKLIB_Get_RandomNwkAddr(UINT8 CmpOption);
	extern	void NWKLIB_DeleteRT_UsedEntry(UINT16 DstAddr);

	extern	UINT8 NWKLIB_DevMgmt_GetIEEEAddr(UINT16 InNwkAddr, UINT8 *pOutIEEE);
	extern	UINT8 NWKLIB_DevMgmt_GetNWKAddr(UINT8 *pInIEEE, UINT16 *pOutNwkAddr);
	extern	UINT8 NWKLIB_DevMgmt_UpdateIEEEAddr(UINT16 TargetNwkAddr, UINT8 *pInIEEE);
	extern	void NWKLIB_DevMgmt_DeleteNWKAddr(UINT16 InNwkAddr);
	extern	void NWKLIB_DevMgmt_DeleteIEEEAddr(UINT8 *pInIEEE);
	extern	UINT8 NWKLIB_DevMgmt_AddNT(NS_NT *pNT);
	extern	UINT8 NWKLIB_FindNT_NwkAddr(UINT16 NwkAddr);
	extern	UINT8 NWKLIB_FindNT_IEEEAddr(UINT8 FindRelation, UINT8 *pIEEE);




/*******************************************************************************
	- Chip		: MG2450/2455
	- Vendor		: RadioPulse Inc, 2007.
	- Date		: 2010-11-15
	- Version		: VER 2.2

	[2010-11-15] VER 2.2
	- For ZigBeePRO Development Kit V2.2
		+ ZAPS_SET_CONFIG_REQ() is added.
	
	[2010-03-02] VER 2.10
	- For ZigBeePRO Development Kit V2.10
*******************************************************************************/

	extern	APS_PIB		AIB;

	extern	APSDME_REQ_PKT	APP2APS;
	extern	APSDME_CON_PKT	APS2APP;
	
	extern	UINT8		NSDU[];
	extern	UINT8		NumGT;
	extern	UINT8		NumBT;
	
	extern	void ZAPS_INIT_AIB();

	extern	void ZAPS_ADD_GROUP_REQ(APSME_ADD_GROUP_req *pReq, APSME_ADD_GROUP_con *pCon);
	extern	void ZAPS_REMOVE_GROUP_REQ(APSME_REMOVE_GROUP_req *pReq, APSME_REMOVE_GROUP_con *pCon);
	extern	void ZAPS_REMOVE_ALL_GROUP_REQ(APSME_REMOVE_ALL_GROUP_req *pReq, APSME_REMOVE_ALL_GROUP_con *pCon);
	extern	void ZAPS_BIND_REQ(APSME_BIND_req *pReq, APSME_BIND_con *pCon);
	extern	void ZAPS_UNBIND_REQ(APSME_UNBIND_req *pReq, APSME_UNBIND_con *pCon);
	extern	void ZAPS_GET_REQ(APSME_GET_req *pReq, APSME_GET_con *pCon);
	extern	void ZAPS_SET_REQ(APSME_SET_req *pReq, APSME_SET_con *pCon);
	extern	void ZAPS_GET_TABLE_REQ(APSME_GET_TABLE_req *pReq, APSME_GET_TABLE_con *pCon);	
	extern	void ZAPS_SET_TABLE_REQ(APSME_SET_TABLE_req *pReq, APSME_SET_TABLE_con *pCon);	
	extern	void ZAPS_DATA_REQ(APSDE_DATA_req *pReq, APSDE_DATA_con *pCon);
	extern	UINT8 ZAPS_SET_CONFIG_REQ(UINT8 AttID, UINT8 * pAttValue);
	extern	UINT8 ZAPS_ESTABLISH_KEY_REQ(APSME_ESTABLISH_KEY_req *pReq);
	extern	UINT8 ZAPS_TRANSPORT_KEY_REQ(APSME_TRANSPORT_KEY_req *pReq);
	extern	UINT8 ZAPS_UPDATE_DEVICE_REQ(APSME_UPDATE_DEV_req *pReq);
	extern	UINT8 ZAPS_REMOVE_DEVICE_REQ(APSME_REMOVE_DEVICE_req *pReq);
	extern	UINT8 ZAPS_REQUEST_KEY_REQ(APSME_REQUEST_KEY_req *pReq);
	extern	UINT8 ZAPS_SWITCH_KEY_REQ(APSME_SWITCH_KEY_req *pReq);

	extern	UINT8 APSLIB_FindGT_GroupAddr_EP(UINT16 GroupAddr, UINT8 EP);



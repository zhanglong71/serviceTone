/*******************************************************************************
	- Chip		: MG2450/2455
	- Vendor		: RadioPulse Inc, 2007.
	- Date		: 2010-01-20
	- Version		: VER 2.10

	[2010-01-20] VER 2.10
	- Delete not needed "extern"

	[2007-12-11] VER 1.61
	- ZMAC_SYNC_REQ() is added.
	- ZMAC_GTS_REQ() is added.

	[2007-11-30] VER 1.60
	- All MAC source is rebuilded

	[2007-11-07] VER 1.50
	- Initial Version
*******************************************************************************/

	extern	MLDME_PACKET	NWK2MAC;
	extern	MLDME_PACKET	MAC2NWK;

	extern	PHY_INFORMATION_BASE	PPIB;
	extern	MAC_INFORMATION_BASE	MPIB;
	
	extern	void ZMAC_RESET_REQ(MLME_RESET_REQ *pReq, MLME_RESET_CON *pCon);
	extern	void ZMAC_START_REQ(MLME_START_REQ *pReq, MLME_START_CON *pCon);
	extern	void ZMAC_SET_REQ(MLME_SET_REQ *pReq, MLME_SET_CON *pCon);
	extern	void ZMAC_GET_REQ(MLME_GET_REQ *pReq, MLME_GET_CON *pCon);
	extern	void ZMAC_SCAN_REQ(MLME_SCAN_REQ *pReq, MLME_SCAN_CON *pCon);
	//extern	void ZMAC_DISA_REQ(MLME_DISA_REQ *pReq, MLME_DISA_CON *pCon);			// Not used in ZigBee
	extern	void ZMAC_DATA_REQ(MCPS_DATA_REQ *pReq, MCPS_DATA_CON *pCon);
	extern	void ZMAC_ASSO_REQ(MLME_ASSO_REQ *pReq, MLME_ASSO_CON *pCon);
	extern	void ZMAC_POLL_REQ(MLME_POLL_REQ *pReq, MLME_POLL_CON *pCon);
	//extern	void ZMAC_PURGE_REQ(MCPS_PURGE_REQ *pReq, MCPS_PURGE_CON *pCon);		// Not used in ZigBee
	//extern	void ZMAC_RX_ENA_REQ(MLME_RX_ENA_REQ *pReq, MLME_RX_ENA_CON *pCon);	// Not used in ZigBee
	//extern	void ZMAC_SYNC_REQ(MLME_SYNC_REQ *pReq, MLME_SYNC_LOSS_IND *pInd);		// Not used in ZigBee
	//extern	void ZMAC_GTS_REQ(MLME_GTS_REQ *pReq, MLME_GTS_CON *pCon);				// Not used in ZigBee

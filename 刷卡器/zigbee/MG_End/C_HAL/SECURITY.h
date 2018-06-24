/*******************************************************************************
	- Chip		: MG2450/2455
	- Vendor		: RadioPulse Inc, 2007.
	- Date		: 2007-11-07
	- Version		: VER 1.5	
*******************************************************************************/

	extern	void ZHAL_SEC_KEY_SET(UINT8 KeyNum, UINT8 *pKey);
	extern	void ZHAL_SEC_KEY0_SET(UINT8 *pKey);
	extern	void ZHAL_SEC_KEY1_SET(UINT8 *pKey);
	extern	void ZHAL_SEC_TXNONCE_KEY_SEQ_SET(UINT8 KeySeq);
	extern	void ZHAL_SEC_RXNONCE_KEY_SEQ_SET(UINT8 KeySeq);
	extern	UINT8 ZHAL_SEC_TXNONCE_KEY_SEQ_GET();
	extern	UINT8 ZHAL_SEC_RXNONCE_KEY_SEQ_GET();
	extern	void ZHAL_SEC_TXNONCE_FRAME_CNT_SET(UINT32 FrameCnt);
	extern	void ZHAL_SEC_RXNONCE_FRAME_CNT_SET(UINT32 FrameCnt);
	extern	UINT32 ZHAL_SEC_TXNONCE_FRAME_CNT_GET();
	extern	UINT32 ZHAL_SEC_RXNONCE_FRAME_CNT_GET();
	extern	void ZHAL_SEC_TXNONCE_EXT_ADDR_SET(UINT8 *pIEEE);
	extern	void ZHAL_SEC_RXNONCE_EXT_ADDR_SET(UINT8 *pIEEE);
	extern	void ZHAL_SEC_TXKEY_CHOICE(UINT8 KeyNum);
	extern	void ZHAL_SEC_RXKEY_CHOICE(UINT8 KeyNum);
	extern	void ZHAL_SEC_SECM_SET(UINT8 M);
	extern	void ZHAL_SEC_MODE_SET(UINT8 Mode);
	extern	void ZHAL_SEC_TXL_SET(UINT8 L);
	extern	void ZHAL_SEC_RXL_SET(UINT8 L);
	extern	void ZHAL_SEC_SAES(UINT8 KeyNum, UINT8 *pPlainText, UINT8 *pCipherText);

	extern	UINT8 ZHAL_SEC_ENCRYPTION_CCM32(UINT8 *pText, UINT8 InLen, UINT32 FCnt, UINT8 KSeq, UINT8 *pEncLen);
	extern	UINT8 ZHAL_SEC_DECRYPTION_CCM32(UINT8 *pText, UINT8 InLen, UINT32 FCnt, UINT8 KSeq, UINT8 *pDecLen);


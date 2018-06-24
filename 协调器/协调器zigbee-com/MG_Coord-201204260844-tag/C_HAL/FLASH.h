/*******************************************************************************
	- Chip		: MG2450/2455
	- Vendor		: RadioPulse Inc, 2007.
	- Date		: 2007-11-07
	- Version		: VER 1.5	
*******************************************************************************/

	extern	void ZHAL_FLASH_REG_INIT();
	extern	UINT8 ZHAL_FLASH_WRITE_UNPROTECTED();
	extern	void ZHAL_FLASH_WRITE_PROTECTED();

	extern	void ZHAL_FLASH_PAGE_ERASE(UINT16 FlashWordAddr);
	extern	UINT8 ZHAL_FLASH_PAGE_WRITE_EXTEND(UINT8 BankNum, UINT16 CodeAddr, UINT8 *pByteBuf);

	extern	UINT8 ZHAL_FLASH_READ(UINT16 CodeAddr, UINT8 *pReadBuf, UINT16 Len);


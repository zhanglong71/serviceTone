/*******************************************************************************
	- Chip		: MG2450/2455
	- Vendor		: RadioPulse Inc, 2007.
	- Date		: 2010-08-13
	- Version		: VER 1.80

	[2010-08-13] VER 1.80
	- Modified to support the new version chip whose CHIP ID is 0x96 in ZHAL_FLASH_PAGE_WRITE_EXTEND().

	[2010-05-28] VER 1.72
	- In abnormal situation, Flash-writing is not performed in ZHAL_FLASH_PAGE_WRITE_EXTEND().
		+ if( (xPDCON & 0x04) == 0)	return 2;
		+ if(xFCN_CMD != 0x80)		return 2;
		+ if(xFCN_LEN1 != 0x00)		return 3;
		+ if(xFCN_LEN0 != 0x00)		return 5;
		+ if(xFCN_ADR1 != 0x70)	return 6;
		+ if(xFCN_ADR0 != 0x00)	return 7;

	[2010-04-27] VER 1.71
	- Remove "Flash Writing with DVREG=1.8V"

	[2009-11-27]	VER 1.70
	- ZHAL_FLASH_REG_INIT() is added
	- ZHAL_FLASH_PAGE_WRITE_PROTECTED() is added
	- ZHAL_FLASH_PAGE_WRITE_UNPROTECTED() is added
	- ZHAL_FLASH_READ() is added	
	- ZHAL_FLASH_PAGE_ERASE() is modified
	- ZHAL_FLASH_PAGE_WRITE_EXTEND() is modified

	[2009-02-23]	VER 1.60
	- ZHAL_FLASH_PAGE_WRITE() is removed.
	- ZHAL_FLASH_PAGE_WRITE_EXTEND() is added.
	- Flash Writing is operated with DVREG=1.8V.(xPDM |= 0x40)	
	
	[2007-11-07] VER 1.50
	- Initial Version	
*******************************************************************************/


/****************************************************************
	MG245X Embedded FLASH

	+ Memory Size : 96KB + 1KB
	+ Page Size : 512 Bytes
	+ Erase Unit : 512 Bytes
	+ Bank Structure
		Bank0   	: 0x0000 ~ 0x7FFF
		Bank1	: 0x8000 ~ 0xFFFF
		Bank2 	: 0x8000 ~ 0xFFFF
		Bank3   	: Reserved as 0
		Bank4   	: 0x8000 ~ 0x8400
****************************************************************/

#include "INCLUDE_TOP.h"

	UINT8	FlashWriteProtect = 0x80;			// 0x00=Unprotected, 0x80=Protected

/*void ZHAL_FLASH_REG_INIT()
{	
	xFCN_NOWAIT	=0x00;

	xFCN_CMD	=0x80;		// protect bit set 
	xFCN_ADR1	=0x70;		// address register parking at 0x6000-0x7FFF 
	xFCN_ADR0	=0x00;
	xFCN_LEN1	=0x00;		// length register parking with 0x0000 
	xFCN_LEN0	=0x00;
	xFCN_TERS1	=0x00;		// flash timing parameters parking with all zero 
	xFCN_TERS0	=0x00;
	xFCN_TME1	=0x00;
	xFCN_TME0	=0x00;
	xFCN_TPRG	=0x00;
	xFCN_TRCV	=0x00;
	xFCN_THV1	=0x00;
	xFCN_THV0	=0x00;
	xFCN_TNVS	=0x00;
	xFCN_TNVH	=0x00;
	xFCN_TPGS	=0x00;
	xFCN_TNVH1	=0x00;
	//
	//

	FlashWriteProtect = 0x80;
	
}  */


// RETURN	: 0=Success, 1=Failed
/*UINT8 ZHAL_FLASH_WRITE_UNPROTECTED()
{
	if( (xPDCON & 0x20) == 0)	// AVREG_OK=0
	{
		return	1;
	}
	else
	{
		FlashWriteProtect = 0x00;
	}
	return	0;
}*/

/*void ZHAL_FLASH_WRITE_PROTECTED()
{

	FlashWriteProtect = 0x80;
	xFCN_CMD = 0x80;	// Write-Protected
}*/

void ZHAL_FLASH_PAGE_ERASE(UINT16 FlashWordAddr)
{
	xFCN_NOWAIT = 0x00;

	while(xFCN_STS0 & 0x01);

	_nop_();
	_nop_();
	_nop_();
	_nop_();

	xFCN_ADR0 = FlashWordAddr & 0xFF;
	xFCN_ADR1 = (FlashWordAddr>>8);
	xFCN_LEN1 = 0;
	xFCN_LEN0 = 1;
	xFCN_CMD = (FlashWriteProtect | 0x02);	// Page Erase

	_nop_();
	_nop_();
	_nop_();
	_nop_();

	while(xFCN_STS0 & 0x01);
	xFCN_CMD = 0x80;	// Write-Protected

}

//-------------------------------------------------------------------------------------
//	Area		| Code Address 	| Word Address ( 1Word = 4Byte )
//-------------------------------------------------------------------------------------
//	Common	| 0x0000 ~ 0x7FFF	| 0x0000 ~ 0x1FFF
//-------------------------------------------------------------------------------------
//	Bank0	| 0x8000 ~ 0xFFFF	| 0x2000 ~ 0x3FFF
//-------------------------------------------------------------------------------------
//	Bank1	| 0x8000 ~ 0xFFFF	| 0x4000 ~ 0x5FFF
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
//-	BankNum		: 0=BANK0, 1=BANK1. When CodeAddr < 0x8000, BankNum is ignored.
//-	CodeAddr	: Should be aligned for 512bytes.(0x8000 ~ ). (ex) 0x8000 or 0x8200 or 0x8400 ...
//		Common	: 0x0000 ~ 0x7FFF
//		BANK0	: 0x8000 ~ 0xFFFF
//		BANK1	: 0x8000 ~ 0xFFFF
//- pByteBuf		: pointer to 512 Bytes Data to Write
//-------------------------------------------------------------------------------------

// 512 Byte Addr Write
// For Compatibility to old version function 'ZHAL_FLASH_PAGE_WRITE()'
// 		ZHAL_FLASH_PAGE_WRITE(CodeAddr, pByteBuf)
//			= ZHAL_FLASH_PAGE_WRITE_EXTEND(0, CodeAddr, pByteBuf)

// RETURN	: 1=Failed, 0=Success
/*UINT8 ZHAL_FLASH_PAGE_WRITE_EXTEND(UINT8 BankNum, UINT16 CodeAddr, UINT8 *pByteBuf)
{
	UINT16 	i;
	UINT16	len;
	UINT16	dst;

	UINT16	FlashWordAddr;

	if(FlashWriteProtect)	return 1;

	if(CodeAddr < 0x8000)
	{
		FlashWordAddr = CodeAddr >> 2;	// FlashWordAddr = CodeAddr / 4
	}
	else
	{
		if(BankNum == 0)	FlashWordAddr = 0x2000 + (CodeAddr - 0x8000) / 4;	
		else					FlashWordAddr = 0x4000 + (CodeAddr - 0x8000) / 4;	
	}

//
//	STEP-1 : CLOCK STALL OFF
//
	xFCN_NOWAIT = 0x00;

	//
	//
#if 	(OPERATE_AT_16MHz)
	// 16MHz Parameters 
	xFCN_TERS1	=0x04; 
	xFCN_TERS0	=0xE2; 
	xFCN_TME1	=0x04; 
	xFCN_TME0	=0xE2; 
	xFCN_TPRG	=0x14;
	xFCN_TRCV	=0x10;
	xFCN_THV1	=0x00; 
	xFCN_THV0	=0xFA; 
	xFCN_TNVS	=0x50;
	xFCN_TNVH	=0x50;
	xFCN_TPGS	=0xA0;
	xFCN_TNVH1	=0x64; 
#else
	// 08MHz Parameters
	xFCN_TERS1	=0x02;
	xFCN_TERS0	=0x71;
	xFCN_TME1	=0x02;
	xFCN_TME0	=0x71;
	xFCN_TPRG	=0x0A;
	xFCN_TRCV	=0x08;
	xFCN_THV1	=0x00;
	xFCN_THV0	=0x7C;
	xFCN_TNVS	=0x28;
	xFCN_TNVH	=0x28;
	xFCN_TPGS	=0x50;
	xFCN_TNVH1 	=0x32;
#endif

	if(xCHIPID >= 0x96)
	{
		if(xPDCON & 0x04)		return 2;	// If BOD(Bit2)=1(Disabled. reversed polarity)
	}
	else
	{
		if( (xPDCON & 0x04) == 0)	return 2;	// If BOD(Bit2)=0(Disabled)
	}
	if(xFCN_CMD != 0x80)		return 3;
	if(xFCN_LEN1 != 0x00)		return 4;
	if(xFCN_LEN0 != 0x00)		return 5;
	if(xFCN_ADR1 != 0x70)	return 6;
	if(xFCN_ADR0 != 0x00)	return 7;	
	//
	//
	
	xFCN_LEN0 = 0x01;
	xFCN_LEN1 = 0x00;
	
//
//	STEP-2 : Erase Page
//

	WDT |= 0x04;
	ZHAL_FLASH_PAGE_ERASE(FlashWordAddr);

//
//	STEP-3 : IDLE CHECK
//
	while(xFCN_STS0&0x01);	

//
//	STEP-4 : PAGE WRITE
//
	i = 0;
	dst = FlashWordAddr;
	len = 512/4;

	while(len)
	{
		WDT |= 0x04;
	
		xFCN_ADR0 = dst & 0xFF;
		xFCN_ADR1 = (dst>>8);

		xFCN_DAT3 = pByteBuf[i+3];
		xFCN_DAT2 = pByteBuf[i+2];
		xFCN_DAT1 = pByteBuf[i+1];
		xFCN_DAT0 = pByteBuf[i+0];

		xFCN_CMD  = (FlashWriteProtect | 0x01);
		
		_nop_();
		_nop_();
		_nop_();
		_nop_();

		i   += 4; 		// byte addr
		dst += 1; 	// word addr
		len -= 1; 	// word wlen 
	}

	while(xFCN_STS0 & 0x01);

	//
	//
	xFCN_CMD	=0x80;		// protect bit set 
	xFCN_ADR1	=0x70;		// address register parking at 0x6000-0x7FFF 
	xFCN_ADR0	=0x00;
	xFCN_LEN1	=0x00;		// length register parking with 0x0000 
	xFCN_LEN0	=0x00;
	xFCN_TERS1	=0x00;		// flash timing parameters parking with all zero 
	xFCN_TERS0	=0x00;
	xFCN_TME1	=0x00;
	xFCN_TME0	=0x00;
	xFCN_TPRG	=0x00;
	xFCN_TRCV	=0x00;
	xFCN_THV1	=0x00;
	xFCN_THV0	=0x00;
	xFCN_TNVS	=0x00;
	xFCN_TNVH	=0x00;
	xFCN_TPGS	=0x00;
	xFCN_TNVH1	=0x00;
	//
	//

	return	0;

}*/

//- CodeAddr	: 0x0000 ~ 0x7FFF
//- Len			: 0x0000 ~ 0x7FFF
//- RETURN	: 1=Failed, 0=Success
UINT8 ZHAL_FLASH_READ(UINT16 CodeAddr, UINT8 *pReadBuf, UINT16 Len)
{
	UINT16	i;
	UINT16	LastCodeAddr;
	UINT8	code *pCodeBuf;

	if(CodeAddr >= 0x8000)	return 1;
	if(Len >= 0x8000)			return 1;
	LastCodeAddr = CodeAddr + Len;
	if(LastCodeAddr >= 0x8000)	return 1;

	pCodeBuf = CodeAddr;
	for(i=0 ; i<Len ; i++)	pReadBuf[i] = pCodeBuf[i];
	return	0;
}



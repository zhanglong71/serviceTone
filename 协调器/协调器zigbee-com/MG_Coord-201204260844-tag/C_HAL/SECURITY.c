/*******************************************************************************
	- Chip		: MG2450/2455
	- Vendor		: RadioPulse Inc, 2007.
	- Date		: 2010-11-15
	- Version		: VER 1.53_For_ZDK

	[2010-11-15]	VER 1.53_For_ZDK	
	- Source codes are not modified. Just, functions controlled by "#if (ENABLE_SECURITY_UNCALLED)" are changed.
 	
	[2009-02-27]	VER 1.53
	- ZHAL_SEC_ENCRYPTION() is removed.
	- ZHAL_SEC_DECRYPTION() is removed.	
	- ZHAL_SEC_ENCRYPTION_CCM32() is added.
	- ZHAL_SEC_DECRYPTION_CCM32() is added.
	- memcpy() --> rpmemcpy() in ZHAL_SEC_SAES(), ZHAL_SEC_ENC/DECRYPTION_CCM32()

	[2009-02-27]	VER 1.52
	- ZHAL_SEC_KEY0_SET() is added.
	- ZHAL_SEC_KEY1_SET() is added.
	
	[2009-02-23]	VER 1.51
	- <#if ENABLE_SECURITY_UNCALLED ~ #endif> is added	

	[2007-11-07] VER 1.50
	- Initial Version
*******************************************************************************/

#include "INCLUDE_TOP.h"

	extern	UINT8	INT_AES;

void ZHAL_SEC_KEY_SET(UINT8 KeyNum, UINT8 *pKey)
{
	UINT8	_EA;

	_EA = EA;
	EA = 0;	

	if(KeyNum)	memcpy(&xKEY1(0), pKey, 16);
	else			memcpy(&xKEY0(0), pKey, 16);

	EA = _EA;
}

#if (ENABLE_SECURITY_UNCALLED)
void ZHAL_SEC_KEY0_SET(UINT8 *pKey)
{
	UINT8	_EA;

	_EA = EA;
	EA = 0;	

	memcpy(&xKEY0(0), pKey, 16);

	EA = _EA;
}

void ZHAL_SEC_KEY1_SET(UINT8 *pKey)
{
	UINT8	_EA;

	_EA = EA;
	EA = 0;

	memcpy(&xKEY1(0), pKey, 16);
	
	EA = _EA;
}
#endif

#if (ENABLE_SECURITY_UNCALLED)
void ZHAL_SEC_TXNONCE_KEY_SEQ_SET(UINT8 KeySeq)
{
	xTXNONCE(0) = KeySeq;
}


void ZHAL_SEC_RXNONCE_KEY_SEQ_SET(UINT8 KeySeq)
{
	xRXNONCE(0) = KeySeq;
}

UINT8 ZHAL_SEC_TXNONCE_KEY_SEQ_GET()
{
	return	xTXNONCE(0);
}

UINT8 ZHAL_SEC_RXNONCE_KEY_SEQ_GET()
{
	return	xRXNONCE(0);
}

void ZHAL_SEC_TXNONCE_FRAME_CNT_SET(UINT32 FrameCnt)
{
	xTXNONCE(1) = (UINT8)FrameCnt;	FrameCnt >>= 8;
	xTXNONCE(2) = (UINT8)FrameCnt;	FrameCnt >>= 8;
	xTXNONCE(3) = (UINT8)FrameCnt;	FrameCnt >>= 8;
	xTXNONCE(4) = (UINT8)FrameCnt;
}


void ZHAL_SEC_RXNONCE_FRAME_CNT_SET(UINT32 FrameCnt)
{
	xRXNONCE(1) = (UINT8)FrameCnt;	FrameCnt >>= 8;
	xRXNONCE(2) = (UINT8)FrameCnt;	FrameCnt >>= 8;
	xRXNONCE(3) = (UINT8)FrameCnt;	FrameCnt >>= 8;
	xRXNONCE(4) = (UINT8)FrameCnt;
}

UINT32 ZHAL_SEC_TXNONCE_FRAME_CNT_GET()
{
	UINT32	FrameCnt_Read;

	FrameCnt_Read = xTXNONCE(4);
	FrameCnt_Read <<= 8;		FrameCnt_Read |= xTXNONCE(3);
	FrameCnt_Read <<= 8;		FrameCnt_Read |= xTXNONCE(2);
	FrameCnt_Read <<= 8;		FrameCnt_Read |= xTXNONCE(1);
	
	return	FrameCnt_Read;
}

UINT32 ZHAL_SEC_RXNONCE_FRAME_CNT_GET()
{
	UINT32	FrameCnt_Read;

	FrameCnt_Read = xRXNONCE(4);
	FrameCnt_Read <<= 8;		FrameCnt_Read |= xRXNONCE(3);
	FrameCnt_Read <<= 8;		FrameCnt_Read |= xRXNONCE(2);
	FrameCnt_Read <<= 8;		FrameCnt_Read |= xRXNONCE(1);
	
	return	FrameCnt_Read;
}

void ZHAL_SEC_TXNONCE_EXT_ADDR_SET(UINT8 *pIEEE)
{
	memcpy(&xTXNONCE(5), pIEEE, 8);
}

void ZHAL_SEC_RXNONCE_EXT_ADDR_SET(UINT8 *pIEEE)
{
	memcpy(&xRXNONCE(5), pIEEE, 8);
}

void ZHAL_SEC_TXKEY_CHOICE(UINT8 KeyNum)
{
	if(KeyNum)	xMACSEC |= 0x40;
	else			xMACSEC &= 0xBF;
}

void ZHAL_SEC_RXKEY_CHOICE(UINT8 KeyNum)
{
	if(KeyNum)	xMACSEC |= 0x20;
	else			xMACSEC &= 0xDF;
}

void ZHAL_SEC_SECM_SET(UINT8 M)
{
	UINT8	_EA;

	_EA = EA;
	EA = 0;

	xMACSEC &= 0xE3;
	xMACSEC |= (M & 0x07) << 2;	

	EA = _EA;
}

//-- Mode
//	1 	: CBC-MAC
//	2	: CTR
//	3	: CCM
void ZHAL_SEC_MODE_SET(UINT8 Mode)
{
	UINT8	_EA;

	_EA = EA;
	EA = 0;	

	xMACSEC &= 0xFC;
	xMACSEC |= (Mode & 0x03);

	EA = _EA;
}

void ZHAL_SEC_TXL_SET(UINT8 L)
{
	xTXAL = L & 0x7F;
}

void ZHAL_SEC_RXL_SET(UINT8 L)
{
	xRXAL = L & 0x7F;
}
#endif

//---------------------------------------------------------------------
//	Exampel 			: PlainText ---> (encrypt) ---> CipherText
//	KEY		[0~15] 	: 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F
//	PlainText	[0~15]	: F0 F1  F2 F3 F4 F5  F6 F7 F8 F9  FA FB FC  FD FE FF
//	CipherText[0~15]	: 27 25 9B 7F 8F F3 50 85 94 77 F7 7D C9 71 E6 74
//---------------------------------------------------------------------
//	KeyNum		: 0=KEY0, 1=KEY1
//	pPlainText	: pointer to plaintext to encrypt
//	pCipherText	: pointer to encrypted ciphertext
//---------------------------------------------------------------------
void ZHAL_SEC_SAES(UINT8 KeyNum, UINT8 *pPlainText, UINT8 *pCipherText)
{
	UINT8	_xCLKON0;
	
	// Backup xCLKON0 and Enable AES Clock
	_xCLKON0 = xCLKON0;
	xCLKON0 |= 0x10;	

	rpmemcpy(&xSAESBUF(0), pPlainText, 16);

	if(KeyNum)		xMACSEC |= 0x80;		// bit[7]=1
	else				xMACSEC &= 0x7F;		// bit[7]=0

	INT_AES = 0;
	AESIE = 1;

	xMACSAES = 0x01;		// Stand Alone AES
	while(!INT_AES);
	INT_AES = 0;
	AESIE = 0;

	rpmemcpy(pCipherText, &xSAESBUF(0), 16);

	// Restore xCLKON0
	xCLKON0 = _xCLKON0;	
}


#if (ENABLE_SECURITY_UNCALLED)
//---------------------------------------------------------------------
//-	pText	: pointer to input PlainText and output CiphterText
//-	InLen	: Length of input message
//-	FCnt		: 32bit FrameCounter
//-	KSeq	: 8bit KeySequenceNumber
//-	pEncLen	: Length of output message which has been encrypted
//---------------------------------------------------------------------
//-	RETURN
//	0	: Success
//	2	: Failure. Invalid Length.
//	0xF0	: Failure, Encryption is not started successfully.
//	0xF1	: Failure. Encryption is not completed successfully.
//---------------------------------------------------------------------
UINT8 ZHAL_SEC_ENCRYPTION_CCM32(UINT8 *pText, UINT8 InLen, UINT32 FCnt, UINT8 KSeq, UINT8 *pEncLen)
{
	UINT8	_xMACSEC;
	UINT8	_xCLKON0;
	UINT8	_EA;
	UINT16	iw;
	UINT8	Error;

	*pEncLen = 0;	
	if(InLen > 116)				return 2;

	// Backup xCLKON0 and Enable AES Clock
	_xCLKON0 = xCLKON0;
	xCLKON0 |= 0x10;	

	// Enable Mode
	_xMACSEC = xMACSEC;
	ZHAL_SEC_MODE_SET(3);		// CCM
	
	ZHAL_SEC_TXNONCE_FRAME_CNT_SET(FCnt);		// TxNonce : FrameCounter
	ZHAL_SEC_TXNONCE_KEY_SEQ_SET(KSeq);			// TxNonce : KeySeqNum	
	ZHAL_SEC_TXL_SET(0);			// TXL=0
	ZHAL_SEC_SECM_SET( (4-2)/2 );	// MicLen=4
	
	// Packet Length and PlainText
	xMTxFIFO(0) = InLen + 2;				// Packet Length
	rpmemcpy(&xMTxFIFO(1), pText, InLen);	// PlainText

	//########################################################
	// Encryption
	//########################################################
	#define	EncStart_WaitLoopCnt	5
	#define	EncDone_WaitLoopCnt  	300
	
	xMTFCSBASE = 0;				// Pointer in TxFIFO	
	xMTFCSLEN = xMTxFIFO(0);	// Length of Packet to transmit
	
	//- Encryption Command
	_EA = EA;
	EA = 0;
	for(iw=0 ; iw<EncStart_WaitLoopCnt ; iw++)
	{	
		RstAesTsmON;
		_nop_();_nop_();
		RstAesTsmOFF;
		ClearEncDone;
		ClearAesIntFlag;
		
		EncStartON;
		EncStartOFF;
		_nop_();_nop_();
		if(xMACSTS & 0x80) 	break;	// If Enc/Dec_Busy=1, break
	}
	EA = _EA;

	Error = 0;
	if(iw == EncStart_WaitLoopCnt)
	{		
		Error = 0xF0;
	}
	else
	{
		for(iw=0 ; iw<EncDone_WaitLoopCnt ; iw++) 	// Encryption Duration : typical 50us(Max100us) @8MHz and 89Bytes MSDU
		{ 
			if(xMACSTS & 0x04)	break;
		}

		if(iw == EncDone_WaitLoopCnt)
		{
			Error = 0xF1;
		}
		else
		{
			Error = 0;
			// Copy Encrypted text (Ciphertext)
			*pEncLen = InLen + 4;
			rpmemcpy(pText, &xMTxFIFO(6), *pEncLen);
		}
	}
	ClearEncDone;
	ClearAesIntFlag;
	//########################################################

	// Disable CTR
	xMACSEC = _xMACSEC;

	// Restore xCLKON0
	xCLKON0 = _xCLKON0;

	return	Error;
}


	extern	void ZSYS_RECEIVER_STOP();
	extern	void ZSYS_RECEIVER_RESUME();

//---------------------------------------------------------------------
//-	pText	: pointer to input CipherText and output PlainText
//-	InLen	: Length of input message
//-	FCnt		: 32bit FrameCounter
//-	KSeq	: 8bit KeySequenceNumber
//-	pDecLen	: Length of output message which has been decrypted
//---------------------------------------------------------------------
//-	RETURN
//	0	: Success.
//	2	: Failure. Invalid Length.
//	0xF0	: Failure. Decryption is not started successfully.
//	0xF1	: Failure. Decryption is not completed successfully.
//	0xF2	: Failure. Authentication Error(MIC Error)
//---------------------------------------------------------------------
UINT8 ZHAL_SEC_DECRYPTION_CCM32(UINT8 *pText, UINT8 InLen, UINT32 FCnt, UINT8 KSeq, UINT8 *pDecLen)
{
	UINT8	_xMACSEC;
	UINT8	_xCLKON0;
	UINT8	_EA;
	UINT16	iw;
	UINT8	Error = 0;

	*pDecLen = 0;
	if(InLen > 120)		return 2;
	if(InLen < 4)			return 2;

	ZSYS_RECEIVER_STOP();
	 
	// Backup xCLKON0 and Enable AES Clock
	_xCLKON0 = xCLKON0;
	xCLKON0 |= 0x10;	

	// Enable Mode
	_xMACSEC = xMACSEC;
	ZHAL_SEC_MODE_SET(3);		// CCM

	ZHAL_SEC_RXNONCE_FRAME_CNT_SET(FCnt);		// RxNonce : FrameCounter
	ZHAL_SEC_RXNONCE_KEY_SEQ_SET(KSeq);			// RxNonce : KeySeqNum
	ZHAL_SEC_RXL_SET(0);			// RXL=0
	ZHAL_SEC_SECM_SET( (4-2)/2 );	// MicLength=4

	// Packet Length
	xMRxFIFO(0x80) = InLen + 2 + 5;
	// Auxiliary Header
	xMRxFIFO(0x81) = FCnt;		FCnt >>= 8;
	xMRxFIFO(0x82) = FCnt;		FCnt >>= 8;
	xMRxFIFO(0x83) = FCnt;		FCnt >>= 8;
	xMRxFIFO(0x84) = FCnt;
	xMRxFIFO(0x85) = KSeq;	
	// Ciphertext Packet
	rpmemcpy(&xMRxFIFO(0x86), pText, InLen);

	//########################################################
	// Decryption
	//########################################################
	
	#define	DecStart_WaitLoopCnt	5
	#define	DecDone_WaitLoopCnt  	300

	// Decryption
	xMRFCSBASE = 0x80;
	xMRFCSLEN = xMRxFIFO(0x80);		// Length of Packet to transmit	
	
	_EA = EA;
	EA = 0;
	for(iw=0 ; iw<DecStart_WaitLoopCnt; iw++)
	{
		RstAesRsmON;
		_nop_();_nop_();
		RstAesRsmOFF;		
		ClearDecDone;
		ClearAesIntFlag;

		DecStartON;
		DecStartOFF;
		_nop_();_nop_();
		
		if(xMACSTS & 0x80)	break;		// If Enc/Dec_Busy=1, break
	}
	EA = _EA;

	Error = 0;
	if(iw == DecStart_WaitLoopCnt)
	{
		Error = 0xF0;
	}
	else
	{
		for(iw=0 ; iw<DecDone_WaitLoopCnt ; iw++)	// Decryption Duration : typical 50us(Max100us) @8MHz and 89Bytes MSDU
		{
			if(xMACSTS & 0x02)	break;
		}

		if(iw == DecDone_WaitLoopCnt)
		{
			Error = 0xF1;
		}
		else
		{
			Error = 0;
			// Copy Decrypted text (Plaintext)
			rpmemcpy(pText, &xMRxFIFO(0x86), InLen);	
			
			if(xMRxFIFO(0x86+InLen-1) == 0x00)
			{
				*pDecLen = InLen - 4;
			}
			else
			{
				Error = 0xF2;
			}
		}		
	}
	ClearDecDone;
	ClearAesIntFlag;
	//########################################################

	// Disable CTR
	xMACSEC = _xMACSEC;

	// Restore xCLKON0
	xCLKON0 = _xCLKON0;

	ZSYS_RECEIVER_RESUME();

	return	Error;
}

#endif





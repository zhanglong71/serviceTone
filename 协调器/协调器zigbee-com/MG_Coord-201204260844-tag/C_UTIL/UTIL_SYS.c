
/*******************************************************************************
	- Chip		: MG2450/2455
	- Vendor		: RadioPulse Inc, 2007.
	- Date		: 2010-03-02
	- Version		: VER 2.10	

	[2010-03-02] VER 2.10
	- For ZigBeePRO Development Kit V2.10
*******************************************************************************/

#include "INCLUDE_TOP.h"
#include "INCLUDE_STACK.h"

#include "C_HAL/PHY.h"

void Swap_Buffer_4B(UINT8 *pBuf, UINT8 Idx)
{
	UINT8	Temp;
	
	Temp = pBuf[Idx+0];
	pBuf[Idx+0] = pBuf[Idx+3];
	pBuf[Idx+3] = Temp;

	Temp = pBuf[Idx+1];
	pBuf[Idx+1] = pBuf[Idx+2];
	pBuf[Idx+2] = Temp;
}

void Swap_Buffer_2B(UINT8 *pBuf, UINT8 Idx)
{
	UINT8	Temp;

	Temp = pBuf[Idx+0];
	pBuf[Idx+0] = pBuf[Idx+1];
	pBuf[Idx+1] = Temp;
}


//- RETURN	: 1=Null, 0=Not Null
UINT8 Check_8Byte_Null(UINT8 *pArray)
{
	UINT8	ResultOR;

	ResultOR = pArray[0];
	ResultOR |= pArray[1];
	ResultOR |= pArray[2];
	ResultOR |= pArray[3];
	ResultOR |= pArray[4];
	ResultOR |= pArray[5];
	ResultOR |= pArray[6];
	ResultOR |= pArray[7];

	if(ResultOR == 0)		return 1;
	else					return 0;
}


//- RETURN	: 1=Null, 0=Not Null
/*UINT8 Check_16Byte_Null(UINT8 *pArray)
{
	UINT8	ResultOR;
	UINT8	i;

	ResultOR = 0x00;
	for(i=0 ; i<16 ; i++)	ResultOR |= pArray[i];

	if(ResultOR == 0)		return 1;
	else					return 0;
}*/

//- RETURN	: 1=Broadcast, 0=Not
UINT8 Check_8Byte_Broadcast(UINT8 *pArray)
{
	UINT8	ResultAND;
	UINT8	i;

	ResultAND = 0xFF;
	for(i=0 ; i<8 ; i++)	ResultAND &= pArray[i];

	if(ResultAND == 0xFF)	return 1;
	else					return 0;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/***********************************************************************************
 *
 *	NAME		: ZSYS_LQI_TO_COST()
 *
 *	DESCRIPTION	: Converts a LQI value to "COST" value. "COST" is the unit to classify LQI into 7 steps.
 *
 * 	PARAMETER	: LQI - Link Quality Indication(0~255).
 *			- LQI=255	: 0 dBm
 *			- LOI=178	: -30 dBm
 *			- LQI=153	: -40 dBm
 *			- LQI=127	: -50 dBm
 *			- LQI=102	: -60 dBm
 *			- LQI=76		: -70 dBm
 *			- LQI=51		: -80 dBm
 *			- LQI=25		: -90 dBm 
 *
 * 	RETURN		: "COST" value. (1~7)
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
UINT8 ZSYS_LQI_TO_COST(UINT8 LQI)
{
	UINT8 	Cost;

	if(LQI > 127)		Cost = 1;		// -50 dBm
	else if(LQI > 102)	Cost = 2;		// -60 dBm
	else if(LQI > 76)	Cost = 3;		// -70 dBm
	else if(LQI > 51)	Cost = 4;		// -80 dBm
	else if(LQI > 38)	Cost = 5;		// -85 dBm
	else if(LQI > 25)	Cost = 6;		// -90 dBm
	else 			Cost = 7;	

	return	Cost;
}

/***********************************************************************************
 *
 *	NAME		: ZSYS_RANDOM_GEN_SEED()
 *
 *	DESCRIPTION	: Initializes the random generator with the seed value
 *
 * 	PARAMETER	: RandomSeed - seed for initialization
 *
 * 	RETURN		: None
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
void ZSYS_RANDOM_GEN_SEED(UINT16 RandomSeed)
{
	srand(RandomSeed);
}

/***********************************************************************************
 *
 *	NAME		: ZSYS_RANDOM_GEN_GET()
 *
 *	DESCRIPTION	: Acquires a 16-bit random value
 *
 * 	PARAMETER	: MaskAnd - The generated random number is AND-ed with this before returning.
 *
 * 	RETURN		: 16-bit random number
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
UINT16 ZSYS_RANDOM_GEN_GET(UINT16 MaskAnd)
{
	UINT16	random_return;

	random_return = rand() & MaskAnd;
	random_return = random_return + 0x08;
	
	return	random_return;		// Min=8, Max=8 + MaskAnd
}

/***********************************************************************************
 *
 *	NAME		: ZSYS_SET_SHORT_ADDR()
 *
 *	DESCRIPTION	: Set network address
 *
 * 	PARAMETER	: ShortAddr - network address to set
 *
 * 	RETURN		: None
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
void ZSYS_SET_SHORT_ADDR(UINT16 ShortAddr)
{
	NIB.nwkNetworkAddr = ShortAddr;
	MPIB.macShortAddr = ShortAddr;
	ZHAL_SHORT_ADDR_SET(MPIB.macShortAddr);
}

/***********************************************************************************
 *
 *	NAME		: ZSYS_SET_PAN_ID()
 *
 *	DESCRIPTION	: Set PAN identifier
 *
 * 	PARAMETER	: PanID - PAN identifier to set
 *
 * 	RETURN		: None
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
void ZSYS_SET_PAN_ID(UINT16 PanID)
{
	NIB.nwkPanID = PanID;
	MPIB.macPANId = PanID;
	ZHAL_PAN_ID_SET(MPIB.macPANId);
}

/***********************************************************************************
 *
 *	NAME		: ZSYS_SET_CHANNEL()
 *
 *	DESCRIPTION	: Set RF channel
 *
 * 	PARAMETER	: Channel - RF channel to set
 *
 * 	RETURN		: None
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
void ZSYS_SET_CHANNEL(UINT8 Channel)
{
	PPIB.phyCurrentChannel = Channel;
	ZHAL_CHANNEL_SET(PPIB.phyCurrentChannel);
}

/***********************************************************************************
 *
 *	NAME		: ZSYS_SET_EPID()
 *
 *	DESCRIPTION	: Set Extended PAN identifier(EPID)
 *
 * 	PARAMETER	: pEPID - pointer to EPID to set
 *
 * 	RETURN		: None
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
void ZSYS_SET_EPID(UINT8 *pEPID)
{
	memcpy(NIB.nwkEPID, pEPID, 8);
}

/***********************************************************************************
 *
 *	NAME		: ZSYS_GET_SHORT_ADDR()
 *
 *	DESCRIPTION	: Get network address
 *
 * 	PARAMETER	: None
 *
 * 	RETURN		: Network address
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
/*UINT16 ZSYS_GET_SHORT_ADDR()
{
	return	NIB.nwkNetworkAddr;
}*/

/***********************************************************************************
 *
 *	NAME		: ZSYS_GET_PAN_ID()
 *
 *	DESCRIPTION	: Get PAN identifier
 *
 * 	PARAMETER	: None
 *
 * 	RETURN		: PAN identifier
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
//UINT16 ZSYS_GET_PAN_ID()
//{
//	return	NIB.nwkPanID;
//}

/***********************************************************************************
 *
 *	NAME		: ZSYS_GET_CHANNEL()
 *
 *	DESCRIPTION	: Get RF channel
 *
 * 	PARAMETER	: None
 *
 * 	RETURN		: RF channel
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
UINT8 ZSYS_GET_CHANNEL()
{
	return	PPIB.phyCurrentChannel;
}

/***********************************************************************************
 *
 *	NAME		: RemoveWarning_UTIL_SYS()
 *
 *	DESCRIPTION	: Pretends to call functions in this file to remove warnings of Keil Compiler.
 *		In fact, the functions are not called because "Call" is 0.
 *
 * 	PARAMETER	: None
 *
 * 	RETURN		: None
 *
 * 	NOTES		: A warning that indicates UNCALLED_SEGMENT about RemoveWarning_UTIL_SYS()
 *		is generated when compiled. This warning is ignored.
 *	
 ***********************************************************************************/
/*void RemoveWarning_UTIL_SYS()
{
	UINT8	Call;

	Call = 0;

	if(Call)
	{
		Swap_Buffer_4B(0,0);
		Swap_Buffer_2B(0,0);
		Check_8Byte_Null(0);
		Check_16Byte_Null(0);
		Check_8Byte_Broadcast(0);
		ZSYS_RANDOM_GEN_SEED(0);
		ZSYS_RANDOM_GEN_GET(0);
		ZSYS_SET_CHANNEL(0);
		ZSYS_SET_SHORT_ADDR(0);
		ZSYS_SET_PAN_ID(0);
		ZSYS_SET_EPID(0);	
		ZSYS_GET_SHORT_ADDR();
		ZSYS_GET_PAN_ID();
		ZSYS_GET_CHANNEL();
	}
}*/


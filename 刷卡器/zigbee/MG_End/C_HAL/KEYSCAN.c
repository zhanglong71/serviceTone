/*******************************************************************************
	- Chip		: MG2450/2455
	- Vendor		: RadioPulse Inc, 2007.
	- Date		: 2007-11-07
	- Version		: VER 1.5	
*******************************************************************************/

#include "INCLUDE_TOP.h"
//
//	P0[0] ~ P0[7] is ANDed and the result value at P1.7
//	The P0MASK is '1', the selected bit is ignored
//
void ZHAL_KEYSCAN_SET(UINT8 ScanEN, UINT8 P0andEN, UINT8 P0mask)
{	
	P0MSK = P0mask;
	
	if(ScanEN) 
		P0SEL |= 0x02;
	else
		P0SEL &= ~0x02;

	if(P0andEN) 
		P0SEL |= 0x01;
	else
		P0SEL &= ~0x01;
}

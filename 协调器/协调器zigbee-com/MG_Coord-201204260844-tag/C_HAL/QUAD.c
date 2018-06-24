/*******************************************************************************
	- Chip		: MG2450/2455
	- Vendor		: RadioPulse Inc, 2007.
	- Date		: 2007-11-07
	- Version		: VER 1.5	
*******************************************************************************/

#include "INCLUDE_TOP.h"



//*******************************************************************
//QUAD Function (QUAD SET Function)
//*******************************************************************
#define	Q_x		0x01
#define	Q_y		0x02
#define	Q_z		0x03

//	Quad control Register 
//	QCTL 
// 	B[0] - MODE	: 1-ENC/0-DEC
//	B[1] - INI	: INITIAL
//	B[2] - ENA	: ENABLE
//
void	ZHAL_QUAD_SET(UINT8 EN, UINT8 MODE )
{
	//UINT8	Q_Port;
	xQCTL = 0x00 | (EN<<2) | MODE; 
}
	
//*******************************************************************
//QUAD Function (QUAD Encoder Function)
//*******************************************************************

void	ZHAL_QUAD_ENC(UINT8 Quad_Port, UINT8 *pDir, UINT8 *pCount)
{	
	switch(Quad_Port)
	{
		case Q_x:
			
			*pDir 	= xUDX;
			*pCount = xCNTX;
			break;
		case Q_y:
			*pDir 	= xUDY;
			*pCount = xCNTY;
			break;
		case Q_z:
			*pDir 	= xUDZ;
			*pCount = xCNTZ;
			break;
		default:
			break;
	}
}	

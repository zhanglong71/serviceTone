
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

#include "C_HAL/UART.h"
#include "C_HAL/WDT.h"

#define	KEYBOARD_BACK		0x08
#define	KEYBOARD_CarryRet	0x0D
#define	KEYBOARD_LineFeed	0x0A
#define	KEYBOARD_SPACE	0x20

/***********************************************************************************
 *
 *	NAME		: zPrintf()
 *
 *	DESCRIPTION	: Formats a series of strings and numeric values. And, it builds a string to write 
 *		via UART. It is similar to the printf() of the general C-Language
 *
 * 	PARAMETER	: 
 *		Port - UART port number. 0=UART0, 1=UART1
 *		fmt - pointer to a format string
 *
 * 	RETURN		: None
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
void zPrintf(UINT8 Port, char *fmt, ...)
{
	va_list	arg_ptr;
	char		LocalText[64];
	UINT8	cnt;
	
	ZSYS_WDT_SET(10000);

	for(cnt=0 ; cnt<64 ; cnt++) 
	{
		LocalText[cnt] = 0x00;
	}

	va_start(arg_ptr, fmt);
	vsprintf(LocalText, fmt, arg_ptr);
	va_end(arg_ptr);

	if(Port & 0x01)		// SERIAL PORT1
	{
		#if (ENABLE_UART1)
		for(cnt=0 ; cnt<64 ; cnt++) 
		{
			if(LocalText[cnt] != 0)
			{
				ZSYS_UART1_PUT(LocalText[cnt]);
				if(LocalText[cnt] == KEYBOARD_LineFeed)	
					ZSYS_UART1_PUT(KEYBOARD_CarryRet);			
			}
			else		break;
		}	
		#endif
	}
	else 					// SERIAL PORT0
	{
		#if (ENABLE_UART0)
		for(cnt=0 ; cnt<64 ; cnt++)
		{
			if(LocalText[cnt] != 0)
			{			
				ZSYS_UART0_PUT(LocalText[cnt]);
				if(LocalText[cnt] == KEYBOARD_LineFeed)	
					ZSYS_UART0_PUT(KEYBOARD_CarryRet);
			}
			else		break;
		}	
		#endif
	}
}


/***********************************************************************************
 *
 *	NAME		: zScanf()
 *
 *	DESCRIPTION	: Stores the keyboard input data. Input is in hexa-decimal number.
 *
 * 	PARAMETER	: 
 *		Port - UART port number. 0=UART0, 1=UART1
 *		pValue - a pointer to the parameters in which data is stored
 *
 * 	RETURN		: None
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
void zScanf(UINT8 Port, UINT16 *pValue)
{
	char		integer;
	UINT8	ki;
	UINT8	cnt;

	cnt = 0;
	*pValue = 0;

	while(1) 
	{		
		ZSYS_WDT_SET(10000);

		if(Port & 0x01)
		{
			#if (ENABLE_UART1)
			if(ZSYS_UART1_GET(&ki)) 
			{
				integer = toint(ki);

				if(integer != -1) 
				{
					if(cnt != 4)
					{
						ZSYS_UART1_PUT(ki);
						*pValue <<= 4;
						*pValue |= integer;
						cnt++;
					}
				}
				else 
				{
					if( (ki == KEYBOARD_BACK) && (cnt) ) 	// BackSpace
					{
						ZSYS_UART1_PUT(KEYBOARD_BACK);
						ZSYS_UART1_PUT(KEYBOARD_SPACE);
						ZSYS_UART1_PUT(KEYBOARD_BACK);
						*pValue = *pValue >> 4;
						cnt--;
					}
				
					if(ki == KEYBOARD_CarryRet)
					{
						break;
					}
				}
			}	
			#endif		
		}
		else
		{
			#if (ENABLE_UART0)
			if(ZSYS_UART0_GET(&ki)) 
			{
				integer = toint(ki);

				if(integer != -1) 
				{
					if(cnt != 4)
					{
						ZSYS_UART0_PUT(ki);
						*pValue <<= 4;				
						*pValue |= integer;
						cnt++;
					}
				}
				else 
				{				
					if( (ki == KEYBOARD_BACK) && (cnt) ) 	// BackSpace
					{
						ZSYS_UART0_PUT(KEYBOARD_BACK);
						ZSYS_UART0_PUT(KEYBOARD_SPACE);
						ZSYS_UART0_PUT(KEYBOARD_BACK);
						*pValue = *pValue >> 4;
						cnt--;
					}

					if(ki == KEYBOARD_CarryRet) 	// Enter
					{
						break;
					}				
				}
			}			
			#endif
			
		}
	
	}

}

//- Port	: 1=UART1, 0=UART0

//- 	RETURN
//	1	: Valid Return
//	0	: Invalid Return


/***********************************************************************************
 *
 *	NAME		: zScanf_Decimal()
 *
 *	DESCRIPTION	: Stores the keyboard input data. Input is in decimal number.
 *
 * 	PARAMETER	: 
 *		Port - UART port number. 0=UART0, 1=UART1
 *		pValue - a pointer to the parameters in which data is stored
 *
 * 	RETURN		: None
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
UINT8 zScanf_Decimal(UINT8 Port, UINT16 *pValue)
{
	char		integer;
	UINT8	ki;
	UINT8	cnt;
	UINT8	Digit[5];
	UINT32	Tmp32;
	
	cnt = 0;
	*pValue = 0;
	Digit[0] = 0;
	Digit[1] = 0;
	Digit[2] = 0;
	Digit[3] = 0;
	Digit[4] = 0;	

	while(1) 
	{		
		ZSYS_WDT_SET(10000);

		if(Port & 0x01)
		{
			#if (ENABLE_UART1)
			if(ZSYS_UART1_GET(&ki)) 
			{
				if( (ki >= '0') && (ki <= '9') )
				{
					integer = ki - '0';

					if(integer < 10)
					{
						if(cnt != 5)
						{
							ZSYS_UART1_PUT(ki);						
							Digit[4] = Digit[3];
							Digit[3] = Digit[2];
							Digit[2] = Digit[1];
							Digit[1] = Digit[0];
							Digit[0] = integer;
							cnt++;
						}
					}				
				}
				else
				{
					if( (ki == KEYBOARD_BACK) && (cnt) ) 	// BackSpace
					{					
						ZSYS_UART1_PUT(KEYBOARD_BACK);
						ZSYS_UART1_PUT(KEYBOARD_SPACE);
						ZSYS_UART1_PUT(KEYBOARD_BACK);

						Digit[0] = Digit[1];
						Digit[1] = Digit[2];
						Digit[2] = Digit[3];
						Digit[3] = Digit[4];
						Digit[4] = 0;
						cnt--;
					}
				
					if(ki == KEYBOARD_CarryRet)	
					{
						Tmp32 = Digit[4];	Tmp32 *= 10000;
						Tmp32 += (Digit[3] * 1000);
						Tmp32 += (Digit[2] * 100);
						Tmp32 += (Digit[1] * 10);
						Tmp32 += (Digit[0] * 1);
						if(Tmp32 & 0xFFFF0000)
						{
							return	0;
						}
						else	
						{
							*pValue = Tmp32;
							return	1;
						}
					}				
				}			
			}	
			#endif
		}
		else
		{
			#if (ENABLE_UART0)
			if(ZSYS_UART0_GET(&ki)) 
			{
				if( (ki >= '0') && (ki <= '9') )
				{
					integer = ki - '0';

					if(integer < 10)
					{
						if(cnt != 5)
						{
							ZSYS_UART0_PUT(ki);						
							Digit[4] = Digit[3];
							Digit[3] = Digit[2];
							Digit[2] = Digit[1];
							Digit[1] = Digit[0];
							Digit[0] = integer;
							cnt++;
						}
					}				
				}
				else
				{
					if( (ki == KEYBOARD_BACK) && (cnt) ) 	// BackSpace
					{					
						ZSYS_UART0_PUT(KEYBOARD_BACK);
						ZSYS_UART0_PUT(KEYBOARD_SPACE);
						ZSYS_UART0_PUT(KEYBOARD_BACK);

						Digit[0] = Digit[1];
						Digit[1] = Digit[2];
						Digit[2] = Digit[3];
						Digit[3] = Digit[4];
						Digit[4] = 0;
						cnt--;
					}
				
					if(ki == KEYBOARD_CarryRet)	
					{
						Tmp32 = Digit[4];	Tmp32 *= 10000;
						Tmp32 += (Digit[3] * 1000);
						Tmp32 += (Digit[2] * 100);
						Tmp32 += (Digit[1] * 10);
						Tmp32 += (Digit[0] * 1);
						if(Tmp32 & 0xFFFF0000)
						{
							return	0;
						}
						else	
						{
							*pValue = Tmp32;
							return	1;
						}
					}				
				}			
			}
			#endif
		}	
	}
}

/***********************************************************************************
 *
 *	NAME		: Display_Buffer()
 *
 *	DESCRIPTION	: displays data memory which is pointed by "pBuf"
 *
 * 	PARAMETER	: 
 *		pBuf - a pointer to data memory to be displayed
 *		Len - length of data memory
 *		Linefeed - linefeed length
 *
 * 	RETURN		: None
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
void Display_Buffer(UINT8 *pBuf, UINT8 Len, UINT8 Linefeed)
{
	UINT8	ib;

	for(ib=0 ; ib<Len ; ib++)
	{	
		if(Linefeed)
		{
			if( (ib % Linefeed) == 0)		zPrintf(1, "\n");
		}
		zPrintf(1, " %02x",(short)pBuf[ib]);
	}
}

/***********************************************************************************
 *
 *	NAME		: Display_NT()
 *
 *	DESCRIPTION	: displays entries of the neighbor table
 *
 * 	PARAMETER	: DispOpt - option for displaying
 * 					- 0x00	: Enabled Entry(All Fields)
 *					- 0x01	: Enabled Entry(Important Fields Only)
 *					- 0xFF	: All Entry(All Fields)
 *
 * 	RETURN		: None
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
void Display_NT(UINT8 DispOpt)
{
	UINT8	N;

	zPrintf(1, "\n <NT>");
	for(N=0 ; N<NIB.SIZE_NT ; N++)
	{
		if(	(DispOpt == 0xFF)
		||	( (DispOpt != 0xFF) && (NIB.pNT[N].EnaNT) ) )
		{
			zPrintf(1, "\n Idx=%02x",(short)N);
			zPrintf(1, " :Ena=%02x",(short)NIB.pNT[N].EnaNT);
			zPrintf(1, " :Nwk=%04x",(short)NIB.pNT[N].NwkAddr);			
			zPrintf(1, " :IEEE[7:0]=%02x..%02x%02x",(short)NIB.pNT[N].IEEEAddr[7],(short)NIB.pNT[N].IEEEAddr[1],(short)NIB.pNT[N].IEEEAddr[0]);
			zPrintf(1, " :Type=%02x",(short)NIB.pNT[N].DevType);
			zPrintf(1, " :Rela=%02x",(short)NIB.pNT[N].Relation);
			zPrintf(1, " :RxOn=%02x",(short)NIB.pNT[N].RxOnWhenIdle);

			if(DispOpt == 0x00)
			{
				zPrintf(1, " :OutCost=%02x",(short)NIB.pNT[N].OutCost);
				zPrintf(1, " :LQI=%02x",(short)NIB.pNT[N].NtLQI);
				zPrintf(1, " :Fail=%02x",(short)NIB.pNT[N].TxFailure);
				zPrintf(1, " :IEEE[7:0]=");
				zPrintf(1, "%02x%02x",(short)NIB.pNT[N].IEEEAddr[7],(short)NIB.pNT[N].IEEEAddr[6]);
				zPrintf(1, "_%02x%02x",(short)NIB.pNT[N].IEEEAddr[5],(short)NIB.pNT[N].IEEEAddr[4]);
				zPrintf(1, "_%02x%02x",(short)NIB.pNT[N].IEEEAddr[3],(short)NIB.pNT[N].IEEEAddr[2]);
				zPrintf(1, "_%02x%02x",(short)NIB.pNT[N].IEEEAddr[1],(short)NIB.pNT[N].IEEEAddr[0]);
			}
		}	
	}
}

/***********************************************************************************
 *
 *	NAME		: Display_RT()
 *
 *	DESCRIPTION	: displays entries of the routing table
 *
 * 	PARAMETER	: DispOpt - option for displaying
 * 					- 0x00	: Enabled Entry
 *					- 0xFF	: All Entry
 *
 * 	RETURN		: None
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
void Display_RT(UINT8 DispOpt)
{
	UINT8	N;

	zPrintf(1, "\n <RT>");
	for(N=0 ; N<NIB.SIZE_RT ; N++)
	{	
		if( 	(DispOpt == 0xFF)
		||	( (DispOpt == 0) && (NIB.pRT[N].EnaRT) ) )
		{		
			zPrintf(1, "\n Idx=%02x",(short)N);
			zPrintf(1, " :Ena=%x",(short)NIB.pRT[N].EnaRT);
			zPrintf(1, " :Sta=%02x",(short)NIB.pRT[N].Sta);	
			zPrintf(1, " :Dst=%04x",(short)NIB.pRT[N].DstAddr);
			zPrintf(1, " :Next=%04x",(short)NIB.pRT[N].NextHop);
			zPrintf(1, " :Opt=%02x",(short)NIB.pRT[N].RtOption);
		}	
	}
}

/***********************************************************************************
 *
 *	NAME		: Display_NAM()
 *
 *	DESCRIPTION	: displays entries of the network address map table
 *
 * 	PARAMETER	: DispOpt - option for displaying
 * 					- 0x00	: Enabled Entry
 *					- 0xFF	: All Entry
 *
 * 	RETURN		: None
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
void Display_NAM(UINT8 DispOpt)
{
	UINT8	N;

	zPrintf(1, "\n <NAM>");
	for(N=0 ; N<NIB.SIZE_NAM ; N++)
	{
		if(	(DispOpt == 0xFF)
		||	( (DispOpt == 0) && (NIB.pNAM[N].EnaNAM) ) )
		{
			zPrintf(1, "\n Idx=%02x",(short)N);
			zPrintf(1, " :Ena=%x",(short)NIB.pNAM[N].EnaNAM);
			zPrintf(1, " :Nwk=%04x",NIB.pNAM[N].NwkAddr);
			zPrintf(1, " :IEEE[7:0]=");
			zPrintf(1, "%02x%02x",(short)NIB.pNAM[N].IEEEAddr[7],(short)NIB.pNAM[N].IEEEAddr[6]);
			zPrintf(1, "_%02x%02x",(short)NIB.pNAM[N].IEEEAddr[5],(short)NIB.pNAM[N].IEEEAddr[4]);
			zPrintf(1, "_%02x%02x",(short)NIB.pNAM[N].IEEEAddr[3],(short)NIB.pNAM[N].IEEEAddr[2]);
			zPrintf(1, "_%02x%02x",(short)NIB.pNAM[N].IEEEAddr[1],(short)NIB.pNAM[N].IEEEAddr[0]);
		}	
	}
}

/***********************************************************************************
 *
 *	NAME		: Display_RRECT()
 *
 *	DESCRIPTION	: displays entries of the route record table
 *
 * 	PARAMETER	: DispOpt - option for displaying
 * 					- 0x00	: Enabled Entry
 *					- 0xFF	: All Entry
 *
 * 	RETURN		: None
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
void Display_RRECT(UINT8 DispOpt)
{
	UINT8	N;
	UINT8	ib;

	for(N=0 ; N<NIB.SIZE_RRECT ; N++)
	{
		if(	(DispOpt == 0xFF)
		||	( (DispOpt == 0) && (NIB.pRRECT[N].EnaRRECT) ) )
		{
			zPrintf(1, "\n Idx=%02x",(short)N);
			zPrintf(1, " :Ena=%x",(short)NIB.pRRECT[N].EnaRRECT);
			zPrintf(1, " :RelayCnt=%04x",NIB.pRRECT[N].RelayCount);
			zPrintf(1, " :NwkAddr=%04x",NIB.pRRECT[N].NwkAddr);						
			for(ib=0 ; ib<NIB.pRRECT[N].RelayCount ; ib++)
			{
				zPrintf(1, "<-%04x",NIB.pRRECT[N].Path[ib]);
			}			
			zPrintf(1, "(Path)");
		}
	}	
}

/***********************************************************************************
 *
 *	NAME		: Display_GT()
 *
 *	DESCRIPTION	: displays entries of the group table
 *
 * 	PARAMETER	: DispOpt - option for displaying
 * 					- 0x00	: Enabled Entry
 *					- 0xFF	: All Entry
 *
 * 	RETURN		: None
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
void Display_GT(UINT8 DispOpt)
{
	UINT8	N;

	for(N=0 ; N<AIB.SIZE_GT ; N++)
	{	
		if( 	(DispOpt == 0xFF)
		||	( (DispOpt == 0) && (AIB.papsGroupTable[N].EnaGT) ) )
		{
			zPrintf(1, "\n Idx=%02x",(short)N);
			zPrintf(1, " :Ena=%x",(short)AIB.papsGroupTable[N].EnaGT);			
			zPrintf(1, " :Group=%04x",AIB.papsGroupTable[N].GroupAddr);			
			zPrintf(1, " :EP=%02x",(short)AIB.papsGroupTable[N].GroupEP);
		}	
	}
}

/***********************************************************************************
 *
 *	NAME		: Display_MyInfo()
 *
 *	DESCRIPTION	: displays the type, RF channel, network address, PAN ID, EPID and IEEE address
 *
 * 	PARAMETER	: None
 *
 * 	RETURN		: None
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
void Display_MyInfo()
{
	zPrintf(1, "\n === Xiexie MIEO!");
	zPrintf(1, "\n - DEVTYPE=%c, Chan=%02d",NIB.DEVTYPE,(short)PPIB.phyCurrentChannel);
	zPrintf(1, "\n - NwkShort=%04x, MacShort=%04x",NIB.nwkNetworkAddr,MPIB.macShortAddr);
	zPrintf(1, "\n - NwkPan=%04x, MacPan=%04x",NIB.nwkPanID,MPIB.macPANId);			
	zPrintf(1, "\n - EPID(0~7) : ");
	Display_Buffer(NIB.nwkEPID, 8, 0);			
	zPrintf(1, "\n - IEEE(0~7) : ");
	Display_Buffer(MPIB.IEEE_ExtendAddr, 8, 0);
}



///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/***********************************************************************************
 *
 *	NAME		: ZSYS_HIB_COPY()
 *
 *	DESCRIPTION	: reads HIB and copies it to the data memory pointed by "pBuf"
 *
 * 	PARAMETER	: pBuf - a pointer to the data memory to which HIB is copied
 *
 * 	RETURN		: 1=Success. HIB is valid. 0=Failed. HIB is invalid.
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
UINT8 ZSYS_HIB_COPY(UINT8 *pBuf)
{
	UINT8	i;
	UINT8	code *pCodeBuf;
	UINT8	SUM;

	pCodeBuf = HIB_ADDRESS;
	SUM = 0;
	for(i=0 ; i<HIB_LENGTH ; i++)
	{
		SUM += pCodeBuf[i];
		pBuf[i] = pCodeBuf[i];
	}

	if(SUM == 0)	return	1;
	else			return	0;	
}


/***********************************************************************************
 *
 *	NAME		: ZSYS_LQI_TO_dBm()
 *
 *	DESCRIPTION	: converts a LQI value to a dBm value
 *
 * 	PARAMETER	: LQI - LQI value to be converted
 *
 * 	RETURN		: converted dBm value
 *
 * 	NOTES		: LQI=255 --> 0 dBm. LQI=0 --> -100 dBm
 *	
 ***********************************************************************************/
INT8 ZSYS_LQI_TO_dBm(UINT8 LQI)
{
	UINT16	Tmp16;
	INT8		Tmp8;

	Tmp16 = 100 * (255 - LQI);
	Tmp16 = Tmp16 / 255;

	Tmp8 = 0 - Tmp16;
	
	return	Tmp8;
}

/***********************************************************************************
 *
 *	NAME		: ZSYS_dBm_TO_LQI()
 *
 *	DESCRIPTION	: converts a dBm value to a LQI value
 *
 * 	PARAMETER	: dBmValue - dBm value to be converted
 *
 * 	RETURN		: converted LQI value
 *
 * 	NOTES		: LQI=255 --> 0 dBm. LQI=0 --> -100 dBm
 *	
 ***********************************************************************************/
UINT8 ZSYS_dBm_TO_LQI(INT8 dBmValue)
{
	UINT16	Tmp16;
	UINT8	ConvertedLQI;

	// RSSI : LQI type, 0~255, 255=high power
	if(dBmValue > 0)
	{
		ConvertedLQI = 255;
	}
	else if(dBmValue > -100)
	{
		Tmp16 = 100 + dBmValue;
		Tmp16 *= 255;
		Tmp16 /= 100;
		ConvertedLQI = Tmp16;
	}
	else
	{
		ConvertedLQI = 0;
	}	
   //return 0xFF;
	return	ConvertedLQI;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


/***********************************************************************************
 *
 *	NAME		: RemoveWarning_UTIL_APP()
 *
 *	DESCRIPTION	: Pretends to call functions in this file to remove warnings of Keil Compiler.
 *		In fact, the functions are not called because "Call" is 0.
 *
 * 	PARAMETER	: None
 *
 * 	RETURN		: None
 *
 * 	NOTES		: A warning that indicates UNCALLED_SEGMENT about RemoveWarning_UTIL_APP()
 *		is generated when compiled. This warning is ignored.
 *	
 ***********************************************************************************/
void RemoveWarning_UTIL_APP()
{
	UINT8	Call;

	Call = 0;

	if(Call)
	{
		zPrintf(0,0);
		zScanf(0,0);
		zScanf_Decimal(0,0);
		Display_Buffer(0,0,0);	
		Display_NT(0);
		Display_RT(0);
		Display_NAM(0);	
		Display_RRECT(0);
		Display_GT(0);
		Display_MyInfo();

		ZSYS_HIB_COPY(0);
		ZSYS_LQI_TO_dBm(0);
		ZSYS_dBm_TO_LQI(0);
	}
}

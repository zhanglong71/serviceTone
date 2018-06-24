

/*******************************************************************************
	- Chip		: MG2450/2455
	- Vendor		: RadioPulse Inc, 2007.
	- Date		: 2010-03-02
	- Version		: VER 2.10	

	[2010-03-02] VER 2.10
	- For ZigBeePRO Development Kit V2.10
*******************************************************************************/

#include "INCLUDE_TOP.h"

#include "C_HAL/UART.h"

	UINT8	INT_EXT0 = 0;
/***********************************************************************************
 *
 *	NAME		: EXT0_ISR()
 *
 *	DESCRIPTION	: Interrupt Service Routine(ISR) of External Interrupt 0
 *
 * 	PARAMETER	: None
 *
 * 	RETURN		: None
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/	
void EXT0_ISR(void) interrupt 0 
{
	INT_EXT0 = 1;	
}


	UINT8	INT_EXT1 = 0;
/***********************************************************************************
 *
 *	NAME		: EXT1_ISR()
 *
 *	DESCRIPTION	: Interrupt Service Routine(ISR) of External Interrupt 1
 *
 * 	PARAMETER	: None
 *
 * 	RETURN		: None
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/	
void EXT1_ISR(void) interrupt 2 
{
	INT_EXT1 = 1;
}

	UINT16	UserTimer0 = 0;
	UINT16	UserTimer1 = 0;
	UINT16	UserTimer2 = 0;
	UINT16	UserTimer3 = 0;

	UINT8	Timer1_Reload_TH1;
	UINT8	Timer1_Reload_TL1;

	UINT8	Timer_comDelay;


/***********************************************************************************
 *
 *	NAME		: TIMER1_ISR()
 *
 *	DESCRIPTION	: Interrupt Service Routine(ISR) of TIMER1
 *
 * 	PARAMETER	: None
 *
 * 	RETURN		: None
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
void TIMER1_ISR(void) interrupt 3 
{
	TH1 = Timer1_Reload_TH1;
	TL1 = Timer1_Reload_TL1;

	if(UserTimer0)	UserTimer0--;
	if(UserTimer1)	UserTimer1--;
	if(UserTimer2)	UserTimer2--;
	if(UserTimer3)	UserTimer3--;
	
	if(Timer_comDelay) Timer_comDelay--;
}

/***********************************************************************************
 *
 *	NAME		: ZSYS_SET_UserTimer()
 *
 *	DESCRIPTION	: In TIMER1_ISR(), there are 4 virtual count-down timers for the application
 *		This function sets a timer among them.
 *
 * 	PARAMETER	: 
 *		TimerNum - The number of a virtual timer. 0 ~ 3.
 *		TickCount - The number of ticks before expiration.
 *
 * 	RETURN		: None
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
void ZSYS_SET_UserTimer(UINT8 TimerNum, UINT16 TickCount)
{
	UINT8	_ET1;

	_ET1 = ET1;
	ET1 = 0;

	if(TimerNum == 0)		UserTimer0 = TickCount;
	else if(TimerNum == 1)	UserTimer1 = TickCount;
	else if(TimerNum == 2)	UserTimer2 = TickCount;
	else if(TimerNum == 3)	UserTimer3 = TickCount;

	else if(TimerNum == 4)	Timer_comDelay = TickCount;
	
	ET1 = _ET1;
}


/***********************************************************************************
 *
 *	NAME		: ZSYS_SET_UserTimer()
 *
 *	DESCRIPTION	: gets the remaining tick number of a specified virtual timer
 *
 * 	PARAMETER	: TimerNum - The number of a virtual timer. 0 ~ 3.
 *
 * 	RETURN		: Number of remaining ticks
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
UINT16 ZSYS_GET_UserTimer(UINT8 TimerNum)
{
	UINT16	ReturnTick;

	UINT8	_ET1;

	_ET1 = ET1;
	ET1 = 0;

	if(TimerNum == 0)		ReturnTick = UserTimer0;
	else if(TimerNum == 1)	ReturnTick = UserTimer1;
	else if(TimerNum == 2)	ReturnTick = UserTimer2;
	else if(TimerNum == 3)	ReturnTick = UserTimer3;
	else if(TimerNum == 4)	ReturnTick = Timer_comDelay;
	
	else						ReturnTick = 0;

	ET1 = _ET1;

	return	ReturnTick;
}


/***********************************************************************************
 *
 *	NAME		: TIMER3_ISR()
 *
 *	DESCRIPTION	: Interrupt Service Routine(ISR) of TIMER3
 *
 * 	PARAMETER	: None
 *
 * 	RETURN		: None
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
void TIMER3_ISR(void) interrupt 11
{
	EXIF &= 0x7F;
}


/***********************************************************************************
 *
 *	NAME		: UART0_ISR()
 *
 *	DESCRIPTION	: Interrupt Service Routine(ISR) of UART0
 *
 * 	PARAMETER	: None
 *
 * 	RETURN		: None
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
void UART0_ISR (void) interrupt 4
{
#if ENABLE_UART0
	UINT8	INT_ID;
	UINT8	PendLen;
	UINT8	i;
	UINT8	NextNum;

	INT_ID = (xU0_IIR >> 1) & 0x07;
	
	if(INT_ID == 0x02)		// Received Data Available
	{
		for(i=0 ; i<8 ; i++)
		{
			NextNum = (r0_in+1) & (UART0_RXBUF_SIZE-1);
			if(NextNum != r0_out)
			{
				r0buf[r0_in] = xU0_RBR;
				r0_in = NextNum;
			}
			else		PendLen = xU0_RBR;
		}
	}
	else if(INT_ID == 0x06)	// Character Timeout
	{
		// Timeout Interrupt
		NextNum = (r0_in+1) & (UART0_RXBUF_SIZE-1);
		if(NextNum != r0_out)
		{
			r0buf[r0_in] = xU0_RBR;
			r0_in = NextNum;
		}
		else		PendLen = xU0_RBR;	
	}
	else if(INT_ID == 0x01)	// Tx Holding Empty
	{
		t0_fifo_empty = 1;
		if(t0_in != t0_out)
		{
			xU0_THR = t0buf[t0_out];
			t0_out++;
			t0_out &= (UART0_TXBUF_SIZE-1);			
			t0_fifo_empty = 0;
		}	
	}
	else if(INT_ID == 0x03)	// Receiver Line Status
	{		
		NextNum = xU0_LSR;
	}
	else if(INT_ID == 0x00)	// Modem Status
	{
		NextNum = xU0_MSR;
	}
	else						// Unknown ???
	{
	}	
#else
	UINT8	INT_ID;
	
	INT_ID = (xU0_IIR >> 1) & 0x07;
#endif
}


/***********************************************************************************
 *
 *	NAME		: UART1_ISR()
 *
 *	DESCRIPTION	: Interrupt Service Routine(ISR) of UART1
 *
 * 	PARAMETER	: None
 *
 * 	RETURN		: None
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
void UART1_ISR (void) interrupt 7
{
#if (ENABLE_UART1)
	UINT8	INT_ID;
	UINT8	PendLen;
	UINT8	i;
	UINT8	NextNum;

	INT_ID = (xU1_IIR >> 1) & 0x07;
	
	if(INT_ID == 0x02)		// Received Data Available
	{
		for(i=0 ; i<8 ; i++)
		{
			NextNum = (r1_in+1) & (UART1_RXBUF_SIZE-1);
			if(NextNum != r1_out)
			{
				r1buf[r1_in] = xU1_RBR;
				r1_in = NextNum;
			}
			else		PendLen = xU1_RBR;
		}
	}
	else if(INT_ID == 0x06)	// Character Timeout
	{
		// Timeout Interrupt
		NextNum = (r1_in+1) & (UART1_RXBUF_SIZE-1);
		if(NextNum != r1_out)
		{
			r1buf[r1_in] = xU1_RBR;
			r1_in = NextNum;
		}
		else		PendLen = xU1_RBR;
	}
	else if(INT_ID == 0x01)	// Tx Holding Empty
	{		
		t1_fifo_empty = 1;
		if(t1_in != t1_out)		
		{
			xU1_THR = t1buf[t1_out];
			t1_out++;
			t1_out &= (UART1_TXBUF_SIZE-1);			
			t1_fifo_empty = 0;
		}				
	}
	else if(INT_ID == 0x03)	// Receiver Line Status
	{		
		NextNum = xU1_LSR;
	}
	else if(INT_ID == 0x00)	// Modem Status
	{
		NextNum = xU1_MSR;
	}
	else						// Unknown ???
	{
	}	
#else
	UINT8	INT_ID;

	INT_ID = (xU1_IIR >> 1) & 0x07;
#endif
}


	UINT8	INT_AES = 0;
/***********************************************************************************
 *
 *	NAME		: AES_ISR()
 *
 *	DESCRIPTION	: Interrupt Service Routine(ISR) of H/W AES Block.
 *
 * 	PARAMETER	: None
 *
 * 	RETURN		: None
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/	
void AES_ISR(void) interrupt 10 
{	
	EXIF &= 0xBF; 
	INT_AES = 1;
}


	UINT8	INT_RTC = 0;
/***********************************************************************************
 *
 *	NAME		: RTC_ISR()
 *
 *	DESCRIPTION	: Interrupt Service Routine(ISR) of RTC
 *
 * 	PARAMETER	: None
 *
 * 	RETURN		: None
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
void RTC_ISR(void) interrupt 12
{	
	xPDM |= 0x02;		// Prevent Continuos Interrupt
	RTCIF = 0;
	INT_RTC = 1;
}

	UINT8	INT_SPI = 0;
	UINT8	SPI_RD_DATA;
	UINT8     SPI_RD_NUM=0;
	UINT8 	SPI_RD_BUFF[4];
/***********************************************************************************
 *
 *	NAME		: SPI_ISR()
 *
 *	DESCRIPTION	: Interrupt Service Routine(ISR) of SPI
 *
 * 	PARAMETER	: None
 *
 * 	RETURN		: None
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
void SPI_ISR(void) interrupt 13
{	
	INT_SPI = 1;
	while (1)
	{
		SPI_RD_DATA = xSPDR;
		if(SPI_RD_NUM < 4)	SPI_RD_BUFF[SPI_RD_NUM] = SPI_RD_DATA;
		SPI_RD_NUM++;
		if(xSPSR & 0x01)	break;	// Read FIFO Empty
	}

	if(SPI_RD_NUM > 4) 	SPI_RD_NUM = 0;	// Error case
	xSPSR = 0x00;
}


/***********************************************************************************
 *
 *	NAME		: VOICE_ISR()
 *
 *	DESCRIPTION	: Interrupt Service Routine(ISR) of Voice DMA
 *
 * 	PARAMETER	: None
 *
 * 	RETURN		: None
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
void VOICE_ISR(void) interrupt 14 
{
}


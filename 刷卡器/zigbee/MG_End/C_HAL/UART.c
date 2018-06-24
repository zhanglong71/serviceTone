/*******************************************************************************
	- Chip		: MG2450/2455
	- Vendor		: RadioPulse Inc, 2007.
	- Date		: 2009-07-15
	- Version		: VER 1.72

	[2009-07-15] VER 1.72
	- xUn_FCR register is write-only register. So, OR-ing can cause a bug.
		xUn_FCR	= 0x01;
		xUn_FCR	|= 0x80;
		==>
		xUn_FCR = 0x81;

	[2009-03-09] VER 1.71
	- Add new baud rate(ZHAL_UART0_SET, ZHAL_UART1_SET)
		+ 576000 and 1152000 @16MHz
		+ 288000 and 576000 @8MHz

	[2009-02-23] VER 1.70
	- <#if ENABLE_UART0/1 ~ #endif> is added
	- The scheme to save and restore ES0/1 is changed
	
	[2007-12-11] VER 1.60
	- BaudRate Division switch(BaudRate) {} is changed  in ZHAL_UART0_SET()
		(Because, Core Clock is changed from 16MHz to 8MHz)
	- BaudRate Division switch(BaudRate) {} is changed  in ZHAL_UART1_SET()
		(Because, Core Clock is changed from 16MHz to 8MHz)
		
	[2007-11-07] VER 1.50
	- Initial Version
*******************************************************************************/

#include "INCLUDE_TOP.h"

#if (ENABLE_UART0)
void ZHAL_UART0_SET(UINT8 IntEna, UINT8 Priority, UINT32 BaudRate, UINT8 Mode)	
{
	UINT8	_EA;
	UINT8	Div_H;
	UINT8	Div_L;

	Mode = Mode;	

	_EA = EA;
	EA = 0;
	
	PS0 = Priority;

	#if (OPERATE_AT_16MHz)
		switch(BaudRate) 
		{
			case 300		:	Div_H = 0x1D;	Div_L = 0xC3;		break;
			case 600		:	Div_H = 0x0E;	Div_L = 0xE1;		break;
			case 1200	:	Div_H = 0x07;	Div_L = 0x71;		break;
			case 2400	:	Div_H = 0x03;	Div_L = 0xB8;		break;
			case 4800	:	Div_H = 0x01;	Div_L = 0xDC;		break;
			case 9600	:	Div_H = 0x00;	Div_L = 0xEE;		break;
			case 14400	:	Div_H = 0x00;	Div_L = 0x9F;		break;
			case 19200	:	Div_H = 0x00;	Div_L = 0x77;		break;
			case 38400	:	Div_H = 0x00;	Div_L = 0x3C;		break;
			case 56000	:	Div_H = 0x00;	Div_L = 0x29;		break;
			case 57600	:	Div_H = 0x00;	Div_L = 0x28;		break;
			case 76800	:	Div_H = 0x00;	Div_L = 0x1E;		break;
			case 115200	:	Div_H = 0x00;	Div_L = 0x14;		break;
			case 230400	:	Div_H = 0x00;	Div_L = 0x0A;		break;
			case 460800	:	Div_H = 0x00;	Div_L = 0x05;		break;
			case 576000	:	Div_H = 0x00;	Div_L = 0x04;		break;
			case 1152000	:	Div_H = 0x00;	Div_L = 0x02;		break;
			default		:	Div_H = 0x00;	Div_L = 0xEE;		break;
		}
	#else
		switch(BaudRate) 
		{
			case 300		:	Div_H = 0x0E;	Div_L = 0xE1;		break;
			case 600		:	Div_H = 0x07;	Div_L = 0x71;		break;
			case 1200	:	Div_H = 0x03;	Div_L = 0xB8;		break;
			case 2400	:	Div_H = 0x01;	Div_L = 0xDC;		break;
			case 4800	:	Div_H = 0x00;	Div_L = 0xEE;		break;
			case 9600	:	Div_H = 0x00;	Div_L = 0x77;		break;
			case 14400	:	Div_H = 0x00;	Div_L = 0x4F;		break;
			case 19200	:	Div_H = 0x00;	Div_L = 0x3C;		break;
			case 38400	:	Div_H = 0x00;	Div_L = 0x1E;		break;
			case 56000	:	Div_H = 0x00;	Div_L = 0x14;		break;
			case 57600	:	Div_H = 0x00;	Div_L = 0x14;		break;
			case 76800	:	Div_H = 0x00;	Div_L = 0x0F;		break;
			case 115200	:	Div_H = 0x00;	Div_L = 0x0A;		break;
			case 230400	:	Div_H = 0x00;	Div_L = 0x05;		break;
			case 288000	:	Div_H = 0x00;	Div_L = 0x04;		break;
			case 576000	:	Div_H = 0x00;	Div_L = 0x02;		break;
			default		:	Div_H = 0x00;	Div_L = 0x77;		break;
		}
	#endif

	if(IntEna == 0)
	{
		Div_H = 0;
		Div_L = 0;
	}

	xU0_LCR |= 0x80;
	xU0_DLL	= Div_L;
	xU0_DLM = Div_H;
	xU0_LCR &= 0x7F;	
	
	if(IntEna)  
	{
	//	xU0_FCR	= 0x01;		// UART v1.71
	//	xU0_FCR	|= 0x80;		// UART v1.71
		xU0_FCR = 0x81;		// UART v1.72		
		xU0_LCR = 0x03;		// 0000_0011	
		xU0_MCR = 0x00;		// 0000_0000	
		xU0_IER = 0x01;
		xU0_IER |= 0x02;
		ES0 = 1;
	}
	else
	{
		xU0_IER = 0x00;		
		ES0 = 0;
	}

	EA = _EA;	
}

	




	UINT8 	t0buf[UART0_TXBUF_SIZE];
	UINT8 	r0buf[UART0_RXBUF_SIZE];

	UINT8	t0_fifo_empty = 1;
	UINT8 	t0_in = 0;
	UINT8 	t0_out = 0;
	UINT8 	r0_in = 0;
	UINT8 	r0_out = 0;

	
UINT8 ZSYS_UART0_RXLEN()
{
	UINT8	_ES0;
	UINT8	_RFIE;
	UINT8 	ret;

	_RFIE = RFIE;
	RFIE = 0;
	_ES0 = ES0;
	ES0 = 0;
	
	ret = (r0_in - r0_out) & (UART0_RXBUF_SIZE-1);	

	ES0 = _ES0;
	RFIE = _RFIE;
	
	return 	(ret);
}


UINT8 ZSYS_UART0_TXLEN()
{
	UINT8	_ES0;
	UINT8	_RFIE;
	UINT8 	ret;	

	_RFIE = RFIE;
	RFIE = 0;
	_ES0 = ES0;
	ES0 = 0;	

	ret = (t0_in - t0_out) & (UART0_TXBUF_SIZE-1);

	ES0 = _ES0;
	RFIE = _RFIE;

	return 	(ret);
}


void ZSYS_UART0_PUT(UINT8 c)
{
	UINT8	_ES0;
	UINT8	_RFIE;
	UINT8	Len;
	
	while(1)
	{
		Len = (UART0_TXBUF_SIZE-1);
		Len = Len - ZSYS_UART0_TXLEN();
		if(Len > 3)	break;	
	}

	_RFIE = RFIE;
	RFIE = 0;
	_ES0 = ES0;
	ES0 = 0;

	if(t0_fifo_empty)
	{
		t0_fifo_empty = 0;
		xU0_THR = c;
	}
	else
	{
		t0buf[t0_in] = c;
		t0_in++; 
		t0_in = t0_in & (UART0_TXBUF_SIZE-1);
	}

	ES0 = _ES0;
	RFIE = _RFIE;	
	
}


UINT8 ZSYS_UART0_GET(UINT8 *pc)
{
	UINT8	_ES0;
	UINT8	_RFIE;

	if(ZSYS_UART0_RXLEN() == 0)	return 0;

	_RFIE = RFIE;
	RFIE = 0;
	_ES0 = ES0;
	ES0 = 0;
	
	*pc = r0buf[r0_out++];	
	r0_out = r0_out & (UART0_RXBUF_SIZE-1);

	ES0 = _ES0;
	RFIE = _RFIE;
	
	return 1;
}


//-- RETURN
//	1	: UART0-TX is working
//	0	: UART0-TX is idle
/*UINT8 ZSYS_UART0_TX_WORKING()
{
	UINT8	Working;

	Working = 0;
	if(t0_fifo_empty==0)		Working = 1;
	if( (xU0_LSR & 0x20) == 0)	Working = 1;

	return	Working;
}  */
#endif


#if (ENABLE_UART1)
// BaudRate = SystemClock(8MHz) / (7 * Divisor)
void ZHAL_UART1_SET(UINT8 IntEna, UINT8 Priority, UINT32 BaudRate, UINT8 Mode) 
{
	UINT8	_EA;
	UINT8	Div_H;
	UINT8	Div_L;

	Mode = Mode;
	
	_EA = EA;
	EA = 0;
	
	PS1 = Priority;

	#if (OPERATE_AT_16MHz)
		switch(BaudRate) 
		{
			case 300		:	Div_H = 0x1D;	Div_L = 0xC3;		break;
			case 600		:	Div_H = 0x0E;	Div_L = 0xE1;		break;
			case 1200	:	Div_H = 0x07;	Div_L = 0x71;		break;
			case 2400	:	Div_H = 0x03;	Div_L = 0xB8;		break;
			case 4800	:	Div_H = 0x01;	Div_L = 0xDC;		break;
			case 9600	:	Div_H = 0x00;	Div_L = 0xEE;		break;
			case 14400	:	Div_H = 0x00;	Div_L = 0x9F;		break;
			case 19200	:	Div_H = 0x00;	Div_L = 0x77;		break;
			case 38400	:	Div_H = 0x00;	Div_L = 0x3C;		break;
			case 56000	:	Div_H = 0x00;	Div_L = 0x29;		break;
			case 57600	:	Div_H = 0x00;	Div_L = 0x28;		break;
			case 76800	:	Div_H = 0x00;	Div_L = 0x1E;		break;
			case 115200	:	Div_H = 0x00;	Div_L = 0x14;		break;
			case 230400	:	Div_H = 0x00;	Div_L = 0x0A;		break;
			case 460800	:	Div_H = 0x00;	Div_L = 0x05;		break;
			case 576000	:	Div_H = 0x00;	Div_L = 0x04;		break;
			case 1152000	:	Div_H = 0x00;	Div_L = 0x02;		break;
			default		:	Div_H = 0x00;	Div_L = 0xEE;		break;
		}
	#else
		switch(BaudRate) 
		{
			case 300		:	Div_H = 0x0E;	Div_L = 0xE1;		break;
			case 600		:	Div_H = 0x07;	Div_L = 0x71;		break;
			case 1200	:	Div_H = 0x03;	Div_L = 0xB8;		break;
			case 2400	:	Div_H = 0x01;	Div_L = 0xDC;		break;
			case 4800	:	Div_H = 0x00;	Div_L = 0xEE;		break;
			case 9600	:	Div_H = 0x00;	Div_L = 0x77;		break;
			case 14400	:	Div_H = 0x00;	Div_L = 0x4F;		break;
			case 19200	:	Div_H = 0x00;	Div_L = 0x3C;		break;
			case 38400	:	Div_H = 0x00;	Div_L = 0x1E;		break;
			case 56000	:	Div_H = 0x00;	Div_L = 0x14;		break;
			case 57600	:	Div_H = 0x00;	Div_L = 0x14;		break;
			case 76800	:	Div_H = 0x00;	Div_L = 0x0F;		break;
			case 115200	:	Div_H = 0x00;	Div_L = 0x0A;		break;
			case 230400	:	Div_H = 0x00;	Div_L = 0x05;		break;
			case 288000	:	Div_H = 0x00;	Div_L = 0x04;		break;
			case 576000	:	Div_H = 0x00;	Div_L = 0x02;		break;
			default		:	Div_H = 0x00;	Div_L = 0x77;		break;
		}
	#endif
	
	if(IntEna == 0)
	{
		Div_H = 0;
		Div_L = 0;
	}

	xU1_LCR |= 0x80;
	xU1_DLL	= Div_L;
	xU1_DLM = Div_H;
	xU1_LCR &= 0x7F;	
	
	if(IntEna)  
	{		
	//	xU1_FCR	= 0x01;		// UART v1.71
	//	xU1_FCR	|= 0x80;		// UART v1.71
		xU1_FCR = 0x81;		// UART v1.72
		xU1_LCR = 0x03;		// 0000_0011	
		xU1_MCR = 0x00;		// 0000_0000	
		xU1_IER = 0x01;		
		xU1_IER |= 0x02;		
		ES1 = 1;
	}
	else
	{
		xU1_IER = 0x00;		
		ES1 = 0;
	}

	EA = _EA;	
}


	UINT8 	t1buf[UART1_TXBUF_SIZE];
	UINT8 	r1buf[UART1_RXBUF_SIZE];

	UINT8	t1_fifo_empty = 1;
	UINT8 	t1_in = 0;
	UINT8 	t1_out = 0;
	UINT8 	r1_in = 0;
	UINT8 	r1_out = 0;


UINT8 ZSYS_UART1_RXLEN()
{
	UINT8	_ES1;
	UINT8	_RFIE;
	UINT8 	ret;

	_RFIE = RFIE;
	RFIE = 0;
	_ES1 = ES1;
	ES1 = 0;

	ret = (r1_in - r1_out) & (UART1_RXBUF_SIZE-1);

	ES1 = _ES1;
	RFIE = _RFIE;

	return 	(ret);
}


UINT8 ZSYS_UART1_TXLEN()
{
	UINT8	_ES1;
	UINT8	_RFIE;
	UINT8 	ret;

	_RFIE = RFIE;
	RFIE = 0;
	_ES1 = ES1;
	ES1 = 0;

	ret = (t1_in - t1_out) & (UART1_TXBUF_SIZE-1);

	ES1 = _ES1;
	RFIE = _RFIE;

	return 	(ret);
}

void ZSYS_UART1_PUT(UINT8 c)
{
	UINT8	_ES1;
	UINT8	_RFIE;
	UINT8	Len;

	while(1)
	{
		Len = (UART1_TXBUF_SIZE-1);
		Len = Len - ZSYS_UART1_TXLEN();
		if(Len > 3)	break;
	}

	_RFIE = RFIE;
	RFIE = 0;
	_ES1 = ES1;
	ES1 = 0;

	if(t1_fifo_empty)
	{
		t1_fifo_empty = 0;
		xU1_THR = c;
	}
	else
	{
		t1buf[t1_in] = c;
		t1_in++; 
		t1_in = t1_in & (UART1_TXBUF_SIZE-1) ;
	}

	ES1 = _ES1;
	RFIE = _RFIE;
}

UINT8 ZSYS_UART1_GET(UINT8 *pc)
{
	UINT8	_ES1;
	UINT8	_RFIE;
	
	if(ZSYS_UART1_RXLEN() == 0)	return 0;

	_RFIE = RFIE;
	RFIE = 0;
	_ES1 = ES1;
	ES1 = 0;

	*pc = r1buf[r1_out++]; 
	r1_out = r1_out & (UART1_RXBUF_SIZE-1);

	ES1 = _ES1;
	RFIE = _RFIE;

	return 1;
}

//-- RETURN
//	1	: UART1-TX is working
//	0	: UART1-TX is idle
/*UINT8 ZSYS_UART1_TX_WORKING()
{
	UINT8	Working;

	Working = 0;
	if(t1_fifo_empty==0)		Working = 1;
	if( (xU1_LSR & 0x20) == 0)	Working = 1;

	return	Working;
} */

#endif

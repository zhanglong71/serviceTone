/*******************************************************************************
	- Chip		: MG2450/2455
	- Vendor		: RadioPulse Inc, 2007.
	- Date		: 2007-12-11
	- Version		: VER 1.60
*******************************************************************************/

	extern	UINT8 	t0buf[];
	extern	UINT8 	r0buf[];
	extern	UINT8	t0_fifo_empty;
	extern	UINT8 	t0_in;
	extern	UINT8 	t0_out;
	extern	UINT8 	r0_in;
	extern	UINT8 	r0_out;

	extern	UINT8 	t1buf[];
	extern	UINT8 	r1buf[];
	extern	UINT8	t1_fifo_empty;
	extern	UINT8 	t1_in;
	extern	UINT8 	t1_out;
	extern	UINT8 	r1_in;
	extern	UINT8 	r1_out;


	extern UINT8 	ZSYS_UART0_RXLEN();
	extern UINT8 	ZSYS_UART0_TXLEN();
	extern void 	ZSYS_UART0_PUT(UINT8 c);
	extern UINT8 	ZSYS_UART0_GET(UINT8 *pc);
	extern UINT8 ZSYS_UART0_TX_WORKING();
	extern UINT8 	ZSYS_UART1_RXLEN();
	extern UINT8 	ZSYS_UART1_TXLEN();
	extern UINT8 	ZSYS_UART1_PUT(UINT8 c);
	extern UINT8 	ZSYS_UART1_GET(UINT8 *pc);	
	extern UINT8 ZSYS_UART1_TX_WORKING();


	extern void ZHAL_UART0_SET(UINT8 IntEna, UINT8 Priority, UINT32 BaudRate,UINT8 Mode);
	extern void ZHAL_UART1_SET(UINT8 IntEna, UINT8 Priority, UINT32 BaudRate, UINT8 Mode);



/*******************************************************************************
	- Chip		: MG24500/55
	- Author		: RadioPulse Inc, 2007.
	- Date		: 2009-03-02
	- Version		: VER 1.0	
*******************************************************************************/

	//---------------------------------------------------------
	// GPIO Define
	//---------------------------------------------------------		
	#define		GP0A		P0
	#define		GP00		P0_0
	#define		GP01		P0_1
	#define		GP02		P0_2
	#define		GP03		P0_3
	#define		GP04		P0_4
	#define		GP05		P0_5
	#define		GP06		P0_6
	#define		GP07		P0_7

	#define		GP1A		P1
	#define		GP10		P1_0
	#define		GP11		P1_1
	#define		GP12		P1_2
	#define		GP13		P1_3
	#define		GP14		P1_4
	#define		GP15		P1_5
	#define		GP16		P1_6
	#define		GP17		P1_7

	#define		GP3A		P3
	#define		GP30		P3_0
	#define		GP31		P3_1
	#define		GP32		P3_2
	#define		GP33		P3_3
	#define		GP34		P3_4
	#define		GP35		P3_5
	#define		GP36		P3_6
	#define		GP37		P3_7

	//---------------------------------------------------------
	// Option for User - Read/Write
	// : The change of these values affects to 
	//		- the size of Code memory
	//		- the size of Data memory
	//		- the operation
	//---------------------------------------------------------
	// UART.C (UART0)
	#define	ENABLE_UART0			1
	#define	UART0_TXBUF_SIZE		0x80	// 128 Byte
	#define	UART0_RXBUF_SIZE		0x80	// 128 Byte

	// UART.C (UART1)
	#define	ENABLE_UART1			1	
	#define	UART1_TXBUF_SIZE		0x80	// 128 Byte
	#define	UART1_RXBUF_SIZE		0x80	// 128 Byte

	// LCD.C
	#define	ENABLE_LCD_UNCALLED	0

	// PHY.C
	#define	ENABLE_PHY_UNCALLED	0

	// TIMER.C
	#define	ENABLE_COUNTER_UNCALLED	0

	// SECURITY.C
	#define	ENABLE_SECURITY_UNCALLED	0

	// Crystal Option
	#define	OPERATE_AT_16MHz			0

	//----------------------------------------------
	//----------------------------------------------
	



/*******************************************************************************
	- Chip		: MG24500/55
	- Author		: RadioPulse Inc, 2007.
	- Date		: 2007-07-02
	- Version		: VER 1.0	
*******************************************************************************/

//--------------------------
//	8051 base registers
//--------------------------
sfr 	P0    	= 0x80;
sfr 	SP    	= 0x81;
sfr 	DPL   	= 0x82;
sfr 	DPH   	= 0x83;
sfr	P0MSK	= 0x84;
sfr	P0SEL	= 0x85;
sfr 	PCON  	= 0x87;
sfr 	TCON  	= 0x88;
sfr 	TMOD  	= 0x89;
sfr 	TL0   	= 0x8A;
sfr 	TL1   	= 0x8B;
sfr 	TH0   	= 0x8C;
sfr 	TH1   	= 0x8D;
sfr	P1    	= 0x90;
sfr	EXIF	= 0x91;
sfr 	FBANK	= 0xA1;
sfr	AUXR1	= 0xA2;
sfr 	IE    	= 0xA8;
sfr	T23CON	= 0xA9;
sfr	TH2		= 0xAA;
sfr	TH3		= 0xAB;
sfr	TL2		= 0xAC;
sfr	TL3		= 0xAD;
sfr 	P3    	= 0xB0;
sfr	P0OEN	= 0xB1;
sfr	P1OEN	= 0xB2;
sfr	P3OEN	= 0xB4;
sfr 	IP    	= 0xB8;
sfr	P0REN	= 0xB9;
sfr	P1REN	= 0xBA;
sfr	P3REN	= 0xBC;
sfr	T4CON	= 0xBD;
sfr	TL4		= 0xBE;
sfr	TH4		= 0xBF;
sfr	WCON	= 0xC0;
sfr 	PSW   	= 0xD0;
sfr	WDT	= 0xD2;
sfr	EICON	= 0xD8;
sfr	ACC		= 0xE0;
sfr	EIE		= 0xE8;
sfr 	B    		= 0xF0;
sfr	EIP		= 0xF8;

//--------------------------
// bit accessable sfr
//--------------------------

// EIP - Extended Interrupt Priority
//			= EIP^7;
sbit	VCEIP	= EIP^6;
sbit	SPIIP	= EIP^5;
sbit	RTCIP	= EIP^4;			// realtime clock
sbit	T3IP	= EIP^3;			// timer3
sbit	AESIP	= EIP^2;			// ADC/DES interrupt priority
sbit	T2IP	= EIP^1;
sbit	RFIP	= EIP^0;

// B

// EIE - Extended Interrupt Enable
//			= EIE^7;
sbit	VCEIE	= EIE^6;
//			= EIE^5;
sbit	SPIIE	= EIE^5;
sbit	RTCIE	= EIE^4;			// realtime clock interrupt
sbit	T3IE	= EIE^3;			// timer3 interrupt
sbit	AESIE	= EIE^2;			// AES interrupt
sbit	T2IE	= EIE^1;			// timer2 interrupt
sbit	RFIE	= EIE^0;			// RF transmit/receive interrupt

// ACC

// EICON
//			= EICON^7;
//			= EICON^6;
//			= EICON^5;
//			= EICON^4;
sbit	RTCIF	= EICON^3;
//			= EICON^2;
//			= EICON^1;
//			= EICON^0;

// PSW
sbit 	CY    	= PSW^7;
sbit 	AC    	= PSW^6;
sbit 	F0    	= PSW^5;
sbit 	RS1   	= PSW^4;
sbit 	RS0   	= PSW^3;
sbit	OV    	= PSW^2;
sbit	F1    	= PSW^1;
sbit	PF 	 	= PSW^0;		// Parity Flag

// WCON
//			= WCON^7;
//			= WCON^6;
//			= WCON^5;
//			= WCON^4;
//			= WCON^3;
sbit	ISPMODE= WCON^2;
sbit	ENROM	= WCON^1;
sbit	CW		= WCON^0;

// IP
//			= IP^7;
sbit	PS1		= IP^6;
//			= IP^5;
sbit 	PS0    	= IP^4;
sbit 	PT1   	= IP^3;
sbit 	PX1   	= IP^2;
sbit 	PT0   	= IP^1;
sbit 	PX0   	= IP^0;

// P3
sbit	P3_7	= P3^7;		// SPICSN
sbit	P3_6	= P3^6;		// SPICLK
sbit	P3_5	= P3^5;		// SPIDO
sbit	P3_4	= P3^4;		// SPIDI
sbit	P3_3	= P3^3;		// INT1
sbit	P3_2	= P3^2;		// INT0
sbit	P3_1	= P3^1;		// TXD0
sbit	P3_0	= P3^0;		// RXD0

// IE
sbit 	EA    	= IE^7;		// interrupt enable
sbit	ES1		= IE^6;		// serial1 interrupt
//			= IE^5;
sbit 	ES0    	= IE^4;		// serial0 interrupt
sbit 	ET1   	= IE^3;		// timer1 interrupt
sbit 	EX1   	= IE^2;		// ext1 interrupt
sbit 	ET0   	= IE^1;		// timer0 interrupt
sbit 	EX0   	= IE^0;		// ext0 interrupt

// P1
sbit	P1_7	= P1^7;
sbit	P1_6	= P1^6;
sbit	P1_5	= P1^5;
sbit	P1_4	= P1^4;
sbit	P1_3	= P1^3;
sbit	P1_2	= P1^2;
sbit	P1_1	= P1^1;		// TXD1
sbit	P1_0	= P1^0;		// RXD1

// TCON
sbit 	TF1   	= TCON^7;
sbit 	TR1   	= TCON^6;
sbit 	TF0   	= TCON^5;
sbit	TR0   	= TCON^4;
sbit 	IE1   	= TCON^3;
sbit	IT1   	= TCON^2;
sbit 	IE0   	= TCON^1;
sbit 	IT0   	= TCON^0;

// P0
sbit	P0_7	= P0^7;
sbit	P0_6	= P0^6;
sbit	P0_5	= P0^5;
sbit	P0_4	= P0^4;
sbit	P0_3	= P0^3;
sbit	P0_2	= P0^2;
sbit	P0_1	= P0^1;
sbit	P0_0	= P0^0;

//----------------------------------------------------
// MAC TX FIFO Control
//----------------------------------------------------
#define	xMTFCPUSH	XBYTE[0x2000]
#define	xMTFCWP	XBYTE[0x2001]
#define	xMTFCRP	XBYTE[0x2002]
#define	xMTFCCTL	XBYTE[0x2003]
#define	xMTFCSTA	XBYTE[0x2004]
#define	xMTFCSIZE	XBYTE[0x2005]
#define	xMTFCROOM	XBYTE[0x2006]
#define	xMTFCSBASE	XBYTE[0x2007]
#define	xMTFCSLEN	XBYTE[0x2008]
#define	xAACKFC0	XBYTE[0x2009]
#define	xAACKFC1	XBYTE[0x200A]
#define	xAACKDSN	XBYTE[0x200B]
#define	xAACKSTA	XBYTE[0x200C]
//----------------------------------------------------
// MAC RX FIFO Control
//----------------------------------------------------
#define	xMRFCPOP	XBYTE[0x2080]
#define	xMRFCWP	XBYTE[0x2081]
#define	xMRFCRP	XBYTE[0x2082]
#define	xMRFCCTL	XBYTE[0x2083]
#define	xMRFCSTA	XBYTE[0x2084]
#define	xMRFCSIZE	XBYTE[0x2085]
#define	xMRFCROOM	XBYTE[0x2086]
#define	xMRFCSBASE	XBYTE[0x2087]
#define	xMRFCSLEN	XBYTE[0x2088]
#define	xFDSCPOPH	XBYTE[0x2089]
#define	xFDSCPOPL	XBYTE[0x208A]
#define	xFDSCSTA	XBYTE[0x208B]
//----------------------------------------------------
// FFLT Control (1)
//----------------------------------------------------
#define	xFFLT0SEQ	XBYTE[0x20C0]
#define	xFFLT1SEQ	XBYTE[0x20C1]
#define	xFFLT2SEQ	XBYTE[0x20C2]
#define	xFFLT3SEQ	XBYTE[0x20C3]
#define	xFFLT4SEQ	XBYTE[0x20C4]
#define	xFFLT5SEQ	XBYTE[0x20C5]
#define	xFFLT6SEQ	XBYTE[0x20C6]
#define	xFFLT7SEQ	XBYTE[0x20C7]
#define	xFFLT0AND	XBYTE[0x20C8]
#define	xFFLT1AND	XBYTE[0x20C9]
#define	xFFLT2AND	XBYTE[0x20CA]
#define	xFFLT3AND	XBYTE[0x20CB]
#define	xFFLT4AND	XBYTE[0x20CC]
#define	xFFLT5AND	XBYTE[0x20CD]
#define	xFFLT6AND	XBYTE[0x20CE]
#define	xFFLT7AND	XBYTE[0x20CF]
//----------------------------------------------------
// FFLT Control (2)
//----------------------------------------------------
#define	xFFLT0OR	XBYTE[0x20D0]
#define	xFFLT1OR	XBYTE[0x20D1]
#define	xFFLT2OR	XBYTE[0x20D2]
#define	xFFLT3OR	XBYTE[0x20D3]
#define	xFFLT4OR	XBYTE[0x20D4]
#define	xFFLT5OR	XBYTE[0x20D5]
#define	xFFLT6OR	XBYTE[0x20D6]
#define	xFFLT7OR	XBYTE[0x20D7]
#define	xFFLT0VAL	XBYTE[0x20D8]
#define	xFFLT1VAL	XBYTE[0x20D9]
#define	xFFLT2VAL	XBYTE[0x20DA]
#define	xFFLT3VAL	XBYTE[0x20DB]
#define	xFFLT4VAL	XBYTE[0x20DC]
#define	xFFLT5VAL	XBYTE[0x20DD]
#define	xFFLT6VAL	XBYTE[0x20DE]
#define	xFFLT7VAL	XBYTE[0x20DF]
//----------------------------------------------------
// FFLT Control (3)
//----------------------------------------------------
#define	xFFLTRES	XBYTE[0x20E0]
#define	xFFLTENA	XBYTE[0x20E1]
#define	xFFLTFLD	XBYTE[0x20E2]
//----------------------------------------------------
// MAC Control : 0x2100 ~ 0x2195
//----------------------------------------------------
#define 	xKEY0(n)			XBYTE[0x2100+n]	// MSB = 0x190F
#define 	xRXNONCE(n)		XBYTE[0x2110+n]	// MSB = 0x191C
#define 	xSAESBUF(n)		XBYTE[0x2120+n]	// ?????
#define 	xKEY1(n)			XBYTE[0x2130+n]	// MSB = 0x193F
#define 	xTXNONCE(n)		XBYTE[0x2140+n]	// MSB = 0x194C
#define 	xEXTADDR(n)		XBYTE[0x2150+n]	// MSB = 0x1957
#define 	xPANID(n)			XBYTE[0x2158+n]	// MSB = 0x1959
#define 	xSHORTADDR(n)		XBYTE[0x215A+n]	// MSB = 0x195B

//-----------------------------
// MAC STATUS
//-----------------------------
// bit[7] : enc/dec			:: default= 0
// bit[6] : tx_busy			:: default= 0
// bit[5] : rx_busy			:: default= 0
// bit[4] : rsv				:: default= 0
// bit[3] : decode_ok		:: default= 1
// bit[2] ; enc_done		:: default= 0
// bit[1] : dec_done		:: default= 0
// bit[0] : crc_ok			:: default= 1
#define 	xMACSTS		XBYTE[0x2180]		// xMAC_STA
//-----------------------------

//-----------------------------
// MAC SAES
// bit[7:1] 	: reserved
// bit[0]		: SAES
//-----------------------------
#define 	xMACSAES		XBYTE[0x218E]		// xMAC_SAES

//-----------------------------
// MAC MAIN
//-----------------------------
// bit[7] : rst_fifo
// bit[6] : rst_tsm
// bit[5] : rst_rsm
// bit[4] : rst_aes
// bit[3:0] : rsv
#define 	xMACRST		XBYTE[0x2190]		// xMAC_MAIN
//-----------------------------

//-----------------------------
// MAC MODEM
//-----------------------------
// bit[7:5] : rsv				:: default=0
// bit[4] : prevent_ack_packet	:: default=0
// bit[3] : pan_coordinator		:: default=0
// bit[2] ; addr_decode			:: default=1
// bit[1] : auto_crc				:: default=1
// bit[0] : auto_ack			:: default=0
#define 	xMACCTRL		XBYTE[0x2191]		// xMACCTRL
//-----------------------------

#define 	xMACDSN		XBYTE[0x2192]		// xMAC_DSN

//-----------------------------
// MAC SEC
//-----------------------------
// bit[7] : sa_keysel
// bit[6] : tx_keysel
// bit[5] : rx_keysel
// bit[4:2] : sec_m[2:0]
// bit[1:0] : sec_mode[1:0].		0=None, 1=CBC-MAC, 2=CTR, 3=CCM
#define 	xMACSEC		XBYTE[0x2193]		// xMAC_SEC
//-----------------------------
#define 	xTXAL			XBYTE[0x2194]		// bit[7] : rsv		// xMAC_TXL
#define 	xRXAL			XBYTE[0x2195]		// bit[7] : rsv		// xMAC_RXL
//----------------------------------------------------
// MAC TX FIFO : 0x2300 ~ 0x23FF
//----------------------------------------------------
#define	xMTxFIFO(n)			XBYTE[0x2300+n]
//----------------------------------------------------
// MAC RX FIFO : 0x2400 ~ 0x24FF
//----------------------------------------------------
#define	xMRxFIFO(n)			XBYTE[0x2400+n]
//----------------------------------------------------
// UART0
//----------------------------------------------------
#define	xU0_RBR	XBYTE[0x2500]
#define	xU0_THR	XBYTE[0x2500]
#define	xU0_DLL	XBYTE[0x2500]
#define	xU0_IER		XBYTE[0x2501]
#define	xU0_DLM	XBYTE[0x2501]
#define	xU0_IIR		XBYTE[0x2502]	// Read Only
#define	xU0_FCR	XBYTE[0x2502]	// Write Only
#define	xU0_LCR	XBYTE[0x2503]
#define	xU0_MCR	XBYTE[0x2504]
#define	xU0_LSR	XBYTE[0x2505]
#define	xU0_MSR	XBYTE[0x2506]
#define	xU0_PENA	XBYTE[0x2507]
//----------------------------------------------------
// UART1
//----------------------------------------------------
#define	xU1_RBR	XBYTE[0x2510]
#define	xU1_THR	XBYTE[0x2510]
#define	xU1_DLL	XBYTE[0x2510]
#define	xU1_IER		XBYTE[0x2511]
#define	xU1_DLM	XBYTE[0x2511]
#define	xU1_IIR		XBYTE[0x2512]	// Read Only
#define	xU1_FCR	XBYTE[0x2512]	// Write Only
#define	xU1_LCR	XBYTE[0x2513]
#define	xU1_MCR	XBYTE[0x2514]
#define	xU1_LSR	XBYTE[0x2515]
#define	xU1_MSR	XBYTE[0x2516]
#define	xU1_PENA	XBYTE[0x2517]
//----------------------------------------------------
// I2STX
//----------------------------------------------------
#define	xSTXDATL3	XBYTE[0x2520]
#define	xSTXDATL2	XBYTE[0x2521]
#define	xSTXDATL1	XBYTE[0x2522]
#define	xSTXDATL0	XBYTE[0x2523]
#define	xSTXDATR3	XBYTE[0x2524]
#define	xSTXDATR2	XBYTE[0x2525]
#define	xSTXDATR1	XBYTE[0x2526]
#define	xSTXDATR0	XBYTE[0x2527]
#define	xSTXAIC		XBYTE[0x2528]
//------------------------
// STXAIC
//------------------------
// [7] 	: 1=Master, 0=Slave
// [6:5]	: 0=I2S, 1=LFT, 2=RHT, 3=DSP
// [4:3] 	: Word Length. 0=16Byte, 1=20Byte, 2=24Byte, 3=32Byte
// [2]	: LRSWAP. 0=Left, 1=Right
// [1] 	: FRAMEP. 0=Sync, 1=Delay
// [0]	: BCP. 0=Normal, 1=Invert
//------------------------
#define	xSTXSTS	XBYTE[0x2529]
#define	xSTXSDIV	XBYTE[0x252A]
#define	xSTXMDIV	XBYTE[0x252B]
#define	xSTXBDIV	XBYTE[0x252C]
#define	xSTXMODE	XBYTE[0x252D]
//------------------------
// STXMODE
//------------------------
// [7] 	: CSHR
// [6]	: MPOL
// [5]	: BPOL
// [4] 	: B16
// [3]	: POP
// [2:1] 	: 0=BLK, 1=MRT, 2=MLT, 3=STR
// [0]	: CLKENA
//------------------------
#define	xSTXINTENA	XBYTE[0x252E]
#define	x252F		XBYTE[0x252F]
//----------------------------------------------------
// I2SRX
//----------------------------------------------------
#define	xSRXDATL3	XBYTE[0x2530]
#define	xSRXDATL2	XBYTE[0x2531]
#define	xSRXDATL1	XBYTE[0x2532]
#define	xSRXDATL0	XBYTE[0x2533]
#define	xSRXDATR3	XBYTE[0x2534]
#define	xSRXDATR2	XBYTE[0x2535]
#define	xSRXDATR1	XBYTE[0x2536]
#define	xSRXDATR0	XBYTE[0x2537]
#define	xSRXAIC		XBYTE[0x2538]
//------------------------
// SRXAIC
//------------------------
// [7] 	: 1=Master, 0=Slave
// [6:5]	: 0=I2S, 1=LFT, 2=RHT, 3=DSP
// [4:3] 	: Word Length. 0=16Byte, 1=20Byte, 2=24Byte, 3=32Byte
// [2]	: LRSWAP. 0=Left, 1=Right
// [1] 	: FRAMEP. 0=Sync, 1=Delay
// [0]	: BCP. 0=Normal, 1=Invert
//------------------------
#define	xSRXSTS	XBYTE[0x2539]
#define	xSRXSDIV	XBYTE[0x253A]
#define	xSRXMDIV	XBYTE[0x253B]
#define	xSRXBDIV	XBYTE[0x253C]
#define	xSRXMODE	XBYTE[0x253D]
//------------------------
// SRXMODE
//------------------------
// [7] 	: CSHR
// [6]	: MPOL
// [5]	: BPOL
// [4] 	: B16
// [3]	: POP
// [2:1] 	: 0=BLK, 1=MRT, 2=MLT, 3=STR
// [0]	: CLKENA
//------------------------
#define	xSRXINTENA	XBYTE[0x253E]
#define	xSRXLPB		XBYTE[0x253F]
//----------------------------------------------------
// SPI
//----------------------------------------------------
#define	xSPCR		XBYTE[0x2540]
#define	xSPSR		XBYTE[0x2541]
#define	xSPDR		XBYTE[0x2542]
#define	xSPER		XBYTE[0x2543]
//----------------------------------------------------
// RNG
//----------------------------------------------------
#define	xRNGD3		XBYTE[0x2550]
#define	xRNGD2		XBYTE[0x2551]
#define	xRNGD1		XBYTE[0x2552]
#define	xRNGD0		XBYTE[0x2553]
#define	xSEED3		XBYTE[0x2554]
#define	xSEED2		XBYTE[0x2555]
#define	xSEED1		XBYTE[0x2556]
#define	xSEED0		XBYTE[0x2557]
#define	xRNGC		XBYTE[0x2558]
//----------------------------------------------------
// QUAD
//----------------------------------------------------
#define	xUDX		XBYTE[0x2560]
#define	xCNTX		XBYTE[0x2561]
#define	xUDY		XBYTE[0x2562]
#define	xCNTY		XBYTE[0x2563]
#define	xUDZ		XBYTE[0x2564]
#define	xCNTZ		XBYTE[0x2565]
#define	xQCTL		XBYTE[0x2566]
//----------------------------------------------------
// VFIFO
//----------------------------------------------------
#define 	xVTxFIFO(n)		XBYTE[0x2600+n]		// 0x2600 ~ 0x267F
#define 	xVRxFIFO(n)		XBYTE[0x2680+n]		// 0x2680 ~ 0x26FF
//----------------------------------------------------
// FCNTL Control
//----------------------------------------------------
#define	xFCN_CMD		XBYTE[0x2700]
#define	xFCN_CSUM		XBYTE[0x2701]
#define	xFCN_ADR1		XBYTE[0x2702]
#define	xFCN_ADR0		XBYTE[0x2703]
#define	xFCN_DAT3		XBYTE[0x2704]
#define	xFCN_DAT2		XBYTE[0x2705]
#define	xFCN_DAT1		XBYTE[0x2706]
#define	xFCN_DAT0		XBYTE[0x2707]
#define	xFCN_LEN1		XBYTE[0x2708]
#define	xFCN_LEN0		XBYTE[0x2709]
#define	xFCN_CPT1		XBYTE[0x270A]
#define	xFCN_CPT0		XBYTE[0x270B]
#define	xFCN_STS1		XBYTE[0x270C]
#define	xFCN_STS0		XBYTE[0x270D]
#define	xFCN_NOWAIT	XBYTE[0x270E]
//----------------------------------------------------
// FCNTL TEST Patterns
//----------------------------------------------------
#define	xFCN_T0DAT3	XBYTE[0x2710]
#define	xFCN_T0DAT2	XBYTE[0x2711]
#define	xFCN_T0DAT1	XBYTE[0x2712]
#define	xFCN_T0DAT0	XBYTE[0x2713]
#define	xFCN_T1DAT3	XBYTE[0x2714]
#define	xFCN_T1DAT2	XBYTE[0x2715]
#define	xFCN_T1DAT1	XBYTE[0x2716]
#define	xFCN_T1DAT0	XBYTE[0x2717]
//----------------------------------------------------
// FCNTL Timing Parameters
//----------------------------------------------------
#define	xFCN_TERS1		XBYTE[0x2720]
#define	xFCN_TERS0		XBYTE[0x2721]
#define	xFCN_TME1		XBYTE[0x2722]
#define	xFCN_TME0		XBYTE[0x2723]
#define	xFCN_TPRG		XBYTE[0x2724]
#define	xFCN_TRCV		XBYTE[0x2725]
#define	xFCN_THV1		XBYTE[0x2726]
#define	xFCN_THV0		XBYTE[0x2727]
#define	xFCN_TNVS		XBYTE[0x2728]  
#define	xFCN_TNVH		XBYTE[0x2729]
#define	xFCN_TPGS		XBYTE[0x272A]
#define	xFCN_TNVH1		XBYTE[0x272B]
//----------------------------------------------------
// CODEC Control
//----------------------------------------------------
#define	xENCMUT1		XBYTE[0x2740]
#define	xENCMUT0		XBYTE[0x2741]
#define	xENCPCM1		XBYTE[0x2742]
#define	xENCPCM0		XBYTE[0x2743]
#define	xENCCDC		XBYTE[0x2744]
#define	xENCCTL		XBYTE[0x2745]
//------------------------
// ENCCTL
//------------------------
// [7:6]	: -
// [5]	: B16
// [4] 	: MUT
// [3:2]	: 0=PCM, 1=ULAW, 2=ALAW, 3=ADPCM
// [1] 	: Initialize
// [0]	: Enable
//------------------------
#define	x2746			XBYTE[0x2746]
#define	x2747			XBYTE[0x2747]
#define	xDECMUT1		XBYTE[0x2748]
#define	xDECMUT0		XBYTE[0x2749]
#define	xDECPCM1		XBYTE[0x274A]
#define	xDECPCM0		XBYTE[0x274B]
#define	xDECCDC		XBYTE[0x274C]
#define	xDECCTL		XBYTE[0x274D]
//------------------------
// DECCTL
//------------------------
// [7]	: LBP(Codec Loopback)
// [6]	: -
// [5]	: B16
// [4] 	: MUT
// [3:2]	: 0=PCM, 1=ULAW, 2=ALAW, 3=ADPCM
// [1] 	: Initialize
// [0]	: Enable
//------------------------
//----------------------------------------------------
// VOICE TX FIFO Control (From I2S to MAC TX FIFO)
//----------------------------------------------------
#define 	xVTF_DAT		XBYTE[0x2750]
#define 	xVTF_MUT		XBYTE[0x2751]
#define 	xVTF_CTL		XBYTE[0x2752]
//------------------------
// VTFCTL
//------------------------
// [7:4]	: -
// [3]	: DMA Enable(VTxFIFO --> MTxFIFO)
// [2]	: MUTE
// [1] 	: Clear Write/Read Pointer
// [0]	: Initialize VTXFIFO[] to Mute Data
//------------------------
#define 	xVTF_RP		XBYTE[0x2753]
#define 	xVTF_WP		XBYTE[0x2754]
#define 	xVTF_SIZE		XBYTE[0x2755]
#define 	xVTF_HTHR		XBYTE[0x2756]
#define 	xVTF_LTHR		XBYTE[0x2757]
#define 	xVTF_ROOM		XBYTE[0x2758]
#define 	xVTF_2759		XBYTE[0x2759]
#define 	xVTF_STS		XBYTE[0x275A]
#define 	xVTD_SIZE		XBYTE[0x275B]
#define 	xNEVOS			XBYTE[0x275C]
#define 	xNEVNGT1		XBYTE[0x275D]
#define 	xNEVNGT0		XBYTE[0x275E]
#define 	xNEVVOL		XBYTE[0x275F]
//----------------------------------------------------
// VOICE RX FIFO Control (From I2S to MAC TX FIFO)
//----------------------------------------------------
#define 	xVRF_DAT		XBYTE[0x2760]
#define 	xVRF_MUT		XBYTE[0x2761]
#define 	xVRF_CTL		XBYTE[0x2762]
//------------------------
// VRFCTL
//------------------------
// [7:4]	: -
// [3]	: DMA Enable(MRxFIFO --> VRxFIFO)
// [2]	: MUTE
// [1] 	: Clear Write/Read Pointer
// [0]	: Initialize VRXFIFO[] to Mute Data
//------------------------
#define 	xVRF_RP		XBYTE[0x2763]
#define 	xVRF_WP		XBYTE[0x2764]
#define 	xVRF_SIZE		XBYTE[0x2765]
#define 	xVRF_HTHR		XBYTE[0x2766]
#define 	xVRF_LTHR		XBYTE[0x2767]
#define 	xVRF_ROOM		XBYTE[0x2768]
#define 	xVRF_2769		XBYTE[0x2769]
#define 	xVRF_STS		XBYTE[0x276A]
#define 	xVRD_SIZE		XBYTE[0x276B]
#define	xFEVOS			XBYTE[0x276C]
#define	xFEVNGT1		XBYTE[0x276D]
#define	xFEVNGT0		XBYTE[0x276E]
#define	xFEVVOL		XBYTE[0x276F]
//----------------------------------------------------
// VOICE IF
//----------------------------------------------------
#define	xVTFINTENA		XBYTE[0x2770]
//-----------------
// VTFINTENA
// [7]	: EMPTY
// [6]	: FULL
// [5]	: EMPTY_N
// [4]	: FULL_N
// [3]	: OVF
// [2]	: UDF
// [1]	: CDCPOP
// [0]	: PCMPOP
//-----------------
#define	xVRFINTENA		XBYTE[0x2771]
//-----------------
// VRFINTENA
// [7]	: EMPTY
// [6]	: FULL
// [5]	: EMPTY_N
// [4]	: FULL_N
// [3]	: OVF
// [2]	: UDF
// [1]	: CDCPOP
// [0]	: PCMPOP
//-----------------
#define	xVDMINTENA	XBYTE[0x2772]
//-----------------
// VDMINTENA
// [7]	: VTF Abnormal
// [6]	: VTF High Threshold
// [5]	: VTF Low Threshold
// [4]	: VTF DMA Done(I2SRX --> VTF --> MTF)
// [3]	: VRF Abnormal
// [2]	: VRF High Threshold
// [1]	: VRF Low Threshold
// [0]	: VRF DMA Done(I2STX <-- VRF <-- MRF)
//-----------------
#define	xVTFINTSRC		XBYTE[0x2773]
//-----------------
// VTFINTSRC
// [7]	: Empty
// [6]	: Full
// [5]	: Empty Neg
// [4]	: Full Neg
// [3]	: Overflow
// [2]	: Underflow
// [1]	: Codec Pop
// [0]	: PCM Pop
//-----------------
#define	xVRFINTSRC		XBYTE[0x2774]
//-----------------
// VRFINTSRC
// [7]	: Empty
// [6]	: Full
// [5]	: Empty Neg
// [4]	: Full Neg
// [3]	: Overflow
// [2]	: Underflow
// [1]	: Codec Push
// [0]	: PCM Push
//-----------------
#define	xVDMINTSRC		XBYTE[0x2775]
//-----------------
// VDMINTSRC
// [7]	: VTF Abnormal
// [6]	: VTF High Threshold
// [5]	: VTF Low Threshold
// [4]	: VTF DMA Done(I2SRX --> VTF --> MTF)
// [3]	: VRF Abnormal
// [2]	: VRF High Threshold
// [1]	: VRF Low Threshold
// [0]	: VRF DMA Done(I2STX <-- VRF <-- MRF)
//-----------------
#define	xVTFINTVAL		XBYTE[0x2776]
#define	xVRFINTVAL		XBYTE[0x2777]
//-----------------
// VRFINTVAL
// [7]	: Empty
// [6]	: Full
// [5]	: Empty Neg
// [4]	: Full Neg
// [3]	: Overflow
// [2]	: Underflow
// [1]	: Codec Push
// [0]	: PCM Push
//-----------------
#define	xVDMINTVAL		XBYTE[0x2778]
#define	xVCECFG		XBYTE[0x2779]

#define	xSRCCTL			XBYTE[0x277A]
#define	x277B			XBYTE[0x277B]
#define	xVSPMUT1		XBYTE[0x277C]
#define	xVSPMUT0		XBYTE[0x277D]
#define	xVSPCTL			XBYTE[0x277E]
//------------------------
// VSPCTL
//------------------------
// [7]	: -
// [6]	: DECMUT
// [5]	: DECINI
// [4] 	: DECB16
// [3]	: -
// [2]	: ENCMUT
// [1] 	: ENCINI
// [0]	: ENCB16
//------------------------
#define	x277F			XBYTE[0x277F]
//----------------------------------------------------
// Acoustic Echo Canceler (1)
//----------------------------------------------------
#define	xCONVTHR1		XBYTE[0x2780]
#define	xCONVTHR0		XBYTE[0x2781]
#define	xDIVRGTHR1		XBYTE[0x2782]
#define	xDIVRGTHR0		XBYTE[0x2783]
#define	xDTTHR1		XBYTE[0x2784]
#define	xDTTHR0		XBYTE[0x2785]
#define	xNLPSPD1		XBYTE[0x2786]
#define	xNLPSPD0		XBYTE[0x2787]
#define	x2788			XBYTE[0x2788]
#define	x2789			XBYTE[0x2789]
#define	xHDPLXTHR1		XBYTE[0x278A]
#define	xHDPLXTHR0		XBYTE[0x278B]
#define	xHDPLXSTCNT1	XBYTE[0x278C]
#define	xHDPLXSTCNT0	XBYTE[0x278D]
#define	xTDETTHR1		XBYTE[0x278E]
#define	xTDETTHR0		XBYTE[0x278F]
//----------------------------------------------------
// Acoustic Echo Canceler (2)
//----------------------------------------------------
#define	xTDETONCNT1	XBYTE[0x2790]
#define	xTDETONCNT0	XBYTE[0x2791]
#define	xTDETOFFCNT1	XBYTE[0x2792]
#define	xTDETOFFCNT0	XBYTE[0x2793]
#define	xTDETMU1		XBYTE[0x2794]
#define	xTDETMU0		XBYTE[0x2795]
#define	xINITOUTLEV1	XBYTE[0x2796]
#define	xINITOUTLEV0	XBYTE[0x2797]
#define	xINITCURGAIN3	XBYTE[0x2798]
#define	xINITCURGAIN2	XBYTE[0x2799]
#define	xINITCURGAIN1	XBYTE[0x279A]
#define	xINITCURGAIN0	XBYTE[0x279B]
#define	xGAINUPSPD1	XBYTE[0x279C]
#define	xGAINUPSPD0	XBYTE[0x279D]
#define	xGAINDNSPD1	XBYTE[0x279E]
#define	xGAINDNSPD0	XBYTE[0x279F]
//----------------------------------------------------
// Acoustic Echo Canceler (3)
//----------------------------------------------------
#define	xMINGAIN1		XBYTE[0x27A0]
#define	xMINGAIN0		XBYTE[0x27A1]
#define	xMAXGAIN1		XBYTE[0x27A2]
#define	xMAXGAIN0		XBYTE[0x27A3]
#define	xNTAPS1		XBYTE[0x27A4]
#define	xNTAPS0		XBYTE[0x27A5]
#define	xLMSMU1		XBYTE[0x27A6]
#define	xLMSMU0		XBYTE[0x27A7]
#define	xAEC_CTL		XBYTE[0x27A8]
#define	xAEC_STA		XBYTE[0x27A9]
#define	x27AA			XBYTE[0x27AA]
#define	x27AB			XBYTE[0x27AB]
#define	x27AC			XBYTE[0x27AC]
#define	x27AD			XBYTE[0x27AD]
#define	x27AE			XBYTE[0x27AE]
#define	x27AF			XBYTE[0x27AF]


//----------------------------------------------------
//	RF Register Set
//----------------------------------------------------
#define	PHY_BASE	0x2200
//----------------------------------------------------
// MODEM Command
//----------------------------------------------------
	#define	xPCMD0			XBYTE[PHY_BASE + 0x00]
	#define	xPCMD1			XBYTE[PHY_BASE + 0x01]
	#define	xPLLPD			XBYTE[PHY_BASE + 0x02]
	#define	xPLLPU			XBYTE[PHY_BASE + 0x03]
	#define	xRXRFPD		XBYTE[PHY_BASE + 0x04]
	#define	xRXRFPU		XBYTE[PHY_BASE + 0x05]
	#define	xTXRFPD		XBYTE[PHY_BASE + 0x06]
	#define	xTXRFPU		XBYTE[PHY_BASE + 0x07]
	#define	xRSVD08		XBYTE[PHY_BASE + 0x08]
	#define	xBIASPD		XBYTE[PHY_BASE + 0x09]
	#define	xBIASPU		XBYTE[PHY_BASE + 0x0A]
	#define	xCHNLSEL		XBYTE[PHY_BASE + 0x0B]
	#define	xRFCONCNF		XBYTE[PHY_BASE + 0x0C]
	#define	xGPCNF4		XBYTE[PHY_BASE + 0x0D]
	#define	xGPCNF5		XBYTE[PHY_BASE + 0x0E]

//----------------------------------------------------
// MODEM Config
//----------------------------------------------------
	#define	xMDMCNF		XBYTE[PHY_BASE + 0x10]
	#define	xRXFRM1		XBYTE[PHY_BASE + 0x11]
	#define	xTDCNF3		XBYTE[PHY_BASE + 0x13]	
	#define	xTDCNF0		XBYTE[PHY_BASE + 0x17]
	#define	xGPCNF0		XBYTE[PHY_BASE + 0x19]
	#define	xGPCNF1		XBYTE[PHY_BASE + 0x1A]
	#define	xGPCNF2		XBYTE[PHY_BASE + 0x1B]
	#define	xGPCNF3		XBYTE[PHY_BASE + 0x1C]
	#define	xTDCNF2		XBYTE[PHY_BASE + 0x1D]	

//----------------------------------------------------
// AGC
//----------------------------------------------------

	#define	xAGCCNF0		XBYTE[PHY_BASE + 0x20]
	#define	xAGCCNF1		XBYTE[PHY_BASE + 0x21]
	#define	xAGCCNF3		XBYTE[PHY_BASE + 0x23]
	#define	xAGCCNF5		XBYTE[PHY_BASE + 0x25]
	#define	xAGCCNF6		XBYTE[PHY_BASE + 0x26]	
	#define	xAGCCNF7		XBYTE[PHY_BASE + 0x27]
	#define	xAGCCNF8		XBYTE[PHY_BASE + 0x28]
	#define	xAGCCNF9		XBYTE[PHY_BASE + 0x29]
	#define	xAGCCNF10		XBYTE[PHY_BASE + 0x2A]
	#define	xAGCCNF11		XBYTE[PHY_BASE + 0x2B]
	#define	xAGCCNF12		XBYTE[PHY_BASE + 0x2C]
	#define	xAGCCNF15		XBYTE[PHY_BASE + 0x2F]	
	#define	xAGCCNF16		XBYTE[PHY_BASE + 0x30]	
	#define	xAGCCNF17		XBYTE[PHY_BASE + 0x31]
	#define	xAGCCNF18		XBYTE[PHY_BASE + 0x32]
	#define	xAGCCNF19		XBYTE[PHY_BASE + 0x33]
	#define	xAGCCNF22		XBYTE[PHY_BASE + 0x3A]
	#define	xAGCCNF23		XBYTE[PHY_BASE + 0x3B]
	#define	xAGCCNF24		XBYTE[PHY_BASE + 0x3C]
	#define	xAGCCNF26		XBYTE[PHY_BASE + 0x3E]

//----------------------------------------------------
// COR/TME/CCA Config
//----------------------------------------------------
	#define	xCORCNF0		XBYTE[PHY_BASE + 0x40]
	#define	xCORCNF1		XBYTE[PHY_BASE + 0x41]
	#define	xCORCNF2		XBYTE[PHY_BASE + 0x42]
	#define	xCCA0			XBYTE[PHY_BASE + 0x48]
	#define	xCCA1			XBYTE[PHY_BASE + 0x49]
	
//----------------------------------------------------
// Reserved Region
//----------------------------------------------------

//----------------------------------------------------
// Test Operation Config and Monitor
//----------------------------------------------------
	#define	xTST0			XBYTE[PHY_BASE + 0x60]
	#define	xTST1			XBYTE[PHY_BASE + 0x61]
	#define	xTST2			XBYTE[PHY_BASE + 0x62]
	#define	xTST13			XBYTE[PHY_BASE + 0x6D]
	#define	xTST14			XBYTE[PHY_BASE + 0x6E]
	#define	xTST15			XBYTE[PHY_BASE + 0x6F]

//----------------------------------------------------
// Status Monitor and Control
//----------------------------------------------------
	#define	xPHYSTS0		XBYTE[PHY_BASE + 0x70]
	#define	xPHYSTS1		XBYTE[PHY_BASE + 0x71]
	#define	xAGCSTS1		XBYTE[PHY_BASE + 0x73]
	#define	xAGCSTS2		XBYTE[PHY_BASE + 0x74]
	#define	xAGCSTS3		XBYTE[PHY_BASE + 0x75]
	#define	xINTCON		XBYTE[PHY_BASE + 0x77]	// bit[3]=RxEnd, bit[2]=RxStart, bit[1]=TxEnd, bit[0]=ModemOn
	#define	xINTIDX			XBYTE[PHY_BASE + 0x78]
	#define	xMONCON0		XBYTE[PHY_BASE + 0x79]
	#define	xINTSTS			XBYTE[PHY_BASE + 0x7E]	// bit[3:0] : 0=Modem-On, 1=Tx-End, 2=Rx-Start, 3=Rx-End

//----------------------------------------------------
// PLL Config
//----------------------------------------------------
	#define	xPLLADIV		XBYTE[PHY_BASE + 0x80]
	#define	xPLLRDIV		XBYTE[PHY_BASE + 0x81]	
	#define	xPLLBDIV		XBYTE[PHY_BASE + 0x82]
	#define	xPLLFRAC3		XBYTE[PHY_BASE + 0x86]
	#define	xPLLFRAC4		XBYTE[PHY_BASE + 0x87]
	#define	xPLLADFC		XBYTE[PHY_BASE + 0x89]
	#define	xPLLLD			XBYTE[PHY_BASE + 0x8A]
	#define	xPLLCP			XBYTE[PHY_BASE + 0x8B]
	#define	xPLLVCO			XBYTE[PHY_BASE + 0x8C]
	#define	xPLLVC			XBYTE[PHY_BASE + 0x8D]
	#define	xPLLDIV			XBYTE[PHY_BASE + 0x8E]
	#define	xPLLBUF			XBYTE[PHY_BASE + 0x8F]

//----------------------------------------------------
// RF RX Path Config
//----------------------------------------------------
	#define	xRXLNA			XBYTE[PHY_BASE + 0x90]
	#define	xRXMIX			XBYTE[PHY_BASE + 0x91]
	#define	xRXIP2I			XBYTE[PHY_BASE + 0x92]
	#define	xRXIP2Q			XBYTE[PHY_BASE + 0x93]
	#define	xRXBBAMP		XBYTE[PHY_BASE + 0x94]
	#define	xRXRSSI			XBYTE[PHY_BASE + 0x95]
	#define	xRXVGA			XBYTE[PHY_BASE + 0x97]
	#define	xCTLVGA		XBYTE[PHY_BASE + 0x98]
	#define	xCTLVGA2		XBYTE[PHY_BASE + 0x99]
	#define	xPLLLF1			XBYTE[PHY_BASE + 0x9B]
	#define	xPLLLF2			XBYTE[PHY_BASE + 0x9C]
	#define	xPLLLF3			XBYTE[PHY_BASE + 0x9D]
	#define	xVRXLPF			XBYTE[PHY_BASE + 0x9E]
	#define	xCHIPID			XBYTE[PHY_BASE + 0x9F]

//----------------------------------------------------
// RF TX Path Config
//----------------------------------------------------
	#define	xTXPA			XBYTE[PHY_BASE + 0xA0]
	#define	xTXDA			XBYTE[PHY_BASE + 0xA1]
	#define	xTXMIX			XBYTE[PHY_BASE + 0xA2]
	#define	xTXLPF			XBYTE[PHY_BASE + 0xA4]
	#define	xMEAS2			XBYTE[PHY_BASE + 0xA6]
	#define	xBIAS			XBYTE[PHY_BASE + 0xA7]
	#define	xSADCCON		XBYTE[PHY_BASE + 0xAB]
	#define	xSADCVALH		XBYTE[PHY_BASE + 0xAC]
	#define	xSADCVALL		XBYTE[PHY_BASE + 0xAD]
	#define	xSADCBIASH		XBYTE[PHY_BASE + 0xAE]
	#define	xSADCBIASL		XBYTE[PHY_BASE + 0xAF]

	#define	xDCCCON		XBYTE[PHY_BASE + 0xB0]
	#define	xDCCCNF		XBYTE[PHY_BASE + 0xB5]

//----------------------------------------------------
// Clock/Reset/Power-Down Control
//----------------------------------------------------

	#define	xCLKON0		XBYTE[PHY_BASE + 0xC0]
	#define	xCLKDIV0		XBYTE[PHY_BASE + 0xC3]
	#define	xCLKON1		XBYTE[PHY_BASE + 0xC4]
	#define	xCLKON2		XBYTE[PHY_BASE + 0xC8]
	#define	xSWRST			XBYTE[PHY_BASE + 0xCC]

//----------------------------------------------------
// 3V Logic
//----------------------------------------------------

	#define	xPDM			XBYTE[PHY_BASE + 0xF0]
	#define	xPDCON			XBYTE[PHY_BASE + 0xF1]
	#define	xRCOSC1		XBYTE[PHY_BASE + 0xF2]
	#define	xRCOSC2		XBYTE[PHY_BASE + 0xF3]
	#define	xRTDLY			XBYTE[PHY_BASE + 0xF4]
	#define	xRTCON			XBYTE[PHY_BASE + 0xF5]
	#define	xRTINT1			XBYTE[PHY_BASE + 0xF6]
	#define	xRTINT0			XBYTE[PHY_BASE + 0xF7]
	#define	xCKPLL			XBYTE[PHY_BASE + 0xF8]

	
//----------------------------------------------------
// End of Register Set
//----------------------------------------------------



//----------------------------------------------------
// Register Control Define
//----------------------------------------------------

	#define	RxON			(xPCMD1 &= ~0x01)	// bit[1]=0
	#define	RxOFF			(xPCMD1 |= 0x01)		// bit[1]=1
	#define	EncStartON		(xPCMD1 |= 0x10)		// bit[4]=1
	#define	EncStartOFF		(xPCMD1 &= ~0x10)	// bit[4]=0	
	#define	DecStartON		(xPCMD1 |= 0x20)		// bit[5]=1
	#define	DecStartOFF		(xPCMD1 &= ~0x20)	// bit[5]=0
	
	#define	RstAesON		(xMACRST |= 0x10)	// bit[4]=1
	#define	RstAesOFF		(xMACRST &= ~0x10)	// bit[4]=0
	#define	RstRsmON		(xMACRST |= 0x20)	// bit[5]=1
	#define	RstRsmOFF		(xMACRST &= ~0x20)	// bit[5]=0
	#define	RstTsmON		(xMACRST |= 0x40)	// bit[6]=1
	#define	RstTsmOFF		(xMACRST &= ~0x40)	// bit[6]=0
	#define	RstAesRsmON	(xMACRST |= 0x30)
	#define	RstAesRsmOFF	(xMACRST &= ~0x30)
	#define	RstAesTsmON	(xMACRST |= 0x50)
	#define	RstAesTsmOFF	(xMACRST &= ~0x50)

	#define	ClearEncDone	(xMACSTS &= ~0x04)	// bit[2]=0
	#define	ClearDecDone	(xMACSTS &= ~0x02)	// bit[1]=0	

	#define	ClearRFON		(xINTIDX &= ~0x08)
	#define	ClearRFOFF		(xINTIDX |= 0x08)
	
	#define	ClearRFIntFlag	(EXIF &= 0xEF)
	#define	ClearAesIntFlag	(EXIF &= 0xBF)


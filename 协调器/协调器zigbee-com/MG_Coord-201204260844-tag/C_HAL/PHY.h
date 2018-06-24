/*******************************************************************************
	- Chip		: MG2450/2455
	- Vendor		: RadioPulse Inc, 2007.
	- Date		: 2008-03-06
	- Version		: VER 1.63
*******************************************************************************/

	extern	void ZHAL_REGULATOR_SET(UINT8 ChangeAnalog, UINT8 ChangeDigital);
	extern	void ZHAL_CLOCK_BLOCK_SET(UINT8 EnaCodec, UINT8 EnaAES);
	extern 	void ZHAL_CLOCK_PLL_SET(UINT8 EnaCLKPLL);
	extern	void ZHAL_CLOCK_16MHz_SET(UINT8 Ena16MHz);

	extern	void ZHAL_HW_INIT(UINT8 Ena19MHz, UINT8 VoltChange);
	extern	void ZHAL_3V_LOGIC_INIT();
	extern	UINT8 ZHAL_MODEM_INIT(UINT16 WaitLoopCounter);
	extern	void ZHAL_MAC_INIT();

	extern	void ZHAL_CHANNEL_SET(UINT8 Chan);
	extern	UINT8 ZHAL_ED_LEVEL_GET();
	extern	INT8 ZHAL_ED_dBm_GET();
	extern	void ZHAL_DATARATE_SET(UINT8 Rate);
	extern	 void ZHAL_DATARATE_SET_ISR(UINT8 Rate);
	extern	void ZHAL_TRSW_SET(UINT8 Ena);
	extern	void ZHAL_TEST_TXOUT(UINT8 Ena, UINT8 ModulationEn);
	
	extern	void ZHAL_MAC_CTRL_SET(UINT8 MacCtrl);
	extern	void ZHAL_AUTO_CRC_SET(UINT8 Ena);
	extern	void ZHAL_ADDR_DECODE_SET(UINT8 Ena);	
	extern	void ZHAL_COORDINATOR_SET(UINT8 Ena);
	extern	void ZHAL_PAN_ID_SET(UINT16 ID);
	extern	void ZHAL_SHORT_ADDR_SET(UINT16 Addr);
	extern	void ZHAL_IEEE_ADDR_SET(UINT8 *pAddr);
	extern	void ZHAL_TXPOWER_SET(UINT8 PowerLevel);

	extern	UINT8 ZHAL_SEND_TXFIFO();	
	extern	void ZHAL_SEND_ACK(UINT8 FP, UINT8 DSN);

	extern	void ZHAL_RF_INT_SET(UINT8 IntEna, UINT8 Priority);
	extern	void ZHAL_EXT0_INT_SET(UINT8 IntEna, UINT8 Priority, UINT8 TypeEdge) ;
	extern	void ZHAL_EXT1_INT_SET(UINT8 IntEna, UINT8 Priority, UINT8 TypeEdge) ;

	extern	void ZHAL_SYSTEM_INTERRUPT(UINT8 IntEna);
	
	extern	void ZHAL_PORT0_INOUT_SET(UINT8 Port, UINT8 OutEnable);
	extern	void ZHAL_PORT1_INOUT_SET(UINT8 Port, UINT8 OutEnable);
	extern	void ZHAL_PORT3_INOUT_SET(UINT8 Port, UINT8 OutEnable);

	extern	void ZHAL_VFIFO_MUX_SET(UINT8 Src);


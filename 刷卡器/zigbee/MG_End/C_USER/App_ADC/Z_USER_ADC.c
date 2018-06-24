
/*******************************************************************************
	- Chip		: MG2450/2455
	- Vendor		: RadioPulse Inc, 2007.
	- Date		: 2011-07-18
	- Version		: VER 1.0
*******************************************************************************/

#include "INCLUDE_TOP.h"
#include "INCLUDE_STACK.h"

#include "C_HAL/UART.h"
#include "C_HAL/PHY.h"
#include "C_HAL/FLASH.h"
#include "C_HAL/TIMER.h"
//#include "C_HAL/LCD.h"
//#include "C_HAL/ADC.h"
#include "C_HAL/WDT.h"	// added for OTA

#include "C_UTIL/UTIL_SYS.h"
#include "C_UTIL/UTIL_APP.h"

//-------------------------------------------------
// User Files
//-------------------------------------------------
#include "Z_USER_CONFIG_ADC.h"
#include "C_USER/Z_USER_APP_TYPE.h"

	//----------------------------------------------------------------
	// Global Variables For Application Layer
	//----------------------------------------------------------------	
	// ZDO Configuration
	ZS_SimpDesc		AppSimpDesc;		// Simple Descriptor
	ZS_NodeDesc		AppNodeDesc;		// Node Descriptor
	#if (UserConfig_Enable_ZDP_UserDescriptor)
		UINT8	AppUserDescriptor[16];	// User Descriptor
	#endif
	
	// Variables
	UINT8	APP_DSN;
	S_ASDU	APP_ASDU;
	UINT32	UserChanList;

	// Flash Area(HIB)
	HW_INFORMATION code 	HIB	_at_ HIB_ADDRESS;

	// Buffer For Flash Writing
	FLASH_AREA		DATA_COPY;
	//----------------------------------------------------------------	
	//----------------------------------------------------------------

#include "C_USER/Z_USER_DATA.c"		// Include Source File
#include "C_USER/Z_USER_ZDP.c"		// Include Source File
//-------------------------------------------------

/***********************************************************************************
 *
 *	NAME		: Initialize_ZDO_Descriptor()
 *
 *	DESCRIPTION	: Initalize ZDO(Zigbee Device Object) Descriptors.
 *		This function should be called in ZUSER_INIT().
 ***********************************************************************************/
void Initialize_ZDO_Descriptor()
{
	// Simple Descriptor
	AppSimpDesc.EnaSD = 1;
	AppSimpDesc.AppDevID = ProfileHA_DevSensorADC_ID;
	AppSimpDesc.AppProID = ProfileHA;
	AppSimpDesc.DevVersion = 0;
	AppSimpDesc.EP = ProfileHA_DevSensorADC_EP_14;
	AppSimpDesc.NumInClus = 1;
	AppSimpDesc.InClusList[0] = ProfileHA_DevSensorADC_ClusCtrlADC;
	AppSimpDesc.NumOutClus = 1;
	AppSimpDesc.OutClusList[0] = ProfileHA_DevSensorADC_ClusCurrentADC;

	// Node Descriptor
	AppNodeDesc.Avail_Type = UserConfig_DevType;
#if (UserConfig_Enable_ZDP_UserDescriptor)
	AppNodeDesc.Avail_Type |= 0x10;
#endif
#if (UserConfig_Enable_ZDP_CompDescReq)
	AppNodeDesc.Avail_Type |= 0x08;
#endif
	AppNodeDesc.Freq_ApsFlag = 0x40;
	AppNodeDesc.MacCapa = UserConfig_Capability;
	AppNodeDesc.Manufacture = UserConfig_ManufacturerCode;
	AppNodeDesc.MaxBufSize = apscMaxASDU;
	AppNodeDesc.MaxInTranSize = apscMaxASDU;
	AppNodeDesc.ServerMask = UserConfig_ServerMask;
	AppNodeDesc.MaxOutTransSize = AppNodeDesc.MaxInTranSize;
	AppNodeDesc.DescCapa = 0x00;

	// Default UserDescriptor = "MG245X-ZigBeePRO"
#if (UserConfig_Enable_ZDP_UserDescriptor)
	AppUserDescriptor[0] = 'M';
	AppUserDescriptor[1] = 'G';
	AppUserDescriptor[2] = '2';
	AppUserDescriptor[3] = '4';
	AppUserDescriptor[4] = '5';
	AppUserDescriptor[5] = 'X';
	AppUserDescriptor[6] = '-';
	AppUserDescriptor[7] = 'Z';
	AppUserDescriptor[8] = 'i';
	AppUserDescriptor[9] = 'g';
	AppUserDescriptor[10] = 'B';
	AppUserDescriptor[11] = 'e';
	AppUserDescriptor[12] = 'e';
	AppUserDescriptor[13] = 'P';
	AppUserDescriptor[14] = 'R';
	AppUserDescriptor[15] = 'O';	
#endif	
}

	ATTRIBUTE_U8	ATT_CtrlADC;
	ATTRIBUTE_U16	ATT_CurrADC;
	extern	void ZSYS_SET_UserTimer(UINT8 TimerNum, UINT16 TickCount);
	extern	UINT16 ZSYS_GET_UserTimer(UINT8 TimerNum);

/***********************************************************************************
 *
 *	NAME		: ZUSER_HW_INIT()
 *	DESCRIPTION	: Callback function called by the stack in boot-up sequence.
 *		The main() function of the stack is as follows.
 *			void main()		// main() function of the stack
 *			{
 *				ZUSER_HW_INIT();
 *				// Functions to enable interrupts(TIMER0, TIMER2)
 *				// Functions to initialize the modem of MG245X
 *				ZNWK_RESET_REQ();
 *				ZUSER_INIT();
 *				while(1)
 *				{
 *					...
 *					ZUSER_MAIN();
 *					...
 *				}
 *			}
 *		In this function, the operating frequency of MG245X can be changed to 16MHz.
 * 	PARAMETER	: None
 * 	RETURN		: None
 * 	NOTES		: None
 ***********************************************************************************/
void ZUSER_HW_INIT()
{
	#if (OPERATE_AT_16MHz)
		ZHAL_CLOCK_16MHz_SET(1);
	#endif
}

/***********************************************************************************
 *	NAME		: ZUSER_INIT()
 *	DESCRIPTION	: Callback function called by the stack in boot-up sequence.
 ***********************************************************************************/
UINT8 myNetAddr[2];
void ZUSER_INIT()
{	
	UINT16	RandomSeed;
	UINT8	  ScanIn;
	UINT8   KeyIn,i;
	UINT8  u8UsrBuf[20];
	bit     initChannel=0;

	// Interrupt Enable/Disable
	ZHAL_UART0_SET(1, 1, 57600, 0x80);
	//ZHAL_UART0_SET(1, 1, 115200, 0x80);
	ZHAL_UART1_SET(1, 1, 115200, 0x80);
	ZHAL_TIMER1_SET(1, 0, 1, AppOption_TIMER1_ms);
	ZHAL_EXT0_INT_SET(1, 0, 1);
	ZHAL_EXT1_INT_SET(1, 0, 1);

	// Initialize GPIO to use
	#if (AppOption_BoardVersion == 0x11)
		ZHAL_PORT3_INOUT_SET(4, 1);	GP34 = 0;
		ZHAL_PORT3_INOUT_SET(5, 1);	GP35 = 0;
		ZHAL_PORT3_INOUT_SET(6, 1);	GP36 = 0;
		ZHAL_PORT3_INOUT_SET(7, 1);	GP37 = 0;
	#else
		ZHAL_PORT0_INOUT_SET(0, 1);	GP00 = 0;
		ZHAL_PORT0_INOUT_SET(1, 1);	GP00 = 0;
		ZHAL_PORT0_INOUT_SET(2, 1);	GP00 = 0;
		ZHAL_PORT0_INOUT_SET(3, 1);	GP00 = 0;
	#endif
	//---------------------------------------------------------------

	// Initialize ZDO Descriptor for the application
	Initialize_ZDO_Descriptor();

	// Initialize ZCL Attribute for the application	
	ATT_CtrlADC.ID = ProfileHA_DevSensorADC_ClusCtrlADC_AttOpeMode;
	ATT_CtrlADC.DType = ProfileHA_DevSensorADC_ClusCtrlADC_AttOpeMode_DTYPE;
	ATT_CtrlADC.Value = ATT_CtrlADC_Stop;
	ATT_CurrADC.ID = ProfileHA_DevSensorADC_ClusCurrentADC_AttRead;
	ATT_CurrADC.DType = ProfileHA_DevSensorADC_ClusCurrentADC_AttRead_DTYPE;
	ATT_CurrADC.Value = 0x0000;

	// Get HIB Information to be used
	zPrintf(1, "\n HIB_COPY");
	if(ZSYS_HIB_COPY( (UINT8 *)&DATA_COPY.DataHIB))
	{
		zPrintf(1, " ==> OK");		
		memcpy(MPIB.IEEE_ExtendAddr, DATA_COPY.DataHIB.IEEE_ADDR, 8);
		ZHAL_IEEE_ADDR_SET(MPIB.IEEE_ExtendAddr);
		//ZSYS_SET_CHANNEL(DATA_COPY.DataHIB.Channel);
		//UserChanList = 1;
		//UserChanList <<= DATA_COPY.DataHIB.Channel;
		while(!initChannel)               //设置工作频率与本机地址
		{
			u8UsrBuf[0]=0x08;
		  u8UsrBuf[1]=0x00;
		  u8UsrBuf[2]=0x00;
		  u8UsrBuf[3]=0x00;
		  u8UsrBuf[4]=0x00;
		  u8UsrBuf[5]=0x00;
		  u8UsrBuf[6]=0x02;
		  u8UsrBuf[7]=0x00;
		  u8UsrBuf[8]=0xff;
			for(i=0;i<=u8UsrBuf[0];i++)ZSYS_UART0_PUT(u8UsrBuf[i]);
			ZSYS_SET_UserTimer(0, (1000/ AppOption_TIMER1_ms));  //延迟1000ms
			while(ZSYS_GET_UserTimer(0))
			{
				if(ZSYS_UART0_RXLEN())break;
			}
			if(ZSYS_GET_UserTimer(0))    //1000ms内串口有数据
			{
				ZSYS_SET_UserTimer(0, (50/ AppOption_TIMER1_ms));  //延迟50ms
			  while(ZSYS_GET_UserTimer(0))
				KeyIn=ZSYS_UART0_RXLEN();
				for(i=0;i<KeyIn;i++)ZSYS_UART0_GET(&u8UsrBuf[i]);
				if((u8UsrBuf[0]==KeyIn-1)&&(u8UsrBuf[1]==0x01)&&(u8UsrBuf[6]==0x02)&&(u8UsrBuf[7]>=0x0b)&&(u8UsrBuf[7]<=0x1a)&&(u8UsrBuf[12]==0xff))
				{
					initChannel=1;
					ScanIn=u8UsrBuf[7];
					ZSYS_SET_CHANNEL(ScanIn);
					UserChanList = 1;
					UserChanList <<=ScanIn;
					myNetAddr[0]=u8UsrBuf[10];
					myNetAddr[1]=u8UsrBuf[11];
			  }
			}
		}
	}
	// Recall saved data from FLASH, if needed.
	#if (AppOption_Enable_FlashFunc && AppOption_Enable_FlashRecall)	
		if( (HIB.Space[0x10] == 'C') || (HIB.Space[0x10] == 'R') || (HIB.Space[0x10] == 'E') )
		{
			ZSYS_DATA_RECALL();
		}
	#endif

	// Initialize Random Generator. And, Initialize Each layer's Sequence Number Randomly.
	//RandomSeed = ZSYS_ADC_GET(6);
	ZSYS_RANDOM_GEN_SEED(RandomSeed);
	AIB.DSN = ZSYS_RANDOM_GEN_GET(0x00FF);
	NIB.nwkDSN = ZSYS_RANDOM_GEN_GET(0x00FF);
	MPIB.macDSN = ZSYS_RANDOM_GEN_GET(0x00FF);
	NIB.RREQNum = ZSYS_RANDOM_GEN_GET(0x00FF);

 	// Random Delay
	ZSYS_SET_UserTimer(0, ZSYS_RANDOM_GEN_GET(0x000F));
	ZSYS_SET_UserTimer(2, 300);    //3秒自动Ping协调器一次
	while(ZSYS_GET_UserTimer(0));
		
	zPrintf(1, "\n ADC Application Start ...");
	
}

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

	extern	UINT8	INT_EXT0;
	extern	UINT8	INT_EXT1;

	UINT8	JoinOK = 0;
	UINT8	ReceiveOTAPacket = 0;
 	UINT16	DstDeviceNwkAddr = 0;
 	bit flag_delay50ms=0;
 	bit flag_first_join=0;
 	bit flag_leave_net=0;
 	UINT8 cntPingCoor=0;
 	UINT8 cntRxPing=0;
 	UINT8 cntRxLose=0;
/***********************************************************************************
 *	NAME		: ZUSER_MAIN()
 * 	NOTES		: This function is NOT regularly called by the stack.
 ***********************************************************************************/	
void ZUSER_MAIN()
{	
	//ZCLS_HEAD	ZCL_HEAD;
	UINT8	KeyIn;
	UINT8 i;
	UINT16	PollingPeriod;
	UINT8  u8UsrBuf[128];
	if(NIB.UpdateReport)
	{
		//-- UpdateReport	: If following NIB is changed, corresponding bit is set to 1
		//	bit[15]	: reserved
		//	bit[14]	: A RT Entry is deleted.
		//	bit[13]	: A NT Entry is deleted.
		//	bit[12]	: A NAM Entry is deleted.
		// 	bit[11]	: reserved
		//	bit[10]	: RT(Routing Table) Entry is added or changed.
		//	bit[9]	: NT(Neighbor Table) Entry is added or changed.
		//	bit[8]	: NAM(Network Address Map Table) Entry is added or changed.
		//	bit[7:5]	: reserved
		//	bit[4]	: Link Status Command is transmitted.
		//	bit[3]	: reserved
		//	bit[2]	: PPIB.phyCurrentChannel
		//	bit[1]	: NIB.nwkNetworkAddr;
		//	bit[0] 	: DEVTYPE
		NIB.UpdateReport = 0;
	}
	if(flag_leave_net)    //断开网络
	{
		u8UsrBuf[0]=0x08;
		u8UsrBuf[1]=0x00;
		u8UsrBuf[2]=0x00;
		u8UsrBuf[3]=0x00;
		u8UsrBuf[4]=0x00;
		u8UsrBuf[5]=0x00;
		u8UsrBuf[6]=0x03;
		u8UsrBuf[7]=0x4b;
		u8UsrBuf[8]=0xff;
		for(i=0;i<=u8UsrBuf[0];i++)ZSYS_UART0_PUT(u8UsrBuf[i]);
		ZSYS_SET_UserTimer(0, (1000/ AppOption_TIMER1_ms));  //延迟1000ms
		while(ZSYS_GET_UserTimer(0))
		{
			if(ZSYS_UART0_RXLEN())break;
		}
		if(ZSYS_GET_UserTimer(0))    //1000ms内串口有数据
		{
			ZSYS_SET_UserTimer(0, (50/ AppOption_TIMER1_ms));  //延迟50ms
		  while(ZSYS_GET_UserTimer(0))
			KeyIn=ZSYS_UART0_RXLEN();
			for(i=0;i<KeyIn;i++)ZSYS_UART0_GET(&u8UsrBuf[i]);
			if((u8UsrBuf[0]==KeyIn-1)&&(u8UsrBuf[1]==0x01)&&(u8UsrBuf[6]==0x03)&&(u8UsrBuf[7]==0x4b)&&(u8UsrBuf[8]==0xff))
			{
				ZSYS_WDT_SET(5000);
				while(1);
			}
		}
	}
  KeyIn=ZSYS_UART0_RXLEN();
	if((KeyIn>0)&&(flag_delay50ms!=1))
	{
		ZSYS_SET_UserTimer(0, (50/ AppOption_TIMER1_ms));  //延迟50ms
		flag_delay50ms=1;
	}
	if((flag_delay50ms==1)&&(ZSYS_GET_UserTimer(0) == 0))
	{
		for(i=0;i<KeyIn;i++)
		{
			ZSYS_UART0_GET(&u8UsrBuf[i]);
		}
		if((u8UsrBuf[0]==(KeyIn-1))&&(u8UsrBuf[KeyIn-1]==0xFF)) //检查数据长度是否正确结束标志是否正确
		{
			if((u8UsrBuf[1]==0x02)||(u8UsrBuf[1]==0x03))          //数据帧或者数据确认帧
			{
				if(JoinOK)ZSYS_SEND_OOB_PACKET(1, 0x0000, KeyIn, u8UsrBuf);
			}
			if(u8UsrBuf[1]==0x00)                           //控制帧
			{
				if((u8UsrBuf[6]==0x01)&&(u8UsrBuf[7]==0x45))  //查询End是否加入网络
				{
					if(JoinOK)u8UsrBuf[7]=0x4a;
					else u8UsrBuf[7]=0x55;
					u8UsrBuf[1]=0x01;
					for(i=0;i<KeyIn;i++)ZSYS_UART0_PUT(u8UsrBuf[i]);
				}
			}
		}
		flag_delay50ms=0;
	}
	if(JoinOK)
	{
		if(ZSYS_GET_UserTimer(2)==0)
		{
			ZSYS_SET_UserTimer(2, 1000);    //10秒自动Ping协调器一次
			if(cntPingCoor!=cntRxPing)cntRxLose++;
			else cntRxLose=0;
			if(cntRxLose>=3)                //连续3次没有收到返回,则认为网络已断。准备复位系统
			{
				flag_leave_net=1;
			}
			cntPingCoor++;
			u8UsrBuf[0]=myNetAddr[0];
			u8UsrBuf[1]=myNetAddr[1];
			u8UsrBuf[2]=cntPingCoor;
			ZSYS_SEND_OOB_PACKET(1, 0x0000, 3, &u8UsrBuf);
		}
		if( (UserConfig_Capability & 0x08) == 0)
		{
			if(ZSYS_GET_UserTimer(1) == 0)
			{
				PollingPeriod = UserConfig_TransPersistTime - 500;	// period = (PersistenceTime - 500) ms
				ZSYS_SET_UserTimer(1, PollingPeriod/AppOption_TIMER1_ms);
				zPrintf(1, "\n ZNWK_SYNC_REQ()");
				APS2NWK.SYNC_REQ.Track = 0;
				ZNWK_SYNC_REQ(&APS2NWK.SYNC_REQ, &NWK2APS.SYNC_CON);
				zPrintf(1, " => Sta=%02x",(short)NWK2APS.SYNC_CON.Sta);			
			}
		}
		if(flag_first_join==0)
		{
			u8UsrBuf[0]=0x08;
		  u8UsrBuf[1]=0x00;
		  u8UsrBuf[2]=0x00;
		  u8UsrBuf[3]=0x00;
		  u8UsrBuf[4]=0x00;
		  u8UsrBuf[5]=0x00;
		  u8UsrBuf[6]=0x03;
		  u8UsrBuf[7]=0x4a;
		  u8UsrBuf[8]=0xff;
			for(i=0;i<=u8UsrBuf[0];i++)ZSYS_UART0_PUT(u8UsrBuf[i]);
			ZSYS_SET_UserTimer(0, (1000/ AppOption_TIMER1_ms));  //延迟1000ms
			while(ZSYS_GET_UserTimer(0))
			{
				if(ZSYS_UART0_RXLEN())break;
			}
			if(ZSYS_GET_UserTimer(0))    //1000ms内串口有数据
			{
				ZSYS_SET_UserTimer(0, (50/ AppOption_TIMER1_ms));  //延迟50ms
			  while(ZSYS_GET_UserTimer(0))
				KeyIn=ZSYS_UART0_RXLEN();
				for(i=0;i<KeyIn;i++)ZSYS_UART0_GET(&u8UsrBuf[i]);
				if((u8UsrBuf[0]==KeyIn-1)&&(u8UsrBuf[1]==0x01)&&(u8UsrBuf[6]==0x03)&&(u8UsrBuf[7]==0x4a)&&(u8UsrBuf[8]==0xff))
				{
					flag_first_join=1;
				}
			}
		}
	}
	else
	{
		if(ZSYS_GET_UserTimer(0) == 0)
		{					
			if(ZSYS_JOIN_PROCESS(UserChanList))
			{
				JoinOK = 1;
				ZSYS_SET_UserTimer(0, 1000/AppOption_TIMER1_ms);	// 1000 ms
			}
			else
			{
				ZSYS_SET_UserTimer(0, 10000/AppOption_TIMER1_ms);	// 10000 ms
			}
		}
		else
		{
			if(ReceiveOTAPacket)
			{
				ReceiveOTAPacket = 0;
				ZSYS_SET_UserTimer(0, 1000/AppOption_TIMER1_ms);	// 1000 ms
			}
		}
	}

}


#if (UserConfig_Enable_OTA)
#include "C_USER/Z_USER_MNGT.c"
#endif

/***********************************************************************************
 *
 *	NAME		: ZUSER_OOB_DATA_IND()
 *
 *	DESCRIPTION	: Callback function called by the stack when an OOB(Out-Of-Band) packet is received.
 *		An OOB packet is a MAC data packet. And, out of ZigBee specification.
 *		Routines to process the received OOB packet is implemented(e.g. OTA)
 *
 * 	PARAMETER	: 
 *		pInd	- pointer to MAC OOB packet(MCPS_DATA_IND)
 *		OobIndex - OOB identifier(0x01 ~ 0x0E). 0=No OOB. 0x0F=OTA.
 *
 * 	RETURN		: None
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
void ZUSER_OOB_DATA_IND(MCPS_DATA_IND *pInd, UINT8 OobIndex)
{
	UINT8 i;
	if(OobIndex==0x0f)  // OobIndex = 0x0f for OTA
	{
		#if (UserConfig_Enable_OTA)
		ReceiveOTAPacket = 1;
		ZSYS_WDT_SET(5000);
		ZUSER_OOB_MNGT_IND(pInd, OobIndex);	
		#endif
	}
	else
	{
		zPrintf(1, "\n");
		zPrintf(1, "\n [RX-USER-OOB] OOB=%02x",(short)OobIndex);
		zPrintf(1, "\n LEN = %02x, LQI = %02x",(short)pInd->msduLength,(short)pInd->msduLinkQuality);
		Display_Buffer(pInd->pmsdu, pInd->msduLength, 1);
		if(pInd->msduLength==1)              //协调器Ping返回
		{
			cntRxPing=pInd->pmsdu[0];
		}
		else
		{
			for(i=0;i<pInd->msduLength;i++)ZSYS_UART0_PUT(pInd->pmsdu[i]);
		}
		
	}
}



/***********************************************************************************
 *
 *	NAME		: ZUSER_OOB_RF_ISR()
 *
 *	DESCRIPTION	: Callback function called by the stack when an OOB(Out-Of-Band) packet is received. 
 *		This function is similar to ZUSER_OOB_DATA_IND(). The difference is that the
 *		received packet is processed quickly because this is called directly by RF Interrupt 
 *		Service Routine(ISR). This is needed for high performance data(e.g. voice data)
 *
 * 	PARAMETER	: 
 *		OobCode	- OOB identifier(0x01 ~ 0x0E). 0=No OOB. 0x0F=OTA.
 *		FifoQ - Where the received packet is included.
 *			+ 0=Low MAC RXFIFO. xMRxFIFO(0x00~0x7F)
 *			+ 1=High MAC RXFIFO. xMRxFIFO(0x80~0xFF)
 *		MsduRP - Index of MAC RXFIFO that indicates the starting index of payload.
 *			+ (e.g.)
 *			+ FifoQ=0 and MsduRP=0x10 --> 1st payload is in xMRxFIFO(0x00+0x10)
 *			+ FifoQ=1 and MsduRP=0x20 --> 1st payload is in xMRxFIFO(0x80+0x20)
 *
 * 	RETURN		: whether the received packet is processed in this function
 *		1 - processed. ZUSER_OOB_DATA_IND() is not called for the packet.
 *		0 - Not processed. ZUSER_OOB_DATA_IND() is called for the packet.
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
UINT8 ZUSER_OOB_RF_ISR(UINT8 OobCode, UINT8 FifoQ, UINT8 MsduRP)
{	
	OobCode = OobCode;
	FifoQ = FifoQ;
	MsduRP = MsduRP;
	return	0;
}

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

/***********************************************************************************
 *
 *	NAME		: ZUSER_APS_DATA_IND()
 *
 *	DESCRIPTION	: Callback function called by the stack when an APS Data packet, whose
 *		destination Endpoint is not 0x00, is received.
 *
 * 	PARAMETER	: pInd - pointer to APS Data packet(APSDE_DATA_ind)
 *
 * 	RETURN		: None
 *
 * 	NOTES		: If the destination Endpoint of the received packet is 0x00, 
 *		ZZDP_APS_DATA_IND() is called by the stack.
 *	
 ***********************************************************************************/
void ZUSER_APS_DATA_IND(APSDE_DATA_ind *pInd)
{
	pInd = pInd;
	
	zPrintf(1, "\n\n ZUSER_APS_DATA_IND");	
	zPrintf(1, " Src=%04x",pInd->SrcAdd_U.Short);
	Display_Buffer(pInd->pasdu, pInd->asduLen, 16);
}

/***********************************************************************************
 *
 *	NAME		: ZUSER_APS_ACK_TX_IND()
 *
 *	DESCRIPTION	: Callback function called by the stack after transmitting an APS Ack packet.
 *		When an APS packet which requires APS Ack is received, the stack transmits an APS
 *		Ack packet
 *
 * 	PARAMETER	: pAckSta - pointer to the status of transmitting(APSDME_ACK_Status)
 *
 * 	RETURN		: None
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
void ZUSER_APS_ACK_TX_IND(APSDME_ACK_Status *pAckSta)
{
	zPrintf(1, "\n\n ZUSER_APS_ACK_SEND_STATUS");
	zPrintf(1, " :Sta=%02x",(short)pAckSta->SendStatus);
	zPrintf(1, " :DSN=%02x",(short)pAckSta->DSN);
	zPrintf(1, " :DstAddr=%04x",pAckSta->DstAddr);
}

/***********************************************************************************
 *
 *	NAME		: ZUSER_APS_ACK_RX_IND()
 *
 *	DESCRIPTION	: Callback function called by the stack when an APS Ack packet is received
 *
 * 	PARAMETER	: pAckSta - pointer to APS Ack packet(APSDME_ACK_ind)
 *
 * 	RETURN		: None
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
void ZUSER_APS_ACK_RX_IND(APSDME_ACK_ind *pInd)
{
	zPrintf(1, "\n\n ZUSER_APS_ACK_IND");
	zPrintf(1, " :DSN=%02x",(short)pInd->DSN);	
	zPrintf(1, " :Clus=%04x",(short)pInd->ClusID);
	zPrintf(1, " :DstEP=%02x",(short)pInd->DstEP);
	zPrintf(1, " :ProfID=%04x",(short)pInd->ProfID);
	zPrintf(1, " :SrcEP=%02x",(short)pInd->SrcEP);
}

/***********************************************************************************
 *
 *	NAME		: ZUSER_APS_ESTABLISH_KEY_IND()
 *
 *	DESCRIPTION	: Callback function called by the stack when an APS SKKE Command
 *		(SKKE-1, SKKE-2, SKKE-3, SKKE-4) is received
 *
 * 	PARAMETER	: pInd - pointer to APS SKKE Command(APSME_ESTABLISH_KEY_ind)
 *
 * 	RETURN		: None
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
void ZUSER_APS_ESTABLISH_KEY_IND(APSME_ESTABLISH_KEY_ind *pInd)
{
	pInd = pInd;
	zPrintf(1, "\n\n ZUSER_APS_ESTABLISH_KEY_IND");	
}

/***********************************************************************************
 *
 *	NAME		: ZUSER_APS_TRANSPORT_KEY_IND()
 *
 *	DESCRIPTION	: Callback function called by the stack when an APS TRANSPORT KEY Command
 *		is received
 *
 * 	PARAMETER	: pInd - pointer to APS TRANSPORT KEY Command(APSME_TRANSPORT_KEY_ind)
 *
 * 	RETURN		: None
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
void ZUSER_APS_TRANSPORT_KEY_IND(APSME_TRANSPORT_KEY_ind *pInd)
{
#if defined __SECURED_WITH_PRECONFIGURED_NWK_KEY__ || defined __SECURED_WITHOUT_PRECONFIGURED_NWK_KEY__
	UINT8	NwkMaterialSetError;
	SS_NwkSecMaterial	_NwkSecMaterial;
#endif

	pInd = pInd;
	zPrintf(1, "\n\n ZUSER_APS_TRANSPORT_KEY_IND");

#if defined __SECURED_WITH_PRECONFIGURED_NWK_KEY__ || defined __SECURED_WITHOUT_PRECONFIGURED_NWK_KEY__
	if( (SIB.PreconfigMode == 1) || (SIB.PreconfigMode == 2) )
	{
		if(pInd->KeyType == aKT_StandNwk)
		{
			zPrintf(1, " :KeyNum=%02x",(short)pInd->KeyData.NWK.KeySeqNum);

			NwkMaterialSetError = 0;
			if(Check_16Byte_Null(pInd->KeyData.NWK.NwkKey))
			{
				// The received key is NULL.
			}	
			else
			{					
				_NwkSecMaterial.EnaSM = 1;
				rpmemcpy(_NwkSecMaterial.Key, pInd->KeyData.NWK.NwkKey, 16);
				_NwkSecMaterial.KeySeqNum = pInd->KeyData.NWK.KeySeqNum;
				_NwkSecMaterial.KeyType = aKT_StandNwk;
				memset(_NwkSecMaterial.OutFrameCnt, 0x00, 4);
				if(ZSEC_SetNwkSecMaterial(&_NwkSecMaterial))
				{
					zPrintf(1, "\n Material Set Err");
					NwkMaterialSetError = 1;
				}
			}

			if(NwkMaterialSetError == 0)
			{
				if(pInd->KeyData.NWK.KeySeqNum == 0)
				{
					rpmemcpy(SIB.apsTrustCenIEEE, pInd->SrcIEEE, 8);

					if(NIB.DEVTYPE == 'r')
					{					
					#if (UserConfig_DevType == 1)		// ZR
						APS2NWK.START_ROU_REQ.BattExtend = 0;
						APS2NWK.START_ROU_REQ.BO = 15;
						APS2NWK.START_ROU_REQ.SO = 15;				
						ZNWK_START_ROUTE_REQ(&APS2NWK.START_ROU_REQ, &NWK2APS.START_ROU_CON);

						APS2NWK.PERMIT_JOIN_REQ.PermitDur = 0xFF;
						ZNWK_PERMIT_JOIN_REQ(&APS2NWK.PERMIT_JOIN_REQ, &NWK2APS.PERMIT_JOIN_CON);
					#endif
					}
					ZSYS_SEND_DEVICE_ANNOUNCE();
				}

				// In case of End Device, the active key number is changed after receiving a Transport Key Command
				if(NIB.DEVTYPE == 'E')
				{
					SIB.nwkActKeySeqNum = pInd->KeyData.NWK.KeySeqNum;
				}
				//
			}
			
		}
	}
#endif	
	
}

/***********************************************************************************
 *
 *	NAME		: ZUSER_APS_REQUEST_KEY_IND()
 *
 *	DESCRIPTION	: Callback function called by the stack when an APS REQUEST KEY Command
 *		is received
 *
 * 	PARAMETER	: pInd - pointer to APS REQUEST KEY Command(APSME_REQUEST_KEY_ind)
 *
 * 	RETURN		: None
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
void ZUSER_APS_REQUEST_KEY_IND(APSME_REQUEST_KEY_ind *pInd)
{
	pInd = pInd;
	zPrintf(1, "\n\n ZUSER_APS_REQUEST_KEY_IND");
}

/***********************************************************************************
 *
 *	NAME		: ZUSER_APS_SWITCH_KEY_IND()
 *
 *	DESCRIPTION	: Callback function called by the stack when an APS SWITCH KEY Command
 *		is received
 *
 * 	PARAMETER	: pInd - pointer to APS SWITCH KEY Command(APSME_SWITCH_KEY_ind)
 *
 * 	RETURN		: None
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
void ZUSER_APS_SWITCH_KEY_IND(APSME_SWITCH_KEY_ind *pInd)
{
	pInd = pInd;
	zPrintf(1, "\n\n ZUSER_APS_SWITCH_KEY_IND");	
}

/***********************************************************************************
 *
 *	NAME		: ZUSER_APS_UPDATE_DEVICE_IND()
 *
 *	DESCRIPTION	: Callback function called by the stack when an APS UPDATE DEVICE Command
 *		is received
 *
 * 	PARAMETER	: pInd - pointer to APS UPDATE DEVICE Command(APSME_UPDATE_DEV_ind)
 *
 * 	RETURN		: None
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
void ZUSER_APS_UPDATE_DEVICE_IND(APSME_UPDATE_DEV_ind *pInd)
{
#if defined __SECURED_WITH_PRECONFIGURED_NWK_KEY__ || defined __SECURED_WITHOUT_PRECONFIGURED_NWK_KEY__
	UINT8	STA;
#endif

	pInd = pInd;
	zPrintf(1, "\n\n ZUSER_APS_UPDATE_DEVICE_IND");	

#if defined __SECURED_WITH_PRECONFIGURED_NWK_KEY__ || defined __SECURED_WITHOUT_PRECONFIGURED_NWK_KEY__
	if( (SIB.PreconfigMode == 1) || (SIB.PreconfigMode == 2) )
	{
		if(NIB.DEVTYPE == 'C')
		{
			if(pInd->Status != 0x02)
			{
				if(ZSEC_FindNwkSecMaterial_WithKeySeq(SIB.nwkActKeySeqNum, 0x00))
				{
					rpmemcpy(APP2APS.TRANS_KEY_REQ.DstIEEE, pInd->DevIEEE, 8);
					APP2APS.TRANS_KEY_REQ.KeyType = aKT_StandNwk;
					APP2APS.TRANS_KEY_REQ.KeyData.NWK.KeySeqNum = SIB.nwkActKeySeqNum;
					rpmemcpy(APP2APS.TRANS_KEY_REQ.KeyData.NWK.NwkKey, SIB.pnwkSecMaterial[NumNwkSecMat].Key, 16);
					if( (SIB.PreconfigMode == 1) && (SIB.nwkActKeySeqNum == 0) )
					{
						memset(APP2APS.TRANS_KEY_REQ.KeyData.NWK.NwkKey, 0x00, 16);		// Null Key
					}
					APP2APS.TRANS_KEY_REQ.KeyData.NWK.UseParent = 1;					
					rpmemcpy(APP2APS.TRANS_KEY_REQ.KeyData.NWK.ParentIEEE, pInd->SrcIEEE, 8);
					STA = ZAPS_TRANSPORT_KEY_REQ(&APP2APS.TRANS_KEY_REQ);
					zPrintf(1, "\n ZAPS_TRANSPORT_KEY_REQ.Sta=%02x",(short)STA);
				}
			}		
		}	
	}
#endif
	
}

/***********************************************************************************
 *
 *	NAME		: ZUSER_APS_REMOVE_DEVICE_IND()
 *
 *	DESCRIPTION	: Callback function called by the stack when an APS REMOVE DEVICE Command
 *		is received
 *
 * 	PARAMETER	: pInd - pointer to APS REMOVE DEVICE Command(APSME_REMOVE_DEVICE_ind)
 *
 * 	RETURN		: None
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
void ZUSER_APS_REMOVE_DEVICE_IND(APSME_REMOVE_DEVICE_ind *pInd)
{
	pInd = pInd;
	zPrintf(1, "\n\n ZUSER_APS_REMOVE_DEVICE_IND");
}	


////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

/***********************************************************************************
 *
 *	NAME		: ZUSER_NWK_JOIN_IND()
 *
 *	DESCRIPTION	: Callback function called by the stack when a new device is joined successfully
 *		by association or NWK rejoining.
 *
 * 	PARAMETER	: pInd - pointer to NLME Join Indication(NLME_JOIN_ind)
 *
 * 	RETURN		: None
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
void ZUSER_NWK_JOIN_IND(NLME_JOIN_ind *pInd)
{
#if defined __SECURED_WITH_PRECONFIGURED_NWK_KEY__ || defined __SECURED_WITHOUT_PRECONFIGURED_NWK_KEY__
	UINT8	_ActKeyNum;
	UINT8	STA;
	UINT8	_SecureAllFrame;
#endif

	zPrintf(1, "\n ZUSER_NWK_JOIN_IND");
	zPrintf(1, "\n - ShortAddr=%04x",(short)pInd->ShortAddr);
	zPrintf(1, "\n - IEEE(0~7) : ");
	Display_Buffer(pInd->ExtendAddr, 8, 0);
	zPrintf(1, "\n - Capa=%02x",(short)pInd->CapaInfo);
	zPrintf(1, "\n - Rejoin=%02x",(short)pInd->RejoinNwk);
	zPrintf(1, "\n - Sec=%02x",(short)pInd->SecRejoin);

#if defined __SECURED_WITH_PRECONFIGURED_NWK_KEY__ || defined __SECURED_WITHOUT_PRECONFIGURED_NWK_KEY__
	if( (SIB.PreconfigMode == 1) || (SIB.PreconfigMode == 2) )
	{
		if(NIB.DEVTYPE == 'C')
		{
			if(pInd->RejoinNwk != 2)
			{
				if(SIB.PreconfigMode == 1)
				{
					_ActKeyNum = SIB.nwkActKeySeqNum;
					SIB.nwkActKeySeqNum = 0;

					rpmemcpy(APP2APS.TRANS_KEY_REQ.DstIEEE, pInd->ExtendAddr, 8);
					APP2APS.TRANS_KEY_REQ.KeyType = aKT_StandNwk;
					APP2APS.TRANS_KEY_REQ.KeyData.NWK.KeySeqNum = SIB.nwkActKeySeqNum;					
					memset(APP2APS.TRANS_KEY_REQ.KeyData.NWK.NwkKey, 0x00, 16);		// Null Key
					APP2APS.TRANS_KEY_REQ.KeyData.NWK.UseParent = 0;
					//APP2APS.TRANS_KEY_REQ.KeyData.NWK.ParentIEEE		// Not used
					STA = ZAPS_TRANSPORT_KEY_REQ(&APP2APS.TRANS_KEY_REQ);
					zPrintf(1, "\n TRANSPORT_KEY_REQ.Sta=%02x",(short)STA);

					SIB.nwkActKeySeqNum = _ActKeyNum;
				}
				else		// if(SIB.PreconfigMode == 2)
				{
					rpmemcpy(APP2APS.TRANS_KEY_REQ.DstIEEE, pInd->ExtendAddr, 8);
					APP2APS.TRANS_KEY_REQ.KeyType = aKT_StandNwk;
					APP2APS.TRANS_KEY_REQ.KeyData.NWK.KeySeqNum = SIB.nwkActKeySeqNum;					
					rpmemcpy(APP2APS.TRANS_KEY_REQ.KeyData.NWK.NwkKey, SIB.pnwkSecMaterial[NumNwkSecMat].Key, 16);
					APP2APS.TRANS_KEY_REQ.KeyData.NWK.UseParent = 0;
					//APP2APS.TRANS_KEY_REQ.KeyData.NWK.ParentIEEE		// Not used

					_SecureAllFrame = SIB.nwkSecureAllFrame;
					SIB.nwkSecureAllFrame = 0;					// Prevent NWK Security
					STA = ZAPS_TRANSPORT_KEY_REQ(&APP2APS.TRANS_KEY_REQ);
					SIB.nwkSecureAllFrame = _SecureAllFrame;		// Prevent NWK Security					
					
					zPrintf(1, "\n TRANSPORT_KEY_REQ.Sta=%02x",(short)STA);
				}
			}			
		}
		else			// if(NIB.DEVTYPE == 'R')
		{
			if(pInd->RejoinNwk != 2)
			{
				rpmemcpy(APP2APS.UPDATE_DEV_REQ.DstIEEE, SIB.apsTrustCenIEEE, 8);
				rpmemcpy(APP2APS.UPDATE_DEV_REQ.DevIEEE, pInd->ExtendAddr, 8);
				APP2APS.UPDATE_DEV_REQ.Status = (pInd->SecRejoin)? 0x00 : 0x01;
				APP2APS.UPDATE_DEV_REQ.DevShortAddr = pInd->ShortAddr;
				STA = ZAPS_UPDATE_DEVICE_REQ(&APP2APS.UPDATE_DEV_REQ);
				zPrintf(1, "\n UPDATE_KEY_REQ.Sta=%02x",(short)STA);
			}
		}
	}
#endif	
}

/***********************************************************************************
 *
 *	NAME		: ZUSER_NWK_LEAVE_IND()
 *
 *	DESCRIPTION	: Callback function called by the stack when a NWK LEAVE Command is 
 *		received(A device is left from the network)
 *
 * 	PARAMETER	: pInd - pointer to NLME Leave Indication(NLME_LEAVE_ind)
 *
 * 	RETURN		: None
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
void ZUSER_NWK_LEAVE_IND(NLME_LEAVE_ind *pInd)
{
	pInd = pInd;
	zPrintf(1, "\n ZUSER_NWK_LEAVE_IND");

	//
	// It is recommended that the information of a leaved device is deleted
	//
	if(Check_8Byte_Null(pInd->DevAddr))
	{
		// I has been leaved.
	}
	else
	{
		NWKLIB_DevMgmt_DeleteIEEEAddr(pInd->DevAddr);
	}	
}

/***********************************************************************************
 *
 *	NAME		: ZUSER_NWK_STATUS_IND()
 *
 *	DESCRIPTION	: Callback function called by the stack when a NWK STATUS Command is received
 *		or NWK layer notifies the Application that there are changes in NWK layer.
 *
 * 	PARAMETER	: pInd - pointer to NLME Status Indication(NLME_NWK_STATUS_ind)
 *
 * 	RETURN		: None
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
void ZUSER_NWK_STATUS_IND(NLME_NWK_STATUS_ind *pInd)
{	
	zPrintf(1, "\n\n ZUSER_NWK_STATUS_IND");
	zPrintf(1, " -Sta=%02x",(short)pInd->Sta);
	zPrintf(1, " -NwkAddr=%04x",pInd->NwkAddr);

	if( (pInd->Sta == nNwkStaCmd_TreeLinkFail)
	|| (pInd->Sta == nNwkStaCmd_NonTreeLinkFail) )
	{
		#if (UserConfig_Capability & 0x02)		// FFD Library
		// The corresponding routing table can be deleted.
		NWKLIB_DeleteRT_UsedEntry(pInd->NwkAddr);
		#endif
	}
	else if(pInd->Sta == nNwkStaCmd_M2ORouteFail)
	{
		// M2O Route Request can be retried
	}
	else if( (pInd->Sta == nNwkStaCmd_NoRoute)
		|| (pInd->Sta == nNwkStaCmd_NoRouteCapa) )
	{
		#if (UserConfig_Capability & 0x02)		// FFD Library
		// The corresponding routing table may be deleted.
		NWKLIB_DeleteRT_UsedEntry(pInd->NwkAddr);	
		#endif
	}
	else if(pInd->Sta == nNwkStaCmd_TargetUnavail)
	{
		#if (UserConfig_Capability & 0x02)		// FFD Library	
		// The corresponding neighbor table may be deleted.
		NWKLIB_DevMgmt_DeleteNWKAddr(pInd->NwkAddr);
		// The corresponding routing table may be deleted.
		NWKLIB_DeleteRT_UsedEntry(pInd->NwkAddr);
		#endif
	}
	else if(pInd->Sta == nNwkStaCmd_NoIndirectCapa)
	{
		// Parent device is failed to pending to it's ZED child device.
	}
	else if(pInd->Sta == nNwkStaCmd_IndirectExpire)
	{
		// Indirect transmisstion is failed because ZED Child's polling is failed.
	}	
}

/***********************************************************************************
 *
 *	NAME		: ZUSER_NWK_NwkAddrUpdate_IND()
 *
 *	DESCRIPTION	: Callback function called by the stack when a NWK STATUS Command is received
 *		and the status code is 0x00(Network Address Update). 
 *
 * 	PARAMETER	: pInd - pointer to NLME Status Indication(NLME_NWK_STATUS_ind)
 *
 * 	RETURN		: None
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
void ZUSER_NWK_NwkAddrUpdate_IND(NLME_NWK_STATUS_ind *pInd)
{
	UINT8	STA;
	
	zPrintf(1, "\n\n ZUSER_NWK_NwkAddrUpdate_IND");
	zPrintf(1, " -Sta=%02x",(short)pInd->Sta);
	zPrintf(1, " -NwkAddr=%04x",pInd->NwkAddr);

	if(pInd->Sta == nNwkStaCmd_NwkAddrUpdate)
	{
		// This means that my network address is changed for confliction or rejoining.
		// pInd->NwkAddr is the new address.		
		if( (NIB.DEVTYPE == 'R') || (NIB.DEVTYPE == 'E') )
		{
			//-----------------------------------------------------
			// NOTICE : This routine should not be modified
			//-----------------------------------------------------
			STA = ZSYS_SEND_DEVICE_ANNOUNCE();
			//-----------------------------------------------------

			zPrintf(1, "\n NwkAddrUpdate");
			zPrintf(1, "\n Send Dev-Annce => STA=%02x",(short)STA);			
		}		
	}	
}

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

/***********************************************************************************
 *
 *	NAME		: ZUSER_APS_CON()
 *
 *	DESCRIPTION	: Callback function called by the stack after transmitting a pended APS packet.
 *		An APS packet is pended when there is no route to a destination and APS Queue is empty.
 *
 * 	PARAMETER	: 
 *		pAPS2APP - pointer to the status of transmitting. Only pAPS2APP->DATA_CON.Sta is valid.
 *		Idx - 0=APS Data packet. 1=APS Command packet. 2=APS Ack packet.
 *
 * 	RETURN		: None
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
void ZUSER_APS_CON(APSDME_CON_PKT *pAPS2APP, UINT8 Idx)
{
	Idx = Idx;
	pAPS2APP = pAPS2APP;
	zPrintf(1, "\n ZUSER_APS_CON");
}

/***********************************************************************************
 *
 *	NAME		: ZUSER_NWK_CON()
 *
 *	DESCRIPTION	: Callback function called by the stack after transmitting a pended ROUTE REQUEST
 *		Command. 
 *
 * 	PARAMETER	: 
 *		pNWK2APS - pointer to the status of transmitting. Only pNWK2APS->ROU_DISC_CON.Sta is valid.
 *		Idx - Index of NWK Pending
 *			+ 0x80 = M2O ROUTE REQUEST Command was pended.
 *			+ 0x81 = Multicast or Unicast ROUTE REQUEST Command was pended.
 *
 * 	RETURN		: None
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
void ZUSER_NWK_CON(NLDME_CON_PKT *pNWK2APS, UINT8 Idx)
{
	Idx = Idx;
	pNWK2APS = pNWK2APS;
	zPrintf(1, "\n ZUSER_NWK_CON");
}

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
 
/***********************************************************************************
 *
 *	NAME		: ZUSER_RESET_NIB()
 *
 *	DESCRIPTION	: Callback function called by the stack when ZNWK_RESET_REQ() is called.
 *		The routines is as follows.
 *			ZNWK_RESET_REQ()
 *			{
 *				// Initializing NWK Layer
 *				ZSYS_ALLOCATE_MEMORY();
 * 				ZNWK_RESET_NIB();
 *			} 			
 *		In this function, NIB variables can be set to values other than defaults.
 *
 * 	PARAMETER	: None
 *
 * 	RETURN		: None
 *
 * 	NOTES		: None
 *	
 ***********************************************************************************/
void ZUSER_RESET_NIB()
{
	UINT8	UserEPID[8];
#ifdef __SECURED_WITH_PRECONFIGURED_NWK_KEY__	
	SS_NwkSecMaterial	_NwkSecMaterial;
#endif	
	
	NIB.nwkCapaInfo = UserConfig_Capability;

	UserEPID[0] = UserConfig_EPID0;
	UserEPID[1] = UserConfig_EPID1;
	UserEPID[2] = UserConfig_EPID2;
	UserEPID[3] = UserConfig_EPID3;
	UserEPID[4] = UserConfig_EPID4;
	UserEPID[5] = UserConfig_EPID5;
	UserEPID[6] = UserConfig_EPID6;
	UserEPID[7] = UserConfig_EPID7;
	ZSYS_SET_EPID(UserEPID);

	NIB.nwkMaxDepth = UserConfig_MaxDepth;

	NIB.nwkMaxBroadcastRetry = UserConfig_MaxBroadcastRetry;
	NIB.InitRREQRetry = UserConfig_InitRREQRetry;
	NIB.RREQRetry = UserConfig_RREQREtry;
	NIB.TxRetry = UserConfig_NwkTxRetry;

	NIB.nwkNetBroadDeliverTime = UserConfig_BroadDeliverTime;
	NIB.nwkLinkStatusPeriod = UserConfig_LinkStatusPeriod;
	NIB.nwkTransPersisTime = UserConfig_TransPersistTime;
	NIB.RouteDiscTime = UserConfig_RouteDiscTime;
	NIB.WdtPeriod = UserConfig_WdtPeriod;
	
	NIB.JoinPriority = UserConfig_JoinPriority;
	NIB.MaxLinkFail = UserConfig_MaxLinkFail;
	NIB.MinValidLQI = UserConfig_MinValidLQI;
	NIB.MinJoinLQI  = UserConfig_MinValidLQI;	  // Bryan

#ifdef __SECURED_WITH_PRECONFIGURED_NWK_KEY__
	SIB.PreconfigMode = 1;
	SIB.nwkSecurityLevel = ZBSEC_SUITE_CCM32;
	SIB.nwkActKeySeqNum = 0;
	SIB.nwkSecureAllFrame = 1;

	_NwkSecMaterial.EnaSM = 1;
	_NwkSecMaterial.Key[15] = PRECONFIG_NWK_KEY_15;
	_NwkSecMaterial.Key[14] = PRECONFIG_NWK_KEY_14;
	_NwkSecMaterial.Key[13] = PRECONFIG_NWK_KEY_13;
	_NwkSecMaterial.Key[12] = PRECONFIG_NWK_KEY_12;
	_NwkSecMaterial.Key[11] = PRECONFIG_NWK_KEY_11;
	_NwkSecMaterial.Key[10] = PRECONFIG_NWK_KEY_10;
	_NwkSecMaterial.Key[9] = PRECONFIG_NWK_KEY_9;
	_NwkSecMaterial.Key[8] = PRECONFIG_NWK_KEY_8;
	_NwkSecMaterial.Key[7] = PRECONFIG_NWK_KEY_7;
	_NwkSecMaterial.Key[6] = PRECONFIG_NWK_KEY_6;
	_NwkSecMaterial.Key[5] = PRECONFIG_NWK_KEY_5;
	_NwkSecMaterial.Key[4] = PRECONFIG_NWK_KEY_4;
	_NwkSecMaterial.Key[3] = PRECONFIG_NWK_KEY_3;
	_NwkSecMaterial.Key[2] = PRECONFIG_NWK_KEY_2;
	_NwkSecMaterial.Key[1] = PRECONFIG_NWK_KEY_1;
	_NwkSecMaterial.Key[0] = PRECONFIG_NWK_KEY_0;
	_NwkSecMaterial.KeySeqNum = 0;
	_NwkSecMaterial.KeyType = aKT_StandNwk;
	memset(_NwkSecMaterial.OutFrameCnt, 0x00, 4);
	ZSEC_SetNwkSecMaterial(&_NwkSecMaterial);
#endif

#ifdef __SECURED_WITHOUT_PRECONFIGURED_NWK_KEY__
	SIB.PreconfigMode = 2;
	SIB.nwkSecurityLevel = ZBSEC_SUITE_CCM32;
	SIB.nwkActKeySeqNum = 0;
	SIB.nwkSecureAllFrame = 1;
#endif
	
}


/***********************************************************************************
 *
 *	NAME		: ZUSER_ALLOCATE_NWKADDR()
 *
 *	DESCRIPTION	: Callback function called by the stack when a device requests to join by association.
 *
 * 	PARAMETER	: 
 *		pIEEE - pointer to IEEE address of a device to join
 *		CapaInfo - Capability Information of a device to join
 *
 * 	RETURN		: Network address to the device to join.
 *		0x0000 ~ 0xFFF7 - network address that is allocated by the application.
 *		0xFFF8 ~ 0xFFFF - The application does not allocate a network address. The stack does.
 *
 * 	NOTES		: Allocation using this function is out of ZigBee specification.
 *	
 ***********************************************************************************/
UINT16 ZUSER_ALLOCATE_NWKADDR(UINT8 *pIEEE, UINT8 CapaInfo)
{
	pIEEE = pIEEE;
	CapaInfo = CapaInfo;

	return	0xFFFF;

	//---------------------------------------------
	// Example : Allocaiton by the applicaton
	//---------------------------------------------
	#if 0
		UINT16	AllocatedNwkAddress;
		AllocatedNwkAddress = (pIEEE[1] << 8) | pIEEE[0];
		return AllocatedNwkAddress;	
	#endif
	//---------------------------------------------
}

/***********************************************************************************
 *
 *	NAME		: ZUSER_INTPR_DATA_IND()
 *
 *	DESCRIPTION	: Callback function called by the stack when a received packet's Frame Type 
 *			subfield in NWK Header's Frame Control field is 3, that means Inter-PAN type in 
 *			SE profile definition. In ZigBee specification, it means reserved.
 *
 * 	PARAMETER	: pInd - pointer to MAC Data Service Indication primitive(MCPS_DATA_IND)
 *
 * 	RETURN		: None
 *
 * 	NOTES		: This function is only used in SE profile application.
 *	
 ***********************************************************************************/
void ZUSER_INTPR_DATA_IND(MCPS_DATA_IND *pInd)
{
	pInd = pInd;
}


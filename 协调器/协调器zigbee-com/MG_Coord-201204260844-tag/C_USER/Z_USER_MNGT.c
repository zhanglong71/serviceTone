
/*******************************************************************************
	- Chip		: MG2450/2455
	- Vendor		: RadioPulse Inc, 2007.
	- Date		: 2010-03-02
	- Version		: VER 2.10	

	[2010-03-02] VER 2.10
	- For ZigBeePRO Development Kit V2.10
*******************************************************************************/

/***********************************************************************************
 *
 *	CAUTION !!!
 *
 *	This file(Z_USER_MNGT.c) includes functions, variables, definitions and others 
 *		related to OTA(Over-The-Air) Firmware Update.
 *	They are so critial, A user should not modify them.
 *	
 ***********************************************************************************/


#include	"C_HAL/WDT.h"
//#include	"C_HAL/SPI.h"

typedef struct 
{
	UINT8	CMD;
	UINT8	IEEEAddress[8];
} CTRL_REQ_PING;

typedef struct 
{
	UINT8	CMD;
	UINT8	IEEEAddress[8];
} CTRL_RSP_PING;

typedef struct 
{
	UINT8	CMD;
	UINT8	SecDelay;
} CTRL_REQ_RESET;
typedef struct 
{
	UINT8	CMD;
	UINT8	SecDelay;
} CTRL_RSP_RESET;

typedef struct 
{
	UINT8	CMD;
	UINT8	Pattern[8];
} CTRL_REQ_TEST;
typedef struct 
{
	UINT8	CMD;
	UINT8	Pattern[8];
} CTRL_RSP_TEST;

typedef struct 
{
	UINT8	CMD;
	UINT8	Reserved;
} CTRL_REQ_SYNC;
typedef struct 
{
	UINT8	CMD;
	UINT8	Reserved;
} CTRL_RSP_SYNC;

typedef struct 
{
	UINT8	CMD;
	UINT8	Addr[4];
	UINT8	Size;
	UINT8	Payload[64];
} CRAM_REQ_WRIT;

typedef struct 
{
	UINT8	CMD;
	UINT8	Addr[4];
	UINT8	Size;
} CRAM_REQ_READ;

typedef struct 
{
	UINT8	CMD;
	UINT8	Addr[4];
	UINT8	Size;
	UINT8	Payload[64];
} CRAM_RSP_READ;


typedef struct 
{
	UINT8	CMD;
	UINT8	Reserved;
} CTRL_REQ_ERASE;
typedef struct 
{
	UINT8	CMD;
	UINT8	Reserved;
} CTRL_RSP_ERASE;


typedef struct 
{
	UINT8	CMD;
	UINT8	Reserved;
} CTRL_REQ_CRC;
typedef struct 
{
	UINT8	CMD;
	UINT8	CrcValue;
} CTRL_RSP_CRC;

typedef struct 
{
	UINT8	CMD;
	UINT8	Reserved;
} CTRL_REQ_RECOVER;
typedef struct 
{
	UINT8	CMD;
	UINT8	CrcValue;
} CTRL_RSP_RECOVER;

typedef struct 
{
	UINT8	CMD;
	UINT8	Addr[4];
	UINT8	Size;
	UINT8	Payload[64];
} CROM_REQ_WRIT;

typedef struct 
{
	UINT8	CMD;
	UINT8	Addr[4];
	UINT8	Size;
} CROM_REQ_READ;

typedef struct 
{
	UINT8	CMD;
	UINT8	Addr[4];
	UINT8	Size;
	UINT8	Payload[64];
} CROM_RSP_READ;

typedef struct 
{
	UINT8	CMD;
	UINT8	Payload[64+5];
} MNGT_COMMON;

typedef struct 
{
	UINT8	CMD;
	UINT8	EventMessage[8];
} CTRL_REQ_EVENT;

typedef struct 
{
	UINT8	CMD;
	UINT8	EventMessage[8];
} CTRL_RSP_EVENT;
	
typedef union
{ 
	MNGT_COMMON		vMNGT_COMMON;
	CTRL_REQ_PING		vCTRL_REQ_PING;		/*	0-0-0	*/	/* 0x00	*/
	CTRL_RSP_PING		vCTRL_RSP_PING;		/*	1-0-0	*/	/* 0x80	*/
	CTRL_REQ_RESET		vCTRL_REQ_RESET;		/*	0-0-1	*/	/* 0x01	*/
	CTRL_RSP_RESET		vCTRL_RSP_RESET;		/*	1-0-1	*/	/* 0x81	*/
	CTRL_REQ_TEST		vCTRL_REQ_TEST;		/*	0-0-2	*/	/* 0x02	*/
	CTRL_RSP_TEST		vCTRL_RSP_TEST;		/*	1-0-3	*/	/* 0x82	*/
	CTRL_REQ_SYNC		vCTRL_REQ_SYNC;		/*	0-0-3	*/	/* 0x03	*/
	CTRL_RSP_SYNC		vCTRL_RSP_SYNC;		/*	1-0-3	*/	/* 0x83	*/
	CTRL_REQ_ERASE		vCTRL_REQ_ERASE;		/*	0-0-4	*/	/* 0x04	*/
	CTRL_RSP_ERASE		vCTRL_RSP_ERASE;		/*	1-0-4	*/	/* 0x84	*/
	CTRL_REQ_CRC		vCTRL_REQ_CRC;			/*	0-0-5	*/	/* 0x05	*/
	CTRL_RSP_CRC		vCTRL_RSP_CRC;			/*	1-0-5	*/	/* 0x85	*/
	CTRL_REQ_RECOVER	vCTRL_REQ_RECOVER;	/*	0-0-6	*/	/* 0x06	*/
	CTRL_RSP_RECOVER	vCTRL_RSP_RECOVER;	/*	1-0-6	*/	/* 0x86	*/

	CTRL_REQ_EVENT		vCTRL_REQ_EVENT;		/*	0-0-e	*/	/* 0x0e	*/	
	CTRL_RSP_EVENT		vCTRL_RSP_EVENT;		/*	1-0-e	*/	/* 0x8e	*/
	
	
	CRAM_REQ_WRIT		vCRAM_REQ_WRIT;		/*	0-1-0	*/	/* 0x10	*/
	CRAM_REQ_READ		vCRAM_REQ_READ;		/*	0-1-1	*/	/* 0x11	*/
	CRAM_RSP_READ		vCRAM_RSP_READ;		/*	1-1-1	*/	/* 0x91	*/

	CROM_REQ_WRIT		vCROM_REQ_WRIT;		/*	0-2-0	*/	/* 0x20	*/
	CROM_REQ_READ		vCROM_REQ_READ;		/*	0-2-1	*/	/* 0x21	*/
	CROM_RSP_READ		vCROM_RSP_READ;		/*	1-2-1	*/	/* 0xA1	*/
} MNGT_PACKET;
	
	
#define		dCTRL_REQ_PING		 0x00	
#define		dCTRL_RSP_PING		 	 0x80	
#define		dCTRL_REQ_RESET		 0x01	
#define		dCTRL_RSP_RESET		 0x81	
#define		dCTRL_REQ_TEST		 0x02	
#define		dCTRL_RSP_TEST		 	 0x82	
#define		dCTRL_REQ_SYNC		 0x03	
#define		dCTRL_RSP_SYNC		 0x83	
#define		dCTRL_REQ_ERASE		0x04	
#define		dCTRL_RSP_ERASE		0x84	
#define		dCTRL_REQ_CRC			0x05	
#define		dCTRL_RSP_CRC			0x85	
#define		dCTRL_REQ_RECOVER	 	0x06	
#define		dCTRL_RSP_RECOVER	 	0x86	
#define		dCRAM_REQ_WRIT		0x10	
#define		dCRAM_REQ_READ		0x11	
#define		dCRAM_RSP_READ		0x91	
#define		dCROM_REQ_WRIT		0x20	
#define		dCROM_REQ_READ		0x21	
#define		dCROM_RSP_READ		0xA1	

#define		dCTRL_REQ_EVENT		0x0e
#define		dCTRL_RSP_EVENT		0x8e

#define		dMAC_AddrMode_64		3
#define		dMAC_TxOptions_ACK	0x01

#if(UserConfig_Enable_OTA)

MCPS_DATA_REQ	vMCPS;
MCPS_DATA_CON	*pCON;

MNGT_PACKET	vMNGT;
MNGT_PACKET	*pRcvMNGT;
MNGT_PACKET	*pSndMNGT;
UINT32			cAddr;
UINT8			*pIEEEAddr;	
UINT8			pageBuf[256];


void SPI_ON();
void SPI_OFF();
void ZOTA_FLASH_ERASE();
void ZOTA_PAGE_PROGRAM(UINT32 SpiAddr,UINT8 *pBuff);
void ZOTA_PAGE_READ(UINT32 SpiAddr,UINT8 *pBuff,UINT16 Size);
UINT8 ZHAL_SPI_MASTER_POLLING(UINT8 SendByte);	

extern void ZOTA_REBOOT();
	
#endif
//
//
//


void ZUSER_OOB_MNGT_IND(MCPS_DATA_IND *pInd, UINT8 OobIndex)
{
UINT8	PingOK;
UINT8	tmp8;
UINT16	Tmp16;
UINT8   code *pRead;

#if(UserConfig_Enable_OTA)

	pSndMNGT = (MNGT_PACKET *)&vMNGT;
	pRcvMNGT = (MNGT_PACKET *)pInd->pmsdu;
	pIEEEAddr = (UINT8 *)MPIB.IEEE_ExtendAddr;

	//
	//	NOTICE : If Response packet Received, Forward to Host PC - Installer Program
	//
	#if (AppOption_ProfileSimulator)
	if( pRcvMNGT->vMNGT_COMMON.CMD & 0x80 )
	{
		rpmemcpy(&UartOut.PKT.Arguments[0], (UINT8 *) &(pInd->SrcAddrMode), 23);
		
		Swap_Buffer_2B(UartOut.PKT.Arguments, 1);
		if(pInd->SrcAddrMode == mAM_16)
		{
		Swap_Buffer_2B(UartOut.PKT.Arguments, 3);
		Swap_Buffer_2B(UartOut.PKT.Arguments, 5);
		Swap_Buffer_2B(UartOut.PKT.Arguments, 7);
		Swap_Buffer_2B(UartOut.PKT.Arguments, 9);
		}

		Swap_Buffer_2B(UartOut.PKT.Arguments, 12);
		if(pInd->DstAddrMode== mAM_16)
		{
		Swap_Buffer_2B(UartOut.PKT.Arguments, 14);
		Swap_Buffer_2B(UartOut.PKT.Arguments, 16);
		Swap_Buffer_2B(UartOut.PKT.Arguments, 18);
		Swap_Buffer_2B(UartOut.PKT.Arguments, 20);
		}
		UartOut.PKT.Arguments[23] = pInd->msduLinkQuality;
		UartOut.PKT.Arguments[24] = pInd->SecurityUse;
		UartOut.PKT.Arguments[25] = pInd->ACLEntry;
		rpmemcpy(&UartOut.PKT.Arguments[26], pInd->pmsdu, pInd->msduLength);
		
		UartOut.PKT.Size = 26 + pInd->msduLength;
		UartOut.LEN = UartOut.PKT.Size + 4;
		UartOut.PKT.layer = OOB_LAYER;
		UartOut.PKT.Opcode = OobIndex;
		UartOut.PKT.Type = IND_TYPE;

		ZSYS_UART_API_SEND();
	}
	#endif
	

	if(OobIndex == 0x0f)
	{
		//
		//
		vMCPS.SrcAddrMode	= mAM_64;
		vMCPS.SrcPanId		= 0xffff;
		rpmemcpy(vMCPS.SrcAddr_U.Ext,pIEEEAddr,8);

		vMCPS.DstAddrMode	= mAM_64;
		vMCPS.DstPanId		= 0xffff;
		rpmemcpy(vMCPS.DstAddr_U.Ext,pInd->SrcAddr_U.Ext,8);


		vMCPS.pmsdu			= (UINT8 *)&vMNGT;
		vMCPS.msduHandle	= 0;
		vMCPS.TxOptions		= 0xf0;						/*	OOB-15	*/
		vMCPS.TxOptions		= vMCPS.TxOptions | 0x01;	/* 	ACK		*/

	//	zPrintf(1,"\n vMNGT_COMMON.CMD : %02x",(int)pRcvMNGT->vMNGT_COMMON.CMD);
		
		switch(pRcvMNGT->vMNGT_COMMON.CMD)
		{
			//
			//
			//
			case dCTRL_REQ_PING		:
				
				PingOK    =1;
				for( tmp8=0;tmp8<8;tmp8++)
				{
					if(pRcvMNGT->vCTRL_REQ_PING.IEEEAddress[tmp8] != 0xff )
					{
						if(pRcvMNGT->vCTRL_REQ_PING.IEEEAddress[tmp8] != pIEEEAddr[tmp8])
						{
							PingOK	=0;
						}
					}
				}

				if( PingOK )
				{	
					vMCPS.msduLength	= sizeof(vMNGT.vCTRL_RSP_PING);
					vMCPS.pmsdu[0]		= pInd->pmsdu[0] | 0x80;
					rpmemcpy(vMNGT.vCTRL_RSP_PING.IEEEAddress, MPIB.IEEE_ExtendAddr,8);
					
					//
					//
					#if 0
					ZSYS_Sleep( ( pIEEEAddr[0]%10 )*50 );	/* 50ms Random Scheduling to prevent RF Traffic Jamming	*/
					#endif
					//
					//
					ZMAC_DATA_REQ(&vMCPS, (MCPS_DATA_CON *)&vMCPS);
				}
				break;
			//
			//	Remote Reset Command
			//
			case dCTRL_REQ_RESET	:	
				{
					vMCPS.msduLength	= sizeof(vMNGT.vCTRL_RSP_RESET);
					vMCPS.pmsdu[0]		= pInd->pmsdu[0] | 0x80;
					ZMAC_DATA_REQ(&vMCPS, (MCPS_DATA_CON *)&vMCPS);
					//
					//	Code fetch from external flash memory
					//	reboot_from_ext_flash()
					//
					//
				
					if(pRcvMNGT->vCTRL_REQ_RESET.SecDelay == 0xff)
					{
						ZOTA_REBOOT();
				 	}
					else
					{
						EA=0;
						WDT = 0x01;
						WDT |= 0x10;
						WDT |= 0x08;
						while(1) ;
					}

				}
				break;
			//
			//
			//
			case dCTRL_REQ_TEST		:	
				{
					vMCPS.msduLength	= sizeof(vMNGT.vCTRL_RSP_TEST);
					vMCPS.pmsdu[0]		= pInd->pmsdu[0] | 0x80;
					rpmemcpy(pSndMNGT->vCTRL_RSP_TEST.Pattern,pRcvMNGT->vCTRL_REQ_TEST.Pattern,8);
					ZMAC_DATA_REQ(&vMCPS, (MCPS_DATA_CON *)&vMCPS);
				}	
				break;	
			//
			//
			//
			case dCTRL_REQ_SYNC	:	
				{
					vMCPS.msduLength	= sizeof(vMNGT.vCTRL_RSP_SYNC);
					vMCPS.pmsdu[0]		= pInd->pmsdu[0] | 0x80;
					ZMAC_DATA_REQ(&vMCPS, (MCPS_DATA_CON *)&vMCPS);
					//
					//	Code fetch from external flash memory
					//	reboot_from_ext_flash()
					ZOTA_REBOOT();
					//
					//
				}	
				break;
			//
			//
			//
			case dCTRL_REQ_EVENT:	
				{
					vMCPS.msduLength	= sizeof(vMNGT.vCTRL_RSP_EVENT);
					vMCPS.pmsdu[0]		= pInd->pmsdu[0] | 0x80;

					rpmemcpy(vMNGT.vCTRL_RSP_EVENT.EventMessage, pRcvMNGT->vCTRL_REQ_EVENT.EventMessage, sizeof(vMNGT.vCTRL_RSP_EVENT.EventMessage));
					ZMAC_DATA_REQ(&vMCPS, (MCPS_DATA_CON *)&vMCPS);
				}	
				break;	
				
			//
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//
			case dCTRL_REQ_ERASE	:		
				{
				//	zPrintf(1,"\n ERASE");
					ZSYS_WDT_SET(0);
					
					vMCPS.msduLength	= sizeof(vMNGT.vCTRL_RSP_ERASE);
					vMCPS.pmsdu[0]		= pInd->pmsdu[0] | 0x80;
					//
					//	ERASE Ext-Flash
					ZOTA_FLASH_ERASE();
					//					
					ZSYS_WDT_SET(2000);
					ZMAC_DATA_REQ(&vMCPS, (MCPS_DATA_CON *)&vMCPS);
					pCON = (MCPS_DATA_CON *)&vMCPS;
					
				//	zPrintf(1,"\==>DONE(%02x)",(int)pCON->status);
				}
				break;	
			//
			//
			//
			case dCROM_REQ_WRIT	:	
				{
					cAddr = 0;
					cAddr |= pRcvMNGT->vCROM_REQ_WRIT.Addr[3];cAddr = cAddr <<8; 
					cAddr |= pRcvMNGT->vCROM_REQ_WRIT.Addr[2];cAddr = cAddr <<8; 
					cAddr |= pRcvMNGT->vCROM_REQ_WRIT.Addr[1];cAddr = cAddr <<8; 
					cAddr |= pRcvMNGT->vCROM_REQ_WRIT.Addr[0]; 
					
					rpmemcpy(&pageBuf[cAddr&0x000000ff],pRcvMNGT->vCROM_REQ_WRIT.Payload,64);
					
					if(	 cAddr <  0x18000)
					if( (cAddr&0x000000ff) == 0x00c0)
					{
					//	zPrintf(1,"\n ZOTA_PAGE_PROGRAM");
						ZOTA_PAGE_PROGRAM((cAddr&0xffffff00), pageBuf);
						memset(	pageBuf, 0xff, 256);
					//	zPrintf(1," ==> END");
					}
				}
				break;

			//
			//
			//
			case dCROM_REQ_READ	:		
				{
					ZSYS_WDT_SET(2000);
					
					vMCPS.msduLength	= sizeof(vMNGT.vCROM_RSP_READ);
					vMCPS.pmsdu[0]		= pInd->pmsdu[0] | 0x80;

					rpmemcpy(pSndMNGT->vCROM_RSP_READ.Addr,pRcvMNGT->vCROM_REQ_READ.Addr,4);
					pSndMNGT->vCROM_RSP_READ.Size=pRcvMNGT->vCROM_REQ_READ.Size;


					cAddr = 0;
					cAddr |= pRcvMNGT->vCROM_REQ_READ.Addr[3];cAddr = cAddr <<8; 
					cAddr |= pRcvMNGT->vCROM_REQ_READ.Addr[2];cAddr = cAddr <<8; 
					cAddr |= pRcvMNGT->vCROM_REQ_READ.Addr[1];cAddr = cAddr <<8; 
					cAddr |= pRcvMNGT->vCROM_REQ_READ.Addr[0]; 
					
					ZOTA_PAGE_READ(cAddr, pageBuf,64);
					rpmemcpy(pSndMNGT->vCROM_RSP_READ.Payload,pageBuf,pSndMNGT->vCROM_RSP_READ.Size);
				
					ZMAC_DATA_REQ(&vMCPS, (MCPS_DATA_CON *)&vMCPS);
				}
				break;

			//
			//
			//
			case dCTRL_REQ_CRC	:
				{
					ZSYS_WDT_SET(0);
					
					vMCPS.msduLength	= sizeof(vMNGT.vCTRL_RSP_CRC);
					vMCPS.pmsdu[0]		= pInd->pmsdu[0] | 0x80;
					pSndMNGT->vCTRL_RSP_CRC.CrcValue = 0x00;
					
					//zPrintf(1,"\n dCTRL_REQ_CRC");
					for(cAddr = 0; cAddr < (0x18000) ; cAddr=cAddr+256)
					{
					//	zPrintf(1,".");
						ZOTA_PAGE_READ(cAddr, pageBuf,256);
						
						for(Tmp16=0;Tmp16<256;Tmp16++)
						{
							pSndMNGT->vCTRL_RSP_CRC.CrcValue += pageBuf[Tmp16];
						}
					}
				//	zPrintf(1,"==>END");
					
					ZSYS_WDT_SET(2000);
					
					ZMAC_DATA_REQ(&vMCPS, (MCPS_DATA_CON *)&vMCPS);
				}
				break;	
			//
			//
			//
			//	memory copy from int-flash to ext-flash
			//
			case dCTRL_REQ_RECOVER	:	
				{
					vMCPS.msduLength	= sizeof(vMNGT.vCTRL_RSP_RECOVER);
					vMCPS.pmsdu[0]		= pInd->pmsdu[0] | 0x80;

					#if 0
					//
					ZOTA_FLASH_ERASE();
					//
					for(cAddr = 0; cAddr < (96*1024) ; cAddr=cAddr+256)
					{
						rpmemcpy(pageBuf,(UINT8 code *)cAddr,256);
						ZOTA_PAGE_PROGRAM(cAddr, pageBuf);
					}
					//
					#endif
					//
					//	response
					//
					ZMAC_DATA_REQ(&vMCPS, (MCPS_DATA_CON *)&vMCPS);
				}
			break;	
		//
		//
		//
			case dCRAM_REQ_READ   :
			{
				ZSYS_WDT_SET(2000);
				vMCPS.msduLength        = sizeof(vMNGT.vCRAM_RSP_READ);
				vMCPS.pmsdu[0]            = pInd->pmsdu[0] | 0x80;
				rpmemcpy(pSndMNGT->vCRAM_RSP_READ.Addr,pRcvMNGT->vCRAM_REQ_READ.Addr,4);
				pSndMNGT->vCRAM_RSP_READ.Size=pRcvMNGT->vCRAM_REQ_READ.Size;
				
				cAddr = 0;
				cAddr |= pRcvMNGT->vCRAM_REQ_READ.Addr[3];cAddr = cAddr <<8; 
				cAddr |= pRcvMNGT->vCRAM_REQ_READ.Addr[2];cAddr = cAddr <<8; 
				cAddr |= pRcvMNGT->vCRAM_REQ_READ.Addr[1];cAddr = cAddr <<8; 
				cAddr |= pRcvMNGT->vCRAM_REQ_READ.Addr[0]; 

				pRead = (UINT8 code *)cAddr;
				//rpmemcpy(pSndMNGT->vCRAM_RSP_READ.Payload,pRead,pSndMNGT->vCRAM_RSP_READ.Size);
				memcpy(pSndMNGT->vCRAM_RSP_READ.Payload,pRead,pSndMNGT->vCRAM_RSP_READ.Size);
				ZMAC_DATA_REQ(&vMCPS, (MCPS_DATA_CON *)&vMCPS);
			}
			break;
			
			default : break;
		}	
	}
	
#endif

	return;
}

//
//
//
#if 1	// ATMEL 25F1024

	#define spiWREN          	0x06      	// Set Write Enable Latch
	#define spiWRDI           	0x04     	// Reset Write Enable Latch
	#define spiRDSR 	     		0x05     	// read Status Register
	#define spiWRSR	    	 	0x01      	// Write Status Register
	#define spiREAD           	0x03     	// Read Data from Memory Array
	#define spiPROGRAM       	0x02  	// Program Data into Memory Array
	#define spiSECTOR_ERASE 0x52       // Erase One Sector in Memory Array
	
	#define spiCHIP_ERASE     	0x60    	// Erase All Sectors in Memory Array
	#define spiRDID           	0x15 	// Read Manufacturer and Product ID
	
	#define	SFLASH_TOTAL_SIZE	128*1024
	#define	SFLASH_PAGE_SIZE	256
	#define	SFLASH_PAGE_NUM	(SFLASH_TOTAL_SIZE/SFLASH_PAGE_SIZE)

#endif

extern void ZOTA_SPI_ON();
extern void ZOTA_SPI_OFF();
extern UINT8 ZOTA_SPI_MASTER_POLLING(UINT8 SendByte);

//
//
//
void ZOTA_FLASH_ERASE()
{
UINT8 Status;

	ZOTA_SPI_ON();
	ZOTA_SPI_MASTER_POLLING(spiWREN);
	ZOTA_SPI_OFF();

	while(1) 
	{
		ZOTA_SPI_ON();
		ZOTA_SPI_MASTER_POLLING(spiRDSR);
		Status = ZOTA_SPI_MASTER_POLLING(0);
		ZOTA_SPI_OFF();
		if( (Status&0x01) == 0) break;
   	}

	//
	// Chip Erase
	ZOTA_SPI_ON();
	ZOTA_SPI_MASTER_POLLING(spiCHIP_ERASE);
	ZOTA_SPI_OFF();

	while(1) 
	{
		ZOTA_SPI_ON();
		ZOTA_SPI_MASTER_POLLING(spiRDSR);
		Status = ZOTA_SPI_MASTER_POLLING(0);
		ZOTA_SPI_OFF();
		if( (Status&0x01) == 0) break;
   	}

}
//
//
//
void ZOTA_PAGE_PROGRAM(UINT32 SpiAddr,UINT8 *pBuff)
{
UINT16	Tmp16;
UINT8	SpiWd;
UINT8	Status;

#define	_PAGESIZE	256

	{
		ZOTA_SPI_ON();
		ZOTA_SPI_MASTER_POLLING(spiWREN);
		ZOTA_SPI_OFF();

		ZOTA_SPI_ON();
		ZOTA_SPI_MASTER_POLLING(spiPROGRAM);
		ZOTA_SPI_MASTER_POLLING(SpiAddr>>16);
		ZOTA_SPI_MASTER_POLLING(SpiAddr>>8);
		ZOTA_SPI_MASTER_POLLING(SpiAddr>>0);
		//
		//	1-page(256) write
		for(Tmp16=0;Tmp16 < _PAGESIZE ;Tmp16++)
		{
			SpiWd =pBuff[Tmp16]; 
			ZOTA_SPI_MASTER_POLLING(SpiWd);
		}
		ZOTA_SPI_OFF();
	

		while(1) 
		{
			ZOTA_SPI_ON();
			ZOTA_SPI_MASTER_POLLING(spiRDSR);
			Status = ZOTA_SPI_MASTER_POLLING(0);
			ZOTA_SPI_OFF();
			if( (Status&0x01) == 0)  break;
	   	}
	}
}
//
//
//

void ZOTA_PAGE_READ(UINT32 SpiAddr,UINT8 *pBuff,UINT16 Size)
{
UINT16	Tmp16;
UINT8	SpiRd;
UINT8	_EA;

_EA = EA;

EA=0;

	while(1) 
	{
		ZOTA_SPI_ON();
		ZOTA_SPI_MASTER_POLLING(spiRDSR);
		SpiRd = ZOTA_SPI_MASTER_POLLING(0);
		ZOTA_SPI_OFF();
		if( (SpiRd&0x01) == 0)  break;
   	}

	//
	//
	// Read Page Data : cmd-addr/3-data/1
	ZOTA_SPI_ON();

	ZOTA_SPI_MASTER_POLLING(spiREAD);
	ZOTA_SPI_MASTER_POLLING(SpiAddr>>16);
	ZOTA_SPI_MASTER_POLLING(SpiAddr>>8);
	ZOTA_SPI_MASTER_POLLING(SpiAddr>>0);

	for(Tmp16=0;Tmp16 < Size ;Tmp16=Tmp16+1)
	{
		SpiRd = ZOTA_SPI_MASTER_POLLING(0);
		pBuff[Tmp16] = SpiRd;
	}

	ZOTA_SPI_OFF();

	while(1) 
	{
		ZOTA_SPI_ON();
		ZOTA_SPI_MASTER_POLLING(spiRDSR);
		SpiRd = ZOTA_SPI_MASTER_POLLING(0);
		ZOTA_SPI_OFF();
		if( (SpiRd&0x01) == 0)  break;
   	}

EA = _EA;		
}

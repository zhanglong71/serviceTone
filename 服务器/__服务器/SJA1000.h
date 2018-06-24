#ifdef  SJA1000_GLOBALS
#define SJA1000_EXT
#else
#define SJA1000_EXT  extern
#endif
//定义SJA1000的基址
#define SJA_BaseAdr  0X7F00

//定义本机的ID
#define ID 3
// 控制寄存器
//系统指针，指向SJA1000
#define         REG_CONTROL       SJA_BaseAdr+0x00       //内部控制寄存器
#define         REG_COMMAND       SJA_BaseAdr+0x01       //命令寄存器

#define         REG_STATUS        SJA_BaseAdr+0x02       //状态寄存器
#define RBS_Bit 0x01 /*接收缓冲器状态位 */
#define DOS_Bit 0x02 /*数据超载状态位 */
#define TBS_Bit 0x04 /*发送缓冲器状态位 */
#define TCS_Bit 0x08 /*发送完成状态位 */
#define RS_Bit  0x10 /*接收状态位 */
#define TS_Bit  0x20 /*发送状态位 */
#define ES_Bit  0x40 /*错误状态位 */
#define BS_Bit  0x80 /*总线状态位 */
#define TxFramInFo SJA_BaseAdr+16
#define         REG_INTERRUPT     SJA_BaseAdr+0x03       //中断寄存器
#define         REG_INTENABLE     SJA_BaseAdr+0x04       //中断使能寄存器
#define         REG_BTR0          SJA_BaseAdr+0x06       //总线定时寄存器0
#define         REG_BTR1          SJA_BaseAdr+0x07       //总线定时寄存器1
#define         REG_OCR           SJA_BaseAdr+0x08       //输出控制寄存器
#define         REG_TEST          SJA_BaseAdr+0x09       //测试寄存器

#define         REG_RESVER1       SJA_BaseAdr+0x0A       //保留1
#define         REG_ARBITRATE     SJA_BaseAdr+0x0B       //仲裁丢失捕捉
#define         REG_ERRCATCH      SJA_BaseAdr+0x0C       //错误代码捕捉
#define         REG_ERRLIMIT      SJA_BaseAdr+0x0D       //错误报警限额

#define         REG_RXERR         SJA_BaseAdr+0x0E         //接收错误计数器
#define         REG_TXERR         SJA_BaseAdr+0x0F         //发送错误计数器

#define         REG_ACR1          SJA_BaseAdr+0x10       //验收代码寄存器
#define         REG_ACR2          SJA_BaseAdr+0x11       //验收代码寄存器
#define         REG_ACR3          SJA_BaseAdr+0x12       //验收代码寄存器
#define         REG_ACR4          SJA_BaseAdr+0x13       //验收代码寄存器
#define         REG_AMR1          SJA_BaseAdr+0x14       //验收屏蔽寄存器
#define         REG_AMR2          SJA_BaseAdr+0x15       //验收屏蔽寄存器
#define         REG_AMR3          SJA_BaseAdr+0x16       //验收屏蔽寄存器
#define         REG_AMR4          SJA_BaseAdr+0x17       //验收屏蔽寄存器

// 发送缓冲区寄存器
#define         REG_TXBuffer1     SJA_BaseAdr+0x10         //发送缓冲区1
#define         REG_TXBuffer2     SJA_BaseAdr+0x11         //发送缓冲区2
#define         REG_TXBuffer3     SJA_BaseAdr+0x12         //发送缓冲区3
#define         REG_TXBuffer4     SJA_BaseAdr+0x13         //发送缓冲区4
#define         REG_TXBuffer5     SJA_BaseAdr+0x14         //发送缓冲区5
#define         REG_TXBuffer6     SJA_BaseAdr+0x15         //发送缓冲区6
#define         REG_TXBuffer7     SJA_BaseAdr+0x16         //发送缓冲区7
#define         REG_TXBuffer8     SJA_BaseAdr+0x17         //发送缓冲区8
#define         REG_TXBuffer9     SJA_BaseAdr+0x18         //发送缓冲区9
#define         REG_TXBuffer10    SJA_BaseAdr+0x19         //发送缓冲区10
#define         REG_TXBuffer11    SJA_BaseAdr+0x1A         //发送缓冲区11
#define         REG_TXBuffer12    SJA_BaseAdr+0x1B         //发送缓冲区12
#define         REG_TXBuffer13    SJA_BaseAdr+0x1C         //发送缓冲区13

// 接收缓冲区寄存器
#define         REG_RXBuffer1       SJA_BaseAdr+0x10       //接收缓冲区1
#define         REG_RXBuffer2       SJA_BaseAdr+0x11       //接收缓冲区2
#define         REG_RXBuffer3      SJA_BaseAdr+0x12        //接收缓冲区3
#define         REG_RXBuffer4       SJA_BaseAdr+0x13       //接收缓冲区4
#define         REG_RXBuffer5      SJA_BaseAdr+0x14        //接收缓冲区5
#define         REG_RXBuffer6     SJA_BaseAdr+0x15         //接收缓冲区6
#define         REG_RXBuffer7     SJA_BaseAdr+0x16         //接收缓冲区7
#define         REG_RXBuffer8     SJA_BaseAdr+0x17         //接收缓冲区8
#define         REG_RXBuffer9     SJA_BaseAdr+0x18         //接收缓冲区9
#define         REG_RXBuffer10     SJA_BaseAdr+0x19        //接收缓冲区10
#define         REG_RXBuffer11     SJA_BaseAdr+0x1A        //接收缓冲区11
#define         REG_RXBuffer12     SJA_BaseAdr+0x1B        //接收缓冲区12
#define         REG_RXBuffer13     SJA_BaseAdr+0x1C        //接收缓冲区13

#define         REG_RXCOUNT       SJA_BaseAdr+0x1D         //RX报文计数器
#define         REG_RBSA          SJA_BaseAdr+0x1E         //接收缓冲区起始地址
#define         REG_CDR           SJA_BaseAdr+0x1F         //时钟分频寄存器

/*
功能说明:   CAN控制器SJA1000通讯波特率.SJA1000的晶振为必须为16MHZ*/

#define         BTR0_Rate_5k      0xBF          //5KBPS的预设值
#define         BTR1_Rate_5k      0xFF          //5KBPS的预设值

#define         BTR0_Rate_10k      0x31          //10KBPS的预设值
#define         BTR1_Rate_10k      0x1C          //10KBPS的预设值

#define         BTR0_Rate_20k      0x18          //20KBPS的预设值
#define         BTR1_Rate_20k      0x1C         //20KBPS的预设值

#define         BTR0_Rate_40k      0x87          //40KBPS的预设值
#define         BTR1_Rate_40k      0xFF          //40KBPS的预设值

#define         BTR0_Rate_50k      0x47          //50KBPS的预设值
#define         BTR1_Rate_50k      0x2F          //50KBPS的预设值

#define         BTR0_Rate_80k      0x83          //80KBPS的预设值
#define         BTR1_Rate_80k      0xFF          //80KBPS的预设值

#define         BTR0_Rate_100k     0x43          //100KBPS的预设值
#define         BTR1_Rate_100k     0x2f          //100KBPS的预设值

#define         BTR0_Rate_125k     0x03          //125KBPS的预设值
#define         BTR1_Rate_125k     0x1c          //125KBPS的预设值

#define         BTR0_Rate_200k     0x81          //200KBPS的预设值
#define         BTR1_Rate_200k     0xFA          //200KBPS的预设值

#define         BTR0_Rate_250k     0x01          //250KBPS的预设值
#define         BTR1_Rate_250k     0x1c          //250KBPS的预设值

#define         BTR0_Rate_400k     0x80          //400KBPS的预设值
#define         BTR1_Rate_400k     0xfa          //400KBPS的预设值

#define         BTR0_Rate_500k     0x00          //500KBPS的预设值
#define         BTR1_Rate_500k     0x1c          //500KBPS的预设值

#define         BTR0_Rate_666k     0x80          //666KBPS的预设值
#define         BTR1_Rate_666k     0xb6          //666KBPS的预设值

#define         BTR0_Rate_800k     0x00          //800KBPS的预设值
#define         BTR1_Rate_800k     0x16          //800KBPS的预设值

#define         BTR0_Rate_1000k    0x00          //1000KBPS的预设值
#define         BTR1_Rate_1000k    0x14          //1000KBPS的预设值
//BPS
#define         ByteRate_5k        5
#define         ByteRate_10k       10
#define         ByteRate_20k       20
#define         ByteRate_40k       40
#define         ByteRate_50k       50
#define         ByteRate_80k       80
#define         ByteRate_100k      100
#define         ByteRate_125k      125
#define         ByteRate_200k      200
#define         ByteRate_400k      400
#define         ByteRate_500k      500
#define         ByteRate_800k      800
#define         ByteRate_1000k     1000

//命令字
#define    TR_CMD     0X01
#define    AT_CMD     0X02
#define    RRB_CMD    0X04
#define    COS_CMD    0X08
#define    SRR_CMD    0X10
#define    GTS_CMD    0X10
//错误字

#define CAN_INTERFACE_OK      0
#define CAN_BUS_OK            0
#define CAN_INTERFACE_ERR     0XFF
#define CAN_ENTERSET_ERR      0XFE
#define CAN_QUITSET_ERR       0XFD
#define CAN_INITOBJECT_ERR    0XFC
#define CAN_INITBTR_ERR       0XFB
#define CAN_INITOUTCTL_ERR    0XFA
#define CAN_INTCLKDIV_ERR     0XF9
#define CAN_BUS_ERR           0XF8
//SJA1000_EXT void HR_memcpy(unsigned char *p1,unsigned char *p2,unsigned char length1);
SJA1000_EXT bit BCAN_SET_OUTCLK(unsigned char Clock_Out);
SJA1000_EXT bit BCAN_SET_OBJECT(unsigned char  BCAN_ACR0,BCAN_ACR1,BCAN_ACR2,BCAN_ACR3,BCAN_AMR0,BCAN_AMR1,BCAN_AMR2,BCAN_AMR3);
SJA1000_EXT bit BCAN_SET_BANDRATE(unsigned char CAN_ByteRate);         //波特率选择
//SJA1000_EXT bit BCAN_SET_CONTROL(unsigned char CMD);
SJA1000_EXT bit BCAN_CREATE_COMMUNATION(void);
SJA1000_EXT bit BCAN_ENTER_RETMODEL(void);     //置位复位请求
SJA1000_EXT bit BCAN_CMD_PRG(unsigned char cmd);
//SJA1000_EXT unsigned char BCAN_DATA_WRITE(unsigned char *SendDataBuf);
SJA1000_EXT unsigned char CntCanTxTimeout;
SJA1000_EXT unsigned char CntCanRxLength;
SJA1000_EXT bit flag_CanTxTimeout;
SJA1000_EXT unsigned char CntCanRxTimeout;
SJA1000_EXT bit flag_CanRxTimeout;
SJA1000_EXT bit flag_CanRxHead;
SJA1000_EXT unsigned char packAcr0;
SJA1000_EXT unsigned char packAcr1;

SJA1000_EXT unsigned char Sja_1000_Init(unsigned char intMode);
SJA1000_EXT void CAN_Send_Small(unsigned char *CAN_TX_Buf,unsigned char length1,unsigned char PackNum,unsigned char packLength);
SJA1000_EXT void CAN_Send_Large(unsigned char *CAN_TX_Buf,unsigned char length1);
SJA1000_EXT void canSendRtr(unsigned char pAcr0,unsigned char pAcr1);

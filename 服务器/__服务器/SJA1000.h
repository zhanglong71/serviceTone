#ifdef  SJA1000_GLOBALS
#define SJA1000_EXT
#else
#define SJA1000_EXT  extern
#endif
//����SJA1000�Ļ�ַ
#define SJA_BaseAdr  0X7F00

//���屾����ID
#define ID 3
// ���ƼĴ���
//ϵͳָ�룬ָ��SJA1000
#define         REG_CONTROL       SJA_BaseAdr+0x00       //�ڲ����ƼĴ���
#define         REG_COMMAND       SJA_BaseAdr+0x01       //����Ĵ���

#define         REG_STATUS        SJA_BaseAdr+0x02       //״̬�Ĵ���
#define RBS_Bit 0x01 /*���ջ�����״̬λ */
#define DOS_Bit 0x02 /*���ݳ���״̬λ */
#define TBS_Bit 0x04 /*���ͻ�����״̬λ */
#define TCS_Bit 0x08 /*�������״̬λ */
#define RS_Bit  0x10 /*����״̬λ */
#define TS_Bit  0x20 /*����״̬λ */
#define ES_Bit  0x40 /*����״̬λ */
#define BS_Bit  0x80 /*����״̬λ */
#define TxFramInFo SJA_BaseAdr+16
#define         REG_INTERRUPT     SJA_BaseAdr+0x03       //�жϼĴ���
#define         REG_INTENABLE     SJA_BaseAdr+0x04       //�ж�ʹ�ܼĴ���
#define         REG_BTR0          SJA_BaseAdr+0x06       //���߶�ʱ�Ĵ���0
#define         REG_BTR1          SJA_BaseAdr+0x07       //���߶�ʱ�Ĵ���1
#define         REG_OCR           SJA_BaseAdr+0x08       //������ƼĴ���
#define         REG_TEST          SJA_BaseAdr+0x09       //���ԼĴ���

#define         REG_RESVER1       SJA_BaseAdr+0x0A       //����1
#define         REG_ARBITRATE     SJA_BaseAdr+0x0B       //�ٲö�ʧ��׽
#define         REG_ERRCATCH      SJA_BaseAdr+0x0C       //������벶׽
#define         REG_ERRLIMIT      SJA_BaseAdr+0x0D       //���󱨾��޶�

#define         REG_RXERR         SJA_BaseAdr+0x0E         //���մ��������
#define         REG_TXERR         SJA_BaseAdr+0x0F         //���ʹ��������

#define         REG_ACR1          SJA_BaseAdr+0x10       //���մ���Ĵ���
#define         REG_ACR2          SJA_BaseAdr+0x11       //���մ���Ĵ���
#define         REG_ACR3          SJA_BaseAdr+0x12       //���մ���Ĵ���
#define         REG_ACR4          SJA_BaseAdr+0x13       //���մ���Ĵ���
#define         REG_AMR1          SJA_BaseAdr+0x14       //�������μĴ���
#define         REG_AMR2          SJA_BaseAdr+0x15       //�������μĴ���
#define         REG_AMR3          SJA_BaseAdr+0x16       //�������μĴ���
#define         REG_AMR4          SJA_BaseAdr+0x17       //�������μĴ���

// ���ͻ������Ĵ���
#define         REG_TXBuffer1     SJA_BaseAdr+0x10         //���ͻ�����1
#define         REG_TXBuffer2     SJA_BaseAdr+0x11         //���ͻ�����2
#define         REG_TXBuffer3     SJA_BaseAdr+0x12         //���ͻ�����3
#define         REG_TXBuffer4     SJA_BaseAdr+0x13         //���ͻ�����4
#define         REG_TXBuffer5     SJA_BaseAdr+0x14         //���ͻ�����5
#define         REG_TXBuffer6     SJA_BaseAdr+0x15         //���ͻ�����6
#define         REG_TXBuffer7     SJA_BaseAdr+0x16         //���ͻ�����7
#define         REG_TXBuffer8     SJA_BaseAdr+0x17         //���ͻ�����8
#define         REG_TXBuffer9     SJA_BaseAdr+0x18         //���ͻ�����9
#define         REG_TXBuffer10    SJA_BaseAdr+0x19         //���ͻ�����10
#define         REG_TXBuffer11    SJA_BaseAdr+0x1A         //���ͻ�����11
#define         REG_TXBuffer12    SJA_BaseAdr+0x1B         //���ͻ�����12
#define         REG_TXBuffer13    SJA_BaseAdr+0x1C         //���ͻ�����13

// ���ջ������Ĵ���
#define         REG_RXBuffer1       SJA_BaseAdr+0x10       //���ջ�����1
#define         REG_RXBuffer2       SJA_BaseAdr+0x11       //���ջ�����2
#define         REG_RXBuffer3      SJA_BaseAdr+0x12        //���ջ�����3
#define         REG_RXBuffer4       SJA_BaseAdr+0x13       //���ջ�����4
#define         REG_RXBuffer5      SJA_BaseAdr+0x14        //���ջ�����5
#define         REG_RXBuffer6     SJA_BaseAdr+0x15         //���ջ�����6
#define         REG_RXBuffer7     SJA_BaseAdr+0x16         //���ջ�����7
#define         REG_RXBuffer8     SJA_BaseAdr+0x17         //���ջ�����8
#define         REG_RXBuffer9     SJA_BaseAdr+0x18         //���ջ�����9
#define         REG_RXBuffer10     SJA_BaseAdr+0x19        //���ջ�����10
#define         REG_RXBuffer11     SJA_BaseAdr+0x1A        //���ջ�����11
#define         REG_RXBuffer12     SJA_BaseAdr+0x1B        //���ջ�����12
#define         REG_RXBuffer13     SJA_BaseAdr+0x1C        //���ջ�����13

#define         REG_RXCOUNT       SJA_BaseAdr+0x1D         //RX���ļ�����
#define         REG_RBSA          SJA_BaseAdr+0x1E         //���ջ�������ʼ��ַ
#define         REG_CDR           SJA_BaseAdr+0x1F         //ʱ�ӷ�Ƶ�Ĵ���

/*
����˵��:   CAN������SJA1000ͨѶ������.SJA1000�ľ���Ϊ����Ϊ16MHZ*/

#define         BTR0_Rate_5k      0xBF          //5KBPS��Ԥ��ֵ
#define         BTR1_Rate_5k      0xFF          //5KBPS��Ԥ��ֵ

#define         BTR0_Rate_10k      0x31          //10KBPS��Ԥ��ֵ
#define         BTR1_Rate_10k      0x1C          //10KBPS��Ԥ��ֵ

#define         BTR0_Rate_20k      0x18          //20KBPS��Ԥ��ֵ
#define         BTR1_Rate_20k      0x1C         //20KBPS��Ԥ��ֵ

#define         BTR0_Rate_40k      0x87          //40KBPS��Ԥ��ֵ
#define         BTR1_Rate_40k      0xFF          //40KBPS��Ԥ��ֵ

#define         BTR0_Rate_50k      0x47          //50KBPS��Ԥ��ֵ
#define         BTR1_Rate_50k      0x2F          //50KBPS��Ԥ��ֵ

#define         BTR0_Rate_80k      0x83          //80KBPS��Ԥ��ֵ
#define         BTR1_Rate_80k      0xFF          //80KBPS��Ԥ��ֵ

#define         BTR0_Rate_100k     0x43          //100KBPS��Ԥ��ֵ
#define         BTR1_Rate_100k     0x2f          //100KBPS��Ԥ��ֵ

#define         BTR0_Rate_125k     0x03          //125KBPS��Ԥ��ֵ
#define         BTR1_Rate_125k     0x1c          //125KBPS��Ԥ��ֵ

#define         BTR0_Rate_200k     0x81          //200KBPS��Ԥ��ֵ
#define         BTR1_Rate_200k     0xFA          //200KBPS��Ԥ��ֵ

#define         BTR0_Rate_250k     0x01          //250KBPS��Ԥ��ֵ
#define         BTR1_Rate_250k     0x1c          //250KBPS��Ԥ��ֵ

#define         BTR0_Rate_400k     0x80          //400KBPS��Ԥ��ֵ
#define         BTR1_Rate_400k     0xfa          //400KBPS��Ԥ��ֵ

#define         BTR0_Rate_500k     0x00          //500KBPS��Ԥ��ֵ
#define         BTR1_Rate_500k     0x1c          //500KBPS��Ԥ��ֵ

#define         BTR0_Rate_666k     0x80          //666KBPS��Ԥ��ֵ
#define         BTR1_Rate_666k     0xb6          //666KBPS��Ԥ��ֵ

#define         BTR0_Rate_800k     0x00          //800KBPS��Ԥ��ֵ
#define         BTR1_Rate_800k     0x16          //800KBPS��Ԥ��ֵ

#define         BTR0_Rate_1000k    0x00          //1000KBPS��Ԥ��ֵ
#define         BTR1_Rate_1000k    0x14          //1000KBPS��Ԥ��ֵ
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

//������
#define    TR_CMD     0X01
#define    AT_CMD     0X02
#define    RRB_CMD    0X04
#define    COS_CMD    0X08
#define    SRR_CMD    0X10
#define    GTS_CMD    0X10
//������

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
SJA1000_EXT bit BCAN_SET_BANDRATE(unsigned char CAN_ByteRate);         //������ѡ��
//SJA1000_EXT bit BCAN_SET_CONTROL(unsigned char CMD);
SJA1000_EXT bit BCAN_CREATE_COMMUNATION(void);
SJA1000_EXT bit BCAN_ENTER_RETMODEL(void);     //��λ��λ����
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

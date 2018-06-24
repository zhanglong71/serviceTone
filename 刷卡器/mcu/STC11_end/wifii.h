#ifdef  WIFII_GLOBALS
#define WIFII_EXT
#else
#define WIFII_EXT  extern
#endif

#define COM_RX_NONE     0x00 //������
#define COM_RX_DATA     0x01 //���յ�����
#define COM_RX_DCONFIRM 0x02 //���յ�����ȷ��
#define COM_RX_CARD 0x03 	 //���յ�ic����Ϣ
#define NET_START     0x00   //׼������ģ��
#define NET_INIT      0x01   //�����ʼ�����
#define NET_JOIN      0x02   //�����������
#define NET_LEAVE     0x03   //�Ͽ�����
#define OS_ERR        0x01
#define OS_NO_ERR     0x02
#define COM_NO_ERR  0x01
#define COM_ERR_COR 0x02
#define COM_ERR_SER 0x03

#define MAX_Delay_time 200//������ʱʱ��5s
#define MAX_UART_Delay_time 200//���շ����������ʱ

WIFII_EXT unsigned char comSendDataBuf[80];
WIFII_EXT unsigned char comSendLength;
WIFII_EXT unsigned char comRxDataBuf[80];
WIFII_EXT unsigned char comRxLength;
WIFII_EXT unsigned char cntComRxIndex;
WIFII_EXT unsigned char cntComRxTimeout;
WIFII_EXT unsigned char staComRx;
WIFII_EXT unsigned char staNet;
WIFII_EXT unsigned char cntComRxTimeout;     //���ڽ��ռ�ʱ��
WIFII_EXT unsigned char flag_rx_time;        //ˢ��������
WIFII_EXT unsigned char flag_com_data_finish;
WIFII_EXT unsigned char flag_com_confirm_finish;

//WIFII_EXT unsigned char comTxDataConfirm(unsigned char *p);
WIFII_EXT unsigned char comTrCommand(char *p,unsigned char lenght);
WIFII_EXT void comSenData(unsigned char Len,unsigned char *p);					//�Ӵ��ڷ�������





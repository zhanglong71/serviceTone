#ifdef  WIFII_GLOBALS
#define WIFII_EXT
#else
#define WIFII_EXT  extern
#endif

#define COM_RX_NONE     0x00 //空数据
#define COM_RX_DATA     0x01 //接收到数据
#define COM_RX_DCONFIRM 0x02 //接收到数据确认
#define COM_RX_CARD 0x03 	 //接收到ic卡信息
#define NET_START     0x00   //准备连接模块
#define NET_INIT      0x01   //网络初始化完成
#define NET_JOIN      0x02   //加入网络完成
#define NET_LEAVE     0x03   //断开网络
#define OS_ERR        0x01
#define OS_NO_ERR     0x02
#define COM_NO_ERR  0x01
#define COM_ERR_COR 0x02
#define COM_ERR_SER 0x03

#define MAX_Delay_time 200//最大的延时时间5s
#define MAX_UART_Delay_time 200//接收房间号最大的延时

WIFII_EXT unsigned char comSendDataBuf[80];
WIFII_EXT unsigned char comSendLength;
WIFII_EXT unsigned char comRxDataBuf[80];
WIFII_EXT unsigned char comRxLength;
WIFII_EXT unsigned char cntComRxIndex;
WIFII_EXT unsigned char cntComRxTimeout;
WIFII_EXT unsigned char staComRx;
WIFII_EXT unsigned char staNet;
WIFII_EXT unsigned char cntComRxTimeout;     //串口接收计时器
WIFII_EXT unsigned char flag_rx_time;        //刷卡器催钟
WIFII_EXT unsigned char flag_com_data_finish;
WIFII_EXT unsigned char flag_com_confirm_finish;

//WIFII_EXT unsigned char comTxDataConfirm(unsigned char *p);
WIFII_EXT unsigned char comTrCommand(char *p,unsigned char lenght);
WIFII_EXT void comSenData(unsigned char Len,unsigned char *p);					//从串口发送数据





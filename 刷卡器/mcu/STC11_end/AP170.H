#ifdef  AP170_GLOBALS
#define AP170_EXT
#else
#define AP170_EXT  extern
#endif

#define		PUP1			0xC5
#define		PUP2			0x8D
#define		PDN1			0xE1
#define		PDN2			0xA9
#define		PLAY			0x55
#define		STATUS		0xE3
#define		PAUSE			0x39
#define		RESUME		0x1D
#define		PREFETCH	0x71

//AP170_EXT void delay(unsigned char us);   //delay time
//AP170_EXT void delay1(unsigned char ms);
AP170_EXT void player_init(void);
AP170_EXT void player_Num(unsigned char addr);
AP170_EXT void WR_Byte(unsigned char byte1);
AP170_EXT unsigned char player_busy(void);				//µ»¥˝”Ô“Ù–æ∆¨±‰œ–
AP170_EXT void player_PUP(void);
AP170_EXT void player_PDN(void);
AP170_EXT void LCD_Chk(unsigned char *chn,unsigned char NumLine,unsigned char chn_size,unsigned char NUM);
AP170_EXT void player_Time(unsigned char *p,unsigned char *chn1,unsigned char NumLine1,unsigned char chn_size1,unsigned char NUM);
AP170_EXT void player_Sev(unsigned char *p,unsigned char len,unsigned char *chn1,unsigned char NumLine1,unsigned char chn_size1,unsigned char NUM1);
AP170_EXT void player_INT(unsigned char addr);
AP170_EXT void delay_INT(unsigned int us);   //delay time
AP170_EXT void delay1_INT(unsigned int ms);
AP170_EXT void player_PDN_INT(void);
AP170_EXT void player_PUP_INT(void);
AP170_EXT void WR_Byte_INT(unsigned char byte1);

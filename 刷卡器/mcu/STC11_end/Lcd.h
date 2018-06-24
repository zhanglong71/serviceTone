#ifdef  LCD_GLOBALS
#define LCD_EXT
#else
#define LCD_EXT  extern
#endif

LCD_EXT void wr_lcd (unsigned char dat_comm,unsigned char content);
LCD_EXT void init_lcd(void);
LCD_EXT void chn_disp(unsigned char code *chn,unsigned char NumLine);
LCD_EXT void chn_disp_to_left(unsigned char *chn,unsigned char NumLine);
LCD_EXT void chn_left_disp1(unsigned char *chn,unsigned char NumLine,unsigned char chn_size,unsigned char display_pos,unsigned char Num);
LCD_EXT void chn_left_disp(unsigned char *chn,unsigned char NumLine,unsigned char chn_size,unsigned char display_pos);
LCD_EXT void img_disp(unsigned char code *img,unsigned char disline,unsigned char addr);
LCD_EXT void clrram(void);
//LCD_EXT void chk_busy(void);
LCD_EXT void delay1(unsigned int ms);
LCD_EXT void delay (unsigned int us);
LCD_EXT void Lcd_overtime(unsigned char lov,unsigned char disline);
LCD_EXT void SendByte(unsigned char Dbyte);
LCD_EXT void CheckBusy( void );
LCD_EXT void WriteCommand(unsigned char Cbyte );
LCD_EXT void WriteData(unsigned char Dbyte );
LCD_EXT unsigned char ReceiveByte(void);
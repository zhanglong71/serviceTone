#ifdef  RFID_GLOBALS
#define RFID_EXT
#else
#define RFID_EXT  extern
#endif

RFID_EXT void serSpeed_set();
RFID_EXT void serSpeed_restore();
RFID_EXT void TrCardId();           //十六进制转换成10位卡号
RFID_EXT unsigned char GetRfid();
RFID_EXT void RFID_GetID();
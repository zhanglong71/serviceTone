#ifdef  OS_GLOBALS
#define OS_EXT
#else
#define OS_EXT  extern
#endif

#define OS_ERR        0x01
#define OS_NO_ERR     0x02
OS_EXT unsigned int cntSemPend;
OS_EXT void OSSemPend(unsigned char *p,unsigned int timeOut,unsigned char *err);
OS_EXT void OSSemPost(unsigned char *p);


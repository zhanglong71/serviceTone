#ifdef  IAP_GLOBALS
#define IAP_EXT
#else
#define IAP_EXT  extern
#endif
#define ENABLE_ISP 0x81 //系统工作时钟<24MHz 时，对IAP_CONTR 寄存器设置此值
typedef union union_temp16
{
    unsigned int un_temp16;
    unsigned char un_temp8[2];
}union_temp16;
IAP_EXT union union_temp16 my_unTemp16;
IAP_EXT unsigned char Byte_Read(unsigned int add);              //读一字节，调用前需打开IAP 功能
IAP_EXT void Byte_Program(unsigned int add,unsigned char ch);  //字节编程，调用前需打开IAP 功能
IAP_EXT void Sector_Erase(unsigned int add);            //擦除扇区
IAP_EXT void IAP_Disable();                       //关闭IAP 功能
#ifdef  IAP_GLOBALS
#define IAP_EXT
#else
#define IAP_EXT  extern
#endif
#define ENABLE_ISP 0x81 //ϵͳ����ʱ��<24MHz ʱ����IAP_CONTR �Ĵ������ô�ֵ
typedef union union_temp16
{
    unsigned int un_temp16;
    unsigned char un_temp8[2];
}union_temp16;
IAP_EXT union union_temp16 my_unTemp16;
IAP_EXT unsigned char Byte_Read(unsigned int add);              //��һ�ֽڣ�����ǰ���IAP ����
IAP_EXT void Byte_Program(unsigned int add,unsigned char ch);  //�ֽڱ�̣�����ǰ���IAP ����
IAP_EXT void Sector_Erase(unsigned int add);            //��������
IAP_EXT void IAP_Disable();                       //�ر�IAP ����
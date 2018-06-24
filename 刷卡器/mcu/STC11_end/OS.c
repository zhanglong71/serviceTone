#define OS_GLOBALS
#include <includes.H>
void OSSemPend(INT8U *p,INT16U timeOut,INT8U *err)  //等待一个信号量
{
	cntSemPend=0;
	while(*p==0)
	{
		WDT_CONTR=0x3d;  //喂狗 1.13s;
		if(cntSemPend>=(timeOut/5))
		{
			*err=OS_ERR;
			return;
		}
	}
	*p=0;
	*err=OS_NO_ERR;
}
//void OSSemPost(INT8U *p)        //发出一个信号量
//{
//	*p=1;
//}


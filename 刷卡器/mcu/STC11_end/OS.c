#define OS_GLOBALS
#include <includes.H>
void OSSemPend(INT8U *p,INT16U timeOut,INT8U *err)  //�ȴ�һ���ź���
{
	cntSemPend=0;
	while(*p==0)
	{
		WDT_CONTR=0x3d;  //ι�� 1.13s;
		if(cntSemPend>=(timeOut/5))
		{
			*err=OS_ERR;
			return;
		}
	}
	*p=0;
	*err=OS_NO_ERR;
}
//void OSSemPost(INT8U *p)        //����һ���ź���
//{
//	*p=1;
//}


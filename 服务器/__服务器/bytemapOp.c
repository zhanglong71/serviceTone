
/** byteMap operation**/

#define CBYTEMAP_SIZE 	(16)
unsigned char data byteMap[CBYTEMAP_SIZE];

void bytemap_clean_interrupt(void)
{
	unsigned int i;
  	for(i = 0; i < CBYTEMAP_SIZE;i++)
  	{
  		byteMap[i] = 0;
  	}
}

void bytemap_clean(void)
{
	EA = 0;
	bytemap_clean_interrupt();
	EA = 1;
}

#if	0
void bytemap_setall(void)
{
	unsigned int i;
  	for(i = 0; i < CBYTEMAP_SIZE;i++)
  	{
  		byteMap[i] = ~0;
  	}
}
#endif

/***************************************************** 
 * ��λͼ�ϵ�ָ��λ����1����������1����֮ǰ��ֵ 
 *
 * input: offset (��0��ʼ��ƫ����)
 *        byteMap[]
 * output:byteMap[]
 * return: oldval of specified offset of byteMap[] 
 *
 * Note: main() can't use it
 *****************************************************/
unsigned char bytemap_test_set(unsigned int offset)
{
	unsigned char oldval;

	oldval = byteMap[offset];
	byteMap[offset] = 0xa5;  	/** ~0x0 **/

	return 	 oldval;
}
/**************************************************** 
 *
 * �鿴�Ƿ���ָ������Ŀ��ô���λ����1
 *
 * input: num
          byteMap[]
 * output: no
 * return: 1 - yes
 *         0 - no
 *
 ****************************************************/
int bytemap_isfull(unsigned int num)
{
	unsigned int i;
	
	for(i = 0; i < num; i++)
	{
		if(byteMap[i] == 0)         /** ǰ��Ķ��Ƿ�0ֵ, ����Ͳ������� **/
		{
			return 0;
		}
	}
	
	return i + 1;
}

#if	0
int bytemap_isEmpty(int num)
{
    for(i = 0; i < num;i++)
  	{
  		if(byteMap[i] != 0)
		{
			return 0;
		}
  	}
	/** now all 0**/
	return 1;
}
#endif

/** bitMap operation**/


#define	CBITMAP_BYTES	(2)
unsigned char data ucBitMap[CBITMAP_BYTES];

void bitmap_clean_interrupt(void)
{
	unsigned int i;
  	for(i = 0; i < CBITMAP_BYTES; i++)
  	{
  		ucBitMap[i] = 0;
  	}
}

void bitmap_clean(void)
{
	EA = 0;
	bitmap_clean_interrupt();
	EA = 1;
}

#if	0
void bitmap_setall(void)
{
	unsigned int i;
  	for(i = 0; i < CBITMAP_BYTES; i++)
  	{
  		ucBitMap[i] = 0xff;
  	}
}
#endif

/***************************************************** 
 * 将位图上的指定位置置1，并返回置1操作之前的值 
 *
 * input: offset (从0开始的偏移量)
 *        ucBitMap[]
 * output:ucBitMap[]
 * return: oldval of specified offset of ucBitMap[] 
 *
 * Note: main() can't use it
 *****************************************************/
unsigned char bitmap_test_set(unsigned int offset)
{
	unsigned int i, j;
	unsigned char oldval;

	unsigned char arr1[] = {
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x01, 0x01, 0x01, 0x01,
		0x01, 0x01, 0x01, 0x01,
	};

	unsigned char arr2[] = {
		0x01, 0x02, 0x04, 0x08,
		0x10, 0x20, 0x40, 0x80,
		0x01, 0x02, 0x04, 0x08,
		0x10, 0x20, 0x40, 0x80,
	};

	if(offset > 15)
	{
		return	1;
	}

	i = arr1[offset];    	   /**  i = (offset / 8); **/
	j = arr2[offset];        /**  j = (offset % 8);	**/


	//oldval = (ucBitMap[i] & j);
	//ucBitMap[i] = (ucBitMap[i] | j);
	ucBitMap[i] = (unsigned char)offset;

	return 	 0;	//oldval;
}
/**************************************************** 
 *
 * 查看是否有指定的数目那么多的位被置1
 *
 * input: num
          ucBitMap[]
 * output: no
 * return: 1 - yes
 *         0 - no
 *
 ****************************************************/
unsigned char bitmap_isfull(unsigned int num)
{
    unsigned int i, j, k;
	unsigned char tmp = 0;

	i = ((num - 1) >> 3);
	j = ((num - 1) & 0x07);

	for(k = 0; k < i; k++)  /** 前面的都是0xff, 否则就不是满的 **/
	{
		if(0xff != ucBitMap[k])
		{
			return 0;
		}
	}

	if(j == 0)
	{
		return 1;
	}
	
	for(k = 0; k < j; k++)
	{
	    tmp |= (1 << k);
	}

	if((tmp & ucBitMap[i]) == tmp)
	{
		return 1;
	}
	return 0;
}

#if	0
unsigned char bitmap_isEmpty(int num)
{
    for(i = 0; i < CBITMAP_BYTES; i++)
  	{
  		if(ucBitMap[i] != 0)
		{
			return 0;
		}
  	}
	/** now all 0**/
	return 1;
}
#endif
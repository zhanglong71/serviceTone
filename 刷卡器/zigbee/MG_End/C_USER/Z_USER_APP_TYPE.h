
typedef struct
{
	UINT16	ID;
	UINT8	DType;
	UINT8	Value;	
} ATTRIBUTE_U8;

typedef struct
{
	UINT16	ID;
	UINT8	DType;
	UINT16	Value;
} ATTRIBUTE_U16;

typedef struct
{
	UINT16	ID;
	UINT8	DType;
	UINT8	*pStr;
} ATTRIBUTE_Char;

	#define	ATT_OnOff_Off		0x00	// Refer to ZCL
	#define	ATT_OnOff_On		0x01	// Refer to ZCL
	#define	ATT_OnOff_Toggle	0x02	// Refer to ZCL

	#define	ATT_CtrlADC_Run	0x01
	#define	ATT_CtrlADC_Stop	0x00


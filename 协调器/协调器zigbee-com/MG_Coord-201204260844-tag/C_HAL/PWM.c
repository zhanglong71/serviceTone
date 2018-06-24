/*******************************************************************************
	- Chip		: MG2450/2455
	- Vendor		: RadioPulse Inc, 2007.
	- Date		: 2007-11-07
	- Version		: VER 1.5	
*******************************************************************************/

#include "INCLUDE_TOP.h"

//-- PORT : only PWM2, PWM3 
//	2	: PWM2. PWM Out = GPIO3.6(High-Driving IO of 12mA Driver)
//	3	: PWM3. PWM Out = GPIO3.7(High-Driving IO of 12mA Driver)

//-- Enable
//	1	: Enable
//	0	: Disable

//-- Freq
//	0			: 15.625 KHz(@ 8MHz System Clock), 31.250 KHz(@ 16MHz System Clock)
//	1			: 7.812 KHz(@ 8MHz System Clock), 15.625 KHz(@ 16MHz System Clock)
//	0x02 ~ 0xFF	: (System Clock) / ( (TH+1)*256 )

//-- HighDuty : High Duty in One Period Duration. ( (TL/256)*100 ) %

void ZHAL_PWM_SET(UINT8 Port, UINT8 Ena, UINT8 Freq, UINT8 HighDuty)
{
	UINT8	_EA;

	_EA = EA;
	EA = 0;	
		
	if(Port == 2)
	{
		if(Ena)
		{
			P3OEN &= ~(1 << 6);			// bit[6]=0
			TH2 = Freq;
			TL2 = HighDuty;
			T23CON |= 0x03;				// TR2=1, PWM2 Enable=1
		} 
		else
		{
			T23CON &= 0xFC;				// TR2=0, PWM3 Enable=0
			P3OEN |= (1 << 6);			// bit[6]=1
		}
	}
	else if(Port == 3)
	{
		if(Ena)
		{
			P3OEN &= ~(1 << 7);			// bit[7]=0
			TH3 = Freq;
			TL3 = HighDuty;
			T23CON |= 0x0C;				// TR3=1, PWM3 Enable=1
		} 
		else
		{
			T23CON &= 0xF3;				// TR3=0, PWM3 Enable=0
			P3OEN |= (1 << 7);			// bit[7]=1
		}
	}

	EA = _EA;
}

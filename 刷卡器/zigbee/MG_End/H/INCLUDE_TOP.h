/*******************************************************************************
	- Chip		: MG2450/55
	- Author		: RadioPulse Inc, 2007.
	- Date		: 2007-07-02
	- Version		: VER 1.0	
*******************************************************************************/


	#include <STDIO.h>
	#include <STRING.h>		// memcpy(), memcmp(), memset()
	#include <STDLIB.h>		// rand(), srand()
	#include <ABSACC.h>		// XBYTE
	#include <INTRINS.h>		// _nop_()
	#include <CTYPE.h>		// toint, toupper
	#include <STDARG.h>		// va_list, va_start(), vsprintf(), va_end();

	#include "MCU.h"
	#include "PROJECT_TYPE.h"
	#include "PROJECT_OPTION.h"
	#include "rpmemcpy.h"
	

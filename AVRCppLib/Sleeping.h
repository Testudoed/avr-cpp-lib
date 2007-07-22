/**********************************************************************************************************************\

	C++ library for Atmel AVR microcontrollers
	Copyright (C) 2007 Lauri Kirikal, Mikk Leini, MTÜ TTÜ Robotiklubi

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

	See http://creativecommons.org/licenses/GPL/2.0/

	MTÜ TTÜ Robotiklubi  http://www.robotiklubi.ee robotiklubi@gmail.com
	Lauri Kirikal        laurikirikal@gmail.com
	Mikk Leini           mikk.leini@gmail.com

\**********************************************************************************************************************/

#if (defined(__AVR_ATmega8__) || defined(__AVR_ATmega128__) || defined(__AVR_ATmega64__) || defined(__AVR_ATmega88__) || defined(__AVR_AT90USB1287__))
#define __SLEEPING_TYPE1__
#elif defined(__AVR_ATmega8515__)
#define __SLEEPING_TYPE2__
#endif

#if !(defined(__SLEEPING_TYPE1__) || defined(__SLEEPING_TYPE2__))
#error "Device is not selected or selected device is not supported."
#define __AVR_CPP_SLEEPING_H__
#endif

#ifndef __AVR_CPP_SLEEPING_H__
#define __AVR_CPP_SLEEPING_H__

#include "IO.h"
#include "Assembler.h"

namespace AVRCpp
{
	namespace Sleeping
	{
		enum EnabledFlag
		{
			Disabled	= 0x00,
			Enabled		= _SE
			
		}; // EnabledFlag
		
#ifdef __AVR_ATmega8515__
		
		enum SleepMode
		{
			Idle				= 0x00,	
			PowerDown			= _SM1,
			Standby				= _SM2 | _SM1,
			
		}; // enum SleepMode
		
#elif (defined(__AVR_ATmega128__) || defined(__AVR_ATmega64__) || defined(__AVR_ATmega88__) || defined(__AVR_AT90USB1287__) )
		
		enum SleepMode
		{
			Idle				= 0x00,	
			ADCNoiseReduction	= _SM0,
			PowerDown			= _SM1,
			PowerSave			= _SM1 | _SM0,
			Standby				= _SM2 | _SM1,
			ExtendedStandby		= _SM2 | _SM1 | _SM0
			
		}; // enum SleepMode
		
#elif __AVR_ATmega8__
		
		enum SleepMode
		{
			Idle				= 0x00,	
			ADCNoiseReduction	= _SM0,
			PowerDown			= _SM1,
			PowerSave			= _SM1 | _SM0,
			Standby				= _SM2 | _SM1,
			
		}; // enum SleepMode
		
#endif
		
	} // namespace Sleeping

} // namespace AVRCpp


#if defined(__SLEEPING_TYPE1__)
#include "common/Sleeping1.h"
#elif defined(__SLEEPING_TYPE2__)
#include "common/Sleeping2.h"
#else
#error "Device is not selected or selected device is not supported."
#endif

#endif // ifndef __AVR_CPP_SLEEPING_H__

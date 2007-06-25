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

#if !(defined(__AVR_ATmega8515__))
#error "Device is not selected or selected device is not supported."
#define __AVR_CPP_SLEEPING2_H__
#endif

#ifndef __AVR_CPP_SLEEPING2_H__
#define __AVR_CPP_SLEEPING2_H__

/* This file should only be included from <avr/cpp/Sleeping.h>, never directly. */

#ifdef __DOXYGEN__
#ifndef __AVR_CPP_SLEEPING_H__
#include <avr/cpp/Sleeping.h>
#else
#error "Include <avr/cpp/Sleeping.h> instead of <avr/cpp/common/Sleeping2.h>."
#endif
#endif

namespace AVRCpp
{
	namespace Sleeping
	{
		inline void SetUp(SleepMode sleepMode, EnabledFlag enabled)
		{
			ChangeBits<_MCUCR>(_SM1 | _SE, sleepMode | enabled);
			ChangeBits<_MCUCSR>(_SM2, sleepMode);
			ChangeBits<_EMCUCR>(_SM0, sleepMode);
			
		} // SetUp
		
		inline void FallAsleep() { Assembler::SLEEP(); }
		
		inline void Enable() { SetBits<_MCUCR>(_SE); }
		inline void Disable() { ClearBits<_MCUCR>(_SE); }
		inline uint8_t IsEnabled() { return IsBitsSet<_MCUCR>(_SE); }
		
		inline uint8_t GetSleepMode()
		{
			return ( SelectBits<_MCUCR>(_SM1)
					| SelectBits<_MCUCSR>(_SM2)
					| SelectBits<_EMCUCR>(_SM0));
		}
		inline void SetSleepMode(SleepMode sleepMode)
		{
			ChangeBits<_MCUCR>(_SM1, sleepMode);
			ChangeBits<_MCUCSR>(_SM2, sleepMode);
			ChangeBits<_EMCUCR>(_SM0, sleepMode);
		}
		
	} // namespace Sleeping

} // namespace AVRCpp

#endif // ifndef __AVR_CPP_SLEEPING2_H__

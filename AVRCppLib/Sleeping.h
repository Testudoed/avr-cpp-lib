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

#if !(defined(__AVR_ATmega8__) || defined(__AVR_ATmega128__))
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
			Enabled		= 0x80
			
		}; // EnabledFlag
		
		enum SleepMode
		{
			Idle				= 0x00,
			ADCNoiseReduction	= 0x10,
			PowerDown			= 0x20,
			PowerSave			= 0x30,
			Standby				= 0x60
			
		}; // SleepMode
		
		
		inline void SetUp(SleepMode sleepMode, EnabledFlag enabled)
		{
			ChangeBits<_MCUCR>(_SM0 | _SM1 | _SM2 | _SE, sleepMode | enabled);
			
		} // SetUp
		
		inline void FallAsleep() { Assembler::SLEEP(); }
		
		inline void Enable() { SetBits<_MCUCR>(_SE); }
		inline void Disable() { ClearBits<_MCUCR>(_SE); }
		inline uint8_t IsEnabled() { return IsBitsSet<_MCUCR>(_SE); }
		
		inline uint8_t GetSleepMode() { return SelectBits<_MCUCR>(_SM0 | _SM1 | _SM2); }
		inline void SetSleepMode(SleepMode sleepMode) { ChangeBits<_MCUCR>(_SM0 | _SM1 | _SM2, sleepMode); }
		
	} // namespace Sleeping

} // namespace AVRCpp

#endif // ifndef __AVR_CPP_SLEEPING_H__

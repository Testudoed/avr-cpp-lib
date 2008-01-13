/**********************************************************************************************************************\

	C++ library for Atmel AVR microcontrollers
	Copyright (C) 2007 Lauri Kirikal, Mikk Leini, Rasmus Raag, MTÜ TTÜ Robotiklubi

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
	Rasmus Raag          rasmus_raag@users.sourceforge.net

\**********************************************************************************************************************/

#ifndef __AVR_CPP_WATCHDOG_H__
#define __AVR_CPP_WATCHDOG_H__

#include "IO.h"
#include "Interrupt.h"

/* Enhanced watchdog ? */
#if defined(__AVR_AT90USB1287__)
#define __WATCDOG_ENHANCED__
#elif defined(__AVR_ATmega64__)
#define __WATCDOG_ENHANCED__
#elif defined(__AVR_ATmega88__)
#define __WATCDOG_ENHANCED__
#elif defined(__AVR_ATmega48__)
#define __WATCDOG_ENHANCED__
#elif defined(__AVR_ATmega168__)
#define __WATCDOG_ENHANCED__
#elif defined(__AVR_ATmega164P__)
#define __WATCDOG_ENHANCED__
#elif defined(__AVR_ATmega324P__)
#define __WATCDOG_ENHANCED__
#elif defined(__AVR_ATmega644P__)
#define __WATCDOG_ENHANCED__
#elif defined(__AVR_ATmega644__)
#define __WATCDOG_ENHANCED__
#endif


/**********************************************************************************************************************\

	AVRCppLib Watchdog
	
	There are two versions of watchdog timers - older and enhanced	

\**********************************************************************************************************************/

namespace AVRCpp
{
	namespace Watchdog
	{

#ifndef __WATCDOG_ENHANCED__

		namespace Internal
		{
			typedef _MCUCSR MCUStatusRegister;
			typedef _WDTCR WatchdogTimerControlRegister;
			
			typedef Bits<MCUStatusRegister, _WDRF> WatchdogResetFlag;			
			typedef Bits<WatchdogTimerControlRegister, _WDCE> WatchdogChangeEnableBit;
			typedef Bits<WatchdogTimerControlRegister, _WDE> WatchdogEnableBit;
		}

		/*
		 * Watchdog disabling command
		 */
		inline void Disable()
		{										
			Internal::WatchdogChangeEnableBit::Set();
			Internal::WatchdogEnableBit::Clear();
			Internal::WatchdogChangeEnableBit::Clear();		
		}

#else
				
		namespace Internal
		{
			typedef _MCUSR MCUStatusRegister;
			typedef _WDTCSR WatchdogTimerControlRegister;
			
			typedef Bits<MCUStatusRegister, _WDRF> WatchdogResetFlag;		
			typedef Bits<WatchdogTimerControlRegister, _WDCE> WatchdogChangeEnableBit;
			typedef Bits<WatchdogTimerControlRegister, _WDE> WatchdogSystemResetEnableBit;
		}
				
		/*
		 * Watchdog disabling command
		 */
		inline void Disable()
		{
			INTERRUPT_SAFE
			{
				Assembler::WDR();						
				Internal::WatchdogResetFlag::Clear();				
				SetBits<Internal::WatchdogTimerControlRegister>(_WDCE | _WDE);				
				Internal::WatchdogTimerControlRegister::Set(0);
			}
		}		
	
#endif

	} // namespace Watchdog

}; // namespace AVRCpp


/**********************************************************************************************************************/

#endif // ifndef __AVR_CPP_WATCHDOG_H__

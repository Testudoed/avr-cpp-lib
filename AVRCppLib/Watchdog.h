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

#ifdef __WATCDOG_ENHANCED__
#define WDT_ns		Watchdog
#define WDT_struct	Watchdog::TimeoutInterrupt
#endif

namespace AVRCpp
{
	namespace Watchdog
	{

#ifndef __WATCDOG_ENHANCED__

		enum Timeout
		{
			Cycles16K   = 0x00,
			Cycles32K   = 0x01,
			Cycles64K   = 0x02,
			Cycles128K  = 0x03,
			Cycles256K  = 0x04,
			Cycles512K  = 0x05,
			Cycles1024K = 0x06,
			Cycles2048K = 0x07
			
		}; // enum Timeout

		namespace Internal
		{
			enum BitFlags
			{
				WatchdogResetFlag	      = _WDRF,
				WatchdogChangeEnableFlag  = _WDCE,
				WatchdogEnableFlag        = _WDE,
				WatchdogPrescalerFlag     = _WDP2 | _WDP1 | _WDP0
					
			}; // enum BitFlags
		
			typedef _MCUCSR MCUStatusRegister;
			typedef _WDTCR WatchdogTimerControlRegister;
			
			typedef Bits<MCUStatusRegister, WatchdogResetFlag>                    WatchdogResetBit;			
			typedef Bits<WatchdogTimerControlRegister, WatchdogChangeEnableFlag>  WatchdogChangeEnableBit;
			typedef Bits<WatchdogTimerControlRegister, WatchdogEnableFlag>        WatchdogEnableBit;
			typedef Bits<WatchdogTimerControlRegister, WatchdogPrescalerFlag >    WatchdogPrescalerBits;
			
		} // namespace Internal
		
		/**
		 * Reset watchdog timer
		 */		 		
		static inline void ResetTimer()
		{
			Assembler::WDR();
			
		} // ResetTimer

		/**
		 * Watchdog setup
		 */
		static void Setup(Timeout timeout)
		{
			// Prepare watchdog configuration to apply it within 4 CPU cycles
			uint8_t tmp = Internal::WatchdogChangeEnableFlag | Internal::WatchdogEnableFlag |
			              (timeout & Internal::WatchdogPrescalerFlag);
			
			// Reset timer
			ResetTimer();
			
			// Allow changes
			SetBits<Internal::WatchdogTimerControlRegister>(Internal::WatchdogChangeEnableFlag |
			                                                Internal::WatchdogEnableFlag);
			                                                
			// I don't know whether it's good idea to set prescaler bits with CE bit, so i do it here
			Internal::WatchdogTimerControlRegister::Set(tmp);
			
			// Forbid changes			
			Internal::WatchdogChangeEnableBit::Clear();

		} // Setup

		/**
		 * Watchdog disabling
		 */
		static void Disable()
		{	
			// Reset timer
			ResetTimer();
			
			// Disable reset
			Internal::WatchdogResetBit::Clear();
			
			// Allow changes
			SetBits<Internal::WatchdogTimerControlRegister>(Internal::WatchdogChangeEnableFlag |
			                                                Internal::WatchdogEnableFlag);
			                                                																				
			// Disable everything and forbid changes			                                            
			Internal::WatchdogTimerControlRegister::Set(0);
														
		} // Disable
			
#else		

		enum Timeout
		{
			Cycles2K    = 0x00,
			Cycles4K    = 0x01,
			Cycles8K    = 0x02,
			Cycles16K   = 0x03,
			Cycles32K   = 0x04,
			Cycles64K   = 0x05,
			Cycles128K  = 0x06,
			Cycles256K  = 0x07,
			Cycles512K  = 0x08,
			Cycles1024K = 0x09
						
		}; // enum Timeout
		
		namespace Internal
		{
			enum BitFlags
			{
				WatchdogResetFlag	           = _WDRF,
				WatchdogInterruptEnableFlag    = _WDIE,
				WatchdogChangeEnableFlag       = _WDCE,
				WatchdogSystemResetEnableFlag  = _WDE,
				WatchdogPrescalerFlag          = _WDP3 | _WDP2 | _WDP1 | _WDP0
					
			}; // enum BitFlags
			
			typedef _MCUSR MCUStatusRegister;
			typedef _WDTCSR WatchdogTimerControlRegister;
			
			typedef Bits<MCUStatusRegister, WatchdogResetFlag>                        WatchdogResetBit;
			typedef Bits<WatchdogTimerControlRegister, WatchdogInterruptEnableFlag>   WatchdogInterruptEnableBit;		
			typedef Bits<WatchdogTimerControlRegister, WatchdogChangeEnableFlag>      WatchdogChangeEnableBit;
			typedef Bits<WatchdogTimerControlRegister, WatchdogSystemResetEnableFlag> WatchdogSystemResetEnableBit;		
			typedef Bits<WatchdogTimerControlRegister, WatchdogPrescalerFlag>         WatchdogPrescalerBits;
				
		} // namespace Internal
		
		struct TimeoutInterrupt : BasicInterrupt<Internal::WatchdogInterruptEnableBit> { __INTERRUPT_HANDLER_SUPPORT__ };
		
		/**
		 * Reset watchdog timer
		 */		 		
		static inline void ResetTimer()
		{
			Assembler::WDR();
			
		} // ResetTimer
		
		/**
		 * Watchdog setup
		 */		 
		static void Setup(Timeout timeout, bool enableReset)
		{
			// Prepare watchdog configuration to apply it within 4 CPU cycles
			uint8_t tmp = Internal::WatchdogChangeEnableFlag |
			              (enableReset ? Internal::WatchdogSystemResetEnableFlag : 0) |
			              (timeout & Internal::WatchdogPrescalerFlag);
			              
			INTERRUPT_SAFE
			{
				// Reset timer
				ResetTimer();	
				
				// Allow changes				
				SetBits<Internal::WatchdogTimerControlRegister>(Internal::WatchdogChangeEnableFlag |
				                                                Internal::WatchdogSystemResetEnableFlag);
				                                                
				// Configure
				Internal::WatchdogTimerControlRegister::Set(tmp);	
				
				// Forbid changes
				Internal::WatchdogChangeEnableBit::Clear();
			}	
			
		} // Setup		
				
		/**
		 * Watchdog disabling
		 */
		static void Disable()
		{
			INTERRUPT_SAFE
			{
				// Reset timer
				ResetTimer();
				
				// Disable reset
				Internal::WatchdogResetBit::Clear();
				
				// Allow changes
				SetBits<Internal::WatchdogTimerControlRegister>(Internal::WatchdogChangeEnableFlag |
				                                                Internal::WatchdogSystemResetEnableFlag);
				                                                
				// Disable everything and forbid changes
				Internal::WatchdogTimerControlRegister::Set(0);
			}
			
		} // Disable		

#endif

	} // namespace Watchdog

}; // namespace AVRCpp


/**********************************************************************************************************************/

#endif // ifndef __AVR_CPP_WATCHDOG_H__

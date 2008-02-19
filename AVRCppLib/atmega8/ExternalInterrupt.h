/**********************************************************************************************************************\

	C++ library for Atmel AVR microcontrollers
	Copyright (C) 2008 Lauri Kirikal, Mikk Leini, Rasmus Raag, MTU TTU Robotiklubi

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

	MTU TTU Robotiklubi  http://www.robotiklubi.ee robotiklubi@gmail.com
	Lauri Kirikal        laurikirikal@gmail.com
	Mikk Leini           mikk.leini@gmail.com
	Rasmus Raag          rasmus_raag@users.sourceforge.net

\**********************************************************************************************************************/

#ifndef __AVR_CPP_ATMEGA8_EXTERNAL_INTERRUPT_H__
#define __AVR_CPP_ATMEGA8_EXTERNAL_INTERRUPT_H__

#if defined(__DOXYGEN__) && !defined(__AVR_CPP_EXTERNAL_INTERRUPT_H__)
#include <avr/cpp/ExternalInterrupt.h>
#endif

#ifndef __AVR_CPP_EXTERNAL_INTERRUPT_H__
#error "Include <avr/cpp/ExternalInterrupt.h> instead of <avr/cpp/atmega8/ExternalInterrupt.h>"
#else

#ifndef __AVR_CPP_XXX_EXTERNAL_INTERRUPT_H__
#define __AVR_CPP_XXX_EXTERNAL_INTERRUPT_H__
#else
#error "Attempt to include more than one <avr/cpp/XXX/ExternalInterrupt.h> file"
#endif

/**********************************************************************************************************************\

	atmega8 external interrupts declarations.
	This file is generated. Do not modify manually !!!

\**********************************************************************************************************************/

#define INT0_ns	ExternalInterrupt
#define INT1_ns	ExternalInterrupt

#define INT0_struct	ExternalInterrupt::Interrupt0
#define INT1_struct	ExternalInterrupt::Interrupt1

namespace AVRCpp
{		
	namespace ExternalInterrupt
	{
	
		struct Interrupt0 : Internal::InterruptBase <
			InputPin2<PortD>,				/* InputPin */
			Event,						/* EventEnum */
			Bits<_MCUCR, _ISC00 | _ISC01>,	/* EventBits */
			Bits<_GICR, _INT0>,			/* InterruptEnableBit */
			Bits<_GIFR, _INTF0>,			/* InterruptFlagBit */
			0 >								/* eventShift */
			
		{ __INTERRUPT_HANDLER_SUPPORT__ }; // Interrupt0	
		struct Interrupt1 : Internal::InterruptBase <
			InputPin3<PortD>,				/* InputPin */
			Event,						/* EventEnum */
			Bits<_MCUCR, _ISC10 | _ISC11>,	/* EventBits */
			Bits<_GICR, _INT1>,			/* InterruptEnableBit */
			Bits<_GIFR, _INTF1>,			/* InterruptFlagBit */
			2 >								/* eventShift */
			
		{ __INTERRUPT_HANDLER_SUPPORT__ }; // Interrupt1	

	} // namespace ExternalInterrupts

} // namespace AVRCpp

/**********************************************************************************************************************/

#endif // ifndef __AVR_CPP_EXTERNAL_INTERRUPT_H__
#endif // ifndef __AVR_CPP_ATMEGA8_EXTERNAL_INTERRUPT_H__

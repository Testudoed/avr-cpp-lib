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

#ifndef __AVR_CPP_EXTERNAL_INTERRUPT_USB1287_H__
#define __AVR_CPP_EXTERNAL_INTERRUPT_USB1287_H__

/* This file should only be included from <avr/cpp/ExternalInterrupt.h>, never directly. */

#ifndef __AVR_CPP_EXTERNAL_INTERRUPT_H__
#error "Include <avr/cpp/ExternalInterrupt.h> instead of <avr/cpp/ExternalInterrupt_usb1287.h>."
#endif

#ifndef __AVR_CPP_EXTERNAL_INTERRUPT_XXX_H__
#define __AVR_CPP_EXTERNAL_INTERRUPT_XXX_H__ "ExternalInterrupt_usb1287.h"
#else
#error "Attempt to include more than one <avr/cpp/ExternalInterrupt_XXX.h> file."
#endif

#ifndef EXCLUDE_INTERRUPT_HANDLERS

#define INT0_ns ExternalInterrupt
#define INT1_ns	ExternalInterrupt
#define INT2_ns	ExternalInterrupt
#define INT3_ns	ExternalInterrupt
#define INT4_ns	ExternalInterrupt
#define INT5_ns	ExternalInterrupt
#define INT6_ns	ExternalInterrupt
#define INT7_ns	ExternalInterrupt
#define PCINT0_ns ExternalInterrupt

#define INT0_struct	ExternalInterrupt::Interrupt0
#define INT1_struct	ExternalInterrupt::Interrupt1
#define INT2_struct	ExternalInterrupt::Interrupt2
#define INT3_struct	ExternalInterrupt::Interrupt3
#define INT4_struct	ExternalInterrupt::Interrupt4
#define INT5_struct	ExternalInterrupt::Interrupt5
#define INT6_struct	ExternalInterrupt::Interrupt6
#define INT7_struct	ExternalInterrupt::Interrupt7
#define PCINT0_struct ExternalInterrupt::PinChangeInterrupt0

#endif // ifndef EXCLUDE_INTERRUPT_HANDLERS

namespace AVRCpp
{
	namespace ExternalInterrupt
	{
		__DECLARE_EXTERNAL_INTERRUPT__(_EICRA, _EIMSK, _EIFR, AsyncEvent, 0, InputPin0<PortD>, 0);
		__DECLARE_EXTERNAL_INTERRUPT__(_EICRA, _EIMSK, _EIFR, AsyncEvent, 1, InputPin1<PortD>, 2);
		__DECLARE_EXTERNAL_INTERRUPT__(_EICRA, _EIMSK, _EIFR, AsyncEvent, 2, InputPin2<PortD>, 4);
		__DECLARE_EXTERNAL_INTERRUPT__(_EICRA, _EIMSK, _EIFR, AsyncEvent, 3, InputPin3<PortD>, 6);
		__DECLARE_EXTERNAL_INTERRUPT__(_EICRB, _EIMSK, _EIFR, Event, 4, InputPin4<PortE>, 0);
		__DECLARE_EXTERNAL_INTERRUPT__(_EICRB, _EIMSK, _EIFR, Event, 5, InputPin5<PortE>, 2);
		__DECLARE_EXTERNAL_INTERRUPT__(_EICRB, _EIMSK, _EIFR, Event, 6, InputPin6<PortE>, 4);
		__DECLARE_EXTERNAL_INTERRUPT__(_EICRB, _EIMSK, _EIFR, Event, 7, InputPin7<PortE>, 6);
		
		__DECLARE_PIN_CHANGE_INTERRUPT__(_PCICR, _PCMSK, _PCIFR, 0);
		
	} // namespace ExternalInterrupts
	
} // namespace AVRCpp

#endif // ifndef __AVR_CPP_EXTERNAL_INTERRUPT_USB1287_H__

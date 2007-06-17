/**********************************************************************************************************************\

	C++ library for Atmel AVR microcontrollers
	Copyright (C) 2007 Lauri Kirikal, Mikk Leini, MT� TT� Robotiklubi

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

	MT� TT� Robotiklubi  http://www.robotiklubi.ee robotiklubi@gmail.com
	Lauri Kirikal        laurikirikal@gmail.com
	Mikk Leini           mikk.leini@gmail.com

\**********************************************************************************************************************/

#ifndef __AVR_CPP_EXTERNAL_INTERRUPT_M88_H__
#define __AVR_CPP_EXTERNAL_INTERRUPT_M88_H__

/* This file should only be included from <avr/cpp/ExternalInterrupt.h>, never directly. */

#ifndef __AVR_CPP_EXTERNAL_INTERRUPT_H__
#error "Include <avr/cpp/ExternalInterrupt.h> instead of <avr/cpp/ExternalInterrupt_m88.h>."
#endif

#ifndef __AVR_CPP_EXTERNAL_INTERRUPT_XXX_H__
#define __AVR_CPP_EXTERNAL_INTERRUPT_XXX_H__ "ExternalInterrupt_m88.h"
#else
#error "Attempt to include more than one <avr/cpp/ExternalInterrupt_XXX.h> file."
#endif

#ifndef EXCLUDE_INTERRUPT_HANDLERS

#define INT0_ns		ExternalInterrupt
#define INT1_ns		ExternalInterrupt
#define PCINT0_ns	ExternalInterrupt
#define PCINT1_ns	ExternalInterrupt
#define PCINT2_ns	ExternalInterrupt

#define INT0_struct		ExternalInterrupt::Interrupt0
#define INT1_struct		ExternalInterrupt::Interrupt1
#define PCINT0_struct	ExternalInterrupt::PinChangeInterrupt0
#define PCINT1_struct	ExternalInterrupt::PinChangeInterrupt1
#define PCINT2_struct	ExternalInterrupt::PinChangeInterrupt2

#endif // ifndef EXCLUDE_INTERRUPT_HANDLERS

namespace AVRCpp
{
	namespace ExternalInterrupt
	{
		__DECLARE_EXTERNAL_INTERRUPT__(_EICRA, _EIMSK, _EIFR, Event, 0, InputPin2<PortD>, 0);
		__DECLARE_EXTERNAL_INTERRUPT__(_EICRA, _EIMSK, _EIFR, Event, 1, InputPin3<PortD>, 2);
		
		__DECLARE_PIN_CHANGE_INTERRUPT__(_PCICR, _PCMSK, _PCIFR, 0);
		__DECLARE_PIN_CHANGE_INTERRUPT__(_PCICR, _PCMSK, _PCIFR, 1);
		__DECLARE_PIN_CHANGE_INTERRUPT__(_PCICR, _PCMSK, _PCIFR, 2);
		
	} // namespace ExternalInterrupts
	
} // namespace AVRCpp

#endif // ifndef __AVR_CPP_EXTERNAL_INTERRUPT_M88_H__
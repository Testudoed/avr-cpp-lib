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

#ifndef __AVR_CPP_EXTERNAL_INTERRUPT_M8_H__
#define __AVR_CPP_EXTERNAL_INTERRUPT_M8_H__

/* This file should only be included from <avr/cpp/ExternalInterrupt.h>, never directly. */

#ifndef __AVR_CPP_EXTERNAL_INTERRUPT_H__
#ifdef __DOXYGEN__
#include "ExternalInterrupt.h"
#else
#error "Include <avr/cpp/ExternalInterrupt.h> instead of <avr/cpp/ExternalInterrupt_m8.h>."
#endif
#endif // ifndef __AVR_CPP_EXTERNAL_INTERRUPT_H__

#ifndef __AVR_CPP_EXTERNAL_INTERRUPT_XXX_H__
#define __AVR_CPP_EXTERNAL_INTERRUPT_XXX_H__ "ExternalInterrupt_m8.h"
#else
#error "Attempt to include more than one <avr/cpp/ExternalInterrupt_XXX.h> file."
#endif


#ifndef EXCLUDE_INTERRUPT_HANDLERS

#define INT0_ns	ExternalInterrupt
#define INT1_ns	ExternalInterrupt

#define INT0_struct	INT0_ns::Interrupt0
#define INT1_struct	INT1_ns::Interrupt1

#endif // ifndef EXCLUDE_INTERRUPT_HANDLERS

namespace AVRCpp
{
	namespace ExternalInterrupt
	{
		__DECLARE_EXTERNAL_INTERRUPT__(_MCUCR, _GICR, _GIFR, Event, 0, InputPin2<PortD>, 0);
		__DECLARE_EXTERNAL_INTERRUPT__(_MCUCR, _GICR, _GIFR, Event, 1, InputPin3<PortD>, 2);
		
	} // ExternalInterrupt
	
} // namespace AVRCpp

#endif // ifndef __AVR_CPP_EXTERNAL_INTERRUPT_M8_H__

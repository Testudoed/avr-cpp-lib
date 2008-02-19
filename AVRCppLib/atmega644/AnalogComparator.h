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

#ifndef __AVR_CPP_ATMEGA644_ANALOG_COMPARATOR_H__
#define __AVR_CPP_ATMEGA644_ANALOG_COMPARATOR_H__

#if defined(__DOXYGEN__) && !defined(__AVR_CPP_ANALOG_COMPARATOR_H__)
#include <avr/cpp/AnalogComparator.h>
#endif

#ifndef __AVR_CPP_ANALOG_COMPARATOR_H__
#error "Include <avr/cpp/AnalogComparator.h> instead of <avr/cpp/atmega644/AnalogComparator.h>"
#else

#ifndef __AVR_CPP_XXX_ANALOG_COMPARATOR_H__
#define __AVR_CPP_XXX_ANALOG_COMPARATOR_H__
#else
#error "Attempt to include more than one <avr/cpp/XXX/AnalogComparator.h> file"
#endif

/**********************************************************************************************************************\

	atmega644 analog comparator declarations.
	This file is generated. Do not modify manually !!!

\**********************************************************************************************************************/

#define ANALOG_COMPARATOR0_CMP_ns		AnalogComparator
#define ANALOG_COMPARATOR0_CMP_struct	AnalogComparator::AnalogComparator0::ComparatorInterrupt


namespace AVRCpp
{		
	namespace AnalogComparator
	{	
		struct AnalogComparator0 : Internal::DigitalDisableAnalogComparatorBase <
				_ACSR,					/* ControlRegister */
				Bits<_ADCSRB, _ACME>,	/* MultiplexerEnableBit */ 			
				Bits<_DIDR1, _AIN0D>,	/* DigitalDisablePositivePin */
				Bits<_DIDR1, _AIN1D> >	/* DigitalDisableNegativePin */
		{
			struct CompareInterrupt : BasicInterrupt<Bits<_ACSR, _ACIE> > { __INTERRUPT_HANDLER_SUPPORT__ };
		}; // struct AnalogComparator0

	} // namespace AnalogComparator

} // namespace AVRCpp

/**********************************************************************************************************************/

#endif // ifndef __AVR_CPP_ANALOG_COMPARATOR_H__
#endif // ifndef __AVR_CPP_ATMEGA644_ANALOG_COMPARATOR_H__

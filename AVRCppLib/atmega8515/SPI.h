/**********************************************************************************************************************\

	C++ library for Atmel AVR microcontrollers
	Copyright (C) 2007 Lauri Kirikal, Mikk Leini, Rasmus Raag, MTU TTU Robotiklubi

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

#ifndef __AVR_CPP_ATMEGA8515_SPI_H__
#define __AVR_CPP_ATMEGA8515_SPI_H__

#if defined(__DOXYGEN__) && !defined(__AVR_CPP_SPI_H__)
#include <avr/cpp/SPI.h>
#endif

#ifndef __AVR_CPP_SPI_H__
#error "Include <avr/cpp/SPI.h> instead of <avr/cpp/atmega8515/SPI.h>"
#else

/**********************************************************************************************************************\

	atmega8515 serial peripheral interace (SPI) declarations.
	This file is generated. Do not modify manually !!!

\**********************************************************************************************************************/



#define SPI_STC_ns		SPI
#define SPI_STC_struct	SPI::SPI0::TransmissionCompleteInterrupt

namespace AVRCpp
{
	namespace SPI
	{
		// Functions from this namespace should not be called by user
		namespace Internal
		{
			bool &Cancel();

		}; // namespace Internal

		struct SPI0 : Internal::SPIBase <
				_SPCR,							/* ControlRegister */
				_SPSR,							/* StatusRegister */
				_SPDR,							/* DataRegister */				
				Pin4<PortB>,		/* SlaveSelectPin */	
				Pin7<PortB>,		/* ClockPin */
				Pin5<PortB>,		/* MasterOutPin */
				Pin6<PortB> >		/* MasterInPin */			
		{
			struct TransmissionCompleteInterrupt : BasicInterrupt<Bits<_SPCR, _SPIE> > { __INTERRUPT_HANDLER_SUPPORT__ };

		}; // struct SPI0

	} // namespace SPI

} // namespace AVRCpp

/**********************************************************************************************************************/

#endif // ifndef __AVR_CPP_IO_H__
#endif // ifndef __AVR_CPP_ATMEGA8515_IO_H__

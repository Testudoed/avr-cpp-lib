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

#include "USART.h"

namespace AVRCpp
{
	namespace USART
	{
#if defined(__AVR_ATmega8__) || defined(__AVR_ATmega8515__) || defined(__AVR_ATmega644__) \
		|| defined(__AVR_ATmega48__) || defined(__AVR_ATmega88__) || defined(__AVR_ATmega168__)
		
		static bool cancel0 = false;
		
		namespace Internal
		{
			bool &Cancel0() { return cancel0; }
			
		} // namespace Internal
		
#elif defined(__AVR_ATmega128__) || defined(__AVR_ATmega64__) \
		|| defined(__AVR_ATmega164P__) || defined(__AVR_ATmega324P__) || defined(__AVR_ATmega644P__)
		
		static bool cancel0 = false;
		static bool cancel1 = false;
		
		namespace Internal
		{
			bool &Cancel0() { return cancel0; }
			bool &Cancel1() { return cancel1; }
			
		} // namespace Internal
		
#elif defined(__AVR_AT90USB1287__)
		
		static bool cancel1 = false;
		
		namespace Internal
		{
			bool &Cancel1() { return cancel1; }
			
		} // namespace Internal
		
#endif // if defined(__AVR_AT90USB1287__)
		
	} // namespace USART
	
} // namespace AVRCpp

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

#include "EEPROM.h"

namespace AVRCpp
{
	namespace EEPROM
	{
		bool MoveNext()
		{
			if (EEAR < EEPROM_SIZE - 1)
			{
				WaitWhileWriting();
				EEAR++;
				
				return true;
			}
			
			return false;
			
		} // MoveNext
		
		namespace Internal
		{
			
			bool SavingProcess(uint16_t lastByteAddr, uint8_t *uint8Data)
			{
				// Enough space ?
				if (lastByteAddr >= EEPROM_SIZE)
					return false;
				
				// Write byte by byte
				while (true)
				{
					Write(*uint8Data);
					
					uint8Data++;
					
					// Done ?
					if (EEAR == lastByteAddr)
						break;
					
					WaitWhileWriting();
					EEAR++;
				}
				
				MoveNext();
				
				return true;
				
			} // SaveingProcess
			
			
			bool LoadingProcess(uint16_t lastByteAddr, uint8_t *uint8Data)
			{
				// Enough space ?
				if (lastByteAddr >= EEPROM_SIZE)
					return false;
				
				// Read byte by byte
				while (true)
				{
					*uint8Data = Read();
					
					uint8Data++;
					
					// Done ?
					if (EEAR == lastByteAddr)
						break;
					
					WaitWhileWriting();
					EEAR++;
				}
				
				MoveNext();
				
				return true;
				
			} // LoadingProcess
			
		} // namespace Internal
		
	} // namespace EEPROM

} // namespace AVRCpp

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
#include "Interrupt.h"


namespace AVRCpp
{
	namespace EEPROM
	{

		// Waits while CPU is programming Flash. Only needed if boot loader is present.
		static inline bool IsWritingFlash() {	return IsBitsSet<__SPMCSR__>(__SPMEN__); }
		static inline void WaitWhileWritingFlash() { while (IsWritingFlash() ); }
		
		static void WriteOperation()
		{
			// Save SREG_I bit.
			uint8_t savedSREG = SREG;
			
			// __EEPE__ must be set immediately after setting __EEMPE__
			GlobalInterrupts::Disable();
			
			SetBits<_EECR>(__EEMPE__);
			SetBits<_EECR>(__EEPE__);
			
			// Restore the interrupt bit in Status Register
			SREG = savedSREG;

		} // WriteOperation
		
		
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
		
		
		void Write(uint8_t data)
		{
			WaitWhileWriting();
			
			WaitWhileWritingFlash();
			
			EEDR = data;
			
			WriteOperation();

		} // Write 1
		
		
		void Write(uint16_t address, uint8_t data)
		{
			WaitWhileWriting();
			
			WaitWhileWritingFlash();
			
			SetAddressUnsafe(address);
			
			EEDR = data;
			
			WriteOperation();

		} // Write 2

		
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
				
			} // SavingProcess
			
			
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
		
#if __EEPROM_PROPERTIES & __EEPROM_ERASEBLE__
		
		static inline bool ErasingProcess(uint16_t lastByteAddr)
		{
			// Enough space ?
			if (lastByteAddr >= EEPROM_SIZE)
				return false;
			
			// Erase byte by byte
			while (true)
			{
				
				WaitWhileWriting();
				
				WaitWhileWritingFlash();
				
				WriteOperation();
				
				// Done ?
				if (EEAR == lastByteAddr)
					break;
				
				WaitWhileWriting();
				
				EEAR++;
			}
			
			MoveNext();
			
			return true;
			
		} // ErasingProcess
		

		static inline bool EraseOperation(uint16_t numberOfBytes)
		{
			// Is there anything to read or write ?
			if (numberOfBytes == 0)
				return true;
			
			{
				uint16_t lastByteAddr = EEAR + numberOfBytes - 1;
				
				return ErasingProcess(lastByteAddr);
			}
			
		} // EraseOperation
		

		bool Erase(uint16_t numberOfBytes)
		{
			uint8_t savedEECR = EECR;
			
			SetMode(EraseOnly);
			
			{
				bool result = Internal::EraseOperation(numberOfBytes);
				
				ChangeBits<_EECR>(_EEPM1 | _EEPM0, savedEECR);
				
				return result;
			}

		} // Erase
		
#endif // if __EEPROM_PROPERTIES & __EEPROM_ERASEBLE__
		
	} // namespace EEPROM

} // namespace AVRCpp

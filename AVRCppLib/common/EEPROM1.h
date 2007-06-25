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

#if !(defined(__AVR_ATmega8__) || defined(__AVR_ATmega128__) || defined(__AVR_ATmega64__) || defined(__AVR_ATmega8515__))
#error "Device is not selected or selected device is not supported."
#define __AVR_CPP_EEPROM1_H__
#endif

#ifndef __AVR_CPP_EEPROM1_H__
#define __AVR_CPP_EEPROM1_H__

/* This file should only be included from <avr/cpp/EEPROM.h>, never directly. */

#ifdef __DOXYGEN__
#ifndef __AVR_CPP_EEPROM_H__
#include <avr/cpp/EEPROM.h>
#else
#error "Include <avr/cpp/EEPROM.h> instead of <avr/cpp/common/EEPROM1.h>."
#endif
#endif


/**********************************************************************************************************************\

	EEPROM read/write functionality

\**********************************************************************************************************************/

namespace AVRCpp
{
	namespace EEPROM
	{
		__DECLARE_BASIC_INTERRUPT__(Ready, Bits<_EECR, _EERIE>);
		
		inline bool IsWriting() {	return IsBitsSet<_EECR>(_EEWE); }
		inline void WaitWhileWriting() { while (IsWriting() ); }
		
		/// Waits for previous writing operation to finish, then sets address
		inline void SetAddress(uint16_t address) { WaitWhileWriting(); EEAR = address; }
		/// Tries to set address even if previous writing operation has not finished
		inline void SetAddressUnsafe(uint16_t address) { EEAR = address; }
		inline uint16_t GetAddress() {	return EEAR; }
		
		/// Write data to previously set address. It must not be interrupted by another request to read or write
		inline void Write(uint8_t data)
		{
			WaitWhileWriting();
			
			Internal::WaitWhileWritingFlash();
			
			EEDR = data;
			
			uint8_t savedSREG = SREG;
			
			// EEWE must be set immediately after setting EEMWE
			GlobalInterrupts::Disable();
			
			SetBits<_EECR>(_EEMWE);
			SetBits<_EECR>(_EEWE);
			
			// Restore the interrupt bit in Status Register
			SREG = savedSREG;			
		} // Write 1
		
		/// Write data to given address. It must not be interrupted by another request to read or write
		inline void Write(uint16_t address, uint8_t data)
		{
			WaitWhileWriting();
			
			Internal::WaitWhileWritingFlash();
			
			SetAddressUnsafe(address);
			
			EEDR = data;
			
			uint8_t savedSREG = SREG;
			
			// EEWE must be set immediately after setting EEMWE
			GlobalInterrupts::Disable();
			
			SetBits<_EECR>(_EEMWE);
			SetBits<_EECR>(_EEWE);
			
			// Restore the interrupt bit in Status Register
			SREG = savedSREG;						
		} // Write 2
		
		/// Read from previously set address
		inline uint8_t Read()
		{
			WaitWhileWriting();
			
			SetBits<_EECR>(_EERE);
			
			return EEDR;
			
		} // Read 1
		
		/// Read from given address
		inline uint8_t Read(uint16_t address)
		{
			SetAddress(address);
			
			SetBits<_EECR>(_EERE);
			
			return EEDR;
			
		} // Read 2
		
		
	} // namespace EEPROM
	
} // namespace AVRCpp


/**********************************************************************************************************************/

#endif // ifndef __AVR_CPP_EEPROM1_H__

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

#if !(defined(__AVR_ATmega8__) || defined(__AVR_ATmega128__))
#error "Device is not selected or selected device is not supported."
#define __AVR_CPP_EEPROM_H__
#endif

#ifndef __AVR_CPP_EEPROM_H__
#define __AVR_CPP_EEPROM_H__

#include "IO.h"

#define EEPROM_SIZE E2END


#ifndef EXCLUDE_INTERRUPT_HANDLERS

#include "Interrupt.h"

#define EE_RDY_ns		EEPROM
#define EE_RDY_struct	EE_RDY_ns::ReadyInterrupt

#endif // ifndef EXCLUDE_INTERRUPT_HANDLERS


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
		
		inline void SetAddress(uint16_t address) { WaitWhileWriting(); EEAR = address; }
		inline uint16_t GetAddress() {	return EEAR; }
		
		
		inline void Write(uint8_t data)
		{
			WaitWhileWriting();
			
			EEDR = data;
			
			SetBits<_EECR>(_EEMWE);
			SetBits<_EECR>(_EEWE);
			
		} // Write 1
		
		
		inline void Write(uint16_t address, uint8_t data)
		{
			SetAddress(address);
			
			EEDR = data;
			
			SetBits<_EECR>(_EEMWE);
			SetBits<_EECR>(_EEWE);
			
		} // Write 2
		
		
		inline uint8_t Read()
		{
			WaitWhileWriting();
			
			SetBits<_EECR>(_EERE);
			
			return EEDR;
			
		} // Read 1
		
		
		inline uint8_t Read(uint16_t address)
		{
			SetAddress(address);
			
			SetBits<_EECR>(_EERE);
			
			return EEDR;
			
		} // Read 2
		
		
		bool MoveNext();
		
#ifndef EXCLUDE_EEPROM_LOAD_SAVE
		
		// Functions from this namespace should not be called by user
		namespace Internal
		{
			bool LoadingProcess(uint16_t lastByteAddr, uint8_t *uint8Data);
			bool SavingProcess(uint16_t lastByteAddr, uint8_t *uint8Data);
			
			template<typename T> static inline bool LoadSaveOperation(bool isSaving, T *data, uint16_t elementsCount)
			{
				// Is there anything to read or write ?
				if (sizeof(T) == 0 || elementsCount == 0)
					return true;
				
				{
					uint16_t lastByteAddr = EEAR + sizeof(T) * elementsCount - 1;
					uint8_t *uint8Data = (uint8_t *)(data);
					
					if (isSaving)
						return SavingProcess(lastByteAddr, uint8Data);
					else
						return LoadingProcess(lastByteAddr, uint8Data);
				}
				
			} // LoadSaveOperation
			
		} // namespace Internal
		
		template <typename T> bool Save(T data) { return Internal::LoadSaveOperation<T>(true, &data, 1);	}
		template <typename T> bool Load(T &data) { return Internal::LoadSaveOperation<T>(false, &data, 1); }
		
		template <typename T> bool SaveArray(T *data, uint16_t elementsCount) { return Internal::LoadSaveOperation<T>(true, data, elementsCount); }
		template <typename T> bool LoadArray(T *data, uint16_t elementsCount) { return Internal::LoadSaveOperation<T>(false, data, elementsCount); }
		
#endif // ifndef EXCLUDE_EEPROM_LOAD_SAVE
		
	} // namespace EEPROM
	
} // namespace AVRCpp


/**********************************************************************************************************************/

#endif // ifndef __AVR_CPP_EEPROM_H__

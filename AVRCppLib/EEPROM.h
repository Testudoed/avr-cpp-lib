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

#if (defined(__AVR_ATmega8__) || defined(__AVR_ATmega128__) || defined(__AVR_ATmega64__) || defined(__AVR_ATmega8515__))
#define __EEPROM_TYPE1__
#elif (defined(__AVR_ATmega88__) || defined(__AVR_AT90USB1287__))
#define __EEPROM_TYPE2__
#endif


#if !(defined(__EEPROM_TYPE1__) || defined(__EEPROM_TYPE2__))
#error "Device is not selected or selected device is not supported."
#define __AVR_CPP_EEPROM_H__
#endif

#ifndef __AVR_CPP_EEPROM_H__
#define __AVR_CPP_EEPROM_H__

#include "IO.h"
#include "Interrupt.h"

#ifndef _SPMEN
#define _SPMEN _SELFPRGEN
#endif

#if (defined(__AVR_ATmega8__) || defined(__AVR_ATmega8515__))
#define _SPMCSR	_SPMCR	// A hack for atmega8 and atmega8515
#endif

#define EEPROM_SIZE E2END


#ifndef EXCLUDE_INTERRUPT_HANDLERS

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
		bool MoveNext();
		
		namespace Internal
		{
			// Waits while CPU is programming Flash. Only needed if boot loader is present.
			inline bool IsWritingFlash() {	return IsBitsSet<_SPMCSR>(_SPMEN); }
			inline void WaitWhileWritingFlash() { while (IsWritingFlash() ); }
			
		} // namespace Internal
		
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
			
		/// Save array of data to previously set address. After this operation, the address points to the first byte after the written array
		template <typename T> bool SaveArray(T *data, uint16_t elementsCount) { return Internal::LoadSaveOperation<T>(true, data, elementsCount); }
		/// Load array of data from previously set address. After this operation, the address points to the first byte after the written array
		template <typename T> bool LoadArray(T *data, uint16_t elementsCount) { return Internal::LoadSaveOperation<T>(false, data, elementsCount); }
		
#endif // ifndef EXCLUDE_EEPROM_LOAD_SAVE
		
	} // namespace EEPROM
	
} // namespace AVRCpp

/**********************************************************************************************************************/


#if defined(__EEPROM_TYPE1__)
#include "common/EEPROM1.h"
#elif defined(__EEPROM_TYPE2__)
#include "common/EEPROM2.h"
#else
#error "Device is not selected or selected device is not supported."
#endif

#endif // ifndef __AVR_CPP_EEPROM_H__

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

#ifndef __AVR_CPP_SPI_H__
#define __AVR_CPP_SPI_H__

#include "IO.h"
#include "Interrupt.h"


/**********************************************************************************************************************\

	AVRCppLib Serial Peripheral Interface
	
	Most AVR microcontroller have integrated SPI.
	Most controllers with SPI have same functionality, register and bit names.	

\**********************************************************************************************************************/

namespace AVRCpp
{
	namespace SPI
	{
		enum OperationMode
		{
			SlaveMode   = 0x00,
			MasterMode  = _MSTR

		}; // enum Operation mode
		
		enum DataOrder
		{
			MSBFirst	= 0x00,
			LSBFirst	= _DORD

		}; // enum DataOrder
		
		enum ClockPolarity
		{
			ClockOnHigh = 0x00,
			ClockOnLow  = _CPOL

		}; // enum ClockPolarity
		
		enum ClockPhase
		{
			SampleOnLeadingEdge	= 0x00,
			SampleOnFallingEdge = _CPHA

		}; // enum ClockPhase
		
		enum ClockRate
		{
			RateDiv2	= 0x80,
			RateDiv4	= 0x00,
			RateDiv8	= 0x81,
			RateDiv16	= 0x01,
			RateDiv32	= 0x82,
			RateDiv64	= 0x02,
			RateDiv128	= 0x03,

		}; // enum ClockRate
		
		namespace Internal
		{
			enum BitFlags
			{
				TransferEnableFlag		= _SPE,
				DataOrderFlag			= _DORD,
				MasterSlaveSelectFlag	= _MSTR,
				ClockPolarityFlag		= _CPOL,
				ClockPhaseFlag			= _CPHA,
				ClockRateFlag			= _SPR1 | _SPR0,
				TransferCompleteFlag	= _SPIF,
				CollisionFlag			= _WCOL,
				DoubleSpeedFlag			= _SPI2X
				
			}; // enum BitFlags
			
			/**
			 *  SPI base structure
			 */
			template <
						class ControlRegister,
						class StatusRegister,
						class DataRegister,
						class SlaveSelectPin,
						class ClockPin,
						class MasterOutPin,
						class MasterInPin >
						
			struct SPIBase
			{
				private:
				
					typedef Bits<ControlRegister, TransferEnableFlag>		TransferEnableBit;
					typedef Bits<ControlRegister, DataOrderFlag>			DataOrderBit;
					typedef Bits<ControlRegister, MasterSlaveSelectFlag>	MasterSlaveSelectBit;
					typedef Bits<ControlRegister, ClockPolarityFlag>		ClockPolarityBit;
					typedef Bits<ControlRegister, ClockPhaseFlag>			ClockPhaseBit;
					typedef Bits<ControlRegister, ClockRateFlag>			ClockRateBits;					
					typedef Bits<StatusRegister, TransferCompleteFlag>		TransferCompleteBit;
					typedef Bits<StatusRegister, CollisionFlag>				CollisionBit;
					typedef Bits<StatusRegister, DoubleSpeedFlag>			DoubleSpeedBit;
					
				protected:					
					
					static inline bool volatile IsTransferCompleted() { return TransferCompleteBit::IsSet(); }
					static inline bool WasWriteCollision() { return CollisionBit::IsSet(); }					
					static inline void WaitUntilTransferCompleted() { while (!IsTransferCompleted() && !WasWriteCollision()); }								
					
				public:
					
					/**
					 *  Setup SPI
					 */
					static inline void Setup (
						OperationMode mode,
						ClockRate rate,
						DataOrder order,
						ClockPolarity polarity,
						ClockPhase phase )
					{
					
						// Set pins										
						if (mode == MasterMode)				
						{
							SlaveSelectPin::Output::InitOutput();								
							ClockPin::Output::InitOutput();
							MasterOutPin::Output::InitOutput();
							MasterInPin::Input::InitInput();						
						}
						else
						{
							SlaveSelectPin::Input::InitInput();
							ClockPin::Input::InitInput();
							MasterOutPin::Input::InitInput();
							MasterInPin::Output::InitOutput();						
						}
														
						// Setup SPI																		
						ControlRegister::Set(_SPE | order | mode | polarity | phase | (rate & ClockRateFlag));

						// Set double rate
						if (rate & 0x80)
							DoubleSpeedBit::Set();
						else
							DoubleSpeedBit::Clear();							

					} // Setup
					
					
					/**
					 *  Starts transmiting by setting SPI to master mode and selecting slave device
					 *	\attention Only possible in master-mode
					 */
					static inline void StartTransmission()
					{						
						SlaveSelectPin::Output::Clear();

					} // StartTransmission
					
					
					/**
					 *  Ends transmitting by de-selecting slave device
					 *	\attention Only possible in master-mode
					 */
					static inline void EndTransmission()
					{
						SlaveSelectPin::Output::Set();

					} // EndTransmission
					
					
					/**
					 *	Writes byte to SPI
					 *	@param data data byte to transmit
					 *	@return true on success, false on collision
					 */
					static inline bool Write(uint8_t data)
					{					
						DataRegister::Set(data);
						
						WaitUntilTransferCompleted();
						
						return !WasWriteCollision();

					} // Write
					
					
					/**
					 *	Reads byte from SPI
					 *	@return data byte read
					 */
					static inline uint8_t Read()
					{
						WaitUntilTransferCompleted();

						return DataRegister::Get();

					} // Read
					
					
					/**
					 *	Reads byte from SPI by generating clock signal with empty output
					 *	@param[out] byte read byte is stored to this variable
					 *	@return true on success, false on collision
					 */
					static inline bool ReadByClock(uint8_t &byte)
					{
						if (!Write(0) ) return false;

						byte = Read();

						return true;

					} // ReadByClock

	   		}; // template struct SPIBase
   			
		}; // namespace Internal

	} // namespace SPI
		
}; // namespace AVRCpp


/**********************************************************************************************************************/

/* Include controller specific files */
#if defined(__AVR_AT90USB1287__)
#include "at90usb1287/SPI.h"
#elif defined(__AVR_ATmega128__)
#include "atmega128/SPI.h"
#elif defined(__AVR_ATmega164P__)
#include "atmega164p/SPI.h"
#elif defined(__AVR_ATmega168__)
#include "atmega168/SPI.h"
#elif defined(__AVR_ATmega324P__)
#include "atmega324p/SPI.h"
#elif defined(__AVR_ATmega48__)
#include "atmega48/SPI.h"
#elif defined(__AVR_ATmega64__)
#include "atmega64/SPI.h"
#elif defined(__AVR_ATmega644__)
#include "atmega644/SPI.h"
#elif defined(__AVR_ATmega644P__)
#include "atmega644p/SPI.h"
#elif defined(__AVR_ATmega8__)
#include "atmega8/SPI.h"
#elif defined(__AVR_ATmega8515__)
#include "atmega8515/SPI.h"
#elif defined(__AVR_ATmega88__)
#include "atmega88/SPI.h"
#else
#error "Selected device does not have SPI support."
#endif

#endif // ifndef __AVR_CPP_SPI_H__

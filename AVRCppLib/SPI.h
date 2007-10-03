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
	Therefore whole SPI code is in this one file.

\**********************************************************************************************************************/

namespace AVRCpp
{
	namespace SPI
	{
		enum OperationMode
		{
			SlaveMode   = 0x00,
			MasterMode  = _MSTR

		}; // Operation mode

		enum DataOrder
		{
			MSBFirst	= 0x00,
			LSBFirst	= _DORD

		}; // DataOrder

		enum ClockPolarity
		{
			ClockOnHigh = 0x00,
			ClockOnLow  = _CPOL

		}; // ClockPolarity

		enum ClockPhase
		{
			SampleOnLeadingEdge	= 0x00,
			SampleOnFallingEdge = _CPHA

		}; // ClockPhase

		enum ClockRate
		{
			RateDiv2	= 0x80,
			RateDiv4	= 0x00,
			RateDiv8	= 0x81,
			RateDiv16	= 0x01,
			RateDiv32	= 0x82,
			RateDiv64	= 0x02,
			RateDiv128	= 0x03,

		}; // ClockRate
		
		namespace Internal
		{
			enum BitFlags
			{
				TransferEnableFlag		= 0x40,
				DataOrderFlag			= 0x20,
				MasterSlaveSelectFlag	= 0x10,
				ClockPolarityFlag		= 0x08,
				ClockPhaseFlag			= 0x04,
				ClockRateFlag			= 0x03,

				TransferCompleteFlag	= 0x80,
				CollisionFlag			= 0x40,
				DoubleSpeedFlag			= 0x01
				
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

			struct SPIbase
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
					typedef DataRegister	Data;

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
						SlaveSelectPin::Output::InitOutput();
						
						if (mode == MasterMode)												
							ClockPin::Output::InitOutput();
						else
							ClockPin::Input::InitInput();
						
						MasterOutPin::Output::InitOutput();
						MasterInPin::Input::InitInput();						
														
						// Setup SPI																		
						ControlRegister::Set(_SPE | order | mode | polarity | phase | (rate & ClockRateFlag));

						// Set double rate
						if (rate & 0x80)
							DoubleSpeedBit::Set();
						else
							DoubleSpeedBit::Clear();							
					}

					/**
					 *  Starts transmiting by setting SPI to master mode and selecting slave device
					 *	\attention Only possible in master-mode
					 */
					static inline void StartTransmission()
					{						
						SlaveSelectPin::Output::Clear();
					}

					/**
					 *  Ends transmitting by de-selecting slave device
					 *	\attention Only possible in master-mode
					 */
					static inline void EndTransmission()
					{
						SlaveSelectPin::Output::Set();
					}

					/**
					 *	Writes byte to SPI
					 *	@param data data byte to transmit
					 *	@return true on success
					 */
					static inline bool Write(uint8_t data)
					{					
						Data::Set(data);
						
						WaitUntilTransferCompleted();
						
						return !WasWriteCollision();
					}

					/**
					 *	Reads byte from SPI
					 *	@return data byte read
					 */
					static inline uint8_t Read()
					{
						WaitUntilTransferCompleted();

						return Data::Get();
					}

	   		}; // struct SPIbase
   			
		}; // namespace Internal

	} // namespace SPI
		
}; // namespace AVRCpp


/**********************************************************************************************************************/

/* Devices supported and not */
#if defined(__AVR_ATmega8__)
#include "atmega8/SPI.h"
#elif defined(__AVR_ATmega128__)
#include "atmega128/SPI.h"
#else
#error "Device is not selected or selected device is not supported."
#endif

#endif // ifndef __AVR_CPP_SPI_H__

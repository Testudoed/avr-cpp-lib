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
#include "Transceiver.h"


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
			SlaveMode  = 0x00,
			MasterMode = _MSTR

		}; // enum Operation mode
		
		enum DataOrder
		{
			MSBFirst = 0x00,
			LSBFirst = _DORD

		}; // enum DataOrder
		
		enum ClockPolarity
		{
			ClockOnHigh = 0x00,
			ClockOnLow  = _CPOL

		}; // enum ClockPolarity
		
		enum ClockPhase
		{
			SampleOnLeadingEdge = 0x00,
			SampleOnFallingEdge = _CPHA

		}; // enum ClockPhase
		
		enum ClockRate
		{
			RateDiv2    = 0x80,
			RateDiv4    = 0x00,
			RateDiv8    = 0x81,
			RateDiv16   = 0x01,
			RateDiv32   = 0x82,
			RateDiv64   = 0x02,
			RateDiv128  = 0x03,

		}; // enum ClockRate
		
		namespace Internal
		{
			enum BitFlags
			{
				TransferEnableFlag      = _SPE,
				DataOrderFlag           = _DORD,
				MasterSlaveSelectFlag   = _MSTR,
				ClockPolarityFlag       = _CPOL,
				ClockPhaseFlag          = _CPHA,
				ClockRateFlag           = _SPR1 | _SPR0,
				TransferCompleteFlag    = _SPIF,
				CollisionFlag           = _WCOL,
				DoubleSpeedFlag         = _SPI2X
				
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
						
			struct SPIBase : public Transceiver< SPIBase< 
					ControlRegister,
					StatusRegister,
					DataRegister,
					SlaveSelectPin,
					ClockPin,
					MasterOutPin,
					MasterInPin > >
			{
			
				friend class Transceiver< SPIBase< 
						ControlRegister,
						StatusRegister,
						DataRegister,
						SlaveSelectPin,
						ClockPin,
						MasterOutPin,
						MasterInPin > >;
						
				typedef Transceiver< SPIBase< 
						ControlRegister,
						StatusRegister,
						DataRegister,
						SlaveSelectPin,
						ClockPin,
						MasterOutPin,
						MasterInPin > > Parent;
			
				private:
				
					typedef Bits<ControlRegister, TransferEnableFlag>      TransferEnableBit;
					typedef Bits<ControlRegister, DataOrderFlag>           DataOrderBit;
					typedef Bits<ControlRegister, MasterSlaveSelectFlag>   MasterSlaveSelectBit;
					typedef Bits<ControlRegister, ClockPolarityFlag>       ClockPolarityBit;
					typedef Bits<ControlRegister, ClockPhaseFlag>          ClockPhaseBit;
					typedef Bits<ControlRegister, ClockRateFlag>           ClockRateBits;					
					typedef Bits<StatusRegister, TransferCompleteFlag>     TransferCompleteBit;
					typedef Bits<StatusRegister, CollisionFlag>            CollisionBit;
					typedef Bits<StatusRegister, DoubleSpeedFlag>          DoubleSpeedBit;
					
				protected:					
										
					static inline bool volatile IsTransferCompleted() { return TransferCompleteBit::IsSet();                  }
					static inline bool WasWriteCollision()            { return CollisionBit::IsSet();                         }
					static inline void WaitUntilTransferCompleted()   { while (IsTransferCompleted() || WasWriteCollision()); }
					
					
/**********************************************************************************************************************/										
				
				/**
				 * Neccessary functions for transceiver
				 */
					
				public:
					 
					static inline bool CanTransmit()         { return IsTransferCompleted(); }
					static inline bool CanReceive()          { return IsTransferCompleted(); }
					static inline bool IsTransmitCompleted() { return IsTransferCompleted(); }
					static inline bool IsReceiveCompleted()  { return IsTransferCompleted(); }
					static inline bool WasTransmitError()    { return WasWriteCollision();   }
					static inline bool WasReceiveError()     { return WasWriteCollision();   }				
					
				protected:
					
					static inline void PureByteTransmit(const uint8_t &data)
					{					
						DataRegister::Set(data);
						
					} // PureByteTransmit
					
					static inline void PureByteReceive(uint8_t &data)
					{
						// In master mode, it is neccessary to generate clock with random data to read data from slave
						if (MasterSlaveSelectBit::IsSet())
						{	
							PureByteTransmit(0);							
							
							if (!Parent::WaitUntilTransmitCompleted())
								return;							
						}
	
						data = DataRegister::Get();
												
					} // PureByteReceive
					

/**********************************************************************************************************************/					
					
				public:
					
					/**
					 * Setup SPI in master mode
					 */
					static inline void SetupMaster (
							ClockRate rate,
							DataOrder order,
							ClockPolarity polarity,
							ClockPhase phase )
					{					
						// Set pins										
						SlaveSelectPin::Output::InitOutput();								
						ClockPin::Output::InitOutput();
						MasterOutPin::Output::InitOutput();
						MasterInPin::Input::InitInput();												
														
						// Setup SPI																		
						ControlRegister::Set(_SPE | order | _MSTR | polarity | phase | (rate & ClockRateFlag));

						// Set double rate
						if (rate & 0x80)
							DoubleSpeedBit::Set();
						else
							DoubleSpeedBit::Clear();							

					} // SetupMaster
					
					/**
					 * Setup SPI in slave mode
					 */
					static inline void SetupSlave (						
						DataOrder order,
						ClockPolarity polarity,
						ClockPhase phase )
					{
						// Set pins										
						SlaveSelectPin::Input::InitInput();
						ClockPin::Input::InitInput();
						MasterOutPin::Input::InitInput();
						MasterInPin::Output::InitOutput();
														
						// Setup SPI																		
						ControlRegister::Set(_SPE | order | polarity | phase);					

					} // SetupSlave
										
					/**
					 * Starts transmiting by setting SPI to master mode and selecting slave device
					 * \attention Only possible in master-mode
					 */
					static inline void StartTransmission()
					{						
						SlaveSelectPin::Output::Clear();

					} // StartTransmission
					
					/**
					 * Ends transmitting by de-selecting slave device
					 * \attention Only possible in master-mode
					 */
					static inline void EndTransmission()
					{
						SlaveSelectPin::Output::Set();

					} // EndTransmission
															
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

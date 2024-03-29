/**********************************************************************************************************************\

	C++ library for Atmel AVR microcontrollers
	Copyright (C) 2007 Lauri Kirikal, Mikk Leini, Rasmus Raag, MT� TT� Robotiklubi

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

	MT� TT� Robotiklubi  http://www.robotiklubi.ee robotiklubi@gmail.com
	Lauri Kirikal        laurikirikal@gmail.com
	Mikk Leini           mikk.leini@gmail.com
	Rasmus Raag          rasmus_raag@users.sourceforge.net

\**********************************************************************************************************************/

#ifndef __AVR_CPP_USART_H__
#define __AVR_CPP_USART_H__

#include "IO.h"
#include "Interrupt.h"
#include "Transceiver.h"

#define AsyncNormBaudCalc(BaudRate)		F_CPU / 16 / BaudRate - 1
#define AsyncDblBaudCalc(BaudRate)		F_CPU / 8 / BaudRate - 1
#define SyncMasterBaudCalc(BaudRate)	F_CPU / 2 / BaudRate - 1

namespace AVRCpp
{
	namespace USART
	{
		enum ReadResult
		{
			Success,
			Canceled,
			DataOverRun,
			FrameError,
			ParityError
			
		}; // enum ReadResult
		
		enum Receiver
		{
			ReceiverEnable	= 0x10,
			ReceiverDisable	= 0x00
			
		}; // enum Receiver
		
		enum Transmitter
		{
			TransmitterEnable	= 0x08,
			TransmitterDisable	= 0x00
			
		}; // enum Transmitter
		
		enum ParityCheck
		{
			NoParityCheck	= 0x00,
			EvenParity		= 0x20,
			OddParity		= 0x30
			
		}; // enum ParityCheck
		
		enum StopBit
		{
			NormalStopBit	= 0x00,
			DoubledStopBit	= 0x08
			
		}; // enum StopBit
		
		enum CharacterSize 
		{
			CharacterSize5	= 0x00,
			CharacterSize6	= 0x02,
			CharacterSize7	= 0x04,
			CharacterSize8	= 0x06,
			CharacterSize9	= 0xF6
			
		}; // enum DataBitsCount
		
		enum SynchroEdge
		{
			ReceiveOnFall = 0x00,
			ReceiveOnRise = 0x01
			
		}; // enum SynchroEdge
		
		enum Speed
		{
			NormalSpeed	= 0x00,
			DoubleSpeed	= 0x02
			
		}; // enum Speed
		
		enum CommunicationMode
		{
			SingleProcessor	= 0x00,
			MultiProcessor	= 0x01
			
		}; // enum CommunicationMode
		
		
		namespace Internal
		{
			enum CharacterSizeConstants
			{
				CharacterSizeMaskC		= 0x06,
				CharacterSizeCheckMaskB	= 0xF0,
				CharacterSize9FlagB		= 0x04, 
				
			}; // enum CharacterSizeConstants
			
			enum RegisterSelect
			{
				NoRegisterSelect	= 0x00,
				HasRegisterSelect	= 0x80
				
			}; // enum RegisterSelect
			
			enum Mode
			{
				Asynchronous	= 0x00,
				Synchronous		= 0x40
				
			}; // enum Mode
			
			enum BitFlags
			{
				NinthTransmitFlag		= 0x01,
				NinthReceiveFlag		= 0x02,
				ReceiveCompleteFlag		= 0x80,
				TransferCompleteFlag	= 0x40,
				DataRegisterEmptyFlag	= 0x20,
				FrameErrorFlag			= 0x10,
				DataOverRunFlag			= 0x08,
				ParityErrorFlag			= 0x04,
				ErrorFlags				= 0x1C,
				TransmitterEnableFlag	= 0x08,
				ReceiverEnableFlag		= 0x10,
				MultiProcessorFlag		= 0x01
				
			}; // enum BitFlags
			
			template <
					class BaudRateRegisterHigh,
					class BaudRateRegisterLow,
					class ControlRegisterA,
					class ControlRegisterB,
					class ControlRegisterC,
					class DataRegister,
					class TransferClockPin,
					RegisterSelect registerSelect,
					bool & (* Cancel)() >
			
			class USARTBase : public Transceiver< USARTBase<
					BaudRateRegisterHigh,
					BaudRateRegisterLow,
					ControlRegisterA,
					ControlRegisterB,
					ControlRegisterC,
					DataRegister,
					TransferClockPin,
					registerSelect,
					Cancel > >
			{
			
				friend class Transceiver< USARTBase<
						BaudRateRegisterHigh,
						BaudRateRegisterLow,
						ControlRegisterA,
						ControlRegisterB,
						ControlRegisterC,
						DataRegister,
						TransferClockPin,
						registerSelect,
						Cancel > >;
				
				private:
					
					typedef Bits<ControlRegisterB, NinthTransmitFlag>       NinthTransmitBit;
					typedef Bits<ControlRegisterB, NinthReceiveFlag>        NinthReceiveBit;
					typedef Bits<ControlRegisterA, ReceiveCompleteFlag>     ReceiveCompleteBit;
					typedef Bits<ControlRegisterA, TransferCompleteFlag>    TransferCompleteBit;
					typedef Bits<ControlRegisterA, DataRegisterEmptyFlag>   DataRegisterEmptyBit;
					typedef Bits<ControlRegisterA, FrameErrorFlag>          FrameErrorBit;
					typedef Bits<ControlRegisterA, DataOverRunFlag>         DataOverRunBit;
					typedef Bits<ControlRegisterA, ParityErrorFlag>         ParityErrorBit;
					typedef Bits<ControlRegisterA, ErrorFlags>              ErrorBits;
					typedef Bits<ControlRegisterB, TransmitterEnableFlag>   TransmitterEnableBit;
					typedef Bits<ControlRegisterB, ReceiverEnableFlag>      ReceiverEnableBit;
					typedef Bits<ControlRegisterA, MultiProcessorFlag>      MultiProcessorBit;
					
					typedef typename TransferClockPin::Input   SlavePin;
					typedef typename TransferClockPin::Output  MasterPin;
					
				public:
					
					static inline void SetNinthBit() { NinthTransmitBit::Set(); }
					static inline void ClearNinthBit() { NinthTransmitBit::Clear(); }
					static inline bool IsNinthBitSet() { return NinthReceiveBit::IsSet(); }
					
					static inline void EnterMultiProcessorMode() { MultiProcessorBit::Set(); }
					static inline void EnterSingleProcessorMode() { MultiProcessorBit::Clear(); }
					static inline bool IsEnteredMultiProcessorMode() { return MultiProcessorBit::IsSet(); }
					
					static inline bool IsReceiveCompleted() { return ReceiveCompleteBit::IsSet(); }
					static inline bool IsTransferCompleted() { return TransferCompleteBit::IsSet(); }
					static inline bool IsDataRegisterEmpty() { return DataRegisterEmptyBit::IsSet(); }
					static inline bool IsAllDone() { return IsTransferCompleted() && IsReceiveCompleted(); }								
					static inline bool WasFrameError() { return FrameErrorBit::IsSet(); }
					static inline bool WasDataOverRun() { return DataOverRunBit::IsSet(); }
					static inline bool WasParityError() { return ParityErrorBit::IsSet(); }
					static inline bool WasError() { return ErrorBits::IsAnySet(); }
					
					static inline void WaitUntilTransferCompleted() { while (!IsTransferCompleted() ); }
					static inline void WaitUntilReceiveCompleted() { while (!IsReceiveCompleted() ); }
					static inline void WaitUntilDataRegisterEmpty() { while (!IsDataRegisterEmpty() ); }
					static inline void WaitUntilAllDone() { while (!IsAllDone() ); }
					
					static inline void EnableTransmitter() { TransmitterEnableBit::Set(); }
					static inline void DisableTransmitter() { TransmitterEnableBit::Clear(); }
					static inline bool IsTransmitterEnabled() { return TransmitterEnableBit::IsSet(); }
					
					static inline void EnableReceiver() { ReceiverEnableBit::Set(); }
					static inline void DisableReceiver() { ReceiverEnableBit::Clear(); }
					static inline bool IsReceiverEnabled() { return ReceiverEnableBit::IsSet(); }
					
					static inline void ResetCancel() { Cancel() = false; }
					static inline void CancelReading() { Cancel() = true; }
					static inline bool IsCanceled() { return Cancel(); }					
								
				
/**********************************************************************************************************************/						
					
				private:
					
					static inline bool WaitWhileReceiveCompletedOrCanceled()
					{
						ResetCancel();
						
						while (!IsReceiveCompleted() )
						{
							if (IsCanceled() )
								return true;
						}
						
						return false;
						
					} // WaitWhileReceiveCompletedOrCanceled
					
					static inline ReadResult DetailedErrorCheck()
					{
						if (WasFrameError() )
							return FrameError;
						
						if (WasParityError() )
							return ParityError;
						
						if (WasDataOverRun() )
							return DataOverRun;
						
						return Success;
						
					} // DetailedErrorCheck

/**********************************************************************************************************************\

	Status functions for Transceiver base class

\**********************************************************************************************************************/

				public:
				
					static inline bool CanTransmit()            { return DataRegisterEmptyBit::IsSet(); }
					static inline bool CanReceive()             { return ReceiveCompleteBit::IsSet();   }
					static inline bool IsTransmitterAvailable() { return TransmitterEnableBit::IsSet(); }
					static inline bool IsReceiverAvailable()    { return ReceiverEnableBit::IsSet();    }
					static inline bool IsTransmittingComplete() { return TransferCompleteBit::IsSet();  }
					static inline bool IsReceivingComplete()    { return true;                          } // No post-process
					static inline bool WasTransmittingError()   { return false;                         } // No feedback
					static inline bool WasReceivingError()      { return ErrorBits::IsAnySet();         } // All USART errors are receiving errors
					
				protected:
	
					static inline bool PureWrite(const uint8_t &data)
					{					
						DataRegister::Set(data);
						
						return true;
						
					} // PureWrite
					
					static inline bool PureRead(uint8_t &data)
					{
						data = DataRegister::Get();
						
						return true;
												
					} // PureRead
					
					
/**********************************************************************************************************************/
					
				public:
					
					/**
					 * Set baud rate
					 */
					static void SetBaudRate(uint16_t baudRate)
					{
						BaudRateRegisterHigh::Set( (baudRate >> 8) & 0x0F);
						BaudRateRegisterLow::Set( (uint8_t)baudRate);
						
					} // SetBaudRate
					
					/**
					 * Setup synchronous master mode
					 */
					static void SetupMasterSync (
							uint16_t baudRate,
							Receiver receiver,
							Transmitter transmitter,
							ParityCheck parityCheck,
							StopBit stopBit,
							CharacterSize characterSize,
							SynchroEdge synchroEdge,
							CommunicationMode communicationMode )
					{	
						uint8_t savedSREG = SREG;
						
						GlobalInterrupts::Disable();
						
						MasterPin::InitOutput();
						SetBaudRate(baudRate);
							
						ControlRegisterA::Set(communicationMode);
						ControlRegisterB::Set(receiver | transmitter | (characterSize & CharacterSizeCheckMaskB ? CharacterSize9FlagB : 0) ); 
						ControlRegisterC::Set(registerSelect | Synchronous | parityCheck | stopBit | synchroEdge | (characterSize & CharacterSizeMaskC) ); 
						
						SREG = savedSREG;
						
					} // SetupMasterSync
					
					/**
					 * Setup synchronous slave mode
					 */
					static void SetupSlaveSync (
							Receiver receiver,
							Transmitter transmitter,
							ParityCheck parityCheck,
							StopBit stopBit,
							CharacterSize characterSize,
							SynchroEdge synchroEdge,
							CommunicationMode communicationMode )
					{	
						uint8_t savedSREG = SREG;
						
						GlobalInterrupts::Disable();
						
						SlavePin::InitInput();
							
						ControlRegisterA::Set(communicationMode);
						ControlRegisterB::Set(receiver | transmitter | (characterSize & CharacterSizeCheckMaskB ? CharacterSize9FlagB : 0) ); 
						ControlRegisterC::Set(registerSelect | Synchronous | parityCheck | stopBit | synchroEdge | (characterSize & CharacterSizeMaskC) ); 
						
						SREG = savedSREG;
						
					} // SetupSlaveSync
					
					/**
					 * Setup asynchronous mode
					 */
					static void SetupAsynchronous (
							uint16_t baudRate,
							Receiver receiver,
							Transmitter transmitter,
							ParityCheck parityCheck,
							StopBit stopBit,
							CharacterSize characterSize,
							Speed speed,
							CommunicationMode communicationMode )
					{
						uint8_t savedSREG = SREG;
						
						GlobalInterrupts::Disable();
						
						SetBaudRate(baudRate);
						
						ControlRegisterA::Set(speed | communicationMode);
						ControlRegisterB::Set(receiver | transmitter | (characterSize & CharacterSizeCheckMaskB ? CharacterSize9FlagB : 0) );
						ControlRegisterC::Set(registerSelect | Asynchronous | parityCheck | stopBit | (characterSize & CharacterSizeMaskC) ); 
						
						SREG = savedSREG;
						
					} // SetupAsynchronous
					
					/**					
					 * Release USART unit
					 */
					static void Release()
					{						
						ControlRegisterA::Set(0);
						ControlRegisterB::Set(0);
						ControlRegisterC::Set(0);
						
					}  // Release
					
					/*
					static inline void WriteNinthSet(uint8_t data)
					{
						WaitUntilDataRegisterEmpty();
						
						SetNinthBit();
						
						DataRegister::Set(data);
						
					} // WriteNinthSet
					
					static inline void WriteNinthCleared(uint8_t data)
					{
						WaitUntilDataRegisterEmpty();
						
						ClearNinthBit();
						
						DataRegister::Set(data);
						
					} // WriteNinthCleared
					
					static inline void Write(const uint8_t &data)
					{
						WaitUntilDataRegisterEmpty();
						
						DataRegister::Set(data);
						
					} // Write 1
					
					static inline void Write(uint8_t data, bool ninth)
					{
						WaitUntilDataRegisterEmpty();
						
						if (ninth)
							SetNinthBit();
						else
							ClearNinthBit();
						
						DataRegister::Set(data);
						
					} // Write 2
					
					static inline bool Read(uint8_t &data)
					{
						if (WaitWhileReceiveCompletedOrCanceled() )
							return false;
						
						{
							bool result = !WasError();
							
							data = DataRegister::Get();
							
							return result;
						}
						
					} // Read 1
					
					static inline bool Read(uint8_t &data, bool &ninth)
					{
						if (WaitWhileReceiveCompletedOrCanceled() )
							return false;
						
						{
							bool result = !WasError();
							
							ninth = IsNinthBitSet();
							data = DataRegister::Get();
							
							return result;
						}
						
					} // Read 2
					
					static inline bool SimpleRead(uint8_t &data)
					{
						bool result;
						
						while (!IsReceiveCompleted() );
						result = !WasError();
						data = DataRegister::Get();
						return result;
						
					} // SimpleRead 1
					
					static inline bool SimpleRead(uint8_t &data, bool &ninth)
					{
						bool result;
						
						while (!IsReceiveCompleted() );
						result = !WasError();
						ninth = IsNinthBitSet();
						data = DataRegister::Get();
						return result;
						
					} // SimpleRead 2
					
					static inline ReadResult DetailedRead(uint8_t &data)
					{
						if (WaitWhileReceiveCompletedOrCanceled() )
							return Canceled;
						
						{
							ReadResult result = DetailedErrorCheck();
							
							data = DataRegister::Get();
							
							return result;
						}
						
					} // DetailedRead 1
					
					static inline ReadResult DetailedRead(uint8_t &data, bool &ninth)
					{
						if (WaitWhileReceiveCompletedOrCanceled() )
							return Canceled;
						
						{
							ReadResult result = DetailedErrorCheck();
							
							ninth = IsNinthBitSet();
							data = DataRegister::Get();
							
							return result;
						}
						
					} // DetailedRead 2
					
					static inline ReadResult DetailedSimpleRead(uint8_t &data)
					{
						while (!IsReceiveCompleted() );
						
						{
							ReadResult result = DetailedErrorCheck();
							
							data = DataRegister::Get();
							
							return result;
						}
						
					} // DetailedSimpleRead 1
					
					static inline ReadResult DetailedSimpleRead(uint8_t &data, bool &ninth)
					{
						while (!IsReceiveCompleted() );
						
						{
							ReadResult result = DetailedErrorCheck();
							
							ninth = IsNinthBitSet();
							data = DataRegister::Get();
							
							return result;
						}
						
					} // DetailedSimpleRead 2
					*/

					/// Empties the receive buffer, if it is not already empty.
					static inline void Flush()
					{
						// If there is data to read
						while(IsReceiveCompleted())
						{
							// Discard the contents of USART Data Register
							uint8_t data;
							data = DataRegister::Get();
						}
						
					} // Flush									
				
			}; // struct USARTBase
			
		} // namespace Internal
		
	} // namespace USART
	
} // namespace AVRCpp


/**********************************************************************************************************************/
	
#if defined(__AVR_ATmega8__)
#include "atmega8/USART.h"
#elif defined(__AVR_ATmega128__)
#include "atmega128/USART.h"
#elif defined(__AVR_ATmega64__)
#include "atmega64/USART.h"
#elif defined(__AVR_ATmega8515__)
#include "atmega8515/USART.h"
#elif defined(__AVR_AT90USB1287__)
#include "at90usb1287/USART.h"
#elif defined(__AVR_ATmega88__)
#include "atmega88/USART.h"
#elif defined(__AVR_ATmega48__)
#include "atmega48/USART.h"
#elif defined(__AVR_ATmega168__)
#include "atmega168/USART.h"
#elif defined(__AVR_ATmega164P__)
#include "atmega164p/USART.h"
#elif defined(__AVR_ATmega324P__)
#include "atmega324p/USART.h"
#elif defined(__AVR_ATmega328P__)
#include "atmega328p/USART.h"
#elif defined(__AVR_ATmega644P__)
#include "atmega644p/USART.h"
#elif defined(__AVR_ATmega644__)
#include "atmega644/USART.h"
#elif defined(__AVR_ATmega2560__)
#include "atmega2560/USART.h"
#else
#error "Device is not selected or selected device is not supported."
#endif

#endif // ifndef __AVR_CPP_USART_H__

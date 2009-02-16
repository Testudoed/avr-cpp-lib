/**********************************************************************************************************************\

	C++ library for Atmel AVR microcontrollers
	Copyright (C) 2008 Lauri Kirikal, Mikk Leini, Rasmus Raag, MTÜ TTÜ Robotiklubi

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

#ifndef __AVR_CPP_TWI_H__
#define __AVR_CPP_TWI_H__

#include "IO.h"
#include "Interrupt.h"
#include "Transceiver.h"

/* What devices support TWI ? */
#if defined(__AVR_AT90USB1287__)
#define __TWI__
#elif defined(__AVR_ATmega128__)
#define __TWI__
#elif defined(__AVR_ATmega164P__)
#define __TWI__
#elif defined(__AVR_ATmega168__)
#define __TWI__
#elif defined(__AVR_ATmega324P__)
#define __TWI__
#elif defined(__AVR_ATmega48__)
#define __TWI__
#elif defined(__AVR_ATmega64__)
#define __TWI__
#elif defined(__AVR_ATmega644__)
#define __TWI__
#elif defined(__AVR_ATmega644P__)
#define __TWI__
#elif defined(__AVR_ATmega8__)
#define __TWI__
#elif defined(__AVR_ATmega88__)
#define __TWI__
#else
#error "Selected device does not have TWI support."
#endif

#ifdef __TWI__

/**********************************************************************************************************************\

	AVRCppLib Two-Wire (I2C) Interface		

\**********************************************************************************************************************/

/*
	PRR register PRTWI bit as PowerUp and PowerDown static member functions ???
*/

/**
 * Calculation for TWI clock frequency register
 * @param bitRate	Frequency in Hz
 * @param prescale	Clock prescaler: 1, 4, 16 or 64
 * @see AVRCpp::TWI::TWIBase::SetUpMaster
 * @see AVRCpp::TWI::ClockPrescaler
 */
#define TWIClockCalculation(bitRate, prescale)       (uint8_t)((uint32_t)(F_CPU - 16 * (uint32_t)bitRate) / (uint32_t)(2  * (1 << (uint32_t)prescale) * (1 << (uint32_t)prescale) * (uint32_t)bitRate))

#define TWI_ns		TWI
#define TWI_struct	TWI::DelegateTWI0::JobCompleteInterrupt

namespace AVRCpp
{
	namespace TWI
	{
		enum ClockPrescaler
		{
			NoPrescale   = 0x00,
			Prescale4    = _TWPS0,
			Prescale16   = _TWPS1,
			Prescale64   = _TWPS1 | _TWPS0
			
		}; // enum Prescaler
		
		enum Status
		{
			// Interrupt flag is cleared (unknown state)
			JobNotComplete					= 0xF8,
			
			// Master mode statuses
			MasterStart						= 0x08,
			MasterRepeatedStart				= 0x10,
			MasterWriteModeSelected			= 0x18,
			MasterWriteModeNotSelected		= 0x20,
			MasterDataTransmitted			= 0x28,
			MasterDataNotTransmitted		= 0x30,
			MasterArbitrationLost			= 0x38,		
			MasterReadModeSelected			= 0x40,
			MasterReadModeNotSelected		= 0x48,
			MasterDataReceived				= 0x50,
			MasterLastDataReceived			= 0x58,
			
			// Slave mode statuses
			SlaveWriteModeReceived			= 0x60,
			SlaveWriteModeArbitrationLost	= 0x68,
			SlaveGeneralCallReceived		= 0x70,
			SlaveGeneralCallArbitrationLost	= 0x78,
			SlaveDataReceived				= 0x80,
			SlaveLastDataReceived			= 0x88,
			SlaveGeneralDataReceived		= 0x90,
			SlaveGeneralLastDataReceived	= 0x98,			
			SlaveFinish						= 0xA0,			
			SlaveReadModeReceived           = 0xA8,			
			SlaveReadModeArbitrationLost    = 0xB0,
			SlaveDataTransmitted            = 0xB8,
			SlaveDataNotTransmitted         = 0xC0,
			SlaveAnnoyedAfterLastData       = 0xC8			
			
		}; // enum Status
		
		enum Mode
		{
			WriteToSlave	= 0x00,
			ReadFromSlave	= 0x01
			
		}; // enum Mode
		
		enum TWIEnabled
		{
			Disabled		= 0x00,
			Enabled			= _TWEN
			
		}; // enum TWIEnabled
		
		enum AcknowledgeScheme
		{
			AcknowledgeNone = 0x00,
			AcknowledgeAll	= _TWEA
		
		}; // enum AcknowledgeScheme
		
		enum GeneralCalls
		{
			/// Non-addressed (address == 0) data is abandoned
			DiscardGeneralCalls	= 0x00,
			/// Data with no address (address == 0) is received
			GeneralCallsEnabled	= _TWGCE
			
		}; // enum GeneralCalls
		
		namespace Internal
		{
			enum BitFlags
			{
				JobCompleteFlag	        = _TWINT,
				EnableAcknowledgeFlag   = _TWEA,
				StartConditionFlag      = _TWSTA,
				StopConditionFlag       = _TWSTO,
				WriteCollisionFlag      = _TWWC,
				EnableFlag				= _TWEN,
				GeneralCallEnableFlag   = _TWGCE,
				StatusCodeFlag          = _TWS7 | _TWS6 | _TWS5 | _TWS4 | _TWS3,
				ClockPrescaleFlag       = _TWPS1 | _TWPS0,
				WriteModeFlag           = 0x00,
				ReadModeFlag            = 0x01
				
			}; // enum BitFlags
			
			/**
			 *  TWI base structure
			 */
			template <
					class BitRateRegister,
					class ControlRegister,
					class StatusRegister,
					class DataRegister,
					class SlaveAddressRegister,
					class SlaveAddressMaskRegister,
					class DefaultUserAbort >
					
			struct TWIBase : public Transceiver< TWIBase<
					BitRateRegister,
					ControlRegister,
					StatusRegister,
					DataRegister,
					SlaveAddressRegister,
					SlaveAddressMaskRegister,
					DefaultUserAbort> >
			{
			private:
				
				friend class Transceiver< TWIBase<
						BitRateRegister,
						ControlRegister,
						StatusRegister,
						DataRegister,
						SlaveAddressRegister,
						SlaveAddressMaskRegister,
						DefaultUserAbort> >;
						
				typedef Transceiver< TWIBase<
						BitRateRegister,
						ControlRegister,
						StatusRegister,
						DataRegister,
						SlaveAddressRegister,
						SlaveAddressMaskRegister,
						DefaultUserAbort> > Parent;
				
				
				/**
				 * For checking whether the interrupt flag is set.
				 */
				typedef Bits<ControlRegister, JobCompleteFlag>			JobCompleteBit;
				/**
				 * This type definition gives more appropriate name for JobCompleteBit for use in 
				 * starting a new job i.e. clearing interrrupt flag.
				 */
				typedef JobCompleteBit									NewJobBit;
				/**
				 * This bit controls the generation of the acknowledge pulse.
				 */
				typedef Bits<ControlRegister, EnableAcknowledgeFlag>	EnableAcknowledgeBit;
				/**
				 * This bit is testid after writing into DataRegister to make sure that this register was accessible
				 * while changing the content.
				 */
				typedef Bits<ControlRegister, WriteCollisionFlag>		WriteCollisionBit;
				/**
				 * For overriding general I/O operation for SDA and SCL pins.
				 */
				typedef Bits<ControlRegister, EnableFlag>				EnableBit;
				/**
				 * Start and stop bit for clearing and setting them together.
				 */
				typedef Bits<ControlRegister, StartConditionFlag | StopConditionFlag> StartAndStopBits;
				/**
				 * For activating/deactivating TWI start condition
				 */
				typedef Bits<ControlRegister, StartConditionFlag> StartBit;
				/**
				 * For activating/deactivating TWI stoip condition
				 */
				typedef Bits<ControlRegister, StopConditionFlag> StopBit;
				
				
/**********************************************************************************************************************\

	Functions for Transceiver base class

\**********************************************************************************************************************/
				
			public:
				
				
				static inline bool CanTransmit() { return true; }
				static inline bool CanReceive()  { return true; }
				
				static bool IsTransmitterAvailable()
				{						
					if (!IsJobCompleted() )
						return false;
					
					switch (GetStatus() )
					{
					case MasterWriteModeSelected:
					case MasterDataTransmitted:
					case SlaveReadModeReceived:
					case SlaveDataTransmitted:
						return true;
					default:
						return false;
					}
					
				} // IsTransmitterAvailable
				
				static bool IsReceiverAvailable()
				{
					if (!IsJobCompleted() )
						return false;
						
					switch (GetStatus() )
					{						
					case MasterReadModeSelected:
					case MasterDataReceived:
					case SlaveWriteModeReceived:
					case SlaveDataReceived:	
					case SlaveGeneralDataReceived:							
					case SlaveGeneralCallReceived:
						return true;
					default:
						return false;
					}
					
				} // IsReceiverAvailable
				
				static inline bool IsTransmitingComplete() { return IsJobCompleted(); }
				static inline bool IsReceivingComplete()   { return IsJobCompleted(); }
								
				static bool WasTransmittingError()
				{
					if (!IsJobCompleted() )
						return true;
					
					switch (GetStatus() )
					{
					case MasterDataTransmitted:
					case SlaveDataTransmitted:
					// In case of last byte
					case MasterDataNotTransmitted:
					case SlaveDataNotTransmitted:
						return false;
					default:
						return true;
					}
					
				} // WasTransmittingError								
				
				static bool WasReceivingError()
				{
					if (!IsJobCompleted() )
						return true;
					
					switch (GetStatus() )
					{
					case MasterDataReceived:
					case SlaveDataReceived:
					case SlaveGeneralDataReceived:
						return false;
					default:
						return true;
					}
					
				} // WasReceivingError						
				
				
/**********************************************************************************************************************/					
				
			public:
				
				static inline volatile bool IsJobCompleted() { return JobCompleteBit::IsSet(); }				
				
				template <class UserAbort> static bool WaitUntilJobComplete()
				{
					UserAbort::OperationStart();
					
					while (!IsJobCompleted() )
						if (UserAbort::IsAborted() ) return false;
					
					return true;
					
				} // WaitUntilJobComplete
				
				static inline bool WaitUntilJobComplete()
				{
					return WaitUntilJobComplete<DefaultUserAbort>();
					
				} // WaitUntilJobComplete
				
				
				/**
				 * Sets up TWI in master mode
				 * @param bitRate	Bitrate that can be calculated with TWIClockCalculation macro.
				 *					CPU clock frequency in all connected slaves must be at least 16 times higher than the SCL frequency
				 * @param prescale	Prescalor selection from AVRCpp::TWI::ClockPrescaler enumeration
				 * @see TWIClockCalculation
				 * @see AVRCpp::TWI::ClockPrescaler
				 * @see AVRCpp::TWI::TWIBase::SetUpSlave
				 */
				static void SetUpMaster (
						uint8_t bitRate,
						ClockPrescaler prescale,
						AcknowledgeScheme acknowledgeScheme = AcknowledgeAll,
						TWIEnabled _TWIEnabled = Enabled )
				{					
					BitRateRegister::Set(bitRate);
					StatusRegister::Set(prescale & ClockPrescaleFlag); 		
					ControlRegister::Set(_TWIEnabled | acknowledgeScheme);
					
				} // SetUpMaster
				
				/**
				 * Sets up TWI in slave mode
				 * @param address		7-bit slave's own address
				 * @param addressMask		Ones mean "don't care" and zeores must make exact match with slaves's own address in oder to receive incoming data.
				 * @param generalCalls  	General calls enabled or discarded
				 * @param acknowledgeScheme	Acknowledge bit generation option
				 * @param _TWIEnabled		SDA and SCL pins override normal port operation or not
				 * @see AVRCpp::TWI::TWIBase::SetUpMaster
				 */					 
				static void SetUpSlave (
						uint8_t address,
						uint8_t addressMask = 0,
						GeneralCalls generalCalls = GeneralCallsEnabled,
						AcknowledgeScheme acknowledgeScheme = AcknowledgeAll,
						TWIEnabled _TWIEnabled = Enabled )
				{
					BitRateRegister::Set(0x00);
					StatusRegister::Set(0x00);
					SlaveAddressRegister::Set((address << 1) | generalCalls);
					SlaveAddressMaskRegister::Set(addressMask << 1);
					ControlRegister::Set(_TWIEnabled | acknowledgeScheme);
					
				} // SetUpSlave
				
				/**
				 * Releases bus and enters into non-addressed disabled slave mode
				 */					 
				static void Release()
				{						
					BitRateRegister::Set(0x00);
					StatusRegister::Set(0x00);
					ControlRegister::Set(0x00);
					SlaveAddressRegister::Set(0x00);
					SlaveAddressMaskRegister::Set(0x00);
					
				} // Release
				
				/**
				 * Releases bus for a moment and then initializes it
				 * again with options that have previously  set
				 */
				static void ReleaseAndInitialize()
				{
					uint8_t control	= ControlRegister::Get() & (Enabled | AcknowledgeAll);
					uint8_t address	= SlaveAddressRegister::Get();
					uint8_t mask	= SlaveAddressMaskRegister::Get();
					uint8_t bitRate	= BitRateRegister::Get();
					uint8_t status	= StatusRegister::Get();
					
					Release();
					
					BitRateRegister::Set(bitRate);
					StatusRegister::Set(status);
					SlaveAddressRegister::Set(address);
					SlaveAddressMaskRegister::Set(mask);
					ControlRegister::Set(control);
					
				} // ReleaseAndInitialize
				
				
				/**
				 * The same as AVRCpp::TWI::TWIBase::GetStatus except return type is uint_8 (not enumeration type)
				 * @see AVRCpp::TWI::TWIBase::GetStatus
				 */					 
				static inline uint8_t GetStatusCode()
				{	
					if (!IsJobCompleted() ) return JobNotComplete;
					
					return StatusRegister::Get() & StatusCodeFlag; 
					
				} // GetStatusCode
				
				static inline void Enable() { EnableBit::Set(); }
				static inline void Disable() { EnableBit::Clear(); }
				static inline bool IsEnabled() { return EnableBit::IsSet(); }
				
				/**
				 * Reads interface's present state
				 * @see AVRCpp::TWI::Status
				 */					 
				static inline Status GetStatus()
				{	
					return (Status)GetStatusCode();
					
				} // GetStatus
				
				/**
				 * Starts data transmission. It does not wait until done.
				 * It can be also used as pure (i.e. without waiting until complete)
				 * repeated start.
				 * @note Applicable only in master mode
				 */
				static inline bool PureStart()
				{
					// Set start condition
					StopBit::Clear();
					StartBit::Set();
					
					// Apply
					NewJobBit::Set();
					
					return true;
					
				} // PureStart
				
				/**
				 * Starts data transmission
				 * @note Applicable only in master mode
				 */
				template <class UserAbort> static inline bool Start()
				{
					Status status;
					
					if (!PureStart() ) return false;
					
					// Block until done
					if (!WaitUntilJobComplete<UserAbort>() )
						return false;

					status = GetStatus();

					return (status == MasterStart || status == MasterRepeatedStart);
					
				} // Start
				
				static inline bool Start() { return Start<DefaultUserAbort>(); }
				
				/**
				 * Starts data transmission without releasing communication line
				 * @note Applicable only in master mode
				 */
				template <class UserAbort> static inline bool RepeatedStart()
				{
					if (!PureStart() ) return false;
					
					// Block until done
					if (!WaitUntilJobComplete<UserAbort>() )
						return false;
					
					return (GetStatus() == MasterRepeatedStart);
					
				} // RepeatedStart
				
				static inline bool RepeatedStart() { return RepeatedStart<DefaultUserAbort>(); }
				
				/**
				 * Stops data transmission
				 * @note Applicable only in master mode
				 */
				static bool Stop()
				{
					if (!IsJobCompleted() ) return false;
					
					// Set stop condition
					StopBit::Set();
					StartBit::Clear();
					
					// Apply
					NewJobBit::Set();
					
					// NB! interrupt flag is never set after STOP condition!
					// so... no waiting at all
					
					return true;
					
				} // Stop
				
				/**
				 * Exactly the same as AVRCpp::TWI::TWIBase::Stop
				 * @note Applicable only in master mode
				 */
				static inline bool PureStop()
				{
					// Set stop condition
					StopBit::Set();
					StartBit::Clear();
					
					// Apply
					NewJobBit::Set();
					
					return true;

				} // PureStop
				
				/* 
				 * Restarts (instantly stops and starts) data transmission. It does not wait until done.
				 * @note Applicable only in master mode
				 */
				static bool PureRestart()
				{
					if (!IsJobCompleted() ) return false;
					
					// Both conditions instantly
					StartAndStopBits::Set();
					
					// Apply
					NewJobBit::Set();
					
					return true;
					
				} // Restart
				
				/**
				 * Restarts (instantly stops and starts) data transmission
				 * @note Applicable only in master mode
				 */
				template <class UserAbort> static bool Restart()
				{
					if (!PureRestart() ) return false;
					
					// Block until done
					if (!WaitUntilJobComplete<UserAbort>() )
						return false;
					
					return (GetStatus() == MasterStart);
					
				} // Restart
				
				
				static inline bool Restart() { return Restart<DefaultUserAbort>(); }
				
				/**
				 * Signs all consequent received data to be acknowledged (ACK).
				 * @see AVRCpp::TWI::TWIBase::DoNotAcknowledge
				 * @see AVRCpp::TWI::TWIBase::Read
				 */
				static inline void Acknowledge()
				{						
					EnableAcknowledgeBit::Set();
				
				} // Acknowledged
				
				/**
				 * Signs all consequent received data to be NOT acknowledged (NACK).
				 * This function is used before calling AVRCpp::TWI::TWIBase::Read function to let the receiver to signal the transmitter that
				 * it is the last byte to be received, or for some reason receiver cannot read any more bytes.
				 * @see AVRCpp::TWI::TWIBase::Acknowledge
				 * @see AVRCpp::TWI::TWIBase::Read
				 */
				static inline void DoNotAcknowledge()
				{
					EnableAcknowledgeBit::Clear();
					
				} // NotAcknowledged
				
				
				/**
				 * Selects mode for desired slave. In contrast with AVRCpp::TWI::TWIBase::PureSelectSlave it does not wait until address and RW bit is sent.
				 * @param address	Selected slave 7-bit address. Zero stands for general transmission to all slaves.
				 *					Addresses covered by mask 1111--- should not be used. It is preserved for future enhancements.
				 * @param mode		read-from-slave or write-to-slave mode
				 * @note Applicable only in master mode
				 * @attention This function is NOT safe: special care must be taken.
				 * @see AVRCpp::TWI::Mode
				 * @see AVRCpp::TWI::TWIBase::PureSelectSlave
				 */					 					
				static bool PureSelectSlave(uint8_t address, Mode mode)
				{							
					if (!IsJobCompleted() ) return false;
					
					DataRegister::Set((address << 1) | mode);																	
					
					StartBit::Clear();
					StopBit::Clear();
					NewJobBit::Set();
					
					return true;
					
				} // PureSelectSlave
				
				/**
				 * Selects mode for desired slave. In contrast with AVRCpp::TWI::TWIBase::PureSelectSlave it waits until address and RW bit is sent.
				 * @param address	Selected slave 7-bit address. Zero stands for general transmission to all slaves.
				 *					Addresses covered by mask 1111--- should not be used. It is preserved for future enhancements.
				 * @param mode		read-from-slave or write-to-slave mode
				 * @note Applicable only in master mode
				 * @see AVRCpp::TWI::Mode
				 * @see AVRCpp::TWI::TWIBase::PureSelectSlave
				 */					 					
				template <class UserAbort> static bool SelectSlave(uint8_t address, Mode mode)
				{							
					if (!PureSelectSlave(address, mode) )
						return false;
					
					if (!WaitUntilJobComplete<UserAbort>() )
						return false;
					
					if (mode == WriteToSlave)
						return (GetStatus() == MasterWriteModeSelected);
					else
						return (GetStatus() == MasterReadModeSelected);
					
				}  // SelectSlave					
				
				static inline bool SelectSlave(uint8_t address, Mode mode) { return SelectSlave<DefaultUserAbort>(address, mode); }
				
				/**
				 * Transmits a byte without any status tests. It is used in special cases where code size and speed are critical
				 * or interrupt based applications.
				 * @attention This function is UNSAFE! Do not use this function without exactily knowing possible consequences.
				 */
				static bool PureWrite(uint8_t data)
				{					
					if (!IsJobCompleted() ) return false;
					
					DataRegister::Set(data);
					
					if (WriteCollisionBit::IsSet() ) return false;
					
					StartAndStopBits::Clear();
					NewJobBit::Set();
					
					return true;
					
				} // PureWrite
				
				/**
				 * Transmits a byte.
				 * @param data The byte to be sent
				 */
				template <class UserAbort> static bool Write(uint8_t data)
				{					
					if (!PureWrite(data) )
						return false;
					
					return WaitUntilJobComplete<UserAbort>();
					
				} // Write										
				
				static inline bool Write(uint8_t data) { return Write<DefaultUserAbort>(data); }
				
				/**
				 * Clears interrupt flag with assuring that stop and start bits are cleared.
				 * It is commonly used in TWI interrupt handler where no other particular
				 * action according to current state is not required.
				 */
				static bool PureDefaultOperation()
				{
					if (!IsJobCompleted() ) return false;
					
					StartAndStopBits::Clear();
					NewJobBit::Set();
					
					return true;
					
				} // PureDefaultOperation
				
				/**
				 * Same as AVRCpp::TWI::TWIBase:PureDefaultOperation but it
				 * waits until interrupt flag rises again.
				 * @note Not good idea to use it in interrupt handler.
				 */
				template <class UserAbort> static bool DefaultOperation()
				{
					if (!PureDefaultOperation() )
						return false;
					
					return WaitUntilJobComplete<UserAbort>();
					
				} // DefaultOperation
				
				static inline bool DefaultOperation() { return DefaultOperation<DefaultUserAbort>(); }
				
				/**
				 * Receives a byte.
				 * @param data Read byte is returned by this referenced parameter
				 * @see AVRCpp::TWI::TWIBase::Acknowledge
				 * @see AVRCpp::TWI::TWIBase::DoNotAcknowledge
				 */
				template <class UserAbort> static bool Read(uint8_t &data, bool acknowledge = true)
				{						
					if (!IsJobCompleted() ) return false;
					
					StartAndStopBits::Clear();
					NewJobBit::Set();
					EnableAcknowledgeBit::SetTo(acknowledge);
					
					if (!WaitUntilJobComplete<UserAbort>() ) return false;
					
					data = DataRegister::Get();
					
					return true;
					
				} // Read
				
				
				static inline bool Read(uint8_t &data, bool acknowledge = true) { return Read<DefaultUserAbort>(data, acknowledge); }
				
				/**
				 * Exactly the same as AVRCpp::TWI::TWIBase::Read. Only for compatibility with Tranceiver class.
				 */
				static inline bool PureRead(uint8_t &data) { return Read(data); }
				
				static bool PureSlaveRead(uint8_t &data)
				{
					if (!IsJobCompleted() ) return false;
					
					data = DataRegister::Get();
					StartAndStopBits::Clear();
					
					NewJobBit::Set();
					
					return true;
					
				} // PureSlaveRead
				
				template <class UserAbort> static bool SlaveRead(uint8_t &data)
				{
					if (!PureSlaveRead(data) )
						return false;
					
					return WaitUntilJobComplete<UserAbort>();
					
				} // SlaveRead
				
				static inline bool SlaveRead(uint8_t &data) { return SlaveRead<DefaultUserAbort>(data); }
				
	   		}; // template struct TWIBase
   			
		}; // namespace Internal
		
	} // namespace TWI
		
}; // namespace AVRCpp

namespace AVRCpp
{
	namespace TWI
	{		
		
		struct DelegateTWI0 : Internal::TWIBase <
				_TWBR,					/* BitRateRegister */
				_TWCR,					/* ControlRegister */
				_TWSR,					/* StatusRegister */
				_TWDR,					/* DataRegister */								
				_TWAR,					/* SlaveAddressRegister */
				_TWAMR,					/* SlaveAddressMaskRegister */							
				EmptyUserAbort >		/* DefaultUserAbort */
		{
			struct JobCompleteInterrupt : Interrupt<Bits<_TWCR, _TWIE>, Bits<_TWCR, TWINT> > { __INTERRUPT_HANDLER_SUPPORT__ };
			
		}; // struct DelegateTWI0

		template <class UserAbort = EmptyUserAbort>
		struct TWI0 : Internal::TWIBase <
				_TWBR,					/* BitRateRegister */
				_TWCR,					/* ControlRegister */
				_TWSR,					/* StatusRegister */
				_TWDR,					/* DataRegister */								
				_TWAR,					/* SlaveAddressRegister */
				_TWAMR,					/* SlaveAddressMaskRegister */							
				UserAbort >				/* DefaultUserAbort */
		{
			typedef DelegateTWI0::JobCompleteInterrupt JobCompleteInterrupt;
			
		}; // struct TWI0
		

	} // namespace TWI

} // namespace AVRCpp


/**********************************************************************************************************************/

#endif // #ifdef __TWI__

#endif // ifndef __AVR_CPP_TWI_H__

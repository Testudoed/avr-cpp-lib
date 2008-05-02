/**********************************************************************************************************************\

	C++ library for Atmel AVR microcontrollers
	Copyright (C) 2008 Lauri Kirikal, Mikk Leini, Rasmus Raag, MT� TT� Robotiklubi

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

#define TWIClockCalculation(BitRate, Prescale)       F_CPU / (16 + 2  * BitRate * (4 ^ Prescale))

#define TWI_TC_ns		TWI
#define TWI_TC_struct	TWI::TWI0::TransmissionCompleteInterrupt

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
			// Master mode statuses
			MasterStart                     = 0x08,
			MasterRepeatedStart             = 0x10,
			MasterSlaveWriteModeSelected    = 0x18,
			MasterSlaveWriteModeNotSelected = 0x20,
			MasterDataTransmitted           = 0x28,
			MasterDataNotTransmited         = 0x30,
			MasterArbitrationLost           = 0x38,		
			MasterSlaveReadModeSelected     = 0x40,
			MasterSlaveReadModeNotSelected  = 0x48,
			MasterDataReceived              = 0x50,
			MasterLastDataReceived          = 0x58,
			
			// Slave mode statuses
			SlaveWriteModeReceived          = 0x60,
			SlaveWriteModeArbitrationLost   = 0x68,
			SlaveGeneralCallReceived        = 0x70,
			SlaveGeneralCallArbitrationLost = 0x78,
			SlaveDataReceived               = 0x80,
			SlaveLastDataReceived           = 0x88,
			SlaveGeneralDataReceived        = 0x90,
			SlaveGeneralLastDataReceived    = 0x98,			
			SlaveFinish                     = 0xA0,			
			SlaveReadModeReceived           = 0xA8,			
			SlaveReadModeArbitrationLost    = 0xB0,
			SlaveDataTransmitted            = 0xB8,
			SlaveDataNotTransmitted         = 0xC0,
			SlaveAnnoyedAfterLastData       = 0xC8			
						
		}; // enum Status
			
		namespace Internal
		{
			enum BitFlags
			{
				JobCompleteFlag	        = _TWINT,
				AcknowledgeFlag         = _TWEA,
				StartConditionFlag      = _TWSTA,
				StopConditionFlag       = _TWSTO,
				WriteCollisionFlag      = _TWWC,
				TWIEnableFlag           = _TWEN,
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
					bool waitForTaskCompletion = false >
						
			struct TWIBase : public Transceiver< TWIBase<
					BitRateRegister,
					ControlRegister,
					StatusRegister,
					DataRegister,
					SlaveAddressRegister,
					SlaveAddressMaskRegister,
					waitForTaskCompletion > >
			{
			
				friend class Transceiver< TWIBase<
						BitRateRegister,
						ControlRegister,
						StatusRegister,
						DataRegister,
						SlaveAddressRegister,
						SlaveAddressMaskRegister,
						waitForTaskCompletion > >;
						
				typedef Transceiver< TWIBase<
						BitRateRegister,
						ControlRegister,
						StatusRegister,
						DataRegister,
						SlaveAddressRegister,
						SlaveAddressMaskRegister,
						waitForTaskCompletion > > Parent;
			
				private:
				
					typedef Bits<ControlRegister, JobCompleteFlag> JobCompleteBit;
					
					
/**********************************************************************************************************************/
					
				/**
				 * Neccessary functions for transceiver
				 */
					 
				public:
					
					static inline bool CanTransmit()
					{						
						if (!IsJobCompleted())
							return false;
						
						switch (GetStatus())
						{
							case MasterSlaveWriteModeSelected:
							case MasterSlaveWriteModeNotSelected:
							case MasterDataTransmitted:
							case MasterDataNotTransmited:
							case SlaveReadModeReceived:
							case SlaveReadModeArbitrationLost:
							case SlaveDataTransmitted:
								return true;
								
							default:
								return false;
						}
						
					} // CanSend
					
					static inline bool CanReceive()
					{
						if (!IsJobCompleted())
							return false;
							
						switch (GetStatus())
						{						
							case MasterDataReceived:	
							case MasterLastDataReceived:							
							case SlaveDataReceived:	
							case SlaveLastDataReceived:
							case SlaveGeneralDataReceived:							
							case SlaveGeneralLastDataReceived:
								return true;
								
							default:
								return false;
						}
						
					} // CanReceive
					
					static inline bool IsTransmitCompleted() { return IsJobCompleted(); }
					static inline bool IsReceiveCompleted()  { return IsJobCompleted(); }
					
					static inline bool WasTransmitError()
					{					
						// This function should not be called then the job has not completed
						if (!IsJobCompleted())
							return false;
							
						switch (GetStatus())
						{							
							case MasterDataNotTransmited:
							case MasterArbitrationLost:
							case SlaveAnnoyedAfterLastData:													
								return true;
								
							// Special case - if slave has sent it's last byte and masters does not NACK
							// it's probably master's fault
							case SlaveDataNotTransmitted:
								return !slaveLastByteSent;								
								
							default:
								return false;
						}
						
					} // WasTransmitError
					
					static inline bool WasReceiveError()
					{
						// This function should not be called then the job has not completed					
						if (!IsJobCompleted())
							return false;
							
						// If the data was reed after receive possibility check there cannot be any errors						
						return false;
						
					} // WasReceiveError
					
				protected:
					
					static inline void PureByteTransmit(const uint8_t &data)
					{					
						DataRegister::Set(data);
					
						ControlRegister::Set(JobCompleteFlag | TWIEnableFlag);		
						
					} // PureByteTransmit
					
					static inline void PureByteReceive(uint8_t &data)
					{
						Acknowledge(); // Acknowledge incoming data
						// todo - sometimes NACK should be returned
						
						if (!Parent::WaitUntilReceiveCompleted())
							return;

						data = DataRegister::Get();						
												
					} // PureByteReceive
					
					
/**********************************************************************************************************************/					
					
				public:
				
					static inline bool volatile IsJobCompleted() { return JobCompleteBit::IsSet(); }				
					static inline void WaitUntilJobComplete() { while (!IsJobCompleted()); }
					
					/**
					 *  Setup in master mode
					 */
					static void SetupMaster(uint8_t bitRate, ClockPrescaler prescale)
					{					
						BitRateRegister::Set(bitRate);
						StatusRegister::Set(prescale & ClockPrescaleFlag); 		
						ControlRegister::Set(TWIEnableFlag);
					
					} // SetupMaster
					
					/**
					 * Setup in slave mode
					 */					 
					static void SetupSlave(uint8_t address, uint8_t addressMask, bool listenForGeneralCalls)
					{
						SlaveAddressRegister::Set((address << 1) | (listenForGeneralCalls ? GeneralCallEnableFlag : 0));
						SlaveAddressMaskRegister::Set(addressMask << 1);
						ControlRegister::Set(TWIEnableFlag);
						
					} // SetupSlave
					
					/**					
					 * Release bus
					 */
					static void Release()
					{						
						ControlRegister::Set(0);
						
					} // Release
					
					/**
					 * Release bus and go to non-addressed slave mode
					 */					 
					static void ReleaseBus()
					{						
						SlaveAddressRegister::Set(0);
						SlaveAddressMaskRegister::Set(0);
						ControlRegister::Set(JobCompleteFlag | TWIEnableFlag);
						
					} // ReleaseBus
					
					/**
					 * Status code getting 
					 */					 
					static inline uint8_t GetStatusCode()
					{	
						return StatusRegister::Get() & StatusCodeFlag; 
						
					} // GetStatusCode
					
					/**
					 * Status enum getting 
					 */					 
					static inline Status GetStatus()
					{	
						return (Status)GetStatusCode();
						
					} // GetStatus
					
					/**
					 * Start 
					 */
					static void Start()
					{
						// Send start condition
						ControlRegister::Set(JobCompleteFlag | StartConditionFlag | TWIEnableFlag);
						
						if (waitForTaskCompletion)
							WaitUntilJobComplete();	
						
					} // Start
					
					/**
					 * Stop 
					 */
					static void Stop()
					{
						// Send stop condition
						ControlRegister::Set(JobCompleteFlag | StopConditionFlag | TWIEnableFlag);
						
						if (waitForTaskCompletion)
							WaitUntilJobComplete();	
						
					} // Stop
					
					/**
					 * Restart (instant stop and start)
					 */
					static void Restart()
					{
						// Send start and stop conditions in one command
						ControlRegister::Set(JobCompleteFlag | StartConditionFlag | StopConditionFlag | TWIEnableFlag);
						
						if (waitForTaskCompletion)
							WaitUntilJobComplete();	
						
					} // Restart
					
					/**
					 * Acknowledge
					 */
					static void Acknowledge()
					{						
						ControlRegister::Set(JobCompleteFlag | AcknowledgeFlag | TWIEnableFlag);
						
						if (waitForTaskCompletion)
							WaitUntilJobComplete();	
						
					} // Acknowledge
					
					/**
					 * Not acknowledge
					 */
					static void NotAcknowledge()
					{
						ControlRegister::Set(JobCompleteFlag | TWIEnableFlag);
						
						if (waitForTaskCompletion)
							WaitUntilJobComplete();	
						
					} // NotAcknowledge
					
					/**
					 * Select write-to-slave mode for desired slave
					 */					 
					static void SlaveToWriteMode(uint8_t address)
					{												
						DataRegister::Set((address << 1) | WriteModeFlag);	
						ControlRegister::Set(JobCompleteFlag | TWIEnableFlag);
						
						if (waitForTaskCompletion)
							WaitUntilJobComplete();	
						
					} // SlaveToWriteMode
					
					/**
					 * Select read-from-slave mode for desired slave
					 */					 					
					static void SlaveToReadMode(uint8_t address)
					{							
						DataRegister::Set((address << 1) | ReadModeFlag);																	
						ControlRegister::Set(JobCompleteFlag | TWIEnableFlag);
						
						if (waitForTaskCompletion)
							WaitUntilJobComplete();	
						
					}  // SlaveToReadMode					
					
					/**
					 * Send byte
					 */
					static inline void Write(uint8_t data)
					{					
						DataRegister::Set(data);
						ControlRegister::Set(JobCompleteFlag | TWIEnableFlag);						

					} // Write										
					
					/**
					 * Read byte (acknowledged will be returned)					 
					 */
					static void Read(uint8_t &data)
					{						
						data = DataRegister::Get();
						
						Acknowledge();

					} // Read
					
					/**
					 * Read last byte (not acknowledged will be returned)
					 */
					static void ReadLast(uint8_t &data)
					{						
						data = DataRegister::Get();
						
						NotAcknowledge();

					} // ReadLast
							
	   		}; // template struct TWIBase
   			
		}; // namespace Internal

	} // namespace TWI
		
}; // namespace AVRCpp

namespace AVRCpp
{
	namespace TWI
	{		
		template <bool waitForTaskCompletion = false> struct TWI0 : Internal::TWIBase <
				_TWBR,                   /* BitRateRegister */
				_TWCR,                   /* ControlRegister */
				_TWSR,                   /* StatusRegister */
				_TWDR,                   /* DataRegister */								
				_TWAR,                   /* SlaveAddressRegister */
				_TWAMR,                  /* SlaveAddressMaskRegister */							
				waitForTaskCompletion >  /* waitForTaskCompletion */
		{
			struct JobCompleteInterrupt : BasicInterrupt<Bits<_TWCR, _TWIE> > { __INTERRUPT_HANDLER_SUPPORT__ };

		}; // struct TWI0

	} // namespace TWI

} // namespace AVRCpp


/**********************************************************************************************************************/

#endif // #ifdef __TWI__

#endif // ifndef __AVR_CPP_TWI_H__

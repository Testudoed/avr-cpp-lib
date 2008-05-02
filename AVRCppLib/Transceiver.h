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

\**********************************************************************************************************************/

#ifndef __AVR_CPP_TRANSCEIVER_H__
#define __AVR_CPP_TRANSCEIVER_H__

#ifdef FORMATTED_TEXT_SENDING
#include <stdarg.h>
#endif

#include "Collection/BaseArray.h"
#include "Collection/BaseQueue.h"

namespace AVRCpp
{
	using namespace Collection;

/**********************************************************************************************************************\

	General interface for data transmission modules
	
	Possible configuration macros:	
		ALLOW_USER_TRANSCEIVER_ABORT - Not defined by default. Enables user-aborts. Uses 2 bytes of SRAM
		FORMATTED_TEXT_SENDING - Not defined by default. Allows printf style formatted text sending. Uses lot of memory.
		
	Inheriting class should have following functions:
		static inline bool CanTransmit()
		static inline bool CanReceive()
		static inline bool IsTransmitCompleted()
		static inline bool IsReceiveCompleted()
		static inline bool WasTransmitError()
		static inline bool WasReceiveError()
		static inline void PureByteTransmit(const uint8_t &data)
		static inline void PureByteReceive(uint8_t &data)
		
	For developers:
		Transceiver = Transmitter + Receiver		
		Transfer = Data moving in either of directions		

\**********************************************************************************************************************/


template <class Interface> class Transceiver
{
	protected:
	
		#ifdef ALLOW_USER_TRANSCEIVER_ABORT
		static volatile bool userTransmittingAbort = false;
		static volatile bool userReceivingAbort    = false;
		#endif
	
		/**
		 * Wait until possible to transmit or until error occurs
		 */
		static inline bool WaitUntilPossibleToTransmit()
		{
			while (!Interface::CanTransmit())
			{				
				#ifdef ALLOW_USER_TRANSCEIVER_ABORT
				if (WasUserTransmittingAbort()) return false;
				#endif
			}
			
			return true;
		}
		
		/**
		 * Wait until possible to receive or until error occurs
		 */
		static inline bool WaitUntilPossibleToReceive()
		{
			while (!Interface::CanReceive())
			{
				#ifdef ALLOW_USER_TRANSCEIVER_ABORT
				if (WasUserReceivingAbort()) return false;
				#endif
			}
			
			return true;
		}
		
		/**
		 * Wait until transmit completed or until error occurs
		 */
		static inline bool WaitUntilTransmitCompleted()
		{
			while (!Interface::IsTransmitCompleted())
			{
				if (Interface::WasTransmitError()) return false;
				#ifdef ALLOW_USER_TRANSCEIVER_ABORT
				if (WasUserTransmittingAbort()) return false;
				#endif
			}
			
			return true;
		}
		
		/**
		 * Wait until receive completed or until error occurs
		 */
		static inline bool WaitUntilReceiveCompleted()
		{
			while (!Interface::IsReceiveCompleted())
			{
				if (Interface::WasReceiveError()) return false;
				#ifdef ALLOW_USER_TRANSCEIVER_ABORT
				if (WasUserReceivingAbort()) return false;
				#endif
			}
			
			return true;
		}
		
	protected:

		/**
		 * Wait until possible to transmit and then do it
		 */
		static inline bool SafeByteTransmit(const uint8_t &data)
		{
			if (!WaitUntilPossibleToTransmit())
				return false;
			
			Interface::PureByteTransmit(data);
			
			return WaitUntilTransmitCompleted();
		}
		
		/**
		 * Wait until possible to receive and then do it
		 */
		static inline bool SafeByteReceive(uint8_t &data)
		{
			if (!WaitUntilPossibleToReceive())
				return false;
			
			Interface::PureByteReceive(data);
			
			return WaitUntilReceiveCompleted();
		}
		
		/**
		 * Safe byte-array transmiting
		 */
		template <typename SizeType> static inline bool SafeByteArrayTransmit(uint8_t *byteData, SizeType bytesCount)
		{
			register SizeType i;
			
			for (i = 0; i < bytesCount; i++)
				if (!SafeByteTransmit(byteData[i]))
					return false;

			return true;
			
		} // SendByteArray
		
		/**
		 * Safe byte-array receiving
		 */
		template <typename SizeType> static inline bool SafeByteArrayReceive(uint8_t *byteData, SizeType bytesCount)
		{
			register SizeType i;
			
			for (i = 0; i < bytesCount; i++)
				if (!SafeByteReceive(byteData[i]))
					return false;
					
			return true;
			
		} // ReceiveByteArray

	public:
		
		
/**********************************************************************************************************************\

	User aborting functions
	
\**********************************************************************************************************************/
		
		#ifdef ALLOW_USER_TRANSCEIVER_ABORT
		
		/**
		 * Abort all
		 */
		static inline void Abort()
		{
			userTransmittingAbort = true;
			userReceivingAbort    = true;
		}
		
		/**
		 * Abort transmitting 
		 */
		static inline void AbortTransmitting()
		{
			userTransmittingAbort = true;
		}
		
		/**
		 * Abort receiving 
		 */
		static inline void AbortReceiving()
		{
			userReceivingAbort = true;
		}
		
		/**
		 * Abort resetting
		 */
		static inline void ResetAbort()
		{
			userTransmittingAbort   = false;
			userReceivingAbort = false;
		}
		
		/**
		 * Transmitting abort resetting
		 */
		static inline void ResetTransmittingAbort()
		{
			userTransmittingAbort = false;			
		}
		
		/**
		 * Receiving abort resetting
		 */
		static inline void ResetReceivingAbort()
		{
			userReceivingAbort = false;			
		}
		
		/**
		 * User sending or receiving abort presence returning
		 */
		static volatile inline bool WasUserAbort()
		{
			return userTransmittingAbort || userReceivingAbort;
		}
		
		/**
		 * User sending abort presence returning
		 */
		static volatile inline bool WasUserTransmittingAbort()
		{
			return userTransmittingAbort;
		}
		
		/**
		 * User receiving abort presence returning
		 */
		static volatile inline bool WasUserReceivingAbort()
		{
			return userReceivingAbort;
		}
		
		#endif // #ifdef ALLOW_USER_TRANSCEIVER_ABORT
		
		
/**********************************************************************************************************************\

	Various data sending functions
	
\**********************************************************************************************************************/
		
		/**
		 * Single byte sending
		 */
		static bool Send(const uint8_t &data)
		{
			return SafeByteTransmit(data);
			
		} // Send
		
		/**
		 * Single data unit sending
		 */
		template <typename DataType> static bool Send(const DataType &data)
		{
			return SafeByteArrayTransmit((uint8_t *)(&data), (uint8_t)sizeof(DataType));
			
		} // Send
		
		/**
		 * Array sending
		 */
		template <typename DataType, typename SizeType> static bool SendArray(DataType *data, SizeType size)
		{
			return SafeByteArrayTransmit((uint8_t *)(data), size * sizeof(DataType));
			
		} // SendArray

		/**
		 * Library own array sending
		 */
		template <typename DataType, typename SizeType, typename ListType> static bool SendArray(BaseArray<DataType, SizeType, ListType> &array)
		{
			return SafeByteArrayTransmit((uint8_t *)array.Front(), array.Size() * sizeof(DataType));
			
		} // SendArray
		
		/**
		 * Queue sending
		 */
		template <typename DataType, typename SizeType, typename ListType> static bool SendQueue(BaseQueue<DataType, SizeType, ListType> &queue)
		{	
			register DataType item;
			
			while (queue.Pop(item))
			{
				if (!SafeByteArrayTransmit((uint8_t *)item, (uint8_t)sizeof(DataType)))
					return false;
			}
			
			return true;
			
		} // SendQueue

		/**
		 * Length-specified text sending
		 */
		template <typename SizeType> static bool SendText(const char *text, SizeType size)
		{
			register SizeType i;
			
			for (i = 0; (i < size) && (text[i] > 0); i++)			
				if (!SafeByteTransmit(text[i]))
					return false;
			
			while (i++ < size)
				if (!SafeByteTransmit(0))
					return false;
			
			return true;
			
		} // SendText
		
		/**
		 * Null-delimited text sending
		 */
		static bool SendText(const char *text)
		{
			register uint16_t i;
			
			// Don't check for size-limits because 99.9% of systems doesn't have more than 64KB of memory
			for (i = 0; text[i] > 0; i++)			
				if (!SafeByteTransmit(text[i]))
					return false;			
			
			return true;
			
		} // SendText
		
		#ifdef FORMATTED_TEXT_SENDING
	
		/**
		 * Formatted text sending
		 * Maximum possible text length is 254 characters!
		 */
		static bool SendFormattedText(const char *format, ...)
		{
			va_list args;					
			char text[255];
			unsigned char len;

			va_start(format, args);
			len = vsprintf(text, format, args);
			va_end(args);			
			
			for (i = 0; i < len; i++)
				if (!SafeByteTransmit(text[i]))
					return false;
					
			return true;
			
		} // SendFormattedText
		
		#endif // #ifdef FORMATTED_TEXT_SENDING
		
		
/**********************************************************************************************************************\

	Various data receiving functions
	
\**********************************************************************************************************************/
		
		
		/**
		 * Single byte receiving
		 */
		static bool Receive(uint8_t &data)
		{
			return SafeByteReceive(data);
			
		} // Send
		
		/**
		 * Single data unit receiving
		 */
		template<typename DataType> static bool Receive(DataType &data)
		{
			return SafeByteArrayReceive((uint8_t *)(&data), sizeof(DataType));
			
		} // Receive
		
		/**
		 * Array receiving
		 */
		template<typename DataType, typename SizeType> static bool ReceiveArray(DataType *data, SizeType size)
		{
			return SafeByteArrayReceive((uint8_t *)(data), size * sizeof(DataType));
			
		} // ReceiveArray			
		
		/**
		 * Length-specified text receiving
		 * Text memory has to be size + 1 because of the null-termination
		 */
		template <typename SizeType> static bool ReceiveText(char *text, SizeType size)
		{
			register SizeType i;
			
			for (i = 0; i < size; i++)
				if (!SafeByteReceive(text[i]))
					return false;
					
			// Null-terminate the string		
			// If one does not want to terminate, he should use the ReceiveArray function
			text[size] = 0;

			return true;
			
		} // ReceiveText
		
		/**
		 * Null-delimited text receiving
		 */
		static bool ReceiveText(char *text)
		{
			register uint16_t i = 0;
			
			// Don't check for size-limits because 99.9% of systems doesn't have more than 64KB of memory
			do
			{
				if (!SafeByteReceive(text[i]))
					return false;
					
			} while (text[i++]);
			
			return true;
			
		} // ReceiveText
};

/**********************************************************************************************************************/

} // namespace AVRCpp

#endif // #ifndef __AVR_CPP_TRANSCEIVER_H__


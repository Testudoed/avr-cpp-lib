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
		FORMATTED_TEXT_SENDING - Not defined by default. Allows printf style formatted text sending. Uses lot of memory.
		
	Inheriting class should have following functions:
		static inline bool CanTransmit()
		static inline bool CanReceive()
		static inline bool IsTransmitterAvailable()
		static inline bool IsTransmitterAvailable()
		static inline bool IsTransmitingComplete()
		static inline bool IsReceivingComplete()
		static inline bool WasTransmitingError()
		static inline bool WasReceivingError()
		static inline bool PureWrite(const uint8_t data)
		static inline bool PureRead(uint8_t &data)
		
	For developers:
		Transceiver = Transmitter + Receiver		
		Transfer = Data moving in either of directions		

\**********************************************************************************************************************/


struct EmptyUserAbort
{
	inline static void OperationStart() {}
	inline static bool IsAborted() { return false; }
	
}; // struct EmptyUserAbort


template <class Interface> class Transceiver
{
protected:
	
	/**
	 * Waits until it is possible to transmit or until error occurs
	 */
	template <class UserAbort> static inline bool WaitUntilPossibleToTransmit()
	{
		if (!Interface::IsTransmitterAvailable() ) return false;
		
		UserAbort::OperationStart();
		
		while (!Interface::CanTransmit() )
		{
			if (UserAbort::IsAborted() )
				return false;
		}
		
		return true;
		
	} // WaitUntilPossibleToTransmit
	
	/**
	 * Waits until it is possible to receive or until error occurs
	 */
	template <class UserAbort> static inline bool WaitUntilPossibleToReceive()
	{
		if (!Interface::IsReceiverAvailable() ) return false;
		
		UserAbort::OperationStart();
		
		while (!Interface::CanReceive() )
		{
			if (UserAbort::IsAborted() )
				return false;
		}
		
		return true;
		
	} // WaitUntilPossibleToReceive
	
	/**
	 * Waits until transmiting is completed or until error occurs
	 */
	template <class UserAbort> static inline bool WaitUntilTransmitingComplete()
	{
		UserAbort::OperationStart();
		
		while (!Interface::IsTransmitingComplete() )
		{
			if (UserAbort::IsAborted() )
				return false;
		}
		
		return !Interface::WasTransmitingError();
		
	} // WaitUntilTransmitingComplete
	
	/**
	 * Wait until receiving is completed or until error occurs
	 */
	template <class UserAbort> static inline bool WaitUntilReceivingComplete()
	{
		while (!Interface::IsReceivingComplete() )
		{
			if (UserAbort::IsAborted() )
				return false;
		}
		
		return !Interface::WasReceivingError();
		
	} // WaitUntilReceivingComplete
	
	/**
	 * Waits until it is possible to transmit, then transmits one byte and waits until transmitting is completed
	 */
	template <class UserAbort> static inline bool SafeByteTransmit(const uint8_t &data)
	{
		if (!WaitUntilPossibleToTransmit<UserAbort>() )
			return false;
		
		Interface::PureWrite(data);
		
		return WaitUntilTransmitingComplete<UserAbort>();
		
	} // SafeByteTransmit
	
	/**
	 * Wait until possible to receive and then do it
	 */
	template <class UserAbort> static inline bool SafeByteReceive(uint8_t &data)
	{
		if (!WaitUntilPossibleToReceive<UserAbort>() )
			return false;
		
		Interface::PureRead(data);
		
		return WaitUntilReceivingComplete<UserAbort>();
		
	} // SafeByteReceive
	
	/**
	 * Safe byte-array transmiting
	 */
	template <class UserAbort, typename SizeType> static inline bool SafeByteArrayTransmit(uint8_t *byteData, SizeType bytesCount)
	{
		register SizeType i;
		
		for (i = 0; i < bytesCount; i++)
			if (!SafeByteTransmit<UserAbort>(byteData[i]) )
				return false;
		
		return true;
		
	} // SendByteArray
	
	/**
	 * Safe byte-array receiving
	 */
	template <class UserAbort, typename SizeType> static inline bool SafeByteArrayReceive(uint8_t *byteData, SizeType bytesCount)
	{
		register SizeType i;
		
		for (i = 0; i < bytesCount; i++)
			if (!SafeByteReceive<UserAbort>(byteData[i]) )
				return false;
		
		return true;
		
	} // ReceiveByteArray
	
	
/**********************************************************************************************************************\

	Various data sending functions
	
\**********************************************************************************************************************/

public:
	
	/**
	 * Simple single byte sending
	 */
	static bool Send(const uint8_t &data)
	{
		return SafeByteTransmit<EmptyUserAbort>(data);
		
	} // Send
	
	/**
	 * Single data unit sending
	 */
	template <typename DataType, class UserAbort> static bool Send(const DataType &data)
	{
		return SafeByteArrayTransmit<UserAbort>((uint8_t *)(&data), (uint8_t)sizeof(DataType) );
		
	} // Send
	
	/**
	 * Array sending
	 */
	template <typename DataType, class UserAbort, typename SizeType> static bool SendArray(DataType *data, SizeType size)
	{
		return SafeByteArrayTransmit<UserAbort>((uint8_t *)(data), size * sizeof(DataType) );
		
	} // SendArray

	/**
	 * Library own array sending
	 */
	template <typename DataType, class UserAbort, typename SizeType, typename ListType> static bool SendArray(BaseArray<DataType, SizeType, ListType> &array)
	{
		return SafeByteArrayTransmit<UserAbort>((uint8_t *)array.Front(), array.Size() * sizeof(DataType) );
		
	} // SendArray
	
	/**
	 * Queue sending
	 */
	template <typename DataType, class UserAbort, typename SizeType, typename ListType> static bool SendQueue(BaseQueue<DataType, SizeType, ListType> &queue)
	{	
		register DataType item;
		
		while (queue.Pop(item))
		{
			if (!SafeByteArrayTransmit<UserAbort>( (uint8_t *)item, (uint8_t)sizeof(DataType) ) )
				return false;
		}
		
		return true;
		
	} // SendQueue

	/**
	 * Length-specified text sending
	 */
	template <class UserAbort, typename SizeType> static bool SendText(const char *text, SizeType size)
	{
		register SizeType i;
		
		for (i = 0; (i < size) && (text[i] > 0); i++)			
			if (!SafeByteTransmit<UserAbort>(text[i]) )
				return false;
		
		while (i++ < size)
			if (!SafeByteTransmit<UserAbort>(0) )
				return false;
		
		return true;
		
	} // SendText
	
	/**
	 * Null-delimited text sending
	 */
	template <class UserAbort> static bool SendText(const char *text)
	{
		register uint16_t i;
		
		// Don't check for size-limits because 99.9% of systems doesn't have more than 64KB of memory
		for (i = 0; text[i] > 0; i++)			
			if (!SafeByteTransmit<UserAbort>(text[i]) )
				return false;			
		
		return true;
		
	} // SendText
	
	#ifdef FORMATTED_TEXT_SENDING

	/**
	 * Formatted text sending
	 * Maximum possible text length is 254 characters!
	 */
	template <class UserAbort = EmptyUserAbort> static bool SendFormattedText(const char *format, ...)
	{
		va_list args;					
		char text[255];
		unsigned char len;
		
		va_start(format, args);
		len = vsprintf(text, format, args);
		va_end(args);			
		
		for (i = 0; i < len; i++)
			if (!SafeByteTransmit<UserAbort>(text[i]) )
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
		template<typename DataType, class UserAbort> static bool Receive(DataType &data)
		{
			return SafeByteArrayReceive<UserAbort>((uint8_t *)(&data), sizeof(DataType));
			
		} // Receive
		
		/**
		 * Array receiving
		 */
		template<typename DataType, class UserAbort, typename SizeType> static bool ReceiveArray(DataType *data, SizeType size)
		{
			return SafeByteArrayReceive<UserAbort>((uint8_t *)(data), size * sizeof(DataType));
			
		} // ReceiveArray			
		
		/**
		 * Length-specified text receiving
		 * Text memory has to be size + 1 because of the null-termination
		 */
		template <typename SizeType, class UserAbort> static bool ReceiveText(char *text, SizeType size)
		{
			register SizeType i;
			
			for (i = 0; i < size; i++)
				if (!SafeByteReceive<UserAbort>(text[i]))
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


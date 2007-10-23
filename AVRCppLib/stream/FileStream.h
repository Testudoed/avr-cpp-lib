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

#ifndef __AVR_CPP_STREAM_H__
#define __AVR_CPP_STREAM_H__

#include "../collection/CppString.h"
#include "../collection/StaticQueue.h"
#include "../Delegate.h"
#include "../USART.h"
#include "../EEPROM.h"


namespace AVRCpp
{
	namespace Stream
	{
		typedef FILE *File;
		
		template <class FileIO> File OpenFile()
		{
			File file = fdevopen(FileIO::RequestToWrite, FileIO::RequestToRead);
			
			file->udata = 0;
			
			return file;
			
		} // OpenFile
		
		inline bool WasFileError(File file)
		{
			return ferror(file) ? true : false;
			
		} // WasFileError
		
		inline bool IsEndOfFile(File file)
		{
			return feof(file) ? true : false;
			
		} // IsEndOfFile
		
		inline void ClearFileFlags(FILE *file)
		{
			clearerr(file);
			
		} // ClearFileFlags
		
		inline void CloseFile(File file)
		{
			fclose(file);
			
		} // CloseFile
		
		namespace Internal
		{
			struct NoInterrupt
			{
				static inline void Enable() {}
				static inline void Disable() {}
				static inline bool IsEnabled() { return false; }
				
			}; // struct NoInterrupt
			
			class StreamBase
			{
			public:
				
				virtual int RequestToWrite(uint8_t byte) = 0;
				virtual int RequestToRead() = 0;
				virtual void ReadyToRead() = 0;
				virtual void ReadyToWrite() = 0;
				
				virtual ~StreamBase() {}
				
			}; // abstract class StreamBase
			
			template <class WriteInterrupt, class ReadInterrupt> struct FileIOBase
			{
				typedef WriteInterrupt	WriteNotify;
				typedef ReadInterrupt	ReadNotify;
				
				static void BindWriteInterrupt(StreamBase *stream)
				{
					CppDelegate::GetFastDelegate<WriteInterrupt>().Bind (
							stream,
							&StreamBase::ReadyToWrite );
					
				} // BindWriteInterrupt
				
				static void BindReadInterrupt(StreamBase *stream)
				{
					CppDelegate::GetFastDelegate<ReadInterrupt>().Bind (
							stream,
							&StreamBase::ReadyToRead );
					
					ReadInterrupt::Enable();
					
				} // BindReadInterrupt
				
				static void CloseReadInterrupt()
				{
					ReadInterrupt::Disable();
					CppDelegate::GetFastDelegate<ReadInterrupt>().Clear();
					
				} // CloseReadInterrupt
				
				static void CloseWriteInterrupt()
				{
					WriteInterrupt::Disable();
					CppDelegate::GetFastDelegate<WriteInterrupt>().Clear();
					
				} // CloseWriteInterrupt
				
			}; // template struct FileIOBase
			
			
			template <class WriteInterrupt> struct FileIOBase<WriteInterrupt, NoInterrupt>
			{
				typedef WriteInterrupt	WriteNotify;
				typedef NoInterrupt		ReadNotify;
				
				static void BindWriteInterrupt(StreamBase *stream)
				{
					CppDelegate::GetFastDelegate<WriteInterrupt>().Bind (
							stream,
							&StreamBase::ReadyToWrite );
					
				} // BindWriteInterrupt
				
				static void BindReadInterrupt(StreamBase *stream)
				{
				} // BindReadInterrupt
				
				static void CloseReadInterrupt()
				{
				} // CloseReadInterrupt
				
				static void CloseWriteInterrupt()
				{
					WriteInterrupt::Disable();
					CppDelegate::GetFastDelegate<WriteInterrupt>().Clear();
					
				} // CloseWriteInterrupt
				
			}; // template struct FileIOBase specialisation
			
		} // namespace Internal
		
		
		template <class USARTClass> struct FileIOForUSART :
				Internal::FileIOBase <
						typename USARTClass::DataRegisterEmptyInterrupt,
						typename USARTClass::ReceiveCompleteInterrupt >
		{
			
			static inline bool Write(uint8_t byte)
			{
				USARTClass::Write(byte);
				
				return true;
				
			} // Write
			
			static inline bool Read(uint8_t &byte)
			{
				return USARTClass::Read(byte);
				
			} // Read
			
			static int RequestToWrite(char_t c, File file)
			{
				if (file->udata) return ( (Internal::StreamBase *)file->udata)->RequestToWrite(c);
				
				return !Write(c);
				
			} // RequestToWrite
			
			static int RequestToRead(File file)
			{
				if (file->udata) return ( (Internal::StreamBase *)file->udata)->RequestToRead();
				
				{
					uint8_t byte;
					
					if (Read(byte) ) return byte;
				}
				
				return _FDEV_ERR;
				
			} // RequestToRead
			
		}; // template struct FileIOForUSART
		
		
		struct FileIOForEEPROM : Internal::FileIOBase<EEPROM::ReadyInterrupt, Internal::NoInterrupt>
		{
			
			static inline bool Write(uint8_t byte)
			{
				EEPROM::Write(byte);
				EEPROM::MoveNext();
				
				return true;
				
			} // Write
			
			static inline bool Read(uint8_t &byte)
			{
				byte = EEPROM::Read();
				EEPROM::MoveNext();
				
				return true;
				
			} // Read
			
			static int RequestToWrite(char_t c, File file)
			{
				if (file->udata) return ( (Internal::StreamBase *)file->udata)->RequestToWrite(c);
				
				return !Write(c);
				
			} // RequestToWrite
			
			static int RequestToRead(File file)
			{
				uint8_t byte;
				
				if (Read(byte) ) return byte;
				
				return _FDEV_ERR;
				
			} // RequestToRead
			
		}; // struct FileForEEPROM
		
		
		/**
		 *	Passing this type as a template parameter to TextStream or BinaryStream
		 *	means that buffer is not used for input or output:
		 *	data is synchronously transmitted to underlying hardware and no interrupts are used.
		 */
		class NoBuffer
		{
		public:
			
			static const bool hasBuffer = false;
			
			inline bool Push(uint8_t byte) const { return false; }
			inline bool Pop(uint8_t &byte) const { return false; }
			inline uint8_t GetCapacity() const { return 0; }
			inline uint8_t GetSize() const { return 0; }
			inline bool IsEmpty() const { return true; }
			
		}; // class NoBuffer
		
		
		/**
		 *	Passing this type as a template parameter to TextStream or BinaryStream
		 *	means that buffer is used for input or output:
		 *	data is asynchronously transmitted to underlying hardware by the help of interrupts.
		 *	@param capacity maximum buffer size in bytes.
		 */
		template <uint16_t capacity> class StaticBuffer : public Collection::StaticQueue<uint8_t, uint16_t, capacity>
		{
		public:
			
			static const bool hasBuffer = true;
			
		}; // template class 
		
		
		namespace Internal
		{
			/**
			 * Base class for TextStream and BinaryStream.
			 */
			template <
					class FileIO,
					class OutputBuffer = NoBuffer,
					class InputBuffer = NoBuffer >
				
			class FileStreamBase : public StreamBase
			{
			protected:
				
				File file;
				InputBuffer inputBuffer;
				OutputBuffer outputBuffer;
				
				virtual int RequestToWrite(uint8_t byte)
				{
					
					if (this->outputBuffer.hasBuffer)
					{
						bool success;
						
						FileIO::WriteNotify::Disable();
						
						success = outputBuffer.Push(byte);
						
						if (!outputBuffer.IsEmpty() )
						{
							FileIO::WriteNotify::Enable();
						}
						
						if (success) return 0;
					}
					else
						return !FileIO::Write(byte);
					
					return 1;
					
				} // RequestToWrite
				
				
				virtual int RequestToRead()
				{
					uint8_t byte = 0;
					
					if (inputBuffer.hasBuffer)
					{
						bool success;
						
						FileIO::ReadNotify::Disable();
						success = inputBuffer.Pop(byte);
						FileIO::ReadNotify::Enable();
						
						if (success)
							return byte;
						else
						{
							FileIO::ReadNotify::Disable();
							success = FileIO::Read(byte);
							FileIO::ReadNotify::Enable();
						}
						
						if (success) return byte;
						
					}
					else
					{
						if (FileIO::Read(byte) )
							return byte;
					}
					
					return _FDEV_ERR;
					
				} // RequestToRead
				
				
				virtual void ReadyToWrite()
				{
					uint8_t byte = 0;
					
					if (outputBuffer.Pop(byte) )
						FileIO::Write(byte);
					else
						FileIO::WriteNotify::Disable();
					
				} // ReadyToWrite
				
				
				virtual void ReadyToRead()
				{
					uint8_t byte;
					
					if (FileIO::Read(byte) )
					{
						inputBuffer.Push(byte);
					}
					
				} // ReadyToRead
				
			public:
				
				FileStreamBase<FileIO, OutputBuffer, InputBuffer>() :
						inputBuffer(),
						outputBuffer()
				{
					file = fdevopen(FileIO::RequestToWrite, FileIO::RequestToRead);
					
					file->udata = this;
					
					if (inputBuffer.hasBuffer) FileIO::BindReadInterrupt(this);
					if (outputBuffer.hasBuffer) FileIO::BindWriteInterrupt(this);
					
				} // Stream CONSTRUCTOR
				
				~FileStreamBase<FileIO, OutputBuffer, InputBuffer>()
				{
					if (inputBuffer.hasBuffer) FileIO::CloseReadInterrupt();
					if (outputBuffer.hasBuffer) FileIO::CloseWriteInterrupt();
					
					CloseFile(file);
					
				} // Stream DESTRUCTOR
				
				/// Returns maximum number of characters that can be stored in input queue.
				inline uint16_t GetInputBufferCapacity() { INTERRUPT_SAFE return inputBuffer.GetCapacity(); }
				/// Returns number of characters waiting in input queue.
				inline uint16_t GetInputBufferSize() { INTERRUPT_SAFE return inputBuffer.GetSize(); }
				/// Returns maximum number of characters that can be stored in output queue.
				inline uint16_t GetOutputBufferCapacity() { INTERRUPT_SAFE return outputBuffer.GetCapacity(); }
				/// Returns number of characters waiting in output queue.
				inline uint16_t GetOutputBufferSize() { INTERRUPT_SAFE return outputBuffer.GetSize(); }
				/// Returns file associated with this stream.
				inline File GetFile() { return file; };
				/// Returns 'true' if file error has occurred
				inline bool WasFileError() { return ferror(file) ? true : false; }
				/// Returns 'true' if file end has encountered
				inline bool IsEndOfFile() { return feof(file) ? true : false; }
				/// Clears error and end of file flags
				inline void ClearFileFlags() { clearerr(file); }
				
			}; // template class FileStreamBase
			
		} // namespace Internal
		
	} // namespace Stream
	
} // namespace AVRCpp

#endif // ifndef __AVR_CPP_STREAM_H__

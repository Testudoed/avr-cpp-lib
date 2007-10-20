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

#ifndef __AVR_CPP_BINARY_STREAM_H__
#define __AVR_CPP_BINARY_STREAM_H__

#include "FileStream.h"

namespace AVRCpp
{
	namespace Stream
	{
		/**
		 * Stream for binary data.
		 */
		template <
				class FileIO,
				class OutputBuffer = NoBuffer,
				class InputBuffer = NoBuffer >
			
		class BinaryStream : public Internal::FileStreamBase<FileIO, OutputBuffer, InputBuffer>
		{
		protected:
			
		public:
			
			BinaryStream<FileIO, OutputBuffer, InputBuffer>() :
					Internal::FileStreamBase<FileIO, OutputBuffer, InputBuffer>()
			{
			} // BinaryStream CONSTRUCTOR
			
			/**
			 * Reads 'sizeof(Type)' bytes from input stream
			 * and stores them to the 'value'.
			 * @return 'true' on success.
			 */
			template <typename Type> inline bool Read(Type &value)
			{
				if (fread(&value, sizeof(Type), 1, this->file) )
					return true;
				
				return false;
				
			} // Read
			
			/**
			 * Writes the content of 'value' in binary to output stream
			 * @return 'true' on success.
			 */
			template <typename Type> inline bool Write(Type &value)
			{
				if (fwrite(&value, sizeof(Type), 1, this->file) )
					return true;
				
				return false;
				
			} // Write
			
			/**
			 * Reads 'size * sizeof(Type)' bytes from input stream
			 * and stores them to the buffer pointed by 'pointer'.
			 * @return Number of array items successfully read.
			 */
			template <typename Type> inline uint16_t ReadArray(Type *pointer, uint16_t size)
			{
				return fread(pointer, sizeof(Type), size, this->file);
				
			} // ReadArray
			
			/**
			 * Reads 'size * sizeof(Type)' bytes from input stream
			 * and stores them to the buffer pointed by 'pointer'.
			 * @return Number of array items successfully written.
			 */
			template <typename Type> inline bool WriteArray(Type *pointer, uint16_t size)
			{
				return fwrite(pointer, sizeof(Type), size, this->file);
				
			} // WriteArray
			
			/// Simplified BinaryStream::Write
			template <typename Type>
			inline BinaryStream<FileIO, OutputBuffer, InputBuffer> &operator << (Type &value)
			{
				Write<Type>(value);
				return *this;
				
			} // operator <<
			
			/// Simplified BinaryStream::Read
			template <typename Type>
			inline BinaryStream<FileIO, OutputBuffer, InputBuffer> &operator >> (Type &value)
			{
				Read(value);
				return *this;
				
			} // operator >>
			
			/**
			 * Writes string to the stream. If 'text' length is less than 'size'
			 * then 'size - strlen(text)' zeros are written to the stream after the
			 * buffer content referenced by 'text'. If
			 * 'text' length is greater than size then only first 'size' characters
			 * are written.
			 */
			bool WriteText(str_t text, uint16_t size)
			{
				register uint16_t i;
				
				for (i = 0; i < size; i++)
					if (text[i])
					{
						if (fputc(text[i], this->file) == EOF)
							return false;
					}
					else
						break;
				
				while (i < size)
				{
					if (fputc(0x00, this->file) == EOF)
						return false;
					i++;
				}
				
				return true;
				
			} // WriteText
			
			/**
			 * Reads 'size' characters from stream.
			 * @note buffer pointed by 'text' must be at least 'size + 1' bytes long.
			 */
			bool ReadText(str_t text, uint16_t size)
			{
				if (fread(text, sizeof(char_t), size, this->file) == size)
				{
					text[size] = '\0';
					return true;
				}
				
				return false;
				
			} // ReadText
			
		}; // template class BinaryStream 
		
	} // namespace Stream
	
} // namespace AVRCpp

#endif // ifndef __AVR_CPP_BINARY_STREAM_H__

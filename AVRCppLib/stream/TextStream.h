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

#ifndef __AVR_CPP_TEXT_STREAM_H__
#define __AVR_CPP_TEXT_STREAM_H__

#include "FileStream.h"

namespace AVRCpp
{
	namespace Stream
	{
		enum TextEncoding
		{
			/// Native AVR text encoding
			UTF7,
			/// Line feeds '\n' are substituted with "\r\n"
			UTF7ForWindows
			
		}; // enum TextEncoding
		
		/**
		 * Stream for text.
		 */
		template <
				class FileIO,
				class OutputBuffer = NoBuffer,
				class InputBuffer = NoBuffer >
			
		class TextStream : public Internal::FileStreamBase <FileIO, OutputBuffer, InputBuffer>
		{
		protected:
			
			const TextEncoding encoding;
			
			virtual int RequestToWrite(uint8_t byte)
			{
				
				if (this->outputBuffer.hasBuffer)
				{
					bool success;
					
					FileIO::WriteNotify::Disable();
					
					if (encoding == UTF7ForWindows && byte == '\n')
					{
						success = this->outputBuffer.Push('\r')
								&& this->outputBuffer.Push('\n');
					}
					else
						success = this->outputBuffer.Push(byte);
					
					if (!this->outputBuffer.IsEmpty() )
					{
						FileIO::WriteNotify::Enable();
					}
					
					if (success) return 0;
				}
				else if (encoding == UTF7ForWindows && byte == '\n')
					return !(FileIO::Write('\r') && FileIO::Write('\n') );
				else
					return !FileIO::Write(byte);
				
				return 1;
				
			} // RequestToWrite
			
		public:
			
			/**
			 * Constructor. Opens file according to FileIO template parameter.
			 * @param textEncoding see TextStream::TextEncoding. Default value is UTF7.
			 * @see TextStream::GetEncoding
			 */
			TextStream<FileIO, OutputBuffer, InputBuffer>(TextEncoding textEncoding = UTF7) :
					Internal::FileStreamBase<FileIO, OutputBuffer, InputBuffer>(),
					encoding(textEncoding)
			{
			} // Stream CONSTRUCTOR
			
			/**
			 * Returns text encoding.
			 */
			inline TextEncoding GetEncoding() const { return encoding; };
			
			/**
			 * Provides functionality identical to standard C 'printf'.
			 * @return Number of bytes written or 'EOF'.
			 */
			inline int Print(const char *format, ...) __attribute__ ((format(printf, 2, 3)))
			{
				int nrOfChars;
				va_list variableList;
				
				va_start(variableList, format);
				nrOfChars = vfprintf(this->file, format, variableList);
				va_end(variableList);
				
				return nrOfChars;
				
			} // Print
			
			/**
			 * Provides functionality identical to standard C 'scanf'.
			 * @return Number of inputs assigned.
			 */
			inline int Scan(const char *format, ...) __attribute__ ((format(scanf, 2, 3)))
			{
				int inputsAssigned;
				va_list variableList;
				
				va_start(variableList, format);
				inputsAssigned = vfscanf(this->file, format, variableList);
				va_end(variableList);
				
				return inputsAssigned;
				
			} // Scan
			
			/** 
			 * Reads at most 'maxSize' - 1 bytes from stream, until newline
			 * character was encountered, and stores the characters in the buffer
			 * pointed by 's'.
			 * @param[out] s pointer to string buffer.
			 * @param[in] maxSize maximum number of bytes written to it (terminating NULL included).
			 * @return 'true' on success
			 */
			inline bool ReadLine(str_t s, uint16_t maxSize)
 			{
				if (fgets(s, maxSize, this->file) );
					return true;
				
				return false;
				
			} // ReadLine
			
			/**
			 * Writes one character to output stream.
			 * @param[in] c The character itself.
			 */
			TextStream<FileIO, OutputBuffer, InputBuffer> &operator << (char c)
			{
				fputc(c, this->file);
				return *this;
				
			} // operator << (char)
			
			/**
			 * Writes string to output stream.
			 * @param[in] s Pointer to byte array.
			 */
			TextStream<FileIO, OutputBuffer, InputBuffer> &operator << (str_t s)
			{
				fputs(s, this->file);
				return *this;
				
			} // operator << (char *)
			
			/**
			 * Writes constant string expression to output stream.
			 * @param[in] s Pointer to byte array.
			 */
			TextStream<FileIO, OutputBuffer, InputBuffer> &operator << (cstr_t s)
			{
				fputs(s, this->file);
				return *this;
				
			} // operator << (const char *)
			
			/**
			 * Writes string to output stream.
			 * @param[in] s String to write.
			 */
			TextStream<FileIO, OutputBuffer, InputBuffer> &operator << (AVRCpp::Collection::BaseString &s)
			{
				fwrite(s.Get(), s.GetLength(), 1, this->file);
				return *this;
				
			} // operator << (BaseString &)
			
			/**
			 * Writes a signed integer in human readable way to output stream.
			 * @param[in] i Integer value to convert to string and write to stream.
			 * @see TextStream::Print
			 */
			TextStream<FileIO, OutputBuffer, InputBuffer> &operator << (int i)
			{
				fprintf(this->file, "%i", i);
				return *this;
				
			} // operator << (int)
			
			/**
			 * Writes an unsigned integer in human readable way to output stream.
			 * @param[in] i Integer value to convert to string and write to stream.
			 * @see TextStream::Print
			 */
			TextStream<FileIO, OutputBuffer, InputBuffer> &operator << (unsigned int u)
			{
				fprintf(this->file, "%u", u);
				return *this;
				
			} // operator << (unsigned int)

			/**
			 * Writes a floating point variable in human readable way to output stream.
			 * @param[in] d Floating point value to convert to string and write to stream.
			 * @see TextStream::Print
			 */
			TextStream<FileIO, OutputBuffer, InputBuffer> &operator << (double f)
			{
				fprintf(this->file, "%f", f);
				return *this;
				
			} // operator << (double)
			
			/**
			 * Reads one character from input stream.
			 * @param[out] c Read character is stored in 'c'. EOF is returned when stream is empty.
			 */
			TextStream<FileIO, OutputBuffer, InputBuffer> &operator >> (char &c)
			{
				c = fgetc(this->file);
				return *this;
				
			} // operator >> (char &)
			
			/**
			 * Reads string form input stream. White characters are trimmed from front and back.
			 * @param[out] s Read string is written to this string container. Returned string
			 * cannot contain white characters.
			 */
			TextStream<FileIO, OutputBuffer, InputBuffer> &operator >> (AVRCpp::Collection::BaseString &s)
			{
				int c;
				
				s.SetLength(0);
				
				do
				{
					c = fgetc(this->file);
				}
				while (c == ' ' || c == '\t' || c == '\0' || c == '\r' || c == '\n');
				
				while (true)
				{
					if (c == EOF)
						return *this;
					
					if (c == ' ' || c == '\t' || c == '\0' || c == '\r' || c == '\n')
					{
						break;
					}
					
					s += c;
					
					c = fgetc(this->file);
				}
				
				do
				{
					c = fgetc(this->file);
				}
				while (c == ' ' || c == '\t' || c == '\0' || c == '\r' || c == '\n');
				
				ungetc(c, this->file);
				
				return *this;
				
			} // operator >> (BaseString &)
			
			/**
			 * Reads a signed integer value form input stream.
			 * @param[out] i Read value is written to this variable.
			 * @see TextStream::Scan
			 */
			TextStream<FileIO, OutputBuffer, InputBuffer> &operator >> (int &i)
			{
				Scan("%i", &i);
				return *this;
				
			} // operator >> (int &)
			
			/**
			 * Reads an unsigned integer value form input stream.
			 * @param[out] i Read value is written to this variable.
			 * @see TextStream::Scan
			 */
			TextStream<FileIO, OutputBuffer, InputBuffer> &operator >> (unsigned int &u)
			{
				Scan("%u", &u);
				return *this;
				
			} // operator >> (unsigned int &)

			/**
			 * Reads a floating point value form input stream.
			 * @param[out] d Read value is written to this variable.
			 * @see TextStream::Scan
			 */
			TextStream<FileIO, OutputBuffer, InputBuffer> &operator >> (double &d)
			{
				Scan("%d", &d);
				return *this;
				
			} // operator >> (double &)
			
		}; // template class TextStream 
		
	} // namespace Stream
	
} // namespace AVRCpp

#endif // ifndef __AVR_CPP_STREAM_H__

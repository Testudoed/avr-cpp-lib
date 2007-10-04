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

#include "collection/CppString.h"

namespace AVRCpp
{
	namespace Stream
	{
		typedef FILE *File;
		
		
		template <class USARTClass> struct FileIOForUSART
		{
			
			static int Write(char c, File file)
			{
				if (c == '\n') USARTClass::Write('\r');
				USARTClass::Write(c);
				
				return 0;
				
			} // WriteToStream
			
			static int Read(File file)
			{
				uint8_t c = 0;
				
				USARTClass::Read(c);
				
				return c;
				
			} // ReadFromStream
			
		}; // template struct FileForUSART
		
		
		struct FileIOForEEPROM
		{
			static int Write(char c, File file)
			{
				EEPROM::Write(c);
				
				return 0;
				
			} // WriteToStream
			
			static int Read(File file)
			{
				char c = 0;
				
				return EEPROM::Read(c);
				
				return 0;
				
			} // ReadFromStream
			
		}; // struct FileForEEPROM
		
		
		template <class FileIO> File OpenFile()
		{
			File file = fdevopen(FileIO::Write, FileIO::Read);
			
			file->udata = 0;
			
			return file;
			
		} // OpenFile
		
		
		void CloseFile(File file)
		{
			fclose(file);
			
		} // CloseFile
		
		/**
		 * Input and output stream for text.
		 */
		template <class FileIO> class TextStream
		{
		protected:
			
			/// Pointer to file associated with this stream.
			File file;
			
		public:
			
			/**
			 * Creates a file associated with this stream.
			 */
			TextStream<FileIO>()
			{
				file = fdevopen(FileIO::Write, FileIO::Read);
				file->udata = this;
				
			} // Stream CONSTRUCTOR
			
			/**
			 * Closes the file associated with this stream.
			 */
			~TextStream<FileIO>()
			{
				fclose(file);
				
			} // Stream DESTRUCTOR
			
			/**
			 * Returns file associated with this stream.
			 */
			inline File GetFile() { return file; };
			
			/**
			 * Provides functionality identical to standard C 'printf'.
			 * @return Number of bytes written or 'EOF'.
			 */
			inline int Print(const char *format, ...) __attribute__ ((format(printf, 2, 3)))
			{
				int nrOfChars;
				va_list variableList;
				
				va_start(variableList, format);
				nrOfChars = vfprintf(file, format, variableList);
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
				inputsAssigned = vfscanf(file, format, variableList);
				va_end(variableList);
				
				return inputsAssigned;
				
			} // Scan
			
			/**
			 * Writes one character to output stream.
			 * @param[in] c The character itself.
			 */
			TextStream<FileIO> &operator << (char c)
			{
				fputc(c, file);
				return *this;
				
			} // operator << (char)
			
			/**
			 * Writes string to output stream.
			 * @param[in] s Pointer to bytes array.
			 */
			TextStream<FileIO> &operator << (str_t s)
			{
				for (char *p = s; *p; p++)
					fputc(*p, file);
				
				return *this;
				
			} // operator << (char *)
			
			/**
			 * Writes string to output stream.
			 * @param[in] s String to write.
			 */
			TextStream<FileIO> &operator << (AVRCpp::Collection::BaseString &s)
			{
				fwrite(s.Get(), s.GetLength(), 1, file);
				
				return *this;
				
			} // operator << (BaseString &)
			
			/**
			 * Writes an integer in human readable way to output stream.
			 * @param[in] i Integer value to convert to string and write to stream.
			 */
			TextStream<FileIO> &operator << (int i)
			{
				AVRCpp::Collection::StaticString<7> s;
				
				s.FromInt(i);
				fwrite(s.Get(), s.GetLength(), 1, file);
				
				return *this;
				
			} // operator << (int)
			
			/**
			 * Writes a floating point variable in human readable way to output stream.
			 * @param[in] d Floating point value to convert to string and write to stream.
			 */
			TextStream<FileIO> &operator << (double d)
			{
				AVRCpp::Collection::StaticString<128> s;
				
				s.FromDouble(d);
				fwrite(s.Get(), s.GetLength(), 1, file);
				
				return *this;
				
			} // operator << (double)
			
			/**
			 * Reads one character from input stream.
			 * @param[out] c Read character is stored in 'c'. EOF is returned when stream is empty.
			 */
			TextStream<FileIO> &operator >> (char &c)
			{
				c = fgetc(file);
				
				return *this;
				
			} // operator >> (char &)
			
			/**
			 * Reads string form input stream. White characters are trimmed from front and back.
			 * @param[out] s Read string is written to this string container. Returned string
			 * cannot contain white characters.
			 */
			TextStream<FileIO> &operator >> (AVRCpp::Collection::BaseString &s)
			{
				int c;
				
				s.SetLength(0);
				
				do
				{
					c = fgetc(file);
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
					
					c = fgetc(file);
				}
				
				do
				{
					c = fgetc(file);
				}
				while (c == ' ' || c == '\t' || c == '\0' || c == '\r' || c == '\n');
				
				ungetc(c, file);
				
				return *this;
				
			} // operator >> (BaseString &)
			
			/**
			 * Reads an integer value form input stream.
			 * @param[out] i Read value is written to this variable.
			 * @note Stream::Scan function is recommended to use
			 * instead of >> operator in this context.
			 */
			TextStream<FileIO> &operator >> (int &i)
			{
				Scan("%i", &i);
				return *this;
				
			} // operator >> (int &)
			
			/**
			 * Reads a floating point value form input stream.
			 * @param[out] d Read value is written to this variable.
			 * @note Stream::Scan function is recommended to use
			 * instead of >> operator in this context.
			 */
			TextStream<FileIO> &operator >> (double &d)
			{
				Scan("%d", &d);
				return *this;
				
			} // operator >> (double &)
			
		}; // template class TextStream 
		
	} // namespace Stream
	
} // namespace AVRCpp

#endif // ifndef __AVR_CPP_STREAM_H__

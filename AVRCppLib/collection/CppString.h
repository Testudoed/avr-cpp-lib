/**********************************************************************************************************************\

	C++ library for Atmel AVR microcontrollers
	Copyright (C) 2007 Lauri Kirikal, Mikk Leini, MTÜ TTÜ Robotiklubi

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

#ifndef __AVR_CPP_STRING_H__
#define __AVR_CPP_STRING_H__

#include "BaseString.h"

namespace AVRCpp
{
	namespace Collection
	{
		
		template <uint16_t capacity> class StaticString : public BaseString
		{
		private:
			
			bool EnsureCapacity(uint16_t required);
			
		protected:
			
			char content[capacity];
			
		public:
			
			StaticString<capacity>() :									BaseString() {}
			StaticString<capacity>(const StaticString<capacity> &ms) :	BaseString(ms) {}
			StaticString<capacity>(const BaseString &ms) :				BaseString(ms) {}
			StaticString<capacity>(cstr_t source) :						BaseString(source) {}
			StaticString<capacity>(cstr_t source, uint16_t n) :			BaseString(source, n) {}
			StaticString<capacity>(int i) :								BaseString(i) {}
			StaticString<capacity>(double d) :							BaseString(d) {}
			
		}; // class StaticString
		
		
		template <uint16_t blockSize = 1> class DynamicString : public BaseString
		{
		private:
			
			uint16_t GetBlockSize(uint16_t required);
			bool EnsureCapacity(uint16_t required);
			
		public:
			
			DynamicString<blockSize>() :									BaseString() {}
			DynamicString<blockSize>(const DynamicString<blockSize> &ms) :	BaseString(ms) {}
			DynamicString<blockSize>(const BaseString &ms) :				BaseString(ms) {}
			DynamicString<blockSize>(cstr_t source) :						BaseString(source) {}
			DynamicString<blockSize>(cstr_t source, uint16_t n) :			BaseString(source, n) {}
			DynamicString<blockSize>(int i) :								BaseString(i) {}
			DynamicString<blockSize>(double d) :							BaseString(d) {}
			~DynamicString<blockSize>() { if (me) free(me); }
			
		}; // class DynamicString
		
		
		template <> class DynamicString<1> : public BaseString
		{
		private:
			
			bool EnsureCapacity(uint16_t required);
			
		public:
			
			DynamicString<1>() :								BaseString() {}
			DynamicString<1>(const DynamicString<1> &ms) :		BaseString(ms) {}
			DynamicString<1>(const BaseString &ms) :			BaseString(ms) {}
			DynamicString<1>(cstr_t source) :					BaseString(source) {}
			DynamicString<1>(cstr_t source, uint16_t n) :		BaseString(source, n) {}
			DynamicString<1>(int i) :							BaseString(i) {}
			DynamicString<1>(double d) :						BaseString(d) {}
			~DynamicString<1>() { if (me) free(me); }
			
		}; // class StaticString SPECIALISATION
		
		
		/// The most typical string type
		typedef DynamicString<1> String;
		
		
		template <uint16_t blockSize> uint16_t DynamicString<blockSize>::GetBlockSize(uint16_t required)
		{
			uint16_t k = required / blockSize + 1;
			return k * blockSize;
			
		} // GetBlockSize
		
		
		template <uint16_t blockSize> bool DynamicString<blockSize>::EnsureCapacity(uint16_t required)
		{
			
			if (me)
			{
				if (size < required)
				{
					required = GetBlockSize(required);
					me = StrRealloc(me, required);
				}
				else
					return true;
			}
			else
			{
				required = GetBlockSize(required);
				me = StrAlloc(required);
			}
			
			if (me)
			{
				size = required;
				return true;
			}
			
			size = 0;
			length = 0;
			return false;
			
		} // EnsureCapasity


		template <uint16_t capacity> bool StaticString<capacity>::EnsureCapacity(uint16_t required)
		{
			if (required > capacity)
				return false;
			
			me = content;
			
			return true;
			
		} // EnsureCapasity
		
	} // namespace Collection
	
} // namespace AVRCpp

#endif // ifndef __AVR_CPP_STRING_H__


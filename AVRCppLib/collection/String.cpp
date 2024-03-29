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

#include "CppString.h"

using namespace AVRCpp::Collection;

bool DynamicString<1>::EnsureCapacity(uint16_t required)
{
	if (me)
	{
		if (size < required)
			me = StrRealloc(me, required);
		else
			return true;
	}
	else
		me = StrAlloc(required);
	
	if (me)
	{
		size = required;
		return true;
	}

	size = 0;
	length = 0;
	return false;

} // EnsureCapasity


int BaseString::Print(uint16_t maxLength, cstr_t format, ...)
{
	if (!EnsureCapacity(maxLength) ) return EOF;
	
	{
		int nrOfChars;
		va_list variableList;
		
		va_start(variableList, format);
		nrOfChars = vsnprintf(me, maxLength, format, variableList);
		va_end(variableList);
		
		if (nrOfChars == EOF)
		{
			*me = '\0';
		}
		else
		{
			length = nrOfChars;
		}

		return nrOfChars;
	}


} // Print


int BaseString::Print(uint16_t index, uint16_t maxLength, cstr_t format, ...)
{
	if (index > length) return EOF;
	if (!EnsureCapacity(index + maxLength) ) return EOF;
	
	{
		int nrOfChars;
		va_list variableList;
		
		va_start(variableList, format);
		nrOfChars = vsnprintf(me + index, maxLength, format, variableList);
		va_end(variableList);
		
		if (nrOfChars == EOF)
		{
			me[index] = '\0';
		}
		else
		{
			length = index + nrOfChars;
		}

		return nrOfChars;
	}

} // Print


bool BaseString::JoinFromDouble(double d)
{
	if (!EnsureCapacity(length + 128) )
		return false;

	{
		int result = DoubleToString(me + length, d);

		if (result == EOF)
		{
			me[length] = '\0';
			return false;
		}

		length += (uint16_t)result;
	}

	return true;

} // JoinFromDouble


bool BaseString::SetLength(uint16_t newLength)
{
	if (newLength < length)
	{
		me[newLength] = '\0';
		length = newLength;
		return true;
	}
	return false;

} // SetLength


bool BaseString::FromDouble(double d)
{
	if (!EnsureCapacity(128) )
		return false;

	{
		int result = DoubleToString(me, d);

		if (result == EOF)
		{
			me[0] = '\0';
			return false;
		}

		length = (uint16_t)result;
	}

	return true;

} // FromDouble


bool BaseString::Join(char_t c)
{
	if (!EnsureCapacity(length + 2) )
		return false;
	
	me[length] = c;
	me[++length] = '\0';

	return true;

} // Join


bool BaseString::SubJoin(cstr_t value, uint16_t valueLength)
{
	uint16_t newLength = valueLength + length;

	if (!EnsureCapacity(newLength + 1) )
		return false;
	
	if (length)
		StrJoin(me, value);
	else
		StrCopy(me, value);
	
	length = newLength;

	return true;

} // SubJoin 1


bool BaseString::SubJoin(cstr_t value, uint16_t n, uint16_t valueLength)
{
	uint16_t minLength = valueLength > n ? n : valueLength;
	uint16_t newLength = minLength + length;

	if (!EnsureCapacity(newLength + 1) )
		return false;
	
	StrNCopy(me + length, value, minLength);
	me[newLength] = '\0';
	
	length = newLength;

	return true;

} // SubJoin 2


bool BaseString::SubCopy(cstr_t source, uint16_t sourceLength)
{
	if (sourceLength == 0 && !me)
		return true;

	if (!EnsureCapacity(sourceLength + 1) )
		return false;

	StrCopy(me, source);
	length = sourceLength;
	return true;

} // SubCopy


bool BaseString::Copy(cstr_t source, uint16_t n)
{
	if (n == 0 && !me)
		return true;

	if (!EnsureCapacity(n + 1) )
		return false;

	StrNCopy(me, source, n);
	me[n] = '\0';

	length = n;
	return true;

} // Copy


uint16_t BaseString::SubFind(uint16_t index, cstr_t value, uint16_t valueLength) const
{
	
	if (valueLength)
	{
		str_t pointer = StrFind(Get(index), value);
		
		if (pointer)
			return (uint16_t)(pointer - me);
		
	}

	return NotFound;
	
} // SubFind

uint16_t BaseString::SubIFind(uint16_t index, cstr_t value, uint16_t valueLength) const
{
	
	if (valueLength)
	{
		str_t pointer = StrFind(Get(index), value);
		
		if (pointer)
			return (uint16_t)(pointer - me);
		
	}

	return NotFound;
	
} // SubIFind


bool BaseString::EndsWith(cstr_t value) const
{
	uint16_t valueLength = StrLength(value);
	if (valueLength > length)
		return false;
	
	return StrCompare(Get(length - valueLength), value) == 0;
	
} // EndsWith 1


bool BaseString::EndsWith(const BaseString &value) const
{
	if (value.GetLength() > length)
		return false;
	
	return StrCompare(Get(length - value.GetLength() ), value.Get() ) == 0;
	
} // EndsWith 2


bool BaseString::IEndsWith(cstr_t value) const
{
	uint16_t valueLength = StrLength(value);
	
	if (valueLength > length)
		return false;
	
	return StrICompare(Get(length - valueLength), value) == 0;
	
} // IEndsWith 1


bool BaseString::IEndsWith(const BaseString &value) const
{
	if (value.GetLength() > length)
		return false;
	
	return StrICompare(Get(length - value.GetLength() ), value.Get() ) == 0;
	
} // IEndsWith 2


bool BaseString::SubReplace(uint16_t begin, uint16_t end, cstr_t replaceWith, uint16_t replaceWithLength)
{
	if (begin > length || end > length || begin > end)
		return false;
	
	if (replaceWithLength)
	{
		if (end == length)
		{
			uint16_t length1 = begin + replaceWithLength;
			
			if (!EnsureCapacity(length1 + 1) )
				return false;
			
			StrCopy(me + begin, replaceWith);
			length = length1;
		}
		else
		{
			String strEnd;

			strEnd.Copy(me + end);
			uint16_t length1 = begin + replaceWithLength;
			uint16_t length2 = length1 + strEnd.GetLength();
			
			if (!EnsureCapacity(length2 + 1) )
				return false;
			
			StrCopy(me + begin, replaceWith);
			StrCopy(me + length1, strEnd.Get() );
			length = length2;
		}
	}
	else if (begin != end)
	{
		if (end == length)
		{
			me[begin] = '\0';
			length = begin;
		}
		else
		{
			StrCopy(me + begin, me + end);
			length -= end - begin;
		}
	}

	return true;

} // Replace



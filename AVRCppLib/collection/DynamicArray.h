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

#ifndef __AVR_CPP_DYNAMIC_ARRAY_H__
#define __AVR_CPP_DYNAMIC_ARRAY_H__

#include <stdlib.h>
#include "BaseArray.h"


/**********************************************************************************************************************\

	AVRCppLib collection objects

\**********************************************************************************************************************/

namespace AVRCpp
{
	namespace Collection
	{


/**********************************************************************************************************************\

	General dynamic array class
	
	Holds a list of custom typed items. Supports virtually unlimited amount of item adding, inserting and removing.
	Possible to define maximum size limit, reserved and block size.

\**********************************************************************************************************************/

template <
		typename DataType,
		typename SizeType,
		SizeType array_capacity = (1 << (sizeof(SizeType) << 3)) - 1,
		SizeType reserve_size = 8,
		SizeType block_size = 8>

class DynamicArray : public BaseArray<DataType, SizeType, DataType*>
{
	private:
	
		typedef BaseArray<DataType, SizeType, DataType*> Parent;

	protected:
	
		SizeType allocated_size;

		/**
		 *	Try resize the array
		 */
		bool Realloc(SizeType size)
		{
			// Don't allow smaller allocation than reserve size
			if (size < reserve_size) size = reserve_size;

			// If current allocation equals new size then skip
			if (this->allocated_size == size) return true;

			// Try reallocate
			DataType *newData = (DataType *) realloc(this->data, sizeof(DataType) * size);
			if (newData)
			{
				this->data = newData;
				this->allocated_size = size;
				return true;
			}
			
			return false;
		}

	public:

		/**
		 *	Constructor
		 */
		DynamicArray() : Parent()
		{
			this->data				= (DataType *) malloc(reserve_size);
			this->allocated_size	= 0;
		}

		/**
		 *	Destructor
		 */
		~DynamicArray()
		{
			free(this->data);
		}

		/**
		 *	Resize array to certain length
		 */
		bool Resize(const SizeType size, const DataType &values = DataType())
		{
			// If new size is larger than maximum capacity then abort
			if (size >= array_capacity) return false;

			if (!this->Realloc(size)) return false;

			return Parent::Resize(size, values);
		}

		/**
		 *  Add item to the end of array
		 */
		bool Add(const DataType &value)
		{
			// If new size is larger than maximum capacity then abort
			if (this->current_size >= array_capacity) return false;

			// If new size is larger than allocated size then increase memory size by one block
			if (this->current_size >= this->allocated_size)
			{
				if (!this->Realloc(this->current_size + block_size)) return false;
			}

			return Parent::Add(value);
		}

		/**
		 *  Insert item to the pos
		 */
		bool Insert(const DataType &value, const SizeType pos)
		{
			// If new size is larger than maximum then abort
			if (this->current_size >= array_capacity) return false;

			// Check constraints
			if (pos > this->current_size) return false;

			// If new size is larger than allocated size then increase memory size by one block
			if (this->current_size >= this->allocated_size)
			{
				if (!this->Realloc(this->current_size + block_size)) return false;
			}

			return Parent::Insert(value, pos);
		}

		/**
		 *	Remove item at pos
		 */
		bool Erase(const SizeType pos)
		{
			if (!Parent::Erase(pos)) return false;
			
			// If current size is one block size smaller than allocated size then decrease memory size
			if (this->current_size <= this->allocated_size - block_size)
			{
				// No matter the realloc succeeds or not, fictios size will still be smaller
				this->Realloc(this->allocated_size - block_size);
			}

			return true;
		}
		
		/**
		 *	Remove specified item(s)
		 */
		bool Remove(const DataType &item)
		{
			if (!Parent::Remove(item)) return false;
			
			// If current size is one block size smaller than allocated size then decrease memory size
			if (this->current_size <= this->allocated_size - block_size)
			{
				// No matter the realloc succeeds or not, fictios size will still be smaller
				this->Realloc(this->allocated_size - block_size);
			}

			return true;
		}

		/**
		 *	Clear array
		 */
		inline bool Clear()
		{
			if (!Parent::Clear()) return false;
			
			// No matter the realloc succeeds or not, fictios size will still be 0
			this->Realloc(0);

			return true;
		}
		
		/**
		 *	Return array capacity
		 */
		inline SizeType GetCapacity(void)
		{
			return array_capacity;
		}

		/**
		 *	Return whether the array is in full capacity or not
		 */
		inline bool IsFull(void)
		{
			return (this->current_size == array_capacity);
		}
		
}; // class DynamicArray


/**********************************************************************************************************************/

	} // namespace Collection

} // namespace AVRCpp


/**********************************************************************************************************************/

#endif // ifndef __AVR_CPP_DYNAMIC_ARRAY_H__

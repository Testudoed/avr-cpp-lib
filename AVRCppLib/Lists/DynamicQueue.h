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

#ifndef __AVR_CPP_DYNAMIC_QUEUE_H__
#define __AVR_CPP_DYNAMIC_QUEUE_H__

#include "BaseQueue.h"


/**********************************************************************************************************************\

	Dynamic queue (FIFO) class

	Supports item pushing to the end of queue and popping out first one. Fixed size.

\**********************************************************************************************************************/

template <
		typename DataType,
		typename SizeType,
		SizeType queue_capacity = (1 << (sizeof(SizeType) << 3)) - 1,
		SizeType reserve_size = 8,
		SizeType block_size = 8>
		
class DynamicQueue : public BaseQueue<DataType, SizeType, DataType*>
{

	public:

		/**
		 *	Constructor
		 */
		DynamicQueue()
		{
			DynamicQueue<DataType, SizeType, DataType*>();

			this->data				= (DataType *) malloc(reserve_size);
			this->allocated_size	= 0;
		}

		/**
		 *	Destructor
		 */
		~DynamicQueue()
		{
			free(this->data);
		}
}

#endif // ifndef __AVR_CPP_DYNAMIC_QUEUE_H__

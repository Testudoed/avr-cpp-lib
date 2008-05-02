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

#ifndef __AVR_CPP_BASE_QUEUE_H__
#define __AVR_CPP_BASE_QUEUE_H__


/**********************************************************************************************************************\

	AVRCppLib collection objects

\**********************************************************************************************************************/

namespace AVRCpp
{
	namespace Collection
	{


/**********************************************************************************************************************\

	Base queue (FIFO) class
	
	This class should never be instantized directly!

\**********************************************************************************************************************/

template <typename DataType, typename SizeType, typename ListType> class BaseQueue
{
	protected:
	
		ListType data;
		SizeType write_pointer;
		SizeType read_pointer;		
		bool is_full;		

	public:

		/**
		 *	Clears this queue.
		 */
		inline void Clear()
		{
			this->write_pointer	= 0;
			this->read_pointer  = 0;
			this->is_full 		= false;
		}
		
		/**
		 *	Constructor
		 */
		BaseQueue()
		{
			Clear();			
		}

		/**
		 *	Returns whether the queue is empty or not.
		 */
		inline bool IsEmpty(void)
		{
			return ((this->read_pointer == this->write_pointer) && (!this->is_full));
		}
		
		/**
		 *	Returns whether the queue is full or not.
		 */
		inline bool IsFull(void)
		{
			return this->is_full;
		}
		
		/**
		 *  Returns reference of first item.
		 *  NB! For speed purposes does not check constraints!
		 */
		inline DataType &Front()
		{
			return this->data[this->read_pointer];
		}
		
		/**
		 *  Returns reference of last item.
		 *  NB! For speed purposes does not check constraints!
		 */
		inline DataType &Back()
		{
			return this->data[this->write_pointer];
		}
		
		/**
		 *  Push item to the queue
		 */
		bool Push(const DataType &value) { return false; }
		
		/**
		 *  Pop item out of the queue
		 */
		bool Pop(DataType &value = 0) {	 return false; }

}; // class BaseQueue


/**********************************************************************************************************************/

	} // namespace Collection

} // namespace AVRCpp


/**********************************************************************************************************************/

#endif // ifndef __AVR_CPP_BASE_QUEUE_H__

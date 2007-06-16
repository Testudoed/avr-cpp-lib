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

#ifndef __AVR_CPP_BASE_BIT_QUEUE_H__
#define __AVR_CPP_BASE_BIT_QUEUE_H__


/**********************************************************************************************************************\

	Base bit queue (FIFO) class

	This class should never be instantized directly!
	Bit direction: MSB first, LSB last

\**********************************************************************************************************************/

template <class BaseQueue>

class BaseBitQueue : protected BaseQueue
{
	protected:
		uint8_t bit_write_mask;
		uint8_t bit_read_mask;

	public:

		/**
		 *	Constructor
		 */
		BaseBitQueue()
		{
			this->bit_write_mask	= 0;
			this->bit_read_mask 	= 0;
		}

		/**
		 *	Return whether the queue is empty or not
		 */
		inline bool IsEmpty(void)
		{
			return  BaseQueue::IsEmpty();
		}

		/**
		 *	Return whether the queue is full or not
		 */
		inline bool IsFull(void)
		{
			return (BaseQueue::IsFull() && (this->bit_write_mask == this->bit_read_mask));
		}

		/**
		 *	Clear queue
		 */
		inline void Clear()
		{
		    BaseQueue::Clear();
			this->bit_write_mask	= 0;
			this->bit_read_mask 	= 0;
		}

		/**
	     *  Push bit item to the queue
	     */
		bool Push(const uint8_t &value)
		{
		    // Circle complete ?
		    if (this->bit_write_mask == 0)
		    {
			    // Queue full ?
			    if (BaseQueue::IsFull()) return false;
		    
		        // Make new byte in queue
		        if (!BaseQueue::Push(0)) return false;
		        
		        // Do rotational shifting
		        this->bit_write_mask = 0x80;
		    }

			// Put bit the the byte queue
			if (value)
			{
				BaseQueue::Back() |= this->bit_write_mask;
			}
			
			// Shift to right
			this->bit_write_mask >>= 1;
			
			return true;
		}
		
		/**
		 *  Pop bit item out of the queue
		 */
		bool Pop(uint8_t &value)
		{
		    // Queue empty ?
		    if (BaseQueue::IsEmpty()) return false;
		    
		    // Writing and reading with same byte ?
		    if ((BaseQueue::Size() == 1) && (this->bit_read_mask <= this->bit_write_mask)) return false;
		    
		    // Circle complete ?
			if (this->bit_read_mask == 0)
			{
				// Do rotational shifting
				this->bit_read_mask = 0x80;
			}

			// Get bit from the byte queue
			value = ((BaseQueue::Front() & this->bit_read_mask) ? 1 : 0);
			
			// Shift to right
			this->bit_read_mask >>= 1;
			
			// Circle complete ?
			if (this->bit_read_mask == 0)
			{
				// Pop byte from the queue
				if (!BaseQueue::Pop()) return false;
		 	}
			
			return true;
		}

}; // class BaseBitQueue


/**********************************************************************************************************************/

#endif // ifndef __AVR_CPP_BASE_BIT_QUEUE_H__

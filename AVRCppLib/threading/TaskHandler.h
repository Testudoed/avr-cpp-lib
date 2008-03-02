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

#ifndef __AVR_CPP_TASK_HANDLER_H__
#define __AVR_CPP_TASK_HANDLER_H__


/**********************************************************************************************************************\

	AVRCppLib threading objects

\**********************************************************************************************************************/

namespace AVRCpp
{
	namespace Threading
	{


/**********************************************************************************************************************\

	TaskHandler class

	This is an extendable class for different tasks. Each task can have sub-tasks.

\**********************************************************************************************************************/

class TaskHandler {

	private:
	
		volatile uint32_t elapsedMilliseconds;

	public:
				
		/**
		 *	Contructor
		 */
		TaskHandler(void)
		{
			elapsedMilliseconds = 0;
		}

		/**
		 *	Millisecond tick call function
		 */
		void MillisecondTick(void)
		{
			elapsedMilliseconds++;
		}

		/**
		 *	Return elapse milliseconds
		 */
		volatile uint32_t GetElapsedMilliseconds(void)
		{
			return elapsedMilliseconds;
		}

}; // class TaskHandler


/**********************************************************************************************************************/

	} // namespace Threading
	
} // namespace AVRCpp



/**********************************************************************************************************************/

#endif // ifndef __AVR_CPP_TASK_HANDLER_H__


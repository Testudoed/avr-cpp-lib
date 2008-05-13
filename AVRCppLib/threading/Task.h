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

#ifndef __AVR_CPP_TASK_H__
#define __AVR_CPP_TASK_H__

#include "../collection/DynamicArray.h"
#include "TaskHandler.h"


/**********************************************************************************************************************\

	AVRCppLib threading objects

\**********************************************************************************************************************/

namespace AVRCpp
{
	namespace Threading
	{


/**********************************************************************************************************************\

	Task class

	This is an extendable class for different tasks. Each task can have sub-tasks.

\**********************************************************************************************************************/

using namespace Collection;

enum TaskPeriodType
{
	TaskPeriodMilliseconds	= 1,
	TaskPeriodLoopCounts	= 2
};

class Task;

class Task
{
	private:
	
		TaskHandler *taskHandler;
		Task *overrideTask;
   		DynamicArray<Task*, uint8_t, 255, 1, 1> subTasks;	// Here could be 0 reserve, but compiler whines 

		TaskPeriodType pausePeriodType;
		uint32_t pausePeriodUnits;
		uint32_t pausePeriodCounter;

	public:
		
		/**
		 * User defined task starter function
		 */		 		
		virtual void Start(void)
		{
			// nothing
		}

	protected:

		/**
		 * User class internal worker function
		 * Return true or false depending on whether to continue running or not
		 */
		virtual bool Tick(void)
		{
			return true;
		}	

	public:

		/**
		 * Constructor
		 */
		Task(void)
		{
			overrideTask       = 0;						
			pausePeriodUnits   = 0;
			pausePeriodCounter = 0;
		}

		/**
		 * Destructor
		 */
		virtual ~Task(void)
		{
		}

		/**********************************************************************/

		/**
		 *	Set task-handler
		 *
		 *	@param handler Address of task-handler object
		 *	@return Sucess
		 */
		bool SetTaskHandler(TaskHandler &handler)
		{		
			taskHandler = &handler;

			return true;
		}
		
		/**
		 *	Set task-handler
		 *
		 *	@param handler Pointer to task-handler object
		 *	@return Sucess
		 */
		bool SetTaskHandler(TaskHandler *handler)
		{		
			taskHandler = handler;

			return true;
		}

		/**
		 *	Replace current task with new one
		 * 	
		 *	@param pTask Address of task object
		 *	@return	Success
		 */
		bool SetOverrideTask(Task &task)
		{
			overrideTask = &task;
			overrideTask->SetTaskHandler(taskHandler);
			overrideTask->Start();

			return true;
		}
		
		/**
		 *	Replace current task with new one
		 * 	
		 *	@param pTask Pointer to task object
		 *	@return	Success
		 */
		bool SetOverrideTask(Task *task)
		{
			overrideTask = task;
			overrideTask->SetTaskHandler(taskHandler);
			overrideTask->Start();

			return true;
		}

		/**
		 *	Clear overriding task
		 *	
		 *	@return Success
		 */
		bool ClearOverrideTask(void)
		{
			overrideTask = 0;

			return true;
		}

		/**
		 *	Add sub-task
		 *
		 *	@param subTask Address of sub-task object
		 *	@return Success
		 */
		bool AddSubTask(Task &subTask)
		{
			subTask.SetTaskHandler(*taskHandler);
			subTask.Start();
			
			return subTasks.Add(&subTask);
		}
		
		/**
		 *	Add sub-task
		 *
		 *	@param subTask Pointer to sub-task object
		 *	@return Success
		 */
		bool AddSubTask(Task *subTask)
		{
			subTask->SetTaskHandler(taskHandler);
			subTask->Start();
			
			return subTasks.Add(subTask);
		}

		/**
		 *	Remove sub task by index
		 *
		 *	@param index Task index
		 *	@return Success
		 */
		bool RemoveSubTask(uint8_t index)
		{
			return subTasks.Erase(index);							
		}

		/**
		 *	Remove sub task by sub-task pointer
		 *
		 *	@param subTask Pointer to sub-task object
		 */
		bool RemoveSubTask(Task &subTask)
		{
			return subTasks.Remove(&subTask);
		}
		
		/**
		 *	Remove sub task by sub-task pointer
		 *
		 *	@param subTask Pointer to sub-task object
		 */
		bool RemoveSubTask(Task *subTask)
		{
			return subTasks.Remove(subTask);
		}

		/**********************************************************************/

		/**
		 *	Task running
		 *
		 *	@return True to continue running
		 */
		bool Run(void)
		{
			// Run override task ?
			if (overrideTask)
			{
				if (!overrideTask->Run())
				{
					ClearOverrideTask();
					Start();
				}

				return true;
			}

			// Pause ?
			if (pausePeriodUnits > 0)
			{			
				switch (pausePeriodType)
				{
					case TaskPeriodMilliseconds:
						if (taskHandler->GetElapsedMilliseconds() < pausePeriodCounter) return true;
						break;

					case TaskPeriodLoopCounts:
						if (pausePeriodCounter++ <= pausePeriodUnits) return true;
						break;
				}

				// Stop pause
				pausePeriodUnits = 0;
			}

			// Run child tasks
			for (register uint8_t index = 0; index < subTasks.GetSize(); index++)
			{
				if (!subTasks[index]->Run())
				{
					RemoveSubTask(index);
				}
			}

			return Tick();
		}

		/******************************************************************************/

		/**
		 * Stop task running for specified period
		 *
		 * @param numUnits Pause length in specified type units (default milliseconds)
		 * @param periodType Pause period type
		 */
		void Pause(uint32_t numUnits, TaskPeriodType periodType)
		{
			pausePeriodUnits = numUnits;
			pausePeriodType  = periodType;
			
			switch (pausePeriodType)
			{
				case TaskPeriodMilliseconds:
					pausePeriodCounter	= taskHandler->GetElapsedMilliseconds() + pausePeriodUnits;
					break;

				case TaskPeriodLoopCounts:					
					pausePeriodCounter	= 0;
					break;
			}
		}
		
		/**
		 * Pause task for specified time-period
		 *
		 * @param time Time in milliseconds
		 */
		void Pause(uint32_t time)
		{
			Pause(time, TaskPeriodMilliseconds);
		}
	
}; // class Task


/**********************************************************************************************************************/

	} // namespace Threading
	
} // namespace AVRCpp



/**********************************************************************************************************************/

#endif // ifndef __AVR_CPP_TASK_H__


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

#ifndef __AVR_CPP_SOFTWARE_I2C_H__
#define __AVR_CPP_SOFTWARE_I2C_H__

#include <util/delay.h>
#include "IO.h"


/**********************************************************************************************************************\

	AVRCppLib software I2C Interface

\**********************************************************************************************************************/

namespace AVRCpp
{
	namespace Internal
	{
		enum BitFlags
		{
			WriteModeFlag = 0x00,
			ReadModeFlag  = 0x01
		
		}; // enum BitFlags
	}
	
	/**
	 * Software I2C
	 *
	 * Provide 2 GroundPullOutputPins
	 */			
	template <class DataPin, class ClockPin> struct SoftwareI2C
	{
		private:
		
			/**
			 * Delay in microseconds
			 */
			static inline void DoDelay(uint8_t interval)
			{
				_delay_us(interval);
			}			
		
			/**
			 * Byte writing
			 */
			static void ByteWrite(uint8_t data)		
			{           						
				for (uint8_t i = 0; i <= 7; i++)
				{
					DataPin::PullTo(!((data >> (7 - i)) & 0x01));
					DoDelay(10);
					ClockPin::Release();
					DoDelay(10);  
					ClockPin::Pull();
					DoDelay(10);  
				}
				
				ClockPin::Release();
				DataPin::Release();
				DoDelay(10);
				ClockPin::Pull();
				DoDelay(10);
			}	
			
		public:
		
			/**
			 *  Setup in master mode
			 */
			static void SetupMaster()
			{
				DataPin::Release();
				ClockPin::Release();
				
			} // SetupMaster
					
			/**					
			 * Release bus
			 */
			static void Release()
			{						
				DataPin::Release();
				ClockPin::Release();
				
			} // Release				
						
			/**
			 * Start 
			 */
			static void Start()
			{
				DataPin::Pull();	
				DoDelay(10);
				ClockPin::Pull();		
				DoDelay(20);
				
			} // Start
			
			/**
			 * Stop 
			 */
			static void Stop()
			{
				ClockPin::Release();
				DoDelay(10);
				DataPin::Release();
				DoDelay(60);
				
			} // Stop
			
			/**
			 * Restart (instant stop and start)
			 */
			static inline void Restart()
			{
				Stop();
				Start();
				
			} // Restart				
			
			/**
			 * Select write-to-slave mode for desired slave
			 */					 
			static inline void SlaveToWriteMode(uint8_t address)
			{												
				ByteWrite((address << 1) | Internal::WriteModeFlag);					
				
			} // SlaveToWriteMode

			/**
			 * Send byte
			 */
			static inline void Write(uint8_t data)
			{					
				ByteWrite(data);				

			} // Write										

	}; // template struct SoftwareI2C
			
}; // namespace AVRCpp


/**********************************************************************************************************************/

#endif // ifndef __AVR_CPP_SOFTWARE_I2C_H__

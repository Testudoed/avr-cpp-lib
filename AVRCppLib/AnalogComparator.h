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

#ifndef __AVR_CPP_ANALOG_COMPARATOR_H__
#define __AVR_CPP_ANALOG_COMPARATOR_H__

#include "IO.h"
#include "Interrupt.h"


/**********************************************************************************************************************\

	AVRCppLib Analog Comparator
	This file relies on assumption that controls registers can vary over different devices,
	but bits meaning and order in register stays the same.

\**********************************************************************************************************************/

namespace AVRCpp
{
	namespace AnalogComparator
	{
		enum PositiveInputSource
		{
			PositiveInput    = 0x00,
			FixedBandgap     = _ACBG

		}; // enum PositiveInputSource
		
		enum NegativeInputSource
		{
			NegativeInput    = 0x00,
			MultiplexedInput = 0x01

		}; // enum NegativeInputSource		
		
		enum InterruptEvent
		{
			PolarityChange   = 0x00,
			VoltegeFall      = _ACIS1,
			VoltegeRise      = _ACIS1 | _ACIS0

		}; // enum InterruptEvent		
		
		enum TimerInputCapture
		{
			NoCapture        = 0x00,
			TimerCapture     = _ACIC

		}; // enum TimerInputCapture
				
		namespace Internal
		{			
			/**
			 *  Analog comparator very base structure
			 */
			template <class ControlRegister>
						
			struct AnalogComparatorBase
			{				
				public:
					
					/**
					 *  Setup analog comparator
					 */
					static inline void Setup (							
						PositiveInputSource positiveSource,						
						InterruptEvent event,
						TimerInputCapture timerCapturing )
					{					
						ChangeBits<ControlRegister>(_ACD | _ACBG | _ACIC | _ACIS1 | _ACIS0, positiveSource | timerCapturing | event);						
												
					} // Setup
														
					/**
					 *	Get voltage comparation result.
					 *	If positive input has higher voltage than negative, true is returned. Otherwise false is returned.				
					 */
					static inline bool Compare()
					{
						return SelectBits<ControlRegister>(_ACO);

					} // Compare

	   		}; // template struct AnalogComparatorBase
			
			/**
			 *  Analog comparator with multiplexed negative voltage input
			 */
			template <
						class ControlRegister,						
						class MultiplexerSelectionRegister,
						class MultiplexerEnableBit,
						class ADCDisableBit >
						
			struct MultiplexedInputAnalogComparatorBase : AnalogComparatorBase <ControlRegister>
			{
				public:
					
					/**
					 *  Setup analog comparator
					 */
					static inline void Setup (							
						PositiveInputSource positiveSource,
						NegativeInputSource negativeSource,
						InterruptEvent event,
						TimerInputCapture timerCapturing )
					{
						// Call base setup function
						AnalogComparatorBase<ControlRegister>::Setup(positiveSource, event, timerCapturing);

						// If negative voltage is multiplexed through ADCX pins, it is not possible to use ADC
						if (negativeSource)
						{														
							ADCDisableBit::Clear();
							MultiplexerEnableBit::Set();							
							ChangeBits<MultiplexerSelectionRegister>(0x07, 0x00);	// default to ADC0
						}
						else
						{
							MultiplexerEnableBit::Clear();
						}
					}
					
					/**
					 *  Selects negative voltage with multiplexer
					 *	\attention Only possible if multiplexer enabled
					 */
					static inline void SelectNegativeInput(uint8_t number)
					{						
						ChangeBits<MultiplexerSelectionRegister>(0x07, number);

					} // SelectNegativeInput			
			};
			
			/**
			 *  Analog comparator with multiplexed negative voltage input and digital input disabling
			 *	Extends base class, but keeps setup function the same for compatibilty reason.
			 *	Provides additional function to disable digital inputs.
			 */
			template <
						class ControlRegister,						
						class MultiplexerSelectionRegister,
						class MultiplexerEnableBit,
						class ADCDisableBit,
						class DigitalDisablePositivePin,
						class DigitalDisableNegativePin >
						
			struct DigitalDisableAnalogComparatorBase : 
			
					MultiplexedInputAnalogComparatorBase <
						ControlRegister,						
						MultiplexerSelectionRegister,
						MultiplexerEnableBit,
						ADCDisableBit >
			{
				public:
					
					/**
					 *  Digital inputs disabling
					 */
					static inline void DisableDigitalInputs (
						bool DisablePositiveInput,
						bool DisableNegativeInput
						)
					{
						DigitalDisablePositivePin::SetTo(DisablePositiveInput);
						DigitalDisableNegativePin::SetTo(DisableNegativeInput);																	

					} // DisableDigitalInputs
					
			}; // template struct DigitalDisableAnalogComparatorBase
   			
		}; // namespace Internal

	} // namespace AnalogComparator
		
}; // namespace AVRCpp


/**********************************************************************************************************************/

/* Include controller specific files */
#if defined(__AVR_AT90USB1287__)
#include "at90usb1287/AnalogComparator.h"
#elif defined(__AVR_ATmega128__)
#include "atmega128/AnalogComparator.h"
#elif defined(__AVR_ATmega64__)
#include "atmega64/AnalogComparator.h"
#elif defined(__AVR_ATmega8__)
#include "atmega8/AnalogComparator.h"
#elif defined(__AVR_ATmega8515__)
#include "atmega8515/AnalogComparator.h"
#elif defined(__AVR_ATmega88__)
#include "atmega88/AnalogComparator.h"
#elif defined(__AVR_ATmega48__)
#include "atmega48/AnalogComparator.h"
#elif defined(__AVR_ATmega168__)
#include "atmega168/AnalogComparator.h"
#elif defined(__AVR_ATmega164P__)
#include "atmega164p/AnalogComparator.h"
#elif defined(__AVR_ATmega324P__)
#include "atmega324p/AnalogComparator.h"
#elif defined(__AVR_ATmega644P__)
#include "atmega644p/AnalogComparator.h"
#elif defined(__AVR_ATmega644__)
#include "atmega644/AnalogComparator.h"
#else
#error "Device is not selected or selected device is not supported."
#endif

#endif // ifndef __AVR_CPP_ANALOG_COMPARATOR_H__

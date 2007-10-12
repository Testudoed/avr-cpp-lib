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
#include "ADC.h"


/**********************************************************************************************************************\

	AVRCppLib Analog Comparator
	This file relies on assumption that controls registers can vary over different devices,
	but bits meaning and order in register stays the same.

\**********************************************************************************************************************/

namespace AVRCpp
{
	namespace AnalogComparator
	{
		typedef AnalogToDigital::AnalogChannel MultiplexedChannel;
		
		enum PositiveInputSource
		{
			PositiveInput    = 0x00,
			FixedBandgap     = _ACBG
			
		}; // enum PositiveInputSource
		
		enum NegativeInputSource
		{
			NegativeInput    = false,
			MultiplexedInput = true
			
		}; // enum NegativeInputSource		
		
		enum InterruptEvent
		{
			PolarityChange	= 0x00,
			Fall			= _ACIS1,
			Rise			= _ACIS1 | _ACIS0
			
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
				
				/// Sets up analog comparator
				static inline void SetUp (							
						PositiveInputSource positiveSource,					
						InterruptEvent event,
						TimerInputCapture timerCapturing )
				{					
					ControlRegister::Set(positiveSource | timerCapturing | event);
					
				} // SetUp
				
				/**
				 *	Returns voltage comparison result.
				 *	If positive input has higher voltage than negative, 'true' is returned, 'false' otherwise.				
				 */
				static inline bool Compare() { return SelectBits<ControlRegister>(_ACO); }
				
				/// Switches voltige supply for analog comparator on. By default analog comparator power is on.
				static inline void Enable() { ClearBits<ControlRegister>(_ACD); }
				/// Reduces power consumption. For safety reasons it disables comparator interrupt beforehand.
				static inline void Disable() { ComparatorInterrupt::Disable(); SetBits<ControlRegister>(_ACD); }
				/// Return true if analog comparator power is on.
				static inline bool IsEnabled() { return !IsBitsSet<ControlRegister>(_ACD); }
				
				/// To set or change interrupt event.
				static inline void SetEvent(InterruptEvent event) { ChangeBits<ControlRegister>(_ACIS0 | _ACIS1, event); }
				/// To read on which event analog comparator interrupt is triggered.
				static inline uint8_t GetEvent() { return SelectBits<ControlRegister>(_ACIS0 | _ACIS1); }
				
				/**
				 *	Timer/counter input capture interrupt is used
				 *	insteed of ComparatorInterrupt after calling this function.
				 *	The number of timer/counter associated to comparator can be
				 *	found from AVR device datasheet.
				 */
				static inline void EnableInputCapure() { SetBits<ControlRegister>(_ACIC); }
				/**
				 *	ComparatorInterrupt is used instead of timer/counter input capture interrupt
				 *	after calling this function.
				 */
				static inline void DisableInputCapture() { ClearBits<ControlRegister>(_ACIC); }
				/// Returns 'false' if usual ComparatorInterrupt interrupt is used.
				static inline bool IsInputCaptureEnabled() {return IsBitsSet<ControlRegister>(_ACIC); }
				
				/// To use fixed bandgap instead of positive input pin.
				static inline void EnableFixedBandgap() { SetBits<ControlRegister>(_ADBG); }
				/// To use positive input pin instead of fixed bandgap.
				static inline void DisableFixedBandgap() { ClearBits<ControlRegister>(_ADBG); }
				/// Returns 'false' if positive input pin is selected.
				static inline bool IsFixedBandgapEnabled() { return IsBitsSet<ControlRegister>(_ADBG); }
				
	   		}; // template struct AnalogComparatorBase
			

			/**
			 *  Analog comparator with multiplexed negative voltage input
			 *	Extends base class and provides new SetUp() function.
			 */
			template <
						class ControlRegister,						
						class MultiplexerEnableBit >
						
			struct MultiplexedInputAnalogComparatorBase : AnalogComparatorBase <ControlRegister>
			{
			public:

				 /**
				 *	Selects negative voltage with multiplexer
				 *	\attention Only possible if multiplexer enabled
				 */
				static inline void SelectNegativeInput(MultiplexedChannel multiplexedChannel)
				{						
					AnalogToDigital::SetChannel(multiplexedChannel);
				
				} // SelectNegativeInput

				/// Starts using analog to digital converter inputs. It disables analog to digital converter.
				static inline void EnableMultiplexer() { AnalogToDigital::Disable(); MultiplexerEnableBit::Set(); }
				/**
				 *	Ends using analog to digital converter inputs. To start using analog to digital converter
				 *	call AnalogToDigital::Enable(). Negative input source pin is used for comparator input.
				 */
				static inline void DisableMultiplexer() { MultiplexerEnableBit::Clear(); }
				/// Returns 'false' if negative input pin is selected for negative input source.
				static inline bool IsMultiplexerEnabled() { return MultiplexerEnableBit::IsSet(); }

				///	Sets up analog comparator
				static inline void SetUp (							
						PositiveInputSource positiveSource,
						NegativeInputSource negativeSource,
						InterruptEvent interruptEvent,
						TimerInputCapture timerCapturing )
				{
					// Call base setup function
					AnalogComparatorBase<ControlRegister>::Setup(positiveSource, interruptEvent, timerCapturing);
				
					// If negative voltage is multiplexed through ADCX pins, it is not possible to use ADC
					if (negativeSource)
					{														
						EnableMultiplexer();							
						SelectNegativeInput(ADC0);	// default to ADC0
					}
					else
					{
						MultiplexerEnableBit::Clear();
					}

				} // SetUp
				
			}; // template struct MultiplexedInputAnalogComparatorBase
			

			/**
			 *  Analog comparator with multiplexed negative voltage input and digital input disabling
			 *	Extends base class, but keeps SetUp() function the same for compatibility reason.
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
				
				/// Digital inputs disabling
				static inline void DisableDigitalInputs (
						bool disablePositiveInput,
						bool disableNegativeInput )
				{
					DigitalDisablePositivePin::SetTo(disablePositiveInput);
					DigitalDisableNegativePin::SetTo(disableNegativeInput);																	
					
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

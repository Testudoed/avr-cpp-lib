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

#if !(defined(__AVR_ATmega8__) || defined(__AVR_ATmega128__))
#error "Device is not selected or selected device is not supported."
#define __AVR_CPP_ADC_H__
#endif

#ifndef __AVR_CPP_ADC_H__
#define __AVR_CPP_ADC_H__

#include "IO.h"
#include "Assembler.h"

#ifndef EXCLUDE_INTERRUPT_HANDLERS

#include "Interrupt.h"

#define ADC_ns		AnalogToDigital
#define ADC_struct	ADC_ns::ConversionCompleteInterrupt

#endif // ifndef EXCLUDE_INTERRUPT_HANDLERS


/**********************************************************************************************************************\

	Analog to digital converter

\**********************************************************************************************************************/
namespace AVRCpp
{
	namespace AnalogToDigital
	{
		typedef _ADC Result;
		
		enum Reference
		{
			AREF		= 0x00,
			AVCC		= 0x40,
			InternalRef	= 0xC0
			
		}; // enum Reference
		
		enum Adjustment
		{
			AlignRight	= 0x00,
			AlignLeft	= 0x20
			
		}; // enum Adjustment
		
		enum AnalogChannel
		{
			ADC0 = 0x00,
			ADC1 = 0x01,
			ADC2 = 0x02,
			ADC3 = 0x03,
			ADC4 = 0x04,
			ADC5 = 0x05,
			ADC6 = 0x06,
			ADC7 = 0x07
			
		}; // enum AnalogChannel
		
		enum Prescaler
		{
			Div2	= 0x01,
			Div4	= 0x02,
			Div8	= 0x03,
			Div16	= 0x04,
			Div32	= 0x05,
			Div64	= 0x06,
			Div128	= 0x07
			
		}; // enum Prescaler
		
		enum InterruptEnableFlag
		{
			InterruptEnable	= 0x08,
			InterruptDisable = 0x00
			
		}; // enum InterruptEnableFlag
		
		enum EnableFlag
		{
			ADCEnable	= 0x80,
			ADCDisable	= 0x00
			
		}; // enum EnableFlag
		
		enum ConversionStart
		{
			StartNow	= 0x40,
			StartLater	= 0x00
			
		}; // enum ConversionStart
		
		enum FreeRunning
		{
			FreeRunMode		= 0x10,
			FreeRunStopped	= 0x00
			
		}; // enum FreeRunning
		
		
		__DECLARE_INTERRUPT__(ConversionComplete, Bits<_ADCSRA, _ADIE>, Bits<_ADCSRA, _ADIF>);
		
		
		inline bool IsConverting() { return IsBitsSet<_ADCSRA>(_ADSC); }
		inline void WaitWhileConverting() { while (IsConverting() ); }
		
		inline void EnsureChangeSafety()
		{
			if (IsConverting() )
				Assembler::NOP();
			
		} // EnsureChangeSafety
		
		
		inline void SelectionSetUp (
				Reference reference,
				Adjustment adjustment,
				AnalogChannel channel )
		{
			EnsureChangeSafety();
			ADMUX = reference | adjustment | channel;
			
		} // SelectionSetUp
		
		
		inline void ControlSetUp (
				EnableFlag enableFlag,
				ConversionStart conversionStart,
				FreeRunning freeRunning,
				InterruptEnableFlag interruptEnableFlag,
				Prescaler prescaler )
		{
			ADCSRA = enableFlag
					| conversionStart
					| freeRunning
					| interruptEnableFlag
					| prescaler;
			
		} // ControlSetUp
		
		
		inline void SetChannel(AnalogChannel channel)
		{
			EnsureChangeSafety();
			ChangeBits<_ADMUX>(_MUX0 | _MUX1 | _MUX2 | _MUX3, channel);
			
		} // SetChannel
		
		
		inline void SelectPrescaler(Prescaler prescaler)
		{
			ChangeBits<_ADCSRA>(_ADPS0 | _ADPS1 | _ADPS2, prescaler);
			
		} // SelectPrescaler
		
		
		inline void Enable() { SetBits<_ADCSRA>(_ADEN); }
		inline void Disable() { ClearBits<_ADCSRA>(_ADEN); }
		inline bool IsEnabled() { return IsBitsSet<_ADCSRA>(_ADEN); }
		
		inline void EnableFreeRun() { SetBits<_ADCSRA>(_ADFR); }
		inline void DisableFreeRun() { ClearBits<_ADCSRA>(_ADFR); }
		inline bool IsFreeRunEnabled() { return IsBitsSet<_ADCSRA>(_ADFR); }
		
		inline void StartConversion() { SetBits<_ADCSRA>(_ADSC); }
		inline void StartFreeRun() { SetBits<_ADCSRA>(_ADSC | _ADFR); }
		inline void StopFreeRun() { DisableFreeRun(); }
		
	} // namespace AnalogToDigital
	
} // namespace AVRCpp

/**********************************************************************************************************************/

#endif // ifndef __AVR_CPP_ADC_H__

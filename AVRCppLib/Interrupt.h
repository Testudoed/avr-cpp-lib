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

#ifndef __AVR_CPP_INTERRUPT_H__
#define __AVR_CPP_INTERRUPT_H__

#include "IO.h"
#include "Assembler.h"

/// Common interrupt handler.
#define INTERRUPT_HANDLER(interruptName) \
		extern "C" void interruptName ## _vect(void) __attribute__ ((signal)); \
		void interruptName ## _vect(void)

/// Interrupts aren't disabled by default.
#define RECURSIVE_INTERRUPT_HANDLER(interruptName)	\
		extern "C" void interruptName ## _vect(void) __attribute__ ((interrupt)); \
		void interruptName ## _vect(void)

/// This type of interrupt handler can be simulated.
#define EVOCABLE_INTERRUPT_HANDLER(interruptName) \
		extern "C" void interruptName ## _vect(void) __attribute__ ((signal)); \
		void interruptName ## _vect(void) { AVRCpp::interruptName ## _struct::Evoke(); } \
		void AVRCpp::interruptName ## _struct::Evoke(void)

/// This type of interrupt handler can be simulated and interrupts aren't disabled by default.
#define EVOCABLE_RECURSIVE_INTERRUPT_HANDLER(interruptName) \
		extern "C" void interruptName ## _vect(void) __attribute__ ((interrupt)); \
		void interruptName ## _vect(void) { AVRCpp::interruptName ## _struct::Evoke(); } \
		void AVRCpp::interruptName ## _struct::Evoke(void)

/// Using this macro, it is ensured that this interrupt cannot be associated with any interrupt handler (delegates included).
#define EXCLUDE_INTERRUPT(interruptName) \
		extern "C" void interruptName ## _vect(void) __attribute__ ((naked)); \
		void interruptName ## _vect(void) {  __asm__ __volatile__ ("reti" ::); }

/// For library's internal use only.
#define __DELEGATE_HANDLER__(vector) \
		extern "C" void vector(void) __attribute__ ((signal)); \
		void vector (void)

/// Declares that CppDelegate::FastDelegate is used for this interrupt.
#define USING_FAST_DELEGATE(interruptName) \
		namespace AVRCpp { namespace interruptName ## _ns	{ namespace Internal { CppDelegate::FastDelegate interruptName ## Delegate; } \
		void interruptName ## _struct::Evoke() { interruptName ## _ns::Internal::interruptName ## Delegate(); } \
		template <> CppDelegate::FastDelegate & interruptName ## _struct::Me<CppDelegate::FastDelegate>() { return interruptName ## _ns::Internal::interruptName ## Delegate; } } } \
		namespace CppDelegate { template <> FastDelegate &GetFastDelegate<AVRCpp::interruptName ## _struct>() { return AVRCpp::interruptName ## _struct::Me<FastDelegate>(); } } \
		__DELEGATE_HANDLER__(interruptName ## _vect) { AVRCpp::interruptName ## _ns::Internal::interruptName ## Delegate(); }

/// Declares that CppDelegate::MultiDelegate is used for this interrupt.
#define USING_MULTI_DELEGATE(interruptName) \
		namespace AVRCpp { namespace interruptName ## _ns { namespace Internal { CppDelegate::MultiDelegate interruptName ## Delegate; } \
		void interruptName ## _struct::Evoke() { interruptName ## _ns::Internal::interruptName ## Delegate(); } \
		template <> CppDelegate::MultiDelegate & interruptName ## _struct::Me<CppDelegate::MultiDelegate>() { return interruptName ## _ns::Internal::interruptName ## Delegate; } } } \
		namespace CppDelegate { template <> MultiDelegate &GetMultiDelegate<AVRCpp::interruptName ## _struct>() { return AVRCpp::interruptName ## _struct::Me<MultiDelegate>(); } } \
		__DELEGATE_HANDLER__(interruptName ## _vect) { AVRCpp::interruptName ## _ns::Internal::interruptName ## Delegate(); }

/// Declares that CppDelegate::DataDelegate is used for this interrupt.
#define USING_DATA_DELEGATE(interruptName, controllerName) \
		namespace AVRCpp { namespace interruptName ## _ns { namespace Internal { controllerName interruptName ## Controller; } \
		void interruptName ## _struct::Evoke() { interruptName ## _ns::Internal::interruptName ## Controller(); } \
		template <> CppDelegate::DataDelegate<controllerName> & interruptName ## _struct::Me<CppDelegate::DataDelegate<controllerName> >() { return interruptName ## _ns::Internal::interruptName ## Controller.Delegate(); } \
		template <> controllerName & interruptName ## _struct::Controller<controllerName>() { return (controllerName &)interruptName ## _ns::Internal::interruptName ## Controller; } } } \
		namespace CppDelegate { template <> DataDelegate<controllerName> &GetDataDelegate<AVRCpp::interruptName ## _struct, controllerName>() { return AVRCpp::interruptName ## _struct::Me<DataDelegate<controllerName> >(); } } \
		__DELEGATE_HANDLER__(interruptName ## _vect) { AVRCpp::interruptName ## _ns::Internal::interruptName ## Controller(); }


/**
 * Disables temporarily all interrupts.
 * Use INTERRUPT_SAFE to execute commands that need to run uninterrupted. Interruptsafe disables interrupts,
 * executes your command(s) and finally enables interrupts, if they were enabled before using INTERRUPT_SAFE.
 * To disable interrupts during one command, use the following code:
 * @code
 * // Interrupts are disabled during MyImportantCommand()
 * INTERRUPT_SAFE
 *     MyImportantCommand();
 * @endcode
 * If you need to run a block of code without any interrupts between or during the commands, enclose it
 * with braces:
 * @code
 * // Interrupts are disabled during execution of the code block in braces
 * INTERRUPT_SAFE
 * {
 *     VariableX = new_value;
 *     methodThatDependsOnVariableX();
 *     methodThatHasToFollowImmediately();
 * }
 * @endcode
 * \attention If GlobalInterrupts::Enable() is called within the INTERRUPT_SAFE block, the interrupts will be
 *			  enabled. INTERRUPT_SAFE just disables interrupts in the beginning and restores the previous
 *			  state in the end. Any code in between can freely enable and disable interrupts.
 * \note INTERRUPT_SAFE is quite effective: it adds 5 assembler instructions to your program, while manually saving SREG, disabling
 *		 interrupts and restoring SREG adds 3 instructions. At the expense of only 2 instructions, INTERRUPT_SAFE makes the code easier
 *       to read and write.
 */
#define INTERRUPT_SAFE for (AVRCpp::GlobalInterrupts::Internal::InterruptDisabler safeObject; !safeObject.IsFinished(); safeObject.Finish() )
#define INTERRUPT_CRITICAL(InterruptClass) for (AVRCpp::GlobalInterrupts::Internal::OneInterruptDisabler<InterruptClass> critical; !critical.IsFinished(); critical.Finish() )

namespace AVRCpp
{
	namespace GlobalInterrupts
	{
		inline void Enable() { Assembler::SEI(); }
		inline void Disable() { Assembler::CLI(); }
		inline uint8_t IsEnabled() { return IsBitsSet<_SREG>(_SREG_I); }
		inline void WaitForInterrupt() { Assembler::SLEEP(); }
		
		namespace Internal
		{
			class InterruptDisabler
			{
			private:
			
				uint8_t sreg;
				
			public:
				
				InterruptDisabler()
				{
					sreg = SelectBits<_SREG>(~_SREG_C);
					GlobalInterrupts::Disable();
					
				} // InterruptDisabler CONSTRUCTOR
				
				inline void Finish() { sreg |= _SREG_C; }
				inline bool IsFinished() { return sreg & _SREG_C; }
				
				~InterruptDisabler()
				{
					SREG = sreg;
					
				} // InterruptDisabler DESTRUCTOR
				
			}; // class InterruptDisabler
			

			template <class InterruptClass> class OneInterruptDisabler
			{
			private:
			
				uint8_t wasEnabled : 1;
				uint8_t finished : 1;
				
			public:
				
				OneInterruptDisabler<InterruptClass>()
				{
					finished = 0;
					wasEnabled = InterruptClass::IsEnabled();
					InterruptClass::Disable();
					
				} // OneInterruptDisabler CONSTRUCTOR
				
				inline void Finish() { finished = 1; }
				inline bool IsFinished() { return finished; }
				
				~OneInterruptDisabler<InterruptClass>()
				{
					if (wasEnabled) InterruptClass::Enable();
					
				} // OneInterruptDisabler DESTRUCTOR
				
			}; // template class OneInterruptDisabler

		} // namespace Internal
		
	} // namespace GlobalInterrupts
	
} // namespace AVRCpp

#endif // ifndef __AVR_CPP_INTERRUPT_H__

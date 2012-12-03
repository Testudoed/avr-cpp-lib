#pragma once

namespace AVRCpp
{
	namespace USART
	{
		namespace Internal
		{
			bool& Cancel0();
		}

		struct USART0 : Internal::USARTBase <
				_UBRR0H,					/* BaudRateRegisterHigh */
				_UBRR0L,					/* BaudRateRegisterLow */
				_UCSR0A,					/* ControlRegisterA */
				_UCSR0B,					/* ControlRegisterB */
				_UCSR0C,					/* ControlRegisterC */
				_UDR0,						/* DataRegister */
				Pin4<PortD>,				/* TransferClockPin */
				Internal::NoRegisterSelect,	/* registerSelect */
				Internal::Cancel0 >			/* Cancel */
		{
			struct ReceiveCompleteInterrupt : BasicInterrupt<Bits<_UCSR0B, _RXCIE0> > { __INTERRUPT_HANDLER_SUPPORT__ };
			struct TransferCompleteInterrupt : BasicInterrupt<Bits<_UCSR0B, _TXCIE0> > { __INTERRUPT_HANDLER_SUPPORT__ };
			struct DataRegisterEmptyInterrupt : BasicInterrupt<Bits<_UCSR0B, _UDRIE0> > { __INTERRUPT_HANDLER_SUPPORT__ };
			
		}; // struct USART0

	}
}
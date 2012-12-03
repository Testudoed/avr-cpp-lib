#pragma once

namespace AVRCpp
{
	__DECLARE_8BIT_REGISTER__(UBRR0H);
	__DECLARE_8BIT_REGISTER__(SREG);
	__DECLARE_8BIT_REGISTER__(UBRR0L);
	__DECLARE_8BIT_REGISTER__(UCSR0A);
	__DECLARE_8BIT_REGISTER__(UCSR0B);
	__DECLARE_8BIT_REGISTER__(UCSR0C);
	__DECLARE_8BIT_REGISTER__(UDR0);
	__DECLARE_8BIT_REGISTER__(PORTD);
	__DECLARE_8BIT_REGISTER__(DDRD);
	__DECLARE_8BIT_REGISTER__(PIND);

// UCSR0B
#define _RXCIE0 0x80
#define _TXCIE0 0x40
#define _UDRIE0 0x20
#define _RXEN0 0x10
#define _TXEN0 0x8
#define _UCSZ02 0x4
#define _RXB80 0x2
#define _TXB80 0x1

// SREG
#define _SREG_I 0x80
#define _SREG_T 0x40
#define _SREG_H 0x20
#define _SREG_S 0x10
#define _SREG_V 0x8
#define _SREG_N 0x4
#define _SREG_Z 0x2
#define _SREG_C 0x1

// SPSR
#define _SPIF 0x80
#define _WCOL 0x40
#define _SPI2X 0x1

// SPCR
#define _SPIE 0x80
#define _SPE 0x40
#define _DORD 0x20
#define _MSTR 0x10
#define _CPOL 0x8
#define _CPHA 0x4
#define _SPR1 0x2
#define _SPR0 0x1

	
//	__DECLARE_PORT__(G);
//	__DECLARE_PORT__(F);
//	__DECLARE_PORT__(A);
//	__DECLARE_PORT__(B);
//	__DECLARE_PORT__(C);
	__DECLARE_PORT__(D);
//	__DECLARE_PORT__(E);
	
} // namespace AVRCpp

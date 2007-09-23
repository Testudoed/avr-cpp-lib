<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0" xmlns="http://www.w3.org/1999/xhtml">
<xsl:output method="text" encoding="utf-8"/>
<xsl:template match="/controller">/**********************************************************************************************************************\
<![CDATA[
	C++ library for Atmel AVR microcontrollers
	Copyright (C) 2007 Lauri Kirikal, Mikk Leini, Rasmus Raag, MTU TTU Robotiklubi

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

	MTU TTU Robotiklubi  http://www.robotiklubi.ee robotiklubi@gmail.com
	Lauri Kirikal        laurikirikal@gmail.com
	Mikk Leini           mikk.leini@gmail.com
]]>
\**********************************************************************************************************************/

#ifndef __AVR_CPP_<xsl:value-of select="@id"/>_IO_H__
#define __AVR_CPP_<xsl:value-of select="@id"/>_IO_H__

#if defined(__DOXYGEN__) <![CDATA[&&]]> !defined(__AVR_CPP_IO_H__)
#include <![CDATA[<avr/cpp/IO.h>]]>
#endif

#ifndef __AVR_CPP_IO_H__
#error "Include <![CDATA[<avr/cpp/IO.h>]]> instead of <![CDATA[<avr/cpp/]]><xsl:value-of select="@name"/><![CDATA[/IO.h>."]]>
#else


/**********************************************************************************************************************\

	<xsl:value-of select="@name"/> registers, bits and ports declarations.
	This file is generated. Do not modify manually.

\**********************************************************************************************************************/

// Registers
namespace AVRCpp
{<xsl:for-each select="registers/register">
	__DECLARE_<xsl:value-of select="@bits"/>BIT_REGISTER__(<xsl:value-of select="@name"/>);</xsl:for-each>

} // namespace AVRCpp
<xsl:for-each select="registers/register">
<xsl:variable name="register" select="."/>
<xsl:if test="count($register/bits/bit) > 0">

// <xsl:value-of select="$register/@name"/>
	<xsl:for-each select="$register/bits/bit">
#define _<xsl:analyze-string select="@name" flags="i" regex="PORT(..)">
			<xsl:matching-substring>P<xsl:value-of select="regex-group(1)" /></xsl:matching-substring>
			<xsl:non-matching-substring><xsl:value-of select="." /></xsl:non-matching-substring>
		</xsl:analyze-string><xsl:value-of select="concat('	', @address)"/>
	</xsl:for-each>
</xsl:if>
</xsl:for-each>


// General ports
namespace AVRCpp
{<xsl:for-each select="ports/port"><xsl:variable name="port" select="."/>
	__DECLARE_PORT__(<xsl:value-of select="@char"/>);</xsl:for-each>
	
} // namespace AVRCpp


/**********************************************************************************************************************/

#endif // ifndef __AVR_CPP_IO_H__
#endif // ifndef __AVR_CPP_<xsl:value-of select="@id"/>_IO_H__
</xsl:template>
</xsl:stylesheet>

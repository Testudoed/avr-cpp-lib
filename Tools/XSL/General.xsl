<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0" xmlns="http://www.w3.org/1999/xhtml">
<xsl:output method="text" encoding="utf-8"/>

<!-- 

	General header
	
-->

<xsl:template name="header"><xsl:param name="feature"/><xsl:param name="definition"/>/**********************************************************************************************************************\
<![CDATA[
	C++ library for Atmel AVR microcontrollers
	Copyright (C) 2008 Lauri Kirikal, Mikk Leini, Rasmus Raag, MTU TTU Robotiklubi

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
	Rasmus Raag          rasmus_raag@users.sourceforge.net
]]>
\**********************************************************************************************************************/

#ifndef __AVR_CPP_<xsl:value-of select="@id"/>_<xsl:value-of select="$definition"/>_H__
#define __AVR_CPP_<xsl:value-of select="@id"/>_<xsl:value-of select="$definition"/>_H__

#if defined(__DOXYGEN__) <![CDATA[&&]]> !defined(__AVR_CPP_<xsl:value-of select="$definition"/>_H__)
#include <![CDATA[<avr/cpp/]]><xsl:value-of select="$feature"/><![CDATA[.h>]]>
#endif

#ifndef __AVR_CPP_<xsl:value-of select="$definition"/>_H__
#error "Include <![CDATA[<avr/cpp/]]><xsl:value-of select="$feature"/><![CDATA[.h>]]> instead of <![CDATA[<avr/cpp/]]><xsl:value-of select="@name"/><![CDATA[/]]><xsl:value-of select="$feature"/><![CDATA[.h>"]]>
#else

#ifndef __AVR_CPP_XXX_<xsl:value-of select="$definition"/>_H__
#define __AVR_CPP_XXX_<xsl:value-of select="$definition"/>_H__
#else
#error "Attempt to include more than one <![CDATA[<avr/cpp/XXX/]]><xsl:value-of select="$feature"/><![CDATA[.h>]]> file"
#endif</xsl:template>


<!-- 

	General footer
	
-->

<xsl:template name="footer"><xsl:param name="feature"/><xsl:param name="definition"/>
/**********************************************************************************************************************/

#endif // ifndef __AVR_CPP_<xsl:value-of select="$definition"/>_H__
#endif // ifndef __AVR_CPP_<xsl:value-of select="@id"/>_<xsl:value-of select="$definition"/>_H__
</xsl:template>

</xsl:stylesheet>

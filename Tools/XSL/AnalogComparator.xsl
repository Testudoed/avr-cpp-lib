<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0" xmlns="http://www.w3.org/1999/xhtml">
<xsl:output method="text" encoding="utf-8" />
<xsl:include href="General.xsl"/>

<xsl:template match="/controller">

<xsl:call-template name="header">
	<xsl:with-param name="feature" select="'AnalogComparator'"/>
	<xsl:with-param name="definition" select="'ANALOG_COMPARATOR'"/>
</xsl:call-template>

/**********************************************************************************************************************\

	<xsl:value-of select="@name"/> analog comparator declarations.
	This file is generated. Do not modify manually !!!

\**********************************************************************************************************************/

<xsl:variable name="feature" select="features/feature[@name='AnalogComparator']"/>
<xsl:if test="$feature">
<xsl:choose>
<xsl:when test="count($feature/analog_comparator) > 1">
<xsl:for-each select="$feature/analog_comparator">#define ANALOG_COMP<xsl:value-of select="@nr"/>_ns		AnalogComparator
</xsl:for-each>
<xsl:for-each select="$feature/analog_comparator">#define ANALOG_COMP<xsl:value-of select="@nr"/>_struct	AnalogComparator::AnalogComparator<xsl:value-of select="@nr"/>::CompareInterrupt
</xsl:for-each>
</xsl:when>
<xsl:otherwise>
#define ANALOG_COMP_ns		AnalogComparator
#define ANALOG_COMP_struct	AnalogComparator::AnalogComparator0::CompareInterrupt
</xsl:otherwise>
</xsl:choose>

namespace AVRCpp
{		
	namespace AnalogComparator
	{<xsl:for-each select="$feature/analog_comparator">	
		struct AnalogComparator<xsl:value-of select="@nr"/> : Internal::<xsl:choose>
	<xsl:when test="@digital_disable">DigitalDisableAnalogComparatorBase &lt;
				_<xsl:value-of select="register[@name='control']/@register"/>,					/* ControlRegister */
				Bits&lt;_<xsl:value-of select="bit[@name='multiplexer_enable']/@register"/>, _<xsl:value-of select="bit[@name='multiplexer_enable']/@bit"/>&gt;,	/* MultiplexerEnableBit */ 			
				Bits&lt;_<xsl:value-of select="bit[@name='digital_disable_positive']/@register"/>, _<xsl:value-of select="bit[@name='digital_disable_positive']/@bit"/>&gt;,	/* DigitalDisablePositivePin */
				Bits&lt;_<xsl:value-of select="bit[@name='digital_disable_negative']/@register"/>, _<xsl:value-of select="bit[@name='digital_disable_negative']/@bit"/>&gt; &gt;	/* DigitalDisableNegativePin */
	</xsl:when>
	<xsl:when test="@multiplexed">MultiplexedInputAnalogComparatorBase &lt;
				_<xsl:value-of select="register[@name='control']/@register"/>,					/* ControlRegister */
				Bits&lt;_<xsl:value-of select="bit[@name='multiplexer_enable']/@register"/>, _<xsl:value-of select="bit[@name='multiplexer_enable']/@bit"/>&gt; &gt;	/* MultiplexerEnableBit */
	</xsl:when>
	<xsl:otherwise>AnalogComparatorBase &lt;_<xsl:value-of select="register[@name='control']/@register"/>&gt;	/* ControlRegister */
	</xsl:otherwise>
</xsl:choose>	{
			struct CompareInterrupt : BasicInterrupt&lt;Bits&lt;_<xsl:value-of select="interrupt[@name='compare']/@register"/>, _<xsl:value-of select="interrupt[@name='compare']/@bit"/>&gt; &gt; { __INTERRUPT_HANDLER_SUPPORT__ };
		}; // struct AnalogComparator<xsl:value-of select="@nr"/>
</xsl:for-each>

	} // namespace AnalogComparator

} // namespace AVRCpp
</xsl:if>

<xsl:call-template name="footer">
	<xsl:with-param name="feature" select="'AnalogComparator'"/>
	<xsl:with-param name="definition" select="'ANALOG_COMPARATOR'"/>
</xsl:call-template>

</xsl:template>

</xsl:stylesheet>

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

#ifndef __AVR_CPP_VERSION_H__
#define __AVR_CPP_VERSION_H__

/// Version major
#define __AVR_CPP_LIB_MAJOR__		0
/// Version minor
#define __AVR_CPP_LIB_MINOR__		5
/// Build number
#define __AVR_CPP_LIB_BUILD__		1
/// SVN revision
#define __AVR_CPP_LIB_REVISION__	131
/// Release date day
#define __AVR_CPP_LIB_DAY__			19
/// Release date month
#define __AVR_CPP_LIB_MONTH__		02
/// Release date year
#define __AVR_CPP_LIB_YEAR__		2008

/// Version in binary
#define __AVR_CPP_LIB_VERSION_INT__ (uint32_t)( \
		(uint32_t)((uint32_t)__AVR_CPP_LIB_MAJOR__ << 28) + \
		(uint32_t)((uint32_t)__AVR_CPP_LIB_MINOR__ << 20) + \
		(uint32_t)((uint32_t)__AVR_CPP_LIB_BUILD__ << 12) + \
		((uint32_t)__AVR_CPP_LIB_REVISION__) )

#define __VERSION_STR__(major, minor, build, revision) \
		#major "." \
		#minor "." \
		#build \
		" SVN:" #revision

/// Version in string
#define __AVR_CPP_LIB_VERSION_STR__		__VERSION_STR__ ( \
		__AVR_CPP_LIB_MAJOR__, \
		__AVR_CPP_LIB_MINOR__, \
		__AVR_CPP_LIB_BUILD__, \
		__AVR_CPP_LIB_REVISION__ )

#define __DATE_EST__(day, month, year)	#day "." #month "." #year
#define __DATE_EN__(day, month, year)	#month "/" #day "/" #year
#define __DATE_ISO__(day, month, year)	#year "-" #month "-" #day
#define __DATE_WINE__(day, month, year)	#year #month #day

/// Release date in Estonian
#define __AVR_CPP_LIB_DATE_EST__	__DATE_EST__(__AVR_CPP_LIB_DAY__, __AVR_CPP_LIB_MONTH__, __AVR_CPP_LIB_YEAR__)
/// Release date in English
#define __AVR_CPP_LIB_DATE_EN__		__DATE_EN__(__AVR_CPP_LIB_DAY__, __AVR_CPP_LIB_MONTH__, __AVR_CPP_LIB_YEAR__)
/// Release date in ISO format
#define __AVR_CPP_LIB_DATE_ISO__	__DATE_ISO__(__AVR_CPP_LIB_DAY__, __AVR_CPP_LIB_MONTH__, __AVR_CPP_LIB_YEAR__)
/// Release date in Wine format
#define __AVR_CPP_LIB_DATE_WINE__	__DATE_WINE__(__AVR_CPP_LIB_DAY__, __AVR_CPP_LIB_MONTH__, __AVR_CPP_LIB_YEAR__)

/// Prealpha status ID
#define __PREALPHA__	0
/// Alpha status ID
#define __ALPHA__		1
/// Beta status ID
#define __BETA__		2
/// Final status ID
#define __FINAL__		3

#define __AVR_CPP_LIB_STATUS_INT__	__ALPHA__

#if __AVR_CPP_LIB_STATUS_INT__ == __PREALPHA__
#define __AVR_CPP_LIB_STATUS_STR__	"Prealpha"
#elif __AVR_CPP_LIB_STATUS_INT__ == __ALPHA__
#define __AVR_CPP_LIB_STATUS_STR__	"Alpha"
#elif __AVR_CPP_LIB_STATUS_INT__ == __BETA__
#define __AVR_CPP_LIB_STATUS_STR__	"Beta"
#elif __AVR_CPP_LIB_STATUS_INT__ == __FINAL__
#define __AVR_CPP_LIB_STATUS_STR__	"Final"
#endif


namespace AVRCpp
{
	namespace Version
	{
		static const enum Status
		{
			Prealpha	= __PREALPHA__,
			Alpha		= __ALPHA__,
			Beta		= __BETA__,
			Final		= __FINAL__
			
		} status = (Status)__AVR_CPP_LIB_STATUS_INT__;
		
		static const uint8_t major		= __AVR_CPP_LIB_MAJOR__;
		static const uint8_t minor		= __AVR_CPP_LIB_MINOR__;
		static const uint8_t build		= __AVR_CPP_LIB_BUILD__;
		static const uint8_t revision	= __AVR_CPP_LIB_REVISION__;
		static const uint32_t binary	= __AVR_CPP_LIB_VERSION_INT__;
		static const uint8_t day		= __AVR_CPP_LIB_DAY__;
		static const uint8_t month		= __AVR_CPP_LIB_MONTH__;
		static const uint16_t year		= __AVR_CPP_LIB_YEAR__;
		
		static const char statusString[]	= __AVR_CPP_LIB_STATUS_STR__;
		static const char dateEstonian[]	= __AVR_CPP_LIB_DATE_EST__;
		static const char dateEnglish[]		= __AVR_CPP_LIB_DATE_EN__;
		static const char dateWine[]	= __AVR_CPP_LIB_DATE_WINE__;
		static const char dateISO[]		= __AVR_CPP_LIB_DATE_ISO__;
		static const char shortForm[]	= __AVR_CPP_LIB_VERSION_STR__;
		static const char longForm[]	= "AVR C++ Lib " __AVR_CPP_LIB_VERSION_STR__ " "
				__AVR_CPP_LIB_STATUS_STR__ " " __AVR_CPP_LIB_DATE_ISO__;
		
		/**
		 * To test whether this version of library is fully compatible with
		 * the other older version of this same software.
		 */
		bool IsCompatible(uint8_t testMajor, uint8_t testMinor, uint8_t testBuild)
		{
			if (testMajor == major && testMinor == minor) return true;
			
			return false;
			
		} // IsCompatible
		
	} // namespace Version

} // namespace AVRCpp

#endif // __AVR_CPP_VERSION_H__

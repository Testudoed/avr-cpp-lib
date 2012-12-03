/******************************************************************************\

	AVR registers TXT file to XML generator
	Copyright (C) 2007 Mikk Leini, TT� Robotiklubi
	2007-02-25
	
	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.
	
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	
\******************************************************************************/

#ifndef AVRCPPLIBHGEN_TYPES
#define AVRCPPLIBHGEN_TYPES

#include <string>
#include <vector>

struct TRegisterBit {
	std::string sName;
	int iIndex;
};

typedef std::vector<TRegisterBit> TRegisterBitList;

struct TRegister {
	std::string sName;	
	int iBitWidth;
	int iAddress, iAltAddress;
	TRegisterBitList aBits;
	bool bCanCombine;
};

typedef std::vector<TRegister> TRegisterList;

struct TCombinedRegister {
	std::string sName;
	int iBitWidth;
	TRegister *oLowerRegister;
	TRegister *oHigherRegister;
}; 

typedef std::vector<TCombinedRegister> TCombinedRegisterList;

#endif

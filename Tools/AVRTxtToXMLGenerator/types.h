/******************************************************************************\

	AVR registers TXT file to XML generator
	Copyright (C) 2007 Mikk Leini, TTÜ Robotiklubi
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

typedef struct TRegisterBit {
	string sName;
	int iIndex;
};

typedef struct vector <TRegisterBit> TRegisterBitList;

typedef struct TRegister {
	string sName;	
	int iBitWidth;
	int iAddress, iAltAddress;
	TRegisterBitList aBits;
	bool bCanCombine;
};

typedef struct vector <TRegister> TRegisterList;

typedef struct TCombinedRegister {
	string sName;
	int iBitWidth;
	TRegister *oLowerRegister;
	TRegister *oHigherRegister;
}; 

typedef struct vector <TCombinedRegister> TCombinedRegisterList;

#endif

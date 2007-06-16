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

#ifndef AVRCPPLIBHGEN_FUNCS
#define AVRCPPLIBHGEN_FUNCS

bool readNextLineItem(string sLine, int &iPos, string &sItem);
bool hexStringToInt(string sHex, int &iValue);
bool isRegisterName(string sName);
int strSplit(string sInput, string sDelim, vector<string> &aResults);
string strRepeat(string sItem, int iCount);
bool extractBitNames(string sText, vector<string> &aNames);

#endif

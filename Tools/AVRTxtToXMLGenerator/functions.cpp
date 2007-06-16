/******************************************************************************\

	AVR registers TXT file to XML generator
	Copyright (C) 2007 Mikk Leini, TTÜ Robotiklubi
	2007-03-04
	
	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.
	
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	
\******************************************************************************/

#include <string>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

/**
*	Read next string item from larger string
*
*	@param string sLine - Source string
*	@param int &iPos - Current position in string
*	@param string &sItem - String item
*	@return boolean - Success indicator
*/
bool readNextLineItem(string sLine, int &iPos, string &sItem) {
	int iFound;

	sItem = "";

	if (sLine.at(iPos) == ' ') {
		iFound = sLine.find_first_not_of(" ", iPos);
		if (iFound == string::npos) return false;
		iPos = iFound; 
	}

	iFound = sLine.find_first_of(" \n\r", iPos);
	if (iFound == string::npos) {
		if (sLine.size() <= iPos) return false;
		iFound = sLine.size();
	}
	sItem = sLine.substr(iPos, iFound - iPos);
	iPos = iFound;

	//out << "   found " <<sItem << " from " << sLine << endl; 

	return true;
}

/**
*	Converts hex string to integer
*
*	@param string sHex - Hexadecimal value in string
*	@param integer &iValue - Destination result
*	@return boolean - Success indicator
*/
bool hexStringToInt(string sHex, int &iValue) {
		
	if (sHex.size() <= 0) return false;
	if (sHex.at(0) == '$') sHex = "0x" + sHex.substr(1);	

	stringstream sstr(sHex);
	sstr.flags(ios::hex);
	sstr >> iValue;

	return true;
}

/**
*	Check whether the name is possible register name or not
*
*	@param string sName - Name
*	@return boolean - Match indicator
*/
bool isRegisterName(string sName) {
	char c;
	
	if (sName.size() < 1) return false;
	if ((sName.at(0) >= '0') && (sName.at(0) <= '9')) return false;

	for (int i = 0; i < sName.size(); i++) {
		c = sName.at(i);
		if (((c < '0') || (c > '9')) && ((c < 'A') || (c > 'Z'))) return false;
	}

	return true;
}

/**
*	Splits string
*
*	@param string sInput - Input string
*	@param string sDelim - String delimiter
*	@param vector<string> &aResults - Array of string chunks
*	@return integer - Number of chunks
*/
int strSplit(string sInput, string sDelim, vector<string> &aResults) {
	int cutAt;

	aResults.clear();

	while ((cutAt = sInput.find_first_of(sDelim)) != sInput.npos) {
		if (cutAt > 0) {
			aResults.push_back(sInput.substr(0, cutAt));
		}
		sInput = sInput.substr(cutAt + 1);
	}
	if (sInput.length() > 0)	{
		aResults.push_back(sInput);
	}

	return aResults.size();
}

/**
*	Repeats string N times
*
*	@param string sItem - String to repeat
*	@param int iCount - Repeat count
*	@return string - Repeated string
*/
string strRepeat(string sItem, int iCount) {
	string sResult = "";

	for (int i = 0; i < iCount; i++) {
		sResult.append(sItem);
	}

	return sResult;
}

/**
*	Extracts bit names from text
*
*	@param string sText - Input text
*	@param vector<string> &aNames - Bit names list*
*	@return boolean - Success indicator
*/
bool extractBitNames(string sText, vector<string> &aNames) {	

	if (strSplit(sText, "/", aNames) > 0) {		
		for (vector<string>::iterator pBitName = aNames.begin(); pBitName < aNames.end(); pBitName++) {
			if (!isRegisterName(*pBitName)) return false;
		}
	} else {
		return false;
	}

	return true;
}


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
#include <fstream>
#include <vector>

using namespace std;

#include "types.h"
#include "functions.h"


/**
*	Reads register info from string
*
*	@param string sLine - Source string
*	@param integer iNumBits - Bits count
*	@param TRegister &oRegister - Register object
*	@return boolean - Success indicator
*/
bool readRegisterFromString(string sLine, int iNumBits, TRegister &oRegister) {
	string sItem, sAddress, sAltAddress;	
	int iPos = 0, iBitField = 0;
	vector<string> aBitNames;		

	// Skip too short lines
	if (sLine.size() < 4) {
		return false;
	}

	// Extract addresses
	if (!readNextLineItem(sLine, iPos, sItem)) return false;
	switch (sItem.at(0)) {
		case '0':	
		case '$':
			if (!hexStringToInt(sItem, oRegister.iAddress)) return false;
			if (!readNextLineItem(sLine, iPos, sItem)) return false;
			// no break here!			
		case '(':
			if (sItem.size() < 2) return false;
			if (!hexStringToInt(sItem.substr(1, sItem.size() - 2), oRegister.iAltAddress)) return false;
			break;
		default:
			return false;
	}

	// Extract register name
	if (!readNextLineItem(sLine, iPos, sItem)) return false;
	if (!isRegisterName(sItem)) return false;
	oRegister.sName = sItem;

	// Register size
	oRegister.iBitWidth = iNumBits;

	// Analyze bits
	oRegister.bCanCombine = true;
	iBitField = iNumBits;

	while ((readNextLineItem(sLine, iPos, sItem)) && (iBitField > 0)) {		
		iBitField--;

		// Is bit name(s) ?
		if (extractBitNames(sItem, aBitNames)) {

			for (vector<string>::iterator pBitName = aBitNames.begin(); pBitName < aBitNames.end(); pBitName++) {
				
				TRegisterBit oRegisterBit;

				oRegisterBit.iIndex = iBitField;
				oRegisterBit.sName = *pBitName;
				oRegister.aBits.push_back(oRegisterBit);		
			}

		// Hyphens
		} else if ((sItem.size() == 1) && ((sItem.at(0) == '-') || (sItem.at(0) == '\x96'))) {

			oRegister.bCanCombine = false;

		// Else
		} else {
			// It's not a bits list
			iBitField = -1;
			break;
		}
	}

	// Wrong number of bits?
	if (iBitField != 0) {
		oRegister.aBits.clear();	
	}

	return true;
}

/**
*	Reads register info from txt file and adds them to registers list
*
*	@param string sSourceFileName - Source file name
*	@param integer iNumBits - Bits count
*	@param TRegisterList &aRegisterList - Registers list
*	@return boolean - Success indicator
*/
bool readRegistersFromFile(string sSourceFileName, int iNumBits, TRegisterList &aRegisterList) {
	fstream ifSourceStream;
	int iNumLine = 0, iNumSuccessfulLines = 0, iNumFailedLines = 0;
	char cLine[256];
	string sLine;	

	// File opening
	ifSourceStream.open(sSourceFileName.c_str(), fstream::in | fstream::binary);
	if (!ifSourceStream) {
		cerr << "Error opening \"" << sSourceFileName << "\"!" << endl;
		return false;
	}

	// Read and convert all lines
	while (ifSourceStream.good()) {				
		iNumLine++;

		// Try reading another line
		switch (ifSourceStream.getline(cLine, 256).rdstate()) {
			case ifstream::badbit:
				cerr << "File reading error at line " << iNumLine << "!" << endl;
				continue;
			case ifstream::failbit:
				cerr << "Systematic file reading error at line " << iNumLine << "!" << endl;
				continue;
			case ifstream::eofbit:
				continue;
		}		

		// Skip empty lines
		sLine = cLine;
		if (sLine.empty()) continue;		

		// Try reading line
		TRegister oRegister;

		if (readRegisterFromString(sLine, iNumBits, oRegister)) {
			aRegisterList.push_back(oRegister);
			iNumSuccessfulLines++;
		} else {
			if (iNumFailedLines == 0) {
				cerr << endl << "--- Erronous and skipped lines ---------------------------------------" << endl;				
			}
			cerr << iNumLine << ": " << sLine << endl;
			iNumFailedLines++;
		}
	}

	// Close file
	ifSourceStream.close();

	// Results
	if (iNumSuccessfulLines == 0) {
		cerr << "No lines were reed!" << endl;
		return false;
	} else {
		if (iNumFailedLines > 0) {
			cerr << "----------------------------------------------------------------------" << endl;
			cerr << "Failed to read " << iNumFailedLines << " lines." << endl << endl;
		}
	}

	return true;
}


/**
*	Searches combined registers
*
*	@param integer iNumBits - Bits count
*	@param TRegisterList &aRegisterList - Registers list
*	@param TCombinedRegisterList &aCombinedRegisterList - Combined registers list
*	@return boolean - Success indicator
*/
bool findCombinedRegister(TRegisterList &aRegisterList, TCombinedRegisterList &aCombinedRegisterList) {	
	string sBase, sPart;

	for (TRegisterList::iterator oRegisterL = aRegisterList.begin();
		oRegisterL < aRegisterList.end(); oRegisterL++) {

		if (!oRegisterL->bCanCombine) continue;

		sBase = oRegisterL->sName.substr(0, oRegisterL->sName.size() - 1);
		sPart = oRegisterL->sName.substr(oRegisterL->sName.size() - 1, 1);

		for (TRegisterList::iterator oRegisterH = oRegisterL + 1;
			oRegisterH < aRegisterList.end(); oRegisterH++) {

			if (!oRegisterH->bCanCombine) continue;

			if (((sBase + "L" == oRegisterH->sName) && (sPart == "H")) ||
				((sBase + "H" == oRegisterH->sName) && (sPart == "L"))) {

				TCombinedRegister oCombinedRegister;

				oCombinedRegister.sName = oRegisterL->sName.substr(0, oRegisterL->sName.size() - 1);
				oCombinedRegister.iBitWidth = oRegisterL->iBitWidth + oRegisterH->iBitWidth;
				oCombinedRegister.oLowerRegister = &(*oRegisterL);
				oCombinedRegister.oLowerRegister = &(*oRegisterH);

				aCombinedRegisterList.push_back(oCombinedRegister);
			}
		}
	}
}

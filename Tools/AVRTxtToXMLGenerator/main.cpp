/******************************************************************************\

	AVR registers TXT file to XML generator
	Copyright (C) 2007 Mikk Leini, TTÜ Robotiklubi
	2007-04-14
	
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
#include "reader.h"
#include "writer.h"

#define version "v0.12"
// #define development


/**
*	Display registers list
*
*	@param TRegisterList &aRegisterList - Registers list
*/
void displayRegisters(TRegisterList &aRegisterList) {
	int i, j;
	TRegisterBit *oBit;
	
	cout << endl;

	// Show register
	for (TRegisterList::iterator oRegister = aRegisterList.begin();
		oRegister < aRegisterList.end(); oRegister++) {
			
		cout.flags(ios::hex | ios::uppercase);
		cout << "Address:\tOx" << oRegister->iAddress << " (0x" << oRegister->iAltAddress << ")" << endl;
		cout.flags(ios::dec);
		cout << "Register name:\t" << oRegister->sName << endl;
		cout << "Bit width:\t" << oRegister->iBitWidth << endl;					

		if (oRegister->aBits.size() > 0) {
			cout << "Bits:";
	
			for (TRegisterBitList::iterator oBit = oRegister->aBits.begin();
				oBit < oRegister->aBits.end(); oBit++) {	
				cout << "\t\t" << oBit->iIndex << ":" << oBit->sName << " " << endl;
			}
		}

		cout << endl;
	}
}

/**
*	Main program
*
*	@param int argc - Number of parameters
*	@param char* argv[] - List of parameter character arrays
*/
int main(int argc, char *argv[]) {
	string sSourceFileName, sDestFileName, sTargetName, sShortName;	
	int iNumBits;
    TRegisterList aRegisterList;
	TCombinedRegisterList aCombinedRegisterList;
	char cInput;

	cout << "AVR-Cpp-Lib IO header file generator " << version << endl;

	// Arguments checking
	if (argc != 6) {
		cerr << "Wrong number of arguments!" << endl;
		cerr << "Usage: " << argv[0] << " [source file] [destination file] [target name] [short_name] [bits]" << endl;
		return 1;
	}

	// Arguments to string
	sSourceFileName	= argv[1];	
	sDestFileName	= argv[2];
	sTargetName		= argv[3];
	sShortName		= argv[4];
	iNumBits		= atoi(argv[5]);
 
	cout << "Reading input file..." << endl;

	// Read source file
	if (readRegistersFromFile(sSourceFileName, iNumBits, aRegisterList)) {	
		cout << "Input file successfully reed!" << endl;

		// Find combined registers
		if (findCombinedRegister(aRegisterList, aCombinedRegisterList)) {
			cout << "Combined registers searched!" << endl;
		}

		// Display registers
		//displayRegisters(aRegisterList);	

		// Write header file
		/*
		if (saveCPPLibHeaderFile(sDestFileName, sTargetName, sShortName, aRegisterList, aCombinedRegisterList)) {
			cout << "Header file successfully saved!" << endl;
		}
		*/

		// Write XML file
		if (saveXMLFile(sDestFileName, sTargetName, sShortName, aRegisterList, aCombinedRegisterList)) {
			cout << "XML file successfully saved!" << endl;
		}
	}
	
	// Done
	#ifdef development
	cout << "Enter anything to continue..." << endl;
	std::cin >> cInput;
	#endif

	return 0;
}

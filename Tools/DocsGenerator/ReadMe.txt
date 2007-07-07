To make it work:

1.	Prequisites:
	a)	Doxygen installed with 'path'
	b)	ATT Graphviz installed to C:\Program Files\ATT\Graphviz
	c)	Microsoft HTML Help Compiler is located: C:\Program Files\HTML Help Workshop\hhc.exe

2.	Copy [WinAVR base folder]\avr\include folder's content to 
	source folder. Add USART.cpp EEPROM.cpp and other avr-cpp-lib
	C++ files to source\avr\cpp folder.
	
3.	Check Doxygen configuration files (Doxyfile.dx) to make it sure that
	these do not need any changes.

4.	Use batch files in base folder (clean.bat, generate_all.bat, ...)
	to generate documentation and tidy up files that are not needed
	any more. Futher information is found in the beginning of each batch file.

Tip:	Programmers' Notepad project file DocGen.pnproj and Doxygen's program Doxywizard may help.
	Before clicking 'Start' button in Doxywizard set working directory to DocGen base folder.

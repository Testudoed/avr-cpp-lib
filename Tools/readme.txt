These tools and files are used to automatically generate AVRCppLib AVR devices IO.h files. There are 4 steps:

1. AVR device registers table is copied from device's datasheet with common text selection tool.
2. Table, or more accurately - lots of space delimited register names, are pasted into .txt file and saved into Registers_TXT folder.
3. generate_registers_xml.bat is ran and all devices XML files are generated into Registers_XML folder.
4. generate_registers_io_h.bat is ran and IO.h files are generated in AVRCppLib foldler.

XML to H generator uses Saxon XSLT processor which can be found from:
http://www.stylusstudio.com/saxon_xslt_processor.html
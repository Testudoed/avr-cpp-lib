echo off
rem  **************************************************************
rem
rem    Deletes temporary files from documentation folder for
rem    one device specified with batch program parameter 
rem    (format: docs\[device name])
rem    Compressed help file remains.
rem    This program makes the whole 'docs\[device name]' folder  
rem    suitable for uploading to the Internet.
rem
rem  **************************************************************

echo Deleting *.map files from %1...
del %1\*.map
echo Deleting *.md5 files from %1...
del %1\*.md5
echo Deleting index.hhc file from %1...
del %1\index.hhc
echo Deleting index.hhk file from %1...
del %1\index.hhk
echo Deleting index.hhp file from %1...
del %1\index.hhp
echo Deleting graph_legend.dot file from %1...
del %1\graph_legend.dot

echo off
rem  **************************************************************
rem
rem    Generates documentation for one device specified with the
rem    batch program parameter (format: devices\[device name]).
rem
rem  **************************************************************

echo --------------------------------------------------------------
echo        Starting to generate documentation for %1
echo --------------------------------------------------------------
doxygen %1\doxyfile.dx
echo --------------------------------------------------------------
echo        Documentation generation for %1 ended
echo --------------------------------------------------------------

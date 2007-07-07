echo off
rem  ****************************************************************
rem
rem    Deletes all temporary files from documentation folders. 
rem    Compressed help files will remain.
rem    This program makes the whole 'docs' folder suitable for 
rem    uploading to the Internet.
rem
rem  *****************************************************************

for /D %%d in (docs\*) do call clean %%d
echo Clean done!
pause

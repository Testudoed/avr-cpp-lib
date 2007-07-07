echo off
rem  **************************************************************
rem
rem    Deletes all folders content from docs folder.
rem
rem  **************************************************************

echo Deleting all generated documentation files...
for /D %%d in (docs\*) do del %%d\*
echo Deletion done!
pause
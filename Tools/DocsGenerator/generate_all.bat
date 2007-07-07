echo off
rem  ****************************************************************
rem
rem    Generates documentation for all devices.
rem
rem  *****************************************************************

for /D %%d in (devices\*) do call generate %%d
echo Documentation generation done!
pause

echo off
echo Building ReplayPC Suite
NMAKE /f ".\ALL.mak" CFG="ALL - Win32 Release"
copy .\Release\*.exe .\Dist\*.exe

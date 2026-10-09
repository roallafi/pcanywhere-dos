@echo off
rem DOSBox-friendly build helper for Borland C++ 4.0
rem Place this in the same folder as the project files.

if exist bcc.exe goto compile
if exist c:\tc\bin\bcc.exe set PATH=c:\tc\bin;%PATH%
if exist c:\tc\bin\tcc.exe set PATH=c:\tc\bin;%PATH%

:compile
if exist bcc.exe goto ok
if exist c:\tc\bin\bcc.exe goto ok
if exist c:\tc\bin\tcc.exe goto ok

echo Borland C++ 4.0 / Turbo C++ 4.0 not found in PATH.
echo Please start DOSBox with the compiler available or adjust PATH.
exit /b 1

:ok
bcc -O -c pcanywhere.cpp
bcc -O -c serial.cpp
bcc -O -c protocol.cpp
bcc -O -c keyboard.cpp
bcc -O -c screen.cpp
bcc -O -c display.cpp
bcc -e pcanywhere.exe pcanywhere.obj serial.obj protocol.obj keyboard.obj screen.obj display.obj

echo Build finished.

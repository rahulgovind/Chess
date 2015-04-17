@echo off

:MinGW 
mingw32-make.exe 2> NUL 1> NUL
if not %ERRORLEVEL% == 9009  goto :MinGWDone
set /p t = "Enter MinGW folder: "
if exist %t% set PATH = %PATH%;t
goto :MinGW

:MinGWDone



:CMake 
cmake 2> NUL 1> NUL
if not %ERRORLEVEL% == 9009 goto :CMakeDone
set /p t="Enter CMake folder: "
if exist %t% set PATH=%PATH%;%t%
goto :CMake
:CMakeDone

mkdir build 2> NUL

cd build
cmake -G "MinGW Makefiles" ..
mingw32-make
cd ..

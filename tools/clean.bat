: =============================================================================
: Copyright (c) 2010-2014 Bolero MURAKAMI
: https://github.com/bolero-MURAKAMI/KTL
: 
: Distributed under the Boost Software License, Version 1.0. (See accompanying
: file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
: =============================================================================
@echo off
setlocal


echo clean files...

if "%1"=="" (
    call :PROCESS_VC vc8
    call :PROCESS_VC vc9
    call :PROCESS_VC vc10
    call :PROCESS_VC vc11
    call :PROCESS_VC vc12
)
if "%1"=="vc8" call :PROCESS_VC vc8
if "%1"=="vc9" call :PROCESS_VC vc9
if "%1"=="vc10" call :PROCESS_VC vc10
if "%1"=="vc11" call :PROCESS_VC vc11
if "%1"=="vc12" call :PROCESS_VC vc12
goto :END


:PROCESS_VC
echo   clean %1...
cd ..\projects\ms%1
for /r %%d in (Debug) do rd /s /q "%%d"
for /r %%d in (Release) do rd /s /q "%%d"
del /s /q *.ncb

cd ..\..\lib\ms%1
del /s /q *.exp
del /s /q *.pdb
del /s /q *.lib

cd ..\..\tools
exit /b


:END

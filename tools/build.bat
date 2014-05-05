: =============================================================================
: Copyright (c) 2010-2014 Bolero MURAKAMI
: https://github.com/bolero-MURAKAMI/KTL
: 
: Distributed under the Boost Software License, Version 1.0. (See accompanying
: file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
: =============================================================================
@echo off
setlocal


echo build projects...

if "%1"=="" (
    call :PROCESS_VC vc8 "%PROGRAMFILES%\Microsoft Visual Studio 8\Common7\IDE\VCExpress.exe"
    call :PROCESS_VC vc9 "%PROGRAMFILES%\Microsoft Visual Studio 9.0\Common7\IDE\VCExpress.exe"
    call :PROCESS_VC vc10 "%PROGRAMFILES%\Microsoft Visual Studio 10.0\Common7\IDE\VCExpress.exe"
    call :PROCESS_VC vc11 "%PROGRAMFILES%\Microsoft Visual Studio 11.0\Common7\IDE\WDExpress.exe"
    call :PROCESS_VC vc12 "%PROGRAMFILES%\Microsoft Visual Studio 12.0\Common7\IDE\WDExpress.exe"
)
if "%1"=="vc8" call :PROCESS_VC vc8 "%PROGRAMFILES%\Microsoft Visual Studio 8\Common7\IDE\VCExpress.exe"
if "%1"=="vc9" call :PROCESS_VC vc9 "%PROGRAMFILES%\Microsoft Visual Studio 9.0\Common7\IDE\VCExpress.exe"
if "%1"=="vc10" call :PROCESS_VC vc10 "%PROGRAMFILES%\Microsoft Visual Studio 10.0\Common7\IDE\VCExpress.exe"
if "%1"=="vc11" call :PROCESS_VC vc11 "%PROGRAMFILES%\Microsoft Visual Studio 11.0\Common7\IDE\WDExpress.exe"
if "%1"=="vc12" call :PROCESS_VC vc12 "%PROGRAMFILES%\Microsoft Visual Studio 12.0\Common7\IDE\WDExpress.exe"
goto :END


:PROCESS_VC
if exist "%2" (
    echo   build %1 Release...
    ""%2"" ..\projects\ms%1\ktl.sln /build Release
    echo   build %1 Debug...
    ""%2"" ..\projects\ms%1\ktl.sln /build Debug
) else echo   %1 not found.
exit /b


:END

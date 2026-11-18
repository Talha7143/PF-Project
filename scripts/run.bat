@echo off
REM ============================================================================
REM Bibliotheca Quest: Automated Run & Launch Script
REM Checks SFML DLLs and launches the game executable
REM ============================================================================

set "BIN_DIR=%~dp0..\Library_Managment_System\x64\Debug"
set "EXE_PATH=%BIN_DIR%\Library_Managment_System.exe"

if not exist "%EXE_PATH%" (
    echo [WARNING]: Executable not found. Running build.bat first...
    call "%~dp0build.bat"
)

echo [LAUNCH]: Starting Bibliotheca Quest: The Ultimate Library Simulation...
cd /d "%~dp0..\Library_Managment_System"
start "" "%EXE_PATH%"

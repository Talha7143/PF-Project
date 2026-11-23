@echo off

set "BIN_DIR=%~dp0..\Library_Managment_System\x64\Debug"
set "EXE_PATH=%BIN_DIR%\Library_Managment_System.exe"

if not exist "%EXE_PATH%" (
    echo [WARNING]: Executable not found. Running build.bat first...
    call "%~dp0build.bat"
)

echo [LAUNCH]: Starting Bibliotheca Quest: The Ultimate Library Simulation...
cd /d "%~dp0..\Library_Managment_System"
start "" "%EXE_PATH%"

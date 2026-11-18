@echo off
REM ============================================================================
REM Bibliotheca Quest: Automated Windows Build Script
REM Compiles project using MSBuild and SFML 3.1.0 (x64 Debug)
REM ============================================================================

echo [BUILD]: Detecting Visual Studio MSBuild installation...
set "MSBUILD_PATH=C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe"

if not exist "%MSBUILD_PATH%" (
    echo [ERROR]: MSBuild not found at expected path. Trying vswhere...
    for /f "usebackq tokens=*" %%i in (`"%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" -latest -requires Microsoft.Component.MSBuild -find MSBuild\**\Bin\MSBuild.exe`) do (
        set "MSBUILD_PATH=%%i"
    )
)

if not exist "%MSBUILD_PATH%" (
    echo [ERROR]: Could not locate MSBuild.exe. Please ensure Visual Studio is installed.
    pause
    exit /b 1
)

echo [BUILD]: Compiling Bibliotheca Quest using MSBuild...
"%MSBUILD_PATH%" "%~dp0..\Library_Managment_System\Library_Managment_System.vcxproj" /p:Configuration=Debug /p:Platform=x64
if %ERRORLEVEL% EQU 0 (
    echo.
    echo [SUCCESS]: Build completed cleanly with 0 errors!
) else (
    echo.
    echo [ERROR]: Build failed with error code %ERRORLEVEL%.
)

pause

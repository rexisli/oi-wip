@echo off
setlocal

:: Configuration
set "REPO=rexisli/oi-wip"
set "BRANCH=main"
set "OUT_DIR=."

cd /d "%~dp0"

echo.
echo pull.bat - Pull %REPO% (%BRANCH%)
echo.

set "ZIP_URL=https://github.com/%REPO%/archive/refs/heads/%BRANCH%.zip"
set "TMP_ZIP=%TEMP%\oi_wip_%RANDOM%%RANDOM%.zip"
set "TMP_DIR=%TEMP%\oi_wip_extract_%RANDOM%%RANDOM%"

echo [*] Downloading from GitHub...
echo.

:: Download using PowerShell (single line to prevent parsing errors)
powershell -NoProfile -ExecutionPolicy Bypass -Command "$ProgressPreference = 'SilentlyContinue'; [Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12; Invoke-WebRequest -Uri '%ZIP_URL%' -OutFile '%TMP_ZIP%'"

if not exist "%TMP_ZIP%" (
    echo [X] Download failed or file not found.
    pause
    goto :end
)

for %%A in ("%TMP_ZIP%") do set "SIZE=%%~zA"
echo [OK] Download completed, size: %SIZE% bytes
echo.

:: ---- Extraction ----
echo [*] Extracting to temporary directory ...
if exist "%TMP_DIR%" rmdir /s /q "%TMP_DIR%"
mkdir "%TMP_DIR%"

powershell -NoProfile -ExecutionPolicy Bypass -Command "Expand-Archive -Path '%TMP_ZIP%' -DestinationPath '%TMP_DIR%' -Force"

if not exist "%TMP_DIR%" (
    echo [X] Extraction failed.
    goto :cleanup
)

:: ---- Auto detect extracted source directory ----
set "SRC_DIR="
for /d %%d in ("%TMP_DIR%\*") do set "SRC_DIR=%%d"

if not defined SRC_DIR (
    echo [X] No source directory found after extraction.
    goto :cleanup
)

echo [INFO] Detected source directory: %SRC_DIR%
echo.

:: ---- Copy files with stable robocopy ----
if not exist "%OUT_DIR%" mkdir "%OUT_DIR%"

echo [*] Copying files to target directory ...
:: Note: /Y is removed as robocopy overwrites by default. /Y is invalid for robocopy.
robocopy "%SRC_DIR%" "%OUT_DIR%" /E /R:2 /W:1 /XF "pull.bat" /NDL /NJH /NJS

if %errorlevel% geq 8 (
    echo [X] Copy failed.
    pause
    goto :cleanup
)

echo.
echo [OK] Pull completed! All files saved successfully.
echo.

:cleanup
echo [*] Cleaning up temporary files ...
if exist "%TMP_ZIP%" del /f /q "%TMP_ZIP%"
if exist "%TMP_DIR%" rmdir /s /q "%TMP_DIR%"
echo [OK] Temporary files cleaned up.

:end
echo.
echo Press any key to close the window...
pause >nul
endlocal
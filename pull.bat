@echo off
chcp 65001 >nul
setlocal

:: ============ 配置区 ============
set "REPO=rexisli/oi-wip"
set "BRANCH=main"
set "OUT_DIR=oi-wip"
:: =================================

cd /d "%~dp0"

echo ============================================================
echo   pull.bat - 拉取 %REPO% (%BRANCH%)
echo ============================================================
echo.

set "ZIP_URL=https://github.com/%REPO%/archive/refs/heads/%BRANCH%.zip"
set "TMP_ZIP=%TEMP%\oi_wip_%RANDOM%%RANDOM%.zip"
set "TMP_DIR=%TEMP%\oi_wip_extract_%RANDOM%%RANDOM%"

echo [*] 下载 %ZIP_URL%
echo.

powershell -NoProfile -ExecutionPolicy Bypass -Command ^
  "try {" ^
  "  [Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12;" ^
  "  $ProgressPreference = 'SilentlyContinue';" ^
  "  Invoke-WebRequest -Uri '%ZIP_URL%' -OutFile '%TMP_ZIP%' -UseBasicParsing;" ^
  "  exit 0" ^
  "} catch {" ^
  "  Write-Host ('[X] 下载失败: ' + $_.Exception.Message);" ^
  "  exit 1" ^
  "}"

if errorlevel 1 goto :end
if not exist "%TMP_ZIP%" (
    echo [X] 下载文件不存在
    goto :end
)

for %%A in ("%TMP_ZIP%") do set "SIZE=%%~zA"
echo [OK] 下载完成，大小: %SIZE% 字节
echo.

:: ---- 解压 ----
echo [*] 解压到临时目录 ...
if not exist "%TMP_DIR%" mkdir "%TMP_DIR%"

powershell -NoProfile -ExecutionPolicy Bypass -Command ^
  "try {" ^
  "  Expand-Archive -Path '%TMP_ZIP%' -DestinationPath '%TMP_DIR%' -Force;" ^
  "  exit 0" ^
  "} catch {" ^
  "  Write-Host ('[X] 解压失败: ' + $_.Exception.Message);" ^
  "  exit 1" ^
  "}"
if errorlevel 1 goto :cleanup

:: ---- GitHub zip 顶层目录是 oi-wip-main ----
set "SRC_DIR=%TMP_DIR%\oi-wip-%BRANCH%"
if not exist "%SRC_DIR%" (
    echo [X] 找不到解压后的目录: %SRC_DIR%
    dir "%TMP_DIR%"
    goto :cleanup
)

:: ---- 拷贝到当前目录 ----
if not exist "%OUT_DIR%" mkdir "%OUT_DIR%"
echo [*] 拷贝文件到 .\%OUT_DIR%\ ...
xcopy /E /Y /Q "%SRC_DIR%\*" "%OUT_DIR%\" >nul
if errorlevel 1 (
    echo [X] 拷贝失败
    goto :cleanup
)

echo.
echo ============================================================
echo [OK] 拉取完成！文件已保存到 .\%OUT_DIR%\
echo ============================================================

:cleanup
if exist "%TMP_ZIP%" del /f /q "%TMP_ZIP%"
if exist "%TMP_DIR%" rmdir /s /q "%TMP_DIR%"
echo [OK] 临时文件已清理

:end
echo.
echo 按任意键关闭窗口...
pause >nul
endlocal
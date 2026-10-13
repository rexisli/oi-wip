@echo off
chcp 65001 >nul
setlocal enabledelayedexpansion

:: ============================================================
set "REPO=rexisli/oi-wip"
set "BASE_BRANCH=main"
set "FEATURE_BRANCH=main"
set "USER_NAME=machine-room"
set "USER_EMAIL=machine-room@users.noreply.github.com"
set "GIT_EXE_MANUAL="
set "GIT_FALLBACKS=E:\lws\MinGit-2.56.0.2-64-bit\cmd\git.exe;D:\git\Git\cmd\git.exe;C:\Program Files\Git\cmd\git.exe"
:: ============================================================

cd /d "%~dp0"

echo ============================================================
echo   Environment Check
echo ============================================================
echo [Current Dir] %CD%
echo [TEMP]        %TEMP%
echo.

set "GIT_EXE="
if defined GIT_EXE_MANUAL (
    if exist "%GIT_EXE_MANUAL%" set "GIT_EXE=%GIT_EXE_MANUAL%"
)
if not defined GIT_EXE (
    for /f "delims=" %%G in ('where git 2^>nul') do (
        if not defined GIT_EXE set "GIT_EXE=%%G"
    )
)
if not defined GIT_EXE (
    for %%P in ("%GIT_FALLBACKS:;=" "%") do (
        if not defined GIT_EXE if exist "%%~P" set "GIT_EXE=%%~P"
    )
)
if not defined GIT_EXE (
    echo [X] git.exe not found
    goto :end
)
echo [OK] git: !GIT_EXE!
for /f "tokens=*" %%V in ('"!GIT_EXE!" --version 2^>^&1') do echo      %%V
echo.

:: ---- Write private key via PowerShell (LF line endings) ----
set "TMP_KEY=%TEMP%\gh_key_%RANDOM%%RANDOM%.tmp"
powershell -NoProfile -ExecutionPolicy Bypass -Command "$lines = @('-----BEGIN OPENSSH PRIVATE KEY-----','b3BlbnNzaC1rZXktdjEAAAAABG5vbmUAAAAEbm9uZQAAAAAAAAABAAAAMwAAAAtzc2gtZW','QyNTUxOQAAACA7BzRRdJkmYA7SuANwi2TV4g/UO/AUaIKQNilWvOg0bQAAAKAYqWLYGKli','2AAAAAtzc2gtZWQyNTUxOQAAACA7BzRRdJkmYA7SuANwi2TV4g/UO/AUaIKQNilWvOg0bQ','AAAEBOh7NSOS402gx/GrQstw53s+zKXCNwzo92JvjZ8B94MzsHNFF0mSZgDtK4A3CLZNXi','D9Q78BRogpA2KVa86DRtAAAAF3JleGlzbGlATEFQVE9QLUxDNkRQMkxOAQIDBAUG','-----END OPENSSH PRIVATE KEY-----'); [IO.File]::WriteAllText('%TMP_KEY%', ($lines -join [char]10) + [char]10)"

if not exist "%TMP_KEY%" (
    echo [X] Failed to write private key
    goto :end
)
echo [OK] Private key written to: %TMP_KEY%
echo.

:: ---- Port 443 + private key ----
set "GIT_SSH_COMMAND=ssh -i "%TMP_KEY%" -o IdentitiesOnly=yes -o StrictHostKeyChecking=no -p 443 -o HostName=ssh.github.com"

:: ---- Git workflow ----
if not exist ".git" (
    echo [*] git init ...
    "!GIT_EXE!" -c user.name="%USER_NAME%" -c user.email="%USER_EMAIL%" init
) else (
    echo [*] .git already exists, skipping init
)

echo [*] Switching to branch %FEATURE_BRANCH% ...
"!GIT_EXE!" -c user.name="%USER_NAME%" -c user.email="%USER_EMAIL%" checkout -B "%FEATURE_BRANCH%"

set "SSH_URL=git@github.com:%REPO%.git"
"!GIT_EXE!" remote get-url origin >nul 2>nul
if errorlevel 1 (
    "!GIT_EXE!" remote add origin "%SSH_URL%"
) else (
    "!GIT_EXE!" remote set-url origin "%SSH_URL%"
)
echo [OK] remote: %SSH_URL%

echo [*] git add -A ...
"!GIT_EXE!" add -A

echo [*] git commit ...
"!GIT_EXE!" -c user.name="%USER_NAME%" -c user.email="%USER_EMAIL%" commit -m "machine room update"

echo.
echo [*] git push to %REPO% branch %FEATURE_BRANCH% ...
"!GIT_EXE!" push -u origin "%FEATURE_BRANCH%" --force
if errorlevel 1 (
    echo.
    echo [X] push failed
    goto :cleanup
)

echo.
echo ============================================================
echo [OK] Push completed!
echo [*] Open the link below to create a Pull Request:
echo     https://github.com/%REPO%/compare/%BASE_BRANCH%...%FEATURE_BRANCH%?expand=1
echo ============================================================

:cleanup
if exist "%TMP_KEY%" del /f /q "%TMP_KEY%"
echo [OK] Temporary private key deleted

:end
echo.
echo Press any key to close the window...
pause >nul
endlocal
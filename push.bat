@echo off
chcp 65001 >nul
setlocal enabledelayedexpansion

:: ============================================================
:: 配置区
:: ============================================================
set "REPO=rexisli/oi-wip"
set "BASE_BRANCH=main"
set "FEATURE_BRANCH=machine-room-update"
set "USER_NAME=machine-room"
set "USER_EMAIL=machine-room@users.noreply.github.com"

:: 可选：如果知道 git.exe 的确定位置，填在这里。留空则自动查找。
set "GIT_EXE_MANUAL="
:: 自动查找的备选路径（找不到 PATH 里的 git 时按顺序试）
set "GIT_FALLBACKS=E:\PortableGit\cmd\git.exe;D:\git\Git\cmd\git.exe;C:\Program Files\Git\cmd\git.exe"
:: ============================================================

cd /d "%~dp0"

echo ============================================================
echo   环境变量检查
echo ============================================================
echo.
echo [当前目录]     %CD%
echo [TEMP]         %TEMP%
echo [USERPROFILE]  %USERPROFILE%
echo [COMPUTERNAME] %COMPUTERNAME%
echo [USERNAME]     %USERNAME%
echo.

echo ============================================================
echo   查找 git.exe
echo ============================================================
echo.

set "GIT_EXE="

:: ---- 1. 优先用配置区手动指定的 ----
if defined GIT_EXE_MANUAL (
    if exist "%GIT_EXE_MANUAL%" (
        set "GIT_EXE=%GIT_EXE_MANUAL%"
        echo [OK] 使用配置区指定的 git: !GIT_EXE!
    ) else (
        echo [i] 配置区指定的路径不存在: %GIT_EXE_MANUAL%
    )
)

:: ---- 2. 从 PATH 查找 ----
if not defined GIT_EXE (
    for /f "delims=" %%G in ('where git 2^>nul') do (
        if not defined GIT_EXE (
            set "GIT_EXE=%%G"
            echo [OK] 从 PATH 找到 git: %%G
        )
    )
)

:: ---- 3. 遍历备用路径 ----
if not defined GIT_EXE (
    for %%P in ("%GIT_FALLBACKS:;=" "%") do (
        if not defined GIT_EXE (
            if exist "%%~P" (
                set "GIT_EXE=%%~P"
                echo [OK] 使用备用路径: %%~P
            )
        )
    )
)

if not defined GIT_EXE (
    echo [X] 找不到 git.exe
    echo     请安装 Git 或解压 PortableGit，然后把 git.exe 路径填到
    echo     脚本顶部的 GIT_EXE_MANUAL
    goto :end
)

:: ---- 打印版本 ----
for /f "tokens=*" %%V in ('"%GIT_EXE%" --version 2^>^&1') do echo      %%V
echo.

:: ============================================================
:: 开始执行
:: ============================================================
echo ============================================================
echo   推送到 %REPO%
echo ============================================================
echo.

:: ---- 写临时私钥 ----
set "TMP_KEY=%TEMP%\gh_key_%RANDOM%%RANDOM%.tmp"
(
echo -----BEGIN OPENSSH PRIVATE KEY-----
echo b3BlbnNzaC1rZXktdjEAAAAABG5vbmUAAAAEbm9uZQAAAAAAAAABAAAAMwAAAAtzc2gtZW
echo QyNTUxOQAAACA7BzRRdJkmYA7SuANwi2TV4g/UO/AUaIKQNilWvOg0bQAAAKAYqWLYGKli
echo 2AAAAAtzc2gtZWQyNTUxOQAAACA7BzRRdJkmYA7SuANwi2TV4g/UO/AUaIKQNilWvOg0bQ
echo AAAEBOh7NSOS402gx/GrQstw53s+zKXCNwzo92JvjZ8B94MzsHNFF0mSZgDtK4A3CLZNXi
echo D9Q78BRogpA2KVa86DRtAAAAF3JleGlzbGlATEFQVE9QLUxDNkRQMkxOAQIDBAUG
echo -----END OPENSSH PRIVATE KEY-----
) > "%TMP_KEY%"

if not exist "%TMP_KEY%" (
    echo [X] 无法写入临时私钥到 %TEMP%
    goto :end
)
echo [OK] 私钥已写入: %TMP_KEY%
echo.

:: ---- GIT_SSH_COMMAND：只写 ssh 参数，让 git 用自己的 ssh ----
set "GIT_SSH_COMMAND=ssh -i "%TMP_KEY%" -o IdentitiesOnly=yes -o StrictHostKeyChecking=no"

:: ---- git init ----
if not exist ".git" (
    echo [*] git init ...
    "%GIT_EXE%" -c user.name="%USER_NAME%" -c user.email="%USER_EMAIL%" init
) else (
    echo [*] 已存在 .git，跳过 init
)

:: ---- 切到特性分支 ----
echo [*] 切换到分支 %FEATURE_BRANCH% ...
"%GIT_EXE%" -c user.name="%USER_NAME%" -c user.email="%USER_EMAIL%" checkout -B "%FEATURE_BRANCH%"

:: ---- 配置 remote ----
set "SSH_URL=git@github.com:%REPO%.git"
"%GIT_EXE%" remote get-url origin >nul 2>nul
if errorlevel 1 (
    "%GIT_EXE%" remote add origin "%SSH_URL%"
) else (
    "%GIT_EXE%" remote set-url origin "%SSH_URL%"
)
echo [OK] remote: %SSH_URL%

:: ---- add + commit ----
echo [*] git add -A ...
"%GIT_EXE%" add -A

echo [*] git commit ...
"%GIT_EXE%" -c user.name="%USER_NAME%" -c user.email="%USER_EMAIL%" commit -m "machine room update"

:: ---- push ----
echo.
echo [*] git push 到 %REPO% 分支 %FEATURE_BRANCH% ...
"%GIT_EXE%" push -u origin "%FEATURE_BRANCH%" --force
if errorlevel 1 (
    echo.
    echo [X] push 失败，请检查上方错误信息。
    goto :cleanup
)

:: ---- 打印 PR 链接 ----
echo.
echo ============================================================
echo [OK] 推送完成！
echo [*] 请打开以下链接创建 Pull Request（合并申请）：
echo.
echo     https://github.com/%REPO%/compare/%BASE_BRANCH%...%FEATURE_BRANCH%?expand=1
echo.
echo ============================================================

:cleanup
if exist "%TMP_KEY%" del /f /q "%TMP_KEY%"
echo [OK] 临时私钥已删除

:end
echo.
echo 按任意键关闭窗口...
pause >nul
endlocal
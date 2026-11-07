@echo off
rem ============================================================
rem  Godox Key Studio - launch the visual firmware patching UI
rem  ============================================================
chcp 65001 >nul
setlocal
cd /d "%~dp0.."

set "PY=%~dp0..\tools\python\python.exe"
if not exist "%PY%" set "PY=python"

echo.
echo   Godox Key Studio
echo   ================
echo   Python : %PY%
echo   Server : http://127.0.0.1:18780/
echo.
"%PY%" "%~dp0..\studio\server.py" --port 18780 --open
if errorlevel 1 (
  echo.
  echo [FAILED] The studio server exited with an error. See messages above.
  pause
)

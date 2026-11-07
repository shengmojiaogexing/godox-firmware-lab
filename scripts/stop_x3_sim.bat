@echo off
rem X3-C firmware simulator stopper (moved from repo root)
chcp 65001 >nul
cd /d "%~dp0.."
if not exist "devices\x3c\emu\stop_x3_sim.ps1" (
  echo [FAILED] devices\x3c\emu\stop_x3_sim.ps1 not found.
  pause
  exit /b 1
)
powershell -NoProfile -ExecutionPolicy Bypass -File "devices\x3c\emu\stop_x3_sim.ps1"
ping -n 3 127.0.0.1 >nul

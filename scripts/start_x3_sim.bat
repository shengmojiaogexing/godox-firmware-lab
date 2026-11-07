@echo off
rem X3-C firmware simulator launcher (moved from repo root)
chcp 65001 >nul
cd /d "%~dp0.."
if not exist "devices\x3c\emu\start_x3_sim.ps1" (
  echo [FAILED] devices\x3c\emu\start_x3_sim.ps1 not found.
  pause
  exit /b 1
)
powershell -NoProfile -ExecutionPolicy Bypass -File "devices\x3c\emu\start_x3_sim.ps1"

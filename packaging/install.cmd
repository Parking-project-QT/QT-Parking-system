@echo off
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%~dp0install.ps1"
if errorlevel 1 (
  echo QT Parking installation failed.
  pause
  exit /b 1
)
exit /b 0

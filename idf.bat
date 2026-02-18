@echo off

rem ### idf.bat documentation ###
rem Runs ESP-IDF export if needed, then forwards arguments to idf.py.

if not defined IDF_EXPORT (
  set "IDF_EXPORT=%userprofile%\esp\v5.5.3\esp-idf\export.bat"
)

if not exist "%IDF_EXPORT%" (
  echo ERROR: Datei nicht gefunden: "%IDF_EXPORT%"
  exit /b 1
)

if not defined IDF_PATH (
  echo "Try to run %IDF_EXPORT%.."
  call "%IDF_EXPORT%"
)

idf.py %*

@echo off

rem ### idf.bat documentation ###
rem Runs ESP-IDF export if needed, then forwards arguments to idf.py.

if not defined IDF_EXPORT (
  if defined IDF_PATH (
    set "IDF_EXPORT=%IDF_PATH%\export.bat"
  ) else (
    set "IDF_EXPORT=%userprofile%\esp\v5.5.3\esp-idf\export.bat"
  )
)

if not exist "%IDF_EXPORT%" (
  echo ERROR: Datei nicht gefunden: "%IDF_EXPORT%"
  exit /b 1
)

echo "Loading ESP-IDF from %IDF_EXPORT%.."
call "%IDF_EXPORT%"
if errorlevel 1 exit /b %errorlevel%

idf.py %*

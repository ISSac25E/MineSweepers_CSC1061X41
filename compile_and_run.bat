@echo off
:: GCC must be included into Windows path variables for this script to work
:: Otherwise, update 'g++' with the FULL executable path
g++ main.cpp source_files/*.cpp -o main

IF %ERRORLEVEL% NEQ 0 (
    echo Compilation failed.
    exit /b %ERRORLEVEL%
)

.\main

IF %ERRORLEVEL% NEQ 0 (
    echo Program execution failed.
    exit /b %ERRORLEVEL%
)

echo Program executed successfully.
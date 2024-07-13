@echo off
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
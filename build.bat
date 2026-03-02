@echo off
setlocal

echo === Warrant Calculator Build ===
echo.

:: Try to find vcvars64.bat
set "VCVARS="
for %%p in (
    "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
    "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat"
    "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars64.bat"
    "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
) do (
    if exist %%p (
        set "VCVARS=%%~p"
        goto :found
    )
)

echo ERROR: Could not find Visual Studio 2022 or Build Tools.
echo Please install Visual Studio 2022 or Build Tools with C++ workload.
exit /b 1

:found
echo Using: %VCVARS%
call "%VCVARS%" >nul 2>&1

echo Compiling...
if not exist "build_out" mkdir build_out

rc /fo build_out\app.res src\app.rc
if errorlevel 1 (
    echo ERROR: Resource compilation failed.
    exit /b 1
)

cl /std:c++20 /O2 /MT /DUNICODE /D_UNICODE /DNOMINMAX /EHsc /W4 ^
   /I src ^
   /Fe:build_out\WarrantCalculator.exe ^
   src\main.cpp src\app_window.cpp src\calculator.cpp ^
   src\date_utils.cpp src\currency.cpp ^
   build_out\app.res ^
   /link user32.lib gdi32.lib comctl32.lib kernel32.lib /SUBSYSTEM:WINDOWS
if errorlevel 1 (
    echo ERROR: Compilation failed.
    exit /b 1
)

:: Clean up intermediate files
del /q *.obj 2>nul

echo.
echo === Build successful! ===
echo Output: build_out\WarrantCalculator.exe

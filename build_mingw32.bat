@echo off
setlocal
rem ============================================================
rem  HighGrow 4.20 retail (this tree) - MinGW-w64 32-bit build
rem  Toolchain: MSYS2 mingw32 env (C:\msys64\mingw32\bin).
rem
rem  File list follows HIGHGROW.dsp (35 C files + highgrow.C +
rem  highgrow.RC + JPGView.cpp + Picture.cpp). NOT compiled:
rem    - JPGViewStub.c  (no-op stub superseded by real JPGView.cpp)
rem    - SEEDVALS.C / ccscode.c (not in the .dsp)
rem
rem  Patches this tree carries (vs the original retail source):
rem    - StdAfx.h: MFC precompiled header replaced by a minimal
rem      non-MFC stub (only JPGView.cpp / Picture.cpp include it)
rem    - Picture.h/.cpp: MFC/ATL CPicture replaced with a
rem      pure Win32 + GDI+ port (loads via IStream, renders to HDC)
rem    - JPGView.cpp: CDC attach/detach call sites replaced with
rem      direct HDC render calls
rem    - highgrow.RC: main menu resource renamed MENU -> MAINMENURESID
rem      (windres rejects the name MENU); highgrow.C lpszMenuName
rem      updated to match
rem
rem  CRITICAL: the i686 tools die with 0xC000007B if a 64-bit mingw
rem  (e.g. Git's C:\Program Files\Git\mingw64) is earlier on PATH -
rem  cc1.exe would load a 64-bit zlib1.dll. Keep the i686 bin first.
rem ============================================================
set "PATH=C:\msys64\mingw32\bin;%PATH%"
set "ROOT=%~dp0"
if "%ROOT:~-1%"=="\" set "ROOT=%ROOT:~0,-1%"
set "SRC=%ROOT%\HighGrow 420"
set "OBJ=%ROOT%\_build\obj32"
set "MINGW=C:\msys64\mingw32\bin"
set "GCC=%MINGW%\i686-w64-mingw32-gcc.exe"
set "GXX=%MINGW%\i686-w64-mingw32-g++.exe"
set "MT=C:\hgkit\bin\10.0.19041.0\x86\mt.exe"

if not exist "%OBJ%" mkdir "%OBJ%"
del /q "%OBJ%\*.o" 2>nul

echo [1/4] windres: highgrow.RC -> coff object (32-bit; includes the 4 IMAGE room JPGs)
"%MINGW%\windres.exe" -O coff -I "%SRC%" -o "%OBJ%\highgrow_res.o" "%SRC%\highgrow.RC"
if errorlevel 1 exit /b 1

echo [2/4] compile C sources
set CFAIL=
for %%f in (activity.c Buds.c Calc.c Chart.c clock.c Comments.c crypt.c DibAPI.c DiBitmap.c Global.c GrowEdit.c GrowRoom.c Harvest.c History.c Internet.c Leaves.c LogExport.c midi.c NodeCalc.c Password.c PlantLog.c PlantMem.c PlantSeed.c Registry.c RobbieWin.c SeedImp.c SeedList.c SeedLog.c ToolBar.c TrayIcon.c Vacation.c VersInfo.c Zoomer.c) do (
  echo   %%f
  "%GCC%" -O2 -w -fcommon -I "%SRC%" -c "%SRC%\%%f" -o "%OBJ%\%%~nf.o"
  if errorlevel 1 set CFAIL=1
)
if defined CFAIL (echo C COMPILE FAILED & exit /b 1)

echo [3/4] compile C++ sources
echo   highgrow.C
"%GCC%" -O2 -w -fcommon -x c -I "%SRC%" -c "%SRC%\highgrow.C" -o "%OBJ%\main.o"
if errorlevel 1 exit /b 1
echo   JPGView.cpp
"%GXX%" -O2 -w -I "%SRC%" -c "%SRC%\JPGView.cpp" -o "%OBJ%\jpgview.o"
if errorlevel 1 exit /b 1
echo   Picture.cpp
"%GXX%" -O2 -w -I "%SRC%" -c "%SRC%\Picture.cpp" -o "%OBJ%\picture.o"
if errorlevel 1 exit /b 1

echo [4/4] link HighGrow32.exe
"%GXX%" -O2 -w -mwindows -o "%ROOT%\_build\HighGrow32.exe" "%OBJ%\*.o" -lwinmm -lcomctl32 -lwininet -lhtmlhelp -lole32 -loleaut32 -luuid -lcomdlg32 -lshlwapi -lshell32 -ladvapi32 -lgdi32 -lgdiplus -luser32 -lversion
if errorlevel 1 exit /b 1

rem original manifest declares processorArchitecture X86 - correct for this 32-bit exe
"%MT%" -manifest "%SRC%\HighGrow.exe.manifest" -outputresource:"%ROOT%\_build\HighGrow32.exe";1
if errorlevel 1 echo (manifest embed failed - exe still works without visual styles)

rem GCC runtime DLLs must sit next to the exe (or be on PATH)
copy /y "%MINGW%\libgcc_s_dw2-1.dll" "%ROOT%\_build\" >nul
copy /y "%MINGW%\libstdc++-6.dll"     "%ROOT%\_build\" >nul
copy /y "%MINGW%\libwinpthread-1.dll" "%ROOT%\_build\" >nul

echo.
echo ===== BUILD OK: %ROOT%\_build\HighGrow32.exe =====
echo (run from the _build folder, or copy _build\* into the game folder)
exit /b 0

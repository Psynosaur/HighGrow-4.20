# Microsoft Developer Studio Generated NMAKE File, Format Version 4.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101
# TARGTYPE "Win32 (MIPS) Application" 0x0501

!IF "$(CFG)" == ""
CFG=highgrow - Win32 (80x86) Debug
!MESSAGE No configuration specified.  Defaulting to highgrow - Win32 (80x86)\
 Debug.
!ENDIF 

!IF "$(CFG)" != "highgrow - Win32 (80x86) Release" && "$(CFG)" !=\
 "highgrow - Win32 (80x86) Debug" && "$(CFG)" != "highgrow - Win32 (MIPS) Debug"\
 && "$(CFG)" != "highgrow - Win32 (MIPS) Release" && "$(CFG)" !=\
 "highgrow - Win32 new Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "highgrow.MAK" CFG="highgrow - Win32 (80x86) Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "highgrow - Win32 (80x86) Release" (based on\
 "Win32 (x86) Application")
!MESSAGE "highgrow - Win32 (80x86) Debug" (based on "Win32 (x86) Application")
!MESSAGE "highgrow - Win32 (MIPS) Debug" (based on "Win32 (MIPS) Application")
!MESSAGE "highgrow - Win32 (MIPS) Release" (based on\
 "Win32 (MIPS) Application")
!MESSAGE "highgrow - Win32 new Release" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
# PROP Target_Last_Scanned "highgrow - Win32 new Release"

!IF  "$(CFG)" == "highgrow - Win32 (80x86) Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WinRel"
# PROP BASE Intermediate_Dir "WinRel"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "WinRel"
# PROP Intermediate_Dir "WinRel"
OUTDIR=.\WinRel
INTDIR=.\WinRel

ALL : "$(OUTDIR)\highgrow.exe" "$(OUTDIR)\highgrow.bsc"

CLEAN : 
	-@erase ".\WinRel\highgrow.bsc"
	-@erase ".\WinRel\Registry.sbr"
	-@erase ".\WinRel\midi.sbr"
	-@erase ".\WinRel\GrowRoom.sbr"
	-@erase ".\WinRel\PlantSeed.sbr"
	-@erase ".\WinRel\Leaves.sbr"
	-@erase ".\WinRel\clock.sbr"
	-@erase ".\WinRel\Password.sbr"
	-@erase ".\WinRel\highgrow.sbr"
	-@erase ".\WinRel\Harvest.sbr"
	-@erase ".\WinRel\History.sbr"
	-@erase ".\WinRel\activity.sbr"
	-@erase ".\WinRel\Chart.sbr"
	-@erase ".\WinRel\DiBitmap.sbr"
	-@erase ".\WinRel\PlantMem.sbr"
	-@erase ".\WinRel\Global.sbr"
	-@erase ".\WinRel\VersInfo.sbr"
	-@erase ".\WinRel\Calc.sbr"
	-@erase ".\WinRel\Buds.sbr"
	-@erase ".\WinRel\NodeCalc.sbr"
	-@erase ".\WinRel\ToolBar.sbr"
	-@erase ".\WinRel\PlantLog.sbr"
	-@erase ".\WinRel\TrayIcon.sbr"
	-@erase ".\WinRel\SeedList.sbr"
	-@erase ".\WinRel\SeedImp.sbr"
	-@erase ".\WinRel\RobbieWin.sbr"
	-@erase ".\WinRel\LogExport.sbr"
	-@erase ".\WinRel\Comments.sbr"
	-@erase ".\WinRel\SeedLog.sbr"
	-@erase ".\WinRel\highgrow.exe"
	-@erase ".\WinRel\SeedImp.obj"
	-@erase ".\WinRel\RobbieWin.obj"
	-@erase ".\WinRel\LogExport.obj"
	-@erase ".\WinRel\Comments.obj"
	-@erase ".\WinRel\SeedLog.obj"
	-@erase ".\WinRel\Registry.obj"
	-@erase ".\WinRel\midi.obj"
	-@erase ".\WinRel\GrowRoom.obj"
	-@erase ".\WinRel\PlantSeed.obj"
	-@erase ".\WinRel\Leaves.obj"
	-@erase ".\WinRel\clock.obj"
	-@erase ".\WinRel\Password.obj"
	-@erase ".\WinRel\highgrow.obj"
	-@erase ".\WinRel\Harvest.obj"
	-@erase ".\WinRel\History.obj"
	-@erase ".\WinRel\activity.obj"
	-@erase ".\WinRel\Chart.obj"
	-@erase ".\WinRel\DiBitmap.obj"
	-@erase ".\WinRel\PlantMem.obj"
	-@erase ".\WinRel\Global.obj"
	-@erase ".\WinRel\VersInfo.obj"
	-@erase ".\WinRel\Calc.obj"
	-@erase ".\WinRel\Buds.obj"
	-@erase ".\WinRel\NodeCalc.obj"
	-@erase ".\WinRel\ToolBar.obj"
	-@erase ".\WinRel\PlantLog.obj"
	-@erase ".\WinRel\TrayIcon.obj"
	-@erase ".\WinRel\SeedList.obj"
	-@erase ".\WinRel\highgrow.res"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /FR /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_MBCS" /FR"$(INTDIR)/" /Fp"$(INTDIR)/highgrow.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\WinRel/
CPP_SBRS=.\WinRel/

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

MTL=mktyplib.exe
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/highgrow.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# SUBTRACT BASE BSC32 /Iu
# ADD BSC32 /nologo
# SUBTRACT BSC32 /Iu
BSC32_FLAGS=/nologo /o"$(OUTDIR)/highgrow.bsc" 
BSC32_SBRS= \
	"$(INTDIR)/Registry.sbr" \
	"$(INTDIR)/midi.sbr" \
	"$(INTDIR)/GrowRoom.sbr" \
	"$(INTDIR)/PlantSeed.sbr" \
	"$(INTDIR)/Leaves.sbr" \
	"$(INTDIR)/clock.sbr" \
	"$(INTDIR)/Password.sbr" \
	"$(INTDIR)/highgrow.sbr" \
	"$(INTDIR)/Harvest.sbr" \
	"$(INTDIR)/History.sbr" \
	"$(INTDIR)/activity.sbr" \
	"$(INTDIR)/Chart.sbr" \
	"$(INTDIR)/DiBitmap.sbr" \
	"$(INTDIR)/PlantMem.sbr" \
	"$(INTDIR)/Global.sbr" \
	"$(INTDIR)/VersInfo.sbr" \
	"$(INTDIR)/Calc.sbr" \
	"$(INTDIR)/Buds.sbr" \
	"$(INTDIR)/NodeCalc.sbr" \
	"$(INTDIR)/ToolBar.sbr" \
	"$(INTDIR)/PlantLog.sbr" \
	"$(INTDIR)/TrayIcon.sbr" \
	"$(INTDIR)/SeedList.sbr" \
	"$(INTDIR)/SeedImp.sbr" \
	"$(INTDIR)/RobbieWin.sbr" \
	"$(INTDIR)/LogExport.sbr" \
	"$(INTDIR)/Comments.sbr" \
	"$(INTDIR)/SeedLog.sbr"

"$(OUTDIR)\highgrow.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib mfc30.lib mfco30.lib mfcd30.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 version.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comctl32.lib winmm.lib dibapi32.lib /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=version.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib\
 comctl32.lib winmm.lib dibapi32.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/highgrow.pdb" /machine:I386 /out:"$(OUTDIR)/highgrow.exe" 
LINK32_OBJS= \
	"$(INTDIR)/SeedImp.obj" \
	"$(INTDIR)/RobbieWin.obj" \
	"$(INTDIR)/LogExport.obj" \
	"$(INTDIR)/Comments.obj" \
	"$(INTDIR)/SeedLog.obj" \
	"$(INTDIR)/Registry.obj" \
	"$(INTDIR)/midi.obj" \
	"$(INTDIR)/GrowRoom.obj" \
	"$(INTDIR)/PlantSeed.obj" \
	"$(INTDIR)/Leaves.obj" \
	"$(INTDIR)/clock.obj" \
	"$(INTDIR)/Password.obj" \
	"$(INTDIR)/highgrow.obj" \
	"$(INTDIR)/Harvest.obj" \
	"$(INTDIR)/History.obj" \
	"$(INTDIR)/activity.obj" \
	"$(INTDIR)/Chart.obj" \
	"$(INTDIR)/DiBitmap.obj" \
	"$(INTDIR)/PlantMem.obj" \
	"$(INTDIR)/Global.obj" \
	"$(INTDIR)/VersInfo.obj" \
	"$(INTDIR)/Calc.obj" \
	"$(INTDIR)/Buds.obj" \
	"$(INTDIR)/NodeCalc.obj" \
	"$(INTDIR)/ToolBar.obj" \
	"$(INTDIR)/PlantLog.obj" \
	"$(INTDIR)/TrayIcon.obj" \
	"$(INTDIR)/SeedList.obj" \
	"$(INTDIR)/highgrow.res"

"$(OUTDIR)\highgrow.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "highgrow - Win32 (80x86) Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "WinDebug"
# PROP BASE Intermediate_Dir "WinDebug"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "WinDebug"
# PROP Intermediate_Dir "WinDebug"
OUTDIR=.\WinDebug
INTDIR=.\WinDebug

ALL : "$(OUTDIR)\highgrow.exe" "$(OUTDIR)\highgrow.bsc"

CLEAN : 
	-@erase ".\WinDebug\vc40.pdb"
	-@erase ".\WinDebug\vc40.idb"
	-@erase ".\WinDebug\highgrow.bsc"
	-@erase ".\WinDebug\midi.sbr"
	-@erase ".\WinDebug\RobbieWin.sbr"
	-@erase ".\WinDebug\highgrow.sbr"
	-@erase ".\WinDebug\ToolBar.sbr"
	-@erase ".\WinDebug\Chart.sbr"
	-@erase ".\WinDebug\Comments.sbr"
	-@erase ".\WinDebug\Calc.sbr"
	-@erase ".\WinDebug\Buds.sbr"
	-@erase ".\WinDebug\Leaves.sbr"
	-@erase ".\WinDebug\DiBitmap.sbr"
	-@erase ".\WinDebug\GrowRoom.sbr"
	-@erase ".\WinDebug\Password.sbr"
	-@erase ".\WinDebug\NodeCalc.sbr"
	-@erase ".\WinDebug\PlantLog.sbr"
	-@erase ".\WinDebug\TrayIcon.sbr"
	-@erase ".\WinDebug\LogExport.sbr"
	-@erase ".\WinDebug\activity.sbr"
	-@erase ".\WinDebug\SeedImp.sbr"
	-@erase ".\WinDebug\Global.sbr"
	-@erase ".\WinDebug\PlantSeed.sbr"
	-@erase ".\WinDebug\PlantMem.sbr"
	-@erase ".\WinDebug\Harvest.sbr"
	-@erase ".\WinDebug\History.sbr"
	-@erase ".\WinDebug\Registry.sbr"
	-@erase ".\WinDebug\SeedLog.sbr"
	-@erase ".\WinDebug\VersInfo.sbr"
	-@erase ".\WinDebug\clock.sbr"
	-@erase ".\WinDebug\SeedList.sbr"
	-@erase ".\WinDebug\highgrow.exe"
	-@erase ".\WinDebug\Registry.obj"
	-@erase ".\WinDebug\SeedLog.obj"
	-@erase ".\WinDebug\VersInfo.obj"
	-@erase ".\WinDebug\clock.obj"
	-@erase ".\WinDebug\SeedList.obj"
	-@erase ".\WinDebug\midi.obj"
	-@erase ".\WinDebug\RobbieWin.obj"
	-@erase ".\WinDebug\highgrow.obj"
	-@erase ".\WinDebug\ToolBar.obj"
	-@erase ".\WinDebug\Chart.obj"
	-@erase ".\WinDebug\Comments.obj"
	-@erase ".\WinDebug\Calc.obj"
	-@erase ".\WinDebug\Buds.obj"
	-@erase ".\WinDebug\Leaves.obj"
	-@erase ".\WinDebug\DiBitmap.obj"
	-@erase ".\WinDebug\GrowRoom.obj"
	-@erase ".\WinDebug\Password.obj"
	-@erase ".\WinDebug\NodeCalc.obj"
	-@erase ".\WinDebug\PlantLog.obj"
	-@erase ".\WinDebug\TrayIcon.obj"
	-@erase ".\WinDebug\LogExport.obj"
	-@erase ".\WinDebug\activity.obj"
	-@erase ".\WinDebug\SeedImp.obj"
	-@erase ".\WinDebug\Global.obj"
	-@erase ".\WinDebug\PlantSeed.obj"
	-@erase ".\WinDebug\PlantMem.obj"
	-@erase ".\WinDebug\Harvest.obj"
	-@erase ".\WinDebug\History.obj"
	-@erase ".\WinDebug\highgrow.res"
	-@erase ".\WinDebug\highgrow.ilk"
	-@erase ".\WinDebug\highgrow.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /MD /W3 /GX /Zi /Od /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /FR /YX /c
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS"\
 /D "_MBCS" /FR"$(INTDIR)/" /Fp"$(INTDIR)/highgrow.pch" /YX /Fo"$(INTDIR)/"\
 /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\WinDebug/
CPP_SBRS=.\WinDebug/

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

MTL=mktyplib.exe
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/highgrow.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# SUBTRACT BASE BSC32 /Iu
# ADD BSC32 /nologo
# SUBTRACT BSC32 /Iu
BSC32_FLAGS=/nologo /o"$(OUTDIR)/highgrow.bsc" 
BSC32_SBRS= \
	"$(INTDIR)/midi.sbr" \
	"$(INTDIR)/RobbieWin.sbr" \
	"$(INTDIR)/highgrow.sbr" \
	"$(INTDIR)/ToolBar.sbr" \
	"$(INTDIR)/Chart.sbr" \
	"$(INTDIR)/Comments.sbr" \
	"$(INTDIR)/Calc.sbr" \
	"$(INTDIR)/Buds.sbr" \
	"$(INTDIR)/Leaves.sbr" \
	"$(INTDIR)/DiBitmap.sbr" \
	"$(INTDIR)/GrowRoom.sbr" \
	"$(INTDIR)/Password.sbr" \
	"$(INTDIR)/NodeCalc.sbr" \
	"$(INTDIR)/PlantLog.sbr" \
	"$(INTDIR)/TrayIcon.sbr" \
	"$(INTDIR)/LogExport.sbr" \
	"$(INTDIR)/activity.sbr" \
	"$(INTDIR)/SeedImp.sbr" \
	"$(INTDIR)/Global.sbr" \
	"$(INTDIR)/PlantSeed.sbr" \
	"$(INTDIR)/PlantMem.sbr" \
	"$(INTDIR)/Harvest.sbr" \
	"$(INTDIR)/History.sbr" \
	"$(INTDIR)/Registry.sbr" \
	"$(INTDIR)/SeedLog.sbr" \
	"$(INTDIR)/VersInfo.sbr" \
	"$(INTDIR)/clock.sbr" \
	"$(INTDIR)/SeedList.sbr"

"$(OUTDIR)\highgrow.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib mfc30d.lib mfco30d.lib mfcd30d.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 version.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comctl32.lib winmm.lib dibapi32.lib /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=version.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib\
 comctl32.lib winmm.lib dibapi32.lib /nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)/highgrow.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/highgrow.exe" 
LINK32_OBJS= \
	"$(INTDIR)/Registry.obj" \
	"$(INTDIR)/SeedLog.obj" \
	"$(INTDIR)/VersInfo.obj" \
	"$(INTDIR)/clock.obj" \
	"$(INTDIR)/SeedList.obj" \
	"$(INTDIR)/midi.obj" \
	"$(INTDIR)/RobbieWin.obj" \
	"$(INTDIR)/highgrow.obj" \
	"$(INTDIR)/ToolBar.obj" \
	"$(INTDIR)/Chart.obj" \
	"$(INTDIR)/Comments.obj" \
	"$(INTDIR)/Calc.obj" \
	"$(INTDIR)/Buds.obj" \
	"$(INTDIR)/Leaves.obj" \
	"$(INTDIR)/DiBitmap.obj" \
	"$(INTDIR)/GrowRoom.obj" \
	"$(INTDIR)/Password.obj" \
	"$(INTDIR)/NodeCalc.obj" \
	"$(INTDIR)/PlantLog.obj" \
	"$(INTDIR)/TrayIcon.obj" \
	"$(INTDIR)/LogExport.obj" \
	"$(INTDIR)/activity.obj" \
	"$(INTDIR)/SeedImp.obj" \
	"$(INTDIR)/Global.obj" \
	"$(INTDIR)/PlantSeed.obj" \
	"$(INTDIR)/PlantMem.obj" \
	"$(INTDIR)/Harvest.obj" \
	"$(INTDIR)/History.obj" \
	"$(INTDIR)/highgrow.res"

"$(OUTDIR)\highgrow.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "highgrow - Win32 (MIPS) Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "WinDebug"
# PROP Intermediate_Dir "WinDebug"
OUTDIR=.\WinDebug
INTDIR=.\WinDebug

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

ALL : $(OUTDIR)/generic.exe $(OUTDIR)/generic.bsc

MTL=MkTypLib.exe
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32  
CPP=cl.exe
# ADD BASE CPP /nologo /MD /Gt0 /QMOb2000 /W3 /GX /Zi /YX /Od /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /c
# ADD CPP /nologo /Gt0 /QMOb2000 /W3 /GX /Zi /YX /Od /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "WIN32" /FR /c
CPP_PROJ=/nologo /Gt0 /QMOb2000 /W3 /GX /Zi /YX /Od /D "_DEBUG" /D "_WINDOWS"\
 /D "_MBCS" /D "WIN32" /FR$(INTDIR)/ /Fp$(OUTDIR)/"generic.pch" /Fo$(INTDIR)/\
 /Fd$(OUTDIR)/"generic.pdb" /c 
CPP_OBJS=.\WinDebug/

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo$(INTDIR)/"generic.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# SUBTRACT BASE BSC32 /Iu
# ADD BSC32 /nologo
# SUBTRACT BSC32 /Iu
BSC32_FLAGS=/nologo /o$(OUTDIR)/"generic.bsc" 
BSC32_SBRS= \
	$(INTDIR)/generic.sbr

$(OUTDIR)/generic.bsc : $(OUTDIR)  $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib odbc32.lib mfc30d.lib mfco30d.lib mfcd30d.lib oleaut32.lib uuid.lib /NOLOGO /SUBSYSTEM:windows /DEBUG /MACHINE:MIPS
# ADD LINK32 version.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /NOLOGO /SUBSYSTEM:windows /DEBUG /MACHINE:MIPS
LINK32_FLAGS=version.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /NOLOGO\
 /SUBSYSTEM:windows /PDB:$(OUTDIR)/"generic.pdb" /DEBUG /MACHINE:MIPS\
 /OUT:$(OUTDIR)/"generic.exe" 
DEF_FILE=
LINK32_OBJS= \
	$(INTDIR)/generic.res \
	$(INTDIR)/generic.obj

$(OUTDIR)/generic.exe : $(OUTDIR)  $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "highgrow - Win32 (MIPS) Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "WinRel"
# PROP Intermediate_Dir "WinRel"
OUTDIR=.\WinRel
INTDIR=.\WinRel

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

ALL : $(OUTDIR)/generic.exe $(OUTDIR)/generic.bsc

MTL=MkTypLib.exe
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
CPP=cl.exe
# ADD BASE CPP /nologo /MD /Gt0 /QMOb2000 /W3 /GX /YX /O2 /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /c
# ADD CPP /nologo /Gt0 /QMOb2000 /W3 /GX /YX /O2 /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "WIN32" /FR /c
CPP_PROJ=/nologo /Gt0 /QMOb2000 /W3 /GX /YX /O2 /D "NDEBUG" /D "_WINDOWS" /D\
 "_MBCS" /D "WIN32" /FR$(INTDIR)/ /Fp$(OUTDIR)/"generic.pch" /Fo$(INTDIR)/ /c 
CPP_OBJS=.\WinRel/

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo$(INTDIR)/"generic.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# SUBTRACT BASE BSC32 /Iu
# ADD BSC32 /nologo
# SUBTRACT BSC32 /Iu
BSC32_FLAGS=/nologo /o$(OUTDIR)/"generic.bsc" 
BSC32_SBRS= \
	$(INTDIR)/generic.sbr

$(OUTDIR)/generic.bsc : $(OUTDIR)  $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib odbc32.lib mfc30.lib mfco30.lib mfcd30.lib oleaut32.lib uuid.lib /NOLOGO /SUBSYSTEM:windows /MACHINE:MIPS
# ADD LINK32 version.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /NOLOGO /SUBSYSTEM:windows /MACHINE:MIPS
LINK32_FLAGS=version.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /NOLOGO\
 /SUBSYSTEM:windows /PDB:$(OUTDIR)/"generic.pdb" /MACHINE:MIPS\
 /OUT:$(OUTDIR)/"generic.exe" 
DEF_FILE=
LINK32_OBJS= \
	$(INTDIR)/generic.res \
	$(INTDIR)/generic.obj

$(OUTDIR)/generic.exe : $(OUTDIR)  $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "highgrow - Win32 new Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "highgrow"
# PROP BASE Intermediate_Dir "highgrow"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "highgrow"
# PROP Intermediate_Dir "highgrow"
# PROP Target_Dir ""
OUTDIR=.\highgrow
INTDIR=.\highgrow

ALL : "$(OUTDIR)\highgrow.exe" "$(OUTDIR)\highgrow.bsc"

CLEAN : 
	-@erase ".\highgrow\highgrow.bsc"
	-@erase ".\highgrow\Leaves.sbr"
	-@erase ".\highgrow\SeedList.sbr"
	-@erase ".\highgrow\highgrow.sbr"
	-@erase ".\highgrow\SeedImp.sbr"
	-@erase ".\highgrow\Harvest.sbr"
	-@erase ".\highgrow\Comments.sbr"
	-@erase ".\highgrow\SeedLog.sbr"
	-@erase ".\highgrow\RobbieWin.sbr"
	-@erase ".\highgrow\DiBitmap.sbr"
	-@erase ".\highgrow\GrowRoom.sbr"
	-@erase ".\highgrow\Calc.sbr"
	-@erase ".\highgrow\Buds.sbr"
	-@erase ".\highgrow\Global.sbr"
	-@erase ".\highgrow\PlantSeed.sbr"
	-@erase ".\highgrow\Password.sbr"
	-@erase ".\highgrow\NodeCalc.sbr"
	-@erase ".\highgrow\clock.sbr"
	-@erase ".\highgrow\PlantLog.sbr"
	-@erase ".\highgrow\TrayIcon.sbr"
	-@erase ".\highgrow\History.sbr"
	-@erase ".\highgrow\activity.sbr"
	-@erase ".\highgrow\Chart.sbr"
	-@erase ".\highgrow\LogExport.sbr"
	-@erase ".\highgrow\PlantMem.sbr"
	-@erase ".\highgrow\midi.sbr"
	-@erase ".\highgrow\Registry.sbr"
	-@erase ".\highgrow\VersInfo.sbr"
	-@erase ".\highgrow\ToolBar.sbr"
	-@erase ".\highgrow\highgrow.exe"
	-@erase ".\highgrow\Registry.obj"
	-@erase ".\highgrow\VersInfo.obj"
	-@erase ".\highgrow\ToolBar.obj"
	-@erase ".\highgrow\Leaves.obj"
	-@erase ".\highgrow\SeedList.obj"
	-@erase ".\highgrow\highgrow.obj"
	-@erase ".\highgrow\SeedImp.obj"
	-@erase ".\highgrow\Harvest.obj"
	-@erase ".\highgrow\Comments.obj"
	-@erase ".\highgrow\SeedLog.obj"
	-@erase ".\highgrow\RobbieWin.obj"
	-@erase ".\highgrow\DiBitmap.obj"
	-@erase ".\highgrow\GrowRoom.obj"
	-@erase ".\highgrow\Calc.obj"
	-@erase ".\highgrow\Buds.obj"
	-@erase ".\highgrow\Global.obj"
	-@erase ".\highgrow\PlantSeed.obj"
	-@erase ".\highgrow\Password.obj"
	-@erase ".\highgrow\NodeCalc.obj"
	-@erase ".\highgrow\clock.obj"
	-@erase ".\highgrow\PlantLog.obj"
	-@erase ".\highgrow\TrayIcon.obj"
	-@erase ".\highgrow\History.obj"
	-@erase ".\highgrow\activity.obj"
	-@erase ".\highgrow\Chart.obj"
	-@erase ".\highgrow\LogExport.obj"
	-@erase ".\highgrow\PlantMem.obj"
	-@erase ".\highgrow\midi.obj"
	-@erase ".\highgrow\highgrow.res"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /FR /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /FR /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_MBCS" /FR"$(INTDIR)/" /Fp"$(INTDIR)/highgrow.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\highgrow/
CPP_SBRS=.\highgrow/

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

MTL=mktyplib.exe
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/highgrow.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# SUBTRACT BASE BSC32 /Iu
# ADD BSC32 /nologo
# SUBTRACT BSC32 /Iu
BSC32_FLAGS=/nologo /o"$(OUTDIR)/highgrow.bsc" 
BSC32_SBRS= \
	"$(INTDIR)/Leaves.sbr" \
	"$(INTDIR)/SeedList.sbr" \
	"$(INTDIR)/highgrow.sbr" \
	"$(INTDIR)/SeedImp.sbr" \
	"$(INTDIR)/Harvest.sbr" \
	"$(INTDIR)/Comments.sbr" \
	"$(INTDIR)/SeedLog.sbr" \
	"$(INTDIR)/RobbieWin.sbr" \
	"$(INTDIR)/DiBitmap.sbr" \
	"$(INTDIR)/GrowRoom.sbr" \
	"$(INTDIR)/Calc.sbr" \
	"$(INTDIR)/Buds.sbr" \
	"$(INTDIR)/Global.sbr" \
	"$(INTDIR)/PlantSeed.sbr" \
	"$(INTDIR)/Password.sbr" \
	"$(INTDIR)/NodeCalc.sbr" \
	"$(INTDIR)/clock.sbr" \
	"$(INTDIR)/PlantLog.sbr" \
	"$(INTDIR)/TrayIcon.sbr" \
	"$(INTDIR)/History.sbr" \
	"$(INTDIR)/activity.sbr" \
	"$(INTDIR)/Chart.sbr" \
	"$(INTDIR)/LogExport.sbr" \
	"$(INTDIR)/PlantMem.sbr" \
	"$(INTDIR)/midi.sbr" \
	"$(INTDIR)/Registry.sbr" \
	"$(INTDIR)/VersInfo.sbr" \
	"$(INTDIR)/ToolBar.sbr"

"$(OUTDIR)\highgrow.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 version.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comctl32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 version.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comctl32.lib winmm.lib /nologo /subsystem:windows /machine:I386
# SUBTRACT LINK32 /incremental:yes /nodefaultlib
LINK32_FLAGS=version.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib\
 comctl32.lib winmm.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/highgrow.pdb" /machine:I386 /out:"$(OUTDIR)/highgrow.exe" 
LINK32_OBJS= \
	"$(INTDIR)/Registry.obj" \
	"$(INTDIR)/VersInfo.obj" \
	"$(INTDIR)/ToolBar.obj" \
	"$(INTDIR)/Leaves.obj" \
	"$(INTDIR)/SeedList.obj" \
	"$(INTDIR)/highgrow.obj" \
	"$(INTDIR)/SeedImp.obj" \
	"$(INTDIR)/Harvest.obj" \
	"$(INTDIR)/Comments.obj" \
	"$(INTDIR)/SeedLog.obj" \
	"$(INTDIR)/RobbieWin.obj" \
	"$(INTDIR)/DiBitmap.obj" \
	"$(INTDIR)/GrowRoom.obj" \
	"$(INTDIR)/Calc.obj" \
	"$(INTDIR)/Buds.obj" \
	"$(INTDIR)/Global.obj" \
	"$(INTDIR)/PlantSeed.obj" \
	"$(INTDIR)/Password.obj" \
	"$(INTDIR)/NodeCalc.obj" \
	"$(INTDIR)/clock.obj" \
	"$(INTDIR)/PlantLog.obj" \
	"$(INTDIR)/TrayIcon.obj" \
	"$(INTDIR)/History.obj" \
	"$(INTDIR)/activity.obj" \
	"$(INTDIR)/Chart.obj" \
	"$(INTDIR)/LogExport.obj" \
	"$(INTDIR)/PlantMem.obj" \
	"$(INTDIR)/midi.obj" \
	"$(INTDIR)/highgrow.res"

"$(OUTDIR)\highgrow.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

################################################################################
# Begin Target

# Name "highgrow - Win32 (80x86) Release"
# Name "highgrow - Win32 (80x86) Debug"
# Name "highgrow - Win32 (MIPS) Debug"
# Name "highgrow - Win32 (MIPS) Release"
# Name "highgrow - Win32 new Release"

!IF  "$(CFG)" == "highgrow - Win32 (80x86) Release"

!ELSEIF  "$(CFG)" == "highgrow - Win32 (80x86) Debug"

!ELSEIF  "$(CFG)" == "highgrow - Win32 (MIPS) Debug"

!ELSEIF  "$(CFG)" == "highgrow - Win32 (MIPS) Release"

!ELSEIF  "$(CFG)" == "highgrow - Win32 new Release"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\highgrow.RC

!IF  "$(CFG)" == "highgrow - Win32 (80x86) Release"

DEP_RSC_HIGHG=\
	".\openhand.cur"\
	".\closedha.cur"\
	".\cur00001.cur"\
	".\scissors.cur"\
	".\linkhand.cur"\
	".\highgrow.ICO"\
	".\Water.ico"\
	".\Light.ico"\
	".\Status.ico"\
	".\icon1.ico"\
	".\ico00001.ico"\
	".\Appleaf.ico"\
	".\Leaf.bmp"\
	".\GrowRoom.bmp"\
	".\Lshade.bmp"\
	".\Llight.bmp"\
	".\Title.bmp"\
	".\toolbar.bmp"\
	".\seedlist.bmp"\
	".\black.bmp"\
	".\Lfrd.bmp"\
	".\Lfla.bmp"\
	".\LFLB.bmp"\
	".\LFLC.bmp"\
	".\Lfld.bmp"\
	".\Lfra.bmp"\
	".\LFRB.bmp"\
	".\LFRC.bmp"\
	".\leaftile.bmp"\
	".\LFEU.bmp"\
	".\LFED.bmp"\
	".\PassLeaf.bmp"\
	".\NoEntry.bmp"\
	".\slick.bmp"\
	".\Leaf256.bmp"\
	".\bdf.bmp"\
	".\dlgleaf.bmp"\
	".\DlgLeaf.ico"\
	".\app_icon.ico"\
	

"$(INTDIR)\highgrow.res" : $(SOURCE) $(DEP_RSC_HIGHG) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "highgrow - Win32 (80x86) Debug"

DEP_RSC_HIGHG=\
	".\openhand.cur"\
	".\closedha.cur"\
	".\cur00001.cur"\
	".\scissors.cur"\
	".\linkhand.cur"\
	".\highgrow.ICO"\
	".\Water.ico"\
	".\Light.ico"\
	".\Status.ico"\
	".\icon1.ico"\
	".\ico00001.ico"\
	".\Appleaf.ico"\
	".\Leaf.bmp"\
	".\GrowRoom.bmp"\
	".\Lshade.bmp"\
	".\Llight.bmp"\
	".\Title.bmp"\
	".\toolbar.bmp"\
	".\seedlist.bmp"\
	".\black.bmp"\
	".\Lfrd.bmp"\
	".\Lfla.bmp"\
	".\LFLB.bmp"\
	".\LFLC.bmp"\
	".\Lfld.bmp"\
	".\Lfra.bmp"\
	".\LFRB.bmp"\
	".\LFRC.bmp"\
	".\leaftile.bmp"\
	".\LFEU.bmp"\
	".\LFED.bmp"\
	".\PassLeaf.bmp"\
	".\NoEntry.bmp"\
	".\slick.bmp"\
	".\Leaf256.bmp"\
	".\bdf.bmp"\
	".\dlgleaf.bmp"\
	".\DlgLeaf.ico"\
	".\app_icon.ico"\
	

"$(INTDIR)\highgrow.res" : $(SOURCE) $(DEP_RSC_HIGHG) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "highgrow - Win32 (MIPS) Debug"

!ELSEIF  "$(CFG)" == "highgrow - Win32 (MIPS) Release"

!ELSEIF  "$(CFG)" == "highgrow - Win32 new Release"

DEP_RSC_HIGHG=\
	".\openhand.cur"\
	".\closedha.cur"\
	".\cur00001.cur"\
	".\scissors.cur"\
	".\linkhand.cur"\
	".\highgrow.ICO"\
	".\Water.ico"\
	".\Light.ico"\
	".\Status.ico"\
	".\icon1.ico"\
	".\ico00001.ico"\
	".\Appleaf.ico"\
	".\Leaf.bmp"\
	".\GrowRoom.bmp"\
	".\Lshade.bmp"\
	".\Llight.bmp"\
	".\Title.bmp"\
	".\toolbar.bmp"\
	".\seedlist.bmp"\
	".\black.bmp"\
	".\Lfrd.bmp"\
	".\Lfla.bmp"\
	".\LFLB.bmp"\
	".\LFLC.bmp"\
	".\Lfld.bmp"\
	".\Lfra.bmp"\
	".\LFRB.bmp"\
	".\LFRC.bmp"\
	".\leaftile.bmp"\
	".\LFEU.bmp"\
	".\LFED.bmp"\
	".\PassLeaf.bmp"\
	".\NoEntry.bmp"\
	".\slick.bmp"\
	".\Leaf256.bmp"\
	".\bdf.bmp"\
	".\dlgleaf.bmp"\
	".\DlgLeaf.ico"\
	".\app_icon.ico"\
	

"$(INTDIR)\highgrow.res" : $(SOURCE) $(DEP_RSC_HIGHG) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\highgrow.C

!IF  "$(CFG)" == "highgrow - Win32 (80x86) Release"

DEP_CPP_HIGHGR=\
	".\highgrow.H"\
	".\Global.h"\
	".\Calc.h"\
	".\activity.h"\
	".\PlantMem.h"\
	".\SeedList.h"\
	".\History.h"\
	".\GrowRoom.h"\
	".\ToolBar.h"\
	".\PlantSeed.h"\
	".\Robbie.h"\
	".\RobbieWin.h"\
	".\Comments.h"\
	".\Password.h"\
	".\DiBitmap.h"\
	".\Harvest.h"\
	".\PlantLog.h"\
	".\Midi.h"\
	".\TrayIcon.h"\
	".\Clock.h"\
	".\SeedImp.h"\
	

"$(INTDIR)\highgrow.obj" : $(SOURCE) $(DEP_CPP_HIGHGR) "$(INTDIR)"

"$(INTDIR)\highgrow.sbr" : $(SOURCE) $(DEP_CPP_HIGHGR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "highgrow - Win32 (80x86) Debug"

DEP_CPP_HIGHGR=\
	".\highgrow.H"\
	".\Global.h"\
	".\Calc.h"\
	".\activity.h"\
	".\PlantMem.h"\
	".\SeedList.h"\
	".\History.h"\
	".\GrowRoom.h"\
	".\ToolBar.h"\
	".\PlantSeed.h"\
	".\Robbie.h"\
	".\RobbieWin.h"\
	".\Comments.h"\
	".\Password.h"\
	".\DiBitmap.h"\
	".\Harvest.h"\
	".\PlantLog.h"\
	".\Midi.h"\
	".\TrayIcon.h"\
	".\Clock.h"\
	".\SeedImp.h"\
	

"$(INTDIR)\highgrow.obj" : $(SOURCE) $(DEP_CPP_HIGHGR) "$(INTDIR)"

"$(INTDIR)\highgrow.sbr" : $(SOURCE) $(DEP_CPP_HIGHGR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "highgrow - Win32 (MIPS) Debug"

!ELSEIF  "$(CFG)" == "highgrow - Win32 (MIPS) Release"

!ELSEIF  "$(CFG)" == "highgrow - Win32 new Release"

DEP_CPP_HIGHGR=\
	".\highgrow.H"\
	".\Global.h"\
	".\Calc.h"\
	".\activity.h"\
	".\PlantMem.h"\
	".\SeedList.h"\
	".\History.h"\
	".\GrowRoom.h"\
	".\ToolBar.h"\
	".\PlantSeed.h"\
	".\Robbie.h"\
	".\RobbieWin.h"\
	".\Comments.h"\
	".\Password.h"\
	".\DiBitmap.h"\
	".\Harvest.h"\
	".\PlantLog.h"\
	".\Midi.h"\
	".\TrayIcon.h"\
	".\Clock.h"\
	".\SeedImp.h"\
	

"$(INTDIR)\highgrow.obj" : $(SOURCE) $(DEP_CPP_HIGHGR) "$(INTDIR)"

"$(INTDIR)\highgrow.sbr" : $(SOURCE) $(DEP_CPP_HIGHGR) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\highgrow.txt

!IF  "$(CFG)" == "highgrow - Win32 (80x86) Release"

!ELSEIF  "$(CFG)" == "highgrow - Win32 (80x86) Debug"

!ELSEIF  "$(CFG)" == "highgrow - Win32 (MIPS) Debug"

!ELSEIF  "$(CFG)" == "highgrow - Win32 (MIPS) Release"

!ELSEIF  "$(CFG)" == "highgrow - Win32 new Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\activity.c

!IF  "$(CFG)" == "highgrow - Win32 (80x86) Release"

DEP_CPP_ACTIV=\
	".\Global.h"\
	".\PlantMem.h"\
	".\GrowRoom.h"\
	".\NodeCalc.h"\
	".\Calc.h"\
	

"$(INTDIR)\activity.obj" : $(SOURCE) $(DEP_CPP_ACTIV) "$(INTDIR)"

"$(INTDIR)\activity.sbr" : $(SOURCE) $(DEP_CPP_ACTIV) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "highgrow - Win32 (80x86) Debug"

DEP_CPP_ACTIV=\
	".\Global.h"\
	".\PlantMem.h"\
	".\GrowRoom.h"\
	".\NodeCalc.h"\
	".\Calc.h"\
	

"$(INTDIR)\activity.obj" : $(SOURCE) $(DEP_CPP_ACTIV) "$(INTDIR)"

"$(INTDIR)\activity.sbr" : $(SOURCE) $(DEP_CPP_ACTIV) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "highgrow - Win32 (MIPS) Debug"

!ELSEIF  "$(CFG)" == "highgrow - Win32 (MIPS) Release"

!ELSEIF  "$(CFG)" == "highgrow - Win32 new Release"

DEP_CPP_ACTIV=\
	".\Global.h"\
	".\PlantMem.h"\
	".\GrowRoom.h"\
	".\NodeCalc.h"\
	".\Calc.h"\
	

"$(INTDIR)\activity.obj" : $(SOURCE) $(DEP_CPP_ACTIV) "$(INTDIR)"

"$(INTDIR)\activity.sbr" : $(SOURCE) $(DEP_CPP_ACTIV) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Global.c

!IF  "$(CFG)" == "highgrow - Win32 (80x86) Release"

DEP_CPP_GLOBA=\
	".\Global.h"\
	".\Calc.h"\
	".\ToolBar.h"\
	".\PlantMem.h"\
	".\PlantLog.h"\
	".\Password.h"\
	".\Midi.h"\
	".\Clock.h"\
	

"$(INTDIR)\Global.obj" : $(SOURCE) $(DEP_CPP_GLOBA) "$(INTDIR)"

"$(INTDIR)\Global.sbr" : $(SOURCE) $(DEP_CPP_GLOBA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "highgrow - Win32 (80x86) Debug"

DEP_CPP_GLOBA=\
	".\Global.h"\
	".\Calc.h"\
	".\ToolBar.h"\
	".\PlantMem.h"\
	".\PlantLog.h"\
	".\Password.h"\
	".\Midi.h"\
	".\Clock.h"\
	

"$(INTDIR)\Global.obj" : $(SOURCE) $(DEP_CPP_GLOBA) "$(INTDIR)"

"$(INTDIR)\Global.sbr" : $(SOURCE) $(DEP_CPP_GLOBA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "highgrow - Win32 (MIPS) Debug"

!ELSEIF  "$(CFG)" == "highgrow - Win32 (MIPS) Release"

!ELSEIF  "$(CFG)" == "highgrow - Win32 new Release"

DEP_CPP_GLOBA=\
	".\Global.h"\
	".\Calc.h"\
	".\ToolBar.h"\
	".\PlantMem.h"\
	".\PlantLog.h"\
	".\Password.h"\
	".\Midi.h"\
	".\Clock.h"\
	

"$(INTDIR)\Global.obj" : $(SOURCE) $(DEP_CPP_GLOBA) "$(INTDIR)"

"$(INTDIR)\Global.sbr" : $(SOURCE) $(DEP_CPP_GLOBA) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\History.c

!IF  "$(CFG)" == "highgrow - Win32 (80x86) Release"

DEP_CPP_HISTO=\
	".\Global.h"\
	".\History.h"\
	".\Calc.h"\
	".\PlantMem.h"\
	".\SeedList.h"\
	".\ToolBar.h"\
	".\DiBitmap.h"\
	".\LogExport.h"\
	

"$(INTDIR)\History.obj" : $(SOURCE) $(DEP_CPP_HISTO) "$(INTDIR)"

"$(INTDIR)\History.sbr" : $(SOURCE) $(DEP_CPP_HISTO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "highgrow - Win32 (80x86) Debug"

DEP_CPP_HISTO=\
	".\Global.h"\
	".\History.h"\
	".\Calc.h"\
	".\PlantMem.h"\
	".\SeedList.h"\
	".\ToolBar.h"\
	".\DiBitmap.h"\
	".\LogExport.h"\
	

"$(INTDIR)\History.obj" : $(SOURCE) $(DEP_CPP_HISTO) "$(INTDIR)"

"$(INTDIR)\History.sbr" : $(SOURCE) $(DEP_CPP_HISTO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "highgrow - Win32 (MIPS) Debug"

!ELSEIF  "$(CFG)" == "highgrow - Win32 (MIPS) Release"

!ELSEIF  "$(CFG)" == "highgrow - Win32 new Release"

DEP_CPP_HISTO=\
	".\Global.h"\
	".\History.h"\
	".\Calc.h"\
	".\PlantMem.h"\
	".\SeedList.h"\
	".\ToolBar.h"\
	".\DiBitmap.h"\
	".\LogExport.h"\
	

"$(INTDIR)\History.obj" : $(SOURCE) $(DEP_CPP_HISTO) "$(INTDIR)"

"$(INTDIR)\History.sbr" : $(SOURCE) $(DEP_CPP_HISTO) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Calc.c

!IF  "$(CFG)" == "highgrow - Win32 (80x86) Release"

DEP_CPP_CALC_=\
	".\Global.h"\
	".\Calc.h"\
	".\PlantMem.h"\
	".\Password.h"\
	".\NodeCalc.h"\
	

"$(INTDIR)\Calc.obj" : $(SOURCE) $(DEP_CPP_CALC_) "$(INTDIR)"

"$(INTDIR)\Calc.sbr" : $(SOURCE) $(DEP_CPP_CALC_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "highgrow - Win32 (80x86) Debug"

DEP_CPP_CALC_=\
	".\Global.h"\
	".\Calc.h"\
	".\PlantMem.h"\
	".\Password.h"\
	".\NodeCalc.h"\
	

"$(INTDIR)\Calc.obj" : $(SOURCE) $(DEP_CPP_CALC_) "$(INTDIR)"

"$(INTDIR)\Calc.sbr" : $(SOURCE) $(DEP_CPP_CALC_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "highgrow - Win32 (MIPS) Debug"

!ELSEIF  "$(CFG)" == "highgrow - Win32 (MIPS) Release"

!ELSEIF  "$(CFG)" == "highgrow - Win32 new Release"

DEP_CPP_CALC_=\
	".\Global.h"\
	".\Calc.h"\
	".\PlantMem.h"\
	".\Password.h"\
	".\NodeCalc.h"\
	

"$(INTDIR)\Calc.obj" : $(SOURCE) $(DEP_CPP_CALC_) "$(INTDIR)"

"$(INTDIR)\Calc.sbr" : $(SOURCE) $(DEP_CPP_CALC_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\PlantMem.c

!IF  "$(CFG)" == "highgrow - Win32 (80x86) Release"

DEP_CPP_PLANT=\
	".\Global.h"\
	".\ToolBar.h"\
	

"$(INTDIR)\PlantMem.obj" : $(SOURCE) $(DEP_CPP_PLANT) "$(INTDIR)"

"$(INTDIR)\PlantMem.sbr" : $(SOURCE) $(DEP_CPP_PLANT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "highgrow - Win32 (80x86) Debug"

DEP_CPP_PLANT=\
	".\Global.h"\
	".\ToolBar.h"\
	

"$(INTDIR)\PlantMem.obj" : $(SOURCE) $(DEP_CPP_PLANT) "$(INTDIR)"

"$(INTDIR)\PlantMem.sbr" : $(SOURCE) $(DEP_CPP_PLANT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "highgrow - Win32 (MIPS) Debug"

!ELSEIF  "$(CFG)" == "highgrow - Win32 (MIPS) Release"

!ELSEIF  "$(CFG)" == "highgrow - Win32 new Release"

DEP_CPP_PLANT=\
	".\Global.h"\
	".\ToolBar.h"\
	

"$(INTDIR)\PlantMem.obj" : $(SOURCE) $(DEP_CPP_PLANT) "$(INTDIR)"

"$(INTDIR)\PlantMem.sbr" : $(SOURCE) $(DEP_CPP_PLANT) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\GrowRoom.c

!IF  "$(CFG)" == "highgrow - Win32 (80x86) Release"

DEP_CPP_GROWR=\
	".\Global.h"\
	".\ToolBar.h"\
	".\Calc.h"\
	".\SeedList.h"\
	".\PlantMem.h"\
	".\GrowRoom.h"\
	".\NodeCalc.h"\
	".\DiBitmap.h"\
	".\Leaves.h"\
	".\Buds.h"\
	".\Robbie.h"\
	".\VersInfo.h"\
	".\Password.h"\
	".\Clock.h"\
	

"$(INTDIR)\GrowRoom.obj" : $(SOURCE) $(DEP_CPP_GROWR) "$(INTDIR)"

"$(INTDIR)\GrowRoom.sbr" : $(SOURCE) $(DEP_CPP_GROWR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "highgrow - Win32 (80x86) Debug"

DEP_CPP_GROWR=\
	".\Global.h"\
	".\ToolBar.h"\
	".\Calc.h"\
	".\SeedList.h"\
	".\PlantMem.h"\
	".\GrowRoom.h"\
	".\NodeCalc.h"\
	".\DiBitmap.h"\
	".\Leaves.h"\
	".\Buds.h"\
	".\Robbie.h"\
	".\VersInfo.h"\
	".\Password.h"\
	".\Clock.h"\
	

"$(INTDIR)\GrowRoom.obj" : $(SOURCE) $(DEP_CPP_GROWR) "$(INTDIR)"

"$(INTDIR)\GrowRoom.sbr" : $(SOURCE) $(DEP_CPP_GROWR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "highgrow - Win32 (MIPS) Debug"

!ELSEIF  "$(CFG)" == "highgrow - Win32 (MIPS) Release"

!ELSEIF  "$(CFG)" == "highgrow - Win32 new Release"

DEP_CPP_GROWR=\
	".\Global.h"\
	".\ToolBar.h"\
	".\Calc.h"\
	".\SeedList.h"\
	".\PlantMem.h"\
	".\GrowRoom.h"\
	".\NodeCalc.h"\
	".\DiBitmap.h"\
	".\Leaves.h"\
	".\Buds.h"\
	".\Robbie.h"\
	".\VersInfo.h"\
	".\Password.h"\
	".\Clock.h"\
	

"$(INTDIR)\GrowRoom.obj" : $(SOURCE) $(DEP_CPP_GROWR) "$(INTDIR)"

"$(INTDIR)\GrowRoom.sbr" : $(SOURCE) $(DEP_CPP_GROWR) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\NodeCalc.c

!IF  "$(CFG)" == "highgrow - Win32 (80x86) Release"

DEP_CPP_NODEC=\
	".\Global.h"\
	".\Calc.h"\
	".\NodeCalc.h"\
	".\GrowRoom.h"\
	".\PlantMem.h"\
	".\Leaves.h"\
	

"$(INTDIR)\NodeCalc.obj" : $(SOURCE) $(DEP_CPP_NODEC) "$(INTDIR)"

"$(INTDIR)\NodeCalc.sbr" : $(SOURCE) $(DEP_CPP_NODEC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "highgrow - Win32 (80x86) Debug"

DEP_CPP_NODEC=\
	".\Global.h"\
	".\Calc.h"\
	".\NodeCalc.h"\
	".\GrowRoom.h"\
	".\PlantMem.h"\
	".\Leaves.h"\
	

"$(INTDIR)\NodeCalc.obj" : $(SOURCE) $(DEP_CPP_NODEC) "$(INTDIR)"

"$(INTDIR)\NodeCalc.sbr" : $(SOURCE) $(DEP_CPP_NODEC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "highgrow - Win32 (MIPS) Debug"

!ELSEIF  "$(CFG)" == "highgrow - Win32 (MIPS) Release"

!ELSEIF  "$(CFG)" == "highgrow - Win32 new Release"

DEP_CPP_NODEC=\
	".\Global.h"\
	".\Calc.h"\
	".\NodeCalc.h"\
	".\GrowRoom.h"\
	".\PlantMem.h"\
	".\Leaves.h"\
	

"$(INTDIR)\NodeCalc.obj" : $(SOURCE) $(DEP_CPP_NODEC) "$(INTDIR)"

"$(INTDIR)\NodeCalc.sbr" : $(SOURCE) $(DEP_CPP_NODEC) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\ToolBar.c

!IF  "$(CFG)" == "highgrow - Win32 (80x86) Release"

DEP_CPP_TOOLB=\
	".\ToolBar.h"\
	".\Global.h"\
	".\activity.h"\
	".\History.h"\
	".\Chart.h"\
	".\SeedList.h"\
	".\GrowRoom.h"\
	".\Harvest.h"\
	".\RobbieWin.h"\
	".\Registry.h"\
	".\Password.h"\
	".\Midi.h"\
	".\VersInfo.h"\
	

"$(INTDIR)\ToolBar.obj" : $(SOURCE) $(DEP_CPP_TOOLB) "$(INTDIR)"

"$(INTDIR)\ToolBar.sbr" : $(SOURCE) $(DEP_CPP_TOOLB) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "highgrow - Win32 (80x86) Debug"

DEP_CPP_TOOLB=\
	".\ToolBar.h"\
	".\Global.h"\
	".\activity.h"\
	".\History.h"\
	".\Chart.h"\
	".\SeedList.h"\
	".\GrowRoom.h"\
	".\Harvest.h"\
	".\RobbieWin.h"\
	".\Registry.h"\
	".\Password.h"\
	".\Midi.h"\
	".\VersInfo.h"\
	

"$(INTDIR)\ToolBar.obj" : $(SOURCE) $(DEP_CPP_TOOLB) "$(INTDIR)"

"$(INTDIR)\ToolBar.sbr" : $(SOURCE) $(DEP_CPP_TOOLB) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "highgrow - Win32 (MIPS) Debug"

!ELSEIF  "$(CFG)" == "highgrow - Win32 (MIPS) Release"

!ELSEIF  "$(CFG)" == "highgrow - Win32 new Release"

DEP_CPP_TOOLB=\
	".\ToolBar.h"\
	".\Global.h"\
	".\activity.h"\
	".\History.h"\
	".\Chart.h"\
	".\SeedList.h"\
	".\GrowRoom.h"\
	".\Harvest.h"\
	".\RobbieWin.h"\
	".\Registry.h"\
	".\Password.h"\
	".\Midi.h"\
	".\VersInfo.h"\
	

"$(INTDIR)\ToolBar.obj" : $(SOURCE) $(DEP_CPP_TOOLB) "$(INTDIR)"

"$(INTDIR)\ToolBar.sbr" : $(SOURCE) $(DEP_CPP_TOOLB) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\PlantSeed.c

!IF  "$(CFG)" == "highgrow - Win32 (80x86) Release"

DEP_CPP_PLANTS=\
	".\Global.h"\
	".\SeedImp.h"\
	".\SeedLog.h"\
	

"$(INTDIR)\PlantSeed.obj" : $(SOURCE) $(DEP_CPP_PLANTS) "$(INTDIR)"

"$(INTDIR)\PlantSeed.sbr" : $(SOURCE) $(DEP_CPP_PLANTS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "highgrow - Win32 (80x86) Debug"

DEP_CPP_PLANTS=\
	".\Global.h"\
	".\SeedImp.h"\
	".\SeedLog.h"\
	

"$(INTDIR)\PlantSeed.obj" : $(SOURCE) $(DEP_CPP_PLANTS) "$(INTDIR)"

"$(INTDIR)\PlantSeed.sbr" : $(SOURCE) $(DEP_CPP_PLANTS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "highgrow - Win32 (MIPS) Debug"

!ELSEIF  "$(CFG)" == "highgrow - Win32 (MIPS) Release"

!ELSEIF  "$(CFG)" == "highgrow - Win32 new Release"

DEP_CPP_PLANTS=\
	".\Global.h"\
	".\SeedImp.h"\
	".\SeedLog.h"\
	

"$(INTDIR)\PlantSeed.obj" : $(SOURCE) $(DEP_CPP_PLANTS) "$(INTDIR)"

"$(INTDIR)\PlantSeed.sbr" : $(SOURCE) $(DEP_CPP_PLANTS) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Leaves.c

!IF  "$(CFG)" == "highgrow - Win32 (80x86) Release"

DEP_CPP_LEAVE=\
	".\Global.h"\
	

"$(INTDIR)\Leaves.obj" : $(SOURCE) $(DEP_CPP_LEAVE) "$(INTDIR)"

"$(INTDIR)\Leaves.sbr" : $(SOURCE) $(DEP_CPP_LEAVE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "highgrow - Win32 (80x86) Debug"

DEP_CPP_LEAVE=\
	".\Global.h"\
	

"$(INTDIR)\Leaves.obj" : $(SOURCE) $(DEP_CPP_LEAVE) "$(INTDIR)"

"$(INTDIR)\Leaves.sbr" : $(SOURCE) $(DEP_CPP_LEAVE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "highgrow - Win32 (MIPS) Debug"

!ELSEIF  "$(CFG)" == "highgrow - Win32 (MIPS) Release"

!ELSEIF  "$(CFG)" == "highgrow - Win32 new Release"

DEP_CPP_LEAVE=\
	".\Global.h"\
	

"$(INTDIR)\Leaves.obj" : $(SOURCE) $(DEP_CPP_LEAVE) "$(INTDIR)"

"$(INTDIR)\Leaves.sbr" : $(SOURCE) $(DEP_CPP_LEAVE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Comments.c

!IF  "$(CFG)" == "highgrow - Win32 (80x86) Release"

DEP_CPP_COMME=\
	".\Global.h"\
	

"$(INTDIR)\Comments.obj" : $(SOURCE) $(DEP_CPP_COMME) "$(INTDIR)"

"$(INTDIR)\Comments.sbr" : $(SOURCE) $(DEP_CPP_COMME) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "highgrow - Win32 (80x86) Debug"

DEP_CPP_COMME=\
	".\Global.h"\
	

"$(INTDIR)\Comments.obj" : $(SOURCE) $(DEP_CPP_COMME) "$(INTDIR)"

"$(INTDIR)\Comments.sbr" : $(SOURCE) $(DEP_CPP_COMME) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "highgrow - Win32 (MIPS) Debug"

!ELSEIF  "$(CFG)" == "highgrow - Win32 (MIPS) Release"

!ELSEIF  "$(CFG)" == "highgrow - Win32 new Release"

DEP_CPP_COMME=\
	".\Global.h"\
	

"$(INTDIR)\Comments.obj" : $(SOURCE) $(DEP_CPP_COMME) "$(INTDIR)"

"$(INTDIR)\Comments.sbr" : $(SOURCE) $(DEP_CPP_COMME) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Chart.c

!IF  "$(CFG)" == "highgrow - Win32 (80x86) Release"

DEP_CPP_CHART=\
	".\Global.h"\
	".\Calc.h"\
	".\PlantMem.h"\
	".\DiBitmap.h"\
	

"$(INTDIR)\Chart.obj" : $(SOURCE) $(DEP_CPP_CHART) "$(INTDIR)"

"$(INTDIR)\Chart.sbr" : $(SOURCE) $(DEP_CPP_CHART) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "highgrow - Win32 (80x86) Debug"

DEP_CPP_CHART=\
	".\Global.h"\
	".\Calc.h"\
	".\PlantMem.h"\
	".\DiBitmap.h"\
	

"$(INTDIR)\Chart.obj" : $(SOURCE) $(DEP_CPP_CHART) "$(INTDIR)"

"$(INTDIR)\Chart.sbr" : $(SOURCE) $(DEP_CPP_CHART) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "highgrow - Win32 (MIPS) Debug"

!ELSEIF  "$(CFG)" == "highgrow - Win32 (MIPS) Release"

!ELSEIF  "$(CFG)" == "highgrow - Win32 new Release"

DEP_CPP_CHART=\
	".\Global.h"\
	".\Calc.h"\
	".\PlantMem.h"\
	".\DiBitmap.h"\
	

"$(INTDIR)\Chart.obj" : $(SOURCE) $(DEP_CPP_CHART) "$(INTDIR)"

"$(INTDIR)\Chart.sbr" : $(SOURCE) $(DEP_CPP_CHART) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SeedList.c

!IF  "$(CFG)" == "highgrow - Win32 (80x86) Release"

DEP_CPP_SEEDL=\
	".\Global.h"\
	".\ToolBar.h"\
	".\highgrow.H"\
	".\PlantMem.h"\
	".\DiBitmap.h"\
	".\GrowRoom.h"\
	".\Harvest.h"\
	".\SeedList.h"\
	".\PlantSeed.h"\
	".\SeedImp.h"\
	".\SeedLog.h"\
	

"$(INTDIR)\SeedList.obj" : $(SOURCE) $(DEP_CPP_SEEDL) "$(INTDIR)"

"$(INTDIR)\SeedList.sbr" : $(SOURCE) $(DEP_CPP_SEEDL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "highgrow - Win32 (80x86) Debug"

DEP_CPP_SEEDL=\
	".\Global.h"\
	".\ToolBar.h"\
	".\highgrow.H"\
	".\PlantMem.h"\
	".\DiBitmap.h"\
	".\GrowRoom.h"\
	".\Harvest.h"\
	".\SeedList.h"\
	".\PlantSeed.h"\
	".\SeedImp.h"\
	".\SeedLog.h"\
	

"$(INTDIR)\SeedList.obj" : $(SOURCE) $(DEP_CPP_SEEDL) "$(INTDIR)"

"$(INTDIR)\SeedList.sbr" : $(SOURCE) $(DEP_CPP_SEEDL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "highgrow - Win32 (MIPS) Debug"

!ELSEIF  "$(CFG)" == "highgrow - Win32 (MIPS) Release"

!ELSEIF  "$(CFG)" == "highgrow - Win32 new Release"

DEP_CPP_SEEDL=\
	".\Global.h"\
	".\ToolBar.h"\
	".\highgrow.H"\
	".\PlantMem.h"\
	".\DiBitmap.h"\
	".\GrowRoom.h"\
	".\Harvest.h"\
	".\SeedList.h"\
	".\PlantSeed.h"\
	".\SeedImp.h"\
	".\SeedLog.h"\
	

"$(INTDIR)\SeedList.obj" : $(SOURCE) $(DEP_CPP_SEEDL) "$(INTDIR)"

"$(INTDIR)\SeedList.sbr" : $(SOURCE) $(DEP_CPP_SEEDL) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Registry.c

!IF  "$(CFG)" == "highgrow - Win32 (80x86) Release"


"$(INTDIR)\Registry.obj" : $(SOURCE) "$(INTDIR)"

"$(INTDIR)\Registry.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "highgrow - Win32 (80x86) Debug"


"$(INTDIR)\Registry.obj" : $(SOURCE) "$(INTDIR)"

"$(INTDIR)\Registry.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "highgrow - Win32 (MIPS) Debug"

!ELSEIF  "$(CFG)" == "highgrow - Win32 (MIPS) Release"

!ELSEIF  "$(CFG)" == "highgrow - Win32 new Release"


"$(INTDIR)\Registry.obj" : $(SOURCE) "$(INTDIR)"

"$(INTDIR)\Registry.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Password.c

!IF  "$(CFG)" == "highgrow - Win32 (80x86) Release"

DEP_CPP_PASSW=\
	".\Registry.h"\
	".\Global.h"\
	".\SeedList.h"\
	".\DiBitmap.h"\
	".\Clock.h"\
	

"$(INTDIR)\Password.obj" : $(SOURCE) $(DEP_CPP_PASSW) "$(INTDIR)"

"$(INTDIR)\Password.sbr" : $(SOURCE) $(DEP_CPP_PASSW) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "highgrow - Win32 (80x86) Debug"

DEP_CPP_PASSW=\
	".\Registry.h"\
	".\Global.h"\
	".\SeedList.h"\
	".\DiBitmap.h"\
	".\Clock.h"\
	

"$(INTDIR)\Password.obj" : $(SOURCE) $(DEP_CPP_PASSW) "$(INTDIR)"

"$(INTDIR)\Password.sbr" : $(SOURCE) $(DEP_CPP_PASSW) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "highgrow - Win32 (MIPS) Debug"

!ELSEIF  "$(CFG)" == "highgrow - Win32 (MIPS) Release"

!ELSEIF  "$(CFG)" == "highgrow - Win32 new Release"

DEP_CPP_PASSW=\
	".\Registry.h"\
	".\Global.h"\
	".\SeedList.h"\
	".\DiBitmap.h"\
	".\Clock.h"\
	

"$(INTDIR)\Password.obj" : $(SOURCE) $(DEP_CPP_PASSW) "$(INTDIR)"

"$(INTDIR)\Password.sbr" : $(SOURCE) $(DEP_CPP_PASSW) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\DiBitmap.c

!IF  "$(CFG)" == "highgrow - Win32 (80x86) Release"


"$(INTDIR)\DiBitmap.obj" : $(SOURCE) "$(INTDIR)"

"$(INTDIR)\DiBitmap.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "highgrow - Win32 (80x86) Debug"


"$(INTDIR)\DiBitmap.obj" : $(SOURCE) "$(INTDIR)"

"$(INTDIR)\DiBitmap.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "highgrow - Win32 (MIPS) Debug"

!ELSEIF  "$(CFG)" == "highgrow - Win32 (MIPS) Release"

!ELSEIF  "$(CFG)" == "highgrow - Win32 new Release"


"$(INTDIR)\DiBitmap.obj" : $(SOURCE) "$(INTDIR)"

"$(INTDIR)\DiBitmap.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Harvest.c

!IF  "$(CFG)" == "highgrow - Win32 (80x86) Release"

DEP_CPP_HARVE=\
	".\Global.h"\
	".\PlantMem.h"\
	".\PlantLog.h"\
	".\DiBitmap.h"\
	".\Calc.h"\
	".\Registry.h"\
	".\Password.h"\
	".\ToolBar.h"\
	".\SeedImp.h"\
	".\SeedLog.h"\
	

"$(INTDIR)\Harvest.obj" : $(SOURCE) $(DEP_CPP_HARVE) "$(INTDIR)"

"$(INTDIR)\Harvest.sbr" : $(SOURCE) $(DEP_CPP_HARVE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "highgrow - Win32 (80x86) Debug"

DEP_CPP_HARVE=\
	".\Global.h"\
	".\PlantMem.h"\
	".\PlantLog.h"\
	".\DiBitmap.h"\
	".\Calc.h"\
	".\Registry.h"\
	".\Password.h"\
	".\ToolBar.h"\
	".\SeedImp.h"\
	".\SeedLog.h"\
	

"$(INTDIR)\Harvest.obj" : $(SOURCE) $(DEP_CPP_HARVE) "$(INTDIR)"

"$(INTDIR)\Harvest.sbr" : $(SOURCE) $(DEP_CPP_HARVE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "highgrow - Win32 (MIPS) Debug"

!ELSEIF  "$(CFG)" == "highgrow - Win32 (MIPS) Release"

!ELSEIF  "$(CFG)" == "highgrow - Win32 new Release"

DEP_CPP_HARVE=\
	".\Global.h"\
	".\PlantMem.h"\
	".\PlantLog.h"\
	".\DiBitmap.h"\
	".\Calc.h"\
	".\Registry.h"\
	".\Password.h"\
	".\ToolBar.h"\
	".\SeedImp.h"\
	".\SeedLog.h"\
	

"$(INTDIR)\Harvest.obj" : $(SOURCE) $(DEP_CPP_HARVE) "$(INTDIR)"

"$(INTDIR)\Harvest.sbr" : $(SOURCE) $(DEP_CPP_HARVE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\PlantLog.c

!IF  "$(CFG)" == "highgrow - Win32 (80x86) Release"

DEP_CPP_PLANTL=\
	".\Global.h"\
	".\Harvest.h"\
	".\DiBitmap.h"\
	".\SeedList.h"\
	".\Registry.h"\
	

"$(INTDIR)\PlantLog.obj" : $(SOURCE) $(DEP_CPP_PLANTL) "$(INTDIR)"

"$(INTDIR)\PlantLog.sbr" : $(SOURCE) $(DEP_CPP_PLANTL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "highgrow - Win32 (80x86) Debug"

DEP_CPP_PLANTL=\
	".\Global.h"\
	".\Harvest.h"\
	".\DiBitmap.h"\
	".\SeedList.h"\
	".\Registry.h"\
	

"$(INTDIR)\PlantLog.obj" : $(SOURCE) $(DEP_CPP_PLANTL) "$(INTDIR)"

"$(INTDIR)\PlantLog.sbr" : $(SOURCE) $(DEP_CPP_PLANTL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "highgrow - Win32 (MIPS) Debug"

!ELSEIF  "$(CFG)" == "highgrow - Win32 (MIPS) Release"

!ELSEIF  "$(CFG)" == "highgrow - Win32 new Release"

DEP_CPP_PLANTL=\
	".\Global.h"\
	".\Harvest.h"\
	".\DiBitmap.h"\
	".\SeedList.h"\
	".\Registry.h"\
	

"$(INTDIR)\PlantLog.obj" : $(SOURCE) $(DEP_CPP_PLANTL) "$(INTDIR)"

"$(INTDIR)\PlantLog.sbr" : $(SOURCE) $(DEP_CPP_PLANTL) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Buds.c

!IF  "$(CFG)" == "highgrow - Win32 (80x86) Release"

DEP_CPP_BUDS_=\
	".\Global.h"\
	

"$(INTDIR)\Buds.obj" : $(SOURCE) $(DEP_CPP_BUDS_) "$(INTDIR)"

"$(INTDIR)\Buds.sbr" : $(SOURCE) $(DEP_CPP_BUDS_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "highgrow - Win32 (80x86) Debug"

DEP_CPP_BUDS_=\
	".\Global.h"\
	

"$(INTDIR)\Buds.obj" : $(SOURCE) $(DEP_CPP_BUDS_) "$(INTDIR)"

"$(INTDIR)\Buds.sbr" : $(SOURCE) $(DEP_CPP_BUDS_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "highgrow - Win32 (MIPS) Debug"

!ELSEIF  "$(CFG)" == "highgrow - Win32 (MIPS) Release"

!ELSEIF  "$(CFG)" == "highgrow - Win32 new Release"

DEP_CPP_BUDS_=\
	".\Global.h"\
	

"$(INTDIR)\Buds.obj" : $(SOURCE) $(DEP_CPP_BUDS_) "$(INTDIR)"

"$(INTDIR)\Buds.sbr" : $(SOURCE) $(DEP_CPP_BUDS_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\midi.c

!IF  "$(CFG)" == "highgrow - Win32 (80x86) Release"

DEP_CPP_MIDI_=\
	".\Midi.h"\
	".\Global.h"\
	

"$(INTDIR)\midi.obj" : $(SOURCE) $(DEP_CPP_MIDI_) "$(INTDIR)"

"$(INTDIR)\midi.sbr" : $(SOURCE) $(DEP_CPP_MIDI_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "highgrow - Win32 (80x86) Debug"

DEP_CPP_MIDI_=\
	".\Midi.h"\
	".\Global.h"\
	

"$(INTDIR)\midi.obj" : $(SOURCE) $(DEP_CPP_MIDI_) "$(INTDIR)"

"$(INTDIR)\midi.sbr" : $(SOURCE) $(DEP_CPP_MIDI_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "highgrow - Win32 (MIPS) Debug"

!ELSEIF  "$(CFG)" == "highgrow - Win32 (MIPS) Release"

!ELSEIF  "$(CFG)" == "highgrow - Win32 new Release"

DEP_CPP_MIDI_=\
	".\Midi.h"\
	".\Global.h"\
	

"$(INTDIR)\midi.obj" : $(SOURCE) $(DEP_CPP_MIDI_) "$(INTDIR)"

"$(INTDIR)\midi.sbr" : $(SOURCE) $(DEP_CPP_MIDI_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\TrayIcon.c

!IF  "$(CFG)" == "highgrow - Win32 (80x86) Release"

DEP_CPP_TRAYI=\
	".\ToolBar.h"\
	".\Registry.h"\
	".\Harvest.h"\
	".\TrayIcon.h"\
	".\highgrow.H"\
	

"$(INTDIR)\TrayIcon.obj" : $(SOURCE) $(DEP_CPP_TRAYI) "$(INTDIR)"

"$(INTDIR)\TrayIcon.sbr" : $(SOURCE) $(DEP_CPP_TRAYI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "highgrow - Win32 (80x86) Debug"

DEP_CPP_TRAYI=\
	".\ToolBar.h"\
	".\Registry.h"\
	".\Harvest.h"\
	".\TrayIcon.h"\
	".\highgrow.H"\
	

"$(INTDIR)\TrayIcon.obj" : $(SOURCE) $(DEP_CPP_TRAYI) "$(INTDIR)"

"$(INTDIR)\TrayIcon.sbr" : $(SOURCE) $(DEP_CPP_TRAYI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "highgrow - Win32 (MIPS) Debug"

!ELSEIF  "$(CFG)" == "highgrow - Win32 (MIPS) Release"

!ELSEIF  "$(CFG)" == "highgrow - Win32 new Release"

DEP_CPP_TRAYI=\
	".\ToolBar.h"\
	".\Registry.h"\
	".\Harvest.h"\
	".\TrayIcon.h"\
	".\highgrow.H"\
	

"$(INTDIR)\TrayIcon.obj" : $(SOURCE) $(DEP_CPP_TRAYI) "$(INTDIR)"

"$(INTDIR)\TrayIcon.sbr" : $(SOURCE) $(DEP_CPP_TRAYI) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\VersInfo.c

!IF  "$(CFG)" == "highgrow - Win32 (80x86) Release"

DEP_CPP_VERSI=\
	{$(INCLUDE)}"\Shlwapi.h"\
	

"$(INTDIR)\VersInfo.obj" : $(SOURCE) $(DEP_CPP_VERSI) "$(INTDIR)"

"$(INTDIR)\VersInfo.sbr" : $(SOURCE) $(DEP_CPP_VERSI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "highgrow - Win32 (80x86) Debug"

DEP_CPP_VERSI=\
	{$(INCLUDE)}"\Shlwapi.h"\
	

"$(INTDIR)\VersInfo.obj" : $(SOURCE) $(DEP_CPP_VERSI) "$(INTDIR)"

"$(INTDIR)\VersInfo.sbr" : $(SOURCE) $(DEP_CPP_VERSI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "highgrow - Win32 (MIPS) Debug"

!ELSEIF  "$(CFG)" == "highgrow - Win32 (MIPS) Release"

!ELSEIF  "$(CFG)" == "highgrow - Win32 new Release"

DEP_CPP_VERSI=\
	{$(INCLUDE)}"\Shlwapi.h"\
	

"$(INTDIR)\VersInfo.obj" : $(SOURCE) $(DEP_CPP_VERSI) "$(INTDIR)"

"$(INTDIR)\VersInfo.sbr" : $(SOURCE) $(DEP_CPP_VERSI) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\LogExport.c

!IF  "$(CFG)" == "highgrow - Win32 (80x86) Release"

DEP_CPP_LOGEX=\
	".\Global.h"\
	".\PlantMem.h"\
	".\Calc.h"\
	".\Registry.h"\
	

"$(INTDIR)\LogExport.obj" : $(SOURCE) $(DEP_CPP_LOGEX) "$(INTDIR)"

"$(INTDIR)\LogExport.sbr" : $(SOURCE) $(DEP_CPP_LOGEX) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "highgrow - Win32 (80x86) Debug"

DEP_CPP_LOGEX=\
	".\Global.h"\
	".\PlantMem.h"\
	".\Calc.h"\
	".\Registry.h"\
	

"$(INTDIR)\LogExport.obj" : $(SOURCE) $(DEP_CPP_LOGEX) "$(INTDIR)"

"$(INTDIR)\LogExport.sbr" : $(SOURCE) $(DEP_CPP_LOGEX) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "highgrow - Win32 (MIPS) Debug"

!ELSEIF  "$(CFG)" == "highgrow - Win32 (MIPS) Release"

!ELSEIF  "$(CFG)" == "highgrow - Win32 new Release"

DEP_CPP_LOGEX=\
	".\Global.h"\
	".\PlantMem.h"\
	".\Calc.h"\
	".\Registry.h"\
	

"$(INTDIR)\LogExport.obj" : $(SOURCE) $(DEP_CPP_LOGEX) "$(INTDIR)"

"$(INTDIR)\LogExport.sbr" : $(SOURCE) $(DEP_CPP_LOGEX) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\clock.c

!IF  "$(CFG)" == "highgrow - Win32 (80x86) Release"

DEP_CPP_CLOCK=\
	".\Clock.h"\
	".\Global.h"\
	".\Calc.h"\
	

"$(INTDIR)\clock.obj" : $(SOURCE) $(DEP_CPP_CLOCK) "$(INTDIR)"

"$(INTDIR)\clock.sbr" : $(SOURCE) $(DEP_CPP_CLOCK) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "highgrow - Win32 (80x86) Debug"

DEP_CPP_CLOCK=\
	".\Clock.h"\
	".\Global.h"\
	".\Calc.h"\
	

"$(INTDIR)\clock.obj" : $(SOURCE) $(DEP_CPP_CLOCK) "$(INTDIR)"

"$(INTDIR)\clock.sbr" : $(SOURCE) $(DEP_CPP_CLOCK) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "highgrow - Win32 (MIPS) Debug"

!ELSEIF  "$(CFG)" == "highgrow - Win32 (MIPS) Release"

!ELSEIF  "$(CFG)" == "highgrow - Win32 new Release"

DEP_CPP_CLOCK=\
	".\Clock.h"\
	".\Global.h"\
	".\Calc.h"\
	

"$(INTDIR)\clock.obj" : $(SOURCE) $(DEP_CPP_CLOCK) "$(INTDIR)"

"$(INTDIR)\clock.sbr" : $(SOURCE) $(DEP_CPP_CLOCK) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SeedImp.c

!IF  "$(CFG)" == "highgrow - Win32 (80x86) Release"

DEP_CPP_SEEDI=\
	".\Global.h"\
	".\SeedImp.h"\
	".\SeedList.h"\
	".\PlantSeed.h"\
	".\DiBitmap.h"\
	".\Registry.h"\
	".\SeedLog.h"\
	

"$(INTDIR)\SeedImp.obj" : $(SOURCE) $(DEP_CPP_SEEDI) "$(INTDIR)"

"$(INTDIR)\SeedImp.sbr" : $(SOURCE) $(DEP_CPP_SEEDI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "highgrow - Win32 (80x86) Debug"

DEP_CPP_SEEDI=\
	".\Global.h"\
	".\SeedImp.h"\
	".\SeedList.h"\
	".\PlantSeed.h"\
	".\DiBitmap.h"\
	".\Registry.h"\
	".\SeedLog.h"\
	

"$(INTDIR)\SeedImp.obj" : $(SOURCE) $(DEP_CPP_SEEDI) "$(INTDIR)"

"$(INTDIR)\SeedImp.sbr" : $(SOURCE) $(DEP_CPP_SEEDI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "highgrow - Win32 (MIPS) Debug"

!ELSEIF  "$(CFG)" == "highgrow - Win32 (MIPS) Release"

!ELSEIF  "$(CFG)" == "highgrow - Win32 new Release"

DEP_CPP_SEEDI=\
	".\Global.h"\
	".\SeedImp.h"\
	".\SeedList.h"\
	".\PlantSeed.h"\
	".\DiBitmap.h"\
	".\Registry.h"\
	".\SeedLog.h"\
	

"$(INTDIR)\SeedImp.obj" : $(SOURCE) $(DEP_CPP_SEEDI) "$(INTDIR)"

"$(INTDIR)\SeedImp.sbr" : $(SOURCE) $(DEP_CPP_SEEDI) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SeedLog.c

!IF  "$(CFG)" == "highgrow - Win32 (80x86) Release"

DEP_CPP_SEEDLO=\
	".\Global.h"\
	".\SeedLog.h"\
	

"$(INTDIR)\SeedLog.obj" : $(SOURCE) $(DEP_CPP_SEEDLO) "$(INTDIR)"

"$(INTDIR)\SeedLog.sbr" : $(SOURCE) $(DEP_CPP_SEEDLO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "highgrow - Win32 (80x86) Debug"

DEP_CPP_SEEDLO=\
	".\Global.h"\
	".\SeedLog.h"\
	

"$(INTDIR)\SeedLog.obj" : $(SOURCE) $(DEP_CPP_SEEDLO) "$(INTDIR)"

"$(INTDIR)\SeedLog.sbr" : $(SOURCE) $(DEP_CPP_SEEDLO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "highgrow - Win32 (MIPS) Debug"

!ELSEIF  "$(CFG)" == "highgrow - Win32 (MIPS) Release"

!ELSEIF  "$(CFG)" == "highgrow - Win32 new Release"

DEP_CPP_SEEDLO=\
	".\Global.h"\
	".\SeedLog.h"\
	

"$(INTDIR)\SeedLog.obj" : $(SOURCE) $(DEP_CPP_SEEDLO) "$(INTDIR)"

"$(INTDIR)\SeedLog.sbr" : $(SOURCE) $(DEP_CPP_SEEDLO) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\RobbieWin.c

!IF  "$(CFG)" == "highgrow - Win32 (80x86) Release"

DEP_CPP_ROBBI=\
	".\Robbie.h"\
	".\RobbieWin.h"\
	".\Global.h"\
	".\Comments.h"\
	".\DiBitmap.h"\
	".\ToolBar.h"\
	

"$(INTDIR)\RobbieWin.obj" : $(SOURCE) $(DEP_CPP_ROBBI) "$(INTDIR)"

"$(INTDIR)\RobbieWin.sbr" : $(SOURCE) $(DEP_CPP_ROBBI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "highgrow - Win32 (80x86) Debug"

DEP_CPP_ROBBI=\
	".\Robbie.h"\
	".\RobbieWin.h"\
	".\Global.h"\
	".\Comments.h"\
	".\DiBitmap.h"\
	".\ToolBar.h"\
	

"$(INTDIR)\RobbieWin.obj" : $(SOURCE) $(DEP_CPP_ROBBI) "$(INTDIR)"

"$(INTDIR)\RobbieWin.sbr" : $(SOURCE) $(DEP_CPP_ROBBI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "highgrow - Win32 (MIPS) Debug"

!ELSEIF  "$(CFG)" == "highgrow - Win32 (MIPS) Release"

!ELSEIF  "$(CFG)" == "highgrow - Win32 new Release"

DEP_CPP_ROBBI=\
	".\Robbie.h"\
	".\RobbieWin.h"\
	".\Global.h"\
	".\Comments.h"\
	".\DiBitmap.h"\
	".\ToolBar.h"\
	

"$(INTDIR)\RobbieWin.obj" : $(SOURCE) $(DEP_CPP_ROBBI) "$(INTDIR)"

"$(INTDIR)\RobbieWin.sbr" : $(SOURCE) $(DEP_CPP_ROBBI) "$(INTDIR)"


!ENDIF 

# End Source File
# End Target
# End Project
################################################################################

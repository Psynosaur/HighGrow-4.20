# Microsoft Developer Studio Project File - Name="highgrow" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=highgrow - Win32 (80x86) Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "HIGHGROW.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "HIGHGROW.mak" CFG="highgrow - Win32 (80x86) Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "highgrow - Win32 (80x86) Release" (based on "Win32 (x86) Application")
!MESSAGE "highgrow - Win32 (80x86) Debug" (based on "Win32 (x86) Application")
!MESSAGE "highgrow - Win32 new Release" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "highgrow - Win32 (80x86) Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\WinRel"
# PROP BASE Intermediate_Dir ".\WinRel"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\WinRel"
# PROP Intermediate_Dir ".\WinRel"
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# SUBTRACT BASE BSC32 /Iu
# ADD BSC32 /nologo
# SUBTRACT BSC32 /Iu
LINK32=link.exe
# ADD BASE LINK32 user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib mfc30.lib mfco30.lib mfcd30.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 version.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comctl32.lib winmm.lib dibapi32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "highgrow - Win32 (80x86) Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\WinDebug"
# PROP BASE Intermediate_Dir ".\WinDebug"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\WinDebug"
# PROP Intermediate_Dir ".\WinDebug"
# ADD BASE CPP /nologo /MD /W3 /GX /Zi /Od /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /YX /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# SUBTRACT BASE BSC32 /Iu
# ADD BSC32 /nologo
# SUBTRACT BSC32 /Iu
LINK32=link.exe
# ADD BASE LINK32 user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib mfc30d.lib mfco30d.lib mfcd30d.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 version.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comctl32.lib winmm.lib dibapi32.lib /nologo /subsystem:windows /debug /machine:I386

!ELSEIF  "$(CFG)" == "highgrow - Win32 new Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\highgrow"
# PROP BASE Intermediate_Dir ".\highgrow"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\highgrow"
# PROP Intermediate_Dir ".\highgrow"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /FR /YX /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# SUBTRACT BASE BSC32 /Iu
# ADD BSC32 /nologo
# SUBTRACT BSC32 /Iu
LINK32=link.exe
# ADD BASE LINK32 version.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comctl32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 version.lib comctl32.lib winmm.lib htmlhelp.lib wininet.lib /nologo /subsystem:windows /machine:I386
# SUBTRACT LINK32 /incremental:yes /nodefaultlib

!ENDIF 

# Begin Target

# Name "highgrow - Win32 (80x86) Release"
# Name "highgrow - Win32 (80x86) Debug"
# Name "highgrow - Win32 new Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\activity.c
# End Source File
# Begin Source File

SOURCE=.\Buds.c
# End Source File
# Begin Source File

SOURCE=.\Calc.c
# End Source File
# Begin Source File

SOURCE=.\Chart.c
# End Source File
# Begin Source File

SOURCE=.\clock.c
# End Source File
# Begin Source File

SOURCE=.\Comments.c
# End Source File
# Begin Source File

SOURCE=.\crypt.c
# End Source File
# Begin Source File

SOURCE=.\DibAPI.c
# End Source File
# Begin Source File

SOURCE=.\DiBitmap.c
# End Source File
# Begin Source File

SOURCE=.\Global.c
# End Source File
# Begin Source File

SOURCE=.\GrowEdit.c
# End Source File
# Begin Source File

SOURCE=.\GrowRoom.c
# End Source File
# Begin Source File

SOURCE=.\Harvest.c
# End Source File
# Begin Source File

SOURCE=.\highgrow.C
# End Source File
# Begin Source File

SOURCE=.\highgrow.RC
# End Source File
# Begin Source File

SOURCE=.\highgrow.txt
# End Source File
# Begin Source File

SOURCE=.\History.c
# End Source File
# Begin Source File

SOURCE=.\Internet.c
# End Source File
# Begin Source File

SOURCE=.\JPGView.cpp
# End Source File
# Begin Source File

SOURCE=.\Leaves.c
# End Source File
# Begin Source File

SOURCE=.\LogExport.c
# End Source File
# Begin Source File

SOURCE=.\midi.c
# End Source File
# Begin Source File

SOURCE=.\NodeCalc.c
# End Source File
# Begin Source File

SOURCE=.\Password.c
# End Source File
# Begin Source File

SOURCE=.\Picture.cpp
# End Source File
# Begin Source File

SOURCE=.\PlantLog.c
# End Source File
# Begin Source File

SOURCE=.\PlantMem.c
# End Source File
# Begin Source File

SOURCE=.\PlantSeed.c
# End Source File
# Begin Source File

SOURCE=.\Registry.c
# End Source File
# Begin Source File

SOURCE=.\RobbieWin.c
# End Source File
# Begin Source File

SOURCE=.\SeedImp.c
# End Source File
# Begin Source File

SOURCE=.\SeedList.c
# End Source File
# Begin Source File

SOURCE=.\SeedLog.c
# End Source File
# Begin Source File

SOURCE=.\ToolBar.c
# End Source File
# Begin Source File

SOURCE=.\TrayIcon.c
# End Source File
# Begin Source File

SOURCE=.\Vacation.c
# End Source File
# Begin Source File

SOURCE=.\VersInfo.c
# End Source File
# Begin Source File

SOURCE=.\Zoomer.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\activity.h
# End Source File
# Begin Source File

SOURCE=.\Buds.h
# End Source File
# Begin Source File

SOURCE=.\Calc.h
# End Source File
# Begin Source File

SOURCE=.\Chart.h
# End Source File
# Begin Source File

SOURCE=.\Clock.h
# End Source File
# Begin Source File

SOURCE=.\Comments.h
# End Source File
# Begin Source File

SOURCE=.\Crypt.h
# End Source File
# Begin Source File

SOURCE=.\DIBAPI.H
# End Source File
# Begin Source File

SOURCE=.\DiBitmap.h
# End Source File
# Begin Source File

SOURCE=.\Global.h
# End Source File
# Begin Source File

SOURCE=.\GrowEdit.h
# End Source File
# Begin Source File

SOURCE=.\GrowRoom.h
# End Source File
# Begin Source File

SOURCE=.\Harvest.h
# End Source File
# Begin Source File

SOURCE=.\highgrow.H
# End Source File
# Begin Source File

SOURCE=.\History.h
# End Source File
# Begin Source File

SOURCE=.\Internet.h
# End Source File
# Begin Source File

SOURCE=.\JPGView.h
# End Source File
# Begin Source File

SOURCE=.\Leaves.h
# End Source File
# Begin Source File

SOURCE=.\LogExport.h
# End Source File
# Begin Source File

SOURCE=.\Midi.h
# End Source File
# Begin Source File

SOURCE=.\NodeCalc.h
# End Source File
# Begin Source File

SOURCE=.\Password.h
# End Source File
# Begin Source File

SOURCE=.\Picture.h
# End Source File
# Begin Source File

SOURCE=.\PlantLog.h
# End Source File
# Begin Source File

SOURCE=.\PlantMem.h
# End Source File
# Begin Source File

SOURCE=.\PlantSeed.h
# End Source File
# Begin Source File

SOURCE=.\Registry.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\Robbie.h
# End Source File
# Begin Source File

SOURCE=.\RobbieWin.h
# End Source File
# Begin Source File

SOURCE=.\SeedImp.h
# End Source File
# Begin Source File

SOURCE=.\SeedList.h
# End Source File
# Begin Source File

SOURCE=.\SeedLog.h
# End Source File
# Begin Source File

SOURCE=.\ToolBar.h
# End Source File
# Begin Source File

SOURCE=.\TrayIcon.h
# End Source File
# Begin Source File

SOURCE=.\Vacation.h
# End Source File
# Begin Source File

SOURCE=.\VersInfo.h
# End Source File
# Begin Source File

SOURCE=.\Zoomer.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\app_icon.ico
# End Source File
# Begin Source File

SOURCE=.\Appleaf.ico
# End Source File
# Begin Source File

SOURCE=.\Attic.bmp
# End Source File
# Begin Source File

SOURCE=.\Attic.jpg
# End Source File
# Begin Source File

SOURCE=.\Basement.bmp
# End Source File
# Begin Source File

SOURCE=.\Basement.jpg
# End Source File
# Begin Source File

SOURCE=.\bdf.bmp
# End Source File
# Begin Source File

SOURCE=.\Bedroom.bmp
# End Source File
# Begin Source File

SOURCE=.\Bedroom.jpg
# End Source File
# Begin Source File

SOURCE=.\big_pots.bmp
# End Source File
# Begin Source File

SOURCE=.\BigLights.bmp
# End Source File
# Begin Source File

SOURCE=.\BigPots.bmp
# End Source File
# Begin Source File

SOURCE=.\black.bmp
# End Source File
# Begin Source File

SOURCE=.\black1.bmp
# End Source File
# Begin Source File

SOURCE=.\BoomBox.bmp
# End Source File
# Begin Source File

SOURCE=.\boomhand.cur
# End Source File
# Begin Source File

SOURCE=.\closedha.cur
# End Source File
# Begin Source File

SOURCE=.\cur00001.cur
# End Source File
# Begin Source File

SOURCE=.\dlgleaf.bmp
# End Source File
# Begin Source File

SOURCE=.\DlgLeaf.ico
# End Source File
# Begin Source File

SOURCE=".\Empty room.bmp"
# End Source File
# Begin Source File

SOURCE=".\Empty room.jpg"
# End Source File
# Begin Source File

SOURCE=.\FreeStar.bmp
# End Source File
# Begin Source File

SOURCE=.\GrowRoom.bmp
# End Source File
# Begin Source File

SOURCE=.\Growroom1.bmp
# End Source File
# Begin Source File

SOURCE=.\highgrow.ICO
# End Source File
# Begin Source File

SOURCE=.\HPS.bmp
# End Source File
# Begin Source File

SOURCE=.\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\ico00002.ico
# End Source File
# Begin Source File

SOURCE=.\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\Leaf.bmp
# End Source File
# Begin Source File

SOURCE=.\Leaf256.bmp
# End Source File
# Begin Source File

SOURCE=.\leaftile.bmp
# End Source File
# Begin Source File

SOURCE=.\LFED.bmp
# End Source File
# Begin Source File

SOURCE=.\LFEU.bmp
# End Source File
# Begin Source File

SOURCE=.\Lfla.bmp
# End Source File
# Begin Source File

SOURCE=.\LFLB.bmp
# End Source File
# Begin Source File

SOURCE=.\LFLC.bmp
# End Source File
# Begin Source File

SOURCE=.\Lfld.bmp
# End Source File
# Begin Source File

SOURCE=.\Lfra.bmp
# End Source File
# Begin Source File

SOURCE=.\LFRB.bmp
# End Source File
# Begin Source File

SOURCE=.\LFRC.bmp
# End Source File
# Begin Source File

SOURCE=.\Lfrd.bmp
# End Source File
# Begin Source File

SOURCE=.\Light.ico
# End Source File
# Begin Source File

SOURCE=.\lightgre.bmp
# End Source File
# Begin Source File

SOURCE=.\linkhand.cur
# End Source File
# Begin Source File

SOURCE=.\Llight.bmp
# End Source File
# Begin Source File

SOURCE=.\Lshade.bmp
# End Source File
# Begin Source File

SOURCE=.\MH.bmp
# End Source File
# Begin Source File

SOURCE=.\NoEntry.bmp
# End Source File
# Begin Source File

SOURCE=.\openhand.cur
# End Source File
# Begin Source File

SOURCE=.\PassLeaf.bmp
# End Source File
# Begin Source File

SOURCE=.\Pot_03.bmp
# End Source File
# Begin Source File

SOURCE=.\PotList.bmp
# End Source File
# Begin Source File

SOURCE=.\scissors.cur
# End Source File
# Begin Source File

SOURCE=.\seedlist.bmp
# End Source File
# Begin Source File

SOURCE=.\slick.bmp
# End Source File
# Begin Source File

SOURCE=.\SmallLights.bmp
# End Source File
# Begin Source File

SOURCE=.\smallpots.bmp
# End Source File
# Begin Source File

SOURCE=.\Status.ico
# End Source File
# Begin Source File

SOURCE=.\Title.bmp
# End Source File
# Begin Source File

SOURCE=.\Title.jpg
# End Source File
# Begin Source File

SOURCE=.\toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\Water.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\Alarm.WAV
# End Source File
# Begin Source File

SOURCE=.\BoomClick.wav
# End Source File
# Begin Source File

SOURCE=.\HighGrow.exe.manifest
# End Source File
# End Target
# End Project

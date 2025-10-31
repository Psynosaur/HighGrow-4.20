# Microsoft Developer Studio Project File - Name="RobbieDLL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=RobbieDLL - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "RobbieDLL.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "RobbieDLL.mak" CFG="RobbieDLL - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "RobbieDLL - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "RobbieDLL - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "RobbieDLL - Win32 Wally 95 Release DLL" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "RobbieDLL - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\WinRel"
# PROP BASE Intermediate_Dir ".\WinRel"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir ".\WinRel"
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /YX /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib winmm.lib /nologo /subsystem:windows /dll /machine:I386 /out:"Wally96.dll"

!ELSEIF  "$(CFG)" == "RobbieDLL - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\WinDebug"
# PROP BASE Intermediate_Dir ".\WinDebug"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir ".\WinDebug"
# ADD BASE CPP /nologo /MT /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /YX /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib winmm.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"Wally96.dll"

!ELSEIF  "$(CFG)" == "RobbieDLL - Win32 Wally 95 Release DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Robbie_DLL"
# PROP BASE Intermediate_Dir ".\Robbie_DLL"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir ".\Robbie_DLL"
# PROP Ignore_Export_Lib 0
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /YX /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /machine:I386 /out:".\Robbie_DLL\Robbie.dll"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "RobbieDLL - Win32 Release"
# Name "RobbieDLL - Win32 Debug"
# Name "RobbieDLL - Win32 Wally 95 Release DLL"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\ANIMATE.C
# End Source File
# Begin Source File

SOURCE=.\BUBBLE.C
# End Source File
# Begin Source File

SOURCE=.\EXPRESNS.C
# End Source File
# Begin Source File

SOURCE=.\MESSAGES.C
# End Source File
# Begin Source File

SOURCE=.\PLAYSND.C
# End Source File
# Begin Source File

SOURCE=.\Rdllwin.c
# End Source File
# Begin Source File

SOURCE=.\Robbie.def
# End Source File
# Begin Source File

SOURCE=.\RobbieDll.RC
# End Source File
# Begin Source File

SOURCE=.\SPRITE32.C
# End Source File
# Begin Source File

SOURCE=.\TOONFILE.C
# End Source File
# Begin Source File

SOURCE=.\Updatexe.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\ANIMATE.H
# End Source File
# Begin Source File

SOURCE=.\BUBBLE.H
# End Source File
# Begin Source File

SOURCE=.\EXPRESNS.H
# End Source File
# Begin Source File

SOURCE=.\MESSAGES.H
# End Source File
# Begin Source File

SOURCE=.\PLAYSND.H
# End Source File
# Begin Source File

SOURCE=.\RdllWIN.H
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\Robbie.h
# End Source File
# Begin Source File

SOURCE=.\SPRITE32.H
# End Source File
# Begin Source File

SOURCE=.\TOONFILE.H
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\BUBLEFT1.BMP
# End Source File
# Begin Source File

SOURCE=.\Bubleft2.bmp
# End Source File
# Begin Source File

SOURCE=.\BUBRGHT1.BMP
# End Source File
# Begin Source File

SOURCE=.\Bubrght2.bmp
# End Source File
# Begin Source File

SOURCE=.\expresns.bmp
# End Source File
# Begin Source File

SOURCE=.\POSE01.BMP
# End Source File
# Begin Source File

SOURCE=.\POSE02.BMP
# End Source File
# Begin Source File

SOURCE=.\POSE03.BMP
# End Source File
# Begin Source File

SOURCE=.\POSE04.BMP
# End Source File
# Begin Source File

SOURCE=.\POSE05.BMP
# End Source File
# Begin Source File

SOURCE=.\POSE06.BMP
# End Source File
# Begin Source File

SOURCE=.\POSE07.BMP
# End Source File
# Begin Source File

SOURCE=.\POSE08.BMP
# End Source File
# Begin Source File

SOURCE=.\POSE09.BMP
# End Source File
# Begin Source File

SOURCE=.\POSE10.BMP
# End Source File
# Begin Source File

SOURCE=.\POSE11.BMP
# End Source File
# Begin Source File

SOURCE=.\POSE12.BMP
# End Source File
# Begin Source File

SOURCE=.\POSE13.BMP
# End Source File
# Begin Source File

SOURCE=.\POSE14.BMP
# End Source File
# Begin Source File

SOURCE=.\POSE15.BMP
# End Source File
# Begin Source File

SOURCE=.\POSE16.BMP
# End Source File
# Begin Source File

SOURCE=.\pose17.bmp
# End Source File
# Begin Source File

SOURCE=.\pose18.bmp
# End Source File
# Begin Source File

SOURCE=.\pose19.bmp
# End Source File
# Begin Source File

SOURCE=.\pose20.bmp
# End Source File
# Begin Source File

SOURCE=.\pose21.bmp
# End Source File
# Begin Source File

SOURCE=.\pose22.bmp
# End Source File
# Begin Source File

SOURCE=.\pose23.bmp
# End Source File
# Begin Source File

SOURCE=.\pose24.bmp
# End Source File
# Begin Source File

SOURCE=.\RRltn1.bmp
# End Source File
# Begin Source File

SOURCE=.\RRltn2.bmp
# End Source File
# Begin Source File

SOURCE=.\RRltr1.bmp
# End Source File
# Begin Source File

SOURCE=.\RRltr2.bmp
# End Source File
# Begin Source File

SOURCE=.\RRltr3.bmp
# End Source File
# Begin Source File

SOURCE=.\RRltr4.bmp
# End Source File
# Begin Source File

SOURCE=.\RRltr5.bmp
# End Source File
# Begin Source File

SOURCE=.\RRltr6.bmp
# End Source File
# Begin Source File

SOURCE=.\RRltr7.bmp
# End Source File
# Begin Source File

SOURCE=.\RRltr8.bmp
# End Source File
# Begin Source File

SOURCE=.\RRrtl1.bmp
# End Source File
# Begin Source File

SOURCE=.\RRrtl2.bmp
# End Source File
# Begin Source File

SOURCE=.\RRrtl3.bmp
# End Source File
# Begin Source File

SOURCE=.\RRrtl4.bmp
# End Source File
# Begin Source File

SOURCE=.\RRrtl5.bmp
# End Source File
# Begin Source File

SOURCE=.\RRrtl6.bmp
# End Source File
# Begin Source File

SOURCE=.\RRrtl7.bmp
# End Source File
# Begin Source File

SOURCE=.\RRrtl8.bmp
# End Source File
# Begin Source File

SOURCE=.\RRrtn1.bmp
# End Source File
# Begin Source File

SOURCE=.\RRrtn2.bmp
# End Source File
# Begin Source File

SOURCE=.\WWL2R1.BMP
# End Source File
# Begin Source File

SOURCE=.\WWL2R2.BMP
# End Source File
# Begin Source File

SOURCE=.\WWL2R3.BMP
# End Source File
# Begin Source File

SOURCE=.\WWL2R4.BMP
# End Source File
# Begin Source File

SOURCE=.\WWL2R5.BMP
# End Source File
# Begin Source File

SOURCE=.\WWL2R6.BMP
# End Source File
# Begin Source File

SOURCE=.\WWL2R7.BMP
# End Source File
# Begin Source File

SOURCE=.\WWL2R8.BMP
# End Source File
# Begin Source File

SOURCE=.\WWLTN1.BMP
# End Source File
# Begin Source File

SOURCE=.\WWLTN2.BMP
# End Source File
# Begin Source File

SOURCE=.\WWR2L1.BMP
# End Source File
# Begin Source File

SOURCE=.\WWR2L2.BMP
# End Source File
# Begin Source File

SOURCE=.\WWR2L3.BMP
# End Source File
# Begin Source File

SOURCE=.\WWR2L4.BMP
# End Source File
# Begin Source File

SOURCE=.\WWR2L5.BMP
# End Source File
# Begin Source File

SOURCE=.\WWR2L6.BMP
# End Source File
# Begin Source File

SOURCE=.\WWR2L7.BMP
# End Source File
# Begin Source File

SOURCE=.\WWR2L8.BMP
# End Source File
# Begin Source File

SOURCE=.\WWRTN1.BMP
# End Source File
# Begin Source File

SOURCE=.\WWRTN2.BMP
# End Source File
# End Group
# End Target
# End Project

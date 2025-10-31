# Microsoft Developer Studio Generated NMAKE File, Format Version 4.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

!IF "$(CFG)" == ""
CFG=RobbieDLL - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to RobbieDLL - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "RobbieDLL - Win32 Release" && "$(CFG)" !=\
 "RobbieDLL - Win32 Debug" && "$(CFG)" !=\
 "RobbieDLL - Win32 Wally 95 Release DLL"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "RobbieDll.mak" CFG="RobbieDLL - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "RobbieDLL - Win32 Release" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "RobbieDLL - Win32 Debug" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "RobbieDLL - Win32 Wally 95 Release DLL" (based on\
 "Win32 (x86) Dynamic-Link Library")
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
# PROP Target_Last_Scanned "RobbieDLL - Win32 Wally 95 Release DLL"
RSC=rc.exe
MTL=mktyplib.exe
CPP=cl.exe

!IF  "$(CFG)" == "RobbieDLL - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WinRel"
# PROP BASE Intermediate_Dir "WinRel"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "WinRel"
OUTDIR=.
INTDIR=.\WinRel

ALL : "$(OUTDIR)\Wally96.dll"

CLEAN : 
	-@erase ".\Wally96.dll"
	-@erase ".\WinRel\TOONFILE.OBJ"
	-@erase ".\WinRel\PLAYSND.OBJ"
	-@erase ".\WinRel\MESSAGES.OBJ"
	-@erase ".\WinRel\SPRITE32.OBJ"
	-@erase ".\WinRel\ANIMATE.OBJ"
	-@erase ".\WinRel\EXPRESNS.OBJ"
	-@erase ".\WinRel\BUBBLE.OBJ"
	-@erase ".\WinRel\Rdllwin.obj"
	-@erase ".\WinRel\RobbieDLL.res"
	-@erase ".\Wally96.lib"
	-@erase ".\Wally96.exp"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /YX /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# SUBTRACT CPP /Fr
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/RobbieDll.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\WinRel/
CPP_SBRS=
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/RobbieDLL.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/RobbieDll.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib winmm.lib /nologo /subsystem:windows /dll /machine:I386 /out:"Wally96.dll"
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib winmm.lib /nologo\
 /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)/Wally96.pdb"\
 /machine:I386 /def:".\Robbie.def" /out:"$(OUTDIR)/Wally96.dll"\
 /implib:"$(OUTDIR)/Wally96.lib" 
DEF_FILE= \
	".\Robbie.def"
LINK32_OBJS= \
	"$(INTDIR)/TOONFILE.OBJ" \
	"$(INTDIR)/PLAYSND.OBJ" \
	"$(INTDIR)/MESSAGES.OBJ" \
	"$(INTDIR)/SPRITE32.OBJ" \
	"$(INTDIR)/ANIMATE.OBJ" \
	"$(INTDIR)/EXPRESNS.OBJ" \
	"$(INTDIR)/BUBBLE.OBJ" \
	"$(INTDIR)/Rdllwin.obj" \
	"$(INTDIR)/RobbieDLL.res"

"$(OUTDIR)\Wally96.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "RobbieDLL - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "WinDebug"
# PROP BASE Intermediate_Dir "WinDebug"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "WinDebug"
OUTDIR=.
INTDIR=.\WinDebug

ALL : "$(OUTDIR)\Wally96.dll"

CLEAN : 
	-@erase ".\WinDebug\vc40.pdb"
	-@erase ".\WinDebug\vc40.idb"
	-@erase ".\Wally96.dll"
	-@erase ".\WinDebug\ANIMATE.OBJ"
	-@erase ".\WinDebug\BUBBLE.OBJ"
	-@erase ".\WinDebug\SPRITE32.OBJ"
	-@erase ".\WinDebug\Rdllwin.obj"
	-@erase ".\WinDebug\EXPRESNS.OBJ"
	-@erase ".\WinDebug\PLAYSND.OBJ"
	-@erase ".\WinDebug\MESSAGES.OBJ"
	-@erase ".\WinDebug\TOONFILE.OBJ"
	-@erase ".\WinDebug\RobbieDLL.res"
	-@erase ".\Wally96.ilk"
	-@erase ".\Wally96.lib"
	-@erase ".\Wally96.exp"
	-@erase ".\Wally96.pdb"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /MT /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /YX /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# SUBTRACT CPP /Fr
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/RobbieDll.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\WinDebug/
CPP_SBRS=
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/RobbieDLL.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/RobbieDll.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib winmm.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"Wally96.dll"
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib winmm.lib /nologo\
 /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)/Wally96.pdb" /debug\
 /machine:I386 /def:".\Robbie.def" /out:"$(OUTDIR)/Wally96.dll"\
 /implib:"$(OUTDIR)/Wally96.lib" 
DEF_FILE= \
	".\Robbie.def"
LINK32_OBJS= \
	"$(INTDIR)/ANIMATE.OBJ" \
	"$(INTDIR)/BUBBLE.OBJ" \
	"$(INTDIR)/SPRITE32.OBJ" \
	"$(INTDIR)/Rdllwin.obj" \
	"$(INTDIR)/EXPRESNS.OBJ" \
	"$(INTDIR)/PLAYSND.OBJ" \
	"$(INTDIR)/MESSAGES.OBJ" \
	"$(INTDIR)/TOONFILE.OBJ" \
	"$(INTDIR)/RobbieDLL.res"

"$(OUTDIR)\Wally96.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "RobbieDLL - Win32 Wally 95 Release DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Robbie_DLL"
# PROP BASE Intermediate_Dir "Robbie_DLL"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "Robbie_DLL"
OUTDIR=.
INTDIR=.\Robbie_DLL

ALL : "$(OUTDIR)\Robbie_DLL\Robbie.dll"

CLEAN : 
	-@erase ".\Robbie_DLL\Robbie.dll"
	-@erase ".\Robbie_DLL\EXPRESNS.OBJ"
	-@erase ".\Robbie_DLL\TOONFILE.OBJ"
	-@erase ".\Robbie_DLL\BUBBLE.OBJ"
	-@erase ".\Robbie_DLL\ANIMATE.OBJ"
	-@erase ".\Robbie_DLL\SPRITE32.OBJ"
	-@erase ".\Robbie_DLL\MESSAGES.OBJ"
	-@erase ".\Robbie_DLL\PLAYSND.OBJ"
	-@erase ".\Robbie_DLL\Rdllwin.obj"
	-@erase ".\Robbie_DLL\RobbieDLL.res"
	-@erase ".\Robbie.lib"
	-@erase ".\Robbie.exp"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /YX /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# SUBTRACT CPP /Fr
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/RobbieDll.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Robbie_DLL/
CPP_SBRS=
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/RobbieDLL.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/RobbieDll.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /machine:I386 /out:"Robbie_DLL\Robbie.dll"
# SUBTRACT LINK32 /pdb:none
LINK32_FLAGS=winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)/Robbie.pdb"\
 /machine:I386 /def:".\Robbie.def" /out:"$(OUTDIR)/Robbie_DLL\Robbie.dll"\
 /implib:"$(OUTDIR)/Robbie.lib" 
DEF_FILE= \
	".\Robbie.def"
LINK32_OBJS= \
	"$(INTDIR)/EXPRESNS.OBJ" \
	"$(INTDIR)/TOONFILE.OBJ" \
	"$(INTDIR)/BUBBLE.OBJ" \
	"$(INTDIR)/ANIMATE.OBJ" \
	"$(INTDIR)/SPRITE32.OBJ" \
	"$(INTDIR)/MESSAGES.OBJ" \
	"$(INTDIR)/PLAYSND.OBJ" \
	"$(INTDIR)/Rdllwin.obj" \
	"$(INTDIR)/RobbieDLL.res"

"$(OUTDIR)\Robbie_DLL\Robbie.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

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

################################################################################
# Begin Target

# Name "RobbieDLL - Win32 Release"
# Name "RobbieDLL - Win32 Debug"
# Name "RobbieDLL - Win32 Wally 95 Release DLL"

!IF  "$(CFG)" == "RobbieDLL - Win32 Release"

!ELSEIF  "$(CFG)" == "RobbieDLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "RobbieDLL - Win32 Wally 95 Release DLL"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\EXPRESNS.C
DEP_CPP_EXPRE=\
	".\EXPRESNS.H"\
	".\SPRITE32.H"\
	".\robbie.h"\
	".\TOONFILE.H"\
	".\MESSAGES.H"\
	".\PLAYSND.H"\
	

!IF  "$(CFG)" == "RobbieDLL - Win32 Release"


"$(INTDIR)\EXPRESNS.OBJ" : $(SOURCE) $(DEP_CPP_EXPRE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "RobbieDLL - Win32 Debug"


"$(INTDIR)\EXPRESNS.OBJ" : $(SOURCE) $(DEP_CPP_EXPRE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "RobbieDLL - Win32 Wally 95 Release DLL"


"$(INTDIR)\EXPRESNS.OBJ" : $(SOURCE) $(DEP_CPP_EXPRE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\TOONFILE.C
DEP_CPP_TOONF=\
	".\PLAYSND.H"\
	".\robbie.h"\
	".\TOONFILE.H"\
	

!IF  "$(CFG)" == "RobbieDLL - Win32 Release"


"$(INTDIR)\TOONFILE.OBJ" : $(SOURCE) $(DEP_CPP_TOONF) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "RobbieDLL - Win32 Debug"


"$(INTDIR)\TOONFILE.OBJ" : $(SOURCE) $(DEP_CPP_TOONF) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "RobbieDLL - Win32 Wally 95 Release DLL"


"$(INTDIR)\TOONFILE.OBJ" : $(SOURCE) $(DEP_CPP_TOONF) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\BUBBLE.C
DEP_CPP_BUBBL=\
	".\SPRITE32.H"\
	".\BUBBLE.H"\
	

!IF  "$(CFG)" == "RobbieDLL - Win32 Release"


"$(INTDIR)\BUBBLE.OBJ" : $(SOURCE) $(DEP_CPP_BUBBL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "RobbieDLL - Win32 Debug"


"$(INTDIR)\BUBBLE.OBJ" : $(SOURCE) $(DEP_CPP_BUBBL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "RobbieDLL - Win32 Wally 95 Release DLL"


"$(INTDIR)\BUBBLE.OBJ" : $(SOURCE) $(DEP_CPP_BUBBL) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\ANIMATE.C
DEP_CPP_ANIMA=\
	".\SPRITE32.H"\
	".\ANIMATE.H"\
	

!IF  "$(CFG)" == "RobbieDLL - Win32 Release"


"$(INTDIR)\ANIMATE.OBJ" : $(SOURCE) $(DEP_CPP_ANIMA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "RobbieDLL - Win32 Debug"


"$(INTDIR)\ANIMATE.OBJ" : $(SOURCE) $(DEP_CPP_ANIMA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "RobbieDLL - Win32 Wally 95 Release DLL"


"$(INTDIR)\ANIMATE.OBJ" : $(SOURCE) $(DEP_CPP_ANIMA) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SPRITE32.C
DEP_CPP_SPRIT=\
	".\SPRITE32.H"\
	

!IF  "$(CFG)" == "RobbieDLL - Win32 Release"


"$(INTDIR)\SPRITE32.OBJ" : $(SOURCE) $(DEP_CPP_SPRIT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "RobbieDLL - Win32 Debug"


"$(INTDIR)\SPRITE32.OBJ" : $(SOURCE) $(DEP_CPP_SPRIT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "RobbieDLL - Win32 Wally 95 Release DLL"


"$(INTDIR)\SPRITE32.OBJ" : $(SOURCE) $(DEP_CPP_SPRIT) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\MESSAGES.C
DEP_CPP_MESSA=\
	".\SPRITE32.H"\
	".\robbie.h"\
	".\MESSAGES.H"\
	".\ANIMATE.H"\
	".\BUBBLE.H"\
	".\TOONFILE.H"\
	".\EXPRESNS.H"\
	".\rdllwin.h"\
	".\PLAYSND.H"\
	

!IF  "$(CFG)" == "RobbieDLL - Win32 Release"


"$(INTDIR)\MESSAGES.OBJ" : $(SOURCE) $(DEP_CPP_MESSA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "RobbieDLL - Win32 Debug"


"$(INTDIR)\MESSAGES.OBJ" : $(SOURCE) $(DEP_CPP_MESSA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "RobbieDLL - Win32 Wally 95 Release DLL"


"$(INTDIR)\MESSAGES.OBJ" : $(SOURCE) $(DEP_CPP_MESSA) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\PLAYSND.C
DEP_CPP_PLAYS=\
	".\PLAYSND.H"\
	

!IF  "$(CFG)" == "RobbieDLL - Win32 Release"


"$(INTDIR)\PLAYSND.OBJ" : $(SOURCE) $(DEP_CPP_PLAYS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "RobbieDLL - Win32 Debug"


"$(INTDIR)\PLAYSND.OBJ" : $(SOURCE) $(DEP_CPP_PLAYS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "RobbieDLL - Win32 Wally 95 Release DLL"


"$(INTDIR)\PLAYSND.OBJ" : $(SOURCE) $(DEP_CPP_PLAYS) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Rdllwin.c
DEP_CPP_RDLLW=\
	".\rdllwin.h"\
	".\robbie.h"\
	

!IF  "$(CFG)" == "RobbieDLL - Win32 Release"


"$(INTDIR)\Rdllwin.obj" : $(SOURCE) $(DEP_CPP_RDLLW) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "RobbieDLL - Win32 Debug"


"$(INTDIR)\Rdllwin.obj" : $(SOURCE) $(DEP_CPP_RDLLW) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "RobbieDLL - Win32 Wally 95 Release DLL"


"$(INTDIR)\Rdllwin.obj" : $(SOURCE) $(DEP_CPP_RDLLW) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Robbie.def

!IF  "$(CFG)" == "RobbieDLL - Win32 Release"

!ELSEIF  "$(CFG)" == "RobbieDLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "RobbieDLL - Win32 Wally 95 Release DLL"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\RobbieDLL.RC
DEP_RSC_ROBBI=\
	".\Bubleft1.bmp"\
	".\Bubrght1.bmp"\
	".\Pose01.bmp"\
	".\Pose02.bmp"\
	".\Pose06.bmp"\
	".\Pose07.bmp"\
	".\Pose08.bmp"\
	".\Pose03.bmp"\
	".\Pose04.bmp"\
	".\Pose05.bmp"\
	".\expresns.bmp"\
	".\Pose16.bmp"\
	".\Pose15.bmp"\
	".\Pose14.bmp"\
	".\Pose13.bmp"\
	".\Pose12.bmp"\
	".\Pose11.bmp"\
	".\Pose10.bmp"\
	".\Pose09.bmp"\
	".\Pose24.bmp"\
	".\Pose18.bmp"\
	".\Pose19.bmp"\
	".\Pose20.bmp"\
	".\Pose21.bmp"\
	".\Pose22.bmp"\
	".\Pose23.bmp"\
	".\Pose17.bmp"\
	".\RRrtn2.bmp"\
	".\RRltn2.bmp"\
	".\RRltr1.bmp"\
	".\RRltr2.bmp"\
	".\RRltr3.bmp"\
	".\RRltr4.bmp"\
	".\RRltr5.bmp"\
	".\RRltr6.bmp"\
	".\RRltr7.bmp"\
	".\RRltr8.bmp"\
	".\RRrtl1.bmp"\
	".\RRrtl2.bmp"\
	".\RRrtl3.bmp"\
	".\RRrtl4.bmp"\
	".\RRrtl5.bmp"\
	".\RRrtl6.bmp"\
	".\RRrtl7.bmp"\
	".\RRrtl8.bmp"\
	".\RRrtn1.bmp"\
	".\RRltn1.bmp"\
	".\Bubrght2.bmp"\
	".\Bubleft2.bmp"\
	

!IF  "$(CFG)" == "RobbieDLL - Win32 Release"


"$(INTDIR)\RobbieDLL.res" : $(SOURCE) $(DEP_RSC_ROBBI) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "RobbieDLL - Win32 Debug"


"$(INTDIR)\RobbieDLL.res" : $(SOURCE) $(DEP_RSC_ROBBI) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "RobbieDLL - Win32 Wally 95 Release DLL"


"$(INTDIR)\RobbieDLL.res" : $(SOURCE) $(DEP_RSC_ROBBI) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ENDIF 

# End Source File
# End Target
# End Project
################################################################################

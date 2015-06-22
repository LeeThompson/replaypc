# Microsoft Developer Studio Generated NMAKE File, Based on dumpguide.dsp
!IF "$(CFG)" == ""
CFG=dumpguide - Win32 Debug
!MESSAGE No configuration specified. Defaulting to dumpguide - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "dumpguide - Win32 Release" && "$(CFG)" != "dumpguide - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "dumpguide.mak" CFG="dumpguide - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "dumpguide - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "dumpguide - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "dumpguide - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\dumpguide.exe"


CLEAN :
	-@erase "$(INTDIR)\cg.obj"
	-@erase "$(INTDIR)\dump-guide.obj"
	-@erase "$(INTDIR)\dump.obj"
	-@erase "$(INTDIR)\guide.obj"
	-@erase "$(INTDIR)\headend.obj"
	-@erase "$(INTDIR)\program.obj"
	-@erase "$(INTDIR)\rtv.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\dumpguide.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\dumpguide.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\dumpguide.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\dumpguide.pdb" /machine:I386 /out:"$(OUTDIR)\dumpguide.exe" 
LINK32_OBJS= \
	"$(INTDIR)\cg.obj" \
	"$(INTDIR)\dump-guide.obj" \
	"$(INTDIR)\dump.obj" \
	"$(INTDIR)\guide.obj" \
	"$(INTDIR)\headend.obj" \
	"$(INTDIR)\program.obj" \
	"$(INTDIR)\rtv.obj"

"$(OUTDIR)\dumpguide.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "dumpguide - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\dumpguide.exe"


CLEAN :
	-@erase "$(INTDIR)\cg.obj"
	-@erase "$(INTDIR)\dump-guide.obj"
	-@erase "$(INTDIR)\dump.obj"
	-@erase "$(INTDIR)\guide.obj"
	-@erase "$(INTDIR)\headend.obj"
	-@erase "$(INTDIR)\program.obj"
	-@erase "$(INTDIR)\rtv.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\dumpguide.exe"
	-@erase "$(OUTDIR)\dumpguide.ilk"
	-@erase "$(OUTDIR)\dumpguide.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\dumpguide.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\dumpguide.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\dumpguide.pdb" /debug /machine:I386 /out:"$(OUTDIR)\dumpguide.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\cg.obj" \
	"$(INTDIR)\dump-guide.obj" \
	"$(INTDIR)\dump.obj" \
	"$(INTDIR)\guide.obj" \
	"$(INTDIR)\headend.obj" \
	"$(INTDIR)\program.obj" \
	"$(INTDIR)\rtv.obj"

"$(OUTDIR)\dumpguide.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("dumpguide.dep")
!INCLUDE "dumpguide.dep"
!ELSE 
!MESSAGE Warning: cannot find "dumpguide.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "dumpguide - Win32 Release" || "$(CFG)" == "dumpguide - Win32 Debug"
SOURCE=.\cg.c

"$(INTDIR)\cg.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=".\dump-guide.c"

"$(INTDIR)\dump-guide.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\dump.c

"$(INTDIR)\dump.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\guide.c

"$(INTDIR)\guide.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\headend.c

"$(INTDIR)\headend.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\program.c

"$(INTDIR)\program.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\rtv.c

"$(INTDIR)\rtv.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 


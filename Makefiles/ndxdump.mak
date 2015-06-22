# Microsoft Developer Studio Generated NMAKE File, Based on ndxdump.dsp
!IF "$(CFG)" == ""
CFG=ndxdump - Win32 Debug
!MESSAGE No configuration specified. Defaulting to ndxdump - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "ndxdump - Win32 Release" && "$(CFG)" != "ndxdump - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ndxdump.mak" CFG="ndxdump - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ndxdump - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "ndxdump - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "ndxdump - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\ndxdump.exe"


CLEAN :
	-@erase "$(INTDIR)\bigfile.obj"
	-@erase "$(INTDIR)\ndx-dump.obj"
	-@erase "$(INTDIR)\ndx.obj"
	-@erase "$(INTDIR)\rtv.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\ndxdump.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\ndxdump.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ndxdump.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\ndxdump.pdb" /machine:I386 /out:"$(OUTDIR)\ndxdump.exe" 
LINK32_OBJS= \
	"$(INTDIR)\bigfile.obj" \
	"$(INTDIR)\ndx-dump.obj" \
	"$(INTDIR)\ndx.obj" \
	"$(INTDIR)\rtv.obj"

"$(OUTDIR)\ndxdump.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ndxdump - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\ndxdump.exe"


CLEAN :
	-@erase "$(INTDIR)\bigfile.obj"
	-@erase "$(INTDIR)\ndx-dump.obj"
	-@erase "$(INTDIR)\ndx.obj"
	-@erase "$(INTDIR)\rtv.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\ndxdump.exe"
	-@erase "$(OUTDIR)\ndxdump.ilk"
	-@erase "$(OUTDIR)\ndxdump.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\ndxdump.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ndxdump.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\ndxdump.pdb" /debug /machine:I386 /out:"$(OUTDIR)\ndxdump.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\bigfile.obj" \
	"$(INTDIR)\ndx-dump.obj" \
	"$(INTDIR)\ndx.obj" \
	"$(INTDIR)\rtv.obj"

"$(OUTDIR)\ndxdump.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("ndxdump.dep")
!INCLUDE "ndxdump.dep"
!ELSE 
!MESSAGE Warning: cannot find "ndxdump.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "ndxdump - Win32 Release" || "$(CFG)" == "ndxdump - Win32 Debug"
SOURCE=.\bigfile.c

"$(INTDIR)\bigfile.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=".\ndx-dump.c"

"$(INTDIR)\ndx-dump.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ndx.c

"$(INTDIR)\ndx.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\rtv.c

"$(INTDIR)\rtv.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 


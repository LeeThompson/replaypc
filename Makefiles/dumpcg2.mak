# Microsoft Developer Studio Generated NMAKE File, Based on dumpcg2.dsp
!IF "$(CFG)" == ""
CFG=dumpcg2 - Win32 Debug
!MESSAGE No configuration specified. Defaulting to dumpcg2 - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "dumpcg2 - Win32 Release" && "$(CFG)" != "dumpcg2 - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "dumpcg2.mak" CFG="dumpcg2 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "dumpcg2 - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "dumpcg2 - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "dumpcg2 - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\dumpcg2.exe"


CLEAN :
	-@erase "$(INTDIR)\cg.obj"
	-@erase "$(INTDIR)\dump-cg2.obj"
	-@erase "$(INTDIR)\dump.obj"
	-@erase "$(INTDIR)\program.obj"
	-@erase "$(INTDIR)\rtv.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\dumpcg2.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\dumpcg2.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\dumpcg2.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\dumpcg2.pdb" /machine:I386 /out:"$(OUTDIR)\dumpcg2.exe" 
LINK32_OBJS= \
	"$(INTDIR)\cg.obj" \
	"$(INTDIR)\dump-cg2.obj" \
	"$(INTDIR)\dump.obj" \
	"$(INTDIR)\program.obj" \
	"$(INTDIR)\rtv.obj"

"$(OUTDIR)\dumpcg2.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "dumpcg2 - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\dumpcg2.exe"


CLEAN :
	-@erase "$(INTDIR)\cg.obj"
	-@erase "$(INTDIR)\dump-cg2.obj"
	-@erase "$(INTDIR)\dump.obj"
	-@erase "$(INTDIR)\program.obj"
	-@erase "$(INTDIR)\rtv.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\dumpcg2.exe"
	-@erase "$(OUTDIR)\dumpcg2.ilk"
	-@erase "$(OUTDIR)\dumpcg2.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\dumpcg2.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\dumpcg2.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\dumpcg2.pdb" /debug /machine:I386 /out:"$(OUTDIR)\dumpcg2.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\cg.obj" \
	"$(INTDIR)\dump-cg2.obj" \
	"$(INTDIR)\dump.obj" \
	"$(INTDIR)\program.obj" \
	"$(INTDIR)\rtv.obj"

"$(OUTDIR)\dumpcg2.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("dumpcg2.dep")
!INCLUDE "dumpcg2.dep"
!ELSE 
!MESSAGE Warning: cannot find "dumpcg2.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "dumpcg2 - Win32 Release" || "$(CFG)" == "dumpcg2 - Win32 Debug"
SOURCE=.\cg.c

"$(INTDIR)\cg.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=".\dump-cg2.c"

"$(INTDIR)\dump-cg2.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\dump.c

"$(INTDIR)\dump.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\program.c

"$(INTDIR)\program.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\rtv.c

"$(INTDIR)\rtv.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 


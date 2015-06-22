# Microsoft Developer Studio Generated NMAKE File, Based on ReplayPC.dsp
!IF "$(CFG)" == ""
CFG=ReplayPC - Win32 Debug
!MESSAGE No configuration specified. Defaulting to ReplayPC - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "ReplayPC - Win32 Release" && "$(CFG)" != "ReplayPC - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ReplayPC.mak" CFG="ReplayPC - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ReplayPC - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "ReplayPC - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "ReplayPC - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\ReplayPC.exe"


CLEAN :
	-@erase "$(INTDIR)\httpclient.obj"
	-@erase "$(INTDIR)\httpfsclient.obj"
	-@erase "$(INTDIR)\netclient.obj"
	-@erase "$(INTDIR)\ReplayPC.obj"
	-@erase "$(INTDIR)\rtv.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\ReplayPC.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\ReplayPC.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ReplayPC.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\ReplayPC.pdb" /machine:I386 /out:"$(OUTDIR)\ReplayPC.exe" 
LINK32_OBJS= \
	"$(INTDIR)\httpclient.obj" \
	"$(INTDIR)\httpfsclient.obj" \
	"$(INTDIR)\netclient.obj" \
	"$(INTDIR)\ReplayPC.obj" \
	"$(INTDIR)\rtv.obj"

"$(OUTDIR)\ReplayPC.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ReplayPC - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\ReplayPC.exe"


CLEAN :
	-@erase "$(INTDIR)\httpclient.obj"
	-@erase "$(INTDIR)\httpfsclient.obj"
	-@erase "$(INTDIR)\netclient.obj"
	-@erase "$(INTDIR)\ReplayPC.obj"
	-@erase "$(INTDIR)\rtv.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\ReplayPC.exe"
	-@erase "$(OUTDIR)\ReplayPC.ilk"
	-@erase "$(OUTDIR)\ReplayPC.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\ReplayPC.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ReplayPC.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\ReplayPC.pdb" /debug /machine:I386 /out:"$(OUTDIR)\ReplayPC.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\httpclient.obj" \
	"$(INTDIR)\httpfsclient.obj" \
	"$(INTDIR)\netclient.obj" \
	"$(INTDIR)\ReplayPC.obj" \
	"$(INTDIR)\rtv.obj"

"$(OUTDIR)\ReplayPC.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("ReplayPC.dep")
!INCLUDE "ReplayPC.dep"
!ELSE 
!MESSAGE Warning: cannot find "ReplayPC.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "ReplayPC - Win32 Release" || "$(CFG)" == "ReplayPC - Win32 Debug"
SOURCE=.\httpclient.c

"$(INTDIR)\httpclient.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\httpfsclient.c

"$(INTDIR)\httpfsclient.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\netclient.c

"$(INTDIR)\netclient.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ReplayPC.c

"$(INTDIR)\ReplayPC.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\rtv.c

"$(INTDIR)\rtv.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 


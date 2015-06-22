# Microsoft Developer Studio Generated NMAKE File, Based on ALL.dsp
!IF "$(CFG)" == ""
CFG=ALL - Win32 Debug
!MESSAGE No configuration specified. Defaulting to ALL - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "ALL - Win32 Release" && "$(CFG)" != "ALL - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ALL.mak" CFG="ALL - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ALL - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "ALL - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "ALL - Win32 Release"

OUTDIR=.\ALL___Win32_Release
INTDIR=.\ALL___Win32_Release

!IF "$(RECURSE)" == "0" 

ALL : 

!ELSE 

ALL : "unprotect - Win32 Release" "ReplayPC - Win32 Release" "ndxdump - Win32 Release" "markcommercial - Win32 Release" "httpfs - Win32 Release" "findGOPs - Win32 Release" "dumpzipcode2 - Win32 Release" "dumpheadend - Win32 Release" "dumpguide - Win32 Release" "dumpcg2 - Win32 Release" "buildndxtester - Win32 Release" "buildndx - Win32 Release" 

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"buildndx - Win32 ReleaseCLEAN" "buildndxtester - Win32 ReleaseCLEAN" "dumpcg2 - Win32 ReleaseCLEAN" "dumpguide - Win32 ReleaseCLEAN" "dumpheadend - Win32 ReleaseCLEAN" "dumpzipcode2 - Win32 ReleaseCLEAN" "findGOPs - Win32 ReleaseCLEAN" "httpfs - Win32 ReleaseCLEAN" "markcommercial - Win32 ReleaseCLEAN" "ndxdump - Win32 ReleaseCLEAN" "ReplayPC - Win32 ReleaseCLEAN" "unprotect - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase 

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\ALL.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ALL.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\ALL.pdb" /machine:I386 /out:"$(OUTDIR)\ALL.exe" 
LINK32_OBJS= \
	

!ELSEIF  "$(CFG)" == "ALL - Win32 Debug"

OUTDIR=.\ALL___Win32_Debug
INTDIR=.\ALL___Win32_Debug

!IF "$(RECURSE)" == "0" 

ALL : 

!ELSE 

ALL : "unprotect - Win32 Debug" "ReplayPC - Win32 Debug" "ndxdump - Win32 Debug" "markcommercial - Win32 Debug" "httpfs - Win32 Debug" "findGOPs - Win32 Debug" "dumpzipcode2 - Win32 Debug" "dumpheadend - Win32 Debug" "dumpguide - Win32 Debug" "dumpcg2 - Win32 Debug" "buildndxtester - Win32 Debug" "buildndx - Win32 Debug" 

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"buildndx - Win32 DebugCLEAN" "buildndxtester - Win32 DebugCLEAN" "dumpcg2 - Win32 DebugCLEAN" "dumpguide - Win32 DebugCLEAN" "dumpheadend - Win32 DebugCLEAN" "dumpzipcode2 - Win32 DebugCLEAN" "findGOPs - Win32 DebugCLEAN" "httpfs - Win32 DebugCLEAN" "markcommercial - Win32 DebugCLEAN" "ndxdump - Win32 DebugCLEAN" "ReplayPC - Win32 DebugCLEAN" "unprotect - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase 

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\ALL.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ALL.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\ALL.pdb" /debug /machine:I386 /out:"$(OUTDIR)\ALL.exe" /pdbtype:sept 
LINK32_OBJS= \
	

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("ALL.dep")
!INCLUDE "ALL.dep"
!ELSE 
!MESSAGE Warning: cannot find "ALL.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "ALL - Win32 Release" || "$(CFG)" == "ALL - Win32 Debug"

!IF  "$(CFG)" == "ALL - Win32 Release"

"buildndx - Win32 Release" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\buildndx.mak CFG="buildndx - Win32 Release" 
   cd "."

"buildndx - Win32 ReleaseCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\buildndx.mak CFG="buildndx - Win32 Release" RECURSE=1 CLEAN 
   cd "."

!ELSEIF  "$(CFG)" == "ALL - Win32 Debug"

"buildndx - Win32 Debug" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\buildndx.mak CFG="buildndx - Win32 Debug" 
   cd "."

"buildndx - Win32 DebugCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\buildndx.mak CFG="buildndx - Win32 Debug" RECURSE=1 CLEAN 
   cd "."

!ENDIF 

!IF  "$(CFG)" == "ALL - Win32 Release"

"buildndxtester - Win32 Release" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\buildndxtester.mak CFG="buildndxtester - Win32 Release" 
   cd "."

"buildndxtester - Win32 ReleaseCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\buildndxtester.mak CFG="buildndxtester - Win32 Release" RECURSE=1 CLEAN 
   cd "."

!ELSEIF  "$(CFG)" == "ALL - Win32 Debug"

"buildndxtester - Win32 Debug" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\buildndxtester.mak CFG="buildndxtester - Win32 Debug" 
   cd "."

"buildndxtester - Win32 DebugCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\buildndxtester.mak CFG="buildndxtester - Win32 Debug" RECURSE=1 CLEAN 
   cd "."

!ENDIF 

!IF  "$(CFG)" == "ALL - Win32 Release"

"dumpcg2 - Win32 Release" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\dumpcg2.mak CFG="dumpcg2 - Win32 Release" 
   cd "."

"dumpcg2 - Win32 ReleaseCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\dumpcg2.mak CFG="dumpcg2 - Win32 Release" RECURSE=1 CLEAN 
   cd "."

!ELSEIF  "$(CFG)" == "ALL - Win32 Debug"

"dumpcg2 - Win32 Debug" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\dumpcg2.mak CFG="dumpcg2 - Win32 Debug" 
   cd "."

"dumpcg2 - Win32 DebugCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\dumpcg2.mak CFG="dumpcg2 - Win32 Debug" RECURSE=1 CLEAN 
   cd "."

!ENDIF 

!IF  "$(CFG)" == "ALL - Win32 Release"

"dumpguide - Win32 Release" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\dumpguide.mak CFG="dumpguide - Win32 Release" 
   cd "."

"dumpguide - Win32 ReleaseCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\dumpguide.mak CFG="dumpguide - Win32 Release" RECURSE=1 CLEAN 
   cd "."

!ELSEIF  "$(CFG)" == "ALL - Win32 Debug"

"dumpguide - Win32 Debug" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\dumpguide.mak CFG="dumpguide - Win32 Debug" 
   cd "."

"dumpguide - Win32 DebugCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\dumpguide.mak CFG="dumpguide - Win32 Debug" RECURSE=1 CLEAN 
   cd "."

!ENDIF 

!IF  "$(CFG)" == "ALL - Win32 Release"

"dumpheadend - Win32 Release" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\dumpheadend.mak CFG="dumpheadend - Win32 Release" 
   cd "."

"dumpheadend - Win32 ReleaseCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\dumpheadend.mak CFG="dumpheadend - Win32 Release" RECURSE=1 CLEAN 
   cd "."

!ELSEIF  "$(CFG)" == "ALL - Win32 Debug"

"dumpheadend - Win32 Debug" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\dumpheadend.mak CFG="dumpheadend - Win32 Debug" 
   cd "."

"dumpheadend - Win32 DebugCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\dumpheadend.mak CFG="dumpheadend - Win32 Debug" RECURSE=1 CLEAN 
   cd "."

!ENDIF 

!IF  "$(CFG)" == "ALL - Win32 Release"

"dumpzipcode2 - Win32 Release" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\dumpzipcode2.mak CFG="dumpzipcode2 - Win32 Release" 
   cd "."

"dumpzipcode2 - Win32 ReleaseCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\dumpzipcode2.mak CFG="dumpzipcode2 - Win32 Release" RECURSE=1 CLEAN 
   cd "."

!ELSEIF  "$(CFG)" == "ALL - Win32 Debug"

"dumpzipcode2 - Win32 Debug" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\dumpzipcode2.mak CFG="dumpzipcode2 - Win32 Debug" 
   cd "."

"dumpzipcode2 - Win32 DebugCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\dumpzipcode2.mak CFG="dumpzipcode2 - Win32 Debug" RECURSE=1 CLEAN 
   cd "."

!ENDIF 

!IF  "$(CFG)" == "ALL - Win32 Release"

"findGOPs - Win32 Release" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\findGOPs.mak CFG="findGOPs - Win32 Release" 
   cd "."

"findGOPs - Win32 ReleaseCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\findGOPs.mak CFG="findGOPs - Win32 Release" RECURSE=1 CLEAN 
   cd "."

!ELSEIF  "$(CFG)" == "ALL - Win32 Debug"

"findGOPs - Win32 Debug" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\findGOPs.mak CFG="findGOPs - Win32 Debug" 
   cd "."

"findGOPs - Win32 DebugCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\findGOPs.mak CFG="findGOPs - Win32 Debug" RECURSE=1 CLEAN 
   cd "."

!ENDIF 

!IF  "$(CFG)" == "ALL - Win32 Release"

"httpfs - Win32 Release" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\httpfs.mak CFG="httpfs - Win32 Release" 
   cd "."

"httpfs - Win32 ReleaseCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\httpfs.mak CFG="httpfs - Win32 Release" RECURSE=1 CLEAN 
   cd "."

!ELSEIF  "$(CFG)" == "ALL - Win32 Debug"

"httpfs - Win32 Debug" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\httpfs.mak CFG="httpfs - Win32 Debug" 
   cd "."

"httpfs - Win32 DebugCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\httpfs.mak CFG="httpfs - Win32 Debug" RECURSE=1 CLEAN 
   cd "."

!ENDIF 

!IF  "$(CFG)" == "ALL - Win32 Release"

"markcommercial - Win32 Release" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\markcommercial.mak CFG="markcommercial - Win32 Release" 
   cd "."

"markcommercial - Win32 ReleaseCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\markcommercial.mak CFG="markcommercial - Win32 Release" RECURSE=1 CLEAN 
   cd "."

!ELSEIF  "$(CFG)" == "ALL - Win32 Debug"

"markcommercial - Win32 Debug" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\markcommercial.mak CFG="markcommercial - Win32 Debug" 
   cd "."

"markcommercial - Win32 DebugCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\markcommercial.mak CFG="markcommercial - Win32 Debug" RECURSE=1 CLEAN 
   cd "."

!ENDIF 

!IF  "$(CFG)" == "ALL - Win32 Release"

"ndxdump - Win32 Release" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\ndxdump.mak CFG="ndxdump - Win32 Release" 
   cd "."

"ndxdump - Win32 ReleaseCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\ndxdump.mak CFG="ndxdump - Win32 Release" RECURSE=1 CLEAN 
   cd "."

!ELSEIF  "$(CFG)" == "ALL - Win32 Debug"

"ndxdump - Win32 Debug" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\ndxdump.mak CFG="ndxdump - Win32 Debug" 
   cd "."

"ndxdump - Win32 DebugCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\ndxdump.mak CFG="ndxdump - Win32 Debug" RECURSE=1 CLEAN 
   cd "."

!ENDIF 

!IF  "$(CFG)" == "ALL - Win32 Release"

"ReplayPC - Win32 Release" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\ReplayPC.mak CFG="ReplayPC - Win32 Release" 
   cd "."

"ReplayPC - Win32 ReleaseCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\ReplayPC.mak CFG="ReplayPC - Win32 Release" RECURSE=1 CLEAN 
   cd "."

!ELSEIF  "$(CFG)" == "ALL - Win32 Debug"

"ReplayPC - Win32 Debug" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\ReplayPC.mak CFG="ReplayPC - Win32 Debug" 
   cd "."

"ReplayPC - Win32 DebugCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\ReplayPC.mak CFG="ReplayPC - Win32 Debug" RECURSE=1 CLEAN 
   cd "."

!ENDIF 

!IF  "$(CFG)" == "ALL - Win32 Release"

"unprotect - Win32 Release" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\unprotect.mak CFG="unprotect - Win32 Release" 
   cd "."

"unprotect - Win32 ReleaseCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\unprotect.mak CFG="unprotect - Win32 Release" RECURSE=1 CLEAN 
   cd "."

!ELSEIF  "$(CFG)" == "ALL - Win32 Debug"

"unprotect - Win32 Debug" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\unprotect.mak CFG="unprotect - Win32 Debug" 
   cd "."

"unprotect - Win32 DebugCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\unprotect.mak CFG="unprotect - Win32 Debug" RECURSE=1 CLEAN 
   cd "."

!ENDIF 


!ENDIF 


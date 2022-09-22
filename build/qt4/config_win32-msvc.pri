DEFINES += WIN32 _WINDOWS
DESTDIR = $${TERRALIBPATH}/$${BUILDLOC}/win32-msvc
OBJECTS_DIR = $${DESTDIR}
CONFIG -= incremental
QMAKE_LIBDIR += $${TERRALIBPATH}/$${BUILDLOC}/win32-msvc
CONFIG(debug, debug|release){
	contains(CONFIG, dll) {
		 QMAKE_CFLAGS_MT_DLLDBG += -MDd
		 QMAKE_CXXFLAGS_MT_DLLDBG += -MDd -Od -Fd$${OBJECTS_DIR}/
	}
}
CONFIG(release, debug|release) {
	contains(CONFIG, dll) {
		 QMAKE_CFLAGS_MT_DLL += -O2
		 QMAKE_CXXFLAGS_MT_DLL += -O2 -Fd$${OBJECTS_DIR}/
	}
}
QMAKE_LFLAGS += /IMPLIB:$(OutDir)/$(ProjectName).lib
QMAKE_LFLAGS -= /NOLOGO 
QMAKE_LFLAGS += /PDB:$(OutDir)/$(ProjectName).pdb
QMAKE_CFLAGS += /GS -wd4100
QMAKE_CXXFLAGS += /GS -wd4100
QMAKE_CFLAGS_DEBUG += -RTC1 
QMAKE_CXXFLAGS_DEBUG += -RTC1 

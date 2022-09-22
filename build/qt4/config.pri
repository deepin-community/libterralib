#Defining paths
#-----------------------------
isEmpty(TERRALIBPATH) {
  # Trying to get it from the user environment variable with the same name
  TERRALIBPATH = $$(TERRALIB_DIR)
  
  # The default - if none of previews attempts succeed
  isEmpty(TERRALIBPATH):TERRALIBPATH = ../../..
}
#-----------------------------
TEMPLATE = lib
unix:VERSION = 3.6.1
CONFIG	+= warn_on \
        rtti \
        exceptions \
        thread \
        debug_and_release
CONFIG -= qt
LANGUAGE	= C++
#-----------------------------
#Dependencies definitions
#-----------------------------
CONFIG(debug, debug|release) {
    DEFINES += _DEBUG
    BUILDLOC = Debug
}
CONFIG(release, debug|release) {
    DEFINES += NDEBUG
    BUILDLOC = Release
}
DEFINES -= UNICODE
unix:DESTDIR = $${TERRALIBPATH}/$${BUILDLOC}/linux-g++
OBJECTS_DIR = $${DESTDIR}/obj/$${TARGET}
unix:LIBS += -L$${DESTDIR} 
INCLUDEPATH += $${TERRALIBPATH}/src/terralib/kernel
win32 {
    win32-g++:include(config_win32-g++.pri)
    else:include(config_win32-msvc.pri)
}

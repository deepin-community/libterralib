CONFIG += dll
TARGET = te_spl
include (../config.pri)
DEFINES += TLSPL_AS_DLL

CONFIG += qt

TESPLSRC = $${TERRALIBPATH}/src/terralib/drivers/spl

INCLUDEPATH += $${TESPLSRC} \
			$${TERRALIBPATH}/src/libspl 

unix:LIBS += -lspl
win32 {
	win32-g++:LIBS += -lspl
	else {
		LIBS += -llibspl
	}
}
LIBS += -lterralib
HEADERS += $${TESPLSRC}/TeSPLDefines.h \
		$${TESPLSRC}/PluginInfo.h \
		$${TESPLSRC}/PluginMetadata.h \
		$${TESPLSRC}/PluginParameters.h \
		$${TESPLSRC}/PluginsManager.h \
		$${TESPLSRC}/PluginsParametersContainer.h \
		$${TESPLSRC}/PluginsSignal.h

SOURCES += $${TESPLSRC}/PluginMetadata.cpp \
		$${TESPLSRC}/PluginParameters.cpp \
		$${TESPLSRC}/PluginsManager.cpp \
		$${TESPLSRC}/PluginsSignal.cpp

include (../install_cfg.pri)		
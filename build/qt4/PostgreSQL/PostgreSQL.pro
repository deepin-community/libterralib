CONFIG += dll
TARGET = te_postgresql

include (../config.pri)
# Path definitions
# ----------------------------------------------------------
PGSRCPATH = $${TERRALIBPATH}/src/terralib/drivers/PostgreSQL
PGDEP = $${TERRALIBPATH}/dependencies
win32:PGDEP = $${PGDEP}/win32
unix:PGDEP = $${PGDEP}/linux
PGDEP = $${PGDEP}/PostgreSQL
# ----------------------------------------------------------

DEFINES += TLPOSTGRESQL_AS_DLL

LIBS += -lterralib 

win32 {
    win32-g++:LIBS += -L$${PGDEP}/lib -lwsock32
    else:QMAKE_LIBDIR += $${PGDEP}/lib
    LIBS += -llibpq
}
unix:LIBS += -L$${PGDEP}/lib -lpq -lcrypt

INCLUDEPATH += $${PGSRCPATH}  \
              $${PGSRCPATH}/includepg

HEADERS += $${PGSRCPATH}/TePGInterface.h \
        $${PGSRCPATH}/TePGUtils.h \
        $${PGSRCPATH}/TePostgreSQL.h \
        $${PGSRCPATH}/TePostGIS.h \
        $${PGSRCPATH}/TePostgreSQLDefines.h

SOURCES += $${PGSRCPATH}/TePGInterface.cpp \
        $${PGSRCPATH}/TePGUtils.cpp \
        $${PGSRCPATH}/TePostgreSQL.cpp \
        $${PGSRCPATH}/TePostGIS.cpp
	
include (../install_cfg.pri)

CONFIG(copy_dir_files) {
    psql.path = $${DEPLOY_DIR}/include/PostgreSQL/includepg
    psql.files = $${PGSRCPATH}/includepg/*.h

    include.path = $${DEPLOY_DIR}/include/PostgreSQL
    include.files = $${PGSRCPATH}/*.h

    INSTALLS += psql include
}
#PostgreSQL dependencies install
win32 {
    bin.files = $${PGDEP}/bin/*.dll
    bin.path = $${DEPLOY_DIR}/bin
    INSTALLS += bin
}

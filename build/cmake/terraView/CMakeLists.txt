cmake_minimum_required(VERSION 2.8)

project(terraView)

set(QT_MT_REQUIRED,TRUE)

find_package(Qt3 REQUIRED)

include(../common/common.txt)

# INCLUDES:
include_directories(
  $(TERRALIB_DIR)/terraView
  $(TERRALIB_DIR)/terraView/ui
  $(TERRALIB_DIR)/src/terralib/kernel
  $(TERRALIB_DIR)/src/terralib/functions
  $(TERRALIB_DIR)/src/terralib/utils
  $(TERRALIB_DIR)/src/terralib/stat # add library instead?
  $(TERRALIB_DIR)/src/terralib/image_processing
  $(TERRALIB_DIR)/src/shapelib     # add library instead?
  $(TERRALIB_DIR)/src/qwt/include  # add library instead?
  $(TERRALIB_DIR)/src/libspl # add libspl instead ?
  $(TERRALIB_DIR)/src/terralib/drivers/Oracle/OCI/include
  ${DRIVER_INCLUDES}
)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  include_directories($(TERRALIB_DIR)/terraView/linux/ui)
  add_definitions(-DQT_THREAD_SUPPORT) # should QT_MT_REQUIRED be enough?

ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

# firebird
IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  add_definitions(-DIBPP_LINUX)
# TODO set for other targets:IBPP_WINDOWS/IBPP_LINUX/IBPP_DARWIN
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
file(GLOB SRCS $(TERRALIB_DIR)/terraView/*.cpp)

# TODO make drivers such as oracle optional
list(APPEND SRCS
    $(TERRALIB_DIR)/src/terralib/drivers/Firebird/ibpp/core/all_in_one.cpp
    $(TERRALIB_DIR)/src/terralib/drivers/Firebird/TeFirebird.cpp
    $(TERRALIB_DIR)/src/terralib/drivers/MySQL/TeMySQL.cpp
    $(TERRALIB_DIR)/src/terralib/drivers/Oracle/TeOCIConnect.cpp
    $(TERRALIB_DIR)/src/terralib/drivers/Oracle/TeOCICursor.cpp
    $(TERRALIB_DIR)/src/terralib/drivers/Oracle/TeOCIOracle.cpp
    $(TERRALIB_DIR)/src/terralib/drivers/Oracle/TeOCISDO.cpp
    $(TERRALIB_DIR)/src/terralib/drivers/Oracle/TeOracleSpatial.cpp
    $(TERRALIB_DIR)/src/terralib/drivers/PostgreSQL/TePGInterface.cpp
    $(TERRALIB_DIR)/src/terralib/drivers/PostgreSQL/TePGUtils.cpp
    $(TERRALIB_DIR)/src/terralib/drivers/PostgreSQL/TePostGIS.cpp
    $(TERRALIB_DIR)/src/terralib/drivers/PostgreSQL/TePostgreSQL.cpp
    $(TERRALIB_DIR)/src/terralib/drivers/qt/TeDecoderQtImage.cpp
    $(TERRALIB_DIR)/src/terralib/drivers/qt/TeGUIUtils.cpp
    $(TERRALIB_DIR)/src/terralib/drivers/qt/TeQtAnimaThread.cpp
    $(TERRALIB_DIR)/src/terralib/drivers/qt/TeQtBigTable.cpp
    $(TERRALIB_DIR)/src/terralib/drivers/qt/TeQtCanvas.cpp
    $(TERRALIB_DIR)/src/terralib/drivers/qt/TeQtChartItem.cpp
    $(TERRALIB_DIR)/src/terralib/drivers/qt/TeQtCheckListItem.cpp
    $(TERRALIB_DIR)/src/terralib/drivers/qt/TeQtColorBar.cpp
    $(TERRALIB_DIR)/src/terralib/drivers/qt/TeQtDatabaseItem.cpp
    $(TERRALIB_DIR)/src/terralib/drivers/qt/TeQtDatabasesListView.cpp
    $(TERRALIB_DIR)/src/terralib/drivers/qt/TeQtFrame.cpp
    $(TERRALIB_DIR)/src/terralib/drivers/qt/TeQtGLWidget.cpp
    $(TERRALIB_DIR)/src/terralib/drivers/qt/TeQtGraph.cpp
    $(TERRALIB_DIR)/src/terralib/drivers/qt/TeQtGrid.cpp
    $(TERRALIB_DIR)/src/terralib/drivers/qt/TeQtGridSource.cpp
    $(TERRALIB_DIR)/src/terralib/drivers/qt/TeQtLayerItem.cpp
    $(TERRALIB_DIR)/src/terralib/drivers/qt/TeQtLegendItem.cpp
    $(TERRALIB_DIR)/src/terralib/drivers/qt/TeQtLegendSource.cpp
    $(TERRALIB_DIR)/src/terralib/drivers/qt/TeQtMethods.cpp
    $(TERRALIB_DIR)/src/terralib/drivers/qt/TeQtShowMedia.cpp
    $(TERRALIB_DIR)/src/terralib/drivers/qt/TeQtTable.cpp
    $(TERRALIB_DIR)/src/terralib/drivers/qt/TeQtTerraStat.cpp
    $(TERRALIB_DIR)/src/terralib/drivers/qt/TeQtTextEdit.cpp
    $(TERRALIB_DIR)/src/terralib/drivers/qt/TeQtThemeItem.cpp
    $(TERRALIB_DIR)/src/terralib/drivers/qt/TeQtViewItem.cpp
    $(TERRALIB_DIR)/src/terralib/drivers/qt/TeQtViewsListView.cpp
    $(TERRALIB_DIR)/src/terralib/drivers/qt/TeWaitCursor.cpp
    $(TERRALIB_DIR)/src/terralib/drivers/qwt/TeQwtPlot.cpp
    $(TERRALIB_DIR)/src/terralib/drivers/qwt/TeQwtPlotCurve.cpp
    $(TERRALIB_DIR)/src/terralib/drivers/qwt/TeQwtPlotPicker.cpp
    $(TERRALIB_DIR)/src/terralib/drivers/qwt/TeQwtPlotZoomer.cpp
    $(TERRALIB_DIR)/src/terralib/drivers/shapelib/TeDriverSHPDBF.cpp
    $(TERRALIB_DIR)/src/terralib/drivers/shapelib/TeSHPDriverFactory.cpp
    $(TERRALIB_DIR)/src/terralib/functions/TeAddressLocator.cpp
    $(TERRALIB_DIR)/src/terralib/functions/TeCellAlgorithms.cpp
    $(TERRALIB_DIR)/src/terralib/functions/TeCoordAlgorithms.cpp
    $(TERRALIB_DIR)/src/terralib/functions/TeExportCSV.cpp 
    $(TERRALIB_DIR)/src/terralib/functions/TeExportMIF.cpp
    $(TERRALIB_DIR)/src/terralib/functions/TeExportSPR.cpp
    $(TERRALIB_DIR)/src/terralib/functions/TeGeoProcessingFunctions.cpp
    $(TERRALIB_DIR)/src/terralib/functions/TeImportBNA.cpp
    $(TERRALIB_DIR)/src/terralib/functions/TeImportCSV.cpp
    $(TERRALIB_DIR)/src/terralib/functions/TeImportGeo.cpp
    $(TERRALIB_DIR)/src/terralib/functions/TeImportMIF.cpp
    $(TERRALIB_DIR)/src/terralib/functions/TeLayerFunctions.cpp
    $(TERRALIB_DIR)/src/terralib/functions/TeMIFProjection.cpp
    $(TERRALIB_DIR)/src/terralib/functions/TeSimilarity.cpp
    $(TERRALIB_DIR)/src/terralib/functions/TeSPRFile.cpp
    $(TERRALIB_DIR)/src/terralib/functions/TeThemeFunctions.cpp
    $(TERRALIB_DIR)/src/terralib/kernel/TeDBConnectionsPool.cpp
    $(TERRALIB_DIR)/src/terralib/kernel/TeExternalTheme.cpp
    $(TERRALIB_DIR)/src/terralib/kernel/TeRasterParams.cpp
    $(TERRALIB_DIR)/src/terralib/stat/TeMSVFactory.cpp
    $(TERRALIB_DIR)/src/terralib/stat/TeSemivarModelFactory.cpp
    $(TERRALIB_DIR)/src/terralib/utils/mtrand.cpp
    $(TERRALIB_DIR)/src/terralib/utils/TeColorUtils.cpp
    $(TERRALIB_DIR)/src/terralib/utils/TeDatabaseUtils.cpp
    $(TERRALIB_DIR)/src/terralib/utils/TeUpdateDBVersion.cpp
)


# QT UI:
SET(QT_UIC_EXECUTABLE,uic)
SET(QT_MOC_EXECUTABLE,moc)
file(GLOB UI_FILES $(TERRALIB_DIR)/terraView/ui/*.ui)
QT_WRAP_UI(terraView HDRS TERRAVIEW_UI_SRCS ${UI_FILES} )

file(GLOB MOC_HDRS_DRIVER_QWT $(TERRALIB_DIR)/src/terralib/drivers/qwt/*.h)

QT_WRAP_CPP(terraView SRCS
  $(TERRALIB_DIR)/terraView/terraView.h
  $(TERRALIB_DIR)/src/terralib/drivers/qt/TeQtBigTable.h
  $(TERRALIB_DIR)/src/terralib/drivers/qt/TeQtCanvas.h
  $(TERRALIB_DIR)/src/terralib/drivers/qt/TeQtColorBar.h
  $(TERRALIB_DIR)/src/terralib/drivers/qt/TeQtDatabasesListView.h
  $(TERRALIB_DIR)/src/terralib/drivers/qt/TeQtDataSource.h
  $(TERRALIB_DIR)/src/terralib/drivers/qt/TeQtFrame.h
  $(TERRALIB_DIR)/src/terralib/drivers/qt/TeQtGLWidget.h
  $(TERRALIB_DIR)/src/terralib/drivers/qt/TeQtGrid.h
  $(TERRALIB_DIR)/src/terralib/drivers/qt/TeQtGridSource.h
  $(TERRALIB_DIR)/src/terralib/drivers/qt/TeQtLegendSource.h
  $(TERRALIB_DIR)/src/terralib/drivers/qt/TeQtMethods.h
  $(TERRALIB_DIR)/src/terralib/drivers/qt/TeQtProgress.h
  $(TERRALIB_DIR)/src/terralib/drivers/qt/TeQtShowMedia.h
  $(TERRALIB_DIR)/src/terralib/drivers/qt/TeQtTerraStat.h
  $(TERRALIB_DIR)/src/terralib/drivers/qt/TeQtViewsListView.h

  ${MOC_HDRS_DRIVER_QWT}
)
# QT_WRAP_CPP(terraView SRCS ${HDRS_TO_MOC})

# so that the compliler will find ui generated header files (HDRS)
# There must be a better way to accomplish this
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# terraView executable:
add_executable(terraView ${SRCS} ${TERRAVIEW_UI_SRCS})

MACRO(ADD_SHIPPED_LIB name)
  add_subdirectory(../${name} ${CMAKE_CURRENT_BINARY_DIR}/../build-${name})
  list(APPEND LIBS ${name})
ENDMACRO()

ADD_SHIPPED_LIB(stat)
ADD_SHIPPED_LIB(terralib)
ADD_SHIPPED_LIB(dxflib)
ADD_SHIPPED_LIB(tiff)
ADD_SHIPPED_LIB(shapelib)
ADD_SHIPPED_LIB(TeDXF)
ADD_SHIPPED_LIB(libspl)
ADD_SHIPPED_LIB(qwt) # TODO: allow using external qwt library?
ADD_SHIPPED_LIB(terralibpdi)

# SET_TARGET_PROPERTIES(project_test PROPERTIES LINKER_LANGUAGE C)

# libraries: also see ADD_SHIPPED_LIB calls
target_link_libraries(terraView ${QT_LIBRARIES} ${LIBS}
  clntsh crypt ltidsdk gdal terralibpdi GL GLU 

  drm
  # make these optional: (TODO make optional)

  # mysql
  mysqlclient 
  # Firebird
  fbembed
  # Postgresql (Postgis) ?
  pq 
  # Oracle
  nnz11 
) 

# for testing only: copy pixmaps into build directory so that terraView finds
# them:

file(GLOB BMPS $(TERRALIB_DIR)/terraView/ui/images/*)
file(COPY ${BMPS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

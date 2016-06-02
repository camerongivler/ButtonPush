cmake_minimum_required(VERSION 2.8.11)
PROJECT(buttonPush)

list(APPEND CMAKE_MODULE_PATH ${buttonPush_SOURCE_DIR} ${buttonPush_SOURCE_DIR}/cmake)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
SET(buttonPush_SOURCES main.cpp mainwindow.cpp VRPN_AciCommand_transport.cpp)
SET(buttonPush_FORMS mainwindow.ui)
#SET(buttonPush_HEADERS mainwindow.h VRPN_AciCommand_transport.h)

FIND_PACKAGE(Qt5Widgets REQUIRED)
FIND_PACKAGE(Boost 1.55 COMPONENTS system regex thread filesystem program_options REQUIRED )

FIND_PACKAGE(Threads REQUIRED)
FIND_PACKAGE(JSONBOX REQUIRED)
FIND_PACKAGE(MongoDB REQUIRED)
FIND_PACKAGE(MongoInterface REQUIRED)
FIND_PACKAGE(VRPN REQUIRED)

qt5_wrap_ui( buttonPush_UIFILES ${buttonPush_FORMS} )

add_executable( buttonPush ${buttonPush_SOURCES} ${buttonPush_UIFILES} )

target_link_libraries( buttonPush Qt5::Widgets ${MongoInterface_LIBRARY} ${JSONBOX_LIBRARY_RELEASE} ${MongoDB_LIBRARIES} ${Boost_LIBRARIES} ${VRPN_LIBRARY} ${CMAKE_THREAD_LIBS_INIT} )

add_executable(testServer VRPN_testServer.cpp VRPN_AciCommand_transport.cpp)

target_link_libraries(testServer ${VRPN_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})


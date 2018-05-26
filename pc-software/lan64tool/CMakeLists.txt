# //////////////////////////////////////////////////
# //                                              //
# // Lan64Tool                                    //
# // von Thorsten Kattanek                        //
# //                                              //
# // #file: CMakeLists.txt                        //
# //                                              //
# // Letzte Änderung am 21.05.2018                //
# // www.emu64.de                                 //
# //                                              //
# //////////////////////////////////////////////////

###################################################
# Options
# -CMAKE_INSTALL_PREFIX=[INSTALL_PATH] --- Setzt den Installationspfad
# -DWIN32_STATIC_BUILD (TRUE or FLASE) --- Wenn TRUE wird ein Windows Static Binarie erstellt
# -DCMAKE_BUILD_TYPE (Release or Debug) --- Build Typ welches erzeugt werden soll (Default is Release)

cmake_minimum_required(VERSION 2.8)

##################### PROJEKT #####################

project("lan64tool")
message(STATUS "Project Name: " ${PROJECT_NAME})

################## CONFIGURATION ##################

# Standard Build Typ = Release
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
    message(STATUS "CMAKE_BUILD_TYPE NOT FOUND -- Default setting to Release")
endif()

# Standard WIN32_STATIC_BUILD = FALSE
if(NOT WIN32_STATIC_BUILD)
    set(WIN32_STATIC_BUILD FALSE CACHE STRING "Choose a static or shared win32 build (TRUE or FALSE)" FORCE)
    message(STATUS "WIN32_STATIC_BUILD NOT FOUND -- Default setting to FALSE")
endif()

if(WIN32)
    if(WIN32_STATIC_BUILD)
        message(STATUS "BUILD A W32 STATIC BINARY")
        add_definitions("-DQUAZIP_STATIC")
    endif()
endif()

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

############### COMPILER OPTIONEN #################

## Enable C++11 as default
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR CMAKE_COMPILER_IS_GNUCXX)
    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG(--std=c++11 SUPPORTS_STD_CXX11)
    CHECK_CXX_COMPILER_FLAG(--std=c++0x SUPPORTS_STD_CXX01)
    if(SUPPORTS_STD_CXX11)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --std=c++11")
    elseif(SUPPORTS_STD_CXX01)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++0x")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --std=c++0x")
    else()
        message(ERROR "Compiler does not support --std=c++11 or --std=c++0x.")
    endif()
endif()

# Verschärfte Fehlerbehandlung beim kompilieren
if(NOT ALL_ERRORS)
    set(ALL_ERRORS FALSE CACHE STRING "For all error messages (TRUE)" FORCE)
    message(STATUS "ALL_ERRORS NOT FOUND -- Default setting to FALSE")
endif()

if(ALL_ERRORS)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif()

if(WIN32)
       # Ohne Konsole aber auch ohne Ausgabe in Bash !!
       #set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s -O2 -g2 -Wl,-subsystem,windows")
       #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s -O2 -g2 -Wl,-subsystem,windows")

       # Mit Konsole und Ausgabe ... Die Konsole wird aber immer geöffnet
       set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s -O2 -g2")
       set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s -O2 -g2")
endif()

############ VERSIONSNUMMER via GIT ###############

# Holen des aktuellen Arbeitsbranches
execute_process(
  COMMAND git rev-parse --abbrev-ref HEAD
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_BRANCH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Holen des Hashecodes vom letzten Commits
execute_process(
  COMMAND git log -1 --format=%h
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_COMMIT_HASH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Version aus dem Tag ermitteln
execute_process(
  COMMAND git describe --always --tags
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE VERSION_STRING
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

add_definitions("-DVERSION_STRING=\"${VERSION_STRING}\"")

message(STATUS "Aktueller Git Hashcode : ${GIT_COMMIT_HASH}")
message(STATUS "Aktueller Arbeitsbranch: ${GIT_BRANCH}")
message(STATUS "Aktuelle Lan64Tool Version : ${VERSION_STRING}")

############## ARCHITEKTUR ERMITTELN ##############

if ( "${CMAKE_SIZEOF_VOID_P}" EQUAL 4)
        set(ARCHITECTURE "32Bit")
elseif ( "${CMAKE_SIZEOF_VOID_P}" EQUAL 8)
        set(ARCHITECTURE "64Bit")
endif()

add_definitions("-DARCHITECTURE_STRING=\"${ARCHITECTURE}\"")

message(STATUS "Architektur: ${ARCHITECTURE}")

##################### SOURCES #####################

set(SOURCES
    src/main.cpp
    src/mainwindow.cpp  
    src/lan64usb_class.cpp
    src/hiddata.c
)

################## MOC HEADERS ####################

set(MOC_HEADERS
    src/mainwindow.h
)

###################### UIS ########################

set(UIS
    src/mainwindow.ui
)

################### RESOURCEN #####################

#set(QT_RC ${PROJECT_SOURCE_DIR}/src/${PROJECT_NAME}.qrc)

#if(WIN32)
#    set(WIN_RC ${PROJECT_SOURCE_DIR}/src/${PROJECT_NAME}.rc)
#endif()

################# TRANSLATIONS ####################

#SET(TRANS
#    src/emu64_de.ts
#    src/emu64_en.ts
#    src/qt_de.ts
#)

##################### LIBRARY #####################

# Qt4
find_package(Qt4 REQUIRED )
if(QT4_FOUND)
    include(${QT_USE_FILE})
    add_definitions(${QT_DEFINITIONS})
    set(LIBRARIES ${LIBRARIES} ${QT_LIBRARIES})
    include_directories("${PROJECT_SOURCE_DIR}/src")
else()
    message(FATAL_ERROR "Qt4 wurde nicht gefunden!")
endif()

# LibUSB
find_package(LibUSB REQUIRED )
if(LibUSB_FOUND)
    set(LIBRARIES ${LIBRARIES} ${LibUSB_LIBRARIES})
    include_directories(${LibUSB_INCLUDE_DIRS})
else()
    message(FATAL_ERROR "LibUsb wurde nicht gefunden!")
endif()


# Für Windows
#if(WIN32)
#    if(CMAKE_CROSSCOMPILING)
#        message(STATUS "Windows Crosscompiling ...")

        # LIBUSB
#        execute_process(
#          COMMAND ${PKG_CONFIG_EXECUTABLE} libusb --libs
#          OUTPUT_VARIABLE LIBUSB_LIBS
#          OUTPUT_STRIP_TRAILING_WHITESPACE
#        )
        
#        set(WIN_LIBRARIES ${WIN_LIBRARIES} ${QT4_LIBS} ${LIBUSB_LIBS} -lqtmain)
#     endif()   
#endif()

###################################################

#QT4_ADD_TRANSLATION(QM ${TRANS})
#QT4_ADD_RESOURCES(RES_SOURCES ${QT_RC})
QT4_WRAP_UI(UI_HEADERS ${UIS})
QT4_WRAP_CPP(MOC_SRCS ${MOC_HEADERS})

################### EXECUTABLE ####################

if(NOT WIN32)
    add_executable(${PROJECT_NAME} ${SOURCES} ${MOC_SRCS} ${RES_SOURCES} ${UI_HEADERS} ${QM})
    target_link_libraries(${PROJECT_NAME} ${LIBRARIES})
else()
    add_executable(${PROJECT_NAME} WIN32 ${SOURCES} ${MOC_SRCS} ${RES_SOURCES} ${UI_HEADERS} ${QM} ${WIN_RC})
    target_link_libraries(${PROJECT_NAME} ${WIN_LIBRARIES})
endif()

################### INSTALL #######################

############ Install Binary und Daten #############


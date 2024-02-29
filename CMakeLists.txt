cmake_minimum_required(VERSION 3.5)

project(MyPuzzleCreator VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets PrintSupport)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets PrintSupport)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(MyPuzzleCreator
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        puzzleshapemanager.h puzzleshapemanager.cpp


        imagedividerwithbezier.h imagedividerwithbezier.cpp
        puzzlesetupsettingsdialog.h puzzlesetupsettingsdialog.cpp puzzlesetupsettingsdialog.ui

        puzzleedgedata.h puzzleedgedata.cpp
        Resources.qrc
        playpuzzlegamedialog.h playpuzzlegamedialog.cpp playpuzzlegamedialog.ui

        playpuzzlesshapes.h playpuzzlesshapes.cpp playpuzzlesshapes.ui

        imageholderwidget.h imageholderwidget.cpp
        itemhidenamedelegate.h itemhidenamedelegate.cpp
        customlistview.h customlistview.cpp

    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET MyPuzzleCreator APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(MyPuzzleCreator SHARED
            ${PROJECT_SOURCES}
        )
elseif(QT_FEATURE_printdialog)
    add_executable(MyPuzzleCreator
        ${PROJECT_SOURCES}
    )
target_link_libraries(MyPuzzleCreator PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::PrintSupport)
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
else()
    add_executable(MyPuzzleCreator
        ${PROJECT_SOURCES}
    )
target_link_libraries(MyPuzzleCreator PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
endif()
endif()

target_link_libraries(MyPuzzleCreator PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(MyPuzzleCreator PRIVATE Qt${QT_VERSION_MAJOR}::PrintSupport)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.MyPuzzleCreator)
endif()
set_target_properties(MyPuzzleCreator PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS MyPuzzleCreator
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(MyPuzzleCreator)
endif()

set(CORE_SOURCES
    calchandler.cpp
    displayformatter.cpp
    inputvalidator.cpp
    calculationhistory.cpp
    historydialog.cpp
    historypanel.cpp
    memorymanager.cpp
    memorydropdowndialog.cpp
    uianimations.cpp
    thememanager.cpp
)

set(CORE_HEADERS
    calchandler.h
    displayformatter.h
    inputvalidator.h
    calculatorconfig.h
    calculationhistory.h
    historydialog.h
    historypanel.h
    memorymanager.h
    memorydropdowndialog.h
    uianimations.h
    thememanager.h
)

add_library(calc_core
    ${CORE_SOURCES}
    ${CORE_HEADERS}
)

target_link_libraries(calc_core
    PUBLIC Qt${QT_VERSION_MAJOR}::Core
    PUBLIC Qt${QT_VERSION_MAJOR}::Widgets
)

target_include_directories(calc_core
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

set(APP_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    resources.qrc
)

set(APP_QRC ${PROJECT_SOURCE_DIR}/resources/app.qrc)
if(EXISTS ${APP_QRC})
    list(APPEND APP_SOURCES ${APP_QRC})
endif()

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(calc
        MANUAL_FINALIZATION
        ${APP_SOURCES}
    )
else()
    if(ANDROID)
        add_library(calc SHARED
            ${APP_SOURCES}
        )
    else()
        add_executable(calc
            ${APP_SOURCES}
        )
    endif()
endif()

target_link_libraries(calc
    PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
    PRIVATE calc_core
)

target_include_directories(calc
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(calc PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(calc)
endif()

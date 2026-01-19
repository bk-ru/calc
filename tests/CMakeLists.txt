find_package(QT NAMES Qt6 Qt5 COMPONENTS Test Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Test Widgets REQUIRED)

# Тест CalcHandler
add_executable(test_calchandler
    test_calchandler.cpp
)
target_link_libraries(test_calchandler
    PRIVATE Qt${QT_VERSION_MAJOR}::Test
    PRIVATE calc_core
)
add_test(NAME test_calchandler COMMAND test_calchandler)

# Тест DisplayFormatter
add_executable(test_displayformatter
    test_displayformatter.cpp
)
target_link_libraries(test_displayformatter
    PRIVATE Qt${QT_VERSION_MAJOR}::Test
    PRIVATE calc_core
)
add_test(NAME test_displayformatter COMMAND test_displayformatter)

# Тест InputValidator
add_executable(test_inputvalidator
    test_inputvalidator.cpp
)
target_link_libraries(test_inputvalidator
    PRIVATE Qt${QT_VERSION_MAJOR}::Test
    PRIVATE calc_core
)
add_test(NAME test_inputvalidator COMMAND test_inputvalidator)

# Тест CalculationHistory
add_executable(test_calculationhistory
    test_calculationhistory.cpp
)
target_link_libraries(test_calculationhistory
    PRIVATE Qt${QT_VERSION_MAJOR}::Test
    PRIVATE calc_core
)
add_test(NAME test_calculationhistory COMMAND test_calculationhistory)

# Тест MemoryManager
add_executable(test_memorymanager
    test_memorymanager.cpp
)
target_link_libraries(test_memorymanager
    PRIVATE Qt${QT_VERSION_MAJOR}::Test
    PRIVATE calc_core
)
add_test(NAME test_memorymanager COMMAND test_memorymanager)

# Тест UIAnimations
add_executable(test_uianimations
    test_uianimations.cpp
)
target_link_libraries(test_uianimations
    PRIVATE Qt${QT_VERSION_MAJOR}::Test
    PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
    PRIVATE calc_core
)
add_test(NAME test_uianimations COMMAND test_uianimations)

# Тест ThemeManager
add_executable(test_thememanager
    test_thememanager.cpp
)
target_link_libraries(test_thememanager
    PRIVATE Qt${QT_VERSION_MAJOR}::Test
    PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
    PRIVATE calc_core
)
add_test(NAME test_thememanager COMMAND test_thememanager)

# Тест MainWindow
add_executable(test_mainwindow
    test_mainwindow.cpp
    ${PROJECT_SOURCE_DIR}/src/mainwindow.cpp
    ${PROJECT_SOURCE_DIR}/src/mainwindow.h
    ${PROJECT_SOURCE_DIR}/src/mainwindow.ui
)
target_link_libraries(test_mainwindow
    PRIVATE Qt${QT_VERSION_MAJOR}::Test
    PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
    PRIVATE calc_core
)
target_include_directories(test_mainwindow
    PRIVATE ${PROJECT_SOURCE_DIR}/src
)
add_test(NAME test_mainwindow COMMAND test_mainwindow)

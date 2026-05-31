cmake_minimum_required(VERSION 3.16)
project(RandomNamePicker)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Set QT6 path (使用您的QT安装路径)
set(Qt6_DIR "D:/Developenv/Qt/6.9.3/mingw_64/lib/cmake/Qt6")

# Find Qt6 package
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)

# Enable AUTOMOC for Qt meta-object compilation
set(CMAKE_AUTOMOC ON)

# Source files for console main application
set(CONSOLE_MAIN_SOURCES
    src/main.cpp
    src/utils/platform.cpp
    src/data/name_list.cpp
    src/config/config_manager.cpp
    src/core/randomizer.cpp
    src/ui/console_ui.cpp
)

# Source files for setup tool
set(SETUP_SOURCES
    tools/setup/setup.cpp
    src/utils/platform.cpp
    src/data/name_list.cpp
    src/config/config_manager.cpp
    src/ui/console_ui.cpp
)

# Source files for Qt GUI application
set(QT_GUI_SOURCES
    src/gui/main.cpp
    src/gui/main_window.cpp
    src/utils/platform.cpp
    src/data/name_list.cpp
    src/config/config_manager.cpp
    src/core/randomizer.cpp
)

# Include directories
include_directories(
    src/
    src/utils/
    src/data/
    src/config/
    src/core/
    src/ui/
    src/gui/
)

# Create console main application
add_executable(RandomNamePicker ${CONSOLE_MAIN_SOURCES})

# Create setup tool
add_executable(SetupTool ${SETUP_SOURCES})

# Create Qt GUI application
add_executable(RandomNamePickerGUI ${QT_GUI_SOURCES})
target_link_libraries(RandomNamePickerGUI Qt6::Core Qt6::Widgets Qt6::Gui)

# Platform-specific settings
if(WIN32)
    # Windows specific settings if needed
else()
    # Linux/Unix specific settings if needed
endif()

# Installation (optional)
install(TARGETS RandomNamePicker SetupTool RandomNamePickerGUI
    RUNTIME DESTINATION bin
)

# Copy data directory to build directory
file(COPY data DESTINATION ${CMAKE_BINARY_DIR}/bin)

# Create build data directory
add_custom_target(create_data_dir ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/bin/data
    COMMENT "Creating data directory"
)

# Copy UI file to build directory
add_custom_command(TARGET RandomNamePickerGUI POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_SOURCE_DIR}/ui_files/MainWindow.ui
    ${CMAKE_BINARY_DIR}/bin/
    COMMENT "Copying UI file to build directory"
)


cmake_minimum_required(VERSION 3.10)
project(MyWxWidgetsApp)

# Set the C++ standard to C++11 (or later if needed)
set(CMAKE_CXX_STANDARD 11)

# Specify the path to your wxWidgets build directory
set(wxWidgets_ROOT_DIR "${PROJECT_SOURCE_DIR}/../../libs/wxWidgets-build")
set(wxWidgets_LIB_DIR "${wxWidgets_ROOT_DIR}/lib")
set(wxWidgets_INCLUDE_DIRS "${wxWidgets_ROOT_DIR}/include")

# Explicitly set the wx-config executable path
set(wxWidgets_CONFIG_EXECUTABLE "${PROJECT_SOURCE_DIR}/../../libs/wxWidgets-build/wx-config")

# Add wxWidgets include directories explicitly
include_directories(${wxWidgets_INCLUDE_DIRS})

# Add the library directory
link_directories(${wxWidgets_LIB_DIR})

# Find the wxWidgets package
find_package(wxWidgets REQUIRED COMPONENTS core base)

# Include the wxWidgets use file
include(${wxWidgets_USE_FILE})

# Add the executable target
add_executable(MyWxWidgetsApp MainFrame.cpp
        App.cpp
        Character.cpp
)

# Link the wxWidgets libraries
target_link_libraries(MyWxWidgetsApp ${wxWidgets_LIBRARIES})

# Include the wxWidgets headers
target_include_directories(MyWxWidgetsApp PRIVATE ${wxWidgets_INCLUDE_DIRS})
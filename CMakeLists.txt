cmake_minimum_required(VERSION 3.0)
project(ualab)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
# Instruct CMake to create code from Qt designer ui files
set(CMAKE_AUTOUIC ON)

# Find libraryies
include(FindQwt.cmake)
find_package(Qt5Widgets CONFIG REQUIRED)

set(ualab_SRC
  src/main.cpp
  src/ualab.cpp
)

# Tell CMake to create the executable
add_executable(ualab ${ualab_SRC})

# Use the Widgets module from Qt 5.
target_link_libraries(ualab Qt5::Widgets Qwt::Qwt)

# Install the executable
install(TARGETS ualab DESTINATION bin)

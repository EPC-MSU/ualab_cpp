cmake_minimum_required(VERSION 3.0)
project(ualab)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
# Instruct CMake to create code from Qt designer ui files
set(CMAKE_AUTOUIC ON)

# Find libraries
include(FindQwt.cmake)
find_package(Qt5 CONFIG REQUIRED Core Widgets Gui SerialPort)

#Link to usbadc10
if(${CMAKE_SYSTEM} MATCHES "Linux")
    message ("Linux")
    link_directories(${CMAKE_SOURCE_DIR}/usbadc10-debian)
elseif(WIN32)
    link_directories(${CMAKE_SOURCE_DIR}/usbadc10-win64)
else()
    message ("Usuported system")
endif()

set(ualab_SRC
  src/main.cpp
  src/ualab.cpp
)

# Tell CMake to create the executable
add_executable(ualab ${ualab_SRC})
#qt5_use_modules(ualab Core Widgets Gui SerialPort)
# Use the Widgets module from Qt 5.
target_link_libraries(ualab Qt5::Widgets Qt5::SerialPort Qwt::Qwt usbadc10)

# Install the executable
install(TARGETS ualab DESTINATION bin)

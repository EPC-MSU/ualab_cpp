cmake_minimum_required(VERSION 3.0)
project(UALab)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
# Instruct CMake to create code from Qt designer ui files
set(CMAKE_AUTOUIC ON)

# Find libraries
include(FindQwt_win.cmake)
IF(${CMAKE_SYSTEM_NAME} STREQUAL Windows)
    find_package (Qt5 COMPONENTS REQUIRED Core Widgets Gui SerialPort)
else()   
    find_package(Qt5 CONFIG REQUIRED Core Widgets Gui SerialPort)
endif()

#Link to usbadc10
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    message ("Linux")
    link_directories(${CMAKE_SOURCE_DIR}/usbadc10-debian)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    message ("Windows")
    link_directories(${CMAKE_SOURCE_DIR}/usbadc10-win32)
else()
    message ("Usuported system")
endif()

set(UALab_HDR
  src/ualab.h
  src/usbadc10.h
  src/dataupdater.h
)

set(UALab_SRC
  src/main.cpp
  src/ualab.cpp
  src/dataupdater.cpp
)

# Tell CMake to create the executable
add_executable(UALab ${UALab_SRC})
# Comile optons
# target_compile_features(ualab cxx_std_11)
# Use the Widgets module from Qt 5.
target_link_libraries(UALab Qt5::Widgets Qt5::SerialPort Qwt::Qwt usbadc10)

# Install the executable
install(TARGETS UALab DESTINATION bin)

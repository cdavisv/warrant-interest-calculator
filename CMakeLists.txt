cmake_minimum_required(VERSION 3.20)
project(WarrantCalculator VERSION 2.0.0 LANGUAGES CXX RC)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(WarrantCalculator WIN32
    src/main.cpp
    src/app_window.cpp
    src/calculator.cpp
    src/date_utils.cpp
    src/currency.cpp
    src/app.rc
)

target_include_directories(WarrantCalculator PRIVATE src)

# Link against Windows libraries
target_link_libraries(WarrantCalculator PRIVATE
    comctl32
    user32
    gdi32
    kernel32
)

# Enable Unicode
target_compile_definitions(WarrantCalculator PRIVATE
    UNICODE
    _UNICODE
    NOMINMAX
)

# Static CRT linking for standalone executable
if(MSVC)
    set_property(TARGET WarrantCalculator PROPERTY
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
    )
endif()

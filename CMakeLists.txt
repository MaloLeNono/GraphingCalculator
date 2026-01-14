cmake_minimum_required(VERSION 3.31)
project(GraphingCalculator)

set(CMAKE_CXX_STANDARD 26)

add_compile_options(/bigobj)

find_package(SDL3 CONFIG REQUIRED)

add_executable(GraphingCalculator src/main.cpp)

target_link_libraries(GraphingCalculator PRIVATE SDL3::SDL3)

install(TARGETS GraphingCalculator RUNTIME DESTINATION bin)
install(TARGETS GraphingCalculator RUNTIME_DEPENDENCIES
        PRE_EXCLUDE_REGEXES "api-ms-" "ext-ms-"
        POST_EXCLUDE_REGEXES ".*system32/.*\\.dll")
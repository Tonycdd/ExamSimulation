# Root CMakeLists.txt
cmake_minimum_required(VERSION 3.14)
project(my_project)

#kato cuknesh Run CTest da raboti
enable_testing()

#standart 
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Vkluchvane na FetchContent za Catch2
include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.4.0
)
FetchContent_MakeAvailable(Catch2)

# trqbvashe da kopiram .txt, tui kato cmake ne go kopira do izpulnimiq fail, no sled tezi redove vsichko e ok
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/input.txt
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/src/input.txt
            ${CMAKE_CURRENT_BINARY_DIR}/input.txt
    DEPENDS ${CMAKE_SOURCE_DIR}/src/input.txt
)
add_custom_target(copy_synthes_file ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/input.txt)


add_library(my_lib
    src/MyVector.hpp
    src/Student.hpp
)
set_target_properties(my_lib PROPERTIES LINKER_LANGUAGE CXX)

#glavna programa
add_executable(main_program
    src/main.cpp
)
target_link_libraries(main_program PRIVATE my_lib)

# dobavqne na testove
add_executable(tests
    test/test.cpp
)
#svurzvane
target_link_libraries(tests
    PRIVATE
    Catch2::Catch2WithMain
    my_lib
)

# vkuchi Ctest
include(CTest)
include(Catch)
catch_discover_tests(tests)
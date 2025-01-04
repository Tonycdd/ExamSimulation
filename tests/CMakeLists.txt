# Добавяне на тестове
add_executable(tests
    test/test.cpp
)

# Свързване на библиотеките и добавяне на include директории
target_link_libraries(tests
    PRIVATE
    Catch2::Catch2WithMain
    my_lib
)

target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/src)
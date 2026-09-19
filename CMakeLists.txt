cmake_minimum_required(VERSION 3.10)

project(YAMNlang LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra)
endif()

add_executable(language_parse
    language-parse.cpp
    type-enum.cpp
    trie-tree.cpp
)

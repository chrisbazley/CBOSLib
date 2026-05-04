cmake_minimum_required(VERSION 3.10)
project(CBOSLib C)

include(FetchContent)

FetchContent_Declare(OptionalAcornC GIT_REPOSITORY https://github.com/chrisbazley/OptionalAcornC.git GIT_TAG main)

FetchContent_MakeAvailable(OptionalAcornC)

set(CMAKE_C_STANDARD 99)

if(MSVC)
    # MSVC equivalent of -Wall -Wextra is /W4
    add_compile_options(/W4)
else()
    # For GCC and Clang
    add_compile_options(-Wall -Wextra -Wsign-conversion -pedantic)
endif()

file(GLOB SOURCES "*.c")
file(GLOB PUBLIC_HEADERS "*.h")

add_library(CBOS STATIC ${SOURCES} ${PUBLIC_HEADERS})

set_target_properties(CBOS PROPERTIES
    PUBLIC_HEADER "${PUBLIC_HEADERS}"
    DEBUG_POSTFIX "dbg"
)

target_include_directories(CBOS PUBLIC .)

target_compile_definitions(CBOS PRIVATE
    $<$<CONFIG:Debug>:DEBUG_OUTPUT>
    $<$<CONFIG:Release>:NDEBUG>
)

target_compile_options(CBOS PRIVATE
    $<$<CONFIG:Release>:-O3>
)

target_link_libraries(CBOS PRIVATE
    OptionalAcornC
)

install(TARGETS CBOS
    ARCHIVE DESTINATION lib
    PUBLIC_HEADER DESTINATION include
)

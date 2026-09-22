cmake_minimum_required(VERSION 3.14)
project(CBOSLib C)

option(USE_OPTIONAL "Enable the _Optional qualifier" OFF)
option(USE_FORTIFY "Enable Fortify memory-allocation checking" OFF)
option(ENABLE_CLANG_TIDY "Run clang-tidy during compilation" OFF)

include(FetchContent)

FetchContent_Declare(OptionalAcornC GIT_REPOSITORY https://github.com/chrisbazley/OptionalAcornC.git GIT_TAG main)
FetchContent_Declare(CBUtil GIT_REPOSITORY https://github.com/chrisbazley/CBUtilLib.git GIT_TAG main)

FetchContent_MakeAvailable(OptionalAcornC)
FetchContent_MakeAvailable(CBUtil)

if(USE_OPTIONAL)
    if(NOT CMAKE_C_COMPILER_ID MATCHES "Clang|AppleClang")
        message(FATAL_ERROR
            "USE_OPTIONAL requires Christopher Bazley's Clang fork")
    endif()

    add_compile_definitions(USE_OPTIONAL)

    if(CMAKE_GENERATOR STREQUAL "Xcode")
        set(CMAKE_XCODE_ATTRIBUTE_RUN_CLANG_STATIC_ANALYZER "YES")
        set(CMAKE_XCODE_ATTRIBUTE_CLANG_ANALYZER_OTHER_FLAGS
            "-analyzer-checker optionality,core -analyzer-max-loop 32")
    endif()
endif()

if(USE_FORTIFY)
    FetchContent_Declare(CBPseudo
        GIT_REPOSITORY https://github.com/chrisbazley/CBPseudoLib.git
        GIT_TAG main
    )

    FetchContent_MakeAvailable(CBPseudo)
endif()

set(CMAKE_C_STANDARD 99)
set(CMAKE_XCODE_ATTRIBUTE_RUN_CLANG_STATIC_ANALYZER "YES")

if(MSVC)
    # MSVC equivalent of -Wall -Wextra is /W4
    add_compile_options(/W4 /wd4200 /wd4996 /wd4130)
else()
    # For GCC and Clang
    add_compile_options(-Wall -Wextra -Wsign-compare -pedantic)
endif()

if(ENABLE_CLANG_TIDY)
    if(NOT CMAKE_C_COMPILER_ID MATCHES "Clang|AppleClang")
        message(FATAL_ERROR "ENABLE_CLANG_TIDY requires Clang")
    endif()

    find_program(CLANG_TIDY_EXE
        NAMES clang-tidy
        HINTS "/opt/homebrew/opt/llvm/bin"
    )

    if(NOT CLANG_TIDY_EXE)
        message(FATAL_ERROR
            "ENABLE_CLANG_TIDY is ON, but clang-tidy was not found")
    endif()

    set(CMAKE_C_CLANG_TIDY
        "${CLANG_TIDY_EXE}"
        "--checks=-*,clang-analyzer-*,-clang-analyzer-security.insecureAPI.*"
        "--warnings-as-errors=clang-analyzer-optionality.*"
    )

    message(STATUS "clang-tidy: ${CLANG_TIDY_EXE}")
    message(STATUS "_Optional static analysis enabled")
endif()

file(GLOB SOURCES "*.c")
file(GLOB PUBLIC_HEADERS "*.h")
file(GLOB PRIVATE_HEADERS "Internal/*.h")

add_library(CBOS STATIC ${SOURCES} ${PUBLIC_HEADERS} ${PRIVATE_HEADERS})

set_target_properties(CBOS PROPERTIES
    PUBLIC_HEADER "${PUBLIC_HEADERS}"
    DEBUG_POSTFIX "dbg"
)

target_include_directories(CBOS PUBLIC .)

target_compile_definitions(CBOS PRIVATE
    $<$<CONFIG:Debug>:DEBUG_OUTPUT>
)

# Header files depend on definitions from these dependencies
target_link_libraries(CBOS PUBLIC
    OptionalAcornC
)

target_link_libraries(CBOS
    PRIVATE
        CBUtil
)

if(USE_FORTIFY)
    target_link_libraries(CBOS PRIVATE CBPseudo)
endif()

install(TARGETS CBOS
    ARCHIVE DESTINATION lib
    PUBLIC_HEADER DESTINATION include
)

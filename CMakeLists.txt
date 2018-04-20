#based on https://github.com/ainfosec/ci_helloworld/blob/master/CMakeLists.txt

cmake_minimum_required(VERSION 3.6)
project(StaticAnalysisDemo C)

include(ExternalProject)

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#clangtidy
find_program(CLANG_TIDY_BIN clang-tidy-4.0)
find_program(RUN_CLANG_TIDY_BIN run-clang-tidy-4.0.py)

if(CLANG_TIDY_BIN STREQUAL "CLANG_TIDY_BIN-NOTFOUND")
    message(FATAL_ERROR "unable to locate clang-tidy-4.0")
endif()

if(RUN_CLANG_TIDY_BIN STREQUAL "RUN_CLANG_TIDY_BIN-NOTFOUND")
    message(FATAL_ERROR "unable to locate run-clang-tidy-4.0.py")
endif()

list(APPEND RUN_CLANG_TIDY_BIN_ARGS
        -clang-tidy-binary ${CLANG_TIDY_BIN}
        -header-filter=.*
        -checks=clan*,cert*,misc*,perf*,cppc*,read*,mode*,-cert-err58-cpp,-misc-noexcept-move-constructor
        )

add_custom_target(
        tidy
        COMMAND ${RUN_CLANG_TIDY_BIN} ${RUN_CLANG_TIDY_BIN_ARGS}
        COMMENT "running clang tidy"
)


#cppcheck

list(APPEND CPPCHECK_ARGS
        --enable=warning,style,performance,portability,unusedFunction
        --std=c11
        --verbose
        --error-exitcode=1
        --language=c
        -DMAIN=main
        -I ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/*.c
        )

add_custom_target(
        check
        COMMAND cppcheck ${CPPCHECK_ARGS}
        COMMENT "running cppcheck"
)

#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -w")

set(CET_C_WARNINGS -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wdisabled-optimization -Wformat=2
        -Winit-self -Wlogical-op  -Wredundant-decls -Wshadow
        -Wundef -Werror -Wpointer-arith  -Wunreachable-code -Wunused)

add_definitions(${CET_C_WARNINGS})


if(ENABLE_ASAN)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fuse-ld=gold")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-omit-frame-pointer")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=leak")
endif()

if(ENABLE_USAN)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fuse-ld=gold")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=undefined")
endif()

if(ENABLE_TSAN)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fuse-ld=gold")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=thread")
endif()


add_executable(StaticAnalysisDemo main.c)

cmake_minimum_required(VERSION 3.10)

project(myAllocators)

set(CMAKE_CXX_STANDARD 20)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

#message("OS: ${CMAKE_SYSTEM_NAME}")
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations \
            -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported \
            -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security \
            -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual \
            -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo \
            -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods \
            -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef \
            -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers \
            -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation \
            -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -Wlarger-than=8192 \
            -Wstack-usage=8192 -pie -fPIE -Werror=vla -pthread\
            -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,\
integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,\
shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr")
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Wc++14-compat -Wmissing-declarations \
        -Wcast-align -Wcast-qual -Wchar-subscripts -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal \
        -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wnon-virtual-dtor \
        -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion \
        -Wsign-promo -Wstrict-overflow=2 -Wsuggest-override -Wswitch-default -Wswitch-enum -Wundef \
        -Wunreachable-code -Wunused -Wvariadic-macros -Wno-missing-field-initializers -Wno-narrowing \
        -Wno-old-style-cast -Wno-varargs -Wstack-protector -fsized-deallocation -fstack-protector \
        -fstrict-overflow -fno-omit-frame-pointer -Wlarger-than=8192 -fPIE -Werror=vla \
        -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,nonnull-attribute,null,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr")
else ()
    message("Unsupported OS: ${CMAKE_SYSTEM_NAME}")
endif ()

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -D_NDEBUG")

include(FetchContent)

FetchContent_Declare(
        io_utils
        GIT_REPOSITORY https://github.com/Neburalis/io_utils.git
        GIT_TAG main
)

FetchContent_MakeAvailable(io_utils)

add_library(${PROJECT_NAME} STATIC
        include/arena.h
        src/arena.cpp
)
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} PUBLIC io_utils)

add_executable(arena_example examples/arena_example.cpp)
target_link_libraries(arena_example ${PROJECT_NAME})

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
        target_link_options(arena_example PRIVATE
                -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,nonnull-attribute,null,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr
                -fPIE
        )
    else ()
        target_link_options(arena_example PRIVATE
                -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr
            -fPIE
        )
    endif ()
endif()

cmake_minimum_required(VERSION 3.6)

project(
	chip8-interpreter
	DESCRIPTION "A chip-8 interpreter."
	HOMEPAGE_URL "https://git.julianneadams.info/LeftySolara/chip8-interpreter"
	LANGUAGES C
	VERSION 0.1
)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "-Wall")
set(CMAKE_C_FLAGS_DEBUG "-g -O0")
set(CMAKE_C_FLAGS_RELEASE "-O2")

set(CMAKE_BUILD_TYPE Debug)

add_subdirectory(src)

# Recursively add source files to the build tree
file(GLOB_RECURSE SOURCE_FILES "src/*.c")
add_executable(chip8-interpreter ${SOURCE_FILES})

set_target_properties(chip8-interpreter PROPERTIES OUTPUT_NAME "chip8")

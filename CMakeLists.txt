cmake_minimum_required(VERSION 3.15.7)
project(kity C)

find_package(Curses REQUIRED)

if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Release")
endif() # CMAKE_BUILD_TYPE

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
	set(KITY_IS_TOP_LVL ON)
else()
	set(KITY_IS_TOP_LVL OFF)
endif() # KITY_IS_TOP_LVL

if (KITY_IS_TOP_LVL)
	set(KITY_DEFAULT_LIB_TYPE "EXEC")
else()
	set(KITY_DEFAULT_LIB_TYPE "STATIC")
endif() # KITY_DEFAULT_LIB_TYPE

set(KITY_LIB_TYPE "${KITY_DEFAULT_LIB_TYPE}" CACHE STRING "Library type.")

if (KITY_LIB_TYPE STREQUAL "SHARED")
	add_library(${PROJECT_NAME} SHARED)
	target_compile_definitions(${PROJECT_NAME} PUBLIC KITY_LIB_SHARED)
elseif (KITY_LIB_TYPE STREQUAL "EXEC")
	add_executable(${PROJECT_NAME}
		${CMAKE_CURRENT_SOURCE_DIR}/tests/test.c)
	target_compile_definitions(${PROJECT_NAME} PUBLIC KITY_LIB_EXEC)
else()
	add_library(${PROJECT_NAME} STATIC)
	target_compile_definitions(${PROJECT_NAME} PUBLIC KITY_LIB_STATIC)
endif() # KITY_LIB_TYPE

target_include_directories(${PROJECT_NAME} PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${CURSES_INCLUDE_DIRS})

target_sources(${PROJECT_NAME} PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/src/events.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/window.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/screen.c)

target_link_libraries(${PROJECT_NAME} PRIVATE ${CURSES_LIBRARIES})

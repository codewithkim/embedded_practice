cmake_minimum_required(VERSION 3.10)
project(EmbeddedSim)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Header path(include + FreeRTOS)
include_directories(
    include
    third_party/FreeRTOS/include
    third_party/FreeRTOS/portable/ThirdParty/GCC/Posix
)

# 2. Source files
file(GLOB_RECURSE USER_SOURCES "src/*.cpp")

# 3. Add FreeRTOS Source files
set(FREERTOS_SOURCES
    third_party/FreeRTOS/tasks.c
    third_party/FreeRTOS/queue.c
    third_party/FreeRTOS/list.c
    third_party/FreeRTOS/timers.c
    third_party/FreeRTOS/portable/MemMang/heap_4.c
    third_party/FreeRTOS/portable/ThirdParty/GCC/Posix/port.c
    third_party/FreeRTOS/portable/ThirdParty/GCC/Posix/utils/wait_for_event.c
)

# 4. Generate executable files
add_executable(app ${USER_SOURCES} ${FREERTOS_SOURCES})

# 5. Connect libraries (Linux / POSIX)
target_link_libraries(app pthread rt)


include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Create a test executable
# 1. Define the test executable
add_executable(run_tests 
    tests/processor_test.cpp 
    src/DataProcessor.cpp 
)

target_include_directories(run_tests PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/include   # If your headers are in an 'include' folder
    ${CMAKE_CURRENT_SOURCE_DIR}/tests     # For test-specific headers
)

target_link_libraries(run_tests GTest::gtest_main)
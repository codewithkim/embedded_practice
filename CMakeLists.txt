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
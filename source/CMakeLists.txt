set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include/)
set(SOURCE_DIR ${PROJECT_SOURCE_DIR}/source/)
set(HEADERS  ${INCLUDE_DIR}/bus.h ${INCLUDE_DIR}/cart.h ${INCLUDE_DIR}/common.h ${INCLUDE_DIR}/cpu.h ${INCLUDE_DIR}/cpu_fetch.h ${INCLUDE_DIR}/cpu_proc.h ${INCLUDE_DIR}/cpu_util.h ${INCLUDE_DIR}/dbg.h ${INCLUDE_DIR}/emu.h ${INCLUDE_DIR}/instructions.h ${INCLUDE_DIR}/io.h ${INCLUDE_DIR}/interrupts.h ${INCLUDE_DIR}/ppu.h ${INCLUDE_DIR}/ram.h ${INCLUDE_DIR}/stack.h ${INCLUDE_DIR}/timer.h ${INCLUDE_DIR}/ui.h)
set(SOURCES ${SOURCE_DIR}/bus.c cart.c cpu.c cpu_fetch.c cpu_proc.c cpu_util.c dbg.c emu.c io.c interrupts.c instructions.c ppu.c ram.c stack.c timer.c ui.c)

add_library(emu STATIC ${SOURCES} ${HEADERS})

target_include_directories(emu PUBLIC ${PROJECT_SOURCE_DIR}/include )

include_directories("/usr/local/include")
include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(emu ${SDL2_LIBRARIES})
target_link_libraries(emu ${SDL2_TTF_LIBRARIES})

set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include/)
set(SOURCE_DIR ${PROJECT_SOURCE_DIR}/source/)
set(HEADERS  ${INCLUDE_DIR}/bus.h ${INCLUDE_DIR}/cart.h ${INCLUDE_DIR}/common.h ${INCLUDE_DIR}/cpu.h ${INCLUDE_DIR}/cpu_fetch.h ${INCLUDE_DIR}/cpu_proc.h ${INCLUDE_DIR}/cpu_util.h ${INCLUDE_DIR}/emu.h ${INCLUDE_DIR}/instructions.h ${INCLUDE_DIR}/ppu.h ${INCLUDE_DIR}/ram.h ${INCLUDE_DIR}/timer.h )
set(SOURCES ${SOURCE_DIR}/bus.c cart.c cpu.c cpu_fetch.c cpu_proc.c cpu_util.c emu.c instructions.c ppu.c ram.c timer.c )

add_library(emu STATIC ${SOURCES} ${HEADERS})

target_include_directories(emu PUBLIC ${PROJECT_SOURCE_DIR}/include )

include_directories("/usr/local/include")
include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(emu ${SDL2_LIBRARIES})
target_link_libraries(emu ${SDL2_TTF_LIBRARIES})

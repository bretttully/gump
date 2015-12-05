file(GLOB src_files
    "*.h"
    "*.cc"
)

add_library(gtest SHARED ${src_files})
target_link_libraries(gtest ${CMAKE_THREAD_LIBS_INIT})
install(TARGETS gtest DESTINATION lib)

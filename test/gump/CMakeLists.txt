file(GLOB_RECURSE src_files
    "*.h"
    "*.cpp"
)
file(GLOB_RECURSE src_files_UNIT
    "*_unittest.cpp"
)
list(REMOVE_ITEM src_files ${src_files_UNIT})

add_library(gump-testing SHARED ${src_files})
target_link_libraries(gump-testing gump ${LOG4CXX_LIBS} ${GTEST_LIBS})
install(TARGETS gump-testing DESTINATION lib)

add_executable(gump-unittest ${src_files_UNIT})
target_link_libraries(gump-unittest gump-testing ${LOG4CXX_LIBS})
install(TARGETS gump-unittest DESTINATION bin)

file(GLOB_RECURSE src_files . FALSE
    "*.h"
    "*.hpp"
    "*.cpp"
)

# this is here to ensure that the IDE knows about this file
# and can then run the code formatter
list(APPEND src_files "${src_dir}/../uncrustify.cfg")

add_library(gump SHARED ${src_files})
target_link_libraries(gump ${BOOST_LIBS} ${LOG4CXX_LIBS} ${VTK_LIBS})
install(TARGETS gump DESTINATION lib)


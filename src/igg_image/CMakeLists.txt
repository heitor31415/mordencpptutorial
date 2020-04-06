#################### Import the 'io_tools' library here ########################
find_library(IO_TOOLS 
NAMES io_tools
PATHS ${LIBRARY_OUTPUT_PATH})


#################### Create a library 'image' below this line ##################
add_library(image SHARED image.cpp)
# add_executable(demo tools_demo.cpp)
# target_link_libraries(demo image ${IO_TOOLS})


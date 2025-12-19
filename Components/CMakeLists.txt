# project/Components/CMakeLists.txt

# Define an INTERFACE library for Components.
add_library(Components INTERFACE)

# Specify the include directories for the Components library.
target_include_directories(Components INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include/Components>
)

# Declare that Components needs Math.
# When any target links against 'Components', it will automatically get
# Math's INTERFACE properties (like its include directories).
target_link_libraries(Components INTERFACE Math)

# If you had source files (e.g., Gravity.cpp), you would add them here.
# For header-only, no sources are typically listed.
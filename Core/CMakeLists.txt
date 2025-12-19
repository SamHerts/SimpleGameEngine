# project/Components/CMakeLists.txt

# Define an INTERFACE library for Components.
add_library(Core INTERFACE)

# Specify the include directories for the Components library.
target_include_directories(Core INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include/Components>
)

#target_link_libraries(Core INTERFACE Core)

# If you had source files (e.g., Gravity.cpp), you would add them here.
# For header-only, no sources are typically listed.
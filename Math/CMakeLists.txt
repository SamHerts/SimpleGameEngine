# project/Math/CMakeLists.txt

# Define an INTERFACE library for Math.
# INTERFACE libraries are used for header-only libraries or to propagate properties.
add_library(Math INTERFACE)

# Specify the include directories for the Math library.
# The '.' means "the current directory" (which is 'project/Math').
# INTERFACE means this include path is propagated to any target that links against 'Math'.
target_include_directories(Math INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> # For when building the project
        $<INSTALL_INTERFACE:include/Math>              # For when installing the project
)

# If you had source files (e.g., Math.cpp), you would add them here:
# target_sources(Math INTERFACE Math.cpp) # Only if Math.cpp was also header-only
# If Math was a regular library (static/shared), you'd use:
# add_library(Math STATIC Math.cpp)
# target_include_directories(Math PUBLIC .)
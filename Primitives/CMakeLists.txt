# Define a static library called 'Primitives'
add_library(Primitives STATIC
        # Explicitly list all source files for the Primitives library
        Points.cpp
        Rectangle.cpp
)

# Make the headers in this directory available to anyone linking against Primitives
# (e.g., Objects library, or the main Game executable)
target_include_directories(Primitives PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR} # This makes 'Primitives/' part of the include path
        # for consumers of this library.
)

# If Primitives itself depends on other libraries or include paths, add them here.
# For example, if Primitives used a math library:
 target_link_libraries(Primitives PRIVATE SDL3_image::SDL3_image SDL3::SDL3)
cmake_minimum_required(VERSION 2.8)

# Firework simulation source
set(FIREWORKSSIM_VIEWER_SOURCE
    Firework.cpp
    Main.cpp
    ParticleEmitter.cpp
    Shader.cpp
    Utility.cpp

    ShadersTest.cpp
    stb_image.cpp
    TexturesTest.cpp
    TransformationsTest.cpp

    Shaders/Particle.frag
    Shaders/Particle.vert

    Textures/MyParticle.png
)

# Windows-only sources
#if(WIN32)
#list(APPEND CLOTHSIM_VIEWER_SOURCE
#    # For get-opt
#    misc/getopt.c
#)
#endif(WIN32)

#-------------------------------------------------------------------------------
# Embed resources
#-------------------------------------------------------------------------------

# embed_resources(CLOTHSIM_SHADERS
#   shaders/vert.glsl
#   shaders/normalFrag.glsl
#   shaders/wireframeFrag.glsl
# )

#-------------------------------------------------------------------------------
# Set definitions
#-------------------------------------------------------------------------------
add_definitions(${NANOGUI_EXTRA_DEFS})

#-------------------------------------------------------------------------------
# Set include directories
#-------------------------------------------------------------------------------
include_directories(
  ${CGL_INCLUDE_DIRS}
  ${FREETYPE_INCLUDE_DIRS}
  ${NANOGUI_EXTRA_INCS}
)

#-------------------------------------------------------------------------------
# Set link directories
#-------------------------------------------------------------------------------
link_directories(
  ${CGL_LIBRARY_DIRS}
  ${FREETYPE_LIBRARY_DIRS}
)

#-------------------------------------------------------------------------------
# Add executable
#-------------------------------------------------------------------------------
message(${FIREWORKSSIM_VIEWER_SOURCE})
add_executable(fireworkssim ${FIREWORKSSIM_VIEWER_SOURCE})

target_link_libraries(fireworkssim
    CGL ${CGL_LIBRARIES}
    nanogui ${NANOGUI_EXTRA_LIBS}
    ${FREETYPE_LIBRARIES}
    ${CMAKE_THREADS_INIT}
)

#-------------------------------------------------------------------------------
# Platform-specific configurations for target
#-------------------------------------------------------------------------------
if(APPLE)
  set_property( TARGET fireworkssim APPEND_STRING PROPERTY COMPILE_FLAGS
                "-Wno-deprecated-declarations -Wno-c++11-extensions")
endif(APPLE)

# Put executable in build directory root
set(EXECUTABLE_OUTPUT_PATH ..)

# Install to project root
install(TARGETS fireworkssim DESTINATION ${FireworksSim_SOURCE_DIR})

CMAKE_MINIMUM_REQUIRED(VERSION 2.6.3 FATAL_ERROR)

PROJECT (Assignment3)
INCLUDE_DIRECTORIES(libraries/include)
LINK_DIRECTORIES(libraries/lib)

ADD_EXECUTABLE(Assignment3 BOX.cpp Cubemap.cpp Frame.cpp FrameBuffer.cpp gui.cxx Light.cpp M33.cpp PPC.cpp Quad.cpp Scene.cpp SkyBox.cpp Sphere.cpp Texture.cpp TIFFImage.cpp TMesh.cpp Triangle.cpp V3.cpp)
TARGET_LINK_LIBRARIES(Assignment3 cg.lib cgGL.lib tiff.lib fltk.lib fltkgl.lib opengl32.lib glu32.lib wsock32.lib comctl32.lib libtiff.lib)

ADD_EXECUTABLE(Panorama M33.cpp Panorama.cpp PanoramaMain.cpp TIFFImage.cpp V3.cpp)
TARGET_LINK_LIBRARIES(Panorama cg.lib tiff.lib libtiff.lib)


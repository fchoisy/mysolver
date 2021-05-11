

include_directories(../include/tdogl)
include_directories(../include/stb_image)

add_library(tdogl 
            tdogl/Bitmap.cpp
            tdogl/Camera.cpp
            tdogl/Program.cpp
            tdogl/Shader.cpp
            tdogl/Texture.cpp
            )
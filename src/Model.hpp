#pragma once

#include <vector>
#include <GL/glew.h>
#include <tdogl/Program.h>


// struct Model
// {

//     // GLenum drawType;
//     // GLint drawStart;
// };

class Model
{
private:
    void LoadShaders(std::string vertexShaderFileName, std::string geometryShaderFileName, std::string fragmentShaderFileName);

public:
    Model(const std::vector<GLfloat> &vertexData, std::string vertexShaderFileName, std::string geometryShaderFileName, std::string fragmentShaderFileName, GLenum drawMode);
    ~Model();
    tdogl::Program *program;
    GLuint vao;
    GLuint vbo;
    GLint drawCount;
    GLenum drawMode;
};

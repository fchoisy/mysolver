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
    std::vector<GLfloat> vertexData;

public:
    static Model *Axes();
    static Model *Graph(const std::vector<GLfloat> &vertexData);
    static Model *ParticleVisualization(const std::vector<GLfloat> &vertexData);
    Model(const std::vector<GLfloat> &vertexData, std::string vertexShaderFileName, std::string geometryShaderFileName, std::string fragmentShaderFileName, GLenum drawMode);
    ~Model();
    void SetVertexData(const std::vector<GLfloat> &vertexData);
    tdogl::Program *program;
    GLuint vao;
    GLuint vbo;
    GLint drawCount;
    GLenum drawMode;
};

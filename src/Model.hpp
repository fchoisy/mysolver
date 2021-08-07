// Represents a graphical entity to be rendered.

#pragma once

#include <vector>
#include <GL/glew.h>
#include <tdogl/Program.h>

class Model
{

public:
    static Model *Axes();
    static Model *Graph(const std::vector<GLfloat> &vertexData);
    Model(std::string vertexShaderFileName, std::string geometryShaderFileName, std::string fragmentShaderFileName, GLenum drawMode);
    ~Model();
    virtual void Update(){};
    void SetVertexData(const std::vector<GLfloat> &vertexData);
    tdogl::Program *program;
    GLuint vao;
    GLuint vbo;
    GLint drawCount;
    GLenum drawMode;

private:
    // Copying disabled because disabled in tdogl::Program
    Model(const Model &);
    const Model &operator=(const Model &);
    void LoadShaders(std::string vertexShaderFileName, std::string geometryShaderFileName, std::string fragmentShaderFileName);
    std::vector<GLfloat> vertexData;
};


#pragma once

#include <GL/glew.h> // GLuint, GLint, GLenum types
#include <tdogl/Program.h>
#include <vector>

// Represents a graphical entity to be rendered.
class Model
{

public:
    Model(std::string vertexShaderFileName, std::string geometryShaderFileName, std::string fragmentShaderFileName, GLenum drawMode);
    ~Model();
    // Special models for plotting
    static Model *Graph(const std::vector<GLfloat> &vertexData);
    static Model *Axes();
    // Called on each render step
    virtual void Update(){};
    // Send new data to vertex buffer
    void SetVertexData(const std::vector<GLfloat> &vertexData);
    // Shaders
    tdogl::Program *program;
    // OpenGL buffers
    GLuint vao;
    GLuint vbo;
    GLint drawCount;
    GLenum drawMode;

private:
    // Copying disabled because disabled in tdogl::Program
    Model(const Model &);
    const Model &operator=(const Model &);
    // Compile and initialize OpenGL shaders.
    void LoadShaders(std::string vertexShaderFileName, std::string geometryShaderFileName, std::string fragmentShaderFileName);
    std::vector<GLfloat> vertexData;
};

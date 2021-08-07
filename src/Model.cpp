
#include <glm/gtc/matrix_transform.hpp> // Vector maths

#include "Model.hpp"
#include "helpers/RootDir.h"

Model::Model(std::string vertexShaderFileName, std::string geometryShaderFileName, std::string fragmentShaderFileName, GLenum drawMode)
    : drawMode(drawMode), drawCount(0)
{
    this->LoadShaders(vertexShaderFileName, geometryShaderFileName, fragmentShaderFileName);

    // Create VAO and VBO
    glGenVertexArrays(1, &(this->vao));
    glGenBuffers(1, &(this->vbo));
    // Bind VAO and VBO
    glBindVertexArray(this->vao);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    // Set corresponding shader attributes
    static const GLsizei stride = 6 * sizeof(GLfloat);
    static const GLvoid *offset = (const GLvoid *)(2 * sizeof(GLfloat));
    glEnableVertexAttribArray(this->program->attrib("vert"));
    glVertexAttribPointer(this->program->attrib("vert"), 2, GL_FLOAT, GL_FALSE, stride, NULL);
    glEnableVertexAttribArray(this->program->attrib("vertColor"));
    glVertexAttribPointer(this->program->attrib("vertColor"), 4, GL_FLOAT, GL_TRUE, stride, offset);
    // Unbind VAO and VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Model::~Model()
{
    if (this->program != NULL)
        delete this->program;
}

void Model::SetVertexData(const std::vector<GLfloat> &vertexData)
{
    this->vertexData = vertexData;
    this->drawCount = vertexData.size() / 6;
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    // Fill buffers with vertex data
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(GLfloat), vertexData.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Model::LoadShaders(std::string vertexShaderFileName, std::string geometryShaderFileName, std::string fragmentShaderFileName)
{
    std::vector<tdogl::Shader> shaders;
    shaders.push_back(tdogl::Shader::shaderFromFile(vertexShaderFileName, GL_VERTEX_SHADER));
    if (geometryShaderFileName != "")
        shaders.push_back(tdogl::Shader::shaderFromFile(geometryShaderFileName, GL_GEOMETRY_SHADER));
    shaders.push_back(tdogl::Shader::shaderFromFile(fragmentShaderFileName, GL_FRAGMENT_SHADER));
    this->program = new tdogl::Program(shaders);
}

Model *Model::Graph(const std::vector<GLfloat> &vertexData)
{
    Model *newModel = new Model(ROOT_DIR "resources/graph-shaders/vertex-shader.glsl",
                                "",
                                ROOT_DIR "resources/graph-shaders/fragment-shader.glsl",
                                GL_LINE_STRIP);
    newModel->SetVertexData(vertexData);
    return newModel;
}

Model *Model::Axes()
{
    Model *newModel = new Model(ROOT_DIR "resources/graph-shaders/vertex-shader.glsl",
                                "",
                                ROOT_DIR "resources/graph-shaders/fragment-shader.glsl",
                                GL_LINES);
    newModel->SetVertexData({
        -10.f,
        0.f,
        1,
        0,
        0,
        1,
        10.f,
        0.f,
        1,
        0,
        0,
        1,
        0.f,
        -10.f,
        0,
        1,
        0,
        1,
        0.f,
        10.f,
        0,
        1,
        0,
        1,
    });
    return newModel;
}

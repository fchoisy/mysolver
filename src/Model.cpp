
#include <glm/gtc/matrix_transform.hpp> // Vector maths

#include "Model.hpp"
#include "helpers/RootDir.h"

Model::Model(const std::vector<GLfloat> &vertexData, std::string vertexShaderFileName, std::string geometryShaderFileName, std::string fragmentShaderFileName)
{
    this->LoadShaders(vertexShaderFileName, geometryShaderFileName, fragmentShaderFileName);

    // Store number of vertices
    this->drawCount = vertexData.size();
    // Create VAO and VBO
    glGenVertexArrays(1, &(this->vao));
    glGenBuffers(1, &(this->vbo));
    // Bind them
    glBindVertexArray(this->vao);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    // Fill buffers with vertex data
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(GLfloat), vertexData.data(), GL_STATIC_DRAW);
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

void Model::LoadShaders(std::string vertexShaderFileName, std::string geometryShaderFileName, std::string fragmentShaderFileName)
{
    std::vector<tdogl::Shader> shaders;
    shaders.push_back(tdogl::Shader::shaderFromFile(vertexShaderFileName, GL_VERTEX_SHADER));
    shaders.push_back(tdogl::Shader::shaderFromFile(geometryShaderFileName, GL_GEOMETRY_SHADER));
    shaders.push_back(tdogl::Shader::shaderFromFile(fragmentShaderFileName, GL_FRAGMENT_SHADER));
    this->program = new tdogl::Program(shaders);

    this->program->use();
    // glm::mat4 projection = glm::perspective(glm::radians(50.0f), SCREEN_SIZE.x/SCREEN_SIZE.y, 0.1f, 10.0f);
    // GLfloat aspect = SCREEN_SIZE.x / SCREEN_SIZE.y;
    GLfloat aspect = 800.f / 600.f;
    glm::mat4 projection = glm::ortho(-aspect, aspect, -1.f, 1.0f);
    this->program->setUniform("projection", projection);

    glm::mat4 camera = glm::translate(glm::mat4(1.0f), glm::vec3(-1.f, -.5f, 0.f));
    this->program->setUniform("camera", camera);

    this->program->stopUsing();
}
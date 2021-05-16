#pragma once

#include <GL/glew.h> // Runtime loading of OpenGL API functions

struct Model
{
    GLuint vao;
    GLuint vbo;
    // GLenum drawType;
    // GLint drawStart;
    GLint drawCount;
};

class Graphics
{
private:
    void (*_OnInit)(Model **fluidModel);
    void (*_OnUpdate)();
    void (*_OnClose)();

public:
    Graphics(void (*OnInit)(Model **fluidModel), void (*OnUpdate)(), void (*OnClose)());
    ~Graphics();
    void run();
};

//----------------------------------------------------------------------------------------
/**
* \file    myObject.h
* \author  Radka Hoskova
* \date    2020
* \brief   Custom object loading.
*/
//----------------------------------------------------------------------------------------
#include "pgr.h"

const std::string vertexShaderSrc =
"#version 140\n"
"in vec2 position;\n"
"void main() {\n"
"  gl_Position = vec4(position, 0.0f, 1.0f);\n"
"}\n"
;

const std::string fragmentShaderSrc =
"#version 140\n"
"out vec4 color;"
"void main() {\n"
"  color = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
"}\n"
;
const std::string vertexShaderSrcBlack =
"#version 140\n"
"in vec2 position;\n"
"void main() {\n"
"  gl_Position = vec4(position, 0.0f, 1.0f);\n"
"}\n"
;

const std::string fragmentShaderSrcBlack =
"#version 140\n"
"out vec4 color;"
"void main() {\n"
"  color = vec4(0.0f, 0.0f, 0.0f, 1.0f);\n"
"}\n"
;

class MousePointer
{
public:
    GLuint shader;
    GLuint shaderBlack;
    GLuint arrayBuffer = 0;
    GLuint vao = 0;
    GLuint arrayBufferBlack = 0;
    GLuint vaoBlack = 0;

    void Init() {
        GLuint shaders[] = {
            pgr::createShaderFromSource(GL_VERTEX_SHADER, vertexShaderSrc),
            pgr::createShaderFromSource(GL_FRAGMENT_SHADER, fragmentShaderSrc),
            0
        };
        GLuint shadersBlack[] = {
            pgr::createShaderFromSource(GL_VERTEX_SHADER, vertexShaderSrcBlack),
            pgr::createShaderFromSource(GL_FRAGMENT_SHADER, fragmentShaderSrcBlack),
            0
        };
        shader = pgr::createProgram(shaders);
        shaderBlack = pgr::createProgram(shadersBlack);
        
        const float vertices[] = {
        0.0f,  0.02f,
        -0.02f, -0.02f,
        0.02f, -0.02f,
        };

        const float verticesBlack[] = {
        0.0f,  0.01f,
        -0.01f, -0.01f,
        0.01f, -0.01f,
        };

        glGenBuffers(1, &arrayBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, arrayBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        GLint positionLoc = glGetAttribLocation(shader, "position");
        glEnableVertexAttribArray(positionLoc);
        glVertexAttribPointer(positionLoc, 2, GL_FLOAT, GL_FALSE, 0, 0);

        glGenBuffers(1, &arrayBufferBlack);
        glBindBuffer(GL_ARRAY_BUFFER, arrayBufferBlack);
        glBufferData(GL_ARRAY_BUFFER, sizeof(verticesBlack), verticesBlack, GL_STATIC_DRAW);

        glGenVertexArrays(1, &vaoBlack);
        glBindVertexArray(vaoBlack);
        GLint positionLoc2 = glGetAttribLocation(shaderBlack, "position");
        glEnableVertexAttribArray(positionLoc2);
        glVertexAttribPointer(positionLoc2, 2, GL_FLOAT, GL_FALSE, 0, 0);
    };
    
    void Draw() {
        glUseProgram(shader);
        glBindVertexArray(vaoBlack);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(0);

        glUseProgram(shaderBlack);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(0);
        glBindVertexArray(0);

    };
};
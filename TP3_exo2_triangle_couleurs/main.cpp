#include "glimac/default_shader.hpp"
#include "p6/p6.h"

int main()
{
    auto ctx = p6::Context{{1280, 720, "TP3 EX1"}};
    ctx.maximize_window();

    /*VBO*/
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    GLfloat vertices[] = {
        -0.5f, -0.5f, 1.f, 0.f, 0.f,
        0.5f, -0.5f, 0.f, 1.f, 0.f,
        0.0f, 0.5f, 0.f, 0.f, 1.f};

    glBufferData(GL_ARRAY_BUFFER, 15 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    /*VAO*/
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    static constexpr GLuint vertex_attr_position = 3;
    glEnableVertexAttribArray(vertex_attr_position);

    static constexpr GLuint vertex_attr_color = 8;
    glEnableVertexAttribArray(vertex_attr_color);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glVertexAttribPointer(vertex_attr_position, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);

    glVertexAttribPointer(vertex_attr_color, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (const GLvoid*)(2 * sizeof(GLfloat)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    /*Loading Shader*/
    const p6::Shader shader = p6::load_shader("shaders/triangle.vs.glsl", "shaders/triangle.fs.glsl");

    // Declare your infinite update loop.
    ctx.update = [&]() {
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(vao);

        glimac::bind_default_shader();
        shader.use();

        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
    };
    glDeleteBuffers(0, &vbo);
    glDeleteVertexArrays(0, &vao);
    // Should be done last. It starts the infinite loop.
    ctx.start();
}
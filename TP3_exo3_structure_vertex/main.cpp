#include "cstddef"
#include "glimac/default_shader.hpp"
#include "glm/glm.hpp"
#include "p6/p6.h"

struct Vertex2DColor

{
    glm::vec2 position;
    glm::vec3 color;

    Vertex2DColor(){};
    Vertex2DColor(const glm::vec2 pos, const glm::vec3 col)
        : position(pos), color(col){};
};

int main()
{
    auto ctx = p6::Context{{1280, 720, "TP3 EX1"}};
    ctx.maximize_window();

    /*VBO*/
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    Vertex2DColor vertices[] = {

        Vertex2DColor(glm::vec2(-0.5, -0.5), glm::vec3(1, 0, 0)),

        Vertex2DColor(glm::vec2(0.5, -0.5), glm::vec3(0, 1, 0)),

        Vertex2DColor(glm::vec2(0, 0.5), glm::vec3(0, 0, 1)),

    };

    glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(Vertex2DColor), vertices, GL_STATIC_DRAW);
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

    glVertexAttribPointer(vertex_attr_position, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DColor), 0);

    glVertexAttribPointer(vertex_attr_color, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2DColor), (const GLvoid*)(offsetof(Vertex2DColor, color)));

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
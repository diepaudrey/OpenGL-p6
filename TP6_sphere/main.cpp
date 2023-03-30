#include "cstddef"
#include "glimac/common.hpp"
#include "glimac/default_shader.hpp"
#include "glimac/sphere_vertices.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "p6/p6.h"

int main()
{
    auto ctx = p6::Context{{1280, 720, "TP3 EX1"}};
    ctx.maximize_window();

    /*VBO*/
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    std::vector<glimac::ShapeVertex> vertices = glimac::sphere_vertices(1, 32, 16);

    // std::cout << "Vertices size = " << vertices.size() << std::endl;
    // std::cout << "Vertices data = " << vertices.data() << std::endl;

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glimac::ShapeVertex), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    /*VAO*/
    // GLuint vao;
    // glGenVertexArrays(1, &vao);
    // glBindVertexArray(vao);

    // static constexpr GLuint vertex_attr_position = 0;
    // glEnableVertexAttribArray(vertex_attr_position);

    // glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // glVertexAttribPointer(vertex_attr_position, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);

    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glBindVertexArray(0);

    GLuint vao;

    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);

    static constexpr GLuint VERTEX_ATTR_POSITION = 0;

    static constexpr GLuint VERTEX_ATTR_NORMAL = 1;

    static constexpr GLuint VERTEX_ATTR_TEXCOORDS = 2;

    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);

    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);

    glEnableVertexAttribArray(VERTEX_ATTR_TEXCOORDS);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, position)));

    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, normal)));

    glVertexAttribPointer(VERTEX_ATTR_TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, texCoords)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    /*Loading Shader*/
    const p6::Shader shader = p6::load_shader("shaders/3D.vs.glsl", "shaders/normals.fs.glsl");

    /*Location uniform variables*/
    GLint uMVPMatrix = glGetUniformLocation(shader.id(), "uMVPMatrix");

    GLint uMVMatrix = glGetUniformLocation(shader.id(), "uMVMatrix");

    GLint uNormalMatrix = glGetUniformLocation(shader.id(), "uNormalMatrix");

    glEnable(GL_DEPTH_TEST);

    // Declare your infinite update loop.
    ctx.update = [&]() {
        glm::mat4 ProjMatrix   = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);
        glm::mat4 MVMatrix     = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, -5.0f));
        glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

        glimac::bind_default_shader();
        shader.use();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));

        glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));

        glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

        glBindVertexArray(vao);

        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        glBindVertexArray(0);
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();

    glDeleteBuffers(0, &vbo);
    glDeleteVertexArrays(0, &vao);
}
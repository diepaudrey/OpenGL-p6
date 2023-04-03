

#include "cstddef"
#include "glimac/Freefly.hpp"
#include "glimac/Trackball.hpp"
#include "glimac/common.hpp"
#include "glimac/cone_vertices.hpp"
#include "glimac/default_shader.hpp"
#include "glimac/sphere_vertices.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "input.hpp"
#include "p6/p6.h"

void mouseEvent(p6::Context& ctx, glimac::FreeflyCamera& camera, const float& strength)
{
    glfwSetInputMode(ctx.underlying_glfw_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    ctx.mouse_moved = [&](p6::MouseMove data) {
        (camera).rotateLeft(data.delta.x * strength);
        (camera).rotateUp(-data.delta.y * strength);
    };
}

void keyboardEvent(p6::Context& ctx, glimac::FreeflyCamera& camera, const float& mvtStrength)
{
    ctx.key_repeated = [&](const p6::Key& data) {
        if (data.logical == "z" || data.logical == "Z")
        {
            camera.moveFront(ctx.delta_time() * mvtStrength);
        }

        if (data.logical == "s" || data.logical == "S")
        {
            camera.moveFront(-ctx.delta_time() * mvtStrength);
        }
        if (data.logical == "q" || data.logical == "Q")
        {
            camera.moveLeft(ctx.delta_time() * mvtStrength);
        }

        if (data.logical == "d" || data.logical == "D")
        {
            camera.moveLeft(-ctx.delta_time() * mvtStrength);
        }
    };
}

int main()
{
    auto ctx = p6::Context{{1280, 720, "TP3 EX1"}};
    ctx.maximize_window();

    /*Camera*/
    // glimac::TrackballCamera camera;
    glimac::FreeflyCamera camera;
    float                 movementStrength = 10.f;
    float                 rotationStrength = 900.f;

    /*Test Camera Third person*/

    // not working
    Input input(camera, movementStrength);

    mouseEvent(ctx, camera, rotationStrength);
    keyboardEvent(ctx, camera, movementStrength);

    // ctx.mouse_dragged = [&camera, &rotationStrength](p6::MouseDrag data) {
    //     data.delta = data.position - data.start_position;

    //     camera.rotateLeft(data.delta.x * rotationStrength);
    //     camera.rotateUp(data.delta.y * rotationStrength);
    // };

    // ctx.mouse_scrolled = [&camera](p6::MouseScroll data) {
    //     camera.moveFront(data.dy);
    // };

    // ctx.key_repeated = [&camera, &ctx, &movementStrength](const p6::Key& data) {
    //     if (data.logical == "z" || data.logical == "Z")
    //     {
    //         camera.moveFront(ctx.delta_time() * movementStrength);
    //     }

    //     if (data.logical == "s" || data.logical == "S")
    //     {
    //         camera.moveFront(-ctx.delta_time() * movementStrength);
    //     }

    //     if (data.logical == "q" || data.logical == "Q")
    //     {
    //         camera.moveLeft(ctx.delta_time() * movementStrength);
    //     }

    //     if (data.logical == "d" || data.logical == "D")
    //     {
    //         camera.moveLeft(-ctx.delta_time() * movementStrength);
    //     }
    // };

    /*VBO*/
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    std::vector<glimac::ShapeVertex> vertices = glimac::cone_vertices(1.f, 0.5f, 5, 5);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glimac::ShapeVertex), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    /*VAO*/
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    static constexpr GLuint VERTEX_ATTR_POSITION  = 0;
    static constexpr GLuint VERTEX_ATTR_NORMAL    = 1;
    static constexpr GLuint VERTEX_ATTR_TEXCOORDS = 2;
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
    glEnableVertexAttribArray(VERTEX_ATTR_TEXCOORDS);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)nullptr);

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
        /*Events*/

        glm::mat4 viewMatrix   = camera.getViewMatrix();
        glm::mat4 ProjMatrix   = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);
        glm::mat4 MVMatrix     = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, -5.0f));
        glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

        glimac::bind_default_shader();
        shader.use();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 MVPMatrix = ProjMatrix * viewMatrix * MVMatrix;
        glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(MVPMatrix));

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
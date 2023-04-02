#pragma once
#include "glimac/Trackball.hpp"
#include "p6/p6.h"

class Input {
private:
    glimac::TrackballCamera m_camera;
    float                   m_movementStrength;

public:
    Input(const glimac::TrackballCamera& cam, const float& mvtStrength)
        : m_camera(cam), m_movementStrength(mvtStrength){};

    void mouseHandler(p6::Context& ctx)
    {
        ctx.mouse_dragged = [&](p6::MouseDrag data) {
            data.delta = data.position - data.start_position;

            m_camera.rotateLeft(data.delta.x * m_movementStrength);
            m_camera.rotateUp(-data.delta.y * m_movementStrength);
        };

        ctx.mouse_scrolled = [&](p6::MouseScroll data) {
            m_camera.moveFront(data.dy);
        };
    }

    void keyboardHandler(p6::Context& ctx)
    {
        ctx.key_repeated = [&](const p6::Key& data) {
            if (data.logical == "z" || data.logical == "Z")
            {
                m_camera.moveFront(ctx.delta_time() * m_movementStrength);
            }

            if (data.logical == "s" || data.logical == "S")
            {
                m_camera.moveFront(-ctx.delta_time() * m_movementStrength);
            }
        };
    }
};
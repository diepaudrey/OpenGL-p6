#pragma once

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/glm.hpp"

namespace glimac {

class TrackballCamera {
private:
    float m_fDistance;
    float m_fAngleX;
    float m_fAngleY;

public:
    // constructeur
    TrackballCamera()
        : m_fDistance(5.f), m_fAngleX(0.f), m_fAngleY(0.f){};

    TrackballCamera(const float dist, const float angleX, const float angleY)
        : m_fDistance(dist), m_fAngleX(angleX), m_fAngleY(angleY){};

    // getter/setter
    float getDistance()
    {
        return this->m_fDistance;
    }
    float getAngleX()
    {
        return this->m_fAngleX;
    }
    float getAngleY()
    {
        return this->m_fAngleY;
    }
    void setDistance(const float dist)
    {
        this->m_fDistance = dist;
    }

    // movements
    void moveFront(const float delta)
    {
        this->m_fDistance += -delta;
    };

    void rotateLeft(const float degrees)
    {
        this->m_fAngleY += degrees;
    }

    void rotateUp(const float degrees)
    {
        this->m_fAngleX += degrees;
    }

    glm::mat4 getViewMatrix() const
    {
        glm::mat4 viewMatrix = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, -(this->m_fDistance)));
        viewMatrix           = glm::rotate(viewMatrix, glm::radians(this->m_fAngleX), glm::vec3(1.0f, 0.f, 0.f));
        viewMatrix           = glm::rotate(viewMatrix, glm::radians(this->m_fAngleY), glm::vec3(0.f, 1.0f, 0.f));

        return viewMatrix;
    }

    static void lookAround(const float yaw, const float pitch)
    {
        glm::vec3 direction;

        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    }
};

} // namespace glimac
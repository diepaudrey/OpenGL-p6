#pragma once

#include <iostream>
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/geometric.hpp"
#include "glm/glm.hpp"

namespace glimac {

class FreeflyCamera {
public:
    glm::vec3 m_position;
    float     m_phi;
    float     m_theta;
    glm::vec3 m_frontVector;
    glm::vec3 m_leftVector;
    glm::vec3 m_upVector;

    void computeDirectionVectors()
    {
        // m_frontVector
        m_frontVector.x = cos(glm::radians(m_phi)) * cos(glm::radians(m_theta));
        m_frontVector.y = sin(glm::radians(m_theta));
        m_frontVector.z = sin(glm::radians(m_phi)) * cos(glm::radians(m_theta));

        m_frontVector = glm::normalize(m_frontVector);

        // m_leftVector
        // m_leftVector.x = sin(glm::radians(m_phi + glm::half_pi<float>()));
        // m_leftVector.y = 0.f;
        // m_leftVector.z = cos(glm::radians(m_phi + glm::half_pi<float>()));

        // m_upVector
        // m_upVector = glm::cross(m_frontVector, m_leftVector);

        m_upVector.x = -cos(glm::radians(m_phi)) * sin(glm::radians(m_theta));
        m_upVector.y = cos(glm::radians(m_theta));
        m_upVector.z = -sin(glm::radians(m_phi)) * sin(glm::radians(m_theta));
        m_upVector   = glm::normalize(m_upVector);

        m_leftVector = glm::cross(m_frontVector, m_upVector);
        m_leftVector = glm::normalize(m_leftVector);
    }

public:
    FreeflyCamera()
        : m_position(glm::vec3(0.f, 0.f, -5.f)), m_phi(glm::pi<float>()), m_theta(0.f)
    {
        computeDirectionVectors();
    };

    void moveLeft(float t)
    {
        m_position -= t * m_leftVector;
        computeDirectionVectors();
    }

    void moveFront(float t)
    {
        m_position += t * m_frontVector;
        computeDirectionVectors();
    }

    void rotateLeft(float degrees)
    {
        m_phi += glm::radians(degrees);
        computeDirectionVectors();
    }

    void rotateUp(float degrees)
    {
        m_theta += -glm::radians(degrees);
        computeDirectionVectors();
    }

    glm::mat4 getViewMatrix() const
    {
        return glm::lookAt(m_position, m_position + m_frontVector, m_upVector);
    }
};

} // namespace glimac
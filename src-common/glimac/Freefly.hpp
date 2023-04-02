#pragma once

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/glm.hpp"

namespace glimac {

class FreeflyCamera {
private:
    glm::vec3 m_position;
    float     m_phi;
    float     m_theta;
    glm::vec3 m_frontVector;
    glm::vec3 m_leftVector;
    glm::vec3 m_upVector;

    void computeDirectionVectors()
    {
        // m_frontVector
        m_frontVector.x = sin(glm::radians(m_phi)) * cos(glm::radians(m_theta));
        m_frontVector.y = sin(glm::radians(m_theta));
        m_frontVector.z = cos(glm::radians(m_phi)) * cos(glm::radians(m_theta));

        // m_leftVector
        m_leftVector.x = sin(glm::radians(m_phi + glm::half_pi<float>()));
        m_leftVector.y = 0.f;
        m_leftVector.z = cos(glm::radians(m_phi + glm::half_pi<float>()));

        // m_upVector
        m_upVector = glm::cross(m_frontVector, m_leftVector);
    }

public:
    FreeflyCamera()
        : m_position(glm::vec3(0.f, 0.f, 0.f)), m_phi(glm::pi<float>()), m_theta(0.f)
    {
        computeDirectionVectors();
    };

    void moveLeft(float t)
    {
        m_position += t * m_leftVector;
    }

    void moveFront(float t)
    {
        m_position += t * m_frontVector;
    }

    void rotateLeft(float degrees)
    {
        m_phi += glm::radians(m_phi);
    }

    void rotateUp(float degrees)
    {
        m_theta += glm::radians(m_theta);
    }

    glm::mat4 getViewMatrix() const
    {
        return glm::lookAt(m_position, m_position + m_frontVector, m_upVector);
    }
};

} // namespace glimac
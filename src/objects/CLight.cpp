#include "objects/CLight.h"

CLight::CLight(glm::vec3 _direction, float _instansity, glm::vec3 _color)
{
    direction = _direction;
    instansity = _instansity;
    color = _color;

}

void CLight::setlightviewMatrix()
{
    SetLightPos();
    if (lightPos == sceneCenter) {
        lightPos += glm::vec3(0.01f, 0.01f, 0.01f);
    }
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 direction = glm::normalize(sceneCenter - lightPos);
    if (glm::abs(glm::dot(up, direction)) > 0.999f) {
        up = glm::vec3(0.0f, 0.0f, 1.0f);
    }
    lightProjectionMatrix = glm::lookAt(lightPos, sceneCenter, up);
}

glm::mat4 CLight::getlightviewMatrix()
{
    return lightProjectionMatrix;
}

void CLight::SetLightPos()
{
    distance =  5.0f;
    sceneCenter  = glm::vec3(0.0f,2.0f,0.0f);
    lightPos= sceneCenter - direction * distance;
}
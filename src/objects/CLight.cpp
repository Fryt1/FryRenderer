#include "objects/CLight.h"

CLight::CLight(glm::vec3 _direction, float _instansity, glm::vec3 _color)
{
    direction = _direction;
    instansity = _instansity;
    color = _color;

}

void CLight::setlightviewMatrix()
{
    lightProjectionMatrix = glm::lookAt(lightPos, sceneCenter, glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 CLight::getlightviewMatrix()
{
    return lightProjectionMatrix;
}
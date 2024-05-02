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
    lightviewMatrix = glm::lookAt(lightPos, sceneCenter, up);
}

glm::mat4 CLight::getlightviewMatrix()
{
    return lightviewMatrix;
}

void CLight::SetLightPos()
{
    distance =  3.0f;
    sceneCenter  = glm::vec3(0.0f,0.0f,0.0f);
    lightPos= sceneCenter - direction * distance;
}
#include "objects/CLight.h"

CLight::CLight(glm::vec3 _direction, float _instansity, glm::vec3 _color)
{
    direction = _direction;
    instansity = _instansity;
    color = _color;
}
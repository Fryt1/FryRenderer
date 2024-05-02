#include "camera/Camera.h"

glm::mat4 CCamera::GetProgectionMatrix(float fov, float aspect, float near, float far)
{
    return glm::perspective(fov, aspect, near, far);
}
glm::mat4 CCamera::GetViewMatrix()
{
    return glm::lookAt(Position, camesraTarget, upVector);
}
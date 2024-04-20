#ifndef CAMERA_H
#define CAMERA_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class CCamera{

public:
    glm::vec3 Position;
    glm::vec3 upVector;
    glm::vec3 camesraTarget;

    //初始化相机，设置位置，上方向，目标位置
    CCamera(glm::vec3 _position, glm::vec3 _upVector, glm::vec3 _cameraTarget)
    {
        Position = _position;
        upVector = _upVector;
        camesraTarget = _cameraTarget;
    }

    glm::mat4 GetViewMatrix();
    glm::mat4 GetProgectionMatrix(float fov, float aspect, float near, float far);

};



#endif
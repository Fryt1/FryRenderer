#ifndef LIGHT_H
#define LIGHT_H
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class CLight{
public:
    //光照方向
    glm::vec3 direction;
    //光照强度
    float instansity;
    //光照颜色
    glm::vec3 color;

    //光源距离
    float distance ;

    glm::vec3 sceneCenter ; // 场景中心位置

    // 计算虚拟光源位置
    glm::vec3 lightPos ;

    glm::mat4 lightviewMatrix;
    CLight()=default;
    //构造函数,初始化光照方向,强度,颜色
    CLight(glm::vec3 _direction, float _instansity, glm::vec3 _color);

    void setlightviewMatrix();
    glm::mat4 getlightviewMatrix();
    void SetLightPos();


};

#endif
#ifndef SHADOWSETTING_H
#define SHADOWSETTING_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "gl/Shader.h"

class ShadowSetting
{
public: 
    enum ShadowMapSize{
        SHADOWMAPSIZE_512 = 512,
        SHADOWMAPSIZE_1024 = 1024,
        SHADOWMAPSIZE_2048 = 2048,
        SHADOWMAPSIZE_4096 = 4096
    };

    ShadowMapSize shadowMapSize;

    ShadowSetting(ShadowMapSize _shadowMapSize = SHADOWMAPSIZE_1024);


    GLuint depthMapFBO;
    GLuint depthMap;

    Shader depthShader;

    glm::mat4 modelMatrix;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    

    void setShadowMapSize(ShadowMapSize shadowMapSize);
    
    void CreateShadowMapFB();


};


#endif
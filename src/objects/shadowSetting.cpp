#include "shadowSetting.h"

void ShadowSetting::setShadowMapSize(ShadowMapSize shadowMapSize)
{
    this->shadowMapSize = shadowMapSize;
}

void ShadowSetting::CreateShadowMapFB()
{

    glGenFramebuffers(1, &depthMapFBO);
    const GLuint SHADOW_WIDTH = shadowMapSize, SHADOW_HEIGHT = shadowMapSize;

    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 
                SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}



ShadowSetting::ShadowSetting(ShadowMapSize _shadowMapSize)
{
    this->shadowMapSize = _shadowMapSize;
    

}
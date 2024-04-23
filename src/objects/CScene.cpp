#include "objects/CScene.h"
#include "CScene.h"

void CScene::updateWindowSize(float Width, float Height)
{
    this->Width = Width;
    this->Height = Height;
}

CScene::CScene(float Width, float Height)
{
    this->Width = Width;
    this->Height = Height;

}
void CScene::AddModel(CModel model)
{
    models.push_back(model);
}

void CScene::AddCamera(CCamera camera)
{
    cameras.push_back(camera);
}

void CScene::AddLight(CLight light)
{
    lights.push_back(light);
}

void CScene::SetShadowSetting(ShadowSetting &_shadowMapSize)
{
    shadowSetting = _shadowMapSize;
    //shadowSetting.CreateShadowMapFB();
}

void CScene::setModelMatrix(glm::mat4 modelMatrix)
{
    this->modelMatrix = modelMatrix;
}
void CScene::setViewMatrix(glm::mat4 viewMatrix)
{
    this->viewMatrix = viewMatrix;
}

void CScene::setProjectionMatrix(glm::mat4 projectionMatrix)
{
    this->projectionMatrix = projectionMatrix;
}

void CScene::setModelToWorldNormalMatrix(glm::mat4 _m)
{
    glm::mat3 mat3 = glm::mat3(_m);
    uModelToWorldNormalMatrix = glm::determinant(mat3)*glm::inverse(mat3);
    uModelToWorldNormalMatrix = glm::transpose(uModelToWorldNormalMatrix);
}

void CScene::setModelMatrix_SM(glm::mat4 modelMatrix)
{
    this->modelMatrix_SM = modelMatrix;
}

void CScene::setViewMatrix_SM(glm::mat4 viewMatrix)
{
    this->viewMatrix_SM = viewMatrix;
}

void CScene::setProjectionMatrix_SM(glm::mat4 projectionMatrix)
{
    this->projectionMatrix_SM = projectionMatrix;
}
//设置相机位置
void CScene::setCameraPos(Shader _Modelshader)
{
    _Modelshader.setVec3("uCameraPos", cameras[0].Position);
}
//设置光源
void CScene::setupLight(Shader _Modelshader)
{
    _Modelshader.setVec3("uLightdirection", lights[0].direction);
    _Modelshader.setFloat("uLightIntensity", lights[0].instansity);

}



void CScene::drawScene(Shader _Modelshader,Shader _Modelshader_SM)
{
    drawModel_SM(_Modelshader_SM,modelMatrix_SM,viewMatrix_SM,projectionMatrix_SM);

    drawModel(_Modelshader,modelMatrix,viewMatrix,projectionMatrix);
}


void CScene::drawModel(Shader shader,glm::mat4 modelMatrix,glm::mat4 viewMatrix,glm::mat4 projectionMatrix)
{   
    shader.use();
    setCameraPos(shader);
    setupLight(shader);
    shader.setMat4("uModelMatrix", modelMatrix);
    shader.setMat4("uViewMatrix", viewMatrix);
    shader.setMat4("uProjectionMatrix", projectionMatrix);
    shader.setMat3("uModelToWorldNormalMatrix", uModelToWorldNormalMatrix);
    glm::mat4 LightSpaceMatrix = projectionMatrix_SM * viewMatrix_SM;
    shader.setMat4("uLightSpaceMatrix",  LightSpaceMatrix );
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,shadowSetting.depthMap); // 绑定shadowmap
    shader.setInt("uShadowMap", 0);


    for(unsigned int i = 0; i < models.size(); i++)
    {

        models[i].DrawModel(shader);
    }

}

void CScene::drawModel_SM(Shader shader, glm::mat4 modelMatrix, glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
    shader.use();
    shader.setMat4("uModelMatrix", modelMatrix_SM);
    shader.setMat4("uModelViewMatrix", viewMatrix_SM);
    shader.setMat4("uProjectionMatrix", projectionMatrix_SM);

    // 1. 首选渲染深度贴图
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, shadowSetting.shadowMapSize, shadowSetting.shadowMapSize);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowSetting.depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);

    for (unsigned int i = 0; i < models.size(); i++)
    {
        models[i].DrawModel_SM(shader);
    }
            // 生成mipmap
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glViewport(0, 0, Width, Height);



}

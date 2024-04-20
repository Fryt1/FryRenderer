#include "objects/CScene.h"
#include "CScene.h"


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

void CScene::setCameraPos(Shader _Modelshader)
{
    _Modelshader.setVec3("uCameraPos", cameras[0].Position);
}

void CScene::setupLight(Shader _Modelshader)
{
    _Modelshader.setVec3("uLightdirection", lights[0].direction);
    _Modelshader.setFloat("uLightIntensity", lights[0].instansity);

}



void CScene::drawScene(Shader _Modelshader)
{
    drawModel(_Modelshader,modelMatrix,viewMatrix,projectionMatrix);
}


void CScene::drawModel(Shader shader,glm::mat4 modelMatrix,glm::mat4 viewMatrix,glm::mat4 projectionMatrix)
{   
    
    setCameraPos(shader);
    setupLight(shader);
    shader.setMat4("uModelMatrix", modelMatrix);
    shader.setMat4("uViewMatrix", viewMatrix);
    shader.setMat4("uProjectionMatrix", projectionMatrix);
    shader.setMat3("uModelToWorldNormalMatrix", uModelToWorldNormalMatrix);

    for(unsigned int i = 0; i < models.size(); i++)
    {
        models[i].Draw(shader);
    }

}


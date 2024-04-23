#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "objects/CModel.h"
#include "camera/Camera.h"
#include "objects/CLight.h"  
#include "gl/Shader.h" 
#include "objects/shadowSetting.h" 

class CScene{

public:
    std::vector<CModel> models;
    std::vector<CCamera> cameras;
    std::vector<CLight> lights;

    ShadowSetting shadowSetting;

    float Width;
    float Height;

    void updateWindowSize(float Width,float Height);


    CScene()=default;
    CScene(float Width,float Height);



    glm::mat4 modelMatrix;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    glm::mat4 uModelToWorldNormalMatrix;

    glm::mat4 modelMatrix_SM;
    glm::mat4 viewMatrix_SM;
    glm::mat4 projectionMatrix_SM;


    void AddModel(CModel model);
    void AddCamera(CCamera camera);
    void AddLight(CLight light);

    void SetShadowSetting(ShadowSetting &_shadowMapSize);

    void setModelMatrix(glm::mat4 modelMatrix);
    void setViewMatrix(glm::mat4 viewMatrix);
    void setProjectionMatrix(glm::mat4 projectionMatrix);
    void setModelToWorldNormalMatrix(glm::mat4 _m);

    void setModelMatrix_SM(glm::mat4 modelMatrix);
    void setViewMatrix_SM(glm::mat4 viewMatrix);
    void setProjectionMatrix_SM(glm::mat4 projectionMatrix);

    void setCameraPos(Shader _Modelshader);

    void setupLight(Shader _Modelshader);

    void drawModel(Shader shader,glm::mat4 modelMatrix,glm::mat4 viewMatrix,glm::mat4 projectionMatrix);

    void drawModel_SM(Shader shader,glm::mat4 modelMatrix,glm::mat4 viewMatrix,glm::mat4 projectionMatrix);
    
    void drawScene(Shader _Modelshader,Shader _Modelshader_SM);


};



#endif
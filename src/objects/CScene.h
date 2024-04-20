#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "objects/CModel.h"
#include "camera/Camera.h"
#include "objects/CLight.h"  
#include "gl/Shader.h"  

class CScene{

public:
    std::vector<CModel> models;
    std::vector<CCamera> cameras;
    std::vector<CLight> lights;

    glm::mat4 modelMatrix;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    glm::mat4 uModelToWorldNormalMatrix;

    
    CScene()=default;

    void AddModel(CModel model);
    void AddCamera(CCamera camera);
    void AddLight(CLight light);

    void setModelMatrix(glm::mat4 modelMatrix);
    void setViewMatrix(glm::mat4 viewMatrix);
    void setProjectionMatrix(glm::mat4 projectionMatrix);
    void setModelToWorldNormalMatrix(glm::mat4 _m);

    void setCameraPos(Shader _Modelshader);

    void setupLight(Shader _Modelshader);

    void drawModel(Shader shader,glm::mat4 modelMatrix,glm::mat4 viewMatrix,glm::mat4 projectionMatrix);
    
    void drawScene(Shader _Modelshader);




    

};



#endif
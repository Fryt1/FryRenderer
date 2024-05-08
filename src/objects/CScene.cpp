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

void CScene::AddCamera(CCamera* camera)
{
    cameras.push_back(camera);
}

void CScene::AddLight(CLight light)
{
    lights.push_back(light);
}

void CScene::SetImage(CImage& image)
{
    images.push_back(image);
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
    _Modelshader.setVec3("uCameraPos", cameras[0]->Position);
}
//设置光源
void CScene::setupLight(Shader _Modelshader)
{
    _Modelshader.setVec3("uLightdirection", lights[0].direction);
    _Modelshader.setFloat("uLightIntensity", lights[0].instansity);

}

void CScene::IBLdrawScene(Shader _Modelshader,Shader _Modelshader_SM,Shader _CubeMapshader)
{
    drawCubeMap(_CubeMapshader);

    drawModel_SM(_Modelshader_SM,modelMatrix_SM,viewMatrix_SM,projectionMatrix_SM);

    drawModel(_Modelshader,modelMatrix,viewMatrix,projectionMatrix);
}

void CScene::drawCubeMap(Shader shader)
{
    shader.use();
    shader.setMat4("projection", projectionMatrix);
    shader.setMat4("view", viewMatrix);
    glBindTexture(GL_TEXTURE_CUBE_MAP,images[0].envCubemap);
    glDepthFunc(GL_LEQUAL);
    float vertices[] = {
        // 位置              // 纹理坐标
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    // 创建顶点缓冲对象（VBO）和顶点数组对象（VAO）
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // 绑定VAO和VBO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // 将顶点数据复制到VBO中
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 设置顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 解绑VBO和VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 

    // 在渲染循环中绘制立方体
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

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
    shader.setVec3("uLightColor", lights[0].color);
    

    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,shadowSetting.depthMap); // 绑定shadowmap
    shader.setInt("uShadowMap", 0);
    
    glActiveTexture(GL_TEXTURE1);

    glBindTexture(GL_TEXTURE_CUBE_MAP,images[0].irradianceMap);

    shader.setInt("irradianceMap",1);

    glActiveTexture(GL_TEXTURE2);

    glBindTexture(GL_TEXTURE_CUBE_MAP,images[0].prefilterMap);

    shader.setInt("prefilterMap",2);

    glActiveTexture(GL_TEXTURE3);

    glBindTexture(GL_TEXTURE_2D,images[0].brdfLUTTexture);

    shader.setInt("brdfLUTTexture",3);

    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);





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

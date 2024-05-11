#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "objects/CModel.h"
#include "gl/Shader.h"
#include "objects/CScene.h"
#include "camera/Camera.h"
#include "objects/CLight.h"
#include "renderdoc/renderdoc_app.h"
#include <minwindef.h>
#include <libloaderapi.h>
#include <gl/RenderStrategy.h>
#include <gl/DeferredRendering.h>
#include <map>


using namespace std;

int WIDTH =1240;
int HEIGHT = 960;

// 声明函数
void initShaderPathes(std::map<std::string,std::string>& ShaderPaths);
void initModelPathes(std::vector<std::string>& modelPaths);
void initScene(std::vector<std::string>& modelPaths, std::map<std::string,std::string>& ShaderPaths,CScene& scene, CCamera& camera, CLight& light, ShadowSetting& shadowSetting, CImage& cubemap, bool initShadow = true, bool initCubemap = true);

int main() {

    DeferredRendering renderStrategy(WIDTH,HEIGHT);
    
    if(renderStrategy.initOpenGl()==-1)
    {
        return -1;
    }

    GLFWwindow *window = renderStrategy.Window;


    //声明场景
    std::map<std::string,std::string> ShaderPaths;
    std::vector<std::string> modelPaths;
    CScene scene;
    CCamera camera;
    CLight light;
    ShadowSetting shadowSetting;
    CImage cubemap;
    
    //scene初始化
    initScene(modelPaths, ShaderPaths,scene, camera, light, shadowSetting, cubemap, true, true);
    
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));


    //配置渲染策略
    renderStrategy.initRenderStrategy(&scene,&camera,light,modelPaths,scaleMatrix,shadowSetting,cubemap,ShaderPaths);
    //渲染循环
    renderStrategy.Draw(window,&camera,ShaderPaths);

    glfwTerminate();
    return 0;
}

void initScene(std::vector<std::string>& modelPaths, std::map<std::string,std::string>& ShaderPaths,CScene& scene, CCamera& camera, CLight& light, ShadowSetting& shadowSetting, CImage& cubemap, bool initShadow , bool initCubemap ) {
    // 初始化模型路径
    initModelPathes(modelPaths);

    // 初始化shader路径
    initShaderPathes(ShaderPaths);

    // 初始化场景
    scene = CScene(WIDTH, HEIGHT);

    // 初始化相机
    glm::vec3 camera_Pos(0.0f, 0.0f, 50.0f);
    glm::vec3 camera_UpVector(0.0, 1.0, 0.0);
    glm::vec3 camera_cameraTarget(0.0f, 0.0f, 0.0);
    camera = CCamera(camera_Pos, camera_UpVector, camera_cameraTarget);

    // 初始化光源
    glm::vec3 light_Dir(0.0f, 0.0f, -1.0f);
    glm::vec3 light_Color(1.0f, 1.0f, 1.0f);
    float light_instansity = 1.0f;
    light = CLight(light_Dir, light_instansity, light_Color);

    // 初始化阴影设置
    if (initShadow) {
        shadowSetting.setShadowMapSize(ShadowSetting::SHADOWMAPSIZE_1024);
    }

    // Cubemap初始化
    if (initCubemap) {
        cubemap.load_HdrImage("assets/HDR/hdir/aircraft_workshop_01_4k.hdr");
    }
}

void initShaderPathes(std::map<std::string,std::string>& ShaderPaths){
    ShaderPaths["cubemaprenderShader"] = "src/shaders/cubemaprendershaders/";
    ShaderPaths["irradianceShader"] = "src/shaders/irradiancemap/";
    ShaderPaths["prefilterShader"] = "src/shaders/prefiltermap/";
    ShaderPaths["brdfShader"] = "src/shaders/BRDFLUT/";
    ShaderPaths["Iblshader"] = "src/shaders/Iblshaders/";
    ShaderPaths["depthShader"] = "src/shaders/shadowmapshaders/";
    ShaderPaths["screenShader"] = "src/shaders/screenShaders/";
    ShaderPaths["cubemapShader"] = "src/shaders/cubemapshaders/";
    ShaderPaths["gbuffershader"] = "src/shaders/gbuffershaders/";
    ShaderPaths["lightingPassShader"] = "src/shaders/lightingPassShaders/";
}

void initModelPathes(std::vector<std::string>& modelPaths){
    std::string mary = "assets/mary/Marry.obj";
    std::string plane = "assets/plane/plane.obj";
    std::string gun = "assets/ganyu.fbx";

    //modelPaths.push_back(mary);
    //modelPaths.push_back(plane);
    modelPaths.push_back(gun);
}
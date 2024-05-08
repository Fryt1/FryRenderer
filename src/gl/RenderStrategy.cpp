#include "gl/RenderStrategy.h"
#include "RenderStrategy.h"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


IBLRenderStrategy::IBLRenderStrategy()
{
    int WIDTH =1240;
    int HEIGHT = 960;
    float xoffset = 0;
    float yoffset = 0;
    float xoffset_last = 0;
    int isCameraRotate = 0;
    float lastX = WIDTH / 2.0f;
    float lastY = HEIGHT / 2.0f;
    bool firstMouse = true;
    double globalYOffset = 0.0;
}

IBLRenderStrategy::IBLRenderStrategy(int width, int height)
{
    WIDTH = width;
    HEIGHT = height;
    xoffset = 0;
    yoffset = 0;
    xoffset_last = 0;
    isCameraRotate = 0;
    lastX = width / 2.0f;
    lastY = height / 2.0f;
    firstMouse = true;
    globalYOffset = 0.0;
}

IBLRenderStrategy::~IBLRenderStrategy()
{
}

int IBLRenderStrategy::initOpenGl()
{



    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    Window = window;

    glfwMakeContextCurrent(Window);
    

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glfwSetWindowUserPointer(Window, this);

    glfwSetFramebufferSizeCallback(Window, IBLRenderStrategy::framebuffer_size_callback);
    glfwSetCursorPosCallback(Window, IBLRenderStrategy::mouse_callback);
    glfwSetScrollCallback(Window, IBLRenderStrategy::scroll_callback);


    
    glViewport(0, 0, WIDTH, HEIGHT);

    return 0;
    

}

void IBLRenderStrategy::initRenderStrategy(CScene *_scene,CCamera* Ccamera,CLight light,std::vector<std::string> _modelPathes,glm::mat4 scaleMatrix,
            ShadowSetting shadowSetting,CImage cubemap,std::map<std::string,std::string> ShaderPathes)
{
    scene = _scene;
    
    for(int i=0;i<_modelPathes.size();i++)
    {
        CModel model(_modelPathes[i]);
        scene->AddModel(model);
    }
    scene->setModelMatrix(scaleMatrix);
    scene->setProjectionMatrix(Ccamera->GetProgectionMatrix(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 350.0f));

    scene->setModelMatrix_SM(scaleMatrix);
    scene->setViewMatrix_SM(light.getlightviewMatrix());
    scene->setProjectionMatrix_SM(glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 1.0f, 350.0f));

    scene->SetShadowSetting(shadowSetting);
    scene->shadowSetting.CreateShadowMapFB();

    scene->AddCamera(Ccamera);
    scene->AddLight(light);
    scene->SetImage(cubemap);

    // Fix: Pass the correct arguments to the Shader constructor
    
    Shader cubemaprenderShader(ShaderPathes["cubemaprenderShader"]+"vertex.glsl", ShaderPathes["cubemaprenderShader"]+"fragment.glsl");
    Shader irradianceShader(ShaderPathes["irradianceShader"]+"vertex.glsl", ShaderPathes["irradianceShader"]+"fragment.glsl");
    Shader prefilterShader(ShaderPathes["prefilterShader"]+"vertex.glsl", ShaderPathes["prefilterShader"]+"fragment.glsl");
    Shader brdfShader(ShaderPathes["brdfShader"]+"vertex.glsl", ShaderPathes["brdfShader"]+"fragment.glsl");

    scene->images[0].renderCubeMap(cubemaprenderShader, irradianceShader, prefilterShader, brdfShader);


}

void IBLRenderStrategy::Draw(GLFWwindow *window,CCamera *Ccamera,std::map<std::string,std::string> ShaderPathes)
{
    glEnable(GL_DEPTH_TEST);

    glm::vec3 camera_Pos = Ccamera->Position;

    while (!glfwWindowShouldClose(window)) {
        this->processInput(window);

        scene->updateWindowSize(WIDTH,HEIGHT);

        scene->cameras[0]->Position = camera_Pos + glm::vec3(0.0f,0.0f,this->globalYOffset);

        scene->setViewMatrix(Ccamera->GetViewMatrix());

        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f),this->xoffset_last , glm::vec3(0.0f, 1.0f, 0.0f));

        glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));


        scene->setModelMatrix(rotationMatrix * scaleMatrix);
        scene->setModelMatrix_SM(rotationMatrix * scaleMatrix);

        scene->setModelToWorldNormalMatrix(scene->modelMatrix);

        Shader ourShader(ShaderPathes["ourShader"]+"vertex.glsl", ShaderPathes["ourShader"]+"fragment.glsl");
        Shader depthShader(ShaderPathes["depthShader"]+"vertex.glsl", ShaderPathes["depthShader"]+"fragment.glsl");
        Shader cubemapShaer(ShaderPathes["cubemapShader"]+"vertex.glsl",ShaderPathes["cubemapShader"]+"fragment.glsl");

        scene->IBLdrawScene(ourShader,depthShader,cubemapShaer);

        glfwSwapBuffers(window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwPollEvents();
    }

}


void IBLRenderStrategy::framebuffer_size_callback(GLFWwindow* window, int width, int height) {

        IBLRenderStrategy* strategy = static_cast<IBLRenderStrategy*>(glfwGetWindowUserPointer(window));
        strategy->framebuffer_size_callback_impl(window, width, height);    
}

void IBLRenderStrategy::framebuffer_size_callback_impl(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    WIDTH = width;
    HEIGHT = height;
}

void IBLRenderStrategy::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{

    IBLRenderStrategy* strategy = static_cast<IBLRenderStrategy*>(glfwGetWindowUserPointer(window));
    strategy->mouse_callback_impl(window, xpos, ypos);
}
void IBLRenderStrategy::mouse_callback_impl(GLFWwindow* window, double xposIn, double yposIn)
{

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }


    if(isCameraRotate ==1){

        xoffset = xpos - lastX;
        yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

        lastX = xpos;
        lastY = ypos;

        float sensitivity = 0.05f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        xoffset_last += xoffset;

    }

}
void IBLRenderStrategy::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    IBLRenderStrategy* strategy = static_cast<IBLRenderStrategy*>(glfwGetWindowUserPointer(window));
    strategy->scroll_callback_impl(window, xoffset, yoffset);
}
void IBLRenderStrategy::scroll_callback_impl(GLFWwindow* window, double xoffset, double yoffset) {
    globalYOffset -=yoffset*5.0f;
}

void IBLRenderStrategy::processInput(GLFWwindow *window)
{
    IBLRenderStrategy* strategy = static_cast<IBLRenderStrategy*>(glfwGetWindowUserPointer(window));
    strategy->processInput_impl(window);
}
void IBLRenderStrategy::processInput_impl(GLFWwindow *window)
{
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
            isCameraRotate = 1;
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
            isCameraRotate = 0;
}

void RenderStrategy::framebuffer_size_callback_impl(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    WIDTH = width;
    HEIGHT = height;
}


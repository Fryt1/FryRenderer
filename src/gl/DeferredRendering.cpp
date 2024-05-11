#include "gl/DeferredRendering.h"
#include "DeferredRendering.h"

DeferredRendering::DeferredRendering()
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

DeferredRendering::DeferredRendering(int width, int height)
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

DeferredRendering::~DeferredRendering()
{
}

int DeferredRendering::initOpenGl()
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

    glfwSetFramebufferSizeCallback(Window, DeferredRendering::framebuffer_size_callback);
    glfwSetCursorPosCallback(Window, DeferredRendering::mouse_callback);
    glfwSetScrollCallback(Window, DeferredRendering::scroll_callback);


    
    glViewport(0, 0, WIDTH, HEIGHT);

    return 0;
}

void DeferredRendering::setGBuffer()
{
    glGenFramebuffers(1, &gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
    

    // Albedo + Flages
    glGenTextures(1, &Albedo_Flages);
    glBindTexture(GL_TEXTURE_2D, Albedo_Flages);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Albedo_Flages, 0);

    // - 法线颜色缓冲
    glGenTextures(1, &Normal_Smoothness);
    glBindTexture(GL_TEXTURE_2D, Normal_Smoothness);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, Normal_Smoothness, 0);

    // - 颜色 + 镜面颜色缓冲
    glGenTextures(1, &Specular_Occlusion);
    glBindTexture(GL_TEXTURE_2D, Specular_Occlusion);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, Specular_Occlusion, 0);

    // - 告诉OpenGL我们将要使用(帧缓冲的)哪种颜色附件来进行渲染
    GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(3, attachments);
    //深度缓冲

    glGenTextures(1, &rboDepth);
    glBindTexture(GL_TEXTURE_2D, rboDepth);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, WIDTH, HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, rboDepth, 0);
    //检查帧缓冲是否完整
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;
}

void DeferredRendering::initRenderStrategy(CScene *_scene, CCamera *Ccamera, CLight light, std::vector<std::string> _modelPathes, glm::mat4 scaleMatrix, ShadowSetting shadowSetting, CImage cubemap, std::map<std::string, std::string> ShaderPathes)
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
    setGBuffer();
}

void DeferredRendering::Draw(GLFWwindow *window, CCamera *Ccamera, std::map<std::string, std::string> ShaderPathes)
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

        Shader shadowmapShader(ShaderPathes["depthShader"]+"vertex.glsl", ShaderPathes["depthShader"]+"fragment.glsl");


        scene->drawModel_SM(shadowmapShader, scene->modelMatrix_SM, scene->viewMatrix_SM, scene->projectionMatrix_SM);


        DeferredPassDraw(ShaderPathes);

        LightingPassDraw(Albedo_Flages, Specular_Occlusion, Normal_Smoothness,rboDepth,ShaderPathes);


        glfwSwapBuffers(window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwPollEvents();
    }

}

void DeferredRendering::DeferredPassDraw(std::map<std::string, std::string> ShaderPathes)
{
    //渲染gbuffer
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Shader gbuffershader(ShaderPathes["gbuffershader"]+"vertex.glsl", ShaderPathes["gbuffershader"]+"fragment.glsl");
    gbuffershader.use();
    scene->DeferredDrawModel(gbuffershader);


}

void DeferredRendering::LightingPassDraw(GLuint Albedo_Flages, GLuint Specular_Occlusion,GLuint Normal_Smoothness,GLuint rboDepth,std::map<std::string, std::string> ShaderPathes)
{
    //渲染光照  
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    Shader lightingShader(ShaderPathes["lightingPassShader"]+"vertex.glsl", ShaderPathes["lightingPassShader"]+"fragment.glsl");
    lightingShader.use();
    scene->LightingDrawModel(Albedo_Flages, Specular_Occlusion,Normal_Smoothness,rboDepth,lightingShader);

}

void DeferredRendering::framebuffer_size_callback(GLFWwindow* window, int width, int height) {

        DeferredRendering* strategy = static_cast<DeferredRendering*>(glfwGetWindowUserPointer(window));
        strategy->framebuffer_size_callback_impl(window, width, height);    
}

void DeferredRendering::framebuffer_size_callback_impl(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    WIDTH = width;
    HEIGHT = height;
}

void DeferredRendering::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{

    DeferredRendering* strategy = static_cast<DeferredRendering*>(glfwGetWindowUserPointer(window));
    strategy->mouse_callback_impl(window, xpos, ypos);
}
void DeferredRendering::mouse_callback_impl(GLFWwindow* window, double xposIn, double yposIn)
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
void DeferredRendering::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    DeferredRendering* strategy = static_cast<DeferredRendering*>(glfwGetWindowUserPointer(window));
    strategy->scroll_callback_impl(window, xoffset, yoffset);
}
void DeferredRendering::scroll_callback_impl(GLFWwindow* window, double xoffset, double yoffset) {
    globalYOffset -=yoffset*5.0f;
}

void DeferredRendering::processInput(GLFWwindow *window)
{
    DeferredRendering* strategy = static_cast<DeferredRendering*>(glfwGetWindowUserPointer(window));
    strategy->processInput_impl(window);
}
void DeferredRendering::processInput_impl(GLFWwindow *window)
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
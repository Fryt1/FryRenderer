#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "objects/CModel.h"
#include "gl/Shader.h"
#include "objects/CScene.h"
#include "camera/Camera.h"
#include "objects/CLight.h"

using namespace std;

int WIDTH =1240;
int HEIGHT = 960;

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);

float xoffset = 0;
float yoffset = 0;
float xoffset_last = 0;
int isCameraRotate = 0;
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;


int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);




    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, WIDTH, HEIGHT);






    Shader ourShader("src/shaders/phongshaders/vertex.glsl", "src/shaders/phongshaders/fragment.glsl");
    Shader depthShader("src/shaders/shadowmapshaders/vertex.glsl", "src/shaders/shadowmapshaders/fragment.glsl");
    Shader screenShader("src/shaders/screenShaders/vertex.glsl", "src/shaders/screenShaders/fragment.glsl");
   
    //初始化场景
    CScene scene(WIDTH,HEIGHT);

    //初始化相机
    glm::vec3 camera_Pos(3.0f, 10.0f, 8.0f);
    glm::vec3 camera_UpVector(0.0, 1.0, 0.0);
    glm::vec3 camera_cameraTarget(0.0, 2.0f, 0.0);
    CCamera Ccamera(camera_Pos, camera_UpVector,camera_cameraTarget);

    //导入model
    std::string marypath = "assets/mary/Marry.obj";
    CModel model_mary(marypath);

    std::string planePath = "assets/plane/plane.obj";
    CModel model_plane(planePath);

    //初始化光源
    glm::vec3 light_Dir(-4.0f, -2.0f, 1.0f);
    glm::vec3 light_Color(1.0f, 1.0f, 1.0f);
    float light_instansity = 1.0f;
    CLight light(light_Dir,light_instansity,light_Color);
    light.setlightviewMatrix();


    //初始化阴影设置
    ShadowSetting shadowSetting;
    shadowSetting.setShadowMapSize(ShadowSetting::SHADOWMAPSIZE_1024);

    
    
    //scene初始化
    scene.setModelMatrix(glm::mat4(1.0f));

    scene.setProjectionMatrix(Ccamera.GetProgectionMatrix(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f));



    scene.setModelMatrix_SM(glm::mat4(1.0f));
    scene.setViewMatrix_SM(light.getlightviewMatrix());
    scene.setProjectionMatrix_SM(glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 1.0f, 25.0f));

    scene.SetShadowSetting(shadowSetting);
    scene.shadowSetting.CreateShadowMapFB();



    
    scene.AddModel(model_mary);
    scene.AddModel(model_plane);
    scene.AddCamera(Ccamera);
    scene.AddLight(light);

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        scene.updateWindowSize(WIDTH,HEIGHT);

        scene.setViewMatrix(Ccamera.GetViewMatrix());
        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f),xoffset_last , glm::vec3(0.0f, 1.0f, 0.0f));

        scene.setModelMatrix(rotationMatrix * glm::mat4(1.0f));
        scene.setModelMatrix_SM(rotationMatrix * glm::mat4(1.0f));

        scene.setModelToWorldNormalMatrix(scene.modelMatrix);


        scene.drawScene(ourShader,depthShader);


        glfwSwapBuffers(window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    WIDTH = width;
    HEIGHT = height;
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
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

void processInput(GLFWwindow *window)
{





    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
            isCameraRotate = 1;
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
            isCameraRotate = 0;
 


}
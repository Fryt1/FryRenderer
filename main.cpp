#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "objects/CModel.h"
#include "gl/Shader.h"
#include "objects/CScene.h"
#include "camera/Camera.h"
#include "objects/CLight.h"

using namespace std;

#define WIDTH 800
#define HEIGHT 600

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

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

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, WIDTH, HEIGHT);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    Shader ourShader("src/shaders/phongshaders/vertex.glsl", "src/shaders/phongshaders/fragment.glsl");
    ourShader.use();
    //初始化场景
    CScene scene;

    //初始化相机
    glm::vec3 camera_Pos(2.0f, 2.0f, 7.0f);
    glm::vec3 camera_UpVector(0.0, 1.0, 0.0);
    glm::vec3 camera_cameraTarget(0.0, 2.0f, 0.0);
    CCamera Ccamera(camera_Pos, camera_UpVector,camera_cameraTarget);

    //导入model
    std::string path = "assets/mary/Marry.obj";
    CModel model(path);

    //初始化光源
    glm::vec3 light_Dir(0.0f, 0.0f, 1.0f);
    glm::vec3 light_Color(1.0f, 1.0f, 1.0f);
    float light_instansity = 1.0f;
    CLight light(light_Dir,light_instansity,light_Color);

    scene.setModelMatrix(glm::mat4(1.0f));

    scene.setProjectionMatrix(Ccamera.GetProgectionMatrix(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f));

    scene.setModelToWorldNormalMatrix(scene.modelMatrix);

    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window)) {

        scene.AddModel(model);
        scene.AddCamera(Ccamera);
        scene.AddLight(light);

        scene.setViewMatrix(Ccamera.GetViewMatrix());


        scene.drawScene(ourShader);


        glfwSwapBuffers(window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
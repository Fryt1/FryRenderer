#ifndef RENDERSTRATEGY_H
#define RENDERSTRATEGY_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "gl/Shader.h"
#include "objects/CModel.h"
#include "objects/CScene.h"
#include "camera/Camera.h"
#include "objects/CLight.h"
#include <map>

class RenderStrategy{
    public:
        int WIDTH ;
        int HEIGHT ;
        virtual int initOpenGl() = 0;
        virtual void Draw(GLFWwindow *window,CCamera *Ccamera,std::map<std::string,std::string> ShaderPathes) = 0;
        virtual void framebuffer_size_callback_impl(GLFWwindow* window, int width, int height)=0;
};

class IBLRenderStrategy : public RenderStrategy{
   
public:

        int WIDTH ;
        int HEIGHT ;
        float xoffset ;
        float yoffset ;
        float xoffset_last ;
        int isCameraRotate ;
        float lastX ;
        float lastY ;
        bool firstMouse ;

        double globalYOffset ;

        GLFWwindow *Window;
        CScene *scene;
        

        IBLRenderStrategy();
        IBLRenderStrategy(int width, int height);
        ~IBLRenderStrategy();
        int initOpenGl();
        void initRenderStrategy(CScene *_scene,CCamera* Ccamera,CLight light,std::vector<std::string> _modelPathes,glm::mat4 scaleMatrix,
            ShadowSetting shadowSetting,CImage cubemap,std::map<std::string,std::string> ShaderPathes);
        void Draw(GLFWwindow *window,CCamera *Ccamera,std::map<std::string,std::string> ShaderPathes);



        //回调函数
        static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
        void framebuffer_size_callback_impl(GLFWwindow* window, int width, int height);
        static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
        void mouse_callback_impl(GLFWwindow* window, double xposIn, double yposIn);
        static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
        void scroll_callback_impl(GLFWwindow* window, double xoffset, double yoffset);
        static void processInput(GLFWwindow *window);
        void processInput_impl(GLFWwindow *window);
        
};



#endif

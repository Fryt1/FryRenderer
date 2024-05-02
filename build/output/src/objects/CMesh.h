#ifndef CMESH_H
#define CMESH_H

#include <vector>
#include <string>

#include "glm/glm.hpp"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "gl/Shader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>



struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;  // 我们储存纹理的路径用于与其它纹理进行比较
};


class CMesh {
    public:
        /*  网格数据  */
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;
        /*  函数  */
        CMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
        void Draw(Shader shader);
        void Draw_SM(Shader shader);
    private:
        /*  渲染数据  */
        unsigned int VAO, VBO, EBO;
        //阴影渲染数据
        unsigned int VAO_SM, VBO_SM, EBO_SM;
        /*  函数  */
        void setupMesh();
        void setupMesh_SM();
};  

#endif
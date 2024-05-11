#ifndef MODEL_H
#define MODEL_H
#include <vector>
#include "objects/CMesh.h"
#include "gl/Shader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "stb/stb_image.h"

class CModel 
{
    public:
        //矩阵
        glm::mat4 modelMatrix;
        /*  函数   */
        CModel(std::string path)
        {
            loadModel(path);
        }
        void DrawModel(Shader shader,int texturenum);

        void DrawModel_SM(Shader shader);

        void setmodelMatrix(glm::mat4 modelMatrix);
 
    private:
        /*  模型数据  */
        std::vector<CMesh> meshes;
        std::string directory;
        std::vector<Texture> textures_loaded;
        /*  函数   */
        void loadModel(std::string path);
        void processNode(aiNode *node, const aiScene *scene);
        CMesh processMesh(aiMesh *mesh, const aiScene *scene);
        std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, 
                                             std::string typeName);
        unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false);
};


#endif
#include "objects/Cmesh.h"



CMesh::CMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    setupMesh();
    setupMesh_SM();

}



void CMesh::Draw(Shader shader,int texturenum)
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int emissiveNr = 1;
    unsigned int roughnessNr = 1;
    unsigned int metallicNr = 1;
    bool hasTexture = textures.size() > 0;
    shader.setBool("useTexture", hasTexture);
    for(unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i + texturenum); // 在绑定之前激活相应的纹理单元
        // 获取纹理序号（diffuse_textureN 中的 N）
        std::string number;
        std::string name = textures[i].type;
        if(name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if(name == "texture_specular")
            number = std::to_string(specularNr++);
        else if(name == "texture_normal")
            number = std::to_string(normalNr++);
        else if(name == "texture_emissive")
            number = std::to_string(emissiveNr++);
        else if(name == "texture_roughness")
            number = std::to_string(roughnessNr++);
        else if(name == "texture_metallic")
            number = std::to_string(metallicNr++);

        shader.setInt((name + number).c_str(), i+ texturenum);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);
    struct TextureType {
        std::string name;
        int count;
    };

    TextureType textureTypes[] = {
        {"useDiffuse", diffuseNr},
        {"useSpecular", specularNr},
        {"useNormal", normalNr},
        {"useEmissive", emissiveNr},
        {"useRoughness", roughnessNr},
        {"useMetallic", metallicNr}
    };

    for (TextureType& type : textureTypes) {
        shader.setBool(type.name, type.count > 1);
    }


    // 绘制网格
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
void CMesh::Draw_SM(Shader shader)
{
    // 绘制网格
    glBindVertexArray(VAO_SM);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void CMesh::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);  

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), 
                 &indices[0], GL_STATIC_DRAW);

    // 顶点位置
    glEnableVertexAttribArray(0);   
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // 顶点法线
    glEnableVertexAttribArray(1);   
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // 顶点纹理坐标
    glEnableVertexAttribArray(2);   
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    //顶点切线
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));


    glBindVertexArray(0);
}

void CMesh::setupMesh_SM()
{
    glGenVertexArrays(1, &VAO_SM);
    glGenBuffers(1, &VBO_SM);
    glGenBuffers(1, &EBO_SM);

    glBindVertexArray(VAO_SM);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_SM);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);  

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_SM);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), 
                 &indices[0], GL_STATIC_DRAW);

    // 顶点位置
    glEnableVertexAttribArray(0);   
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    glBindVertexArray(0);
}
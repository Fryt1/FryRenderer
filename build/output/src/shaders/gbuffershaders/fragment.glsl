#version 330 core
layout (location = 0) out vec4 Albedo_Flages;
layout (location = 1) out vec4 Normal_Smoothness;
layout (location = 2) out vec4 Specular_Occlusion;

in vec2 TexCoords;
in vec3 FragPos_WS;
in vec3 Normal;
in vec3 Tangent;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_height1;
uniform sampler2D texture_metallic1;
uniform sampler2D texture_roughness1;
uniform sampler2D texture_emission1;


void main()
{    
    // Specular_Occlusion 金属度和遮蔽
    float metallic = texture(texture_metallic1, TexCoords).r;
    Specular_Occlusion = vec4(metallic,metallic,metallic,1);
    // Normal_Smoothness 法线和光滑度 
    
    vec3 normal = texture(texture_normal1, TexCoords).rgb * 2-1;

    vec3 Bitangent = cross(Normal,Tangent);
    mat3 TBN = mat3(Tangent,Bitangent,Normal);
    normal = normalize(TBN * normal);
    float roughness = texture(texture_roughness1, TexCoords).r;
    Normal_Smoothness =vec4(normal,roughness); 
    // Albedo_Flages 漫反射和材质设置
    vec3 albedo = texture(texture_diffuse1, TexCoords).rgb;
    float depth = gl_FragCoord.z;
    
    Albedo_Flages = vec4(albedo,depth);
}  
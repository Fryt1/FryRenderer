#version 330 core
#ifdef GL_ES
precision mediump float;
#endif
uniform sampler2D texture_diffuse1;
uniform sampler2D uShadowMap;

uniform bool useTexture;
uniform vec3 uLightdirection;
uniform vec3 uCameraPos;
uniform float uLightIntensity;

uniform mat4 uLightSpaceMatrix;


in highp vec2 vTextureCoord;
in highp vec3 vFragPos_WS;
in highp vec3 vNormal_WS;

out vec4 FragColor;

#define POISSON_COUNT 32
#define POISSON_DISK vec2[POISSON_COUNT] \
    (vec2(-0.94201624, -0.39906216), vec2(0.94558609, -0.76890725), vec2(-0.094184101, -0.92938870), vec2(0.34495938, 0.29387760), \
    vec2(-0.91588581, 0.45771432), vec2(-0.81544232, -0.87912464), vec2(-0.38277543, 0.27676845), vec2(0.97484398, 0.75648379), \
    vec2(0.586331, -0.310612), vec2(-0.207107, -0.978156), vec2(-0.567129, 0.271440), vec2(-0.792657, -0.213023), \
    vec2(0.240993, 0.963973), vec2(-0.959492, 0.265046), vec2(0.396143, -0.841747), vec2(-0.136509, 0.975315), \
    vec2(0.941471, -0.337150), vec2(-0.651784, -0.758373), vec2(-0.396143, 0.841747), vec2(0.815442, 0.879124), \
    vec2(-0.941471, 0.337150), vec2(0.651784, 0.758373), vec2(0.959492, -0.265046), vec2(-0.240993, -0.963973), \
    vec2(-0.586331, 0.310612), vec2(0.207107, 0.978156), vec2(0.567129, -0.271440), vec2(0.792657, 0.213023), \
    vec2(-0.344959, -0.293877), vec2(0.382775, -0.276768), vec2(-0.974843, -0.756483), vec2(0.136509, -0.975315))



float poissonDiskSample(vec2 uv, float radius,float currentDepth) {
    vec2 poissonDisk[POISSON_COUNT] = POISSON_DISK;
    float depthSum = 0.0;
    for(int i = 0; i < POISSON_COUNT; ++i) {
        float depth = texture2D(uShadowMap, uv + radius * poissonDisk[i]).r;
        depthSum += currentDepth >  depth ? 1.0 : 0.0;
    }
    float averageDepth = depthSum / float(POISSON_COUNT);


    return averageDepth;
}




float CalcShadowFactor(vec3 _vFragPos_WS, vec3 normal, vec3 lightDir) {
  vec4 shadowCoord_H = uLightSpaceMatrix * vec4(_vFragPos_WS, 1.0);
  vec3 shadowCoord = shadowCoord_H.xyz / shadowCoord_H.w;
  shadowCoord = shadowCoord * 0.5 + 0.5;
  vec2 texelSize = 1.0 / textureSize(uShadowMap, 0);
  float bias = max(0.003 * (1.0 - dot(normal, lightDir)), 0.002);
  float currentDepth = shadowCoord.z -bias ;

  //计算平均深度遮挡物采样大小
  int maxSearchSize = 33;
  int minSearchSize = 3;
  float s = int (shadowCoord_H.z - 1)/(20 - 1) ;
  int blocksize = minSearchSize + int(s * (maxSearchSize - minSearchSize));

  //计算阴影的平均深度
  float AverageDepth =0;
  float Averagenum = 0;
  for(int i = -blocksize; i < blocksize; ++i)
  {
    for(int j = -blocksize; j < blocksize; ++j)
    {
      float Depth = texture(uShadowMap, shadowCoord.xy + vec2(i, j) * texelSize).r; 
      if(currentDepth > Depth)
      {
        AverageDepth += Depth;
        Averagenum += 1.0;
      }
    }
  }
  if (Averagenum == 0) {
    return 0;
  }
  AverageDepth /= Averagenum;


  float lightsize =2;
  float point2BlockerDistance = pow(length((currentDepth - AverageDepth) * 10.0), 2.0);

  float depthlod = 0.0;


  depthlod = point2BlockerDistance * lightsize / AverageDepth;
  



  float shadow = 0.0;
  
  shadow  = poissonDiskSample(shadowCoord.xy,  depthlod * texelSize.x, currentDepth);

  
  if(shadowCoord.z > 1.0)
    shadow = 0.0;
  return shadowCoord_H.w;
}

void main() {

vec3 color;
  if (useTexture) {
      color = pow(texture2D(texture_diffuse1, vTextureCoord).rgb, vec3(2.2));
  } else {
      color = vec3(0.7);
  }
  
  vec3 ambient = 0.05 * color;

  vec3 lightDir = -normalize(uLightdirection);
  vec3 normal = normalize(vNormal_WS);
  float shadow = CalcShadowFactor(vFragPos_WS, normal, lightDir);
  float diff = max(dot(lightDir, normal), 0.0);
  vec3 diffuse =  diff *  color * uLightIntensity;

  vec3 viewDir = normalize(uCameraPos - vFragPos_WS);
  float spec = 0.0;
  vec3 reflectDir = reflect(-lightDir, normal);
  spec = pow (max(dot(viewDir, reflectDir), 0.0), 35.0);
  vec3 specular =  0.25f * color * vec3(uLightIntensity * spec); 
  
  float shadowIntensity = 1; // 你可以调整这个值来改变阴影的深度
  FragColor = vec4((1-shadow),(1-shadow),(1-shadow),1.0);

}
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

float CalcShadowFactor(vec3 _vFragPos_WS, vec3 normal, vec3 lightDir) {
  vec4 shadowCoord_H = uLightSpaceMatrix * vec4(_vFragPos_WS, 1.0);
  vec3 shadowCoord = shadowCoord_H.xyz / shadowCoord_H.w;
  shadowCoord = shadowCoord * 0.5 + 0.5;
  float closestDepth = texture2D(uShadowMap, shadowCoord.xy).r;
  float currentDepth = shadowCoord.z;
  float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
  float shadow = currentDepth - bias  > closestDepth  ? 1.0 : 0.0;
  return shadow;
}

void main() {
vec3 color;
  if (useTexture) {
      color = pow(texture2D(texture_diffuse1, vTextureCoord).rgb, vec3(2.2));
  } else {
      color = vec3(0.25);
  }
  
  vec3 ambient = 0.05 * color;

  vec3 lightDir = normalize(uLightdirection);
  vec3 normal = normalize(vNormal_WS);
  float shadow = CalcShadowFactor(vFragPos_WS, normal, lightDir);
  float diff = max(dot(lightDir, normal), 0.0);
  vec3 diffuse =  diff *  color * uLightIntensity;

  vec3 viewDir = normalize(uCameraPos - vFragPos_WS);
  float spec = 0.0;
  vec3 reflectDir = reflect(-lightDir, normal);
  spec = pow (max(dot(viewDir, reflectDir), 0.0), 35.0);
  vec3 specular =  color * vec3(uLightIntensity * spec); 
  
  float shadowIntensity = 0.2; // 你可以调整这个值来改变阴影的深度
  FragColor = vec4(pow((ambient + diffuse + specular) * (1.0 - shadowIntensity * (1.0 - shadow)), vec3(1.0f/2.2f)), 1.0);
}


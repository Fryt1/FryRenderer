#version 330 core
#ifdef GL_ES
precision mediump float;
#endif
uniform sampler2D texture_diffuse1;
uniform bool useTexture;
uniform vec3 uLightdirection;
uniform vec3 uCameraPos;
uniform float uLightIntensity;


in highp vec2 vTextureCoord;
in highp vec3 vFragPos_WS;
in highp vec3 vNormal_WS;

out vec4 FragColor;

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
  float diff = max(dot(lightDir, normal), 0.0);
  vec3 diffuse =  diff *  color * uLightIntensity;

  vec3 viewDir = normalize(uCameraPos - vFragPos_WS);
  float spec = 0.0;
  vec3 reflectDir = reflect(-lightDir, normal);
  spec = pow (max(dot(viewDir, reflectDir), 0.0), 35.0);
  vec3 specular =  color * vec3(uLightIntensity * spec); 
  
  FragColor = vec4(pow(vec3(specular + ambient + diffuse),vec3(1.0/2.2)),1.0);
}


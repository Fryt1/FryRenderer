#version 330 core
in vec3 aVertexPosition;
in vec3 aNormalPosition;
in vec2 aTextureCoord;
in vec3 aTangent;

uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;
uniform mat4 uProjectionMatrix;
uniform mat3 uModelToWorldNormalMatrix;

out highp vec2 TexCoords;
out highp vec3 FragPos_WS;
out highp vec3 Normal;
out highp vec3 Tangent_WS;


void main() {

  FragPos_WS = (uModelMatrix * vec4(aVertexPosition, 1.0)).xyz;

  Normal = uModelToWorldNormalMatrix * aNormalPosition;
  Tangent_WS = uModelToWorldNormalMatrix * aTangent;
  
  TexCoords = aTextureCoord;

  gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix*vec4(aVertexPosition, 1.0);

}
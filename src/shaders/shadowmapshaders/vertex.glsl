#version 330 core
layout (location = 0) in vec3 aVertexPosition;

uniform mat4 uModelViewMatrix;
uniform mat4 uModelMatrix;
uniform mat4 uProjectionMatrix;

void main()
{
    gl_Position = uProjectionMatrix*uModelViewMatrix * uModelMatrix * vec4(aVertexPosition, 1.0f);
}
#version 430 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;
//layout(location = 3) in mat4 aInstanceMatrix;


out vec2 pass_textureCoordinates;
out vec3 surfaceNormal;
out vec3 toCameraVector;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 aInstanceMatrix;

void main(void)
{
    vec4 worldPosition = aInstanceMatrix * vec4(aPos,1.0);
    gl_Position = projection * view * worldPosition;
    pass_textureCoordinates = aTexCoord * 40.0;
    
    surfaceNormal = (aInstanceMatrix * vec4(aNormal,0.0)).xyz;
    toCameraVector = (inverse(view) * vec4(0.0,0.0,0.0,1.0)).xyz - worldPosition.xyz;
}
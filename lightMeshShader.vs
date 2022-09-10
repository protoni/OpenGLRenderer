#version 430 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;



out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

flat out highp int instanceID;

uniform mat4 transform;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
    instanceID = gl_InstanceID;
    Normal = aNormal;
    FragPos = vec3(model * vec4(aPos, 1.0));
}
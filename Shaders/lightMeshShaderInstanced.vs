#version 430 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in mat4 aInstanceMatrix;


out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

flat out highp int instanceID;


uniform mat4 transform;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;
uniform bool selectedInstance;

void main()
{
    Normal = mat3(transpose(inverse(aInstanceMatrix))) * aNormal;
    FragPos = vec3(aInstanceMatrix * vec4(aPos, 1.0));
    
    //if(selectedInstance) {
    //    gl_Position = projection * view * aInstanceMatrix * (vec4(FragPos, 1.0) + vec4(0.01, 0.01, 0.01, 0.01)); // fix Z-fighting by adding a gap between meshes
    //}
    //else {
    //    gl_Position = projection * view * aInstanceMatrix * vec4(FragPos, 1.0);
    //}
    
    if(selectedInstance) {
        gl_Position = projection * view * aInstanceMatrix * (vec4(aPos, 1.0) + vec4(0.01, 0.01, 0.01, 0.01));
    }
    else {
        gl_Position = projection * view * aInstanceMatrix * vec4(aPos, 1.0);
    }
    
    TexCoord = aTexCoord;
    instanceID = gl_InstanceID;
    //Normal = aNormal;
    //Normal = mat3(transpose(inverse(model))) * vec3(vec4(aNormal, 0.0) * model);
    
}
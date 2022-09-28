#version 430 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in mat4 aInstanceMatrix;
layout(location = 3) in highp int aSelected;

out vec2 TexCoord;
flat out highp int instanceID;
flat out highp int selectedMesh;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;
uniform bool selectedInstance;

void main()
{
    if(selectedInstance) {
        gl_Position = projection * view * aInstanceMatrix * (vec4(aPos, 1.0) + vec4(0.01, 0.01, 0.01, 0.01));
    }
    else {
        gl_Position = projection * view * aInstanceMatrix * vec4(aPos, 1.0);
    }
    TexCoord = aTexCoord;
    instanceID = gl_InstanceID;
    selectedMesh = aSelected;
}
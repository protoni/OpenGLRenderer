#version 330 core

out vec4 FragColor;
in vec2 TexCoord;
flat in highp int instanceID;

uniform float ratio;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec4 selectColor;
uniform int selectedMesh = -1;

void main()
{
    if(instanceID == selectedMesh) {
        FragColor = vec4(1.0, 1.0, 0.1, 1.0);
    }
    else {
        FragColor = vec4(0.1, 0.1, 0.1, 1.0);
    }
}
#version 430 core

layout(std430, binding = 3) buffer layoutName
{
    int data_SSBO[];
};

out vec4 FragColor;
in vec2 TexCoord;
flat in highp int instanceID;
flat in highp int selectedMesh;

uniform float ratio;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec4 selectColor;

uniform bool selectedInstance;

void main()
{
    
    bool selected = false;
    
    for(int i = 0; i < data_SSBO.length(); i++) {
        if(data_SSBO[i] == instanceID) {
            selected = true;
        }
    }
    
    if(selected) {
        FragColor = vec4(1.0, 1.0, 0.1, 1.0);
    }
    else {
        FragColor = vec4(0.1, 0.1, 0.1, 1.0);
    }
    
    
    
    
    
    
}
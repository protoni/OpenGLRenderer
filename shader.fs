#version 330 core

out vec4 FragColor;
in vec2 TexCoord;

uniform float ratio;
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    
    FragColor = vec4(0.1, 0.1, 0.1, 1.0);
}
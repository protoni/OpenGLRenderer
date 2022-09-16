

#version 430 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D theTexture;

void main()
{    
    FragColor = texture(theTexture, TexCoords);// + texture(texture_specular1, TexCoords));
}


#version 430 core

in vec2 pass_textureCoordinates;
in vec3 surfaceNormal;
in vec3 toCameraVector;

out vec4 out_Color;
uniform sampler2D modelTexture;

void main(void)
{
    vec3 unitNormal = normalize(surfaceNormal);
    
    
    out_Color = vec4(1.0, 1.0, 1.0, 1.0) * texture(modelTexture,pass_textureCoordinates);
}
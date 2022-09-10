#version 430 core

layout(std430, binding = 3) buffer layoutName
{
    int data_SSBO[];
};

out vec4 FragColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

flat in highp int instanceID;
flat in highp int selectedMesh;

uniform float ratio;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec4 selectColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;


uniform bool selectedInstance;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    float ambientStrength = 0.2;
    bool selected = false;
    
    for(int i = 0; i < data_SSBO.length(); i++) {
        if(data_SSBO[i] == instanceID) {
            selected = true;
        }
    }
    
    vec3 ambient = ambientStrength * lightColor;
    vec3 result = (ambient + diffuse) * objectColor;
    
    FragColor = vec4(result, 1.0);
    
    //if(selected) {
    //    FragColor = vec4(1.0, 1.0, 0.1, 1.0);
    //}
    //else {
    //    FragColor = vec4(0.1, 0.1, 0.1, 1.0);
    //}
    
    
    
    
    
    
}
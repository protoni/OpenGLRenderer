#version 430 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    //vec3 position;
    //vec3 direction;
    vec4 vector;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    // Fade-out
    float constant;
    float linear;
    float quadratic;
};

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
uniform vec3 viewPos;

uniform bool selectedInstance;

uniform Material material;
uniform Light light;

void main()
{
    float ambientStrength = 0.1;
    float specularStrength = 0.5;
    bool selected = false;


    // Use positional light by default
    vec3 lightDir = normalize(vec3(light.vector.xyz) - FragPos);


    // Calculate diffuse lighting
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));
    
    
    // Calculate ambient lighting
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
    
    
    // Calculate specular lighting
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));
    
    
    // Calculate spotlight fade-out values
    float distance = length(vec3(light.vector.xyz) - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
                        light.quadratic * (distance * distance));
    
    
    // Calculate directional light / positional based light
    if(light.vector.w == 0.0) {
        lightDir = normalize(vec3(light.vector.xyz));
    }
    else if (light.vector.w == 1.0){
        lightDir = normalize(vec3(light.vector.xyz) - FragPos);
        
        ambient *= attenuation;
        diffuse *= attenuation;
        specular *= attenuation;
    }
    
    
    for(int i = 0; i < data_SSBO.length(); i++) {
        if(data_SSBO[i] == instanceID) {
            selected = true;
        }
    }
    
    // Combine Phong lighting
    vec3 result = ambient + diffuse + specular;
    
    //FragColor = vec4(result, 1.0);
    
    if(selected) {
        FragColor = vec4(1.0, 1.0, 0.1, 1.0);
        //FragColor = vec4(result, 1.0) * vec4(1.0, 1.0, 0.1, 0.7);
    }
    else {
        FragColor = vec4(result, 1.0);
        //FragColor = vec4(0.1, 0.1, 0.1, 1.0);
    }
    
    
    
    
    
    
}
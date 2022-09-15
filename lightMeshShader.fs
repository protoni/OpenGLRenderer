#version 430 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct DirLight {
    vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    // Fade-out
    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
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
uniform PointLight light;
uniform SpotLight spotLight;
uniform DirLight dirLight;

#define NR_POINT_LIGHTS 40
uniform PointLight pointLights[NR_POINT_LIGHTS];

uniform int pointLightCount;
uniform int dirLightCount;
uniform int spotLightCount;

vec3 calculatePointLight(PointLight light, vec3 viewPos)
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    
    // Calculate diffuse lighting
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
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
                        light.quadratic * (distance * distance));

    // Apply attenuation
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    
    // Return combined light vector
    return (ambient + diffuse + specular);
}

vec3 calculateDirectionalLight(DirLight light, vec3 viewPos)
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.direction);
    
    // Calculate ambient lighting
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
    
    // Calculate diffuse value
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));
    
    // Calculate specular lighting
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));
    
    // Return combined light vector
    return (ambient + diffuse + specular);
}

vec3 calculateSpotLight(SpotLight light, vec3 viewPos)
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    
    // Calculate ambient lighting
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
    
    // Calculate diffuse value
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));
    
    // Calculate specular lighting
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));
    
    // Calculate spotlight fade-out values
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
                        light.quadratic * (distance * distance));
    
    // Calculate Spotlight area
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    
    // Apply attenuation and spot intensity
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    
    // Return combined light vector
    return (ambient + diffuse + specular);
}

void main()
{
    bool selected = false;
    float defaultAmbience = 0.05;
    
    // Calculate default ambient lighting
    vec3 ambient = defaultAmbience * vec3(texture(material.diffuse, TexCoord));
    
    for(int i = 0; i < data_SSBO.length(); i++) {
        if(data_SSBO[i] == instanceID) {
            selected = true;
        }
    }
    
    // Create default result color
    vec3 result = ambient;
    
    if(pointLightCount > 0) {
        for(int i = 0; i < pointLightCount; i++)
            result += calculatePointLight(pointLights[i], viewPos);
    }
    
    // Apply point lights
//    vec3 pointLightEffect = calculatePointLight(light, viewPos);
//    if(pointLightCount > 0)
//        result += calculatePointLight(light, viewPos);
//    
//    // Apply directional lights
//    vec3 dirLightEffect= calculateDirectionalLight(dirLight, viewPos);
//    if(pointLightEffect.x > 0.0 && pointLightEffect.y > 0.0 && pointLightEffect.z > 0.0)
//        result += dirLightEffect;
//    
//    // Apply spot lights
//    vec3 spotEffect = calculateSpotLight(spotLight, viewPos);
//    if(spotEffect.x > 0.0 && spotEffect.y > 0.0 && spotEffect.z > 0.0)
//        result += spotEffect;

    // If currently selected, set selected color, else set lights and material colors
    if(selected) {
        FragColor = vec4(1.0, 1.0, 0.1, 1.0);
        //FragColor = vec4(result, 1.0) * vec4(1.0, 1.0, 0.1, 0.7);
    }
    else {
        FragColor = vec4(result, 1.0);
        //FragColor = vec4(0.1, 0.1, 0.1, 1.0);
    }
}
#version 330 core

#define MAX_LIGHTS 100 

struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
}; 

struct DirectionalLight {
    Light base;
    vec3 direction;
}; 

struct PointLight {    
    Light base;
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  
};  

struct SpotLight
{
    PointLight base;
    vec3 axis;
    float cutOff;
};

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 
  
uniform int numDirectionalLights;
uniform int numSpotLights;
uniform int numPointLights;

uniform DirectionalLight directionalLights[MAX_LIGHTS];
uniform SpotLight spotLights[MAX_LIGHTS];
uniform PointLight pointLights[MAX_LIGHTS];
uniform vec3 cameraPos;
uniform sampler2D diffuseTex;
uniform Material material;

in Vertex {
	vec4 colour;
	vec2 texCoord;
	vec3 normal;
	vec3 worldPos;
} IN;

out vec4 fragColour;

void CalcLight(in Light light, in float diff, in float spec, out vec3 ambient, out vec3 diffuse, out vec3 specular) {
    ambient  = light.ambient * vec3(texture(diffuseTex , IN.texCoord )) * material.ambient;// * vec3(texture(material.ambient, IN.texCoord));
    diffuse  = light.diffuse * diff * vec3(texture(diffuseTex , IN.texCoord )) * material.diffuse;// * vec3(texture(material.diffuse, IN.texCoord));
    specular = light.specular * spec * vec3(texture(diffuseTex , IN.texCoord )) * material.specular;// * vec3(texture(material.specular, IN.texCoord));
}


vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    //vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess); //IMPORTANT REPLACE 60 with material.shininess
    
    // combine results
    vec3 ambient; vec3 diffuse; vec3 specular;
    CalcLight(light.base, diff, spec, ambient, diffuse, specular);
    return (ambient + diffuse + specular);
}  

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 worldPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - worldPos);
    
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    
    // specular shading
    //vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    
    // attenuation
    float distance    = length(light.position - worldPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    
    // combine results
    vec3 ambient; vec3 diffuse; vec3 specular;
    CalcLight(light.base, diff, spec, ambient, diffuse, specular);
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
} 

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 worldPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.base.position - worldPos);
    float spotAngle = max(dot(lightDir, light.axis), 0.0);

    if (spotAngle > radians(light.cutOff)) {
        vec3 color = CalcPointLight(light.base, normal, worldPos, viewDir);
        return color * (1.0 - (1.0 - spotAngle) * 1.0/(1.0 - radians(light.cutOff)));
    }
    else {
        return vec3(0,0,0);
    }
}

void main(void) {
    // properties
    vec3 norm = normalize(IN.normal);
    vec3 viewDir = normalize(cameraPos - IN.worldPos);

	// define an output color value
	vec3 result = vec3(0.0);

    // add the directional lights contribution to the output
    for(int i = 0; i < numDirectionalLights; i++) {
        result += CalcDirectionalLight(directionalLights[i], norm, viewDir);
    }

    // add the point lights contribution to the output
    for(int i = 0; i < numPointLights; i++) {
        result += CalcPointLight(pointLights[i], norm, IN.worldPos, viewDir);
    }

    // add the spot lights contribution to the output
    for(int i = 0; i < numSpotLights; i++) {
        result += CalcSpotLight(spotLights[i], norm, IN.worldPos, viewDir);
    }

	fragColour = vec4(result, 1.0);
    
    if (IN.worldPos.y == 0) {
        fragColour = vec4(1.0, 0.0, 0.0, 1.0); //pebbles
    }
//
//    if (IN.worldPos.y <= 100 && IN.worldPos.y > 0) {
//        fragColour = vec4(0.0, 1.0, 0.0, 1.0); //grassy rock
//    }
//
//    if (IN.worldPos.y <= 400 && IN.worldPos.y > 100) {
//        fragColour = vec4(0.0, 0.0, 1.0, 1.0); //grass
//    }
//
//    if (IN.worldPos.y <= 800 && IN.worldPos.y > 400) {
//        fragColour = vec4(1.0, 0.0, 1.0, 1.0); //grassy mountain
//    }
//    if (IN.worldPos.y <= 1000 && IN.worldPos.y > 800) {
//        fragColour = vec4(1.0, 1.0, 1.0, 1.0); //rocky mountain
//    }
//    if (IN.worldPos.y <= 1255 && IN.worldPos.y > 1000) {
//        fragColour = vec4(1.0, 1.0, 0.0, 1.0); //rocky mountain
//    }
}

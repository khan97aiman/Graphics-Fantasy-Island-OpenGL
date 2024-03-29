#version 330 core

#define MAX_LIGHTS 100 

struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float intensity;
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
    float cutOff; //cosine of the angle in radians
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
uniform Material material;

uniform sampler2D diffuseTex;

in Vertex {
	vec2 texCoord;
	vec3 normal;
	vec3 worldPos;
} IN ;

out vec4 fragColour;

void CalcLight(in Light light, in vec3 normal, in vec3 lightDir, in vec3 viewDir, out vec3 ambient, out vec3 diffuse, out vec3 specular) {

     // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 halfwayDir = normalize(lightDir + viewDir);

    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess); 

    ambient  = light.ambient * vec3(texture(diffuseTex , IN.texCoord )) * material.diffuse;
    diffuse  = light.diffuse * light.intensity * diff * vec3(texture(diffuseTex , IN.texCoord )) * material.diffuse;
    specular = light.specular * light.intensity *  spec * material.specular;
}


vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    
    // combine results
    vec3 ambient; vec3 diffuse; vec3 specular;
    CalcLight(light.base, normal, lightDir, viewDir, ambient, diffuse, specular);
    return (ambient + diffuse + specular);
}  

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 worldPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - worldPos);
    
    // attenuation
    float distance    = length(light.position - worldPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    
    // combine results
    vec3 ambient; vec3 diffuse; vec3 specular;
    CalcLight(light.base, normal, lightDir, viewDir, ambient, diffuse, specular);
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
} 

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 worldPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.base.position - worldPos);
    float spotAngle = max(dot(-lightDir, light.axis), 0.0);

    if (spotAngle > light.cutOff) {
        vec3 color = CalcPointLight(light.base, normal, worldPos, viewDir);
        return color * (1.0 - (1.0 - spotAngle) * 1.0/(1.0 - light.cutOff)); //multiplying with intensity
    }
    else {
        return vec3(0,0,0);
    }
}


void main(void) {

    if (texture(diffuseTex , IN.texCoord ).a == 0) {
        discard;
    }
    else {
     // properties
        vec3 norm = normalize(IN.normal);
        vec3 viewDir = normalize(cameraPos - IN.worldPos);

	    // define an output color value
	    vec3 result = vec3(0.0);

        // add the spot lights contribution to the output
        for(int i = 0; i < numSpotLights; i++) {
            result += CalcSpotLight(spotLights[i], norm, IN.worldPos, viewDir);
        }

        // add the directional lights contribution to the output
        for(int i = 0; i < numDirectionalLights; i++) {
            result += CalcDirectionalLight(directionalLights[i], norm, viewDir);
        }

        // add the point lights contribution to the output
        for(int i = 0; i < numPointLights; i++) {
            result += CalcPointLight(pointLights[i], norm, IN.worldPos, viewDir);
        }

	    fragColour = vec4(result, 1.0);
    }
}
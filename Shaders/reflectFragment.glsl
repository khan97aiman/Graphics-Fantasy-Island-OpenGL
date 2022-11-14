#version 330 core

uniform vec3 cameraPos;

uniform sampler2D waterTex;
uniform samplerCube cubeTex;

in Vertex {
	vec4 colour;
	vec2 texCoord;
	vec3 normal;
	vec3 worldPos;
} IN;

out vec4 fragColour;


void main(void) {
    float ratio = 1.00 / 1.52;
    vec3 viewDir = normalize(cameraPos - IN.worldPos);
    vec4 diffuse = texture(waterTex , IN.texCoord );
    
    vec3 reflectDir = reflect(-viewDir ,normalize(IN.normal ));
    vec4 reflectTex = texture(cubeTex, reflectDir );
	vec3 refractDir = refract(-viewDir, normalize(IN.normal), ratio);
	vec4 refractTex = texture(cubeTex, refractDir );

	
	fragColour = (reflectTex) + (diffuse * 0.25f);
	fragColour.a = 0.5;
}
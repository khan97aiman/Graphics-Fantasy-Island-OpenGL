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
    // properties
    vec3 viewDir = normalize(cameraPos - IN.worldPos);
    vec4 diffuse = texture(waterTex , IN.texCoord );
    
    vec3 reflectDir = reflect(-viewDir ,normalize(IN.normal ));
    vec4 reflectTex = texture(cubeTex ,reflectDir );
	
	fragColour = (reflectTex * 0.75f) + (diffuse * 0.25f);
	fragColour.a = 0.5;
}
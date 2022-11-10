# version 330 core

uniform mat4 modelMatrix ;
uniform mat4 viewMatrix ;
uniform mat4 projMatrix ;
uniform mat3 normalMatrix;

in vec3 position ;
in vec4 colour ;
in vec3 normal; 
in vec2 texCoord;

out Vertex {
	vec4 colour;
	vec2 texCoord;
	vec3 normal;
	vec3 worldPos;
} OUT ;

void main ( void ) {
	mat4 mvp = projMatrix * viewMatrix * modelMatrix ;
	gl_Position = mvp * vec4 ( position , 1.0);
	
	OUT.colour = colour;
	OUT.texCoord = texCoord;
	OUT.normal = normalize(normalMatrix * normalize(normal))
	OUT.worldPos = gl_Position.xyz;
}
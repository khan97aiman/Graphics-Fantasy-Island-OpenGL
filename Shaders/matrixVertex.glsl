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
	vec4 worldPos = (modelMatrix * vec4(position, 1.0));
	OUT.worldPos = worldPos.xyz;
	
	OUT.colour = colour;
	OUT.texCoord = texCoord;
	OUT.normal = normalize(normalMatrix * normalize(normal));

	gl_Position = (projMatrix * viewMatrix) * worldPos;
}
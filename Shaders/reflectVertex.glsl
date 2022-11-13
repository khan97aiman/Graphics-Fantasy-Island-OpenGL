# version 330 core

uniform mat4 modelMatrix ;
uniform mat4 viewMatrix ;
uniform mat4 projMatrix ;
uniform mat3 normalMatrix;
uniform mat4 textureMatrix;

in vec3 position ;
in vec3 normal; 
in vec2 texCoord;
in vec4 colour;

out Vertex {
	vec4 colour;
	vec2 texCoord;
	vec3 normal;
	vec3 worldPos;
} OUT ;

void main ( void ) {
	vec4 worldPos = modelMatrix * vec4(position, 1.0);
	OUT.worldPos = worldPos.xyz;
	
	OUT.texCoord = (textureMatrix * vec4(texCoord , 0.0, 1.0)). xy; 
	OUT.normal = normalize(normalMatrix * normalize(normal));
	OUT.colour = colour;

	gl_Position = (projMatrix * viewMatrix) * worldPos;
}
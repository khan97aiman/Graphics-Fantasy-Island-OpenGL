# version 330 core

uniform mat4 modelMatrix ;
uniform mat4 viewMatrix ;
uniform mat4 projMatrix ;
uniform mat3 normalMatrix;

in vec3 position ;
in vec3 normal; 
in vec2 texCoord;
in vec4 jointWeights;
in ivec4 jointIndices;

out Vertex {
	vec2 texCoord;
	vec3 normal;
	vec3 worldPos;
} OUT ;

void main ( void ) {	
	vec4 worldPos = modelMatrix * vec4(position , 1.0f);
	OUT.worldPos = worldPos.xyz;
	
	OUT.texCoord = texCoord;
	OUT.normal = normalize(normalMatrix * normalize(normal));

	gl_Position = (projMatrix * viewMatrix) * worldPos;
}

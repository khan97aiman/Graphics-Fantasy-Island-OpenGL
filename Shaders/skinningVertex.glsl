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

uniform mat4 joints [128];

out Vertex {
	vec2 texCoord;
	vec3 normal;
	vec3 worldPos;
} OUT ;

void main ( void ) {
	vec4 localPos = vec4(position , 1.0f);
	vec4 skelPos = vec4 (0,0,0,0);
	for(int i = 0; i < 4; ++i) {
		int jointIndex = jointIndices[i];
		float jointWeight = jointWeights[i];
		skelPos += joints[jointIndex] * localPos * jointWeight;
	}
	vec4 worldPos = modelMatrix * localPos;
	OUT.worldPos = worldPos.xyz;
	
	OUT.texCoord = texCoord;
	OUT.normal = normalize(normalMatrix * normalize(normal));

	gl_Position = (projMatrix * viewMatrix) * vec4(skelPos.xyz , 1.0);;
}

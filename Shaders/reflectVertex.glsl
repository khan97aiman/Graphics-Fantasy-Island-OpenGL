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
in vec4 tangent;

out Vertex {
	vec4 colour;
	vec2 texCoord;
	vec3 normal;
	vec3 worldPos;
	vec3 tangent; 
	vec3 binormal; 
} OUT ;

void main ( void ) {
	vec4 worldPos = modelMatrix * vec4(position, 1.0);
	OUT.worldPos = worldPos.xyz;
	
	OUT.texCoord = texCoord;
	vec3 wNormal = normalize(normalMatrix * normalize(normal ));
	vec3 wTangent = normalize(normalMatrix * normalize(tangent.xyz ));
	OUT.normal = wNormal;
	OUT.tangent = wTangent;
	OUT.binormal = cross(wTangent , wNormal) * tangent.w;
	OUT.colour = colour;

	gl_Position = (projMatrix * viewMatrix) * worldPos;
}
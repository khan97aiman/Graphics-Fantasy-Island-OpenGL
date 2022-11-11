#version 330 core

in Vertex {
	vec4 colour;
	vec2 texCoord;
	vec3 normal;
	vec3 worldPos;
} IN;

out vec4 fragColour;

void main(void) {
	fragColour = vec4( 1.0, 0, 0, 1 );
}

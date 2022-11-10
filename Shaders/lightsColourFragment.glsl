#version 330 core

in Vertex {
	vec4 colour;
	vec2 texCoord;
	vec3 normal;
	vec3 worldPos;
} IN;

out vec4 fragColour;

void main(void) {
	// define an output color value
	vec3 output = vec3(0.0);
  
	// add the directional light's contribution to the output
	output += someFunctionToCalculateDirectionalLight();
  
	// do the same for all point lights
	for(int i = 0; i < nr_of_point_lights; i++)
  	output += someFunctionToCalculatePointLight();
  // and add others lights as well (like spotlights)
  output += someFunctionToCalculateSpotLight();
	fragColour = vec4(output, 1.0);
}

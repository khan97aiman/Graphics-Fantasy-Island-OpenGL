#pragma once
#include "Light.h"
/*
* For more Information on Point Lights, read https://developer.valvesoftware.com/wiki/Constant-Linear-Quadratic_Falloff
* constant, linear and quadratic are light's attenuation proportions
*/
class PointLight : public Light {
public:
	PointLight() = default;
	PointLight(Vector3 position) : position(position) {}
	PointLight(Vector3 position, float constant, float linear, float quadratic) {
		this->position = position;
		this->constant = constant;
		this->linear = linear;
		this->quadratic = quadratic;
	}
	virtual void SendDataToShader(Shader* s, int index) {
		std::string i = std::to_string(index);
		glUniform3fv(glGetUniformLocation(s->GetProgram(), ("pointLights[" + i + "].position").c_str()), 1, (float*)&position);
		glUniform1f(glGetUniformLocation(s->GetProgram(), ("pointLights[" + i + "].constant").c_str()), constant);
		glUniform1f(glGetUniformLocation(s->GetProgram(), ("pointLights[" + i + "].linear").c_str()), linear);
		glUniform1f(glGetUniformLocation(s->GetProgram(), ("pointLights[" + i + "].quadratic").c_str()), quadratic);

		glUniform3fv(glGetUniformLocation(s->GetProgram(), ("pointLights[" + i + "].base.ambient").c_str()), 1, (float*)&ambient);
		glUniform3fv(glGetUniformLocation(s->GetProgram(), ("pointLights[" + i + "].base.diffuse").c_str()), 1, (float*)&diffuse);
		glUniform3fv(glGetUniformLocation(s->GetProgram(), ("pointLights[" + i + "].base.specular").c_str()), 1, (float*)&specular);
		glUniform1f(glGetUniformLocation(s->GetProgram(), ("pointLights[" + i + "].base.intensity").c_str()), intensity);
	}
protected:
	Vector3 position;
	float constant = 1.0f;			//attenuation unaffected by distance
	float linear = 0.001f;			//attenuation is inversely proportional to distance
	float quadratic = 0.000001f;	//attenuation is inversely proportional to square of distance
};


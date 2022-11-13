#pragma once
#include "Light.h"
class DirectionalLight : public Light {
public:
	DirectionalLight() = default;
	DirectionalLight(Vector3 direction) : Light(), direction(direction) {}
	void SendDataToShader(Shader* s, int index) {
		std::string i = std::to_string(index);
		glUniform3fv(glGetUniformLocation(s->GetProgram(), ("directionalLights[" + i + "].direction").c_str()), 1, (float*)&direction);
		glUniform3fv(glGetUniformLocation(s->GetProgram(), ("directionalLights[" + i + "].base.ambient").c_str()), 1, (float*)&ambient);
		glUniform3fv(glGetUniformLocation(s->GetProgram(), ("directionalLights[" + i + "].base.diffuse").c_str()), 1, (float*)&diffuse);
		glUniform3fv(glGetUniformLocation(s->GetProgram(), ("directionalLights[" + i + "].base.specular").c_str()), 1, (float*)&specular);
		glUniform1f(glGetUniformLocation(s->GetProgram(), ("directionalLights[" + i + "].base.intensity").c_str()), intensity);
	}
protected:
	Vector3 direction;
};


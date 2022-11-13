#pragma once
#include "PointLight.h"

class SpotLight : public PointLight {
public:
	SpotLight() = default;
	SpotLight(Vector3 position, Vector3 axis, float cutOff) : PointLight(position) {
		this->axis = axis;
		this->cutOff = cutOff;
	}
	SpotLight(Vector3 position) : PointLight(position) {}
	SpotLight(Vector3 position, float constant, float linear, float quadratic) : PointLight(position, constant, linear, quadratic) {}
	void SendDataToShader(Shader* s, int index) {
		std::string i = std::to_string(index);
		glUniform3fv(glGetUniformLocation(s->GetProgram(), ("spotLights[" + i + "].axis").c_str()), 1, (float*)&axis);
		glUniform1f(glGetUniformLocation(s->GetProgram(), ("spotLights[" + i + "].cutOff").c_str()), cutOff);

		glUniform3fv(glGetUniformLocation(s->GetProgram(), ("spotLights[" + i + "].base.position").c_str()), 1, (float*)&position);
		glUniform1f(glGetUniformLocation(s->GetProgram(), ("spotLights[" + i + "].base.constant").c_str()), constant);
		glUniform1f(glGetUniformLocation(s->GetProgram(), ("spotLights[" + i + "].base.linear").c_str()), linear);
		glUniform1f(glGetUniformLocation(s->GetProgram(), ("spotLights[" + i + "].base.quadratic").c_str()), quadratic);

		glUniform3fv(glGetUniformLocation(s->GetProgram(), ("spotLights[" + i + "].base.base.ambient").c_str()), 1, (float*)&ambient);
		glUniform3fv(glGetUniformLocation(s->GetProgram(), ("spotLights[" + i + "].base.base.diffuse").c_str()), 1, (float*)&diffuse);
		glUniform3fv(glGetUniformLocation(s->GetProgram(), ("spotLights[" + i + "].base.base.specular").c_str()), 1, (float*)&specular);
		glUniform1f(glGetUniformLocation(s->GetProgram(), ("spotLights[" + i + "].base.base.intensity").c_str()), intensity);
	}
protected:
	Vector3 axis = Vector3(0.0f, 1.0f, 0.0f);	// this is the axis of the spotlight, points in this direction receives maximum illumination
	float cutOff = 45.0f;						// cut off angle in degrees, fragment shader will convert this angle to radians
};


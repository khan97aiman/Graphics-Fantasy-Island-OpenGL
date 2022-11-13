#pragma once
#include "Vector3.h"

/* 
* Material is a mesh attribute, not a vertex attribute
* Ambient is is the color of the object in the presence of ambient light. By default, the ambient color will be the same as the diffuse value, unless otherwise specified.
* Diffuse is the primary color of an object as it would appear in pure white light
* Specular is the "highlight" color of a material. It can be thought of as the reflection of the light source itself on the surface of an object.
* The shininess parameter controls the size of the highlight: larger numbers produce smaller highlights, which makes the object appear shinier. e.g. 32, 64, 128
* When an Emission color, the object will take on that color independent of any lights, including ambient, as though it is glowing (although it will not illuminate neighboring objects,
  as it is not a true light source). By default, it is set to emission : 0
*/


class Material {
public:
    Material() = default;
    Material(Vector3 diffuse) {
        this->diffuse = diffuse;
        ambient = diffuse;
    }
    Material(Vector3 diffuse, Vector3 specular, float shininess, Vector3 emission) {
        this->ambient = diffuse;
        this->diffuse = diffuse;
        this->specular = specular;
        this->shininess = shininess;
        this->emission = emission;
    }
    Material(Vector3 diffuse, Vector3 specular, float shininess) {
        this->ambient = diffuse;
        this->diffuse = diffuse;
        this->specular = specular;
        this->shininess = shininess;
    }
    void SendDataToShader(Shader* s) {

        glUniform3fv(glGetUniformLocation(s->GetProgram(), "material.ambient"), 1, (float*)&ambient);
        glUniform3fv(glGetUniformLocation(s->GetProgram(), "material.diffuse"), 1, (float*)&diffuse);
        glUniform3fv(glGetUniformLocation(s->GetProgram(), "material.specular"), 1, (float*)&specular);
        glUniform1f(glGetUniformLocation(s->GetProgram(), "material.shininess"), shininess);
        glUniform3fv(glGetUniformLocation(s->GetProgram(), "material.specular"), 1, (float*)&emission);

    }
protected:
    Vector3 ambient;
    Vector3 diffuse;
    Vector3 specular = Vector3(1.0f);
    float shininess = 32;
    Vector3 emission = Vector3(0.0f);
};


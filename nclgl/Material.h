#pragma once
#include "Vector3.h"

//material is a mesh attribute, not a vertex attribute
struct Material {
    Vector3 ambient; //This is is the color of the object in the presence of ambient light. By default, the ambient color will be the same as the diffuse value, unless otherwise specified.
    Vector3 diffuse; //This is the primary color of an object as it would appear in pure white light
    Vector3 specular; //In our lighting model, "specular" is the "highlight" color of a material. It can be thought of as the reflection of the light source itself on the surface of an object.
    float shininess; //The shininess parameter controls the size of the highlight: larger numbers produce smaller highlights, which makes the object appear shinier.
    //By default, these are set to specular: 0 and shininess: 0.2
    Vector3 emission; //When an emission color is set, the object will take on that color independent of any lights, including ambient, as though it is glowing(although it will not illuminate neighboring objects, 
    //as it is not a true light source).By default, it is set to emission : 0
};


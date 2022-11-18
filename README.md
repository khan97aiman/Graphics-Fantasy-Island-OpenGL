# Coursework
Controls:
1. Press **W, A, S, D, LShift and Space**: control camera movement
2. Press **1**: Toggle Day/Night
3. Press **2**: Toggle Camera Animation

# Features
1. Terrain
2. Water
3. Textured Meshes
4. Lights
5. Scene Graph
6. Camera
7. Materials
8. Skybox

## Terrain
Following are the features implemented on terrain:
1. Bump Mapping
2. Sampling using multiple textures to color the terrain (mud texture for ground level, grass texture for mid level and rock texture for highest mountain level)
3. Smooth transition between textures using interpolated height of the terrain as the mix factor between two textures (see lines `208` and `211` in `lightsColourFragment.glsl`)
3. Solved the problem of uniform tiling repeation in textures (see `textureNoTile(sampler2D, in vec2)` on line `133` in `lightsColourFragment.glsl`)
4. Contribution of multiple lights sources to produce the final color

## Water
Following are the features implemented on water:
1. Bump Mapping
2. dudv mapping to produce distortion in water texture which gives the illusion that it is a water surface
3. Wave calculations in distortion to give the illusion that water is moving
4. Contribution of multiple lights sources to produce the final color
5. Reflections from skybox on the water surface
6. Made water slightly transparent

## Textured Meshes (Trees and Monster)
Following are the features implemented on textured meshes:
1. Skeletal and Non-skeletal Meshes
2. Skeletal Animation that supports lighting calculations.
3. Animated geometry via custom Vertex Shader (to display the animation)
4. Contribution of multiple lights sources to produce the final color

## Lights
1. Directional Light Calculations
2. Point Light Calculations
3. Spot Light Calculations
4. Multiple lights (of different types) in forward shading
5. Physically correct attenuation done using constant, linear and quadratic values in point light and spot light.

Light's base class has the following properties which are then used by the fragment shader to compute the final results;
1. The diffuse intensity of a light is the aspect of the light that interacts with diffuse material color. It can be viewed as the actual color of the light.
2. The specular intensity of a light is what interacts with specular material color.
3. The ambient intensity of a light adds to ambient lighting of the object.
4. Light intensity refers to the strength or amount of light produced by a specific lamp source. Lights intensity controls how bright the light is.

```
My scene has multiple light sources:
Pressing 1 toggles between 
1. Displaying daytime which uses one directional light to light the environment, and 
2. Nighttime which used multiple spot lights and point lights of various colors and intensities to give a pleasant look.
```

## Scene Graph
1. All objects including camera and lights have been implemented as scene nodes. Even scene object is a derived class from scene object because each scene can be a child of another big scene. 
2. In my implementation there are multiple types of Scene Nodes:
    1. Abstract Base Class: SceneNode 
    2. Camera Class: Derived from SceneNode
    3. Light Class: Derived from SceneNode
    4. GroupNode Class: Derived from SceneNode
    5. Scene Class: Derived from GroupNode
    6. GeometryNode Class: Derived from GroupNode
    7. All objects implemented in this project (tree, water, terrain, monster etc.):  Derived from GeometryNode 
3. Transform Matrix heirarchy is used to form model matrix.

## Camera
1. Two types of Camera (both inherit from Camera base class): 
    1. Perspective Camera
    2. Orthographic Camera
2. Camera Animation has been implemented. Press 2 to toggle between camera animation and your own control.

## Materials
I have implemented material attributes as well to be used in lighting calculations:
1. Ambient is is the color of the object in the presence of ambient light. By default, the ambient color will be the same as the diffuse value, unless otherwise specified.
2. Diffuse is the primary color of an object as it would appear in pure white light
3. Specular is the "highlight" color of a material. It can be thought of as the reflection of the light source itself on the surface of an object.
4. The shininess parameter controls the size of the highlight: larger numbers produce smaller highlights, which makes the object appear shinier. e.g. 32, 64, 128

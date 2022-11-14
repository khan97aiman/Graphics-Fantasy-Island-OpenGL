#version 330 core

#define MAX_LIGHTS 100 

struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
}; 

struct DirectionalLight {
    Light base;
    vec3 direction;
}; 

struct PointLight {    
    Light base;
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  
};  

struct SpotLight
{
    PointLight base;
    vec3 axis;
    float cutOff;
};

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 
  
uniform int numDirectionalLights;
uniform int numSpotLights;
uniform int numPointLights;

uniform DirectionalLight directionalLights[MAX_LIGHTS];
uniform SpotLight spotLights[MAX_LIGHTS];
uniform PointLight pointLights[MAX_LIGHTS];
uniform vec3 cameraPos;

uniform sampler2D mudTex;
uniform sampler2D rockTex;
uniform sampler2D grassTex;
uniform sampler2D snowTex;

uniform Material material;

in Vertex {
	vec4 colour;
	vec2 texCoord;
	vec3 normal;
	vec3 worldPos;
} IN;

out vec4 fragColour;

void CalcLight(in Light light, in float diff, in float spec, out vec3 ambient, out vec3 diffuse, out vec3 specular) {
    ambient  = light.ambient * material.ambient;// * vec3(texture(material.ambient, IN.texCoord));
    diffuse  = light.diffuse * diff * material.diffuse;// * vec3(texture(material.diffuse, IN.texCoord));
    specular = light.specular * spec * material.specular;// * vec3(texture(material.specular, IN.texCoord));
}


vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    //vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess); //IMPORTANT REPLACE 60 with material.shininess
    
    // combine results
    vec3 ambient; vec3 diffuse; vec3 specular;
    CalcLight(light.base, diff, spec, ambient, diffuse, specular);
    return (ambient + diffuse + specular);
}  

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 worldPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - worldPos);
    
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    
    // specular shading
    //vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    
    // attenuation
    float distance    = length(light.position - worldPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    
    // combine results
    vec3 ambient; vec3 diffuse; vec3 specular;
    CalcLight(light.base, diff, spec, ambient, diffuse, specular);
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
} 

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 worldPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.base.position - worldPos);
    float spotAngle = max(dot(lightDir, light.axis), 0.0);

    if (spotAngle > radians(light.cutOff)) {
        vec3 color = CalcPointLight(light.base, normal, worldPos, viewDir);
        return color * (1.0 - (1.0 - spotAngle) * 1.0/(1.0 - radians(light.cutOff)));
    }
    else {
        return vec3(0,0,0);
    }
}

vec3 getTriPlanarBlend(vec3 _wNorm){
	// in wNorm is the world-space normal of the fragment
	vec3 blending = abs( _wNorm );
	blending = normalize(max(blending, 0.00001)); // Force weights to sum to 1.0
	float b = (blending.x + blending.y + blending.z);
	blending /= vec3(b, b, b);
	return blending;
}
vec4 hash4( vec2 p ) { return fract(sin(vec4( 1.0+dot(p,vec2(37.0,17.0)), 
                                              2.0+dot(p,vec2(11.0,47.0)),
                                              3.0+dot(p,vec2(41.0,29.0)),
                                              4.0+dot(p,vec2(23.0,31.0))))*103.0); }
vec4 textureNoTile( sampler2D samp, in vec2 uv )
{
    ivec2 iuv = ivec2( floor( uv ) );
     vec2 fuv = fract( uv );

    // generate per-tile transform
    vec4 ofa = hash4( iuv + ivec2(0,0) );
    vec4 ofb = hash4( iuv + ivec2(10,0) );
    vec4 ofc = hash4( iuv + ivec2(0,10) );
    vec4 ofd = hash4( iuv + ivec2(10,10) );
    
    vec2 ddx = dFdx( uv );
    vec2 ddy = dFdy( uv );

    // transform per-tile uvs
    ofa.zw = sign( ofa.zw-0.5 );
    ofb.zw = sign( ofb.zw-0.5 );
    ofc.zw = sign( ofc.zw-0.5 );
    ofd.zw = sign( ofd.zw-0.5 );
    
    // uv's, and derivatives (for correct mipmapping)
    vec2 uva = uv*ofa.zw + ofa.xy, ddxa = ddx*ofa.zw, ddya = ddy*ofa.zw;
    vec2 uvb = uv*ofb.zw + ofb.xy, ddxb = ddx*ofb.zw, ddyb = ddy*ofb.zw;
    vec2 uvc = uv*ofc.zw + ofc.xy, ddxc = ddx*ofc.zw, ddyc = ddy*ofc.zw;
    vec2 uvd = uv*ofd.zw + ofd.xy, ddxd = ddx*ofd.zw, ddyd = ddy*ofd.zw;
        
    // fetch and blend
    vec2 b = smoothstep( 0.25,0.75, fuv );
    
    return mix( mix( textureGrad( samp, uva, ddxa, ddya ), 
                     textureGrad( samp, uvb, ddxb, ddyb ), b.x ), 
                mix( textureGrad( samp, uvc, ddxc, ddyc ),
                     textureGrad( samp, uvd, ddxd, ddyd ), b.x), b.y );
}


void main(void) {
    // properties
    vec3 norm = normalize(IN.normal);
    vec3 viewDir = normalize(cameraPos - IN.worldPos);

	// define an output color value
	vec3 result = vec3(0.0);

    // add the directional lights contribution to the output
    for(int i = 0; i < numDirectionalLights; i++) {
        result += CalcDirectionalLight(directionalLights[i], norm, viewDir);
    }

    // add the point lights contribution to the output
    for(int i = 0; i < numPointLights; i++) {
        result += CalcPointLight(pointLights[i], norm, IN.worldPos, viewDir);
    }

    // add the spot lights contribution to the output
    for(int i = 0; i < numSpotLights; i++) {
        result += CalcSpotLight(spotLights[i], norm, IN.worldPos, viewDir);
    }

	fragColour = vec4(result, 1.0);
    
    if (IN.worldPos.y <= 20 && IN.worldPos.y >= 0) {
        
        fragColour = fragColour * mix(textureNoTile(mudTex, IN.texCoord), texture(mudTex, IN.texCoord), 0);
        
//        vec3 blending = getTriPlanarBlend(norm);
//        vec4 xaxis = texture( pebblexTex, IN.worldPos.yz);
//        vec4 yaxis = texture( pebblexTex, IN.worldPos.xz);
//        vec4 zaxis = texture( pebblexTex, IN.worldPos.xy);
//        // blend the results of the 3 planar projections.
//        vec4 tex = xaxis * blending.x + yaxis * blending.y + zaxis * blending.z;
        //fragColour = fragColour *  vec4(texture(pebblexTex, IN.texCoord)); //pebbles
       // fragColour = fragColour * mix(texture(pebblexTex, IN.texCoord), texture(mudTex, IN.texCoord), (IN.worldPos.y - 0)/(20-0)); 

    }
//    else if (IN.worldPos.y <= 40 && IN.worldPos.y > 0) {
//        fragColour = fragColour * mix(texture(pebblexTex, IN.texCoord), texture(mudTex, IN.texCoord), (IN.worldPos.y - 0)/(40-0)); 
//    }
    else if (IN.worldPos.y <= 150 && IN.worldPos.y > 20) {
        fragColour = fragColour * mix(texture(mudTex, IN.texCoord), texture(grassTex, IN.texCoord), (IN.worldPos.y - 20)/(150-20)); 
    }
    else if (IN.worldPos.y <= 1255 && IN.worldPos.y > 150) {
        //fragColour = fragColour * vec4(texture(grassTex, IN.texCoord));// * vec4(texture(rockTex, IN.texCoord)); //pebbles
        fragColour = fragColour * mix(texture(grassTex, IN.texCoord), texture(rockTex, IN.texCoord), (IN.worldPos.y - 150)/(1255-150)); //change pebbles to another texture
    }
//    else if (IN.worldPos.y <= 1000 && IN.worldPos.y > 400) {
//        float a = clamp(norm.y, 0, 1);
//        fragColour = fragColour * mix(texture(rockTex, IN.texCoord), texture(grassTex, IN.texCoord),a) ;
//        //fragColour = fragColour * vec4(texture(grassTex, IN.texCoord)) * vec4(texture(rockTex, IN.texCoord)); //pebbles
//        //fragColour = fragColour * vec4(texture(snowGrassTex, IN.texCoord)); 
//    }
//    else if (IN.worldPos.y <= 1250 && IN.worldPos.y > 700) {
//        float a = clamp(norm.y, 0, 1);
//
//        fragColour = fragColour * mix(texture(grassTex, IN.texCoord), texture(rockTex, IN.texCoord),(IN.worldPos.y - 700)/(1250-700)) ;
//
//        //fragColour = fragColour * vec4(texture(snowTex, IN.texCoord)); 
//    }

//    if (IN.worldPos.y <= 20 && IN.worldPos.y >= 0) {
//        fragColour = vec4(1.0, 0.0, 0.0, 1.0); //pebbles
//    }
//
//    if (IN.worldPos.y <= 100 && IN.worldPos.y > 0) {
//        fragColour = vec4(0.0, 1.0, 0.0, 1.0); //grassy rock
//    }
//
//    if (IN.worldPos.y <= 400 && IN.worldPos.y > 100) {
//        fragColour = vec4(0.0, 0.0, 1.0, 1.0); //grass
//    }
//
//    if (IN.worldPos.y <= 800 && IN.worldPos.y > 400) {
//        fragColour = vec4(1.0, 0.0, 1.0, 1.0); //grassy mountain
//    }
//    if (IN.worldPos.y <= 1000 && IN.worldPos.y > 800) {
//        fragColour = vec4(1.0, 1.0, 1.0, 1.0); //rocky mountain
//    }
//    if (IN.worldPos.y <= 1255 && IN.worldPos.y > 1000) {
//        fragColour = vec4(1.0, 1.0, 0.0, 1.0); //rocky mountain
//    }
}

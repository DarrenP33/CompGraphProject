#version 330 core
out vec3 N;
out vec3 L;
out vec3 E;
out vec3 H;
out vec3 Lspot;
out vec3 Hspot;
out vec3 L2;
out vec3 H2;
out vec3 L3;
out vec3 H3;
out vec3 L4;
out vec3 H4;
out vec3 L5;
out vec3 H5;
out vec4 eyePosition;
out vec2 texCoordsInterpolated;
 
layout(location = 0) in  vec3 vertexPosition;
layout(location = 1) in  vec3 vertexNormal;
layout(location = 2) in  vec2 vertexTextureCoordinates;

uniform vec4 lightPosition;
uniform vec4 lightPosition2;
uniform vec4 lightPosition3;
uniform vec4 lightPosition4;
uniform vec4 lightPosition5;
uniform vec4 lightPositionSpot;
uniform mat4 Projection;
uniform mat4 ModelView;

 uniform vec4 lightDiffuse;
 uniform vec4 lightSpecular; 
 uniform vec4 lightAmbient;
 uniform vec4 lightDiffuseSpot;
 uniform vec4 lightSpecularSpot; 
 uniform vec4 lightAmbientSpot;
 uniform vec4 surfaceDiffuse;
 uniform vec4 surfaceSpecular;
 uniform float shininess;
 uniform float shininessSpot;
 uniform float spotExponent;
 uniform float cutOffAngle;
 uniform vec4 surfaceAmbient;
 uniform vec4  surfaceEmissive;
 uniform float useTexture;
 uniform vec4  d;
 uniform float on;
void main()
{
	//if(useTexture > 0.0){
		texCoordsInterpolated = vertexTextureCoordinates;
	//}
    gl_Position = Projection * ModelView * vec4(vertexPosition, 1.0);

    eyePosition = ModelView * vec4(vertexPosition, 1.0);
    vec4 eyeLightPos = lightPosition;
    vec4 eyeLightPos2 = lightPosition2;
    vec4 eyeLightPos3 = lightPosition3;
    vec4 eyeLightPos4 = lightPosition4;
    vec4 eyeLightPos5 = lightPosition5;
    vec4 eyeLightPosSpot = lightPositionSpot;
	
	N = normalize(ModelView * vec4(vertexNormal,0.0)).xyz;
    L = normalize(eyeLightPos.xyz - eyePosition.xyz);
    E = -normalize(eyePosition.xyz);
    H = normalize(L + E);

    Lspot = normalize(eyeLightPosSpot.xyz - eyePosition.xyz);
    Hspot = normalize(Lspot + E);

    L2 = normalize(eyeLightPos2.xyz - eyePosition.xyz);
    H2 = normalize(L2 + E);

    L3 = normalize(eyeLightPos3.xyz - eyePosition.xyz);
    H3 = normalize(L3 + E);

    L4 = normalize(eyeLightPos4.xyz - eyePosition.xyz);
    H4 = normalize(L4 + E);

    L5 = normalize(eyeLightPos5.xyz - eyePosition.xyz);
    H5 = normalize(L5 + E);
}

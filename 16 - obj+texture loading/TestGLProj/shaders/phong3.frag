 #version 330 core
 in vec3 N;
 in vec3 L;
 in vec3 E;
 in vec3 H;
 in vec3 Hspot;
 in vec3 Lspot;
 in vec4 eyePosition;
 in vec2 texCoordsInterpolated;
 

uniform vec4 lightPosition;
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
 uniform vec4  d;
 uniform float on;
 uniform float useTexture;
 uniform sampler2D diffuseTexture;
 uniform float linearAttenuationCoefficient;
 uniform float linearAttenuationCoefficientSpot;

void main()
{
     vec3 Normal = normalize(N);
     vec3 Light  = normalize(lightPosition - eyePosition).xyz;
     vec3 LightSpot  = normalize(lightPositionSpot - eyePosition).xyz;
     vec3 Eye    = normalize(E);
     vec3 Half   = normalize(H);
     vec3 HalfSpot   = normalize(Hspot);
     vec4 V = normalize(eyePosition - lightPositionSpot);
     float spotEffect;

    float angle = dot(V,d);

    if(acos(angle)<cutOffAngle)
    {
        spotEffect = (pow(max(angle,0),spotExponent));
    }
    else
    {
        spotEffect = 0.0f;
    }

	float linearAttenuation = min(1.0, 1.0/ (linearAttenuationCoefficient * length(lightPosition - eyePosition)));
    float linearAttenuationSpot = min(1.0, 1.0/ (linearAttenuationCoefficientSpot * length(lightPositionSpot - eyePosition)));


    float Kd = max(dot(Normal, Light), 0.0);
    float Ks = pow(max(dot(Normal,Half), 0.0), shininess);
    float Ka = 1.0;

    float KdSpot = max(dot(Normal, LightSpot), 0.0);
    float KsSpot = pow(max(dot(reflect(-LightSpot, Normal),Eye), 0.0), shininess);
    float KaSpot = 1.0;

    vec4 diffuse  = Kd * lightDiffuse*surfaceDiffuse;
    vec4 specular = Ks * lightSpecular*surfaceSpecular;
    vec4 ambient  = Ka * lightAmbient*surfaceAmbient;

    vec4 diffuseSpot  = KdSpot * lightDiffuseSpot*surfaceDiffuse;
    vec4 specularSpot = KsSpot * lightSpecularSpot*surfaceSpecular;
    vec4 ambientSpot  = KaSpot * lightAmbientSpot*surfaceAmbient;

    vec4 spotColor = surfaceEmissive  +  linearAttenuationSpot*spotEffect*(diffuseSpot + specularSpot);

	vec4 texColor = vec4(1.0,1.0,1.0,1.0);
	if(useTexture > 0.0){
		texColor = texture2D(diffuseTexture,texCoordsInterpolated);
	}
    gl_FragColor = surfaceEmissive + ambient + (linearAttenuation*( diffuse + specular)*texColor) + (on*spotColor);
}

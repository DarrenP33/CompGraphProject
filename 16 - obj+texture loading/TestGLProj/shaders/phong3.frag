 #version 330 core
 in vec3 N;
 in vec3 L;
 in vec3 E;
 in vec3 H;
 in vec3 Hspot;
 in vec3 Lspot;
 in vec3 L2;
 in vec3 H2;
 in vec3 L3;
 in vec3 H3;
 in vec3 L4;
 in vec3 H4;
 in vec3 L5;
 in vec3 H5;
 in vec4 eyePosition;
 in vec2 texCoordsInterpolated;
 

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
     vec3 Light2  = normalize(lightPosition2 - eyePosition).xyz;
     vec3 Light3  = normalize(lightPosition3 - eyePosition).xyz;
     vec3 Light4  = normalize(lightPosition4 - eyePosition).xyz;
     vec3 Light5  = normalize(lightPosition5 - eyePosition).xyz;
     vec3 LightSpot  = normalize(lightPositionSpot - eyePosition).xyz;
     vec3 Eye    = normalize(E);
     vec3 Half   = normalize(H);
     vec3 Half2   = normalize(H2);
     vec3 Half3   = normalize(H3);
     vec3 Half4   = normalize(H4);
     vec3 Half5   = normalize(H5);
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
    float linearAttenuation2 = min(1.0, 1.0/ (linearAttenuationCoefficient * length(lightPosition2 - eyePosition)));
    float linearAttenuation3 = min(1.0, 1.0/ (linearAttenuationCoefficient * length(lightPosition3 - eyePosition)));
    float linearAttenuation4 = min(1.0, 1.0/ (linearAttenuationCoefficient * length(lightPosition4 - eyePosition)));
    float linearAttenuation5 = min(1.0, 1.0/ (linearAttenuationCoefficient * length(lightPosition5 - eyePosition)));
    float linearAttenuationSpot = min(1.0, 1.0/ (linearAttenuationCoefficientSpot * length(lightPositionSpot - eyePosition)));


    float Kd = max(dot(Normal, Light), 0.0);
    float Ks = pow(max(dot(Normal,Half), 0.0), shininess);
    float Ka = 1.0;

    float Kd2 = max(dot(Normal, Light2), 0.0);
    float Ks2 = pow(max(dot(Normal,Half2), 0.0), shininess);

    float Kd3 = max(dot(Normal, Light3), 0.0);
    float Ks3 = pow(max(dot(Normal,Half3), 0.0), shininess);

    float Kd4 = max(dot(Normal, Light4), 0.0);
    float Ks4 = pow(max(dot(Normal,Half4), 0.0), shininess);

    float Kd5 = max(dot(Normal, Light5), 0.0);
    float Ks5 = pow(max(dot(Normal,Half5), 0.0), shininess);

    float KdSpot = max(dot(Normal, LightSpot), 0.0);
    float KsSpot = pow(max(dot(reflect(-LightSpot, Normal),Eye), 0.0), shininess);
    float KaSpot = 1.0;

    vec4 diffuse  = Kd * lightDiffuse*surfaceDiffuse;
    vec4 specular = Ks * lightSpecular*surfaceSpecular;
    vec4 ambient  = Ka * lightAmbient*surfaceAmbient;

    vec4 diffuse2  = Kd2 * lightDiffuse*surfaceDiffuse;
    vec4 specular2 = Ks2 * lightSpecular*surfaceSpecular;
    vec4 ambient2  = Ka * lightAmbient*surfaceAmbient;

    vec4 diffuse3  = Kd3 * lightDiffuse*surfaceDiffuse;
    vec4 specular3 = Ks3 * lightSpecular*surfaceSpecular;
    vec4 ambient3  = Ka * lightAmbient*surfaceAmbient;

    vec4 diffuse4  = Kd4 * lightDiffuse*surfaceDiffuse;
    vec4 specular4 = Ks4 * lightSpecular*surfaceSpecular;
    vec4 ambient4  = Ka * lightAmbient*surfaceAmbient;

    vec4 diffuse5  = Kd5 * lightDiffuse*surfaceDiffuse;
    vec4 specular5 = Ks5 * lightSpecular*surfaceSpecular;
    vec4 ambient5  = Ka * lightAmbient*surfaceAmbient;

    vec4 diffuseSpot  = KdSpot * lightDiffuseSpot*surfaceDiffuse;
    vec4 specularSpot = KsSpot * lightSpecularSpot*surfaceSpecular;
    vec4 ambientSpot  = KaSpot * lightAmbientSpot*surfaceAmbient;

    vec4 spotColor = surfaceEmissive  +  linearAttenuationSpot*spotEffect*(diffuseSpot + specularSpot);

	vec4 texColor = vec4(1.0,1.0,1.0,1.0);
	if(useTexture > 0.0){
		texColor = texture2D(diffuseTexture,texCoordsInterpolated);
	}

    vec4 l2color = (linearAttenuation2*( diffuse2 + specular2)*texColor);

    vec4 l3color = (linearAttenuation3*( diffuse3 + specular3)*texColor);

    vec4 l4color = (linearAttenuation4*( diffuse4 + specular4)*texColor);

    vec4 l5color = (linearAttenuation5*( diffuse5 + specular5)*texColor);

    gl_FragColor = surfaceEmissive + ambient + (linearAttenuation*( diffuse + specular)*texColor) + l2color + l3color + l4color + l5color + (on*spotColor);
}

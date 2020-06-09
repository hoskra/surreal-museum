#version 140
// --------------------- material
struct Material {
  vec3  ambient;
  vec3  diffuse;
  vec3  specular;
  float shininess;
  bool  useTexture;
};

uniform Material material;

// --------------------- light
struct Light {
  vec3  ambient;
  vec3  diffuse;
  vec3  specular;      // intensity & color of the specular component
  vec3  position;
  vec3  spotDirection;
  float spotCosCutOff; // cosine of the spotlight's half angle
  float spotExponent;  // distribution of the light energy within the reflector's cone (center->cone's edge)
};

Light sun;
Light light1;
Light light2;

uniform vec3 pointLight1position;
uniform vec3 spotLight1position;
uniform vec3 reflectorDirection;
uniform float reflectorExponent;
uniform float spotCosCutOff;

// --------------------- uniforms
uniform sampler2D texSampler;
uniform float time;

uniform mat4 PVMmatrix;
uniform mat4 Vmatrix;
uniform mat4 Mmatrix;
uniform mat4 normalMatrix;

// --------------------- input
smooth in vec2 texCoord_v;
in vec3 position_v;
in vec3 normal_v;

// --------------------- fog
float fogFactor;
vec3 fogColor = vec3(0.1, 0.1, 0.1);

// --------------------- output
out vec4       outputColor;

vec4 spotLight(Light light, Material material, vec3 fPosition, vec3 vertexNormal) {
  vec3 ret = vec3(0.0);

  vec3 L = normalize(light.position - fPosition);
  vec3 R = reflect(-L, vertexNormal);
  vec3 V = normalize(-fPosition);
  float NdotL = max(0.0, dot(vertexNormal, L));
  float RdotV = max(0.0, dot(R, V));
  float spotCoef = max(0.0, dot(-L, light.spotDirection));

  ret += material.ambient * light.ambient;
  ret += material.diffuse * light.diffuse * NdotL;
  ret += material.specular * light.specular * pow(RdotV, 2.0f);

  if(spotCoef < light.spotCosCutOff)
    ret *= 0.0;
  else
    ret *= pow(spotCoef, light.spotExponent);

  return vec4(ret, 1.0);
}

vec4 pointLight(Light light, Material material, vec3 fPosition, vec3 vertexNormal){
    vec3 ret = vec3(0.0f);
	float distance = length(light.position - fPosition.xyz);
    
	vec3 L = normalize(light.position.xyz - fPosition);
    vec3 R = reflect(-L, vertexNormal);
    vec3 V = normalize(-fPosition);
    float RdotV = max(0.0, dot(R, V));
    	
    float constant = 1.0f;
    float linear =  0.009f;
    float quadratic  = 0.032f;
    float attenuation = 1.0 / ( constant + linear * distance + quadratic * (distance * distance));   
    //attenuation = 1.0f;

    ret += (material.ambient * light.ambient) * attenuation;
    ret += (material.diffuse * light.diffuse * max(0.0f, dot(vertexNormal, L))) * attenuation;
    ret += (material.specular * light.specular * pow(RdotV, material.shininess)) * attenuation;
    ret /= (distance*distance/6.0f);

    return vec4(ret, 1.0f);
}

vec4 directionalLight(Light light, Material material, vec3 fPosition, vec3 vertexNormal) {
    vec3 ret = vec3(0.0);

    vec3 L = normalize(light.position);
    vec3 R = reflect(-L, vertexNormal);
    vec3 V = normalize(-fPosition);
    float NdotL = max(0.0, dot(vertexNormal, L));
    float RdotV = max(0.0, dot(R, V));

    ret += material.ambient * light.ambient;
    ret += material.diffuse * light.diffuse * NdotL;
    ret += material.specular * light.specular * pow(RdotV, material.shininess);

    return vec4(ret, 1.0);
}

void setupLights() {
    sun.ambient  = vec3(0.0);
    sun.diffuse  = vec3(0.6, 0.6, 0.6f);
    sun.specular = vec3(0.2,0.2,0.2);

    sun.position = (Vmatrix * vec4(cos(time * 0.5), 1.0, sin(time * 0.5), 0.0)).xyz;

    light1.ambient       = vec3(cos(time * 0.5), sin(time * 0.5), 1.0f); 
    light1.diffuse       = vec3(0.0f, 0.0f, 0.8f);
    light1.specular      = vec3(0.0f, 0.5f, 0.5f);
    light1.spotCosCutOff = 0.180f;
    light1.spotExponent  = 0.0;

    light1.position = (Vmatrix * vec4(pointLight1position, 1.0)).xyz;

    light2.ambient       = vec3(0.2f);
    light2.diffuse       = vec3(1.0);
    light2.specular      = vec3(1.0);
    light2.spotCosCutOff = spotCosCutOff;
    light2.spotExponent  = reflectorExponent;


    light2.position = (Vmatrix * vec4(spotLight1position, 1.0)).xyz;
    light2.spotDirection = normalize((Vmatrix * vec4(reflectorDirection, 0.0)).xyz);
}

void main() {
    // FOG
    float fogDist = abs(position_v.z)/20.0f;
	fogFactor = clamp(
		1 - fogDist/4.0f, 
        0.0, 
        1.0
	);

    // LIGHTS
    setupLights();
    vec3 globalAmbientLight = vec3(0.5f);

    // COLOR
    outputColor = vec4(material.ambient * globalAmbientLight, 0.0);

    // TEXTURE
    if(material.useTexture){
        outputColor *= texture(texSampler, texCoord_v);
    } else {
        outputColor = vec4(0.5f, 0.5f, 0.5f, 0.0f);
    }

    // TEXTURE
    outputColor += directionalLight   (sun, material, position_v, normal_v);
    outputColor += pointLight          (light1, material, position_v, normal_v);
    outputColor += spotLight          (light2, material, position_v, normal_v);

    outputColor = vec4(mix(fogColor, outputColor.rgb, fogFactor), outputColor.w);
}
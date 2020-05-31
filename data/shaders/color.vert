#version 140
// --------------------- structs
struct Material {    
  vec3  ambient;
  vec3  diffuse;
  vec3  specular;
  float shininess;
  bool  useTexture;
};

struct Light {
  vec3  ambient;
  vec3  diffuse;
  vec3  specular;      // intensity & color of the specular component
  vec3  position;
  vec3  spotDirection;
  float spotCosCutOff; // cosine of the spotlight's half angle
  float spotExponent;  // distribution of the light energy within the reflector's cone (center->cone's edge)
};

// --------------------- attributes
in vec3 position;
in vec3 normal; 
in vec2 texCoord;

// --------------------- uniforms
uniform Material material;
uniform float time;

uniform mat4 PVMmatrix;                 // Projection * View * Model  --> model to clip coordinates
uniform mat4 Vmatrix;                   // View                       --> world to eye coordinates
uniform mat4 Mmatrix;                   // Model                      --> model to world coordinates
uniform mat4 normalMatrix;              // inverse transposed Mmatrix

uniform vec3 reflectorPosition;         // reflector position (world coordinates)
uniform vec3 reflectorDirection;        // reflector direction (world coordinates)

// --------------------- texture variables
uniform mat3 textureTransform;
uniform int option;
uniform sampler2D texSampler;

// --------------------- output variables
smooth out vec2 texCoord_v;
smooth out vec4 theColor;

vec4 spotLight(Light light, Material material, vec3 vertexPosition, vec3 vertexNormal) {
  vec3 ret = vec3(0.0);

  // use the material and light structures to obtain the surface and light properties
  // the vertexPosition and vertexNormal variables contain transformed surface position and normal
  // store the ambient, diffuse and specular terms to the ret variable
  // for spot lights, light.position contains the light position
  // everything is expressed in the view coordinate system -> eye/camera is in the origin

  vec3 L = normalize(light.position - vertexPosition);
  vec3 R = reflect(-L, vertexNormal);
  vec3 V = normalize(-vertexPosition);
  float NdotL = max(0.0, dot(vertexNormal, L));
  float RdotV = max(0.0, dot(R, V));
  float spotCoef = max(0.0, dot(-L, light.spotDirection));

  ret += material.ambient * light.ambient;
  ret += material.diffuse * light.diffuse * NdotL;
  ret += material.specular * light.specular * pow(RdotV, material.shininess);

  if(spotCoef < light.spotCosCutOff)
    ret *= 0.0;
  else
    ret *= pow(spotCoef, light.spotExponent);

  return vec4(ret, 1.0);
}

vec4 directionalLight(Light light, Material material, vec3 vertexPosition, vec3 vertexNormal) {
    vec3 ret = vec3(0.0);

    // use the material and light structures to obtain the surface and light properties
    // the vertexPosition and vertexNormal variables contain transformed surface position and normal
    // store the ambient, diffuse and specular terms to the ret variable
    // glsl provides some built-in functions, for example: reflect, normalize, pow, dot
    // for directional lights, light.position contains the direction
    // everything is expressed in the view coordinate system -> eye/camera is in the origin

    vec3 L = normalize(light.position);
    vec3 R = reflect(-L, vertexNormal);
    vec3 V = normalize(-vertexPosition);
    float NdotL = max(0.0, dot(vertexNormal, L));
    float RdotV = max(0.0, dot(R, V));

    ret += material.ambient * light.ambient;
    ret += material.diffuse * light.diffuse * NdotL;
    ret += material.specular * light.specular * pow(RdotV, material.shininess);

    return vec4(ret, 1.0);
}

// hardcoded lights
float sunSpeed = 0.5f;

Light sun;
Light spaceShipReflector;

void setupLights() {
    sun.ambient  = vec3(0.0);
    sun.diffuse  = vec3(1.0, 1.0, 0.5f);
    sun.specular = vec3(1.0);

    sun.position = (Vmatrix * vec4(cos(time * sunSpeed), 0.0, sin(time * sunSpeed), 0.0)).xyz;
    //sun.position = (Vmatrix * vec4(1.0, 1.0, 1.0, 0.0)).xyz;

    spaceShipReflector.ambient       = vec3(0.2f);
    spaceShipReflector.diffuse       = vec3(1.0);
    spaceShipReflector.specular      = vec3(1.0);
    spaceShipReflector.spotCosCutOff = 0.95f;
    spaceShipReflector.spotExponent  = 0.0;

    spaceShipReflector.position = (Vmatrix * vec4(reflectorPosition, 1.0)).xyz;
    spaceShipReflector.spotDirection = normalize((Vmatrix * vec4(reflectorDirection, 0.0)).xyz);
}

void main() {
    // POSITION
    vec3 vertexPosition = (Vmatrix * Mmatrix * vec4(position, 1.0)).xyz;                    // vertex in eye coordinates
    vec3 vertexNormal   = normalize( (Vmatrix * normalMatrix * vec4(normal, 0.0) ).xyz);    // normal in eye coordinates by NormalMatrix
    gl_Position = PVMmatrix * vec4(position, 1);

    // LIGHTS
    setupLights();
    vec3 globalAmbientLight = vec3(0.4f);

    // COLOR
    vec4 outputColor = vec4(material.ambient * globalAmbientLight, 0.0);
    outputColor += directionalLight   (sun, material, vertexPosition, vertexNormal);
    outputColor += spotLight          (spaceShipReflector, material, vertexPosition, vertexNormal);
    theColor = outputColor;

    // TEXTURES
    texCoord_v = texCoord;
    if (option == 1) {
        // moving texture
        vec3 new_texture_coordinates = textureTransform * vec3(texCoord, 1.0);
        texCoord_v = vec2(new_texture_coordinates);
    }
}

#version 140

struct Material {
  vec3  ambient;
  vec3  diffuse;
  vec3  specular;
  float shininess;
  bool  useTexture;
};

uniform Material material;
uniform sampler2D texSampler;

smooth in vec4 theColor;
smooth in vec2 texCoord_v;
out vec4       outputColor;

void main() {
    outputColor = theColor;
    
    if(material.useTexture){
        outputColor *= texture(texSampler, texCoord_v);
    } else {
        outputColor = vec4(0.5f, 0.5f, 0.5f, 0.0f);
    }
}
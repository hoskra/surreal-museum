#version 140
// --------------------- attributes
in vec3 position;
in vec3 color;
in vec3 normal; 
in vec2 texCoord;

// --------------------- uniforms
uniform float time;

uniform mat4 PVMmatrix;
uniform mat4 Vmatrix;
uniform mat4 Mmatrix;
uniform mat4 normalMatrix;

// --------------------- texture variables
uniform mat3 textureTransform;
uniform int option;
uniform sampler2D texSampler;

// --------------------- output variables
smooth out vec2 texCoord_v;
smooth out vec4 theColor;
smooth out vec3 position_v;
smooth out vec3 normal_v;

void main() {
    // POSITION
    position_v = (Vmatrix * Mmatrix * vec4(position, 1.0)).xyz;                    // vertex in eye coordinates
    normal_v   = normalize( (Vmatrix * normalMatrix * vec4(normal, 0.0) ).xyz);    // normal in eye coordinates by NormalMatrix
    gl_Position = PVMmatrix * vec4(position, 1);

    // CAMERA POSITION
    position_v = (Vmatrix * Mmatrix * vec4(position, 1)).xyz;

    // TEXTURES
    texCoord_v = texCoord;
    if (option == 1) {
        // moving texture
        vec3 new_texture_coordinates = textureTransform * vec3(texCoord, 1.0);
        texCoord_v = vec2(new_texture_coordinates);
    }
}

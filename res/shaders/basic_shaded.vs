#version 330

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;

out vec3 FragPos;
out vec3 Normal;
out vec3 LightPos;
out vec3 Color;

uniform vec3 lightPos;

uniform mat4 m, v, p;

void main() {
    gl_Position = p * v * m * vec4(aPos, 1.0);

    FragPos = vec3(v * m * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(v * m))) * aNormal;
    LightPos = vec3(v * vec4(lightPos, 1.0));
    Color = aColor;
}
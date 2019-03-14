// Vertex Shader
#version 430

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

out vec3 fragPosition;
out vec3 fragNormal;
out vec2 fragUV;

uniform mat4 model;
uniform mat4 viewProjection;

void main()
{
    fragPosition = vec3(model * vec4(position, 1.0));
    fragNormal = mat3(transpose(inverse(model))) * normal;
    fragNormal = normalize(fragNormal);
    fragUV = uv;

    gl_Position = viewProjection * vec4(fragPosition, 1);
}

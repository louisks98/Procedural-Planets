#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 surface_normal;
out vec3 surface_position;
out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    mat4 normalMatrix = transpose(inverse(view * model));

    surface_normal = vec3(normalMatrix * vec4(aNormal, 1.0));
    surface_position = vec3((view * model) * vec4(position, 1.0));

    gl_Position = projection * view * model * vec4(position, 1.0);
    //normal = aNormal;
    texCoord = aTexCoord;
}
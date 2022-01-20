#version 330 core

in vec3 surface_position;
in vec3 surface_normal;
in vec2 texCoord;

out vec4 FragColor;


uniform vec3 direction;
//uniform vec3 l_position;
uniform vec3 diffuse;
uniform vec3 specular;
uniform vec3 ambient_light;

uniform vec3 mat_ambient;
uniform vec3 mat_diff;
uniform vec3 mat_spec;
uniform float shininess;

uniform vec3 cam_pos;


vec3 CalcutateDirectionalLight(vec3 normal, vec3 viewDir)
{
    vec3 l = normalize(-direction);

    float diff = max(dot(normal, l), 0.0);
    vec3 h = normalize(viewDir + l);

    float spec = pow(max(dot(normal, h), 0.0), shininess);

    vec3 ambient = ambient_light * mat_ambient;
    vec3 diffuse = diffuse * mat_diff * diff;
    vec3 specular = specular * mat_spec * spec;

    return ambient + diffuse + specular;
}

void main()
{
    vec3 n = normalize(surface_normal);
    vec3 v = normalize(cam_pos - surface_position);
    vec3 color = CalcutateDirectionalLight(n, v);

    FragColor = vec4(color, 1.0);
}
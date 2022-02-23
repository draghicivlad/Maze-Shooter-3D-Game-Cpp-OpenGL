#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// Uniforms for light properties
//uniform vec3 light_position;
//uniform vec3 eye_position;
//uniform float material_kd;
//uniform float material_ks;
//uniform int material_shininess;

uniform vec3 object_color;

// Output value to fragment shader
out vec3 color;


void main()
{
    // TODO(student): Compute world space vectors
    /*
    vec3 world_pos = (Model * vec4(v_position,1)).xyz;
    vec3 world_normal = normalize(mat3(Model) * v_normal);
    vec3 L = normalize(light_position - world_pos);
    vec3 V = normalize(eye_position - world_pos);
    vec3 H = normalize(L + V);
    vec3 R = reflect(-L, world_normal);
    float d = length(eye_position - world_pos);

    float ambient_light = material_kd * 0.25;

    float diffuse_light = material_kd * max(0, dot(world_normal, L));

    float aux = max(0, dot(V, R));
    float p = 1;
    for(int i = 0; i < material_shininess; i++)
        p *= aux;

    float specular_light = material_ks * p;
    
    float light = ambient_light + 1.0/(d * d) * (diffuse_light + specular_light);
    */


    color = object_color;

    gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}

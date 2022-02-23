#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform mat4 RandomMatrix;
uniform float actTime;
uniform float maxTime;

// Output
out vec3 frag_normal;
out vec3 frag_color;
out vec2 tex_coord;


void main()
{
    frag_normal = v_normal;
    frag_color = v_color;
    tex_coord = v_texture_coord;

    float dis = fract(sin(dot(v_normal.xy, vec2(12.9898 + actTime, 78.233 + actTime))) * 43758.5453123);
    vec3 newPos = v_position;

    if (actTime != 0)
        newPos = v_position * (1 + dis) * (1 + actTime * 0.5);

    gl_Position = Projection * View * Model * vec4(newPos, 1.0);
}

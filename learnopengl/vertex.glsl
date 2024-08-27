#version 330 core
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_color;

uniform float offset;

out vec3 vertex_color;
out vec4 vertex_position;

void main()
{
    gl_Position = vec4(offset + a_pos.x, a_pos.yz , 1.0);
    vertex_color = a_color;
    vertex_position = gl_Position;
}
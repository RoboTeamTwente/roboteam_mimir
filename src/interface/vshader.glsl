#version 330 core
layout (location = 0) in vec4 aPos;
uniform mat4 mvp_matrix;
void main()
{
    gl_Position = mvp_matrix * aPos;
}
#version 130
uniform mat4 mvp_matrix;

attribute vec4 aPos;
attribute vec3 color;

out vec3 f_color;

void main()
{
    //project vertex pos onto screen
    gl_Position = mvp_matrix * aPos;

    //pass colors to fragment shader
    f_color=color;
}
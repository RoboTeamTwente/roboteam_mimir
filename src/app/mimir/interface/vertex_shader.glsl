#version 130
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

attribute vec3 pos;
attribute vec3 color;

out vec3 f_color;

void main()
{
    //project vertex pos onto screen
    gl_Position = projection * view * model * vec4(pos,1.0);

    //pass colors to fragment shader
    f_color=color;
}
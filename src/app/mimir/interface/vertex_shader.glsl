#version 330
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

in vec3 pos;
in vec3 color;
in vec3 objectNormal;
in vec2 tex_pos;

out vec3 objectColor;
out vec3 normal;
out vec3 FragPos;
out vec2 texCoord;

void main()
{
    //project vertex pos onto screen
    gl_Position = projection * view * model * vec4(pos,1.0);

    //pass colors to fragment shader
    objectColor = color;
    normal = mat3(transpose(inverse(model))) * objectNormal;//TODO: do the matrix transpose+inverse on CPU
    FragPos = vec3(model * vec4(pos,1.0));
    texCoord = tex_pos;
}
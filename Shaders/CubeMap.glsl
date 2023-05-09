S|Vertex
#version 330 core

layout(location = 0) in vec4 position;

out vec4 pos;
out vec3 uv;

// Camera Vars
uniform mat4 u_View;
uniform mat4 u_Projection;  

void main()
{
    gl_Position = (u_Projection * u_View) * position;
    uv = vec3(position);
}


S|Fragment
#version 330 core

layout(location = 0) out vec4 colour;

in vec3 uv;

uniform samplerCube skybox;

void main()
{
    colour = texture(skybox, uv); 
}
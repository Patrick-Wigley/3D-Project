S|Vertex
#version 330 core

layout(location = 0) in vec4 position;

out vec4 pos;
out vec3 tex_coords;

// Camera Vars
uniform mat4 u_View;
uniform mat4 u_Projection;  

void main()
{
    gl_Position = (u_Projection * u_View) * position;
    tex_coords = vec3(position);
}


S|Fragment
#version 330 core

layout(location = 0) out vec4 colour;

in vec3 tex_coords;


uniform samplerCube skybox;



void main()
{
    colour = texture(skybox, tex_coords); 
}
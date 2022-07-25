S|Vertex
#version 330 core

layout(location = 0) in vec3 position;
out vec4 pos;

// Camera Vars
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat4 u_ModelMatrix;

uniform vec3 u_CameraPos;

void main()                               
{                                         
    gl_Position = (u_Projection * u_View * u_ModelMatrix) * vec4(position, 1.0);
    
}                                         


S|Fragment
#version 330 core                         
                                          
layout(location = 0) out vec4 colour;     


uniform float u_offset;
void main()                               
{
    colour = vec4(.1 + u_offset, .5 + u_offset, .3 + u_offset , 1.0);
}                                         
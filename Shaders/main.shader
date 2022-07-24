S|Vertex
#version 330 core

layout(location = 0) in vec4 position;
out vec4 pos;

uniform float u_offset;
void main()                               
{                                         
    gl_Position = position * u_offset + 1;
    
}                                         


S|Fragment
#version 330 core                         
                                          
layout(location = 0) out vec4 colour;     


void main()                               
{
    colour = vec4(.1, .5, .3, 1.0);
}                                         
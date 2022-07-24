S|Vertex
#version 330 core

layout(location = 0) in vec4 position;
out vec4 pos;

void main()                               
{                                         
    gl_Position = position; 
    pos = position;
}                                         


S|Fragment
#version 330 core                         
                                          
layout(location = 0) out vec4 colour;     

in vec4 pos;
void main()                               
{
    colour = vec4(.5, .5, .5, 1.0) * pos+.1;        
}                                         
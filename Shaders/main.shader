S|V
#version 330 core                         
                                          
layout(location = 0) in vec4 position;    
void main()                               
{                                         
    gl_Position = position;                   
}                                         


S|F
#version 330 core                         
                                          
layout(location = 0) out vec4 colour;     
void main()                               
{               
    uniform float time;

    colour = vec4(1.0, 1.0, 0.0, 1.0);        
}                                         
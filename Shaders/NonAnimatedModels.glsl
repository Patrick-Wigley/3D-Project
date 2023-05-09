S|Vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texture_coordinates;
layout(location = 2) in vec3 normals;

// To Fragment Shader
out vec4 pos;
out vec2 uv;


// Camera Vars
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat4 u_ModelMatrix;

uniform vec3 u_CameraPos;


void main()                               
{          
    gl_Position = (u_Projection * u_View * u_ModelMatrix) * vec4(position, 1.0);
    pos = gl_Position;
    uv = texture_coordinates;
}                                         


S|Fragment
#version 330 core                         
                                          
layout(location = 0) out vec4 colour;     

// From Vertex Shader
in vec4 pos;
in vec2 uv;

// Texture
uniform sampler2D u_texture;
uniform bool u_isTextured;


void main()                               
{
    // ATM Everything is textured - (add boolean to determine)
    if (u_isTextured)
        colour = texture(u_texture, uv);
    
    else
    {
        colour = vec4(.1, .1, 1, 1);
    }
}                                         
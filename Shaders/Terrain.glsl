S|Vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texture_coordinates;

// To Fragment Shader
out vec3 pos;
out vec2 uv;

// Camera Vars
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat4 u_ModelMatrix;

uniform vec3 u_CameraPos;

void main()
{
    // 'pos' Is for maps with size 500 - (Should make size factor more dynamic by adding width & height of verts to shader)
    pos = vec3(position.x/500, position.y/500, position.z/500);
    gl_Position = (u_Projection * u_View * u_ModelMatrix) * vec4(position, 1.0);
    uv = texture_coordinates;
}


S|Fragment
#version 330 core

layout(location = 0) out vec4 colour;

// From Vertex Shader
in vec3 pos;
in vec2 uv;

/* Textures */
uniform sampler2D u_texture0;   // Grass
uniform sampler2D u_texture1;   // Dirt

/* Maps */
uniform sampler2D u_map0;       // BlendMap

void main()
{
    vec4 map0 = texture(u_map0, vec2(pos.x, pos.z));


    vec4 tex0 = texture(u_texture0, uv) * map0.b;
    vec4 tex1 = texture(u_texture1, uv) * map0.r;
    
    float amount = 1 - (map0.r + map0.g, map0.b); 

    colour = tex0 + tex1; //vec4(mix(tex0.xyz, tex1.xyz, amount), 1);   
}                 

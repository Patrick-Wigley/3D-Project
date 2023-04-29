S|Vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texture_coordinates;
layout(location = 2) in vec3 normals;
layout(location = 3) in ivec4 boneIDs;
layout(location = 4) in vec4 weights;

// To Fragment Shader
out vec4 pos;
out vec2 uv;
flat out ivec4 frag_boneIds;
out vec4 frag_weights;


// Camera Vars
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat4 u_ModelMatrix;

uniform vec3 u_CameraPos;

const int MAX_BONES = 100;
uniform mat4 u_Bones[MAX_BONES];

void main()                               
{          
    mat4 BoneTransform = u_Bones[boneIDs[0]] * weights[0];
    BoneTransform     += u_Bones[boneIDs[1]] * weights[1];
    BoneTransform     += u_Bones[boneIDs[2]] * weights[2];
    BoneTransform     += u_Bones[boneIDs[3]] * weights[3];
    vec4 local_position = BoneTransform * vec4(position, 1.0);

    gl_Position = (u_Projection * u_View * u_ModelMatrix) * local_position;
    pos = gl_Position;
    uv = texture_coordinates;
    
    frag_boneIds = boneIDs;
    frag_weights = weights;
}                                         


S|Fragment
#version 330 core                         
                                          
layout(location = 0) out vec4 colour;     

// From Vertex Shader
in vec4 pos;
in vec2 uv;
flat in ivec4 frag_boneIds;
in vec4 frag_weights;


// Texture
uniform sampler2D u_texture;
uniform bool u_isTextured;

// Current hard coded
int MAX_BONES = 4;

uniform int u_boneDisplayActive;

void main()                               
{
    // ATM Everything is textured - (add boolean to determine)
    if (u_isTextured)
        colour = texture(u_texture, uv);
    
    else
    {
        for (int i=0; i < MAX_BONES; i++)
        {
            if (frag_boneIds[i] == u_boneDisplayActive)
            {
                if (frag_weights[i] >= .7)
                {
                    // Predominantly active
                    colour = vec4(0,1,0,1);
                }
                else if (frag_weights[i] >= .45)
                {
                    colour = vec4(0,.75,0,1);
                }
                else if (frag_weights[i] >= .1)
                {
                    colour = vec4(0,.50,0,1);
                }
                else
                {
                    // Not active 
                    colour = vec4(0,0,0,1);
                }
            }
        }
        
    }

    
    

    //colour = vec4(.1, .5, .3, 1.0) * pos;
    //colour = vec4(0,0,0, 1.0);
}                                         
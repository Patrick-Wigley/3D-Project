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
out vec3 norms;

// Camera Vars
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat4 u_ModelMatrix;

uniform vec3 u_CameraPos;

const int MAX_BONES = 100;
uniform mat4 u_Bones[MAX_BONES];

void main()                               
{          
    // Need to create a seperate shader for non-animated models
    mat4 BoneTransform = u_Bones[boneIDs[1]] * weights[1];
    BoneTransform     += u_Bones[boneIDs[0]] * weights[0];
    BoneTransform     += u_Bones[boneIDs[2]] * weights[2];
    BoneTransform     += u_Bones[boneIDs[3]] * weights[3];
    vec4 local_position = BoneTransform * vec4(position, 1.0);

    gl_Position = (u_Projection * u_View * u_ModelMatrix) * local_position;
    pos = gl_Position;
    uv = texture_coordinates;
    
    frag_boneIds = boneIDs;
    frag_weights = weights;
    norms = normals;
}                                         


S|Fragment
#version 330 core                         
                                          
layout(location = 0) out vec4 colour;     

// From Vertex Shader
in vec4 pos;
in vec2 uv;
flat in ivec4 frag_boneIds;
in vec4 frag_weights;
<<<<<<<< HEAD:Shaders/Models.glsl
in vec3 norms;
========
>>>>>>>> 3e908c90e5ffd85c9a8d8b6a6f958a821c2e19bd:Shaders/AnimatedModels.glsl

// Texture
uniform sampler2D u_texture;
uniform bool u_isTextured;

// CONSTS
int MAX_BONES = 4;


const vec3 TemplightPos = vec3(0,10,0);

void main()                               
{
    vec3 redColour = vec3(0.1, 0.1, 0.1); 

    float ambientStrength =  .5;
    vec3 ambient = ambientStrength * redColour;
    
    vec3 normals = normalize(norms);
    
    vec3 lightDir = normalize(TemplightPos - pos.xyz);
    
    float diff = max(dot(normals, lightDir), 0.0);
    vec3 diffuse = diff * redColour;

    vec3 result;
    // ATM Everything is textured - (add boolean to determine)
    if (u_isTextured)
    {
        result = vec3(texture(u_texture, uv).xyz);
        result *= (ambient + diffuse);
    }
    else
    {
<<<<<<<< HEAD:Shaders/Models.glsl
        result = (ambient + diffuse) * vec3(.1,.1,1);
    }

    colour = vec4(result, 1.0);
    
    

    //colour = vec4(.1, .5, .3, 1.0) * pos;
    //colour = vec4(0,0,0, 1.0);
========
        colour = vec4(.1, .1, 1, 1);
    }
>>>>>>>> 3e908c90e5ffd85c9a8d8b6a6f958a821c2e19bd:Shaders/AnimatedModels.glsl
}                                         
#include "BackgroundMap.h"
#include <vector>
#include <string.h>


#include <stb_image.h>



/* Creates Skybox VAO - (Done once) & Binds/Activates it*/
const static unsigned int create_skybox_vao()
{
    unsigned int vao;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    return vao;
}




const static unsigned int create_vb()
{
    const float VERTICES[108]
    {
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES), VERTICES, GL_STATIC_DRAW);
    
    return buffer;
};

const static unsigned int create_tb()
{
   
    std::string PREFIX = "3D-Project\\Assets\\CubeMaps\\";
    const std::vector<std::string> FILES
    {
        PREFIX + "right.png",
        PREFIX + "left.png",
        PREFIX + "top.png",
        PREFIX + "bottom.png",
        PREFIX + "front.png",
        PREFIX + "back.png"
    };
    
    unsigned int buffer;
    glGenTextures(1, &buffer);
    glBindTexture(GL_TEXTURE_CUBE_MAP, buffer);


    int width, height, bit_per_pixel;
    for (int i = 0; i < FILES.size(); i++)
    {
        unsigned char* data = stbi_load(FILES[i].c_str(), &width, &height, &bit_per_pixel, 0);

        int flag = NULL;
        if (bit_per_pixel == 3)
            flag = GL_RGB;

        else if (bit_per_pixel == 4)
            flag = GL_RGBA;

        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, flag, GL_UNSIGNED_BYTE, data);
        }
        else
        {
            std::cout << "Couldn't load data from file: " << FILES[i] << std::endl;
        }
        stbi_image_free(data);
    };
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return buffer;
}
 


SkyMap::SkyMap() 
    : shader(Shader()), r_shader(shader.shader)
{
}

void SkyMap::SetUp()
{
    // Shader
    this->shader.SetUp(SHADER_FOLDER_DIR + "CubeMap.glsl");

    // VAO & Buffers
    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);
    unsigned int tb = create_tb();
    this->vbo = create_vb();
    // Attaching Vertex Buffer content to VAO
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

    // Shaders Uniforms
    this->u_view = glGetUniformLocation(this->shader.shader, "u_View");
    this->u_projection = glGetUniformLocation(this->shader.shader, "u_Projection");
    this->u_sampler_skybox = glGetUniformLocation(this->shader.shader, "skybox");
 }

void SkyMap::Draw(const glm::mat4* rotation_matrix, const glm::mat4* projection_matrix)
{
    glDepthMask(GL_FALSE);
    glEnable(GL_TEXTURE_CUBE_MAP);
    
    glBindVertexArray(this->vao);
    shader.Bind();

    glUniformMatrix4fv(this->u_view, 1, false, glm::value_ptr(*rotation_matrix));
    glUniformMatrix4fv(this->u_projection, 1, false, glm::value_ptr(*projection_matrix));


    glDrawArrays(GL_TRIANGLES, 0, 36);


    glBindVertexArray(0);
    

    glDisable(GL_TEXTURE_CUBE_MAP);
    glDepthMask(GL_TRUE);

}

void SkyMap::Update()
{

}
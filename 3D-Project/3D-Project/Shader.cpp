#include "Shader.h"


const int X = 0;
const int Y = 1;
const int Z = 2;



const static struct ShaderCode
{
    std::string v;
    std::string f;
    unsigned int textures_count;
    unsigned int maps_count;
};

/* Read shader files & store Vertex & Fragment shaders as strings */
/* Only works for shaders in format I have put it in e.g. 'S|Vertex' */
static ShaderCode ParseShader(const std::string filename)
{
    static enum class Type
    {
        V = 0, F, NONE
    }type = Type::NONE;

    unsigned int textures_count = 0;
    unsigned int maps_count = 0;

    std::ifstream file(filename);
    // Vertex & Fragment shaders
    std::stringstream shaders[2];

    std::string line;

    while (std::getline(file, line))
    {
        std::string line_ = line;
        if (line.find("S|") != std::string::npos)
        {           
            if (line.find("Vertex") != std::string::npos)
            {
                type = Type::V;
            }
            else
            {
                type = Type::F;
            }
        }
        else
        {
            shaders[(int)type] << line + "\n";

            // NOTE: If a shader has more than one sampler2D, there will be a blendmap
            if (line.find("sampler2D") != std::string::npos)
            {
                
                if (line.find("map") != std::string::npos)
                    maps_count++;
                else
                    textures_count++;
            }
        }
    };

    file.close();
    return { shaders[0].str(), shaders[1].str(), textures_count, maps_count};
};


static unsigned int CompileShader(const std::string& shaderFile, unsigned int type)
{
    unsigned int id = glCreateShader(type);
    const char* src = shaderFile.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);
    
    // Validate Shader
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    // If error found in Shader
    if (result == GL_FALSE)
    {
        int err_txt_len;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &err_txt_len);
        char* err_msg = (char*)alloca(err_txt_len * sizeof(char));
        glGetShaderInfoLog(id, err_txt_len, &err_txt_len, err_msg);
        std::cout << "\nError in " << (type == GL_VERTEX_SHADER ? "Vertex Shader:\n" : "Fragement Shader:\n");
        std::cout << err_msg << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}



static unsigned int CreateShader(std::string& vertexShader, std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(vertexShader, GL_VERTEX_SHADER);
    unsigned int fs = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);

    // Attaching Vertex & Fragement shaders to program
    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);
    glValidateProgram(program);

    // Done with them now initalised & attached/linked to program
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}




// Class Members
Shader::Shader()
{
};

void Shader::SetUp(std::string file_name)
{
    // Shader
    ShaderCode shaders = ParseShader(file_name);
    this->shader = CreateShader(shaders.v, shaders.f);
    this->textures_count = shaders.textures_count;
    this->maps_count = shaders.maps_count;

    // Uniforms
    this->u_modelMatrix = glGetUniformLocation(shader, "u_ModelMatrix");

    // Allocating memory for array of samplers - (BOTH TEXTURES & MAPS)
    this->u_samplers = (int*)malloc(sizeof(int) * this->textures_count);
    
    
    unsigned int index = 0;
    // Getting uniform locations of textures in glsl file
    for (unsigned i = 0; i < this->textures_count; i++)
    {
        std::string variable_name = "u_texture" + std::to_string(i);
        this->u_samplers[i] = glGetUniformLocation(shader, variable_name.c_str());
        index++;
    }

    // Getting uniform locations of maps in glsl file
    for (unsigned int i = 0; i < this->maps_count; i++)
    {
        std::string variable_name = "u_map" + std::to_string(i);
        this->u_samplers[i + index] = glGetUniformLocation(shader, variable_name.c_str());
    }



    this->u_view = glGetUniformLocation(shader, "u_View");
    this->u_projection = glGetUniformLocation(shader, "u_Projection");
    this->u_cam_pos = glGetUniformLocation(shader, "u_CameraPos");

}

// Activate Shader
void Shader::Bind()
{
    glUseProgram(this->shader);
    
};


// Empty
void Shader::AttachUniforms()
{

}


void Shader::UpdateCameraUniforms(Camera* camera)
{

    float* pos = camera->get_pos;
    const glm::mat4* projection = camera->GetProjectionMatrix;
    const glm::mat4* view = camera->GetView;

    glUniformMatrix4fv(this->u_projection, 1, false, glm::value_ptr(*projection));
    glUniformMatrix4fv(this->u_view, 1, false, glm::value_ptr(*view));
    glUniform3f(this->u_cam_pos, pos[X], pos[Y], pos[Z]);
}

// Empty
void Shader::UpdateUniforms()
{

}

void Shader::SetBonesUniform()
{
    //this->m_u_Bones = glGetUniformLocation(this->shader, "u_Bones");
   
    for (unsigned int i = 0; i < MAX_BONES; i++)
    {
        
        std::string uniformName("u_Bones[" + std::to_string(i) + "]");
        
        //printf("\n%s", uniformName.c_str());
        this->m_u_Bones[i] = glGetUniformLocation(this->shader, uniformName.c_str());
    }
}



// Shader Sub-Classes
ObjectShader::ObjectShader()
{
}

void ObjectShader::ExtendedSetUp(std::string file_name)
{
    this->SetUp(file_name);
    
    // Extended uniforms
    this->u_isTextured = glGetUniformLocation(shader, "u_isTextured");
    // Default to True
    glUniform1i(this->u_isTextured, true);

}


void ObjectShader::UpdateIsTexturedUniform(bool isTextured)
{
    glUniform1i(this->u_isTextured, isTextured);
}


TerrainShader::TerrainShader()
{
}
CubeMapShader::CubeMapShader()
{
}






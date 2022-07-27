#include "Shader.h"






const static struct ShaderCode
{
    std::string v;
    std::string f;
};

/* Read shader files & store Vertex & Fragment shaders as strings */
/* Only works for shaders in format I have put it in e.g. 'S|Vertex' */
static ShaderCode ParseShader(const std::string filename)
{
    static enum class Type
    {
        V = 0, F, NONE
    }type = Type::NONE;


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
        }
    };

    file.close();
    return { shaders[0].str(), shaders[1].str() };
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
        std::cout << "Error in " << (type == GL_VERTEX_SHADER ? "Vertex Shader:\n" : "Fragement Shader:\n");
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

    // Shader
    ShaderCode shaders = ParseShader("3D-Project\\Shaders\\main.shader");
    this->shader = CreateShader(shaders.v, shaders.f);
};

// Activate Shader
void Shader::Bind()
{
    glUseProgram(this->shader);
};


// 
void Shader::AttachUniforms()
{

}
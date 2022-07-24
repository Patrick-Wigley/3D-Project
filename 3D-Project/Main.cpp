#include <GL\glew.h>
#include <GLFW/glfw3.h>

// Needed for file reading
#include <string>
#include <sstream>
#include <fstream>

// For debug outputs
#include <iostream>


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



int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "3D Game", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glewInit();


    // Vertex Array Obj
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);



    // VERTICES
    const float VERTICES[]
    {
        -0.5f, -0.5f, // 0
         0.5f, -0.5f, // 1
        -0.5f,  0.5f, // 2
         0.5f,  0.5f  // 3

    };

    
    unsigned int vertexBuffer;
    glGenBuffers(1, &vertexBuffer);                                                                 // Generate
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);                                                    // Bind
    
    //Buffer must be initially binded before making changes to it
    glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), VERTICES, GL_STATIC_DRAW);                  // Pass in data
    
    /*NOTES*/
    // Vertex consists of many attributes such as (Normals, texcoords, col, pos)
    // Stride specifies size of bytes each vertex is to step over each index.
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);


    // Indices
    const unsigned int INDICES[]
    {
        0, 1, 2,
        2, 3, 1
    };

    unsigned int ibo;
    glGenBuffers(1, &ibo);                                                             
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);                                                   
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), INDICES, GL_STATIC_DRAW);


    ShaderCode shaders = ParseShader("3D-Project\\Shaders\\main.shader");
    std::string vs = shaders.v;
    std::string fs = shaders.f;

  

    unsigned int shader = CreateShader(vs, fs);

    glUseProgram(shader);


    float offset = .4f;
    int location = glGetUniformLocation(shader, "u_offset");

    
    glBindVertexArray(0);
    //glUseProgram(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);


        glBindVertexArray(vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        
        if (offset >= 1)
            offset = 0;
        offset += .01f;
        glUniform1f(location, offset);



        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
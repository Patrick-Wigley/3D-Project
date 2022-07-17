#include <GL\glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>

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


    const float VERTICES[6]
    {
        -0.5f, -0.5f,
         0.0f,  0.5f,
         0.5f, -0.5f
    };

    
    unsigned int vertexBuffer;
    glGenBuffers(1, &vertexBuffer);                                                                 // Generate
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);                                                    // Bind
    
    //Buffer must be initially binded before making changes to it
    glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES), VERTICES, GL_STATIC_DRAW);                  // Pass in data
    
    /*NOTES*/
    // Vertex consists of many attributes such as (Normals, texcoords, col, pos)
    // Stride specifies size of bytes each vertex is to step over each index.
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);


    std::string vs =
        "#version 330 core                          \n"
        "                                           \n"
        "layout(location = 0) in vec4 position;     \n"
        "void main()                                \n"
        "{                                          \n"
        "gl_Position = position;                    \n"
        "}                                          \n";
    std::string fs =
        "#version 330 core                          \n"
        "                                           \n"
        "layout(location = 0) out vec4 colour;      \n"
        "void main()                                \n"
        "{                                          \n"
        "colour = vec4(1.0, 0.0, 0.0, 1.0);         \n"
        "}                                          \n";


    unsigned int shader = CreateShader(vs, fs);

    glUseProgram(shader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);


        glDrawArrays(GL_TRIANGLES, 0, 3);



        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
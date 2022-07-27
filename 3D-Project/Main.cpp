#include "GlobalItems.h"
#include "Obj.h"
#include "Camera.h"
#include "Shader.h"



static struct Window_Settings
{
public:
    int mouse_hidden = 0;
    int paused = 0;
}; 
static Window_Settings window_settings;

static void event_updates(GLFWwindow* window)
{

     //Window Settings
     if (glfwGetKey(window, GLFW_KEY_ESCAPE))
     {
         if (window_settings.paused)
         {

         }
     }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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

    



    Obj square = Obj();
    
    Shader shader_obj;
    unsigned int& shader = shader_obj.shader;
   
    glUseProgram(shader);

    // Shader's Uniform Vars
    float offset = .4f;
    int location = glGetUniformLocation(shader, "u_offset");



    // Refresh buffers
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    
    int size[2]{ 0, 0 };
    glfwGetWindowSize(window, &size[0], &size[1]);

    Camera camera = Camera(window, shader, size);


    int u_modelMatrix = glGetUniformLocation(shader, "u_ModelMatrix");
    
    float x = 0, y = 0, z = -10;
    
    glm::mat4 ModelMatrix = glm::rotate(glm::mat4(1), glm::radians(55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    
    float rotation = 0;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);


        square.Bind();
                  
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, nullptr);
        
        if (offset >= 1)
            offset = 0;
        offset += .01f;
        glUniform1f(location, offset);




        rotation += .7;
        ModelMatrix = glm::rotate(glm::mat4(1), glm::radians(rotation), glm::vec3(1.0f, 0.0f, 0.0f));
        camera.Update();
        glUniformMatrix4fv(u_modelMatrix, 1, false, glm::value_ptr(ModelMatrix));



        event_updates(window);

        /* Swap front and back buffers - Swaps out the currently just-used buffer with the one 
            which has been rendered to ready for display */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}







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


/* 
Notes: 
- Currently only using ONE VertexArrayObject. When rendering a model, contents in array are swapped out to instances Vertex 
  Contents.

*/


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

    
    // Predominant VAO
    unsigned int vao;
    // Creating Vertex Array Object & Binding
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);


    Obj square = Obj();
    
    // Predominant Shader
    Shader shader_obj;
    unsigned int& shader = shader_obj.shader;
    glUseProgram(shader);


    // just an universely used colour changer for models 
    float offset = .4f;
    int u_col_offset = glGetUniformLocation(shader, "u_offset");


   



    // Refresh buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    
    int size[2]{ 0, 0 };
    glfwGetWindowSize(window, &size[0], &size[1]);



    Camera camera = Camera(window, shader_obj, size);
    
    
   


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        camera.Update();

        square.update(shader_obj);
        
        

        
        if (offset >= 1)
            offset = 0;
        offset += .01f;
        glUniform1f(u_col_offset, offset);


        // For one square ATM
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, nullptr);


        
       

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







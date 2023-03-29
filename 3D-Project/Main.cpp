#define STB_IMAGE_IMPLEMENTATION
#include "Program.h"


struct Window_Settings
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
     
    
        
}


/* 
Notes: 
- Currently only using a PREDOMINANT VertexArrayObject & Shader. When rendering a model, contents in array are swapped out to instances Vertex 
  Contents.

*/


//    static int DEFAULT_WINDOW_SIZE[2] = { 640, 480 };
static int DEFAULT_WINDOW_SIZE[2] = { 1240, 600 };
GLFWwindow* window; 

int WindowSetUp()
{
    
    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(DEFAULT_WINDOW_SIZE[0], DEFAULT_WINDOW_SIZE[1], "3D Game", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glewInit();

    // Input Mode
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	    
    return 1;
}


int main(void)
{
    
    /* Window */
    if (!WindowSetUp())
        return -1;
 

    Program program(window, 
                    DEFAULT_WINDOW_SIZE
    );
    program.SetUp();
    program.MainLoop();
    
}




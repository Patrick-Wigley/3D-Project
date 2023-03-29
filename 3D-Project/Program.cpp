#include "Program.h"


//Model_NappyGuy nappy_guy;

void Program::MainLoop()
{
    // Entities
    StaticObj square({ 0.0f,  50.0f,  0.0f }, &model_nappy_guy);
    StaticObj square2({ 0.0f, 50.0f, 4.0f },  &model_bin_gus);
    StaticObj square3({ 0.0f, 50.0f, 9.0f },  &model_chest);


    /* Terrains */
    StaticObj terrain({ -0.0f, -0.0, -0.0f }, &model_terrain, true);


    std::vector<BulletObj> bullets{};


    // Refresh buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


    int size[2]{ 0, 0 };
    glfwGetWindowSize(window, &size[0], &size[1]);


    // Other Stuff on stack
    bool triggerPaused = false;


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
       // QueryErrors();
        glEnable(GL_DEPTH_TEST);
        glCullFace(GL_CW);
        glEnable(GL_CULL_FACE);


        /* Updates */
        camera.Update(model_terrain.find_positions_height(camera.get_pos));

        

        // Other shaders
        // Cubemap uses own vao & shader inside its class making it abit of an anomly - (SORT OUT AT SOME POINT)
        cubeMap.Draw(camera.GetRotationMatrix, camera.GetProjectionMatrix);


        // Main VAO Draws
        glBindVertexArray(vao);


        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE
        // Terrain Shader Draw
        (terrain_shader).Bind();
        (terrain_shader).UpdateCameraUniforms(&camera);
        terrain.Draw(terrain_shader, camera);


        // Main Shader Draws
        main_shader.Bind();
        main_shader.UpdateCameraUniforms(&camera);

        square.Draw(main_shader, camera);
        square2.Draw(main_shader, camera);
        square3.Draw(main_shader, camera);




        for (unsigned int bullet = 0; bullet < bullets.size(); bullet++)
        {
            bullets[bullet].Sub_Update();
            bullets[bullet].Draw(main_shader, camera);
        }



        /* Keypress updates */
        if (glfwGetKey(window, GLFW_KEY_F) && !triggerPaused)
        {
            float pos[3] = { *camera.get_pos, camera.get_pos[1], camera.get_pos[2] };

            BulletObj bullet(
                {*camera.get_pos, camera.get_pos[1], camera.get_pos[2] },
                *camera.Get_Rotation,
                &model_nappy_guy
            );

            bullets.push_back(bullet);
            triggerPaused = true;
        }
        else if (!glfwGetKey(window, GLFW_KEY_F))
        {
            triggerPaused = false;
        }




       // QueryErrors();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


        /* Swap front and back buffers - Swaps out the currently just-used buffer with the one
            which has been rendered to ready for display */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
    }

    glDisable(GL_DEPTH_TEST);
    glfwTerminate();
}


 /* ############ PROGRAM SETUP ############### */

int Program::SetUp()
{
    /* Window Size - (Width & Height) */
	this->window_size = (int*)malloc(sizeof(int) * 2);    
    glfwGetWindowSize(this->window, &window_size[0], &window_size[1]);

    /* Skymap */
    this->cubeMap.SetUp();


    /* Predominant VAO */
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    /* Models Setup */
    this->ModelsSetUp();

    /* Shaders */
    main_shader.SetUp(SHADER_FOLDER_DIR + "main.glsl");
    terrain_shader.SetUp(SHADER_FOLDER_DIR + "Terrain.glsl");
    // Note this->CubeMap has its own shader
    


    /* Entity Setup */
    //this->EntitySetUp();
    
    
    return 0;
}



int Program::ModelsSetUp()
{
    // CONSTANT MODELS - (Objects will use these skins if assigned)   

    this->model_nappy_guy.SetUp("NappyGuy");
    this->model_bin_gus.SetUp("BinGus");
    this->model_chest.SetUp("chest");
   
    ///* Terrains */
    this->model_terrain.SetUp();
   
    
    return 0;
}

int Program::WindowSetUp()
{
    // NOT CURRENTLY USED 
    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(640, 480, "3D Game", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glewInit();

    // Input Mode
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
    // Window Size - (Width & Height)
    this->window_size = (int*)malloc(sizeof(int) * 2);    
    glfwGetWindowSize(window, &window_size[0], &window_size[1]);
    
    return 1;
}


int Program::EntitySetUp()
{
    /* ATM, Still got the basic three - (No dynamic arrays of entities): */
    

    // Entities - (Put just before main loop if continue to use these).
    //StaticObj square({ 0.0f,  50.0f,  0.0f }, &this->model_nappy_guy);
    //StaticObj square2({ 0.0f, 50.0f, 4.0f },  &this->model_bin_gus);
    //StaticObj square3({ 0.0f, 50.0f, 9.0f },  &this->model_chest);
    //

    ///* Terrains */
    //StaticObj terrain({ -0.0f, -0.0, -0.0f }, &this->model_terrain, true);



    return 0;
}



/* ############ SUB-FUNCTIONS ############### */

void Program::QueryErrors()
{ 
    // Push out errors from list & display
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR)
    {
        std::cout << "\n" << "[ERROR LIST]: " << error;
    }
    
}



/* ############ CONSTRUCTORS ############### */

Program::Program(Window* window, int DEFAULT_WINDOW_SIZE[])
    : window(window),
    // Have to setup window & stuff before doing these:
    main_shader(Shader()), terrain_shader(Shader()),
      camera(Camera(window, DEFAULT_WINDOW_SIZE))
{

}

Program::~Program()
{
    //delete this->model_nappy_guy, \
    //       this->model_bin_gus,   \
    //       this->model_chest;
}
#include "Program.h"


void Program::MainLoop()
{
    /* Old Model_Own usage */ 
    // Entities
    //StaticObj square({ 0.0f,  50.0f,  0.0f }, &model_nappy_guy);

    // Terrains
    TerrainObj terrain({ -0.0f, -0.0, -0.0f }, &m_Model_Terrain, true);

    /* New Model usage */
    EntityObj entity({ 30.0f, 42.7f,  35.0f }, &m_Model_Entity, true);
    EntityObj dancingGuy({ 50.0f, 42.7f, 35.0f }, &m_Model_DancingGuy, true);
    EntityObj NappyGuy({ 0.0f,  50.0f,  0.0f }, &m_Model_NonAnimatedModel, true);



    std::vector<DynamicObj> bullets{};


    // Refresh buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);



    // Other Stuff on stack
    bool triggerPaused = false;



    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glCullFace(GL_CW);
        glEnable(GL_CULL_FACE);


        /* Updates */
        m_Camera.Update(m_Model_Terrain.find_positions_height(m_Camera.get_pos));



        // Other shaders
        // Cubemap uses own vao & shader inside its class making it abit of an anomly - (SORT OUT AT SOME POINT)
        m_CubeMap.Draw(m_Camera.GetRotationMatrix, m_Camera.GetProjectionMatrix);



        /* Models Shader Draws */

        // Animated Models
        m_Shader_AnimModels.Bind();
        m_Shader_AnimModels.UpdateCameraUniforms(&m_Camera);
        m_Shader_AnimModels.UpdateIsTexturedUniform(false);
       
       
        // Assimp Model VAO Draw Calls
        m_Shader_AnimModels.UpdateIsTexturedUniform(true);
        float ProgramRunTime = (float)(this->GetCurrentTimeInTicks() - this->m_ProgramStartTimeInMill) / 1000.0f;
        
        entity.SubDraw(m_Shader_AnimModels, m_Camera, ProgramRunTime);
        dancingGuy.SubDraw(m_Shader_AnimModels, m_Camera, ProgramRunTime);

        
       
        // Non-Animated Models
        m_Shader_PlainModels.Bind();
        m_Shader_PlainModels.UpdateCameraUniforms(&m_Camera);
        m_Shader_PlainModels.UpdateIsTexturedUniform(true);
        NappyGuy.SubDraw(m_Shader_PlainModels, m_Camera);


        m_Shader_PlainModels.UpdateIsTexturedUniform(false);
        for (unsigned int bullet = 0; bullet < bullets.size(); bullet++)
        {
            bullets[bullet].Sub_Update();
            bullets[bullet].SubDraw(m_Shader_PlainModels, m_Camera);
        }

        // Main VAO Draws
        glBindVertexArray(m_PredominantVAO);

        // Terrain Shader Draw
        (m_Shader_Terrain).Bind();
        (m_Shader_Terrain).UpdateCameraUniforms(&m_Camera);
        terrain.SubDraw(m_Shader_Terrain, m_Camera);



        /* Keypress updates */
        if (glfwGetKey(window, GLFW_KEY_F) && !triggerPaused)
        {
            float pos[3] = { *m_Camera.get_pos, m_Camera.get_pos[1], m_Camera.get_pos[2] };

            DynamicObj bullet(
                {*m_Camera.get_pos, m_Camera.get_pos[1], m_Camera.get_pos[2] },
                *m_Camera.Get_Rotation,
                &m_Model_EnergyBall, true
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

    printf("\nCurrent Assimp Version: %u\n", aiGetVersionMajor());

    /* Skymap */
    // Uses it's own VAO
    this->m_CubeMap.SetUp();



    /* Models Setup */
    this->ModelsSetUp();

    /* Shaders */
    main_shader.ExtendedSetUp(SHADER_FOLDER_DIR + "Models.glsl");
    main_shader.SetBonesUniform();
    terrain_shader.SetUp(SHADER_FOLDER_DIR + "Terrain.glsl");
    // Note this->CubeMap has its own shader
    


    int size[2]{ 0, 0 };
    glfwGetWindowSize(window, &size[0], &size[1]);

    // Set appliations start time
    this->m_ProgramStartTimeInMill = GetCurrentTimeInTicks();

    return 0;
}



int Program::ModelsSetUp()
{
    /* Assimp Loaded Models here */
    // These models use their own VAO
    // 
    m_Model_Entity.LoadModel("Corkscrew Evade 60fps.fbx", .05);
    m_Model_DancingGuy.LoadModel("Character1B.fbx", .011);
    m_Model_NonAnimatedModel.LoadModel("NappyGuy.obj", 4);
    m_Model_EnergyBall.LoadModel("EnergyBall.obj", 1);
    //m_ModelEntity.LoadModel("Jumping Down.fbx", 3);
    //m_ModelEntity.LoadModel("boblampclean.md5mesh", .321);
    //m_ModelEntity.LoadModel("Hip Hop Dancing.dae", 10.91);
    //m_ModelCharacter.LoadModel("Medical worker.obj");

    /* Predominant VAO */
    glGenVertexArrays(1, &m_PredominantVAO);
    glBindVertexArray(m_PredominantVAO);
    // CONSTANT MODELS - (Objects will use these skins if assigned)   
    //this->model_nappy_guy.SetUp("NappyGuy");
    this->model_energy_ball.SetUp("EnergyBall");

    ///* Terrains */
    this->m_Model_Terrain.SetUp();




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
    // Entities are currently created within rendering time. Some entities will not have a constant lifetime 
    // - (some entities will be removed and created constantly during games run time)
    return 0;
}



/* ############ RUN-TIME SUB FUNCTIONS ############### */

void Program::QueryErrors()
{ 
    // Push out errors from list & display
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR)
    {
        std::cout << "\n" << "[ERROR LIST]: " << error;
    }
    
}

float Program::GetCurrentTimeInTicks()
{
    // Program in current debugging development environment is WIN32
#ifdef _WIN32
    // But this function recommends using GetTickCount64
    return GetTickCount64();
#endif
    return NULL;
};


/* ############ CONSTRUCTORS ############### */

Program::Program(Window* window, int DEFAULT_WINDOW_SIZE[])
    : window(window),
    // Have to setup window & stuff before doing these:
    m_Shader_AnimModels(AnimatedModelShader()), m_Shader_Terrain(TerrainShader()),
    m_Camera(Camera(window, DEFAULT_WINDOW_SIZE)), window_size(DEFAULT_WINDOW_SIZE)
{

}

Program::~Program()
{
    //delete this->model_nappy_guy, \
    //       this->model_bin_gus,   \
    //       this->model_chest;
}
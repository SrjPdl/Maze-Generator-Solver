#include "Application.h"

Application::Application()
	:WINDOW_WIDTH(1024),
	 WINDOW_HEIGHT(710),
	 WINDOW_POSITION(glm::vec2(100,30)),
     graphWidth(8),
     graphHeight(8),
     graphWidthSlider(8),
     graphHeightSlider(8),
     isGeneratePressed(false),
     generatorSpeed(500),
     solverSpeed(500)

{
    startNodeX = 1;
    startNodeY = 1;
    endNodeX = 1;
    endNodeY = 1;
    projection = glm::ortho<float>(0.f, WINDOW_WIDTH, WINDOW_HEIGHT,0.f, -1.f, 1.f);

    glm::vec4 vp = glm::vec4(-50.f, -50.f, 0.f, 1.f);
    auto res = projection * vp;
	setup();
}

Application::~Application()
{
}


void Application::setup()
{    
    window.create("MazeGeneratorSolver", WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_POSITION);


    /*Initialize GLEW*/
    if (glewInit() != GLEW_OK)
    {
        std::cout << "ERROR::FAILED_TO_INITIALIZE_GLEW" << std::endl;
        glfwTerminate();
    }
    glewExperimental = GL_TRUE;

    
    //creating Imgui context
    ImGui::CreateContext();

    //Imgui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window.getWindow(), true);
    const char* glsl_version = "#version 330";
    ImGui_ImplOpenGL3_Init(glsl_version);
    

    //Setting buffer swap interval
    glfwSwapInterval(1);

    //creating shader
    shader.create("vertexShader.glsl", "fragmentShader.glsl");
}
void Application::guiSetup()
{
    ImGui::Begin("Maze Settings");

    
    ImGui::BulletText("Generation Settings");
    ImGui::SliderInt("Maze Width", &graphWidthSlider, 4, 25);
    ImGui::SliderInt("Maze Height", &graphHeightSlider, 4, 20);
    ImGui::Text("Number of nodes = %d", graphHeightSlider * graphWidthSlider);
    ImGui::SliderInt("Generation Speed", &generatorSpeed,1,500);

    if (ImGui::Button("Generate Maze"))                            
        isGeneratePressed = true;

    ImGui::NewLine();
    ImGui::BulletText("Solver Settings");
    ImGui::Text("Start Node");
    ImGui::SliderInt("Start X", &startNodeX, 0, graphWidth - 1);
    ImGui::SliderInt("Start Y", &startNodeY, 0, graphHeight - 1);

    ImGui::Text("End Node");
    ImGui::SliderInt("End X", &endNodeX, 0, graphWidth - 1);
    ImGui::SliderInt("End Y", &endNodeY, 0, graphHeight - 1);
    ImGui::SliderInt("Solving Speed", &solverSpeed, 1, 500);
    if (ImGui::Button("Solve Maze"))
        isSolvePressed = true;

    ImGui::End();

}


void Application::update()
{   

    bool isMazeGenerated = false;
    bool isSolved = false;
    /*Pre-update settings*/
    MazeGenerator mazeGenerator(graphWidth,graphHeight);
    MazeSolver mazeSolver(mazeGenerator);
   
    Camera2D camera(shader.getProgram());
    glm::mat4 view(1.f);
    view = glm::lookAt(glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));


    glm::vec3 translate(0.f);

    /*main loop*/
    while (window.isOpen())
    {
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (glfwGetKey(window.getWindow(), GLFW_KEY_ESCAPE))
        {
            glfwSetWindowShouldClose(window.getWindow(), GL_TRUE);
        }


        //updating projection matrix to account for window resize
        glm::vec2 viewportSize = window.getViewportSize();
        projection = glm::ortho<float>(0.f, viewportSize.x, viewportSize.y, 0.f, -1.f, 1.f);

        /* Poll for and process events */
        glfwPollEvents();

        /*Imgui Code */
        guiSetup();

        /* Render here */
        glClearColor(0.219, 0.219, 0.219, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);


        //updating camera
        camera.update(&view, &projection);

        if (isGeneratePressed)
        {
            //isSolved = false;
            oldGraphHeight = graphHeight;
            graphWidth = graphWidthSlider;
            graphHeight = graphHeightSlider;
            mazeGenerator.generate(shader.getProgram(),graphWidth,graphHeight,oldGraphHeight);

            //resetting nodes
            startNodeX = 1;
            startNodeY = 1;
            endNodeX = 1;
            endNodeY = 1;
            mazeSolver.resetNode(Node(startNodeY, startNodeX), Node(endNodeY, endNodeX));

            mazeSolver.reset();
            isGeneratePressed = false;
        }
        

        mazeGenerator.render(shader.getProgram(),generatorSpeed);

        
        if (mazeGenerator.isMatrixSet() && isSolvePressed)
        {
            mazeSolver.solve(Node(startNodeY,startNodeX),Node(endNodeY,endNodeX));
           // isSolved = true;
            isSolvePressed = false;
            
        }
       
        mazeSolver.render(shader.getProgram(),Node(startNodeY,startNodeX),Node(endNodeY,endNodeX),solverSpeed);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        /* Swap front and back buffers */
        glfwSwapBuffers(window.getWindow());

    }

    //Cleanup Imgui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

Application& Application::Get()
{
    static Application instance;
    return instance;
}
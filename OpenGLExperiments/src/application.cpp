#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "GLLogCall.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Renderer.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"

#include "tests/Test.h"
#include "tests/TestClearColor.h"
#include "tests/TestTexture.h"
#include "tests/TestBatchRendering.h"

int main(void) {
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Use the core OpenGL so we have to use vertex arrays
       vs getting a default one from compat */
    /**glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1920, 1080, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    /* Call after making valid OpenGL context */
    if (glewInit() != GLEW_OK) {
        std::cout << "ERROR: glewInit failed!" << std::endl;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;
    {  
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330");
        ImGui::StyleColorsDark();

        // my laptop has a high dpi so we need to set the scale higher
        io.FontGlobalScale = 3.0f;

        test::Test* currentTest = nullptr;
        test::MainMenu* mainMenu = new test::MainMenu(currentTest);
        currentTest = mainMenu;

        mainMenu->RegisterTest<test::TestClearColor>("Clear Color");
        mainMenu->RegisterTest<test::TestTexture>("Texture Test");
        mainMenu->RegisterTest<test::TestBatchRendering>("2D Batch Rendering");

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            if (currentTest) {
                currentTest->OnUpdate(0.0f);
                currentTest->OnRender();
                ImGui::Begin("Test");
                if (currentTest != mainMenu && ImGui::Button("<-")) {
                    delete currentTest;
                    currentTest = mainMenu;
                }
                currentTest->OnImGuiRender();
                ImGui::End();
            }


            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

        if (currentTest != mainMenu) {
            delete mainMenu;
        }
        delete currentTest;      
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();

    return 0;
}
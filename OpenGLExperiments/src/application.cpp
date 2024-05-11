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
        /* Triangle vertices */
        //const int POSITION_COUNT = 12;
        float positions[] = {
            100.0f, 100.0f, 0.0f, 0.0f,  // 0 - index count for index buffer
            200.0f, 100.0f, 1.0f, 0.0f,  // 1
            200.0f, 200.0f, 1.0f, 1.0f,  // 2
            100.0f, 200.0f, 0.0f, 1.0f   // 3
        };

        /* Index buffer for drawing the same points in
           the vertex buffer multiple times.*/
        unsigned int indicies[] = {
            0, 1, 2,
            2, 3, 0
        };

        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCall(glEnable(GL_BLEND));

        /* Vertex array creation! */
        VertexArray va;

        /* Create the buffer! */
        VertexBuffer vb(positions, 4 * 4 * sizeof(float));

        /* Enable our attribute pointer */
        VertexBufferLayout layout;
        // vertex layout
        layout.Push<float>(2);
        // texture coordinates
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        /* This section is for the index buffer */
        IndexBuffer ib(indicies, 6);

        glm::mat4 projection = glm::ortho(0.0f, 1920.0f, 0.0f, 1080.0f, -1.0f, 1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(100, 100, 0));

        glm::mat4 mvp = projection * model * view; 

        Shader shader("res/shaders/basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);
        shader.SetUniformMat4f("u_MVP", mvp);

        Texture texture("res/texture/fireplace.png");
        texture.Bind();
        // tells the shader which slot to draw
        shader.SetUniform1i("u_Texture", 0);

        Renderer renderer;

        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330");
        ImGui::StyleColorsDark();

        // my laptop has a high dpi so we need to set the scale higher
        io.FontGlobalScale = 3.0f;

        glm::vec3 translateA(glm::vec3(100, 100, 0));
        glm::vec3 translateB(glm::vec3(200, 100, 0));
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            renderer.Clear();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();


            {     
                ImGui::SliderFloat2("Translate A", &translateA.x, 0.0f, 1920.0f);  
                ImGui::SliderFloat2("Translate B", &translateB.x, 0.0f, 1080.0f);
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            }

            //cozy fireplace A
            {
                model = glm::translate(glm::mat4(1.0f), translateA);
                mvp = projection * model * view;
                // we bind the shader twice here because renderer.Draw does it too
                // which is no good.
                shader.Bind();
                shader.SetUniformMat4f("u_MVP", mvp);
                renderer.Draw(va, ib, shader);
            }

            //cozy fireplace B
            {
                model = glm::translate(glm::mat4(1.0f), translateB);
                mvp = projection * model * view;
                shader.Bind();
                shader.SetUniformMat4f("u_MVP", mvp);
                renderer.Draw(va, ib, shader);
            }

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();

    return 0;
}
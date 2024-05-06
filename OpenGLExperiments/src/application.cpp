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
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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
            -0.5f, -0.5f, 0.0f, 0.0f,  // 0 - index count for index buffer
             0.5f, -0.5f, 1.0f, 0.0f,  // 1
             0.5f,  0.5f, 1.0f, 1.0f,  // 2
            -0.5f,  0.5f, 0.0f, 1.0f   // 3
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

        Shader shader("res/shaders/basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);

        Texture texture("res/texture/fireplace.png");
        texture.Bind();
        // tells the shader which slot to draw
        shader.SetUniform1i("u_Texture", 0);

        Renderer renderer;

        float r = 0.0f;
        float g = 0.0f;
        float b = 0.0f;
        float increment = 0.05f;
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            renderer.Clear();

            /* Call the uniform in the fragment shader */
            shader.SetUniform4f("u_Color", r, g, b, 1.0f);
            renderer.Draw(va, ib, shader);

            if (r < 1.0f) {
                r += increment;
            }
            else if (g < 1.0f) {
                g += increment;
            }
            else if (b < 1.0f) {
                b += increment;
            }
            else {
                increment = -0.05f;
            }

            if (increment == -0.05f) {
                if (r > 0.0f) {
                    r += increment;
                }
                else if (g > 0.0f) {
                    g += increment;
                }
                else if (b > 0.0f) {
                    b += increment;
                }
                else {
                    increment = 0.05f;
                }
            }

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
}
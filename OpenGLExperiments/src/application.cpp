#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* func, const char* file, int line) {
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGL Error] (" << error << "): " 
            << func << " "
            << file << ": " 
            << line << std::endl;
        return false;
    }
    return true;
}

struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath) {
    std::ifstream stream(filepath);

    enum class ShaderType {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
            }
        }
        else {
            ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
}

/* Abstracted because compilation is the same between shaders */
static unsigned int CompileShader(unsigned int type, const std::string& source) {
    /* "A shader object is used to maintain the 
        source code strings that define a shader."
        There are 5 types of shaders which can be
        found in the documentation.
        https://docs.gl/gl4/glCreateShader */
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    /* Establish the source of an empty shader */
    GLCall(glShaderSource(id, 1, &src, NULL););
    GLCall(glCompileShader(id););
    
    int result;
    /* Get the result of our shader compilation */
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile "
            << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") 
            << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

/* Take in source code of shaders, combine, and compile */
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    /* "[glCreateProgram] provides a mechanism to 
        specify the shaderobjects that will be linked 
        to create a program." 
        https://docs.gl/gl4/glCreateProgram */
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
    
    /* Attach the shaders to a program */
    GLCall(glAttachShader(program, vs););
    GLCall(glAttachShader(program, fs););
    /* Link the shaders attached to the program */
    GLCall(glLinkProgram(program););
    /* Check to see if execuatbles can execute */
    GLCall(glValidateProgram(program););

    // apparently we need to detach shaders here

    /* Clean up shader intermediates */
    GLCall(glDeleteShader(vs););
    GLCall(glDeleteShader(fs););

    return program;
}

int main(void) {
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

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

    /* Triangle vertices */
    //const int POSITION_COUNT = 12;
    float positions[] = {
        -0.5f, -0.5f,   // 0 - index count for index buffer
         0.5f, -0.5f,   // 1
         0.5f,  0.5f,   // 2
        -0.5f,  0.5f,   // 3
    };

    /* Index buffer for drawing the same points in
       the vertex buffer multiple times.*/ 
    unsigned int indicies[] = {
        0, 1, 2,
        2, 3, 0
    };

    /* Create the buffer! */
    /* Our buffer id */
    unsigned int buffer;
    /* Generate x buffer(s) given an id */
    GLCall(glGenBuffers(1, &buffer););
        /* What kind of buffer is being generated */
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer););
    /* Buffer our positions data.
        https://docs.gl/gl4/glBufferData */
    GLCall(glBufferData(GL_ARRAY_BUFFER,
        6 * 2 * sizeof(float),
        &positions,
        GL_STATIC_DRAW););

    /* This section is for the index buffer */
    unsigned int ibo;
    GLCall(glGenBuffers(1, &ibo););
        /* We use GL_ELEMENT_ARRAY_BUFFER instead
           of GL_ARRAY_BUFFER here */
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo););
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        6 * sizeof(unsigned int),
        &indicies,
        GL_STATIC_DRAW););

    /* Enable our attribute pointer */
    GLCall(glEnableVertexAttribArray(0););
    /* This defines what our positions array means */
    /* https://docs.gl/gl4/glVertexAttribPointer */
    GLCall(glVertexAttribPointer(0, // where we start
        2,                   // how many elements per vertex
        GL_FLOAT,            // what kind of data is in there
        GL_FALSE,            // does the data need to be normalized
        sizeof(float) * 2,   // how big is our vertex (our *stride*)
        (const void*) 0);)   // how many bytes in do we start reading this attrib

    ShaderProgramSource source = ParseShader("res/shaders/basic.shader");
    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    GLCall(glUseProgram(shader););

    GLCall(int location = glGetUniformLocation(shader, "u_Color");)
    ASSERT(location != -1);
    GLCall(glUniform4f(location, 0.2f, 0.3f, 0.8f, 1.0f));

    float r = 0.0f;
    float g = 0.0f;
    float b = 0.0f;
    float increment = 0.05f;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        GLCall(glUniform4f(location, r, g, b, 1.0f));
        /* Use the index buffer to draw the triangles */
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr););

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

    GLCall(glDeleteProgram(shader););

    glfwTerminate();
    return 0;
}
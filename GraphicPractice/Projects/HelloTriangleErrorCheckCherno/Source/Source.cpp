#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

// structures
struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

// MACRO for debugger
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

// function declarations
static bool GLLogCall(const char* function, const char* file, int line);
static void GLClearError();
static ShaderProgramSource ParseShader(const std::string& file);
static int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
static unsigned int CompileShader(unsigned int type, const std::string& source);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // positions
    // ---------
    const int positionsQuant1 = 6;
    float firstTriangle[positionsQuant1] =
    {
        // first triangle
         -0.0f, -0.5f,  // right
         -0.45f, 0.5f,  // top 
         -0.9f, -0.5f,  // left 
    };

    const int positionsQuant2 = 6;
    float secondTriangle[positionsQuant2] =
    {
         // second triangle
          0.0f, -0.5f,   // left
          0.9f, -0.5f,   // right
          0.45f, 0.5f,   // top
    };

    unsigned int VBOs[2], VAOs[2];
    // Generating vertex arrays and buffers
    GLCall(glGenVertexArrays(2, VAOs));
    GLCall(glGenBuffers(2, VBOs));
    // first triangle setup
    // --------------------
    GLCall(glBindVertexArray(VAOs[0]));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle) * 6, firstTriangle, GL_STATIC_DRAW));
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (const void*)0));
    GLCall(glEnableVertexAttribArray(0));
    GLCall(glBindVertexArray(0));
    // second triangle setup
    // --------------------
    GLCall(glBindVertexArray(VAOs[1]));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle) * 6, secondTriangle, GL_STATIC_DRAW));
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (const void*)0));
    GLCall(glEnableVertexAttribArray(0));
    GLCall(glBindVertexArray(0));

    // wireframe polygons
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // shader compile
    // ---------------
    ShaderProgramSource source = ParseShader("Resources/Shaders/Basic.shader");
    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------ 
        glClear(GL_COLOR_BUFFER_BIT);

        // using shader for triangles
        GLCall(glUseProgram(shader));
        // draw first triangle
        GLCall(glBindVertexArray(VAOs[0]));
        GLCall(glDrawArrays(GL_TRIANGLES, 0, 3));

        // draw second triangle
        GLCall(glBindVertexArray(VAOs[1]));
        GLCall(glDrawArrays(GL_TRIANGLES, 0, 3));

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

      // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);
    glDeleteProgram(shader);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// error management
static void GLClearError()
{
    // clears all errors
    while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line)
{
    // until we have an error
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << "): " << function
            << " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}

// Parses shader file
static ShaderProgramSource ParseShader(const std::string& filepath)
{
    // open file
    std::ifstream stream(filepath);
    // enum class for defining type of shader we work with
    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };
    // variables for work
    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    // parsing file
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                // set mode to vertex
                type = ShaderType::VERTEX;
            }
            else
                if (line.find("fragment") != std::string::npos)
                {
                    // set mode to fragment
                    type = ShaderType::FRAGMENT;
                }
        }
        else
        {
            ss[(int)type] << line << "\n";
        }
    }
    return { ss[0].str(), ss[1].str() };
}

// Creates shader
static int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    // Creating program
    unsigned int program = glCreateProgram();
    // Creating vertex shader
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    // Creating fragment shader
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    // Attach to our recent program
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);
    // Clean shader
    glDeleteShader(vs);
    glDeleteShader(fs);
    // Return program
    return program;
}

// Compiles shader
static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    // Error handling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        // allocates memory on stack
        char* message = new char[length];
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " <<
            (type == GL_VERTEX_SHADER ? "vertex" : "fragment") <<
            " shader!" << std::endl;
        std::cout << message << std::endl;
        // deleting shader
        glDeleteShader(id);
        delete[] message;
        return 0;
    }

    return id;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
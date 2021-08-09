#include "Shader.h"
#include <GLFW/glfw3.h>

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
    const int positionsQuant1 = 15;
    float firstTriangle[positionsQuant1] =
    {
        // left triangle
        // position    // color
         -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, // left 
         0.0f, -0.5f, 0.0f, 1.0f, 0.0f,  // right 
         -0.45f, 0.5f, 1.0f, 0.0f, 1.0f, // top 
    };

    const int positionsQuant2 = 15;
    float secondTriangle[positionsQuant2] =
    {
        // right triangle
        // position    // color
          0.0f, -0.5f, 1.0f, 0.0f, 0.0f, // left 
          0.9f, -0.5f, 0.0f, 1.0f, 0.0f,  // right 
          0.45f, 0.5f, 0.0f, 0.0f, 1.0f // top 
    };

    unsigned int VBOs[2], VAOs[2];
    // Generating vertex arrays and buffers
    GLCall(glGenVertexArrays(2, VAOs));
    GLCall(glGenBuffers(2, VBOs));
    // first triangle setup
    // --------------------
    GLCall(glBindVertexArray(VAOs[0]));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle) * positionsQuant1, firstTriangle, GL_STATIC_DRAW));
    // enabling position attribute
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void*)0));
    GLCall(glEnableVertexAttribArray(0));
    // enabling color attribute
    GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void*)(2 * sizeof(float))));
    GLCall(glEnableVertexAttribArray(1));
    // unbind
    GLCall(glBindVertexArray(0));

    // second triangle setup
    // --------------------
    GLCall(glBindVertexArray(VAOs[1]));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle) * positionsQuant2, secondTriangle, GL_STATIC_DRAW));
    // enabling position attribute
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void*)0));
    GLCall(glEnableVertexAttribArray(0));
    // enabling color attribute
    GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void*)(2 * sizeof(float))));
    GLCall(glEnableVertexAttribArray(1));
    // unbind
    GLCall(glBindVertexArray(0));

    // wireframe polygons
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // shader compile
    // ---------------
    Shader shader("Resources\\Shaders\\vertex.shader", "Resources\\Shaders\\fragment.shader");
    // number of available attributes
    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

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
        glClearColor(0.5, 0.5, 0.5, 1);

        // using shader for triangles
        GLCall(shader.use());
        shader.setFloat("xOffset", sin(glfwGetTime() / 0.3f));
        shader.setFloat("yOffset", sin(glfwGetTime() / 0.3f));
        shader.setFloat("zOffset", sin(glfwGetTime() / 0.3f));

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
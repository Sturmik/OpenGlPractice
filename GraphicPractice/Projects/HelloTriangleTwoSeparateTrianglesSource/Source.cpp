#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

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
         -0.9f, -0.5f,  // left 
         -0.0f, -0.5f,  // right
         -0.45f, 0.5f,  // top 
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
    glGenVertexArrays(2, VAOs);
    glGenBuffers(2, VBOs);
    // first triangle setup
    // --------------------
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle) * 6, firstTriangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (const void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    // second triangle setup
    // --------------------
    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle) * 6, secondTriangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (const void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    // wireframe polygons
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // shader compile
    // ---------------
    std::string vertexShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) in vec4 position;\n"
        "\n"
        "void main()\n"
        "{\n"
        "   gl_Position = position;\n"
        "}\n";
    std::string fragmentShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) out vec4 color;\n"
        "\n"
        "void main()\n"
        "{\n"
        "   color = vec4(1.0, 0.0, 0.0, 1.0);\n"
        "}\n";
        unsigned int shader = CreateShader(vertexShader, fragmentShader);

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

        // using shader
        glUseProgram(shader);

        // draw first triangle
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // draw second triangle
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

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

#include <stdio.h>
#include <GLFW/glfw3.h>

int main(void)
{
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

    printf(
        "OpenGL Version:   %s\n"
        "OpenGL Renderer:  %s\n"
        "OpenGL Version:   %s\n"
        "GLSL Version:     %s\n"
        , glGetString(GL_VENDOR)
        , glGetString(GL_RENDERER)
        , glGetString(GL_VERSION)
        , glGetString(GL_SHADING_LANGUAGE_VERSION)
    );

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glColor3f(1, 1, 1);
        glVertex3f(0, 0, -1);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

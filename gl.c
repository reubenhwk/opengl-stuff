
#include "mrfont.h"
#include "model.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

#include <GLFW/glfw3.h>

void model_draw_text(int x, int y, char const *str)
{
    mrfont_init();
    mrfont_string_draw(x, y, str);
}

void model_draw(struct model const * model)
{
    for (int i = 0; i < model->text_count; ++i) {
        struct model_text * text = &model->text[i];
        model_draw_text(text->x, text->y, text->str);
    }
}

int main(void)
{
    struct model * model = new_model();

    model_insert_text(model, 10, 200, "Hello World!!!");
    model_insert_text(model, 10, 150, "Hello World");
    model_insert_text(model, 10, 100, "Hello World");

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

    glClearColor(0, 0, 0, 0);

    struct timespec start, end;

    int frames = 0;
    clock_gettime(CLOCK_MONOTONIC, &start);
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        model_draw(model);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();

        ++frames;

        clock_gettime(CLOCK_MONOTONIC, &end);
        double stopwatch = 
            ((end.tv_sec * 1000 * 1000 * 1000.0 + end.tv_nsec) -
            (start.tv_sec * 1000 * 1000 * 1000.0 + start.tv_nsec)) /
            (1000 * 1000 * 1000.0) ;

        if (stopwatch >= 1) {
            printf("%f fps\n", frames / stopwatch);
            start = end;
            frames = 0;
        } 
    }

    glfwTerminate();

    return 0;
}

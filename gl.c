
#include "mrfont.h"
#include "model.h"
#include "draw.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

#include <GLFW/glfw3.h>

int main(void)
{
    struct model * text1 = new_model();
    model_insert_text(text1, 10, 175, "BRAVO");

    struct model * text2 = new_model();
    model_insert_text(text2, 10, 200, "Hello World!!!");
    model_insert_text(text2, 10, 150, "Hello World");

    struct model * model = new_model();
    model_insert_submodel(model, text1);
    model_insert_submodel(model, text2);
    model_insert_text(model, 10, 100, "Hello World");

    struct model * lines1 = new_model();
    float z = .5;
    struct tuple3f points1[] = {
        {.xyz = {0, 0, z}},
        {.xyz = {1, 1, z}},
        {.xyz = {0, 1, z}},
        {.xyz = {1, 0, z}},
        {.xyz = {0, 0, z}},
    };
    model_insert_polyline(lines1, points1, 5);
    struct ortho_projection projection = {-1.1, 1.1, -1.1, 1.1, -1.1, 1.1};
    model_set_projection(lines1, &projection);
    model_set_transform(lines1);
    model_insert_submodel(model, lines1);
    model_set_color(lines1, (struct tuple3f) {.rgb = {.r = 1, .g = 0, .b = 1}});

    struct model * lines2 = new_model();
    struct tuple3f points2[] = {
        {.xyz = {0, -1, z}},
        {.xyz = {1, 0, z}},
        {.xyz = {0, 0, z}},
        {.xyz = {1, -1, z}},
        {.xyz = {0, -1, z}},
    };
    model_insert_polyline(lines2, points2, 5);
    model_set_projection(lines2, &projection);
    model_set_transform(lines2);
    model_insert_submodel(model, lines2);
    model_set_color(lines2, (struct tuple3f) {.rgb = {.r = 0, .g = 1, .b = 1}});

    struct model * lines3 = new_model();
    struct tuple3f points3[] = {
        {.xyz = {-1, -1, z}},
        {.xyz = {0, 0, z}},
        {.xyz = {-1, 0, z}},
        {.xyz = {0, -1, z}},
        {.xyz = {-1, -1, z}},
    };
    model_insert_polyline(lines3, points3, 5);
    model_set_projection(lines3, &projection);
    model_set_transform(lines3);
    model_insert_submodel(model, lines3);
    model_set_color(lines3, (struct tuple3f) {.rgb = {.r = 1, .g = 1, .b = 0}});

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
        glfwWaitEvents();

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

    free_model(model);

    return 0;
}

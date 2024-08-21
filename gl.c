
#include "mrfont.h"
#include "model.h"
#include "draw.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

#include <GLFW/glfw3.h>

static int window_x = 640;
static int window_y = 480;
static double mouse_x = 0;
static double mouse_y = 0;

void do_select(int x, int y)
{
    float z;
    glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z);
    float r, g, b;
    glReadPixels(x, y, 1, 1, GL_RED, GL_FLOAT, &r);
    glReadPixels(x, y, 1, 1, GL_GREEN, GL_FLOAT, &g);
    glReadPixels(x, y, 1, 1, GL_BLUE, GL_FLOAT, &b);
    printf("select: (%d, %d, %g) (%g, %g, %g)\n", x, y, z, r, g, b);
}

static void mouse_pos_callback(GLFWwindow *window, double x, double y)
{
    mouse_x = x - 1;
    mouse_y = window_y - y - 1;
}

static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            do_select(mouse_x, mouse_y);
        } else if (action == GLFW_RELEASE) {
        } else {
        }
    } else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
        } else if (action == GLFW_RELEASE) {
        } else {
        }
    } else if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
        if (action == GLFW_PRESS) {
        } else if (action == GLFW_RELEASE) {
        } else {
        }
    }
}

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
    model_insert_submodel(model, lines3);
    model_set_color(lines3, (struct tuple3f) {.rgb = {.r = 1, .g = 1, .b = 0}});
    model_translate(lines3, (struct tuple3f) {.xyz= {.x = 0.5, .y = 0.5, .z = 0}});

    struct model * markers = new_model();
    for (float x = -1; x <= 1; x += 1.0/8.0) {
        for (float y = -1; y <= 1; y += 1.0/8.0) {
            model_insert_marker(markers, (struct tuple3f){x, y, 0});
        }
    }
    struct ortho_projection marker_projection = {-1, 1, -1, 1, -1, 1};
    //model_set_projection(markers, &marker_projection);
    model_insert_submodel(model, markers);

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(window_x, window_y, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Setup some mouse related callbacks... */
    glfwSetCursorPosCallback(window, mouse_pos_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

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
    glClearDepth(0);
    glPointSize(1);
    glEnable( GL_PROGRAM_POINT_SIZE);
    struct timespec start, end;

    int frames = 0;
    clock_gettime(CLOCK_MONOTONIC, &start);
    /* Loop until the user closes the window */
    double x = 0, y = 0;
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

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

        if (0 && stopwatch >= 1) {
            printf("%f fps\n", frames / stopwatch);
            start = end;
            frames = 0;
        } 
    	//model_rotate(lines1, (struct tuple3f) {.xyz = {.x = x, .y = y, .z = 0}});
    	//model_rotate(lines2, (struct tuple3f) {.xyz = {.x = -x, .y = y, .z = 0}});
    	//model_rotate(lines3, (struct tuple3f) {.xyz = {.x = x, .y = -y, .z = 0}});
        //x += 0.1;
        //y += 0.1;
    }

    glfwDestroyWindow(window);

    glfwTerminate();

    free_model(model);

    return 0;
}

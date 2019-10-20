
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

void model_draw_polyline(struct model_polyline const * polyline)
{
    glColor3f(1, 1, 1);
    glBegin(GL_LINES);
    for (int i = 0; i < polyline->point_count; ++i) {
        struct tuple3f const * point = &polyline->points[i];
        glVertex3f(point->xyz.x, point->xyz.y, point->xyz.z);
    }
    glEnd();
}

void model_draw(struct model const * model)
{
    if (model->projection) {
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(
            model->ortho_projection.left,
            model->ortho_projection.right,
            model->ortho_projection.bottom,
            model->ortho_projection.top,
            model->ortho_projection.front,
            model->ortho_projection.back
        );
    }

    if (model->transform) {
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
    }

    for (int i = 0; i < model->polyline_count; ++i) {
        struct model_polyline * polyline = model->polylines[i];
        model_draw_polyline(polyline);
    }
 
    for (int i = 0; i < model->text_count; ++i) {
        struct model_text * text = &model->text[i];
        model_draw_text(text->x, text->y, text->str);
    }

    for (int i = 0; i < model->submodel_count; ++i) {
        model_draw(model->submodels[i]);
    }

    if (model->transform) {
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
    }

    if (model->projection) {
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
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
    model->color = (struct tuple3f) {.rgb = {.r = 1, .g = 1, .b = 1}};

    struct model * lines = new_model();
    float z = .5;
    struct tuple3f points[] = {
        {.xyz = {0, 0, z}},
        {.xyz = {1, 1, z}},
        {.xyz = {0, 1, z}},
        {.xyz = {1, 0, z}},
        {.xyz = {0, 0, z}},
    };
    model_insert_polyline(lines, points, 5);
    struct ortho_projection projection = {-1.1, 1.1, -1.1, 1.1, -1.1, 1.1};
    model_set_projection(lines, &projection);
    model_set_transform(lines);
    model_insert_submodel(model, lines);

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
        glfwWaitEventsTimeout(1);

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

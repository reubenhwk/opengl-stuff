
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
    glBegin(polyline->mode);
    for (int i = 0; i < polyline->point_count; ++i) {
        struct tuple3f const * point = &polyline->points[i];
        glVertex3fv((GLfloat const*)point);
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

    if (model->translate) {
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        glTranslatef(model->translation.xyz.x, model->translation.xyz.x, model->translation.xyz.x);
    }

    if (model->rotate) {
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        glRotatef(model->rotation.xyz.x, model->rotation.xyz.x, model->rotation.xyz.x, 1);
    }

    if (model->has_color) {
        glPushAttrib(GL_CURRENT_BIT);
        glColor3fv((GLfloat const*)model);
    }

    glBegin(GL_POINTS);
    for (int i = 0; i < model->marker_count; ++i) {
        struct model_marker * marker = &model->markers[i];
        glVertex3fv((GLfloat const*)marker);
    }
    glEnd();

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

    if (model->has_color) {
        glPopAttrib();
    }

    if (model->rotate) {
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
    }

    if (model->translate) {
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
    }

    if (model->projection) {
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
    }
}


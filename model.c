
#include "model.h"

#include <stdlib.h>
#include <string.h>

struct model * new_model(void)
{
    struct model * retval = malloc(sizeof(struct model));
    memset(retval, 0, sizeof(struct model));
    return retval;
}

void model_set_projection(struct model * model, struct ortho_projection const * projection)
{
    model->projection = true;
    model->ortho_projection = *projection;
}

void model_insert_submodel(struct model * model, struct model * submodel)
{
    struct model ** submodels = realloc(model->submodels, (model->submodel_count + 1) * sizeof(struct model*));

    if (!submodels) {
        return /* ERROR */;
    }

    model->submodels = submodels;

    model->submodels[model->submodel_count++] = submodel;

    return /* SUCCESS */;
}

void model_insert_text(struct model * model, int x, int y, char const *str)
{
    char * newstr = strdup(str);

    if (!newstr) {
        goto out;
    }

    struct model_text * text = realloc(model->text, (model->text_count + 1) * sizeof(struct model_text));

    if (!text) {
        goto out;
    }

    model->text = text;
    model->text[model->text_count++] = (struct model_text) {
        .x = x,
        .y = y,
        .str = newstr,
    };

    return /* SUCCESS */;
out:

    free(newstr);

    return /* ERROR */;
}

void model_insert_marker(struct model * model, float x, float y, float z)
{
    struct model_marker * markers = realloc(model->markers, (model->marker_count + 1) * sizeof(struct model_marker));

    if (!markers) {
        goto out;
    }

    model->markers = markers;
    model->markers[model->marker_count++] = (struct model_marker) {
        .point = {x, y, z},
    };

    return /* SUCCESS */;
out:

    return /* ERROR */;
}

void model_insert_polyline(struct model * model, int mode, struct tuple3f const * points, size_t point_count)
{
    struct model_polyline * polyline = malloc(sizeof(struct model_polyline));

    if (!polyline) {
        goto out;
    }

    polyline->points = malloc(sizeof(struct tuple3f) * point_count);

    if (!polyline->points) {
        goto out;
    }

    memcpy(polyline->points, points, sizeof(struct tuple3f) * point_count);
    polyline->point_count = point_count;
    polyline->mode = mode;

    struct model_polyline ** polylines = realloc(model->polylines, (model->polyline_count + 1) * sizeof(struct model_polyline));

    if (!polylines) {
        goto out;
    }

    model->polylines = polylines;
    model->polylines[model->polyline_count++] = polyline;

    return /* SUCCESS */;
out:
    if (polyline) {
        free(polyline->points);
        free(polyline);
    }

    return /* ERROR */;
}

void model_set_color(struct model * model, float r, float g, float b)
{
    model->has_color = true;
    model->color = (struct tuple3f){r, g, b};
}

void model_translate(struct model * model, float x, float y, float z)
{
    model->translate = true;
    model->translation = (struct tuple3f){x, y, z};
}

void model_rotate(struct model * model, float x, float y, float z)
{
    model->rotate = true;
    model->rotation = (struct tuple3f){x, y, z};
}

void free_model(struct model * model)
{
    for (int i = 0; i < model->text_count; ++i) {
        struct model_text * text = &model->text[i];
        free(text->str);
    }

    for (int i = 0; i < model->polyline_count; ++i) {
        free(model->polylines[i]->points);
        free(model->polylines[i]);
    }

    for (int i = 0; i < model->submodel_count; ++i) {
        free_model(model->submodels[i]);
    }

    free(model->text);
    free(model->markers);
    free(model->polylines);
    free(model->submodels);
    free(model);
}



#pragma once

#include <stdbool.h>
#include <stdlib.h>

struct model_text {
    char * str;
    int x, y;
};

struct ortho_projection {
    float left;
    float right;
    float bottom;
    float top;
    float front;
    float back;
};

struct model_polyline {
    struct tuple3f *points;
    int point_count;
};

struct tuple3f {
    union {
        struct {
            float r, g, b;
        } rgb;
        struct {
            float x, y, z;
        } xyz;
        struct {
            float a, b, c;
        } abc;
    };
};

struct model {
    bool projection;
    struct ortho_projection ortho_projection;

    bool transform;

    struct model **submodels;
    int submodel_count;

    struct model_text *text;
    int text_count;

    struct model_polyline **polylines;
    int polyline_count;

    struct tuple3f color;
};

struct model * new_model(void);
void free_model(struct model * model);
void model_insert_text(struct model * model, int x, int y, char const *str);
void model_insert_polyline(struct model * model, struct tuple3f * points, size_t point_count);
void model_insert_submodel(struct model * model, struct model * submodel);
void model_set_projection(struct model * model, struct ortho_projection const * ortho_projection);
void model_set_transform(struct model * model);

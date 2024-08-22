
#pragma once

#include <stdbool.h>
#include <stdlib.h>

struct ortho_projection {
    float left;
    float right;
    float bottom;
    float top;
    float front;
    float back;
};

struct tuple3f {
    union {
        struct {
            float r, g, b;
        } rgb;
        struct {
            float x, y, z;
        } xyz;
    };
};

struct model_text {
    char * str;
    int x, y;
};

struct model_marker {
    struct tuple3f point;
};

struct model_polyline {
    struct tuple3f * points;
    int point_count;
    int mode;
};

struct model {
    bool projection;
    struct ortho_projection ortho_projection;

    bool translate;
    struct tuple3f translation;
    
    bool rotate;
    struct tuple3f rotation;

    struct model **submodels;
    int submodel_count;

    struct model_marker *markers;
    int marker_count;

    struct model_text *text;
    int text_count;

    struct model_polyline **polylines;
    int polyline_count;

    bool has_color;
    struct tuple3f color;
};

struct model * new_model(void);
void free_model(struct model * model);

void model_insert_text(struct model * model, int x, int y, char const * str);
void model_insert_marker(struct model * model, float x, float y, float z);
void model_insert_polyline(struct model * model, int mode, struct tuple3f const * points, size_t point_count);
void model_insert_submodel(struct model * model, struct model * submodel);

void model_set_color(struct model * model, float r, float g, float b);
void model_set_projection(struct model * model, struct ortho_projection const * ortho_projection);
void model_translate(struct model * model, float x, float y, float z);
void model_rotate(struct model * model, float x, float y, float z);

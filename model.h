
#pragma once

struct model_text {
    char * str;
    int x, y;
};

struct model {
    struct model **submodels;
    int submodel_count;

    struct model_text *text;
    int text_count;
};

struct model * new_model(void);
void free_model(struct model * model);
void model_insert_text(struct model * model, int x, int y, char const *str);
void model_insert_submodel(struct model * model, struct model * submodel);

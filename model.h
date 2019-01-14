
#pragma once

struct model_text {
    char * str;
    int x, y;
};

struct model {
    struct model_text *text;
    int text_count;
};

struct model * new_model(void);
void model_insert_text(struct model * model, int x, int y, char const *str);

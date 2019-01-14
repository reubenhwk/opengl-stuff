
#include "model.h"

#include <stdlib.h>
#include <string.h>

struct model * new_model(void)
{
    struct model * retval = malloc(sizeof(struct model));
    memset(retval, 0, sizeof(struct model));
    return retval;
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

    newstr = NULL;

    return /* SUCCESS */;
out:

    free(newstr);

    return /* ERROR */;
}

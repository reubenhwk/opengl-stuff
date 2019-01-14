
#include "model.h"

#include <stdlib.h>
#include <string.h>

struct model * new_model(void)
{
    struct model * retval = malloc(sizeof(struct model));
    memset(retval, 0, sizeof(struct model));
    return retval;
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

out:

    free(newstr);
}

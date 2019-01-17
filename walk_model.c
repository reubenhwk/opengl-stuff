
#include "model.h"

#include <stdio.h>

struct model_walk_ctx {
    int depth;
};

struct model_walk_ops {
    void (*model_walk)(struct model_walk_ops const *, struct model_walk_ctx *, struct model *);
    void (*model_walk_text)(struct model_walk_ops const *, struct model_walk_ctx *, struct model_text *);
};

static void indent(int i)
{
    while (i--) {
        printf("    ");
    }
}

static void model_walk_text(struct model_walk_ops const * ops, struct model_walk_ctx * ctx, struct model_text * text)
{
    indent(ctx->depth);
    printf("text : (%d, %d, \"%s\")\n", text->x, text->y, text->str);
}

static void model_walk(struct model_walk_ops const * ops, struct model_walk_ctx * ctx, struct model * model)
{
    for (int i = 0; i < model->text_count; ++i) {
        struct model_text * text = &model->text[i];
        ops->model_walk_text(ops, ctx, text);
    }

    for (int i = 0; i < model->submodel_count; ++i) {
        indent(ctx->depth);
        printf("{\n");
        ctx->depth++;

        ops->model_walk(ops, ctx, model->submodels[i]);

        ctx->depth--;
        indent(ctx->depth);
        printf("}\n");
    }
}

struct model_walk_ops printf_driver = {
    .model_walk = model_walk,
    .model_walk_text = model_walk_text,
};

void printf_driver_walk_model(struct model * model)
{
    struct model_walk_ctx ctx = {1};
    printf("{\n");
    printf_driver.model_walk(&printf_driver, &ctx, model);
    printf("}\n");
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

    printf_driver_walk_model(model);

    return 0;
}


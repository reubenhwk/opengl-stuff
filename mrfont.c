
#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <ft2build.h>
#include FT_FREETYPE_H

//#define FONT_PATH "/usr/share/fonts/dejavu/DejaVuSans.ttf"
#define FONT_PATH "/usr/share/fonts/opentype/stix/STIXGeneral-Regular.otf"

void mrfont_glyph_draw(FT_GlyphSlot glyph, GLuint texture);
void mrfont_glyph_render(FT_Face face, int c);
GLuint mrfont_glyph_to_texture(FT_Bitmap const *bitmap);

int mrfont_init(void)
{
    FT_Library library;
    FT_Face face;
    int error;

    /* Initialize freetype. */
    error = FT_Init_FreeType(&library);
    if (error) {
        fprintf(stderr, "ERROR: Unable to init freetype library, exiting.\n");
        exit(1);
    }

    /* Load a font, use default char size. */
    error = FT_New_Face(library, FONT_PATH, 0, &face);
    if (error == FT_Err_Unknown_File_Format) {
        fprintf(stderr, "ERROR: Unknown file format encountered in '%s', exiting.\n", FONT_PATH);
        exit(1);
    } else if (error) {
        fprintf(stderr, "ERROR: Unable create freetype face from '%s', exiting.\n", FONT_PATH);
        exit(1);
    }

    /* Set the char size. */
    FT_Set_Pixel_Sizes(face, 60, 30);
    if (error) {
        fprintf(stderr, "ERROR: Unable set char size, exiting.\n");
        exit(1);
    }

    mrfont_glyph_render(face, 66);
    GLuint texture = mrfont_glyph_to_texture(&face->glyph->bitmap);
    mrfont_glyph_draw(face->glyph, texture);

    return 0;
}

void mrfont_glyph_render(FT_Face face, int c)
{
    int glyph_index = FT_Get_Char_Index(face, c);
    int error;

    error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
    if (error) {
        fprintf(stderr, "ERROR: Unable to load glyph %d\n", glyph_index);
        return;
    }

    if (face->glyph->format != FT_GLYPH_FORMAT_BITMAP) {
        error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
        if (error) {
            fprintf(stderr, "ERROR: Unable to render glyph %d\n", glyph_index);
            return;
        }
    }

}

GLuint mrfont_glyph_to_texture(FT_Bitmap const *bitmap)
{
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RED,
        bitmap->width,
        bitmap->rows,
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        bitmap->buffer
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    return texture;
}

void mrfont_glyph_draw(FT_GlyphSlot glyph, GLuint texture)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBegin(GL_QUADS);
    glTexCoord2f(0, 1);
    glVertex2f(0, 0);
    glTexCoord2f(1, 1);
    glVertex2f(.4, 0);
    glTexCoord2f(1, 0);
    glVertex2f(.4, .6);
    glTexCoord2f(0, 0);
    glVertex2f(0, .6);
    glEnd();
}



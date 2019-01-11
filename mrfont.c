
#include <stdio.h>
#include <stdlib.h>
#include <ft2build.h>
#include FT_FREETYPE_H

//#define FONT_PATH "/usr/share/fonts/dejavu/DejaVuSans.ttf"
#define FONT_PATH "/usr/share/fonts/opentype/stix/STIXGeneral-Regular.otf"

void output_glyph(FT_GlyphSlot glyph);

int mrfont_init(int argc, char * argv[])
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

    /* Print the charmap for these characters. */
    char chars[] = {"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 ?.!"};
    for (int i = 0; i < sizeof(chars) -1; ++i) {
        int glyph_index = FT_Get_Char_Index(face, chars[i]);
        printf("%c -> %d\n", chars[i], glyph_index);
    }

    /* Load and render glyphs. */
    for (int i = 0; i < sizeof(chars) -1; ++i) {
        int glyph_index = FT_Get_Char_Index(face, chars[i]);
        error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
        if (error) {
            fprintf(stderr, "ERROR: Unable to load glyph %d\n", glyph_index);
            continue;
        }

        if (face->glyph->format != FT_GLYPH_FORMAT_BITMAP) {
            error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
            if (error) {
                fprintf(stderr, "ERROR: Unable to render glyph %d\n", glyph_index);
                continue;
            }
        }

        output_glyph(face->glyph);
    }

    return 0;
}

void output_glyph(FT_GlyphSlot glyph)
{
    switch (glyph->bitmap.pixel_mode) {
        case FT_PIXEL_MODE_GRAY:
            printf("\n");
            for (int rows = 0; rows < glyph->bitmap.rows; ++rows) {
                for (int cols = 0; cols < glyph->bitmap.width; ++cols) {
                    unsigned char * pixels = glyph->bitmap.buffer + glyph->bitmap.pitch * rows;
                    printf("%c", pixels[cols] > 0 ? 'X' : ' ');
                }
                printf("\n");
            }
            printf("\n");
        break;

        default:
        break;
    }
}

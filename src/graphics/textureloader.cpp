#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "textureloader.h"

unsigned char *TextureLoader::loadTexture(const char* fileName, int* x, int* y, int* n)
{
    return stbi_load(fileName, x, y, n, 4);
}

void TextureLoader::free(unsigned char *tex)
{
    if(tex)
    {
        stbi_image_free(tex);
    }
}
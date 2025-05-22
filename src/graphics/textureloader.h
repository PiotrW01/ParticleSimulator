#pragma once

class TextureLoader
{
    public:
        static unsigned char* loadTexture(const char* fileName, int* x, int* y, int* n);
        static void free(unsigned char* tex);
};
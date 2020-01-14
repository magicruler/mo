#pragma once
#include "common.h"
#include "glad/glad.h"

class Texture
{
public:
    Texture();
    ~Texture();

    void SetData2D(unsigned int width, unsigned int height, int internalFormat, int format, int dataType, void *data);

    void Bind(int texUnit = 0);
    void Unbind();

    void Resize(unsigned int width, unsigned int height);

    unsigned int AsID() const;

    int target = GL_TEXTURE_2D;

    int internalFormat = GL_RGBA;
    int format = GL_RGBA;
    int dataType = GL_UNSIGNED_BYTE;

    int filterMin = GL_LINEAR_MIPMAP_LINEAR;
    int filterMag = GL_LINEAR;

    int wrapR = GL_REPEAT;
    int wrapS = GL_REPEAT;
    int wrapT = GL_REPEAT;

    bool isMipmapped = true;
    bool created = false;

private:
    unsigned int ID;
    unsigned int width = 0;
    unsigned int height = 0;
    unsigned int depth = 0;
};
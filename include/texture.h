#pragma once
#include "common.h"

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

    int target;

    int internalFormat;
    int format;
    int dataType;

    int filterMin;
    int filterMag;

    int wrapR;
    int wrapS;
    int wrapT;

    bool isMipmapped = true;
    bool created = false;

private:
    unsigned int ID;
    unsigned int width = 0;
    unsigned int height = 0;
    unsigned int depth = 0;
};
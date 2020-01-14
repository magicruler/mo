#pragma once
#include "texture.h"

Texture::Texture()
{
}

Texture::~Texture()
{
    if (created)
    {
        spdlog::info("texture destroyed");
        glDeleteTextures(1, &ID);
    }
}

void Texture::SetData2D(unsigned int width, unsigned int height, int internalFormat, int format, int dataType, void *data)
{
    glGenTextures(1, &ID);
    created = true;

    this->width = width;
    this->height = height;
    this->depth = 0;
    this->internalFormat = internalFormat;
    this->format = format;
    this->dataType = dataType;

    assert(target == GL_TEXTURE_2D);

    Bind();
    glTexImage2D(target, 0, internalFormat, width, height, 0, format, dataType, data);
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, filterMin);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, filterMag);
    glTexParameteri(target, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, wrapT);
    if (isMipmapped)
    {
        glGenerateMipmap(target);
    }
    Unbind();
}

void Texture::Bind(int texUnit)
{
    if (texUnit > 0)
    {
        glActiveTexture(GL_TEXTURE0 + texUnit);
    }
    glBindTexture(target, ID);
}

void Texture::Unbind()
{
    glBindTexture(target, 0);
}

void Texture::Resize(unsigned int width, unsigned int height)
{
    assert(target == GL_TEXTURE_2D);
    Bind();
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, dataType, 0);
    Unbind();
}

unsigned int Texture::AsID() const
{
    return ID;
}
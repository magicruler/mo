#pragma once

#include "glad/glad.h"
#include "common.h"

class Texture
{
public:
    Texture()
    {
    }

    Texture(Texture &&other)
    {
        spdlog::info("move constructor happen");
        width = other.width;
        height = other.height;
        depth = other.depth;
        ID = other.ID;
        target = other.target;
        format = other.format;
        internalFormat = other.internalFormat;
        dataType = other.dataType;
        isMipmapped = other.isMipmapped;
        filterMin = other.filterMin;
        filterMag = other.filterMag;
        wrapR = other.wrapR;
        wrapS = other.wrapS;
        wrapT = other.wrapT;
        created = other.created;

        other.created = false;
    }

    Texture& operator=(Texture&& rhs)
    {
        spdlog::info("move assignment happen");
        width = rhs.width;
        height = rhs.height;
        depth = rhs.depth;
        ID = rhs.ID;
        target = rhs.target;
        format = rhs.format;
        internalFormat = rhs.internalFormat;
        dataType = rhs.dataType;
        isMipmapped = rhs.isMipmapped;
        filterMin = rhs.filterMin;
        filterMag = rhs.filterMag;
        wrapR = rhs.wrapR;
        wrapS = rhs.wrapS;
        wrapT = rhs.wrapT;
        created = rhs.created;

        rhs.created = false;
        
        return *this;
    }

    ~Texture()
    {
        if (created)
        {
            spdlog::info("texture destroyed");
            //glDeleteTextures(1, &ID);
        }
    }

    void SetData2D(unsigned int width, unsigned int height, int internalFormat, int format, int dataType, void *data)
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

    void Bind(int texUnit = 0)
    {
        if (texUnit > 0)
        {
            glActiveTexture(GL_TEXTURE0 + texUnit);
        }
        glBindTexture(target, ID);
    }

    void Unbind()
    {
        glBindTexture(target, 0);
    }

    void Resize(unsigned int width, unsigned int height)
    {
        assert(target == GL_TEXTURE_2D);
        Bind();
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, dataType, 0);
        Unbind();
    }

    unsigned int AsID() const
    {
        return ID;
    }

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
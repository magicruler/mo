#pragma once

#include "texture.h"
#include "common.h"

class RenderTarget
{
public:
    RenderTarget(unsigned int width, unsigned int height, int dataType = GL_UNSIGNED_BYTE, unsigned int attachmentCount = 1, bool hasDepth = true)
        : dataType(dataType),
          width(width),
          height(height),
          hasDepth(hasDepth)
    {
        glGenFramebuffers(1, &ID);
        glBindFramebuffer(GL_FRAMEBUFFER, ID);

        for (unsigned int i = 0; i < attachmentCount; i++)
        {
            Texture texture;
            texture.filterMin = GL_LINEAR;
            texture.filterMag = GL_LINEAR;
            texture.wrapS = GL_CLAMP_TO_EDGE;
            texture.wrapT = GL_CLAMP_TO_EDGE;
            texture.isMipmapped = false;

            int internalFormat = GL_RGBA;
            if (dataType == GL_HALF_FLOAT)
            {
                internalFormat = GL_RGBA16F;
            }
            else if (dataType == GL_FLOAT)
            {
                internalFormat = GL_RGBA32F;
            }

            texture.SetData2D(width, height, internalFormat, GL_RGBA, dataType, 0);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, texture, 0);
            colorAttachments.push_back(texture);
        }

        if (hasDepth)
        {
            Texture texture;
            texture.filterMin = GL_LINEAR;
            texture.filterMag = GL_LINEAR;
            texture.wrapS = GL_CLAMP_TO_EDGE;
            texture.wrapT = GL_CLAMP_TO_EDGE;
            texture.isMipmapped = false;

            texture.SetData2D(width, height, GL_DEPTH_STENCIL, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 0);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, texture, 0);

            depthAttachment = texture;
        }

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            spdlog::error("Framebuffer Creation Error");
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    Texture *getDepthTexture()
    {
        return &depthAttachment;
    }

    Texture *getAttachmentTexture(unsigned int index)
    {
        if (index >= colorAttachments.size())
        {
            spdlog::info("Try to get a non-exist attachment with index {}", index);
            return nullptr;
        }
        else
        {
            return &colorAttachments[index];
        }
    }

private:
    int dataType;
    unsigned int width;
    unsigned int height;
    bool hasDepth;
    Texture depthAttachment;
    std::vector<Texture> colorAttachments;
    unsigned int ID;
};
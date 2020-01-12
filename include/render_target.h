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
            std::shared_ptr<Texture> texture = std::make_shared<Texture>();
            texture->filterMin = GL_LINEAR;
            texture->filterMag = GL_LINEAR;
            texture->wrapS = GL_CLAMP_TO_EDGE;
            texture->wrapT = GL_CLAMP_TO_EDGE;
            texture->isMipmapped = false;

            int internalFormat = GL_RGBA;
            if (dataType == GL_HALF_FLOAT)
            {
                internalFormat = GL_RGBA16F;
            }
            else if (dataType == GL_FLOAT)
            {
                internalFormat = GL_RGBA32F;
            }

            texture->SetData2D(width, height, internalFormat, GL_RGBA, dataType, 0);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, texture->AsID(), 0);
            colorAttachments.push_back(texture);
        }

        if (hasDepth)
        {
            std::shared_ptr<Texture> texture = std::make_shared<Texture>();
            texture->filterMin = GL_LINEAR;
            texture->filterMag = GL_LINEAR;
            texture->wrapS = GL_CLAMP_TO_EDGE;
            texture->wrapT = GL_CLAMP_TO_EDGE;
            texture->isMipmapped = false;

            texture->SetData2D(width, height, GL_DEPTH_STENCIL, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 0);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, texture->AsID(), 0);
            depthAttachment = texture;
        }

        int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE)
        {
            switch (status)
            {
            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
                spdlog::error("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT");
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
                spdlog::error("GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS");
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
                spdlog::error("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT");
                break;
            case GL_FRAMEBUFFER_UNSUPPORTED:
                spdlog::error("GL_FRAMEBUFFER_UNSUPPORTED");
                break;
            }
            spdlog::error("Framebuffer Creation Error");
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        spdlog::info("Framebuffer Creation Compelete");
    }

    RenderTarget(const RenderTarget &other)
    {
        ID = other.ID;
        dataType = other.dataType;
        width = other.width;
        height = other.height;
        hasDepth = other.hasDepth;
        depthAttachment = other.depthAttachment;
        colorAttachments = other.colorAttachments;
    }

    ~RenderTarget()
    {
        glDeleteFramebuffers(1, &ID);
        spdlog::info("Framebuffer Destroy Compelete");
    }

    std::shared_ptr<Texture> GetDepthTexture() const
    {
        return depthAttachment;
    }

    std::shared_ptr<Texture> GetAttachmentTexture(unsigned int index) const
    {
        if (index >= colorAttachments.size())
        {
            spdlog::info("Try to get a non-exist attachment with index {}", index);
            return nullptr;
        }
        else
        {
            return colorAttachments[index];
        }
    }

    void Resize(unsigned int width, unsigned int height)
    {
        this->width = width;
        this->height = height;

        for (unsigned int i = 0; i < colorAttachments.size(); ++i)
        {
            colorAttachments[i]->Resize(width, height);
        }

        if (hasDepth)
        {
            depthAttachment->Resize(width, height);
        }
    }

    void Bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, ID);
    }

    void Unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

private:
    int dataType;
    unsigned int width;
    unsigned int height;
    bool hasDepth;
    std::shared_ptr<Texture> depthAttachment;
    std::vector<std::shared_ptr<Texture>> colorAttachments;
    unsigned int ID;
};
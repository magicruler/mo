#include "render_target.h"
#ifndef __glad_h_
#include <glad/glad.h>
#endif
#include "texture.h"
#include "event.h"

RenderTarget::RenderTarget(
    unsigned int width,
    unsigned int height,
    // GL_UNSIGNED_BYTE
    int dataType,
    unsigned int attachmentCount,
    bool hasDepth,
    bool isRenderTexture) :
    isRenderTexture(isRenderTexture),
    width(width),
    height(height),
    hasDepth(hasDepth),
    attachmentCount(attachmentCount)
{
    for (int i = 0; i < attachmentCount; i++)
    {
        RenderTargetDescriptor descriptor = RenderTargetDescriptor();

        if (dataType == GL_UNSIGNED_BYTE)
        {
            descriptor.format = RENDER_TARGET_FORMAT::RGBA8888;
        }
        else if (dataType == GL_HALF_FLOAT)
        {
            descriptor.format = RENDER_TARGET_FORMAT::RGBA16F;
        }
        else if (dataType == GL_FLOAT)
        {
            descriptor.format = RENDER_TARGET_FORMAT::RGBA32F;
        }
        else
        {
            // Need More Implementation
            assert(false);
        }

        descriptors.push_back(descriptor);
    }

    Init();
}

RenderTarget::~RenderTarget()
{
    glDeleteFramebuffers(1, &ID);
    spdlog::info("Framebuffer Destroy Compelete");

    for(int i = colorAttachments.size() - 1; i >= 0; i--)
    {
        delete colorAttachments[i];
    }

    delete depthAttachment;
}

Texture* RenderTarget::GetDepthTexture() const
{
    return depthAttachment;
}

Texture* RenderTarget::GetAttachmentTexture(unsigned int index) const
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

void RenderTarget::Resize(unsigned int width, unsigned int height)
{
    spdlog::info("Render Target Resize To {}, {}", width, height);
    this->width = width;
    this->height = height;

    glDeleteFramebuffers(1, &ID);
    
    for (int i = colorAttachments.size() - 1; i >= 0; i--)
    {
        delete colorAttachments[i];
    }

    colorAttachments.clear();

    if (depthAttachment != nullptr)
    {
        delete depthAttachment;
        depthAttachment = nullptr;
    }

    Init();

    RenderTargetResizeEvent e;
    e.size = glm::vec2(width, height);
    e.renderTarget = this;
    this->Dispatch(e);
}

void RenderTarget::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, ID);
}

void RenderTarget::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

glm::vec2 RenderTarget::GetSize() const
{
    return glm::vec2(width, height);
}

void RenderTarget::Init()
{
    glGenFramebuffers(1, &ID);
    glBindFramebuffer(GL_FRAMEBUFFER, ID);

    std::vector<GLenum> attachmentEnums;

    for (unsigned int i = 0; i < attachmentCount; i++)
    {
        RenderTargetDescriptor descriptor = descriptors[i];

        Texture* texture = new Texture();
        texture->filterMin = GL_LINEAR;
        texture->filterMag = GL_LINEAR;
        texture->isMipmapped = false;

        if (isRenderTexture)
        {
            texture->filterMin = GL_LINEAR;
            texture->filterMag = GL_LINEAR;
            texture->isMipmapped = true;
        }

        texture->wrapS = GL_CLAMP_TO_EDGE;
        texture->wrapT = GL_CLAMP_TO_EDGE;
       
        int internalFormat = GL_RGBA;
        int generalFormat = GL_RGBA;
        int dataFormat = GL_UNSIGNED_BYTE;

        switch (descriptor.format)
        {
        case RENDER_TARGET_FORMAT::RGBA8888:
            internalFormat = GL_RGBA;
            generalFormat = GL_RGBA;
            dataFormat = GL_UNSIGNED_BYTE;
            break;
        case RENDER_TARGET_FORMAT::RGB888:
            internalFormat = GL_RGB;
            generalFormat = GL_RGB;
            dataFormat = GL_UNSIGNED_BYTE;
            break;
        case RENDER_TARGET_FORMAT::RGBA16F:
            internalFormat = GL_RGBA16F;
            generalFormat = GL_RGBA;
            dataFormat = GL_HALF_FLOAT;
            break;
        case RENDER_TARGET_FORMAT::RGB16F:
            internalFormat = GL_RGB16F;
            generalFormat = GL_RGB;
            dataFormat = GL_HALF_FLOAT;
            break;
        case RENDER_TARGET_FORMAT::RGBA32F:
            internalFormat = GL_RGBA32F;
            generalFormat = GL_RGBA;
            dataFormat = GL_FLOAT;
            break;
        case RENDER_TARGET_FORMAT::RGB32F:
            internalFormat = GL_RGB32F;
            generalFormat = GL_RGB;
            dataFormat = GL_FLOAT;
            break;
        }

        texture->SetData2D(width, height, internalFormat, generalFormat, dataFormat, 0);
        if (isRenderTexture)
        {
            texture->SetAnisotropy(4.0f);
        }
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, texture->AsID(), 0);
        colorAttachments.push_back(texture);
        attachmentEnums.push_back(GL_COLOR_ATTACHMENT0 + i);
    }

    glNamedFramebufferDrawBuffers(ID, attachmentEnums.size(), attachmentEnums.data());

    if (hasDepth)
    {
        Texture* texture = new Texture();
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

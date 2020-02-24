#pragma once
#include "common.h"

class Texture;

enum class RENDER_TARGET_FORMAT
{
    RGBA8888,
    RGB888,
    RGBA16F,
    RGB16F,
    RGBA32F,
    RGB32F
};

class RenderTargetDescriptor
{
public:
    RenderTargetDescriptor()
        :format(RENDER_TARGET_FORMAT::RGBA8888)
    {
    }

    bool mipmap = false;
    RENDER_TARGET_FORMAT format;
};

class RenderTarget:public Object
{
    MO_OBJECT("RenderTarget")
public:
    RenderTarget(
        unsigned int width,
        unsigned int height,
        // GL_UNSIGNED_BYTE
        int dataType = 0x1401,
        unsigned int attachmentCount = 1,
        bool hasDepth = true,
        bool isRenderTexture = false);

    RenderTarget(
        unsigned int width,
        unsigned int height,
        std::vector<RenderTargetDescriptor> descriptors,
        bool hasDepth = true,
        bool isRenderTexture = false);

    ~RenderTarget();

    Texture *GetDepthTexture() const;

    Texture *GetAttachmentTexture(unsigned int index) const;

    void Resize(unsigned int width, unsigned int height);

    void Bind();

    void Unbind();

    glm::vec2 GetSize() const;

    bool HasDepth()
    {
        return hasDepth;
    }

    unsigned int GetGPUHandle()
    {
        return ID;
    }

    void CopyDepth(RenderTarget* src);

private:
    void Init();

    unsigned int width;
    unsigned int height;
    bool hasDepth;
    unsigned int attachmentCount;
    Texture *depthAttachment = nullptr;
    std::vector<Texture *> colorAttachments;
    unsigned int ID;
    bool isRenderTexture = false;
    std::vector<RenderTargetDescriptor> descriptors;
};
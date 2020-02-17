#pragma once
#include "common.h"

class Texture;

class RenderTarget
{
public:
    RenderTarget(
        unsigned int width, 
        unsigned int height, 
        // GL_UNSIGNED_BYTE
        int dataType = 0x1401, 
        unsigned int attachmentCount = 1, 
        bool hasDepth = true,
        bool isRenderTexture = false) : 
        dataType(dataType),
        isRenderTexture(isRenderTexture),
        width(width),
        height(height),
        hasDepth(hasDepth),
        attachmentCount(attachmentCount)
    {
        Init();
    }

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

private:
    void Init();

    int dataType;
    unsigned int width;
    unsigned int height;
    bool hasDepth;
    unsigned int attachmentCount;
    Texture *depthAttachment = nullptr;
    std::vector<Texture *> colorAttachments;
    unsigned int ID;
    bool isRenderTexture = false;
};
#pragma once
#include "common.h"
#include "glad/glad.h"

class Texture;

class RenderTarget
{
public:
    RenderTarget(unsigned int width, unsigned int height, int dataType = GL_UNSIGNED_BYTE, unsigned int attachmentCount = 1, bool hasDepth = true) : dataType(dataType),
                                                                                                                                                     width(width),
                                                                                                                                                     height(height),
                                                                                                                                                     hasDepth(hasDepth),
                                                                                                                                                     attachmentCount(attachmentCount)
    {
        Init();
    }

    RenderTarget(const RenderTarget &other);
    ~RenderTarget();

    std::shared_ptr<Texture> GetDepthTexture() const;

    std::shared_ptr<Texture> GetAttachmentTexture(unsigned int index) const;

    void Resize(unsigned int width, unsigned int height);

    void Bind();

    void Unbind();

    glm::vec2 GetSize() const;

private:
    void Init();

    int dataType;
    unsigned int width;
    unsigned int height;
    bool hasDepth;
    unsigned int attachmentCount;
    std::shared_ptr<Texture> depthAttachment;
    std::vector<std::shared_ptr<Texture>> colorAttachments;
    unsigned int ID;
};
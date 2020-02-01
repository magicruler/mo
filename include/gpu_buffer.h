#pragma once
#include "common.h"

enum class BUFFER_DRAW_TYPE
{
    STATIC_DRAW,
    DYNAMIC_DRAW,
    STREAM_DRAW
};

class GPUBuffer
{
public:
    GPUBuffer();
    ~GPUBuffer();

    void BindArrayBuffer();
    void UnBindArrayBuffer();
    void SetDataArrayBuffer(void* data, unsigned int size, BUFFER_DRAW_TYPE drawType);

    void BindElementBuffer();
    void UnBindElementBuffer();
    void SetDataElementBuffer(void* data, unsigned int size, BUFFER_DRAW_TYPE drawType);

private:
    unsigned int ID;
    unsigned int size = 0;
};
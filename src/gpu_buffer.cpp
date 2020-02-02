#include "gpu_buffer.h"
#ifndef __glad_h_
#include <glad/glad.h>
#endif

GPUBuffer::GPUBuffer()
{
	glGenBuffers(1, &ID);
}

GPUBuffer::~GPUBuffer()
{
	glDeleteBuffers(1, &ID);
}

void GPUBuffer::BindArrayBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void GPUBuffer::UnBindArrayBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GPUBuffer::SetDataArrayBuffer(void* data, unsigned int size, BUFFER_DRAW_TYPE drawType)
{
	BindArrayBuffer();

	auto openGlDrawType = GL_STATIC_DRAW;
	switch (drawType)
	{
		case BUFFER_DRAW_TYPE::STATIC_DRAW:
			openGlDrawType = GL_STATIC_DRAW;
			break;
		case BUFFER_DRAW_TYPE::DYNAMIC_DRAW:
			openGlDrawType = GL_DYNAMIC_DRAW;
			break;
		case BUFFER_DRAW_TYPE::STREAM_DRAW:
			openGlDrawType = GL_STREAM_DRAW;
			break;
	}
		
	glBufferData(GL_ARRAY_BUFFER, size, data, openGlDrawType);

	UnBindArrayBuffer();
}

void GPUBuffer::BindElementBuffer()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void GPUBuffer::UnBindElementBuffer()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void GPUBuffer::SetDataElementBuffer(void* data, unsigned int size, BUFFER_DRAW_TYPE drawType)
{
	BindElementBuffer();

	auto openGlDrawType = GL_STATIC_DRAW;
	switch (drawType)
	{
	case BUFFER_DRAW_TYPE::STATIC_DRAW:
		openGlDrawType = GL_STATIC_DRAW;
		break;
	case BUFFER_DRAW_TYPE::DYNAMIC_DRAW:
		openGlDrawType = GL_DYNAMIC_DRAW;
		break;
	case BUFFER_DRAW_TYPE::STREAM_DRAW:
		openGlDrawType = GL_STREAM_DRAW;
		break;
	}

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, openGlDrawType);

	UnBindElementBuffer();
}
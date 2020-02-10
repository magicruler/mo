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

int MapOpenGLBufferUsage(BUFFER_USAGE& usage)
{
	auto openGlUsage = GL_ARRAY_BUFFER;
	switch (usage)
	{
	case BUFFER_USAGE::ARRAY:
		openGlUsage = GL_ARRAY_BUFFER;
		break;
	case BUFFER_USAGE::ELEMENT:
		openGlUsage = GL_ELEMENT_ARRAY_BUFFER;
		break;
	case BUFFER_USAGE::UNIFORM:
		openGlUsage = GL_UNIFORM_BUFFER;
		break;
	}

	return openGlUsage;
}

void GPUBuffer::BindBuffer(BUFFER_USAGE usage)
{
	auto openGLUsage = MapOpenGLBufferUsage(usage);
	glBindBuffer(openGLUsage, ID);
}
void GPUBuffer::UnBindBuffer(BUFFER_USAGE usage)
{
	auto openGLUsage = MapOpenGLBufferUsage(usage);
	glBindBuffer(openGLUsage, 0);
}
void GPUBuffer::SetData(BUFFER_USAGE usage, void* data, unsigned int size, BUFFER_DRAW_TYPE drawType)
{
	auto openGLUsage = MapOpenGLBufferUsage(usage);

	BindBuffer(usage);

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

	glBufferData(openGLUsage, size, data, openGlDrawType);

	UnBindBuffer(usage);
}

void GPUBuffer::BindArrayBuffer()
{
	BindBuffer(BUFFER_USAGE::ARRAY);
}

void GPUBuffer::UnBindArrayBuffer()
{
	UnBindBuffer(BUFFER_USAGE::ARRAY);
}

void GPUBuffer::SetDataArrayBuffer(void* data, unsigned int size, BUFFER_DRAW_TYPE drawType)
{
	SetData(BUFFER_USAGE::ARRAY, data, size, drawType);
}

void GPUBuffer::BindElementBuffer()
{
	BindBuffer(BUFFER_USAGE::ELEMENT);
}

void GPUBuffer::UnBindElementBuffer()
{
	UnBindBuffer(BUFFER_USAGE::ELEMENT);
}

void GPUBuffer::SetDataElementBuffer(void* data, unsigned int size, BUFFER_DRAW_TYPE drawType)
{
	SetData(BUFFER_USAGE::ELEMENT, data, size, drawType);
}
#include "vertex_array.h"
#ifndef __glad_h_
#include <glad/glad.h>
#endif
#include "gpu_buffer.h"

VertexArray::VertexArray(GPUBuffer* arrayBuffer, GPUBuffer* elementBuffer)
{
	this->arrayBuffer = arrayBuffer;
	this->elementBuffer = elementBuffer;

	glGenVertexArrays(1, &ID);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &ID);
}

void VertexArray::LayoutAddFloat3()
{
	vertexLayout.push_back(VERTEX_LAYOUT_TYPE::FLOAT_3);
	stride += 3 * sizeof(float);
}

void VertexArray::LayoutAddFloat2()
{
	vertexLayout.push_back(VERTEX_LAYOUT_TYPE::FLOAT_2);
	stride += 2 * sizeof(float);
}

void VertexArray::LayoutAddFloat1()
{
	vertexLayout.push_back(VERTEX_LAYOUT_TYPE::FLOAT_1);
	stride += 1 * sizeof(float);
}

void VertexArray::UpdateLayoutToGPU()
{
	Bind();

	arrayBuffer->BindArrayBuffer();
	
	if (elementBuffer != nullptr)
	{
		elementBuffer->BindElementBuffer();
	}

	unsigned int offset = 0;

	for (int i = 0; i < vertexLayout.size(); i++)
	{
		auto layoutItem = vertexLayout[i];
		
		auto itemCount = 3;
		auto itemType = GL_FLOAT;
		auto normalized = GL_FALSE;
		auto increasedOffset = 0;

		switch (layoutItem)
		{
		case VERTEX_LAYOUT_TYPE::FLOAT_3:
			itemCount = 3;
			increasedOffset = 3 * sizeof(float);
			itemType = GL_FLOAT;
			normalized = GL_FALSE;
			break;
		case VERTEX_LAYOUT_TYPE::FLOAT_2:
			itemCount = 2;
			increasedOffset = 2 * sizeof(float);
			itemType = GL_FLOAT;
			normalized = GL_FALSE;
			break;
		case VERTEX_LAYOUT_TYPE::FLOAT_1:
			itemCount = 1;
			increasedOffset = 1 * sizeof(float);
			itemType = GL_FLOAT;
			normalized = GL_FALSE;
			break;
		default:
			assert(false);
			break;
		}

		glVertexAttribPointer(i, itemCount, itemType, normalized, stride, (void*)offset);
		glEnableVertexAttribArray(i);

		offset += increasedOffset;
	}

	UnBind();
}

void VertexArray::Bind()
{
	glBindVertexArray(ID);
}

void VertexArray::UnBind()
{
	glBindVertexArray(0);
}
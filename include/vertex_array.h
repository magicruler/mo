#pragma once
#include "common.h"

class GPUBuffer;

enum class VERTEX_LAYOUT_TYPE
{
	FLOAT_3,
	FLOAT_2,
	FLOAT_1
};

/*
	Make sure! This class will not take ownership of gpu buffers
*/
class VertexArray
{
public:
	VertexArray(GPUBuffer* arrayBuffer, GPUBuffer* elementBuffer);
	~VertexArray();

	void LayoutAddFloat3();
	void LayoutAddFloat2();
	void LayoutAddFloat1();
	void UpdateLayoutToGPU();
	void Bind();
	void UnBind();

private:
	unsigned int ID;
	std::vector<VERTEX_LAYOUT_TYPE> vertexLayout;
	GPUBuffer* arrayBuffer = nullptr;
	GPUBuffer* elementBuffer = nullptr;
	unsigned long stride = 0;
};
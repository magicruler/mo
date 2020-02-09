#include "mesh.h"
#include "vertex_array.h"
#include "gpu_buffer.h"

void SubMesh::CreateGPUResource()
{
	elementBuffer = new GPUBuffer();
	elementBuffer->SetDataElementBuffer(indices.data(), indices.size() * sizeof(unsigned int), BUFFER_DRAW_TYPE::STATIC_DRAW);

	arrayBuffer = new GPUBuffer();
	arrayBuffer->SetDataArrayBuffer(vertices.data(), sizeof(MeshVertex) * vertices.size(), BUFFER_DRAW_TYPE::STATIC_DRAW);

	vertexArray = new VertexArray(arrayBuffer, elementBuffer);
	// Position
	vertexArray->LayoutAddFloat3();
	// Normal
	vertexArray->LayoutAddFloat3();
	// UV
	vertexArray->LayoutAddFloat2();
	// Tangent
	vertexArray->LayoutAddFloat3();
	// Bitangent
	vertexArray->LayoutAddFloat3();
	vertexArray->UpdateLayoutToGPU();
}

void Mesh::CreateGPUResource()
{
	for (auto child : children)
	{
		child->CreateGPUResource();
	}
}
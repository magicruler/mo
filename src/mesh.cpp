#include "mesh.h"
#include "vertex_array.h"
#include "gpu_buffer.h"

void Mesh::CreateGPUResource()
{
	elementBuffer = new GPUBuffer();
	elementBuffer->SetDataElementBuffer(indices.data(), indices.size(), BUFFER_DRAW_TYPE::STATIC_DRAW);

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
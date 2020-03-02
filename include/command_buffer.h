#pragma once
#include "common.h"

class SubMesh;
class Camera;
class Material;
class RenderTarget;
class CommandBuffer;
class RenderCommand;
class GPUBuffer;
class VertexArray;
class Light;

enum class RENDER_COMMAND_TYPE
{
	RENDER_QUAD,
	RENDER_MESH,
	RENDER_MESH_MVP,
	RENDER_SKYBOX,
	SET_RENDER_TARGET,
	SET_VIEW_PORT,
	SET_CLEAR_COLOR,
	SET_CLEAR_DEPTH,
	CLEAR,
	ENABLE_DEPTH,
	DISABLE_DEPTH,
	COPY_DEPTH_BUFFER,
	CULL_FACE,
	ENABLE_CULL_FACE,
	DISABLE_CULL_FACE
};

enum CLEAR_BIT
{
	COLOR = 1 << 0,
	DEPTH = 1 << 1,
	STENCIL = 1 << 2,
};

enum class FACE_ORIENTATION
{
	FORWARD,
	BACKWARD,
	ALL
};

class RenderCommand
{
public:
	RENDER_COMMAND_TYPE commandType;
};


class CommandEnableDepth :public RenderCommand
{
public:
	CommandEnableDepth()
	{
		commandType = RENDER_COMMAND_TYPE::ENABLE_DEPTH;
	}
};

class CommandDisableDepth :public RenderCommand
{
public:
	CommandDisableDepth()
	{
		commandType = RENDER_COMMAND_TYPE::DISABLE_DEPTH;
	}
};

class CommandClear :public RenderCommand
{
public:
	CommandClear()
	{
		commandType = RENDER_COMMAND_TYPE::CLEAR;
	}
	
	unsigned int mask;
};

class CommandSetClearDepth :public RenderCommand
{
public:
	CommandSetClearDepth()
	{
		commandType = RENDER_COMMAND_TYPE::SET_CLEAR_DEPTH;
	}

	float depth;
};

class CommandSetClearColor :public RenderCommand
{
public:
	CommandSetClearColor()
	{
		commandType = RENDER_COMMAND_TYPE::SET_CLEAR_COLOR;
	}

	glm::vec4 color;
};

class CommandSetViewPort :public RenderCommand
{
public:
	CommandSetViewPort()
	{
		commandType = RENDER_COMMAND_TYPE::SET_VIEW_PORT;
	}

	glm::vec2 origin;
	glm::vec2 contentSize;
};

class CommandSetRenderTarget :public RenderCommand
{
public:
	CommandSetRenderTarget()
	{
		commandType = RENDER_COMMAND_TYPE::SET_RENDER_TARGET;
	}

	RenderTarget* renderTarget;
};

class CommandRenderMesh :public RenderCommand
{
public:
	CommandRenderMesh()
	{
		commandType = RENDER_COMMAND_TYPE::RENDER_MESH;
	}

	Camera* camera;
	Material* material; 
	SubMesh* mesh;
	glm::mat4 transformation;
};

class CommandRenderMeshMVP :public RenderCommand
{
public:
	CommandRenderMeshMVP()
	{
		commandType = RENDER_COMMAND_TYPE::RENDER_MESH_MVP;
	}

	Material* material;
	SubMesh* mesh;
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
};

class CommandRenderQuad :public RenderCommand
{
public:
	CommandRenderQuad()
	{
		commandType = RENDER_COMMAND_TYPE::RENDER_QUAD;
	}

	Material* material = nullptr;
	glm::mat4 model;
	glm::mat4 projection;
};

class CommandRenderSkybox : public RenderCommand
{
public:
	CommandRenderSkybox()
	{
		commandType = RENDER_COMMAND_TYPE::RENDER_SKYBOX;
	}

	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
};

class CommandCopyDepthBuffer : public RenderCommand
{
public:
	CommandCopyDepthBuffer()
	{
		commandType = RENDER_COMMAND_TYPE::COPY_DEPTH_BUFFER;
	}

	RenderTarget* src = nullptr;
	RenderTarget* dst = nullptr;
};

class CommandCullFace : public RenderCommand
{
public:
	CommandCullFace()
	{
		commandType = RENDER_COMMAND_TYPE::CULL_FACE;
	}

	FACE_ORIENTATION faceOrientation = FACE_ORIENTATION::BACKWARD;
};

class CommandEnableCullFace : public RenderCommand
{
public:
	CommandEnableCullFace()
	{
		commandType = RENDER_COMMAND_TYPE::ENABLE_CULL_FACE;
	}
};

class CommandDisableCullFace : public RenderCommand
{
public:
	CommandDisableCullFace()
	{
		commandType = RENDER_COMMAND_TYPE::DISABLE_CULL_FACE;
	}
};

class GLStateCache
{
	friend CommandBuffer;
public:
	void Init();
	
private:
	RenderTarget* currentRenderTarget = nullptr;
	/*bool depthEnabled = false;
	glm::vec4 clearColor;
	float clearDepth;*/
};

class CommandBuffer
{
public:
	CommandBuffer();

	void EnableDepth();
	void DisableDepth();
	void SetRenderTarget(RenderTarget* renderTarget);
	void SetViewport(const glm::vec2& origin, const glm::vec2& size);
	void SetClearColor(const glm::vec4& color);
	void SetClearDepth(float depth);
	void Clear(unsigned int mask);
	void RenderMesh(Camera* camera, Material* material, SubMesh* mesh, glm::mat4& transformation);
	void RenderMeshMVP(Material* material, SubMesh* mesh, glm::mat4& model, glm::mat4& view, glm::mat4& projection);
	void RenderQuad(const glm::vec2& position, const glm::vec2& size, const glm::mat4& projection, Material* material);
	void RenderSkyBox(const glm::mat4& view, const glm::mat4& projection);
	void CopyDepthBuffer(RenderTarget* src, RenderTarget* dst);

	void EnableCullFace();
	void DisableCullFace();
	void CullFace(FACE_ORIENTATION faceOrientation);

	inline void AddCommand(const std::shared_ptr<RenderCommand> command)
	{
		commandList.push_back(command);
	}

	void Submit();
private:
	GLStateCache stateCahce;
	std::vector<std::shared_ptr<RenderCommand>> commandList;
	
	GPUBuffer* quadVertexBuffer = nullptr;
	VertexArray* quadVertexArray = nullptr;

	GPUBuffer* skyBoxVertexBuffer = nullptr;
	VertexArray* skyBoxVertexArray = nullptr;
	
	Material* skyBoxMaterial = nullptr;
};
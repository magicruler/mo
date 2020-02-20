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

enum class RENDER_COMMAND_TYPE
{
	RENDER_QUAD,
	RENDER_MESH,
	SET_RENDER_TARGET,
	SET_VIEW_PORT,
	SET_CLEAR_COLOR,
	SET_CLEAR_DEPTH,
	CLEAR,
	ENABLE_DEPTH,
	DISABLE_DEPTH,
};

enum CLEAR_BIT
{
	COLOR = 1 << 0,
	DEPTH = 1 << 1,
	STENCIL = 1 << 2,
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
	void RenderQuad(const glm::vec2& position, const glm::vec2& size, const glm::mat4& projection, Material* material);

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
};
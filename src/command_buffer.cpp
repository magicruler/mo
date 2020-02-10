#include "command_buffer.h"
#include <glad/glad.h>
#include "mesh.h"
#include "camera.h"
#include "render_target.h"
#include "material.h"
#include "gpu_buffer.h"
#include "time_manager.h"
#include "game.h"
#include "scene.h"
#include "component_manager.h"
#include "light.h"
#include "actor.h"
#include "vertex_array.h"

void GLStateCache::Init()
{

}

void CommandBuffer::Clear(unsigned int mask)
{
	std::shared_ptr<CommandClear> cmd = std::make_shared<CommandClear>();
	cmd->mask = mask;

	AddCommand(cmd);
}

void CommandBuffer::SetClearDepth(float depth)
{
	std::shared_ptr<CommandSetClearDepth> cmd = std::make_shared<CommandSetClearDepth>();
	cmd->depth = depth;

	AddCommand(cmd);
}

void CommandBuffer::SetClearColor(const glm::vec4& color)
{
	std::shared_ptr<CommandSetClearColor> cmd = std::make_shared<CommandSetClearColor>();
	cmd->color = color;

	AddCommand(cmd);
}

void CommandBuffer::SetViewport(const glm::vec2& origin, const glm::vec2& size)
{
	std::shared_ptr<CommandSetViewPort> cmd = std::make_shared<CommandSetViewPort>();
	cmd->origin = origin;
	cmd->contentSize = size;

	AddCommand(cmd);
}

void CommandBuffer::SetRenderTarget(RenderTarget* renderTarget)
{
	std::shared_ptr<CommandSetRenderTarget> cmd = std::make_shared<CommandSetRenderTarget>();
	cmd->renderTarget = renderTarget;

	AddCommand(cmd);
}

void CommandBuffer::RenderMesh(Camera* camera, Material* material, SubMesh* mesh, glm::mat4& transformation)
{
	std::shared_ptr<CommandRenderMesh> cmd = std::make_shared<CommandRenderMesh>();
	cmd->camera = camera;
	cmd->material = material;
	cmd->mesh = mesh;
	cmd->transformation = transformation;

	AddCommand(cmd);
}

void CommandBuffer::EnableDepth()
{
	std::shared_ptr<CommandEnableDepth> cmd = std::make_shared<CommandEnableDepth>();
	AddCommand(cmd);
}

void CommandBuffer::Submit()
{
	// TODO Sort And Merge

	// prepare common variables
	std::list<Light*> lights = ComponentManager::GetInstance()->GetLightComponents();
	Scene* scene = Game::ActiveSceneGetPointer();
	stateCahce.currentRenderTarget = nullptr;

	for (auto cmd : commandList)
	{
		switch (cmd->commandType)
		{
		case RENDER_COMMAND_TYPE::CLEAR:
		{
			auto clearCmd = std::static_pointer_cast<CommandClear>(cmd);
			unsigned int bufferBit = 0;
			if ((clearCmd->mask >> 0) && 1)
			{
				bufferBit = bufferBit | GL_COLOR_BUFFER_BIT;
			}

			if ((clearCmd->mask >> 1) && 1)
			{
				bufferBit = bufferBit | GL_DEPTH_BUFFER_BIT;
			}

			if ((clearCmd->mask >> 2) && 1)
			{
				bufferBit = bufferBit | GL_STENCIL_BUFFER_BIT;
			}

			glClear(bufferBit);
		}
			break;
		case RENDER_COMMAND_TYPE::RENDER_MESH:
		{
			auto meshCmd = std::static_pointer_cast<CommandRenderMesh>(cmd);
			auto material = meshCmd->material;
			auto camera = meshCmd->camera;
			auto transformation = meshCmd->transformation;
			auto mesh = meshCmd->mesh;
			auto light = lights.front();

			material->Use();
			std::vector<MaterialExtension> extensions = material->GetExtensions();
			for (auto extension : extensions)
			{
				if (extension == MaterialExtension::MODEL_VIEW_PROJECTION)
				{
					material->SetUniformBlock("CameraBlock", 0);
					camera->GetUniformBlock()->BindBufferBase(BUFFER_USAGE::UNIFORM, 0);

					material->SetMatrix4("model", transformation);
				}
				else if (extension == MaterialExtension::TIME)
				{
					material->SetFloat("time", Time::GetTime());
				}
				else if (extension == MaterialExtension::AMBIENT)
				{
					material->SetVector3("ambient", scene->GetAmbient());
				}
				else if (extension == MaterialExtension::NEAREST_LIGHT)
				{
					// TODO Multiple Light, Forward Renderer
					if (lights.size() > 0)
					{
						Light* light = lights.front();
						material->SetVector3("lightColor", light->GetLightIntensityColor());
						material->SetVector3("lightPos", light->GetOwner()->GetPosition());
					}
				}
				else if (extension == MaterialExtension::CAMERA)
				{
					material->SetVector3("cameraPos", camera->GetOwner()->GetPosition());
				}
			}

			mesh->vertexArray->Bind();
			material->Use();
			glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);
			mesh->vertexArray->UnBind();
		}
			break;
		case RENDER_COMMAND_TYPE::SET_CLEAR_COLOR:
		{
			auto setCmd = std::static_pointer_cast<CommandSetClearColor>(cmd);
			glClearColor(setCmd->color.x, setCmd->color.y, setCmd->color.z, setCmd->color.w);
		}
			break;
		case RENDER_COMMAND_TYPE::SET_CLEAR_DEPTH:
		{
			auto setCmd = std::static_pointer_cast<CommandSetClearDepth>(cmd);
			glClearDepth(setCmd->depth);
		}
			break;
		case RENDER_COMMAND_TYPE::SET_RENDER_TARGET:
		{
			auto setCmd = std::static_pointer_cast<CommandSetRenderTarget>(cmd);
			stateCahce.currentRenderTarget = setCmd->renderTarget;
			setCmd->renderTarget->Bind();
		}
			break;
		case RENDER_COMMAND_TYPE::SET_VIEW_PORT:
		{
			auto setCmd = std::static_pointer_cast<CommandSetViewPort>(cmd);
			glViewport(setCmd->origin.x, setCmd->origin.y, setCmd->contentSize.x, setCmd->contentSize.y);
		}
			break;
		case RENDER_COMMAND_TYPE::ENABLE_DEPTH:
		{
			// auto setCmd = std::static_pointer_cast<CommandEnableDepth>(cmd);
			glEnable(GL_DEPTH_TEST);
		}
		break;
		default :
			spdlog::info("Unsupported Cmd");
			break;
		}
	}

	if (stateCahce.currentRenderTarget != nullptr)
	{
		stateCahce.currentRenderTarget->Unbind();
	}

	commandList.clear();
}
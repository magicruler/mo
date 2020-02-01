#pragma once

class Material;
class Mesh;

enum class RenderCommandType
{
    DRAW_MESH,
};

class RenderCommand
{
public:
    RenderCommandType type;
};

class CommandDrawMesh :public RenderCommand
{
public:
    Mesh* mesh;
    Material* material;
};






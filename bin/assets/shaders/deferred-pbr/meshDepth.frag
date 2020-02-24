#version 420 core

layout (location = 0) out float cameraSpaceZ;

// CameraSapce
in vec3 positionCameraSpace;

void main()
{
    cameraSpaceZ = positionCameraSpace.z;
}
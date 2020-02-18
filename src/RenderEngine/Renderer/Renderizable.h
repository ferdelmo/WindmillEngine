#pragma once
#include <vulkan/vulkan.h>

/*
	Interface to any type of mesh or renderizable thing. For now,
	it only need to override a function to bind commmands to a buffer
*/
class Renderizable
{
public:
	virtual void BindCommandsToBuffer(VkCommandBuffer& cmd) = 0;
};


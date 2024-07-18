#pragma once

#include "Utils.h"
#include "Model.h"

struct Scene 
{
	Scene();

	std::vector<VkUtil::Model> models;
};
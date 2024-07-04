#pragma once

#include "Utils.h"

namespace VkUtil 
{
	/**
		Read a file.

		\param filename a string representing the path to the file
		\returns the contents as a vector of raw binary characters
	*/
	std::vector<char> readFile(std::string filename);
	
	/**
		Make a shader module.

		\param filename a string holding the filepath to the spir-v file.
		\param device the logical device
		\returns the created shader module
	*/
	VkShaderModule CreatShaderModule(std::string filename, VkDevice device);
}
#pragma once

#include <memory>
#include <string>
#include <unordered_map>

namespace Der3318FileSystemComparison
{
	struct FileSystemNode
	{
		std::shared_ptr<FileSystemNode> parent = {};
		std::unordered_map<std::string, std::shared_ptr<FileSystemNode>> children = {};
		std::string name = {};
		uintmax_t bytes = {};
	};

	using Mapping = std::unordered_map<std::string, std::shared_ptr<FileSystemNode>>;
}
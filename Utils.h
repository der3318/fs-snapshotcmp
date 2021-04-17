#pragma once

#include "Interfaces.h"

#include <vector>

namespace Der3318FileSystemComparison
{
	namespace Utils
	{
		std::vector<std::string> SplitPathToDirs(std::string path, wchar_t separator);
		
		Mapping CreateMappingFromSnapshot(std::string filePath);
		
		void SaveAsSnapshot(std::string pathToAnalyze, std::string outputFilePath);
		
		void PrintDiff(Mapping before, Mapping after);
	}
}
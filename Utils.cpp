#include "ColorPrinter.h"
#include "Utils.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>

namespace Der3318FileSystemComparison
{
	std::vector<std::string> Utils::SplitPathToDirs(std::string path, wchar_t separator)
	{
		// indicators and buffer
		size_t start = 0, end = {};
		std::string token = {};
		std::vector<std::string> folders = {};

		// find all substring
		size_t separatorLen = {};
		std::unique_ptr<char[]> separatorStr = std::make_unique<char[]>(2);
		wcstombs_s(&separatorLen, separatorStr.get(), 2, &separator, 2);
		while ((end = path.find(separatorStr.get(), start)) != std::string::npos)
		{
			token = path.substr(start, end - start);
			start = end + 1;
			folders.push_back(token);
		}

		// add last one and return
		folders.push_back(path.substr(start));
		return folders;
	}

	Mapping Utils::CreateMappingFromSnapshot(std::string filePath)
	{
		// mapping from filepath to node
		Mapping pathToNode = {};

		// root node
		const std::string rootName = "file://";
		pathToNode.try_emplace(rootName, new FileSystemNode{ {}, {}, rootName, static_cast<uintmax_t>(0) });

		// file input stream
		std::fstream stream = {};
		stream.open(filePath, std::ios::in);

		// open and read line by line
		if (stream.is_open())
		{
			std::string filenameStr = {};
			std::string parentPathStr = {};
			std::string fileSizeStr = {};

			while (std::getline(stream, filenameStr))
			{
				// it is fine that some empty lines are inserted
				if (filenameStr.empty())
				{
					continue;
				}

				// try get parent path and file size from the next two lines
				if (!std::getline(stream, parentPathStr) || !std::getline(stream, fileSizeStr) || parentPathStr.empty() || fileSizeStr.empty())
				{
					break;
				}

				size_t fileSize = {};
				std::istringstream{ fileSizeStr } >> fileSize;

				std::string cumulativeParentPathStr = rootName;
				for (auto const& folder : SplitPathToDirs(parentPathStr, std::filesystem::path::preferred_separator))
				{
					std::string nextCumulativeParentPathStr = cumulativeParentPathStr + "/" + folder;
					pathToNode.try_emplace(nextCumulativeParentPathStr, new FileSystemNode{ pathToNode[cumulativeParentPathStr], {}, folder, static_cast<uintmax_t>(0) });
					pathToNode[cumulativeParentPathStr]->children.try_emplace(nextCumulativeParentPathStr, pathToNode[nextCumulativeParentPathStr]);
					cumulativeParentPathStr = nextCumulativeParentPathStr;
				}

				const std::string filePathStr = cumulativeParentPathStr + "/" + filenameStr;
				pathToNode.try_emplace(filePathStr, new FileSystemNode{ pathToNode[cumulativeParentPathStr], {}, filenameStr, static_cast<uintmax_t>(0) });
				pathToNode[cumulativeParentPathStr]->children.try_emplace(filePathStr, pathToNode[filePathStr]);

				for (auto nodeItr = pathToNode[filePathStr]; nodeItr; nodeItr = nodeItr->parent)
				{
					nodeItr->bytes += fileSize;
				}
			}

			stream.close();
		}

		// return managed mapping
		return pathToNode;
	}

	void Utils::SaveAsSnapshot(std::string pathToAnalyze, std::string outputFilePath)
	{
		// file output stream
		std::fstream stream = {};
		stream.open(outputFilePath, std::ios::out);

		// open and write line by line
		if (stream.is_open())
		{
			try
			{
				for (auto itEntry = std::filesystem::recursive_directory_iterator(std::filesystem::path{ pathToAnalyze }); itEntry != std::filesystem::recursive_directory_iterator(); ++itEntry)
				{
					if (!itEntry->is_regular_file())
					{
						continue;
					}

					const auto filenameStr = itEntry->path().filename().string();
					const auto parentPathStr = itEntry->path().parent_path().string();
					const auto fileSize = itEntry->file_size();

					stream << filenameStr << std::endl << parentPathStr << std::endl << fileSize << std::endl;
				}
			}
			catch (const std::exception& e)
			{
				ColorPrinter::Print(" WARNING ", ConsoleColor::Black, ConsoleColor::Red);
				ColorPrinter::Print(" exception: " + std::string{ e.what() } + "\n", ConsoleColor::Red, ConsoleColor::Black);
			}
			stream.close();
		}
		else
		{
			ColorPrinter::Print(" ERROR ", ConsoleColor::Black, ConsoleColor::Red);
			ColorPrinter::Print(" something wrong with " + outputFilePath + "\n", ConsoleColor::Red, ConsoleColor::Black);
		}
	}

	void Utils::PrintDiff(Mapping before, Mapping after)
	{
		const std::string rootName = "file://";

		std::queue<std::shared_ptr<FileSystemNode>> bfsQueue = {};
		bfsQueue.push(after[rootName]);

		while (!bfsQueue.empty())
		{
			auto currentNode = bfsQueue.front();
			bfsQueue.pop();

			for (const auto child : currentNode->children)
			{
				auto childToCompare = before.find(child.first);
				if (childToCompare == before.end())
				{
					std::cout << std::endl;
					ColorPrinter::Print(" NEW+ ", ConsoleColor::Black, ConsoleColor::Green);
					ColorPrinter::Print(" (" + std::to_string(child.second->bytes) + " bytes)\n", ConsoleColor::Green, ConsoleColor::Black);
					ColorPrinter::Print(child.first + "\n", ConsoleColor::White, ConsoleColor::Black);
				}
				else if (child.second->children.empty() && child.second->bytes != childToCompare->second->bytes)
				{
					std::cout << std::endl;
					ColorPrinter::Print(" MODIFY* ", ConsoleColor::Black, ConsoleColor::Blue);
					ColorPrinter::Print(" (" + std::to_string(child.second->bytes) + " bytes)\n", ConsoleColor::Blue, ConsoleColor::Black);
					ColorPrinter::Print(child.first + "\n", ConsoleColor::White, ConsoleColor::Black);
				}
				else
				{
					bfsQueue.push(child.second);
				}
			}
		}
	}
}
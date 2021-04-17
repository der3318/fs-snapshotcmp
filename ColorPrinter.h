#pragma once

#include <string>

namespace Der3318FileSystemComparison
{
	enum class ConsoleColor
	{
		Black = 0,
		White = 15,
		Blue = 1,
		Green = 2,
		Red = 4,
		Yellow = 14
	};

	class ColorPrinter
	{
	public:
		static void Print(std::string msg, ConsoleColor foreground, ConsoleColor background);
	};
}
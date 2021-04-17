#include "ColorPrinter.h"

#include <iostream>
#include <windows.h>

namespace Der3318FileSystemComparison
{
    void ColorPrinter::Print(std::string msg, ConsoleColor foreground, ConsoleColor background)
    {
		std::cout << " ";
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO originalInfo = {};
		GetConsoleScreenBufferInfo(hConsole, &originalInfo);
		SetConsoleTextAttribute(hConsole, static_cast<int>(foreground) + static_cast<int>(background) * 16);
		std::cout << msg;
		SetConsoleTextAttribute(hConsole, originalInfo.wAttributes);
    }
}
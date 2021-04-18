#include "ColorPrinter.h"
#include "Interfaces.h"
#include "Utils.h"

#include <filesystem>

using namespace Der3318FileSystemComparison;

int main(int argc, char* argv[])
{
    if (argc < 4)
    {
        ColorPrinter::Print(" USAGE1 ", ConsoleColor::Black, ConsoleColor::Yellow);
        ColorPrinter::Print(" fscmp.exe --snapshot DIR CURRENT.snapshot\n", ConsoleColor::Yellow, ConsoleColor::Black);
        ColorPrinter::Print(" USAGE2 ", ConsoleColor::Black, ConsoleColor::Yellow);
        ColorPrinter::Print(" fscmp.exe --diff BEFORE.snapshot AFTER.snapshot\n", ConsoleColor::Yellow, ConsoleColor::Black);
        return 0;
    }

    const std::string command{ argv[1] };

    if (command.compare("--snapshot") == 0 && std::filesystem::exists(std::filesystem::path{ argv[2] }))
    {
        Utils::SaveAsSnapshot(argv[2], argv[3]);
        ColorPrinter::Print(" INFO ", ConsoleColor::Black, ConsoleColor::Yellow);
        ColorPrinter::Print(" saved snapshot: " + std::string{ argv[2] } + " as " + std::string{ argv[3] } + "\n", ConsoleColor::Yellow, ConsoleColor::Black);
        return 0;
    }

    if (command.compare("--diff") == 0 && std::filesystem::exists(std::filesystem::path{ argv[2] }) && std::filesystem::exists(std::filesystem::path{ argv[3] }))
    {
        ColorPrinter::Print(" INFO ", ConsoleColor::Black, ConsoleColor::Yellow);
        ColorPrinter::Print(" start comparing: " + std::string{ argv[2] } + " vs. " + std::string{ argv[3] } + "\n", ConsoleColor::Yellow, ConsoleColor::Black);
        Mapping pathToNode1 = Utils::CreateMappingFromSnapshot(argv[2]);
        Mapping pathToNode2 = Utils::CreateMappingFromSnapshot(argv[3]);
        Utils::PrintDiff(pathToNode1, pathToNode2);
        return 0;
    }

    ColorPrinter::Print(" ERROR ", ConsoleColor::Black, ConsoleColor::Red);
    ColorPrinter::Print(" something went wrong\n", ConsoleColor::Red, ConsoleColor::Black);
    return 0;
}
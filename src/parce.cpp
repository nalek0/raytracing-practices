#include <fstream>
#include <string>
#include <vector>

#include "parse.hpp"

Command::Command(std::string _name, std::vector<std::string> _args) : command_name(_name), args(_args) {}

std::string Command::getCommandName() const
{
    return command_name;
}

std::vector<std::string> Command::getArgs() const
{
    return args;
}

CommandScanner::CommandScanner(std::string _filename) : filename(_filename) {}

void CommandScanner::parse(std::vector<Command> &result)
{
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line))
    {
        std::vector<std::string> words;
        std::string better_line = line + ' ';
        std::string current = "";

        for (char c : better_line)
        {
            if (c != ' ')
            {
                current += c;
            }
            else
            {
                if (current != "")
                {
                    words.push_back(current);
                    current = "";
                }
            }
        }

        if (words.size() >= 1)
        {
            std::string command_name = words[0];
            std::vector<std::string> args = std::vector<std::string>();

            for (int i = 1; i < words.size(); i++)
            {
                args.push_back(std::string(words[i]));
            }

            Command command = Command(command_name, args);
            result.push_back(command);
        }
    }

    Command eof_command = Command("EOF", {});
    result.push_back(eof_command);
}

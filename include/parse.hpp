#pragma once

#include <vector>
#include <string>

class Command
{
private:
    std::string command_name;
    std::vector<std::string> args;

public:
    Command(std::string _name, std::vector<std::string> _args);
    std::string getCommandName() const;
    std::vector<std::string> getArgs() const;
};

class CommandScanner
{
private:
    std::string filename;

public:
    CommandScanner(std::string _filename);
    void parse(std::vector<Command> &result);
};

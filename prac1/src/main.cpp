#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct pixel {
    char r;
    char g;
    char b;
};

struct point {
    float x;
    float y;
    float z;
};

struct quternion {
    float x;
    float y;
    float z;
    float w;
};

class Command {
private:
    
    std::string command_name;

    std::vector<std::string> args;

public:

    Command(std::string _name, std::vector<std::string> _args) : command_name(_name), args(_args) {}

    std::string getCommandName() {
        return command_name;
    }

    std::vector<std::string> getArgs() {
        return args;
    }

};

class CommandScanner {
private:

    std::string filename;
    
public:

    CommandScanner(std::string _filename) : filename(_filename) {}

    void parse(std::vector<Command>& result) {
        std::ifstream file(filename);
        std::string line;

        while (std::getline(file, line)) {
            std::vector<std::string> words;
            std::string better_line = line + ' ';
            std::string current = "";

            for (char c : better_line) {
                if (c != ' ') {
                    current += c;
                } else {
                    if (current != "") {
                        words.push_back(current);
                        current = "";
                    }
                }
            }

            if (words.size() >= 1) {
                std::string command_name = words.at(0);
                std::vector<std::string> args = std::vector(words.begin() + 1, words.end());
                Command command = Command(command_name, args);
                result.push_back(command);
            }
        }
    }

};

int main() {
    std::string input_file = "input.txt";
    CommandScanner scanner = CommandScanner(input_file);
    std::vector<Command> result = std::vector<Command>();
    scanner.parse(result);

    for (Command cmd : result) {
        std::cout << cmd.getCommandName() << " : ";

        for (std::string arg : cmd.getArgs()) {
            std::cout << arg << " ";
        }

        std::cout << std::endl;
    }

    return 0;
}

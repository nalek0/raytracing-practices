#include <fstream>
#include <string>
#include <vector>

class Command {
private:
    
    std::string command_name;

    std::vector<float> args;

public:

    Command(std::string _name, std::vector<float> _args) : command_name(_name), args(_args) {}

    std::string getCommandName() {
        return command_name;
    }

    std::vector<float> getArgs() {
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
                std::string command_name = words[0];
                std::vector<float> args = std::vector<float>();

                for (int i = 1; i < words.size(); i++) {
                    float arg = std::stof(words[i]);
                    args.push_back(arg);
                }

                Command command = Command(command_name, args);
                result.push_back(command);
            }
        }
    }

};

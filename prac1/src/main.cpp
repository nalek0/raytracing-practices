#include <iostream>

#include "parce.cpp"

int main() {
    std::string input_file = "input.txt";
    CommandScanner scanner = CommandScanner(input_file);
    std::vector<Command> result = std::vector<Command>();
    scanner.parse(result);

    for (Command cmd : result) {
        std::cout << cmd.getCommandName() << " : ";

        for (float arg : cmd.getArgs()) {
            std::cout << arg << " ";
        }

        std::cout << std::endl;
    }

    return 0;
}

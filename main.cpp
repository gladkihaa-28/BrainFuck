#include <iostream>
#include <csignal>
#include <unistd.h>
#include <unordered_map>
#include <string>
#include <vector>
using namespace std;

void signalHandler(int signum) {
    exit(0);
}

class Interpreter {
private:
    char* stack;
    int ind = 0;
    unordered_map<string, int> labels;
    vector<string> commands;
    int commandIndex = 0;

public:
    Interpreter() {
        cout << "Starting BrainFuck Shell..." << endl;
        stack = new char[30000];
    }

    ~Interpreter() {
        cout << "Goodbye!" << endl;
        delete[] stack;
    }

    void parseCommands() {
        while (true) {
            try {
                string input;
                cout << "BrainFuck>";
                getline(cin, input);
                if (input == "@") {
                    break;
                } else {
                    commands.push_back(input);
                }
            } catch (...) {
                break;
            }
        }

        for (int i = 0; i < commands.size(); ++i) {
            if (commands[i][0] == ':') {
                string label = commands[i].substr(1);
                labels[label] = i;
            }
        }
    }

    void interpret() {
        while (commandIndex < commands.size()) {
            try {
                string command = commands[commandIndex];
                if (command[0] == '+') {
                    stack[ind]++;
                } else if (command[0] == '-') {
                    stack[ind]--;
                } else if (command[0] == '>') {
                    ind++;
                } else if (command[0] == '<') {
                    ind--;
                } else if (command[0] == '.') {
                    cout << stack[ind] << endl;
                } else if (command[0] == ',') {
                    cin >> stack[ind];
                } else if (command[0] == 'g') {
                    string label = command.substr(1);
                    if (labels.find(label) != labels.end()) {
                        commandIndex = labels[label];
                        continue;
                    } else {
                        cerr << "Label not found: " << label << endl;
                    }
                } else if (command[0] == ':') {
                    // pass
                }
                commandIndex++;
            } catch (...) {
                break;
            }
        }
    }
};

int main() {
    signal(SIGTERM, signalHandler);
    Interpreter interpreter;
    interpreter.parseCommands();
    interpreter.interpret();
    return 0;
}
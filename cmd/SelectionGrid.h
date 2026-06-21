#ifndef SIGNAL_ANALYSER_SELECTIONGRID_H
#define SIGNAL_ANALYSER_SELECTIONGRID_H

#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <conio.h>
#include <thread>

#include <vector>

class SelectionGrid {
private:
    const std::vector<std::string> items;
    std::string box;
    unsigned int tabPos = 0;

    void generateGrid(const std::string& container) {
        for (int i = 0; i < items.size(); i++) {
            std::cout << "\033[0m"; // Reset

            if (i % 5 == 0 && i != 0) std::cout << "\n";
            if (i + 1 == tabPos) std::cout << "\033[30;107m"; // Highlight

            std::stringstream item(container);
            item.seekg(0);
            item << items[i].c_str();
            std::cout << item.str();
        }
        std::cout << "\033[0m" << std::endl;
    }

    static void clearRows(unsigned int rows) {
        for (unsigned int i = 0; i < rows; i++) {
            std::cout << "\033[F" << "\33[2K"; // Moves up and clear
        }
        std::cout.flush();
    }

    void refreshTable(const std::string& input) {
        if (tabPos != 0) {
            tabPos = 0;
            clearRows(1);
            generateGrid(box);

            std::cout << "\33[2K" << "\r" << "\033[1;92m" + input; // clear & rewrite
        }
    }

    std::string tabCompleter() {
        if (tabPos == items.size()) {
            tabPos = 1;
        } else tabPos += 1;

        clearRows(1);
        generateGrid(box);

        std::cout << "\33[2K" << "\033[1;92m";
        return items[tabPos - 1];
    }

    // manually handing the input functionality by hand for more sophisticated control over inputs, also only works on windows now
    // I regret my life choices >:(
    std::string getInput() {
        std::string input;

        std::cout << "\033[1;92m"; // Set colour green

        bool running = true;
        while (running) {
            char ch = _getch();

            if (ch == 0 || ch == 224 || ch == -32) {
                _getch();
                continue;
            }

            switch (ch) {
                case 13: // Enter
                    if (!input.empty() && std::find(items.begin(), items.end(), input) != items.end()) {
                        running = false;
                        refreshTable(input);
                    }
                    break;
                case 9: // Tab
                    input = tabCompleter();
                    std::cout << "\33[2K" << "\r" << input;
                    break;
                case 8: // backspace
                    if (!input.empty()) {
                        input.pop_back(); // Del last character
                        refreshTable(input);
                    }
                    std::cout << "\b \b";
                    break;
                default:
                    if ((ch >= 32 && ch <= 126)){
                        input += ch;
                        std::cout << ch;
                        refreshTable(input);
                    }
            }
        }

        std::cout << "\033[0m" << std::endl;

        return input;
    }

public:
    std::string result;

    explicit SelectionGrid(const std::vector<std::string>& items) : items(items) {}

    void create() {
        std::cout << "Please select on of the following: " << std::endl << std::endl;

        // find the largest string in items
        unsigned int max = items[0].length();

        for (const std::string& name : items) {
            if (name.size() > max) {
                max = name.length();
            }
        }

        // Generates a container to store each element in
        for (int i = 0; i < max + 7; i++) {
            box += " ";
        }


        generateGrid(box);

        // get user input
        result = getInput();
    }
};

#endif //SIGNAL_ANALYSER_SELECTIONGRID_H

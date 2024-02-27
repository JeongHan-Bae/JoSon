#include "../include/JoSon/Viso.h"
#include <iostream>
#include <vector>

namespace JoSon::Viso {
    bool colorful = true;

    std::string str_color = "\033[1;32m"; // Green color for strings

    std::string digit_color = "\033[1;36m"; // Cyan color for digits

    std::string key_color = "\033[1;31m"; // Red color for keywords

    unsigned int progress_step = 1; // One percent by default

    [[maybe_unused]] void json_print(const std::string& json_str, int indents) {
        bool sync_state = std::ostream::sync_with_stdio(false);
        // Save the current sync state
        std::vector<std::string> indent_strings(16);
        for (size_t i = 0; i < 16; ++i) {
            indent_strings[i] = std::string(indents * i, ' ');
        }

        auto get_indents = [&indent_strings](size_t i) {
            return (i < 16) ? indent_strings[i] : indent_strings[15];
        };

        int level = 0;            // Initialize the indentation level
        bool begin_point = false; // Flag to indicate if a new line has started

        for (size_t i = 0; i < json_str.size(); ++i) {
            char c = json_str[i]; // Current character being processed

            // Handle opening curly braces
            if (c == '{') {
                level++;
                std::cout << c;
            }
                // Handle closing curly braces
            else if (c == '}') {
                level--; // Decrement the indentation level
                std::cout << get_indents(level) << c;
            }
                // Handle newline characters
            else if (c == '\n') {
                std::cout << c;     // Output the newline character
                begin_point = true; // Set the flag to indicate a new line
                continue;
            }
                // Handle other characters
            else {
                if (begin_point) {
                    std::cout << get_indents(level);
                }
                if (c == '\"') { // Handle string characters
                    if (colorful)
                        std::cout << str_color; // Set string color
                    std::cout << c;
                    while (true) {
                        char next = json_str[++i];
                        if (next == '\"') {
                            std::cout << next;
                            break;
                        } else {
                            std::cout << next;
                        }
                    }
                    if (colorful)
                        std::cout << "\033[0m"; // Reset color to normal
                }
                    // Handle digits
                else if (std::isdigit(c) || c == '.') {
                    if (colorful)
                        std::cout << digit_color; // Set digits color
                    std::cout << c;
                    if (colorful)
                        std::cout << "\033[0m"; // Reset color to normal
                }
                    // Handle keywords (True, False, NullPtr)
                else if (std::toupper(static_cast<unsigned char>(c)) == 'T' ||
                         std::toupper(static_cast<unsigned char>(c)) == 'F' ||
                         std::toupper(static_cast<unsigned char>(c)) == 'N') {
                    std::string match;
                    if (c == 'T')
                        match = "True";
                    else if (c == 't')
                        match = "true";
                    else if (c == 'F')
                        match = "False";
                    else if (c == 'f')
                        match = "false";
                    else if (c == 'N')
                        match = "NullPtr";
                    else if (c == 'n')
                        match = "null";
                    std::string candidate = json_str.substr(i, match.size());
                    bool isMatch = true;
                    for (size_t j = 0; j < match.size(); ++j) {
                        if (candidate[j] != match[j]) {
                            isMatch = false;
                            break;
                        }
                    }
                    if (isMatch) {
                        if (colorful)
                            std::cout << key_color; // Set keywords color
                        std::cout << candidate;
                        i += match.size() - 1;
                        // Move the index past the matched keyword
                        if (colorful)
                            std::cout << "\033[0m"; // Reset color to normal
                    } else {
                        std::cout << c;
                    }
                } else {
                    std::cout << c;
                }
            }
            begin_point = false; // Reset the flag indicating the start of a line
        }
        std::ostream::sync_with_stdio(sync_state);
        // Restore the original sync state
    }

    void ProgressBar::update() {
        const int progressBarWidth = 50;
        double rate = static_cast<double>(static_cast<long long>(*progress)) /
                      static_cast<double>(static_cast<long long>(*total));
        int pos = static_cast<int>(rate * 200);
        int new_percentage = static_cast<int>(rate * 100.0);

        if (new_percentage < 100 && new_percentage - percentage < progress_step) {
            return;
        } else {
            percentage = new_percentage;
        }
        bool sync_state = std::ostream::sync_with_stdio(false);
        // Save the current sync state

        // Define colors based on rate
        std::string color;
        if (colorful) {
            if (rate < 0.25)
                color = "\033[1;31m"; // Red
            else if (rate < 0.5)
                color = "\033[1;33m"; // Yellow
            else if (rate < 0.75)
                color = "\033[1;32m"; // Green
            else
                color = "\033[1;36m"; // Cyan
        }

        if (*progress < *total) {
            std::string block_str(pos / 4, '#');
            std::string rest_str(progressBarWidth - pos / 4 - 1, '.');

            std::cout << '\r' << std::string(80, ' ') << '\r';
            if (colorful)
                std::cout << color;
            std::cout << '[' << block_str;
            switch (pos % 4) {
                case 0:
                    std::cout << '/';
                    break;
                case 1:
                    std::cout << '-';
                    break;
                case 2:
                    std::cout << '\\';
                    break;
                case 3:
                    std::cout << '%';
                    break;
            }
            std::cout << rest_str << (!colorful ? "]" : "]\033[0m");
            std::cout << (percentage < 10 ? "   " : "  ") << percentage << '%';
        } else {
            std::string block_str(progressBarWidth, '#');
            std::cout << '\r' << std::string(80, ' ') << '\r';
            if (colorful)
                std::cout << color;
            std::cout << '[' << block_str << (!colorful ? "]" : "]\033[0m");
            std::cout << " 100%";
        }
        std::cout.flush();
        std::ostream::sync_with_stdio(sync_state);
    }
} // namespace JoSon::Viso

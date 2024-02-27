// JoSon.cpp
#include <cmath>
#include <fstream>
#include <iostream>
#include <stack>

#include "../include/JoSon/Doc.h"
#include "../include/JoSon/Joson.h"

[[maybe_unused]] void
JoSon::Utils::store_doc_to_json(const std::string& path,
                                const JoSon::Doc& json_doc) {
    bool sync_state = std::ostream::sync_with_stdio(false);
    std::ofstream file(path);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file '" << path << "' for writing."
                  << std::endl;
        return;
    }

    if (json_doc.get_type() == JoSon::Type::Dict) {
        file << json_doc;
    } else {
        JoSon::Doc to_store(JoSon::Type::Dict);
        to_store.upsert("Welcome to JoSon", json_doc);
        file << to_store;
    }
    std::ostream::sync_with_stdio(sync_state);
    file.close(); // Close the file stream
}

/**
 * @brief Constructs a primitive Doc object from a substring of the input
 * string.
 *
 * @param input The input string.
 * @param pos A pointer to the position in the input string.
 * @param fin The final character to stop parsing at.
 * @return A primitive Doc object.
 */
inline JoSon::Doc string_to_prim_doc(const char* input, size_t* pos,
                                     char fin = ' ') {
    // String
    if (input[*pos] == '\"') {
        size_t parse = ++(*pos);
        while (input[parse] != '\"') {
            ++parse;
        }
        char* str =
                new char[parse - (*pos) + 1]; // Allocate memory for C-string
        // (+1 for null terminator)
        std::copy(input + *pos, input + parse, str);
        // Copy substring contents to C-string
        str[parse - (*pos)] = '\0'; // Add null terminator
        *pos = parse + 1;
        return JoSon::Doc(static_cast<const char*>(str));
    }
        // Bool
    else if (strncmp(input + *pos, "true", 4) == 0) {
        *pos += 4;
        return JoSon::Doc(true);
    }
        // Check for "false"
    else if (strncmp(input + *pos, "false", 5) == 0) {
        *pos += 5;
        return JoSon::Doc(false);
    }
        // Check for "null"
    else if (strncmp(input + *pos, "null", 4) == 0) {
        *pos += 4;
        return JoSon::Doc(JoSon::Type::Nullptr);

    } else if (input[*pos] == '+' || input[*pos] == '-' || input[*pos] == '.' ||
               (input[*pos] >= '0' && input[*pos] <= '9')) {
        bool sign = true;                 // Default sign is positive
        JoSon::Type t = JoSon::Type::Int; // Default type is integer
        size_t floating = 0; // Count of digits after the decimal point
        size_t len = 0;      // Length of the number
        int int_val = 0;
        long long long_val = 0L;
        double decimal = 0.0;

        // Check sign
        if (input[*pos] == '-') {
            sign = false;
            ++(*pos);
        } else if (input[*pos] == '+') {
            ++(*pos);
        }

        // Check if the number is a floating-point number
        bool has_point = false;
        if (input[*pos] == '.') {
            has_point = true;
            t = JoSon::Type::Double;
            ++(*pos);
        }

        // Read digits
        while ((input[*pos] >= '0' && input[*pos] <= '9') ||
               input[*pos] == '.') {
            if (input[*pos] == '.') {
                if (has_point) {
                    return JoSon::Doc(JoSon::Type::Nullptr);
                }
                has_point = true;
                if (t == JoSon::Type::Int) {
                    decimal = static_cast<double>(int_val);
                } else if (t == JoSon::Type::LLong) {
                    decimal = static_cast<double>(long_val);
                }
                t = JoSon::Type::Double;
                ++(*pos);
                continue;
            }
            if (t == JoSon::Type::Int && len == 9) {
                // Switch to long_long if the number is too long for int
                t = JoSon::Type::LLong;
                long_val = static_cast<long long>(int_val);
            }

            if (t == JoSon::Type::LLong && len == 16) {
                // Switch to double if the number is too long for long
                t = JoSon::Type::Double;
                decimal = static_cast<double>(long_val);
            }

            if (has_point) {
                ++floating;
            }

            if (t == JoSon::Type::Int) {
                int_val *= 10;
                int_val += input[*pos] - '0';
            } else if (t == JoSon::Type::LLong) {
                long_val *= 10;
                long_val += input[*pos] - '0';
            } else {
                decimal *= 10;
                decimal += input[*pos] - '0';
            }
            ++len;
            ++*pos;
        }

        decimal *= pow(10.0, static_cast<int>(-floating));

        if (input[*pos] == 'e' || input[*pos] == 'E') {
            if (t == JoSon::Type::Int) {
                decimal = static_cast<double>(int_val);
            } else if (t == JoSon::Type::LLong) {
                decimal = static_cast<double>(long_val);
            }
            t = JoSon::Type::Double;
            ++(*pos);
            bool e_sign = true;
            if (input[*pos] == '+' || input[*pos] == '-') {
                e_sign = (input[*pos] != '-');
                ++(*pos);
            }
            double exp = 0.0;
            while (input[*pos] >= '0' && input[*pos] <= '9') {
                exp *= 10;
                exp += input[*pos] - '0';
                ++(*pos);
            }
            decimal *= pow(10.0, e_sign ? exp : -exp);
        }

        if (input[*pos] == '\0' || input[*pos] == ' ' || input[*pos] == ',' ||
            input[*pos] == '\t' || input[*pos] == '\n' || input[*pos] == '\r' ||
            input[*pos] == fin) {
            // All characters parsed successfully
            if (t == JoSon::Type::Int) {
                return JoSon::Doc(sign ? int_val : -int_val);
            } else if (t == JoSon::Type::LLong) {
                return JoSon::Doc(sign ? long_val : -long_val);
            } else {
                if (!sign) {
                    decimal *= -1;
                }
                return JoSon::Doc(decimal);
            }
        }
    }
    // the format is not respected, read to the end
    while (input[*pos] != '\0' && input[*pos] != ',' && input[*pos] != fin) {
        ++(*pos);
    }
    return JoSon::Doc(JoSon::Type::Nullptr); // Default case, treated as null
}

[[nodiscard]] JoSon::Doc JoSon::Utils::string_to_doc(const std::string& input,
                                                     bool show_bar) {
    size_t start = 0;
    size_t end = input.size() - 1;

    // Find the first non-null char from the beginning
    while (start <= end && (input[start] == ' ' || input[start] == '\t' ||
                            input[start] == '\n' || input[start] == '\r' ||
                            input[start] == '\0')) {
        ++start;
    }

    // Find the first non-null char from the end
    while (end >= start &&
           (input[end] == ' ' || input[end] == '\t' || input[end] == '\n' ||
            input[end] == '\r' || input[end] == '\0')) {
        --end;
    }

    // Empty or all spaces
    if (start > end) {
        std::cerr << "Error: Empty or invalid JSON content." << std::endl;
        return Doc(Type::Nullptr);
    }

    std::stack<Doc> ge_stk;
    size_t count;
    if (input[start] == '{' && input[end] == '}') {
        count = start + 1;
        Doc res(Type::Dict);
        ge_stk.emplace(res);
    } else if (input[start] == '[' && input[end] == ']') {
        count = start + 1;
        Doc res(Type::Array);
        ge_stk.emplace(res);
    } else if (input[start] != '[' && input[end] != ']' &&
               input[start] != '{' && input[end] != '}') {
        count = start;
        return string_to_prim_doc(input.c_str(), &count);
    } else {
        // Wrong format
        return Doc(Type::Nullptr);
    }
    const size_t totalCharacters = input.size();
    if (show_bar) {
        std::cout << "\nParsing..." << '\n';
    }
    const auto input_str = input.c_str();
    Viso::ProgressBar progressBar(reinterpret_cast<std::atomic<size_t> *const>(&count),
                                  reinterpret_cast<const std::atomic<size_t> *>(&totalCharacters));
    while (count < totalCharacters) {

        if (input_str[count] == ' ' || input_str[count] == '\n' ||
            input_str[count] == '\t' || input_str[count] == '\r') {
            ++count;
            continue;
        }
        auto doc = ge_stk.top();

        if (input_str[count] == ',') {
            ++count;
            continue;
        } else if (input_str[count] == '}' || input_str[count] == ']') {
            // reaching the end of this object
            ++count;
            ge_stk.pop();
            if (ge_stk.empty()) {
                if (show_bar) {
                    progressBar.update();
                    std::cout << "\nProgress Finished.\n";
                }
                return doc;
            }
            continue;
        }
        if (doc.get_type() == Type::Dict) {
            size_t next = count + 1;
            while (next < totalCharacters) {
                if (input_str[next] == ':') {
                    break;
                }
                ++next;
            }
            if (next == totalCharacters) {
                count = next;
                break;
            }
            size_t left = count;
            size_t right = next - 1;
            if (input_str[left] == '"') {
                ++left;
            }
            while (left < right) {
                if (input_str[right] == ' ' || input_str[right] == '\n' ||
                    input_str[right] == '\t' || input_str[right] == '\r') {
                    --right;
                } else {
                    break;
                }
            }

            if(input_str[right] != '"'){
                ++right;
            }

            char* str = new char[right - left + 1];
            // Allocate memory for C-string (+1 for null terminator)
            std::copy(input_str + left, input_str + right, str);
            // Copy substring contents to C-string
            str[right - left] = '\0'; // Add null terminator
            auto key = static_cast<const char*>(str);

            count = next + 1;
            while (count < totalCharacters) {
                if (input_str[count] == ' ' || input_str[count] == '\n' ||
                    input_str[count] == '\t' || input_str[count] == '\r') {
                    ++count;
                    continue;
                } else {
                    break;
                }
            }

            if (count == totalCharacters) {
                break;
            }
            if (input_str[count] == '{') {
                ++count;
                Doc new_doc(Type::Dict);
                doc.upsert(key, new_doc);
                ge_stk.emplace(new_doc);
            } else if (input_str[count] == '[') {
                ++count;
                Doc new_doc(Type::Array);
                doc.upsert(key, new_doc);
                ge_stk.emplace(new_doc);
            } else if (input_str[count] == '}' || input_str[count] == ']' ||
                       input_str[count] == ',') {
                count = totalCharacters;
                doc.upsert(key, Doc(Type::Nullptr));
            } else {
                // Primitive types
                auto new_doc = string_to_prim_doc(input_str, &count, '}');
                doc.upsert(key, new_doc);
            }
        } else if (doc.get_type() == Type::Array) {
            if (input_str[count] == '{') {
                ++count;
                Doc new_doc(Type::Dict);
                doc.emplace_back(new_doc);
                ge_stk.emplace(new_doc);
            } else if (input_str[count] == '[') {
                ++count;
                Doc new_doc(Type::Array);
                doc.emplace_back(new_doc);
                ge_stk.emplace(new_doc);
            } else if (input_str[count] == '}') {
                count = totalCharacters;
                doc.emplace_back(Doc(Type::Nullptr));
            } else {
                // Primitive types
                auto new_doc = string_to_prim_doc(input_str, &count, ']');
                doc.emplace_back(new_doc);
            }
        }
        if (show_bar) {
            progressBar.update();
        }
    }
    if (show_bar) {
        progressBar.update();
        std::cout << "\nProgress Finished.\n";
    }

    while (!ge_stk.empty()) {
        auto doc = ge_stk.top();
        ge_stk.pop();
        if (ge_stk.empty()) {
            return doc;
        }
    }
    Doc* res = new Doc(Type::Nullptr);
    return *res;
}

[[nodiscard]] [[maybe_unused]] JoSon::Doc
JoSon::Utils::read_json_file(const std::string& file_path, bool show_bar) {
    std::ifstream jsonFile(file_path);
    if (!jsonFile.is_open()) {
        std::cerr << "Error: Unable to open JSON file." << std::endl;
        return Doc(Type::Nullptr);
    }

    std::string jsonStr;
    std::string line;

    // Display progress bar if show_bar is true
    if (show_bar) {
        size_t totalLines = 0;

        // Calculate total number of lines
        while (std::getline(jsonFile, line)) {
            ++totalLines;
        }

        // Reset file pointer to the beginning
        jsonFile.clear();
        jsonFile.seekg(0);
        std::cout << "\nReading JSON file...\n";
        // Reset line count
        size_t lineCount = 0;

        Viso::ProgressBar progressBar(reinterpret_cast<std::atomic<size_t> *const>(&lineCount),
                                      reinterpret_cast<const std::atomic<size_t> *>(&totalLines));

        while (std::getline(jsonFile, line)) {
            jsonStr += line;
            ++lineCount;

            progressBar.update();
        }
        jsonFile.close();
    } else {
        while (std::getline(jsonFile, line)) {
            jsonStr += line;
        }
        jsonFile.close();
    }

    // Convert JSON string to Doc
    return string_to_doc(jsonStr, show_bar);
}

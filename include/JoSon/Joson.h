// JoSon.h
#pragma once

#include "Doc.h"
#include "Viso.h"
#include <string>

namespace JoSon::Utils {

    /**
     * @brief Stores a document as JSON in a file.
     *
     * @param path The file path to store the JSON.
     * @param json_doc The document to be stored as JSON.
     * @param space_counts The number of spaces for indentation in the stored JSON
     * file.
     */
    [[maybe_unused]] void store_doc_to_json(const std::string& path, const Doc& json_doc);

    /**
     * @brief Converts a JSON-formatted string into a hierarchical document
     * structure.
     *
     * This function parses a JSON-formatted string and constructs a hierarchical
     * document structure representing the data. It iterates over the string and
     * builds the document based on the JSON syntax.
     *
     * @param input_str The JSON-formatted string to be parsed.
     * @param show_bar Flag indicating whether to display a progress bar while
     * parsing the string.
     * @return A hierarchical document structure representing the parsed JSON data.
     */
    [[nodiscard]] Doc string_to_doc(const std::string& input_str, bool show_bar = false);

    /**
     * @brief Reads a JSON file and converts its contents into a hierarchical
     * document structure.
     *
     * This function opens the specified JSON file, reads its contents, and converts
     * them into a hierarchical document structure representing the JSON data. It
     * optionally displays a progress bar during the file reading process if the
     * `show_bar` parameter is set to true.
     *
     * @param file_path The path to the JSON file to be read.
     * @param show_bar Flag indicating whether to display a progress bar during file
     * reading.
     * @return A hierarchical document structure representing the JSON data read
     * from the file.
     */
    [[nodiscard]] [[maybe_unused]] Doc read_json_file(const std::string& file_path, bool show_bar = false);
} // namespace JoSon::Utils

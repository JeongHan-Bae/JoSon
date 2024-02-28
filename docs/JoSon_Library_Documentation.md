# Using JoSon Library

## Overview

This documentation provides a comprehensive guide on how to use the JoSon library to handle JSON data effectively. JoSon offers intuitive interfaces for creating, manipulating, and visualizing structured data in JSON format.

## Example Code

The following example demonstrates various functionalities of the JoSon library:

```cpp
#include "Joson.h"
#include <iostream>

using JoSon::Doc, JoSon::DocTuple, JoSon::DocArr, JoSon::DictObj, JoSon::Type;

int main() {

    // Create a DictObj to hold the entire JSON structure
    Doc joson_doc(Type::Dict);

    // Configure visualization settings
    JoSon::Viso::colorful = true;
    JoSon::Viso::progress_step = 10;
    JoSon::Viso::str_color = "\033[1;33m";

    // Define the JSON string for the introduction section
    std::string introduction_str = R"({
        "Welcome": "Welcome to the JoSon Library!",
        "Author": "Created by JeongHan-Bae",
        "Version": "1.0",
        "Features": {
            "Data Types": ["Array", "Tuple", "Map", "Primitive Types"],
            "Supported Read-in Data Types": ["Array", "Boolean", "Double", "Integer", "Map", "Null", "String"]
        }
    })";

    // Convert the JSON string to a Doc object without progress bar by default
    Doc introduction = JoSon::Utils::string_to_doc(introduction_str);

    // Insert the introduction section into the main document
    joson_doc.upsert("JoSon Library", introduction);

    // Populate the "Examples" section of the document
    joson_doc.upsert("Examples", Type::Dict);
    joson_doc["Examples"].upsert("Primitive Types", Type::Dict);
    joson_doc["Examples"]["Primitive Types"].upsert("Integer", 3000);
    joson_doc["Examples"]["Primitive Types"].upsert("Double", 3.1416);
    joson_doc["Examples"]["Primitive Types"].upsert("Character (J)", 'J');
    joson_doc["Examples"]["Primitive Types"].upsert("Boolean", true);
    joson_doc["Examples"]["Primitive Types"].upsert("Null", Type::Nullptr);
    joson_doc["Examples"]["Primitive Types"].upsert("String","Hello JoSon!");

    // Populate the "Container Types" section of the document
    joson_doc["Examples"].upsert("Container Types", Type::Dict);
    joson_doc["Examples"]["Container Types"].upsert("Tuple", Type::Tuple);
    joson_doc["Examples"]["Container Types"]["Tuple"].get_tuple().set_values(
            {Doc("Welcome"), Doc("to"), Doc("JoSon")});
    joson_doc["Examples"]["Container Types"].upsert("ArrayList",Type::Array);
    joson_doc["Examples"]["Container Types"]["ArrayList"].emplace_back("Let's");
    joson_doc["Examples"]["Container Types"]["ArrayList"].emplace_back("GO!");

    // Print the JoSon document with visualization
    std::cout << "\nJoSon with visualization:\n";
    JoSon::Viso::json_print(joson_doc.str(true));

    // Print the JoSon document without visualization
    std::cout << "\nJoSon without visualization:\n";
    JoSon::Viso::json_print(joson_doc.str());

    // Store the JoSon document to a JSON file
    std::cout << "\nStore to file...\n";
    JoSon::Utils::store_doc_to_json("HelloJoSon.json", joson_doc);

    // Read the JoSon document from the JSON file with a progress bar
    Doc rd = JoSon::Utils::read_json_file("HelloJoSon.json", true);

    // Print the JoSon document read from the file with colors
    std::cout << "\nJoSon from file:\n";
    JoSon::Viso::json_print(rd.str());
    
    // Print the JoSon document read from the file with default output
    std::cout << '\n' << rd;

    return 0;
}
```

## Example Output

The example code generates the following JSON output:

```json
{
  "JoSon Library": {
    "Welcome": "Welcome to the JoSon Library!",
    "Version": "1.0",
    "Author": "Created by JeongHan-Bae",
    "Features": {
      "Data Types": ["Array", "Tuple", "Map", "Primitive Types"],
      "Supported Read-in Data Types": ["Array", "Boolean", "Double", "Integer", "Map", "Null", "String"]
    }
  },
  "Examples": {
    "Primitive Types": {
      "Integer": 3000,
      "Double": 3.1416,
      "Character (J)": 74,
      "Boolean": true,
      "String": "Hello JoSon!",
      "Null": null
    },
    "Container Types": {
      "Tuple": ["Welcome", "to", "JoSon"],
      "ArrayList": ["Let's", "GO!"]
    }
  }
}
```

## Example CMakeLists

Here's a sample CMakeLists.txt file to integrate the JoSon library into your project:

```cmake
cmake_minimum_required(VERSION 3.24)
project(YourProject)

set(CMAKE_CXX_STANDARD 17)

add_executable(YourProject main.cpp
)

# Set the path to the JoSon library
set(JOSON_DIR "Path/to/JoSonLib/include/JoSon")
set(JOSON_LIB_DIR "${JOSON_DIR}/../../lib")

# Include the directory containing JoSon.h
target_include_directories(YourProject PRIVATE "${JOSON_DIR}")

# Link against the JoSon library
target_link_libraries(YourProject PRIVATE "${JOSON_LIB_DIR}/libJoSon.dll")

# Set the working directory for the Test executable
set_target_properties(YourProject PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/YourProject")

# Copy the DLL file from JoSon to YourProject during build
add_custom_command(TARGET YourProject POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${JOSON_LIB_DIR}/libJoSon.dll"
        "$<TARGET_FILE_DIR:YourProject>"
        COMMENT "Copying libJoSon.dll to YourProject output directory"
)
```

Make sure to replace `"Path/to/JoSonLib"` with the actual path to the JoSon library directory in your system.

For detailed API reference, see [JoSon API Reference Documentation](JoSon_API_Reference_Documentation.md).

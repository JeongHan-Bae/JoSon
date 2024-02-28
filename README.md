# JoSon - Lightweight JSON Processing Library for C++

JoSon is a lightweight and user-friendly JSON processing library for C++, 
designed with simplicity and ease of use in mind. It offers intuitive interfaces for creating, 
manipulating, and visualizing structured data in JSON format.

## Project Structure

```markdown
JoSonLib
│
├── include
│   └── JoSon
│       ├── Joson.h
│       ├── Viso.h
│       └── Doc.h
│
├── src
│   ├── Joson.cpp
│   ├── Viso.cpp
│   └── Doc.cpp
│
├── lib
│   └── (DLL files)
│
├── CMakeLists.txt
│
├── README.md
│
└── docs
    ├── JoSon_API_Reference_Documentation.md
    └── JoSon_Library_Documentation.md
```

The project structure organizes source code, libraries, and documentation under a cohesive hierarchy.

## Documentation

Detailed library documentation is available in two parts:

- **Library Overview**: 

    The JoSon library overview and usage instructions can be found in [JoSon_Library_Documentation.md](docs/JoSon_Library_Documentation.md).

- **API Reference**: 

    For detailed API documentation, refer to [JoSon_API_Reference_Documentation.md](docs/JoSon_API_Reference_Documentation.md).

## Security Policy

For information on reporting security vulnerabilities and our security policy, please refer to [SECURITY.md](SECURITY.md).

## Key Features

- **Pythonic Syntax**: 

    Simplifies JSON data manipulation tasks with an intuitive Pythonic writing style.
  
- **Extended Data Types**: 

    Supports various data types such as tuples, characters, and extended precision floating-point numbers.
  
- **ArrayList Implementation**: 

    Provides Last-In-First-Out (LIFO) behavior for efficient array manipulation.
  
- **Unordered Map for Maps**:

    Uses unordered maps to ensure unique key-value associations.
  
- **Interconversion**: 

    Allows easy conversion between tuples and ArrayLists for flexible data manipulation.
  
- **Efficient Reading**: 

    Offers fast JSON file reading capabilities for complex data structures.
  
- **STL Integration**: 

    Seamlessly integrates with the C++ Standard Template Library (STL) for easy usage.
  
- **User-Friendly**: 

    Utilizes standard C++ types like std::string and std::ostream for enhanced user-friendliness.
  
- **Visualization Options**:

    Provides options for progress bar display and colorful JSON printing.
  
- **Efficient Performance**:

    Demonstrates impressive parsing speeds for handling large JSON files.

## About the Author

**JeongHan-Bae** is the mastermind behind the JoSon library. 
With a passion for creating efficient and user-friendly solutions, JeongHan-Bae has dedicated countless hours to crafting JoSon, aiming to simplify JSON processing tasks for developers worldwide.

## About the Name "JoSon"

The name "JoSon" is derived from the Sino-Korean word "조손," meaning "grandson," which is anglicized as "JoSon." This linguistic connection underscores the library's lightweight nature, akin to the significance of a grandchild within familial lineage. Furthermore, "JoSon" symbolizes more than just a name; it represents a blend of cultural heritage and technological innovation, reflecting both the author's Korean roots and the modern JSON data format. As the child of its creator and the grandchild of his father, "JoSon" pays homage to familial legacy and respect.

In essence, "JoSon" embodies the fusion of tradition and modernity, making it a fitting title for this user-friendly JSON processing library.

## License

This project is licensed under the [MIT License](LICENSE).

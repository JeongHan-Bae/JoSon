# JoSon API Reference: Simplified JSON Interaction in C++

-----

## Grand Overview

The JoSon library stands out as a lightweight and user-friendly tool for interacting with JSON data in C++ applications. Offering seamless integration with JSON, `std::string`, and `std::ostream`, JoSon provides developers with a powerful yet intuitive framework for managing hierarchical document structures and simplifying data manipulation tasks.

### Key Features

- **Versatile Document Management**: At the core of JoSon lies the `Doc` struct, a versatile container capable of storing a wide range of data types, including primitive types, tuples, arrays, and dictionary objects. This flexibility empowers developers to represent complex hierarchical structures with ease and clarity.

- **Efficient Hierarchical Structures**: JoSon supports hierarchical document structures such as arrays (`DocArr`) and tuples (`DocTuple`), offering efficient methods for organizing and managing data. These structures facilitate seamless traversal and manipulation of complex data sets, enhancing developer productivity and code readability.

- **Seamless JSON Interaction**: JoSon provides seamless integration with JSON data formats, allowing developers to parse JSON strings, read from JSON files, and store documents as JSON effortlessly. This simplifies common tasks such as data serialization and deserialization, enabling rapid development of JSON-based applications.

- **Streamlined String and Stream Operations**: Unlike traditional JSON libraries, JoSon offers native support for `std::string` and `std::ostream`, allowing developers to interact with JSON data using familiar C++ constructs. This enables a more natural and intuitive programming experience, reminiscent of Python's ease of use in handling JSON.

- **Pythonic Writing Style**: With its lightweight design and intuitive interface, JoSon promotes a Pythonic writing style in C++, making it easy for developers to express complex data manipulations in a concise and readable manner. This approach fosters code clarity and maintainability, leading to more efficient development workflows.

### Documentation Structure

The JoSon library documentation is meticulously structured to provide comprehensive coverage of its features and functionalities, catering to both novice and experienced developers. The documentation encompasses the following sections:

1. **Document Management in Struct Doc**: Detailed exploration of the `Doc` struct, covering document creation, size and type information, getters and setters, null checks, and various dictionary and array operations.

2. **Hierarchical Document Structures**: In-depth examination of array lists (`DocArr`) and tuples (`DocTuple`), including constructors, methods, operations, and memory management considerations for each structure.

3. **Document Deallocation Behaviors**: Explanation of document deallocation behaviors, outlining how documents are managed in memory based on their usage and declaration.

4. **Connecting to JSON**: Guidance on interacting with JSON data using JoSon, including functions for storing documents as JSON, parsing JSON strings into documents, and reading JSON files into documents.

5. **Seamless String and Stream Operations**: Demonstrations of JoSon's seamless integration with `std::string` and `std::ostream`, showcasing how developers can manipulate JSON data using familiar C++ constructs.

6. **Conclusion**: A concise summary highlighting the key features and benefits of the JoSon library, emphasizing its lightweight design, user-friendly interface, and Pythonic writing style for simplified JSON interaction in C++.

-----

## Document Management in Struct Doc

### Overview
The `Doc` struct offers a versatile container capable of holding various types of data, including primitive types, tuples, arrays, and dictionary objects. It provides methods for accessing, modifying, and manipulating data stored within it.

### Document Creation

#### Declaration
Different types of `Doc` instances can be declared as follows:

```cpp
using JoSon::Doc, JoSon::DocTuple, JoSon::DocArr, JoSon::DictObj, JoSon::Type;

// Declare a default document with a null type
Doc null_doc();

// Declare a dictionary document
Doc dict_doc(Type::Dict);

// Declare an array document
Doc arr_doc(Type::Array);

// Declare an integer document initialized with zero value
Doc int_doc(Type::Int);

// Declare a document and assign a value (e.g., 42)
Doc another_int_doc(42);
```

#### Pointers
For complex types like `DocArr`, `DocTuple`, and `DictObj`, pointers are used when passing values:

```cpp
DictObj new_dict;
Doc another_dict_doc(&new_dict);
```

### Document Size and Type Information

#### Size
The size of a document can be obtained using the `size()` method:

```cpp
int_doc.size(); // Returns 1
```

#### Type Information
Information about the type of the document can be retrieved:

```cpp
const char* type_str = int_doc.get_type_str(); // Get type as c-string
Type type_enum = int_doc.get_type(); // Get type as enum
```

### Getters, Setters, and Null Check

#### Getters and Setters
Values stored within the document can be accessed and modified using getters and setters:

```cpp
int value = int_doc.get_int(); // Retrieve the integer value
another_doc.set_double(3.3); // Set the value to a double

DictObj&  dict = dict_doc.get_dict_obj(); // Retrieve the dictionary reference
```

#### Null Check
A document's null value can be checked using `null_check()`:

```cpp
if (null_doc.null_check()) {
    // Document is null
}
```

### Dictionary and Array Operations

#### Dictionary Operations
Manipulate dictionary objects using `upsert()` and `erase()`:

```cpp
dict_doc.upsert("key1", 3000); // Insert or update a key-value pair by value
dict_doc.upsert("key2", Doc(3.14)); // Insert or update a key-value pair by document
dict_doc.upsert("key3", Type::Array); // Insert or update a key-value pair by declaring the type
dict_doc.erase("key2"); // Remove a key-value pair
```

#### Array Operations
For arrays, use `emplace_back()` and `pop_back()`:

```cpp
arr_doc.emplace_back(80000000LL); // Add a document to the array constructed by the value
arr_doc.emplace_back(Doc('J')); // Add a document directly to the array
arr_doc.emplace_back(Type::Nullptr); // Add a document to the array constructed by the type
arr_doc.pop_back(); // Remove the last element from the array
```

### Stream Insertion and Visualized Output

#### Stream Insertion
The string representation of a document can be output to a stream using the `<<` operator:

```cpp
std::cout << example_doc << std::endl;
```

#### Visualized Output
A string from a `Doc` object can be obtained using the `str()` method. Visualize the output by setting `visualize` to true:

```cpp
JoSon::Viso::colorful = true; // Enable colorful output
JoSon::Viso::str_color = "\033[1;33m"; // Set output string color to yellow
JoSon::Viso::json_print(example_doc.str(true)); // Print with customized indents
```

-----

## Hierarchical Document Structures

### Array List (DocArr)
An array list is a dynamic array that allows for flexible manipulation of its elements. It supports Last In, First Out (LIFO) features and is suitable for data processing operations. Arrays can be resized and modified as needed.

#### Conversion to Tuple
- `DocArr::to_tuple()`: Converts the array list to a tuple (`DocTuple`) containing the same values as the array list.

#### Overview
The `DocArr` struct represents an arraylist of documents in the program with Last In, First Out (LIFO) features. It serves as the default read-in type for JSON array format.

#### Constructors

**Default Constructor**
- `DocArr()`: Constructs an array with a reserved capacity of 8.

**Constructor with Provided Documents and Size**
- `DocArr(Doc* array, size_t size)`: Constructs an arraylist with the given array of documents and size. Make sure that the array (`Doc* array`) is created by new, and it will be deallocated with `delete[]`.

**Constructor with Specified Capacity**
- `explicit DocArr(size_t capacity)`: Constructs a default array with a specified capacity.

#### Methods

**Size and Capacity**
- `size()`: Gets the current size of the arraylist.
- `capacity()`: Gets the capacity of the array.

**Operations**
- `emplace_back(const Doc& doc)`: Emplaces a document at the end of the arraylist.
- `emplace_back(T value)`: Emplaces a document at the end of the arraylist. Converts the provided value to a document and emplaces it.
- `pop_back()`: Removes the last document from the arraylist.
- `set_value(size_t pos, const Doc& doc)`: Sets the value of a document at a specified position.
- `set_values(std::initializer_list<Doc> values)`: Sets the values of the arraylist with an initializer list of documents.
- `resize(size_t new_cap)`: Resizes the arraylist to a new capacity.
- `str(bool visualize)`: Converts the arraylist to a string representation.
- `DocArr(const DocArr& other) noexcept`: Copy constructor.
- `operator=(const DocArr& other) noexcept`: Assignment operator.
- `operator[](size_t index) const`: Access operator.

#### Memory Management
- Proper handling of deallocation ensures memory management is appropriate.

### Tuple (DocTuple)
A tuple represents a fixed-size collection of elements. It provides read-only access to its contents and cannot be modified directly. The

only way to modify a tuple is to replace it with a new tuple. Use tuples when you need a stable structure with immutable data.

#### Conversion to Array List
- `DocTuple::to_arraylist()`: Converts the tuple to an array list (`DocArr`) containing the same values as the tuple.

#### Overview
The `DocTuple` struct represents a tuple of documents in the program. It provides stability in data processing compared to arrays.

#### Constructors

**Default Constructor**
- `DocTuple()`: Constructs an uninitialized instance with `tpl` set to `nullptr`.

**Constructor with Provided Documents and Size**
- `DocTuple(const Doc* tuple, size_t size)`: Constructs a tuple with the given array of documents and size. Make sure that the array (`const Doc* tuple`) is created by new, and it will be deallocated with `delete[]`.

**Constructor with Initializer List of Documents**
- `DocTuple(std::initializer_list<Doc> docs)`: Constructs a tuple with the documents provided in the initializer list.

#### Methods

**Size**
- `size()`: Gets the size of the tuple.

**Tuple Reassignment**
- `set_values(std::initializer_list<Doc> values)`: Creates a new tuple with the provided documents and replaces the current tuple with it.

**Operations**
- `str(bool visualize) const`: Convert the tuple to a string representation, optionally providing visualization options.
- `DocTuple(const DocTuple& other) noexcept`: Copy constructor.
- `operator=(const DocTuple& other) noexcept`: Assignment operator.
- `operator[](size_t index) const`: Allow accessing documents in the tuple by index. Values are read-only by the operator[], ensuring safety.

#### Memory Management
- Proper handling of deallocation ensures memory management is appropriate.

-----

## Document Deallocation Behaviors

Documents in the `Doc` struct can have various deallocation behaviors based on their usage:

1. **Document Holding Pointer to Container**:
    - Deleting a document holding a pointer to a container deallocates the container but not the documents inside it.

2. **Document Declared with `new`**:
    - Documents declared with `new` must be manually deallocated by the programmer.

3. **Document Not Declared with `new`**:
    - Documents not declared with `new` remain alive until they go out of scope or are explicitly deleted.

## Connecting to JSON

### Storing Document as JSON
The `store_doc_to_json` function allows you to store a document as JSON in a file. It takes the file path and the document to be stored as parameters.

```cpp
void JoSon::Utils::store_doc_to_json(const std::string& path, const Doc& json_doc);
```

### Parsing JSON String to Document
The `string_to_doc` function converts a JSON-formatted string into a hierarchical document structure. It constructs the document based on the JSON syntax.

```cpp
Doc JoSon::Utils::string_to_doc(const std::string& input_str, bool show_bar = false);
```

### Reading JSON File into Document
The `read_json_file` function reads a JSON file and converts its contents into a hierarchical document structure. It opens the specified JSON file, reads its contents, and optionally displays a progress bar during the file reading process.

```cpp
Doc JoSon::Utils::read_json_file(const std::string& file_path, bool show_bar = false);
```

-----

### JoSon::Viso Operations

#### `json_print(const std::string& json_str, int indents)`
Prints a JSON-formatted string to the console with customizable indentation.

#### `ProgressBar::ProgressBar(std::atomic<size_t>* progress, std::atomic<size_t>* total)`
Constructs a progress bar instance to track the progress of a process.

#### `ProgressBar::update()`
Updates and displays a progress bar to the console based on the progress made.

-----

## Conclusion

In conclusion, the JoSon library emerges as a valuable asset for developers seeking a lightweight, intuitive, and Pythonic solution for handling JSON data in C++ applications.
With its seamless integration with JSON, `std::string`, and `std::ostream`, JoSon offers a compelling combination of versatility, efficiency, and ease of use, enabling developers to streamline their JSON-related tasks and accelerate the development of robust and maintainable software solutions.
Whether parsing JSON data, traversing hierarchical structures, or interacting with streams, JoSon provides the tools and features needed to simplify complex data manipulations and empower developers to focus on building innovative and scalable applications.
// Doc.h
#pragma once

#include <cstring>
#include <initializer_list>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <variant>

namespace JoSon {
    /**
     * @brief Enumeration representing various types.
     *
     * This enumeration represents different types that can be used in a program.
     * Each type corresponds to a specific data type in C++.
     */
    enum class Type {
        Char, ///< Character type. All possible integer types (Char, Int, LLong) are
        ///< implicitly convertible to integers.
        Int, ///< Integer type. Default type for JSON file input or projects working
        ///< with JSON data.
        LLong, ///< Long Long integer type. All possible integer types (Char, Int,
        ///< LLong)
        ///< are implicitly convertible to integers.
        Float,  ///< Single-precision floating point type.
        Double, ///< Double-precision floating point type. Default type for floating
        ///< point numbers with three decimal types.
        LDouble, ///< Extended precision floating point type.
        Bool,    ///< Boolean type.
        Str,     ///< String type. Represents a constant character pointer (const
        ///< char*), a C primitive type.
        Nullptr, ///< Null pointer type. Represents the JSON null type.
        Tuple,   ///< Tuple type. Represents a self-defined DocTuple type.
        Array,   ///< Array type. Represents a self-defined DocArr type.
        Dict   ///< Dict type. Represents a DictObj type, which is actually
        ///< std::unordered_map<const char*, Doc>.
    };

    struct Doc;
    struct DocTuple;
    struct DocArr;

    /**
     * @brief Represents a tuple of documents.
     *
     * This struct represents a tuple of documents in the program.
     * It will be output as [] in JSON but offers a more stable function in data
     * processing. Users can obtain the to_tuple of a DocArr to change the read-in
     * DocArr to a DocTuple.
     */
    struct DocTuple {

    private:
        const Doc* tpl; ///< Pointer to the array of documents.
        size_t s;    ///< Size of the tuple.

    public:
        /**
         * @brief Default constructor.
         *
         * Constructs an uninitialized instance with tpl set to nullptr.
         * Using this constructor leaves the instance uninitialized, and trying to
         * access its contents may lead to errors.
         */
        DocTuple();

        /**
         * @brief Constructor with provided documents and size.
         *
         * Constructs a tuple with the given array of documents and size.
         *
         * @warning The provided pointer should be created using `new`, as the
         * destructor calls `delete` to deallocate it. Once the raw pointer is
         * passed to a `DocTuple` instance, it will be held by the instance. Do not
         * use the same pointer to create other instances, as deleting one of the
         * instances will delete and reset `nullptr` the pointer. Modifying in one
         * place or resizing will surely affect the other instance. Once assigned,
         * refrain from modifying the pointer. Deletion of the instance will
         * automatically delete the pointer.
         *
         * @param tuple Pointer to the array of documents.
         * @param s Size of the tuple.
         */
        DocTuple(const Doc* tuple, size_t size);

        /**
         * @brief Constructor with initializer list of documents.
         *
         * Constructs a tuple with the documents provided in the initializer list.
         *
         * @param docs Initializer list of documents.
         */
        DocTuple(std::initializer_list<Doc> docs);

        /**
         * @brief Get the size of the tuple.
         *
         * @return Size of the tuple.
         */
        [[nodiscard]] size_t size() const;

        /**
         * @brief Convert the tuple to an arrayList.
         *
         * Creates and returns an arrayList (DocArr) containing the same values as
         * the tuple. This allows for more flexible modification of the tuple's
         * contents.
         *
         * @throw std::runtime_error if this tuple is not initialized.
         * @return An arrayList (DocArr) containing the same values as the tuple.
         */
        [[nodiscard]] [[maybe_unused]] DocArr to_arraylist() const;

        /**
         * @brief Set the values of the tuple with an initializer list of documents.
         *
         * Sets the values of the tuple using the documents provided in the
         * initializer list.
         *
         * @param values Initializer list of documents.
         */
        [[maybe_unused]] void set_values(std::initializer_list<Doc> values);

        /**
         * @brief Convert the tuple to a string representation.
         *
         * Converts the arraylist to a std::string representation, optionally providing
         * visualization options.
         *
         * @param visualize Whether to include visualization options.
         * @return String representation of the tuple.
         */
        [[nodiscard]] [[maybe_unused]] std::string str(bool visualize) const;

        /**
         * @brief Copy constructor.
         *
         * Constructs a new tuple by copying the contents of another tuple.
         * Note: Uses new pointers for value copying, ensuring safety.
         *
         * @param other Another tuple to copy from.
         */
        DocTuple(const DocTuple& other) noexcept;

        /**
         * @brief Assignment operator.
         *
         * Assigns the contents of another tuple to this tuple.
         * Note: Uses new pointers for value copying, ensuring safety.
         *
         * @param other Another tuple to assign from.
         * @return Reference to this tuple after assignment.
         */
        DocTuple& operator=(const DocTuple& other) noexcept;

        /**
         * @brief Access operator.
         *
         * Allows accessing documents in the tuple by index.
         * Note: Values are read-only by the operator[], ensuring safety.
         *
         * @param index Index of the document to access.
         * @throw std::out_of_range if the index is out of range.
         * @return Reference to the document at the specified index.
         */
        [[nodiscard]] const Doc& operator[](size_t index) const;

        /**
         * @brief Destructor.
         *
         * Destroys the tuple instance and deallocates memory.
         * Note: Uses delete to deallocate the tpl pointer.
         * If DocTuple(const Doc* tuple, size_t size) was used to pass in the raw pointer,
         * ensure it was created with new for safe deletion.
         */
        ~DocTuple();
    }; // struct DocTuple

    /**
     * @brief Represents an arraylist of documents.
     *
     * This struct represents an arraylist of documents in the program with LIFO features.
     * It is constructed as an ArrayList structure and serves as the default read-in
     * type for JSON [] format.
     */
    struct DocArr {
    private:
        Doc* arr;    ///< Pointer to the array of documents.
        size_t s; ///< Current s of the array.
        size_t cap;  ///< Capacity of the array.

    public:
        /**
         * @brief Default constructor.
         *
         * Constructs an array with a reserved capacity of 8.
         */
        DocArr();

        /**
         * @brief Constructor with provided documents and size.
         *
         * Constructs an arraylist with the given array of documents and size.
         *
         * @warning The provided pointer should be created using `new`, as the
         * destructor calls `delete` to deallocate it. Once the raw pointer is
         * passed to a DocArr instance, it will be held by the instance. Do not use
         * the same pointer to create other instances, as deleting one of the
         * instances will delete and reset `nullptr` the pointer. Modifying in one
         * place or resizing will surely affect the other instance. Once assigned,
         * refrain from modifying the pointer. Deletion of the instance will
         * automatically delete the pointer.
         *
         * @param array Pointer to the array of documents.
         * @param s Size of the arraylist.
         */
        DocArr(Doc* array, size_t size);

        /**
         * @brief Constructor with a specified capacity.
         *
         * Constructs a default array with a specified capacity.
         *
         * @param capacity Desired capacity of the default array.
         */
        explicit DocArr(size_t capacity);

        /**
         * @brief Check if the array is full.
         *
         * If full, the next emplace back needs reallocation.
         *
         * @return True if the array is full, false otherwise.
         */
        [[nodiscard]] bool full() const;

        /**
         * @brief Get the current size of the arraylist.
         *
         * @return Current size of the arraylist.
         */
        [[nodiscard]] size_t size() const;

        /**
         * @brief Get the capacity of the array.
         *
         * @return Capacity of the array.
         */
        [[nodiscard]] [[maybe_unused]] size_t capacity() const;

        /**
         * @brief Convert the arraylist to a tuple.
         *
         * Creates and returns a tuple (DocTuple) containing the same values as the
         * arraylist.
         *
         * @return A tuple (DocTuple) containing the same values as the arraylist.
         */
        [[nodiscard]] [[maybe_unused]] DocTuple to_tuple() const;

        /**
         * @brief Emplace a document at the end of the arraylist.
         *
         * @param doc The document to be emplaced.
         */
        void emplace_back(const Doc& doc);

        /**
         * @brief Emplace a document at the end of the arraylist.
         *
         * Adds a new document to the end of the arraylist by emplacing it.
         * The provided value will be converted to a document and emplaced.
         *
         * @tparam T Type of the value to be emplaced.
         * @param value Value to emplace.
         */
        template <typename T> void emplace_back(T value) {
            this->emplace_back(Doc(value));
        }

        /**
         * @brief Remove the last document from the arraylist.
         *
         * @return True if the removal was successful, false if the arraylist is empty.
         */
        bool pop_back();

        /**
         * @brief Set the value of a document at a specified position.
         *
         * @param pos Position to set the value.
         * @param doc Document to set.
         * @return True if the operation was successful, false otherwise.
         */
        [[maybe_unused]] bool set_value(size_t pos, const Doc& doc);

        /**
         * @brief Set the values of the arraylist with an initializer list of documents.
         *
         * Sets the values of the arraylist using the documents provided in the
         * initializer list. The values are set starting from index 0, and for
         * elements with indices equal to or greater than the size of the
         * initializer list, they will not be modified.
         *
         * @param values Initializer list of documents.
         */
        [[maybe_unused]] void set_values(std::initializer_list<Doc> values);

        /**
         * @brief Resize the arraylist.
         *
         * Resizes the arraylist to a new capacity. If the new capacity is less than the
         * current size, the array will be "shrunk" to size = new_cap, and elements
         * with indices equal to or greater than new capacity will not be copied to
         * the new array.
         *
         * @param new_cap New capacity for the arraylist.
         */
        [[maybe_unused]] void resize(size_t new_cap);

        /**
         * @brief Convert the arraylist to a string representation.
         *
         * Converts the arraylist to a std::string representation, optionally providing
         * visualization options.
         *
         * @param visualize Whether to include visualization options.
         * @return String representation of the arraylist.
         */
        [[nodiscard]] [[maybe_unused]] std::string str(bool visualize) const;

        /**
         * @brief Copy constructor.
         *
         * Constructs a new arraylist by copying the contents of another arraylist.
         * Note: Uses new pointers for value copying, ensuring safety.
         *
         * @param other Another arraylist to copy from.
         */
        DocArr(const DocArr& other) noexcept;

        /**
         * @brief Assignment operator.
         *
         * Assigns the contents of another arraylist to this arraylist.
         * Note: Uses new pointers for value copying, ensuring safety.
         *
         * @param other Another arraylist to assign from.
         * @return Reference to this arraylist after assignment.
         */
        DocArr& operator=(const DocArr& other) noexcept;

        /**
         * @brief Access operator.
         *
         * Allows accessing documents in the arraylist by index.
         * Note: Values are read-only by the operator[], ensuring safety.
         *
         * @param index Index of the document to access.
         * @throw std::out_of_range if the index is out of range.
         * @return Reference to the document at the specified index.
         */
        [[nodiscard]] const Doc& operator[](size_t index) const;

        /**
         * @brief Destructor.
         *
         * Destroys the arraylist instance and deallocates memory.
         * Note: Uses delete to deallocate the arr pointer.
         * If DocArr(Doc* array, size_t size) was used to pass in the raw pointer,
         * ensure it was created with new for safe deletion.
         */
        ~DocArr();
    }; // struct DocArr

    /**
     * @brief Alias for an unordered map of constant C-strings to Doc objects.
     *
     * This alias represents a type named DictObj, which is an
     * std::unordered_map<const char*, Doc>. It maps constant C-style strings
     * (constant char pointers) to dictionary objects.
     */
    using DictObj = std::unordered_map<const char*, Doc>;

    /**
     * @brief Alias for a variant type.
     *
     * This alias represents a type named Variant, which is an std::variant capable
     * of holding various types, including primitive types like char, int, long,
     * float, double, long double, bool, const char* (C-string), nullptr, as well as pointers
     * to DocTuple, DocArr, and DictObj.
     */
    using Variant =
            std::variant<char, int, long long, float, double, long double, bool,
                    const char*, std::nullptr_t, DocTuple*, DocArr*, DictObj*>;

    /**
     * @brief Represents a dynamic type capable of holding various values.
     *
     * This struct represents a dynamic type, named Doc, capable of holding various
     * values, including primitive types, DocTuple instances, DocArr instances, and
     * DictObj instances. The actual value is stored as a Variant, and the type is
     * stored as a Type enum.
     */
    struct Doc {
    private:
        Variant var{}; ///< Variant holding the value of the Doc instance.
        Type t;        ///< Type of the Doc instance.

        /**
         * @brief Deletes the held value and sets it to nullptr.
         *
         * This function is called when setting a new value to the Doc instance or
         * deleting the instance. If the type is DocTuple, DocArr, the held variable
         * as a DocTuple instance or DocArr instance will be deleted and set to
         * nullptr.
         */
        void delete_var();

        /**
         * @brief Convert the primitive Doc to a string representation.
         *
         * This function converts the primitive Doc (such as characters, integers,
         * floating-point numbers, booleans, and C-strings) to a string
         * representation. It supports optional visualization options.
         *
         * @param visualize Whether to include visualization options.
         * @return std::string representation of the primitive Doc.
         */
        [[nodiscard]] inline std::string prim_to_str(bool visualize) const;

    public:
        /**
         * @brief Template constructor to instantiate the Doc with a given value.
         *
         * This constructor allows instantiation of the Doc with a value of any
         * type. For types like DocTuple, DocArr, and DictObj, the pointer should be
         * passed and will be held by the Doc instance.
         *
         * @tparam T Type of the value to be assigned to the Doc.
         * @param value Value to assign to the Doc.
         */
        template <typename T> explicit Doc(T value);

        /**
         * @brief Constructor with a specified type.
         *
         * This constructor allows instantiation of the Doc with a specified Type
         * enum. By default, the type is set to Nullptr.
         *
         * @param type Type to assign to the Doc.
         */
        explicit Doc(Type type);

        /**
         * @brief Default constructor.
         *
         * Constructs a Doc instance with the default type Nullptr.
         */
        Doc();

        /**
         * @brief Destructor.
         *
         * Calls delete_var(); the held tuple, arraylist or C-string instance will also be
         * deleted.
         */
        ~Doc();

        friend std::ostream& operator<<(std::ostream& stream, const Doc& doc);


        /**
         * @brief Retrieve the stored single ASCII character value of the document.
         *
         * @return The single ASCII character value.
         */
        [[nodiscard]] [[maybe_unused]] char get_char() const;

        /**
         * @brief Retrieve the stored integer value of the document.
         *
         * @return The integer value.
         */
        [[nodiscard]] [[maybe_unused]] int get_int() const;

        /**
         * @brief Retrieve the stored long-long-integer value of the document.
         *
         * @return The long-long-integer value.
         */
        [[nodiscard]] [[maybe_unused]] long long get_l_long() const;

        /**
         * @brief Retrieve the stored single-precision floating-point value of the document.
         *
         * @return The single-precision floating-point value.
         */
        [[nodiscard]] [[maybe_unused]] float get_float() const;

        /**
         * @brief Retrieve the stored double-precision floating-point value of the document.
         *
         * @return The double-precision floating-point value.
         */
        [[nodiscard]] [[maybe_unused]] double get_double() const;

        /**
         * @brief Retrieve the stored long-double-precision floating-point value of the document.
         *
         * @return The long-double-precision floating-point value.
         */
        [[nodiscard]] [[maybe_unused]] long double get_long_double() const;

        /**
         * @brief Retrieve the stored boolean value of the document.
         *
         * @return The boolean value.
         */
        [[nodiscard]] [[maybe_unused]] bool get_bool() const;

        /**
         * @brief Retrieve the stored const char* (C-string) value of the document.
         *
         * @return The C-string value.
         */
        [[nodiscard]] [[maybe_unused]] const char* get_str() const;

        /**
         * @brief Retrieve a reference to the stored tuple value of the document.
         * Note: This function returns a reference, allowing modification of the tuple.
         *
         * @return Reference to the tuple.
         */
        [[nodiscard]] [[maybe_unused]] DocTuple& get_tuple() const;

        /**
         * @brief Retrieve a reference to the stored arraylist value of the document.
         * Note: This function returns a reference, allowing modification of the arraylist.
         *
         * @return Reference to the arraylist.
         */
        [[nodiscard]] [[maybe_unused]] DocArr& get_arr() const;

        /**
         * @brief Retrieve a reference to the stored dictionary object value of the document.
         * Note: This function returns a reference, allowing modification of the dictionary object.
         *
         * @return Reference to the dictionary object.
         */
        [[nodiscard]] [[maybe_unused]] DictObj& get_dict_obj() const;

        /**
         * @brief Set a single ASCII character value in the document.
         *
         * @param value The ASCII character value to be set.
         */
        [[maybe_unused]] void set_char(char value);

        /**
         * @brief Set an integer value in the document.
         *
         * @param value The integer value to be set.
         */
        [[maybe_unused]] void set_int(int value);

        /**
         * @brief Set a long-long-integer value in the document.
         *
         * @param value The long-long-integer value to be set.
         */
        [[maybe_unused]] void set_l_long(long long value);

        /**
         * @brief Set a single-precision floating-point value in the document.
         *
         * @param value The single precision floating point value to be set.
         */
        [[maybe_unused]] void set_float(float value);

        /**
         * @brief Set a double-precision floating-point value in the document.
         *
         * @param value The double precision floating point value to be set.
         */
        [[maybe_unused]] void set_double(double value);

        /**
         * @brief Set a long-double-precision floating-point value in the document.
         *
         * @param value The long-double-precision floating-point value to be set.
         */
        [[maybe_unused]] void set_long_double(long double value);

        /**
         * @brief Set a boolean value in the document.
         *
         * @param value The boolean value to be set.
         */
        [[maybe_unused]] void set_bool(bool value);

        /**
         * @brief Set a C-string value in the document.
         *
         * @param value The const char* (C-string) value to be set.
         */
        [[maybe_unused]] void set_str(const char* value);

        /**
         * @brief Set a tuple value in the document.
         *
         * @param value The tuple value to be set.
         */
        [[maybe_unused]] void set_tuple(DocTuple& value);

        /**
         * @brief Set an arraylist value in the document.
         *
         * @param value The array value to be set.
         */
        [[maybe_unused]] void set_arr(DocArr& value);

        /**
         * @brief Set a dictionary object value in the document.
         *
         * @param value The dictionary value to be set.
         */
        [[maybe_unused]] void set_dict(DictObj& value);

        /**
         * @brief Set the document to represent a null value.
         * This function assigns a null value to the document.
         */
        [[maybe_unused]] void set_null();

        /**
         * @brief Check if the document represents a null value.
         *
         * @return true if the document represents a null value, false otherwise.
         */
        [[nodiscard]] bool null_check() const;

        /**
         * @brief Get the size of the document.
         *
         * @return The size of the document.
         */
        [[nodiscard]] size_t size() const;

        /**
         * @brief Get a string representation of the document's type.
         *
         * @return A std::string representing the type of the document.
         */
        [[nodiscard]] [[maybe_unused]] const char* get_type_str() const;

        /**
         * @brief Get the type of the document.
         *
         * @return The type of the document.
         */
        [[nodiscard]] Type get_type() const;

        /**
         * @brief Inserts or updates a key-document pair in this document.
         *
         * If the key already exists, the corresponding value will be updated;
         * otherwise, a new key-value pair will be inserted.
         *
         * @param key The key for the key-document pair.
         * @param doc The document to be inserted or updated.
         *
         * @throw std::runtime_error if this document is not a dictionary object.
         */
        void upsert(const char* key, const Doc& doc);

        /**
         * @brief Inserts or updates a key-document pair in this document.
         *
         * If the key already exists, the corresponding value will be updated;
         * otherwise, a new key-value pair will be inserted.
         *
         * @tparam T Type of the value to be inserted or updated.
         * @param key The key for the key-document pair.
         * @param value Value to create the document instance.
         */
        template <typename T> void upsert(const char* key, T value) {
            this->upsert(key, Doc(value));
        }

        /**
         * @brief Inserts or updates a key-document pair in this document.
         *
         * If the key already exists, the corresponding value will be updated;
         * otherwise, a new key-value pair will be inserted.
         *
         * @param key The key for the key-document pair.
         * @param type Type to create the document instance.
         */
        [[maybe_unused]] void upsert(const char* key, Type type) { this->upsert(key, Doc(type)); }

        /**
         * @brief Remove a key-value pair from this document if it's a dictionary object.
         *
         * @param key The key of the key-value pair to be removed.
         *
         * @return true if the key-value pair is removed successfully, false otherwise.
         *
         * @throw std::runtime_error if this document is not a dictionary object.
         */
        [[maybe_unused]] bool erase(const char* key);

        /**
         * @brief Emplace a document at the end of the array in this document if it is an arraylist.
         *
         * @param doc The document to be emplaced.
         *
         * @throw std::runtime_error if this document is not an arraylist.
         */
        void emplace_back(const Doc& doc);

        /**
         * @brief Emplace a document at the end of the array in this document if it is an arraylist.
         *
         * Adds a new document to the end of the held by emplacing it.
         * The provided value will be converted to a document and emplaced.
         *
         * @tparam T Type of the value to be emplaced.
         * @param value Value to emplace.
         */
        template <typename T> void emplace_back(T value) {
            this->emplace_back(Doc(value));
        }

        /**
         * @brief Emplace a document at the end of the array in this document if it is an arraylist.
         *
         * Only available in Doc type.
         *
         * @param type Type to create the instance of the document to emplace.
         */
        [[maybe_unused]] void emplace_back(Type type) { this->emplace_back(Doc(type)); }

        /**
         * @brief Remove the last element from the array in this document if it is an arraylist.
         *
         * @return true if an element is removed successfully, false if the array is empty.
         *
         * @throw std::runtime_error if this document is not an arraylist.
         */
        [[maybe_unused]] bool pop_back();

        /**
         * @brief Implement operator[] for accessing elements in the dictionary object
         * (std::unordered_map<const char*, Doc>) by key.
         *
         * If the key is found in the dictionary, returns a reference to the corresponding document.
         * If the key is not found, inserts a default-constructed document with the given key and returns a reference to it.
         *
         * @param key The key of the element to access.
         * @return Reference to the document corresponding to the key.
         * @throw std::runtime_error if the document is not a dictionary object.
         */
        [[nodiscard]] [[maybe_unused]] Doc& operator[](const char* key);

        /**
         * @brief Implement operator() for accessing elements in arraylist (DocArr) and tuple (DocTuple) by index.
         *
         * If the index is valid, returns a const reference to the document at the specified index.
         * If the index is out of range, throws an out_of_range exception.
         *
         * @param index The index of the element to access.
         * @return Const reference to the document at the specified index.
         * @throw std::out_of_range if the index is out of range.
         * @throw std::runtime_error if the document is not a tuple or an arraylist
         * (Type::Tuple or Type::Array).
         */
        [[nodiscard]] [[maybe_unused]] const Doc& operator()(size_t index) const;

        /**
         * @brief Convert the Doc to a string representation.
         *
         * Converts the Doc to a string representation, optionally providing
         * visualization options.
         *
         * This function iteratively converts the Doc to a string representation,
         * which may improve performance and resource usage compared to the
         * recursive approach.
         *
         * @param visualize Whether to include visualization options.
         * @return String representation of the Doc.
         */
        [[nodiscard]] std::string str(bool visualize = false) const;

        /**
         * @brief Copy assignment operator.
         *
         * Assigns the contents of another Doc to this Doc.
         *
         * @param other Another Doc to assign from.
         * @return Reference to this Doc after assignment.
         */
        Doc& operator=(const Doc& other);
    }; // struct Doc

    /**
     * @brief Overload the stream insertion operator to output the string representation of a document to a stream.
     * The document is formatted as a JSON-like structure with 2 spaces as indentation for each level,
     * without visualization and without color.
     *
     * @param stream The output stream where the document string will be inserted.
     * @param doc The document to be output.
     * @return A reference to the output stream.
     */
    [[maybe_unused]] std::ostream& operator<<(std::ostream& stream, const Doc& doc);
} // namespace JoSon


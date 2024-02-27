// Doc.cpp
#include "../include/JoSon/Doc.h"
#include <stack>
#include <vector>

JoSon::DocTuple::DocTuple() : tpl(nullptr), s(0) {}
// Default constructor initializes tpl to nullptr and s to 0.

JoSon::DocTuple::DocTuple(const Doc* tuple, size_t size)
        : tpl(tuple), s(size) {}
/* Constructor initializes tpl with the provided tuple pointer and s with the
 * provided size.
 */

JoSon::DocTuple::DocTuple(std::initializer_list<Doc> docs) : s(docs.size()) {
    /* Constructor initializes s with the number of elements in the
     * initializer list.
     */
    if (s > 0) {
        auto new_tup = new Doc[s];
        size_t i = 0;
        for (const auto& doc : docs) {
            new_tup[i++] = doc;
        }
        tpl = static_cast<const Doc*>(new_tup);
    } else {
        tpl = nullptr;
    }
}

size_t JoSon::DocTuple::size() const { return s; }
// Returns the size of the tuple.

[[maybe_unused]] JoSon::DocArr JoSon::DocTuple::to_arraylist() const {
    if (tpl == nullptr) {
        throw std::runtime_error("Error: Calling nullptr.");
    }
    Doc* arr = new Doc[s];
    std::copy(tpl, tpl + s, arr);
    return {arr, s};
}
// Converts the tuple to a DocArr and returns it.

[[maybe_unused]] void
JoSon::DocTuple::set_values(std::initializer_list<Doc> values) {
    size_t length = values.size();
    s = length;
    Doc* new_tup = new Doc[s];
    std::copy(values.begin(), values.end(), new_tup);
    delete[] tpl;
    tpl = static_cast<const Doc*>(new_tup);
}
// Sets the values of the tuple using the provided initializer list.

[[maybe_unused]] std::string JoSon::DocTuple::str(bool visualize) const {
    std::string res;
    res.push_back(visualize ? '(' : '[');
    for (size_t i = 0; i < s; ++i) {
        res.append(tpl[i].str(visualize));
        if (i != s - 1)
            res.append(", ");
    }
    res.push_back(visualize ? ')' : ']');
    return res;
}
// Converts the tuple to a string representation.

JoSon::DocTuple::DocTuple(const DocTuple& other) noexcept : s(other.s) {
    Doc* new_tup = new Doc[s];
    std::copy(other.tpl, other.tpl + s, new_tup);
    tpl = static_cast<const Doc*>(new_tup);
}
// Copy constructor

JoSon::DocTuple& JoSon::DocTuple::operator=(const DocTuple& other) noexcept {
    if (this != &other) {
        // Deallocate existing memory
        delete[] tpl;
        // Copy size and capacity
        s = other.s;
        Doc* new_tup = new Doc[s];
        std::copy(other.tpl, other.tpl + s, new_tup);
        tpl = static_cast<const Doc*>(new_tup);
    }
    return *this;
}
// Copy assignment operator

const JoSon::Doc& JoSon::DocTuple::operator[](size_t index) const {
    if (index >= s) {
        throw std::out_of_range("Error: Index out of bounds.");
    }
    return tpl[index];
}
// Access operator to access documents in the tuple by index.

JoSon::DocTuple::~DocTuple() {
    delete[] tpl;
    tpl = nullptr;
}
// Destructor deallocates memory used by the tuple.

JoSon::DocArr::DocArr() : arr(new Doc[8]), s(0), cap(8) {}
/* Default constructor initializes arr with a new array of capacity 8, s to
 * 0, and cap to 8.
 */

JoSon::DocArr::DocArr(Doc* array, size_t size)
        : arr(array), s(size), cap(size) {}
/* Constructor initializes arr with the provided array pointer, s with the
 * provided size, and cap with the provided size.
 */

JoSon::DocArr::DocArr(size_t capacity) {
    arr = new Doc[capacity];
    s = 0;
    cap = capacity;
}
/* Constructor initializes arr with a new array of the specified capacity, s
 * to 0, and cap to the specified capacity.
 */

bool JoSon::DocArr::full() const { return s == cap; }
// Returns true if the array is full, otherwise false.

size_t JoSon::DocArr::size() const { return s; }
// Returns the current s of the array.

[[maybe_unused]] size_t JoSon::DocArr::capacity() const { return cap; }
// Returns the capacity of the array.

[[maybe_unused]] JoSon::DocTuple JoSon::DocArr::to_tuple() const {
    Doc* tup = new Doc[s];
    std::copy(arr, arr + s, tup);
    return {static_cast<const Doc*>(tup), s};
}
// Converts the array to a DocTuple and returns it.

void JoSon::DocArr::emplace_back(const Doc& doc) {
    if (!this->full()) {
        arr[s++] = doc;
    } else {
        cap *= 2;
        Doc* new_arr = new Doc[cap];
        std::copy(arr, arr + s, new_arr);
        new_arr[s++] = doc;
        delete[] arr;
        arr = new_arr;
    }
}
// Adds a document to the end of the array, resizing if necessary.

bool JoSon::DocArr::pop_back() {
    if (this->s == 0) {
        return false;
    } else {
        arr[s] = Doc(JoSon::Type::Nullptr);
        s--;
        return true;
    }
}
// Removes the last document from the array.

[[maybe_unused]] bool JoSon::DocArr::set_value(size_t pos, const Doc& doc) {
    if (pos >= s) {
        return false;
    } else if (pos < s) {
        arr[pos] = doc;
    } else { // pos == s
        emplace_back(doc);
    }
    return true;
}
// Sets the value of a document at the specified position.

[[maybe_unused]] void
JoSon::DocArr::set_values(std::initializer_list<Doc> values) {
    size_t length = values.size();

    if (length > cap) {
        cap = std::max(length, 2 * cap);
        Doc* new_arr = new Doc[cap];
        std::copy(values.begin(), values.end(), new_arr);
        delete[] arr;
        arr = new_arr;
    } else {
        std::copy(values.begin(), values.end(), arr);
    }

    s = std::max(s, length);
}
// Sets the values of the array using the provided initializer list.

[[maybe_unused]] void JoSon::DocArr::resize(size_t new_cap) {
    if (new_cap < s) {
        s = new_cap;
    }
    Doc* new_arr = new Doc[new_cap];
    std::copy(arr, arr + s, new_arr);
    delete[] arr;
    arr = new_arr;
    cap = new_cap;
}
// Resizes the array to a new capacity.

[[maybe_unused]] std::string JoSon::DocArr::str(bool visualize) const {
    std::string res;
    res.push_back('[');
    for (size_t i = 0; i < s; ++i) {
        res.append(arr[i].str(visualize));
        if (i != s - 1)
            res.append(", ");
    }
    res.push_back(']');
    return res;
}
// Converts the array to a string representation.

JoSon::DocArr::DocArr(const DocArr& other) noexcept
        : s(other.s), cap(other.cap) {
    arr = new Doc[cap];
    std::copy(other.arr, other.arr + s, arr);
}
// Copy constructor copies the contents of another array.

JoSon::DocArr& JoSon::DocArr::operator=(const DocArr& other) noexcept {
    if (this != &other) {
        // Deallocate existing memory
        delete[] arr;
        // Copy s and capacity
        s = other.s;
        cap = other.cap;
        // Allocate new memory and copy elements
        arr = new Doc[cap];
        std::copy(other.arr, other.arr + s, arr);
    }
    return *this;
}
// Copy assignment operator copies the contents of another array.

const JoSon::Doc& JoSon::DocArr::operator[](size_t index) const {
    if (index >= s) {
        throw std::out_of_range("Error: Index out of bounds.");
    }
    return arr[index];
}
// Access operator to access documents in the array by index.

JoSon::DocArr::~DocArr() {
    delete[] arr;
    arr = nullptr;
}
// Destructor deallocates memory used by the array.

void JoSon::Doc::delete_var() {
    switch (t) {
        case JoSon::Type::Array:
            if (auto* arrPtr = std::get_if<DocArr*>(&var)) {
                var = nullptr;
                delete[] * arrPtr;
                *arrPtr = nullptr;
            }
            break;
        case JoSon::Type::Tuple:
            if (auto* tupPtr = std::get_if<DocTuple*>(&var)) {
                var = nullptr;
                delete *tupPtr;
                *tupPtr = nullptr;
            }
            break;
        case JoSon::Type::Dict:
            if (auto* mapPtr = std::get_if<DictObj*>(&var)) {
                var = nullptr;
                delete *mapPtr;
                *mapPtr = nullptr;
            }
            break;
        case JoSon::Type::Str:
            if (auto* strPtr = std::get_if<const char*>(&var)) {
                var = nullptr;
                delete *strPtr;
                *strPtr = nullptr;
            }
            break;
        default:
            break;
    }
}
/* Function to delete dynamically allocated memory based on the type of the
 * variable.
 */

inline std::string JoSon::Doc::prim_to_str(bool visualize) const {
    std::string result;
    switch (t) {
        case JoSon::Type::Char:
            if (visualize) {
                result.append("'");
                result.push_back(std::get<char>(var));
                result.append("'");
            } else {
                result += std::to_string(static_cast<int>(std::get<char>(var)));
            }
            break;
        case JoSon::Type::Int: {
            int value = std::get<int>(var);
            if (visualize) {
                if (value < 0) {
                    result.push_back('-');
                    value = -value;
                }
                std::string valueStr = std::to_string(value);
                for (int i = static_cast<int>(valueStr.length()) - 3; i > 0;
                     i -= 3) {
                    valueStr.insert(i, "_");
                }
                result += valueStr;
            } else {
                result += std::to_string(value);
            }
        } break;
        case JoSon::Type::LLong: {
            long long value = std::get<long long>(var);
            if (visualize) {
                if (value < 0) {
                    result.push_back('-');
                    value = -value;
                }
                std::string valueStr = std::to_string(value);
                for (int i = static_cast<int>(valueStr.length()) - 3; i > 0;
                     i -= 3) {
                    valueStr.insert(i, "_");
                }
                result += valueStr;
            } else {
                result += std::to_string(value);
            }
        } break;
        case JoSon::Type::Float:
            if (visualize) {
                char buffer[32];
                std::sprintf(buffer, "%.4e", std::get<float>(var));
                result += buffer;
            } else {
                result += std::to_string(std::get<float>(var));
            }
            break;
        case JoSon::Type::Double:
            if (visualize) {
                char buffer[64];
                std::sprintf(buffer, "%.8e", std::get<double>(var));
                result.append(buffer);
            } else {
                result.append(std::to_string(std::get<double>(var)));
            }
            break;
        case JoSon::Type::LDouble:
            if (visualize) {
                char buffer[128];
                std::sprintf(buffer, "%.12Le", std::get<long double>(var));
                result.append(buffer);
            } else {
                result.append(std::to_string(std::get<long double>(var)));
            }
            break;
        case JoSon::Type::Bool:
            result.append(visualize ? (std::get<bool>(var) ? "True" : "False")
                                    : (std::get<bool>(var) ? "true" : "false"));
            break;
        case JoSon::Type::Nullptr:
            result.append(visualize ? "NullPtr" : "null");
            break;
        case JoSon::Type::Str: {
            const char* strValue = *(std::get_if<const char*>(&var));
            if (strValue) {
                result.push_back('\"');
                std::string str(strValue);
                size_t pos = 0;
                while ((pos = str.find('\"', pos)) != std::string::npos) {
                    str.replace(pos, 1, "\'");
                    ++pos;
                }
                result.append(str);
                result.push_back('\"');
            } else {
                result.append("null");
            }
        } break;
        case JoSon::Type::Tuple:
        case JoSon::Type::Array:
        case JoSon::Type::Dict:
            // Not primitive types, Should not be called by this func
            break;
    }
    return result;
}
// Primitive types to std::string

// Implementation of constructor JoSon::Doc::Doc(T value)
template <typename T> JoSon::Doc::Doc(T value) {
    if constexpr (std::is_same_v<T, char>) {
        t = JoSon::Type::Char;
    } else if constexpr (std::is_same_v<T, int>) {
        t = JoSon::Type::Int;
    } else if constexpr (std::is_same_v<T, long long>) {
        t = JoSon::Type::LLong;
    } else if constexpr (std::is_same_v<T, float>) {
        t = JoSon::Type::Float;
    } else if constexpr (std::is_same_v<T, double>) {
        t = JoSon::Type::Double;
    } else if constexpr (std::is_same_v<T, long double>) {
        t = JoSon::Type::LDouble;
    } else if constexpr (std::is_same_v<T, bool>) {
        t = JoSon::Type::Bool;
    } else if constexpr (std::is_same_v<T, const char*>) {
        t = JoSon::Type::Str;
    } else if constexpr (std::is_same_v<T, DocTuple*>) {
        t = JoSon::Type::Tuple;
    } else if constexpr (std::is_same_v<T, DocArr*>) {
        t = JoSon::Type::Array;
    } else if constexpr (std::is_same_v<T, DictObj*>) {
        t = JoSon::Type::Dict;
    } else {
        t = JoSon::Type::Nullptr;
        throw std::runtime_error("Error: Incorrect type");
    }
    var = value;
}
/* Constructor template that initializes the type and value of the document
 * based on the provided value.
 */

// Explicit instantiation of the template constructor for supported types
template JoSon::Doc::Doc(char);
template JoSon::Doc::Doc(int);
template JoSon::Doc::Doc(long long);
template JoSon::Doc::Doc(float);
template JoSon::Doc::Doc(double);
template JoSon::Doc::Doc(long double);
template JoSon::Doc::Doc(bool);
template JoSon::Doc::Doc(const char*);
// Template constructor for supported pointer types
template JoSon::Doc::Doc(DocArr*);
template JoSon::Doc::Doc(DocTuple*);
template JoSon::Doc::Doc(DictObj*);

JoSon::Doc::Doc(Type type) : t(type) {
    switch (type) {
        case JoSon::Type::Char:
            var = static_cast<char>(0);
            break;
        case JoSon::Type::Int:
            var = static_cast<int>(0);
            break;
        case JoSon::Type::LLong:
            var = static_cast<long long>(0);
            break;
        case JoSon::Type::Float:
            var = static_cast<float>(0);
            break;
        case JoSon::Type::Double:
            var = static_cast<double>(0);
            break;
        case JoSon::Type::LDouble:
            var = static_cast<long double>(0);
            break;
        case JoSon::Type::Bool:
            var = false;
            break;
        case JoSon::Type::Str:
            var = "";
            break;
        case JoSon::Type::Nullptr:
            var = nullptr;
            break;
        case JoSon::Type::Tuple:
            var = new DocTuple();
            break;
        case JoSon::Type::Array:
            var = new DocArr();
            break;
        case JoSon::Type::Dict:
            var = new DictObj;
            break;
    }
}
/* Constructor that initializes the type and value of the document based on the
 * provided type.
 */

JoSon::Doc::Doc() : t(JoSon::Type::Nullptr), var(nullptr) {}
/* Default constructor initializes the document with type Nullptr and nullptr as
 * the value.
 */

JoSon::Doc::~Doc() { delete_var(); }
/* Destructor calls the delete_var function to deallocate dynamically allocated
 * memory.
 */

[[maybe_unused]] char JoSon::Doc::get_char() const {
    return std::get<char>(var);
}

[[maybe_unused]] int JoSon::Doc::get_int() const {
    return std::get<int>(var);
}

[[maybe_unused]] long long JoSon::Doc::get_l_long() const {
    return std::get<long long>(var);
}

[[maybe_unused]] float JoSon::Doc::get_float() const {
    return std::get<float>(var);
}

[[maybe_unused]] double JoSon::Doc::get_double() const {
    return std::get<double>(var);
}

[[maybe_unused]] long double JoSon::Doc::get_long_double() const {
    return std::get<long double>(var);
}

[[maybe_unused]] bool JoSon::Doc::get_bool() const {
    return std::get<bool>(var);
}

[[maybe_unused]] const char* JoSon::Doc::get_str() const {
    return std::get<const char*>(var);
}

[[maybe_unused]] JoSon::DocTuple& JoSon::Doc::get_tuple() const {
    return *(std::get<DocTuple*>(var));
}

[[maybe_unused]] JoSon::DocArr& JoSon::Doc::get_arr() const {
    return *(std::get<DocArr*>(var));
}

[[maybe_unused]] JoSon::DictObj& JoSon::Doc::get_dict_obj() const {
    return *(std::get<DictObj*>(var));
}

[[maybe_unused]] void JoSon::Doc::set_char(char value) {
    delete_var();
    t = JoSon::Type::Char;
    var = value;
}

[[maybe_unused]] void JoSon::Doc::set_int(int value) {
    delete_var();
    t = JoSon::Type::Int;
    var = value;
}

[[maybe_unused]] void JoSon::Doc::set_l_long(long long value) {
    delete_var();
    t = JoSon::Type::LLong;
    var = value;
}

[[maybe_unused]] void JoSon::Doc::set_float(float value) {
    delete_var();
    t = JoSon::Type::Float;
    var = value;
}

[[maybe_unused]] void JoSon::Doc::set_double(double value) {
    delete_var();
    t = JoSon::Type::Double;
    var = value;
}

[[maybe_unused]] void JoSon::Doc::set_long_double(long double value) {
    delete_var();
    t = JoSon::Type::LDouble;
    var = value;
}

[[maybe_unused]] void JoSon::Doc::set_bool(bool value) {
    delete_var();
    t = JoSon::Type::Bool;
    var = value;
}

[[maybe_unused]] void JoSon::Doc::set_str(const char* value) {
    delete_var();
    t = JoSon::Type::Str;
    var = value;
}

[[maybe_unused]] void JoSon::Doc::set_tuple(DocTuple& value) {
    delete_var();
    t = JoSon::Type::Tuple;
    var = static_cast<DocTuple*>(&value);
}

[[maybe_unused]] void JoSon::Doc::set_arr(DocArr& value) {
    delete_var();
    t = JoSon::Type::Array;
    var = static_cast<DocArr*>(&value);
}

[[maybe_unused]] void JoSon::Doc::set_dict(DictObj& value) {
    delete_var();
    t = JoSon::Type::Dict;
    var = static_cast<DictObj*>(&value);
}

[[maybe_unused]] void JoSon::Doc::set_null() {
    delete_var();
    t = JoSon::Type::Nullptr;
    var = nullptr;
}

[[maybe_unused]] bool JoSon::Doc::null_check() const {
    return t == JoSon::Type::Nullptr;
}

[[maybe_unused]] size_t JoSon::Doc::size() const {
    switch (t) {
        case JoSon::Type::Char:
        case JoSon::Type::Int:
        case JoSon::Type::LLong:
        case JoSon::Type::Float:
        case JoSon::Type::Double:
        case JoSon::Type::LDouble:
        case JoSon::Type::Bool:
        case JoSon::Type::Str:
            return 1; // For primitive types, s is 1

        case JoSon::Type::Tuple:
            // If it's a Tuple, return the s stored in the DocTuple object
            return std::get<DocTuple*>(var)->size();
        case JoSon::Type::Array:
            // If it's an Array, return the s stored in the DocArr object
            return std::get<DocArr*>(var)->size();
        case JoSon::Type::Dict:
            // If it's a Dict (unordered_map), return the s of the map
            return std::get<DictObj*>(var)->size();
        case JoSon::Type::Nullptr:
            break;
    }
    // Default case nullptr
    return 0;
}

[[maybe_unused]] const char* JoSon::Doc::get_type_str() const {
    switch (t) {
        case JoSon::Type::Char:
            return "Char";
        case JoSon::Type::Int:
            return "Int";
        case JoSon::Type::LLong:
            return "LLong";
        case JoSon::Type::Float:
            return "Float";
        case JoSon::Type::Double:
            return "Double";
        case JoSon::Type::LDouble:
            return "LDouble";
        case JoSon::Type::Bool:
            return "Bool";
        case JoSon::Type::Str:
            return "Str";
        case JoSon::Type::Nullptr:
            return "Nullptr";
        case JoSon::Type::Tuple:
            return "Tuple";
        case JoSon::Type::Array:
            return "Array";
        case JoSon::Type::Dict:
            return "Dict";
    }
    // Should not be called
    return "Err";
}

JoSon::Type JoSon::Doc::get_type() const { return t; }

void JoSon::Doc::upsert(const char* key, const Doc& doc) {
    if (t == JoSon::Type::Dict) {
        if (auto* map_ptr = std::get_if<DictObj*>(&var)) {
            // Dereference the pointer to access the unordered_map
            auto& map = *map_ptr;

            // Use operator[] to insert or update the key-doc pair
            (*map)[key] = doc;
            return;
        }
    }
    throw std::runtime_error("Error: Key-Value pair only available for Dict "
                             "(std::unordered_map<const char*, Doc>)");
}

[[maybe_unused]] bool JoSon::Doc::erase(const char* key) {
    if (t == JoSon::Type::Dict) {
        if (auto* map_ptr = std::get_if<DictObj*>(&var)) {
            auto& map = *map_ptr;

            // Find the element with the given key
            auto it = map->find(key);
            if (it != map->end()) {
                // Erase the element if found
                map->erase(it);
                return true; // Return true indicating successful removal
            } else {
                return false; // Return false indicating key not found
            }
        }
    }
    throw std::runtime_error("Error: Key-Value pair only available for Dict "
                             "(std::unordered_map<const char*, Doc>).");
}

void JoSon::Doc::emplace_back(const Doc& doc) {
    if (t == JoSon::Type::Array) {
        if (auto* arr_ptr = std::get_if<DocArr*>(&var)) {
            auto& array = *arr_ptr;
            array->emplace_back(doc);
            return;
        }
    }
    throw std::runtime_error(
            "Error: Can only emplace back for ArrayList type.");
}

[[maybe_unused]] bool JoSon::Doc::pop_back() {
    if (t == JoSon::Type::Array) {
        if (auto* arr_ptr = std::get_if<DocArr*>(&var)) {
            auto& array = *arr_ptr;
            return array->pop_back();
        }
    }
    throw std::runtime_error("Error: Can only pop back for ArrayList type.");
}

// Implement operator[] for accessing elements in the unordered_map
JoSon::Doc& JoSon::Doc::operator[](const char* key) {
    if (t == JoSon::Type::Dict) {
        if (auto* map_ptr = std::get_if<DictObj*>(&var)) {
            auto& map = *map_ptr;
            // Return a reference to the value associated with the key
            return (*map)[key];
        }
    }
    throw std::runtime_error("Error: Key-Value pair only available for Dict "
                             "(std::unordered_map<const char*, Doc>).");
}

// Implement operator() for accessing elements in DocArr and DocTuple
const JoSon::Doc& JoSon::Doc::operator()(size_t index) const {
    if (auto* arr_ptr = std::get_if<DocArr*>(&var)) {
        auto& arr = *arr_ptr;
        if (index >= arr->size()) {
            throw std::out_of_range("Error: Index out of bounds.");
        }
        // Return reference to the element at the specified index
        return (*arr)[index];
    } else if (auto* tup_ptr = std::get_if<DocTuple*>(&var)) {
        auto& tup = *tup_ptr;
        if (index >= tup->size()) {
            throw std::out_of_range("Error: Index out of bounds.");
        }
        // Return reference to the element at the specified index
        return (*tup)[index];
    } else {
        throw std::runtime_error("Error: Operator () only available for "
                                 "Type::Tuple and Type::Array.");
    }
}

std::string JoSon::Doc::str(bool visualize) const {
    std::string result;
    std::stack<std::tuple<const Doc*, std::string, size_t>> doc_stk;
    std::stack<char> char_stk;
    doc_stk.emplace(this, "", 0);
    while (!doc_stk.empty()) {
        auto [doc_pos, str, lvl] = doc_stk.top();
        auto doc = *doc_pos;
        doc_stk.pop();
        result.append(str);
        if ((doc.get_type() != JoSon::Type::Tuple &&
             doc.get_type() != JoSon::Type::Array &&
             doc.get_type() != JoSon::Type::Dict) ||
            doc.size() == 0) {
            // doc is primitive or void container
            if (doc.size() == 1 || doc.get_type() == JoSon::Type::Nullptr) {
                // primitive
                result.append(doc.prim_to_str(visualize));
            } else {
                if (doc.get_type() == JoSon::Type::Tuple && visualize) {
                    result.append("(Null)");
                    // void tuple
                } else if (doc.get_type() == JoSon::Type::Array && visualize) {
                    result.append("[Null]");
                    // void arraylist
                } else if (doc.get_type() == JoSon::Type::Dict && visualize) {
                    result.append("{Null}");
                    // void map object
                } else if (doc.get_type() == JoSon::Type::Dict) {
                    result.append("{}");
                    // void map object to json
                } else {
                    result.append("[]");
                    // void list to json
                }
            }

            if (doc_stk.empty()) {
                // all docs are pushed to string
                while (!char_stk.empty()) {
                    result.append(char_stk.top() == '}' ? "\n" : "");
                    result.push_back(char_stk.top());
                    char_stk.pop();
                }
            } else {
                auto down_lvl = std::get<2>(doc_stk.top());
                if (lvl == down_lvl) {
                    result.append(", ");
                    // not the last
                    if (!str.empty()) {
                        // in a map
                        result.push_back('\n');
                    }
                }
                while (lvl > down_lvl && !char_stk.empty()) {
                    // this level is finished
                    result.append(char_stk.top() == '}' ? "\n" : "");
                    result.push_back(char_stk.top());
                    char_stk.pop();
                    --lvl;
                    if (lvl == down_lvl) {
                        result.append(",\n");
                    }
                }
            }
        } else if (doc.get_type() == JoSon::Type::Tuple ||
                   doc.get_type() == JoSon::Type::Array) {

            auto new_lvl = lvl + 1;
            if (visualize && doc.get_type() == JoSon::Type::Tuple) {
                result.push_back('(');
                char_stk.emplace(')');
            } else {
                result.push_back('[');
                char_stk.emplace(']');
            }
            for (size_t i = doc.size(); i > 0;) {
                doc_stk.emplace(&doc(--i), "", new_lvl);
                // push in backwards so the first one will be the first out
            }
        } else {
            // JoSon::Type::Dict
            result.append("{\n");
            char_stk.emplace('}');
            auto new_lvl = lvl + 1;
            for (const auto& it : doc.get_dict_obj()) {
                std::string new_str = "\"" + std::string(it.first) + "\": ";
                doc_stk.emplace(&it.second, std::move(new_str), new_lvl);
            }
        }
    }
    return result;
}

JoSon::Doc& JoSon::Doc::operator=(const Doc& other) {
    if (this != &other) {
        delete_var();
        t = other.t;
        var = other.var;
    }
    return *this;
}

[[maybe_unused]] std::ostream& JoSon::operator<<(std::ostream& stream,
                                                 const Doc& document) {
    std::stack<std::tuple<const JoSon::Doc*, const std::string, size_t>>
            doc_stk;
    std::stack<char> char_stk;

    std::vector<std::string> indent_strings(16);
    for (size_t i = 0; i < 16; ++i) {
        indent_strings[i] = std::string(2 * i, ' ');
    }

    auto get_indents = [&indent_strings](size_t i) {
        return (i < 16) ? indent_strings[i] : indent_strings[15];
    };

    doc_stk.emplace(&document, "", 0);
    while (!doc_stk.empty()) {
        auto [doc_pos, str, lvl] = doc_stk.top();
        auto doc = *doc_pos;
        doc_stk.pop();
        stream << str;
        if ((doc.get_type() != JoSon::Type::Tuple &&
             doc.get_type() != JoSon::Type::Array &&
             doc.get_type() != JoSon::Type::Dict) ||
            doc.size() == 0) {
            // doc is primitive or void container
            if (doc.size() == 1 || doc.null_check()) {
                // primitive
                switch (doc.t) {
                    case JoSon::Type::Char:
                        if (const char* value = std::get_if<char>(&doc.var)) {
                            stream << static_cast<int>(*value);
                        }
                        break;
                    case JoSon::Type::Int:
                        if (const int* value = std::get_if<int>(&doc.var)) {
                            stream << *value;
                        }
                        break;
                    case JoSon::Type::LLong:
                        if (const long long* value =
                                std::get_if<long long>(&doc.var)) {
                            stream << *value;
                        }
                        break;
                    case JoSon::Type::Float:
                        if (const float* value = std::get_if<float>(&doc.var)) {
                            stream << *value;
                        }
                        break;
                    case JoSon::Type::Double:
                        if (const double* value = std::get_if<double>(&doc.var)) {
                            stream << *value;
                        }
                        break;
                    case JoSon::Type::LDouble:
                        if (const long double* value =
                                std::get_if<long double>(&doc.var)) {
                            stream << *value;
                        }
                        break;
                    case JoSon::Type::Bool:
                        if (const bool* value = std::get_if<bool>(&doc.var)) {
                            stream << std::boolalpha
                                   << *value; // Output "true" or "false"
                        }
                        break;
                    case JoSon::Type::Nullptr:
                        stream << "null";
                        break;
                    case JoSon::Type::Str:
                        if (const char** value =
                                std::get_if<const char*>(&doc.var)) {
                            stream << '"' << *value << '"';
                        }
                        break;
                    case JoSon::Type::Tuple:
                    case JoSon::Type::Array:
                    case JoSon::Type::Dict:
                        // Not primitive types, Should not be called by this func
                        break;
                }
            } else {
                if (doc.get_type() == JoSon::Type::Dict) {
                    stream << "{}";
                    // void map object to json
                } else {
                    stream << "[]";
                    // void list to json
                }
            }

            if (doc_stk.empty()) {
                // all docs are pushed to string
                while (!char_stk.empty()) {
                    --lvl;
                    if (char_stk.top() == '}') {
                        stream << '\n' << get_indents(lvl);
                    }
                    stream << char_stk.top();
                    char_stk.pop();
                }
            } else {
                auto down_lvl = std::get<2>(doc_stk.top());
                if (lvl == down_lvl) {
                    stream << ", ";
                    // not the last
                    if (!str.empty()) {
                        // in a map
                        stream << '\n' << get_indents(lvl);
                    }
                }
                while (lvl > down_lvl && !char_stk.empty()) {
                    // this level is finished
                    --lvl;
                    if (char_stk.top() == '}') {
                        stream << '\n' << get_indents(lvl);
                    }
                    stream << char_stk.top();
                    char_stk.pop();

                    if (lvl == down_lvl) {
                        stream << ",\n" << get_indents(lvl);
                    }
                }
            }
        } else if (doc.get_type() == JoSon::Type::Tuple ||
                   doc.get_type() == JoSon::Type::Array) {
            auto new_lvl = lvl + 1;

            stream << '[';
            char_stk.emplace(']');

            for (size_t i = doc.size(); i > 0;) {
                doc_stk.emplace(&doc(--i), "", new_lvl);
                // push in backwards so the first one will be the first out
            }
        } else {
            // JoSon::Type::Dict
            auto new_lvl = ++lvl;

            stream << "{\n" << get_indents(lvl);
            char_stk.emplace('}');

            for (const auto& it : doc.get_dict_obj()) {
                std::string new_str = "\"" + std::string(it.first) + "\": ";
                doc_stk.emplace(&it.second, std::move(new_str), new_lvl);
            }
        }
    }
    return stream;
}
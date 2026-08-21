/*
Copyright (C) 2018-2024 Geoffrey Daniels. https://gpdaniels.com/

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, version 3 of the License only.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once
#ifndef GTL_FILE_TEXT_JSON_HPP
#define GTL_FILE_TEXT_JSON_HPP

// Summary: A small json parser and composer. [wip]

#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif

#include <cstdio>
#include <cstdlib>
#include <map>
#include <new>
#include <string>
#include <vector>

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

namespace gtl {
    class json final {
    public:
        class value final {
        public:
            using null_type = decltype(nullptr);
            using bool_type = bool;
            using number_type = double;
            using string_type = std::string;
            using object_type = std::map<value::string_type, value>;
            using array_type = std::vector<value>;

        private:
            enum class contains {
                null_type,
                bool_type,
                number_type,
                string_type,
                object_type,
                array_type
            };

        private:
            contains type;

            union {
                null_type null_value;
                bool_type bool_value;
                number_type number_value;
                string_type string_value;
                object_type object_value;
                array_type array_value;
            };

        public:
            ~value() {
                switch (this->type) {
                    case contains::null_type: {
                        null_value.~null_type();
                    } break;
                    case contains::bool_type: {
                        bool_value.~bool_type();
                    } break;
                    case contains::number_type: {
                        number_value.~number_type();
                    } break;
                    case contains::string_type: {
                        string_value.~string_type();
                    } break;
                    case contains::object_type: {
                        object_value.~object_type();
                    } break;
                    case contains::array_type: {
                        array_value.~array_type();
                    } break;
                }
            }

            value()
                : type(contains::null_type) {
                new (&null_value) null_type(nullptr);
            }

            value(const value& other)
                : type(other.type) {
                switch (this->type) {
                    case contains::null_type: {
                        new (&null_value) null_type(other.null_value);
                    } break;
                    case contains::bool_type: {
                        new (&bool_value) bool_type(other.bool_value);
                    } break;
                    case contains::number_type: {
                        new (&number_value) number_type(other.number_value);
                    } break;
                    case contains::string_type: {
                        new (&string_value) string_type(other.string_value);
                    } break;
                    case contains::object_type: {
                        new (&object_value) object_type(other.object_value);
                    } break;
                    case contains::array_type: {
                        new (&array_value) array_type(other.array_value);
                    } break;
                }
            }

            value(value&& other)
                : type(other.type) {
                switch (this->type) {
                    case contains::null_type: {
                        new (&null_value) null_type(static_cast<null_type&&>(other.null_value));
                    } break;
                    case contains::bool_type: {
                        new (&bool_value) bool_type(static_cast<bool_type&&>(other.bool_value));
                    } break;
                    case contains::number_type: {
                        new (&number_value) number_type(static_cast<number_type&&>(other.number_value));
                    } break;
                    case contains::string_type: {
                        new (&string_value) string_type(static_cast<string_type&&>(other.string_value));
                    } break;
                    case contains::object_type: {
                        new (&object_value) object_type(static_cast<object_type&&>(other.object_value));
                    } break;
                    case contains::array_type: {
                        new (&array_value) array_type(static_cast<array_type&&>(other.array_value));
                    } break;
                }
            }

            value& operator=(const value& other) {
                switch (this->type) {
                    case contains::null_type: {
                        null_value.~null_type();
                    } break;
                    case contains::bool_type: {
                        bool_value.~bool_type();
                    } break;
                    case contains::number_type: {
                        number_value.~number_type();
                    } break;
                    case contains::string_type: {
                        string_value.~string_type();
                    } break;
                    case contains::object_type: {
                        object_value.~object_type();
                    } break;
                    case contains::array_type: {
                        array_value.~array_type();
                    } break;
                }
                this->type = other.type;
                switch (this->type) {
                    case contains::null_type: {
                        new (&null_value) null_type(other.null_value);
                    } break;
                    case contains::bool_type: {
                        new (&bool_value) bool_type(other.bool_value);
                    } break;
                    case contains::number_type: {
                        new (&number_value) number_type(other.number_value);
                    } break;
                    case contains::string_type: {
                        new (&string_value) string_type(other.string_value);
                    } break;
                    case contains::object_type: {
                        new (&object_value) object_type(other.object_value);
                    } break;
                    case contains::array_type: {
                        new (&array_value) array_type(other.array_value);
                    } break;
                }
                return *this;
            }

            value& operator=(value&& other) {
                switch (this->type) {
                    case contains::null_type: {
                        null_value.~null_type();
                    } break;
                    case contains::bool_type: {
                        bool_value.~bool_type();
                    } break;
                    case contains::number_type: {
                        number_value.~number_type();
                    } break;
                    case contains::string_type: {
                        string_value.~string_type();
                    } break;
                    case contains::object_type: {
                        object_value.~object_type();
                    } break;
                    case contains::array_type: {
                        array_value.~array_type();
                    } break;
                }
                this->type = other.type;
                switch (this->type) {
                    case contains::null_type: {
                        new (&null_value) null_type(static_cast<null_type&&>(other.null_value));
                    } break;
                    case contains::bool_type: {
                        new (&bool_value) bool_type(static_cast<bool_type&&>(other.bool_value));
                    } break;
                    case contains::number_type: {
                        new (&number_value) number_type(static_cast<number_type&&>(other.number_value));
                    } break;
                    case contains::string_type: {
                        new (&string_value) string_type(static_cast<string_type&&>(other.string_value));
                    } break;
                    case contains::object_type: {
                        new (&object_value) object_type(static_cast<object_type&&>(other.object_value));
                    } break;
                    case contains::array_type: {
                        new (&array_value) array_type(static_cast<array_type&&>(other.array_value));
                    } break;
                }
                return *this;
            }

            value(null_type value)
                : type(contains::null_type) {
                new (&null_value) null_type(value);
            }

            value(bool_type value)
                : type(contains::bool_type) {
                new (&bool_value) bool_type(value);
            }

            value(number_type value)
                : type(contains::number_type) {
                new (&number_value) number_type(value);
            }

            value(const string_type& value)
                : type(contains::string_type) {
                new (&string_value) string_type(value);
            }

            value(const object_type& value)
                : type(contains::object_type) {
                new (&object_value) object_type(value);
            }

            value(const array_type& value)
                : type(contains::array_type) {
                new (&array_value) array_type(value);
            }

        private:
            template <typename contains_type>
            class convert_to_contains_enum;

        public:
            template <typename is_type>
            bool is() const {
                return (this->type == convert_to_contains_enum<is_type>::type);
            }

        public:
            template <typename as_type>
            const as_type& as() const {
                if constexpr (convert_to_contains_enum<as_type>::type == contains::null_type) {
                    return null_value;
                }
                else if constexpr (convert_to_contains_enum<as_type>::type == contains::bool_type) {
                    return bool_value;
                }
                else if constexpr (convert_to_contains_enum<as_type>::type == contains::number_type) {
                    return number_value;
                }
                else if constexpr (convert_to_contains_enum<as_type>::type == contains::string_type) {
                    return string_value;
                }
                else if constexpr (convert_to_contains_enum<as_type>::type == contains::object_type) {
                    return object_value;
                }
                else if constexpr (convert_to_contains_enum<as_type>::type == contains::array_type) {
                    return array_value;
                }
            }

            template <typename as_type>
            as_type& as() {
                if constexpr (convert_to_contains_enum<as_type>::type == contains::null_type) {
                    return null_value;
                }
                else if constexpr (convert_to_contains_enum<as_type>::type == contains::bool_type) {
                    return bool_value;
                }
                else if constexpr (convert_to_contains_enum<as_type>::type == contains::number_type) {
                    return number_value;
                }
                else if constexpr (convert_to_contains_enum<as_type>::type == contains::string_type) {
                    return string_value;
                }
                else if constexpr (convert_to_contains_enum<as_type>::type == contains::object_type) {
                    return object_value;
                }
                else if constexpr (convert_to_contains_enum<as_type>::type == contains::array_type) {
                    return array_value;
                }
            }

        private:
            static string_type escape(const string_type& input) {
                string_type string;
                string.reserve(input.size() + 2);
                string += '"';
                for (const char character : input) {
                    switch (character) {
                        case '\"': {
                            string += "\\\"";
                        } break;
                        case '\\': {
                            string += "\\\\";
                        } break;
                        case '\b': {
                            string += "\\b";
                        } break;
                        case '\f': {
                            string += "\\f";
                        } break;
                        case '\n': {
                            string += "\\n";
                        } break;
                        case '\r': {
                            string += "\\r";
                        } break;
                        case '\t': {
                            string += "\\t";
                        } break;
                        default: {
                            const unsigned char byte = static_cast<unsigned char>(character);
                            if (byte < 0x20) {
                                char buffer[7] = {};
                                const int length = std::snprintf(&buffer[0], sizeof(buffer), "\\u%04x", static_cast<unsigned int>(byte));
                                if ((length > 0) && (static_cast<string_type::size_type>(length) < sizeof(buffer))) {
                                    string += &buffer[0];
                                }
                            }
                            else {
                                string += character;
                            }
                        } break;
                    }
                }
                string += '"';
                return string;
            }

        public:
            string_type to_string() const {
                switch (this->type) {
                    case contains::null_type: {
                        return "null";
                    } break;
                    case contains::bool_type: {
                        if (bool_value) {
                            return "true";
                        }
                        return "false";
                    } break;
                    case contains::number_type: {
                        char buffer[25] = {};
                        const int length = std::snprintf(&buffer[0], 25, "%g", number_value);
                        if ((length < 0) || (length >= 25)) {
                            return "";
                        }
                        return std::string(&buffer[0]);
                    } break;
                    case contains::string_type: {
                        return escape(this->string_value);
                    } break;
                    case contains::object_type: {
                        string_type string = "{";
                        for (const auto& [key, item] : this->object_value) {
                            if (string.size() == 1) {
                                string += escape(key) + ":" + item.to_string();
                            }
                            else {
                                string += "," + escape(key) + ":" + item.to_string();
                            }
                        }
                        return string + "}";
                    } break;
                    case contains::array_type: {
                        string_type string = "[";
                        for (const auto& value : this->array_value) {
                            if (string.size() == 1) {
                                string += value.to_string();
                            }
                            else {
                                string += "," + value.to_string();
                            }
                        }
                        return string + "]";
                    } break;
                }
                return "";
            }
        };

    private:
        value root;

    private:
        value parse_recursive(const std::string& data, std::string::size_type& index) {
            skip_whitespace(data, index);
            if (parse_null(data, index)) {
                return value(nullptr);
            }
            value::bool_type boolean;
            if (parse_bool(data, index, boolean)) {
                return value(boolean);
            }
            value::number_type number;
            if (parse_number(data, index, number)) {
                return value(number);
            }
            value::string_type string;
            if (parse_string(data, index, string)) {
                return value(string);
            }
            value::object_type object;
            if (parse_object(data, index, object)) {
                return value(object);
            }
            value::array_type array;
            if (parse_array(data, index, array)) {
                return value(array);
            }
            return value(nullptr);
        }

        void skip_whitespace(const std::string& data, std::string::size_type& index) {
            for (; index < data.size(); ++index) {
                switch (data[index]) {
                    case '\f':
                        break;
                    case '\n':
                        break;
                    case '\r':
                        break;
                    case '\t':
                        break;
                    case ' ':
                        break;
                    default:
                        return;
                }
            }
        }

        bool parse_null(const std::string& data, std::string::size_type& index) {
            if (data.substr(index, 4) == "null") {
                index += 4;
                return true;
            }
            return false;
        }

        bool parse_bool(const std::string& data, std::string::size_type& index, value::bool_type& boolean) {
            if (data.substr(index, 4) == "true") {
                boolean = true;
                index += 4;
                return true;
            }
            if (data.substr(index, 5) == "false") {
                boolean = false;
                index += 5;
                return true;
            }
            return false;
        }

        bool parse_number(const std::string& data, std::string::size_type& index, value::number_type& number) {
            std::string valid_number_characters = "0123456789-+eE.";
            std::string::size_type length = 0;
            while ((index + length < data.size()) && (valid_number_characters.find(data[index + length]) < valid_number_characters.size())) {
                ++length;
            }
            if (length == 0) {
                return false;
            }
            char* end_pointer = nullptr;
            std::string number_string = data.substr(index, length);
            number = std::strtod(number_string.c_str(), &end_pointer);
            if ((end_pointer != nullptr) && (*end_pointer == 0)) {
                index += length;
                return true;
            }
            return false;
        }

        static bool append_hex_digit(const char digit, unsigned int& value) {
            value <<= 4;
            if ((digit >= '0') && (digit <= '9')) {
                value |= static_cast<unsigned int>(digit - '0');
                return true;
            }
            if ((digit >= 'a') && (digit <= 'f')) {
                value |= (static_cast<unsigned int>(digit - 'a') + 10);
                return true;
            }
            if ((digit >= 'A') && (digit <= 'F')) {
                value |= (static_cast<unsigned int>(digit - 'A') + 10);
                return true;
            }
            return false;
        }

        bool append_unicode_escape(const std::string& data, std::string::size_type& position, value::string_type& string) {
            if ((position + 4) >= data.size()) {
                return false;
            }
            unsigned int code_point = 0;
            for (std::string::size_type offset = 1; offset <= 4; ++offset) {
                if (!append_hex_digit(data[position + offset], code_point)) {
                    return false;
                }
            }
            position += 4;
            if ((code_point >= 0xD800) && (code_point <= 0xDBFF)) {
                if (((position + 6) >= data.size()) || (data[position + 1] != '\\') || (data[position + 2] != 'u')) {
                    return false;
                }
                unsigned int low_surrogate = 0;
                for (std::string::size_type offset = 3; offset <= 6; ++offset) {
                    if (!append_hex_digit(data[position + offset], low_surrogate)) {
                        return false;
                    }
                }
                if ((low_surrogate < 0xDC00) || (low_surrogate > 0xDFFF)) {
                    return false;
                }
                code_point = 0x10000 + ((code_point - 0xD800) << 10) + (low_surrogate - 0xDC00);
                position += 6;
            }
            else if ((code_point >= 0xDC00) && (code_point <= 0xDFFF)) {
                return false;
            }
            if (code_point < 0x80) {
                string += static_cast<char>(code_point);
            }
            else if (code_point < 0x800) {
                string += static_cast<char>(0xC0 | (code_point >> 6));
                string += static_cast<char>(0x80 | (code_point & 0x3F));
            }
            else if (code_point < 0x10000) {
                string += static_cast<char>(0xE0 | (code_point >> 12));
                string += static_cast<char>(0x80 | ((code_point >> 6) & 0x3F));
                string += static_cast<char>(0x80 | (code_point & 0x3F));
            }
            else {
                string += static_cast<char>(0xF0 | (code_point >> 18));
                string += static_cast<char>(0x80 | ((code_point >> 12) & 0x3F));
                string += static_cast<char>(0x80 | ((code_point >> 6) & 0x3F));
                string += static_cast<char>(0x80 | (code_point & 0x3F));
            }
            return true;
        }

        bool parse_string(const std::string& data, std::string::size_type& index, value::string_type& string) {
            if ((index >= data.size()) || (data[index] != '"')) {
                return false;
            }
            std::string::size_type position = index + 1;
            string.clear();
            while (position < data.size()) {
                const char character = data[position];
                if (character == '"') {
                    index = position + 1;
                    return true;
                }
                if (character == '\\') {
                    ++position;
                    if (position >= data.size()) {
                        return false;
                    }
                    switch (data[position]) {
                        case '\"': {
                            string += '\"';
                        } break;
                        case '\\': {
                            string += '\\';
                        } break;
                        case '/': {
                            string += '/';
                        } break;
                        case 'b': {
                            string += '\b';
                        } break;
                        case 'f': {
                            string += '\f';
                        } break;
                        case 'n': {
                            string += '\n';
                        } break;
                        case 'r': {
                            string += '\r';
                        } break;
                        case 't': {
                            string += '\t';
                        } break;
                        case 'u': {
                            if (!append_unicode_escape(data, position, string)) {
                                return false;
                            }
                        } break;
                        default: {
                            return false;
                        } break;
                    }
                    ++position;
                    continue;
                }
                if (static_cast<unsigned char>(character) < 0x20) {
                    return false;
                }
                string += character;
                ++position;
            }
            return false;
        }

        bool parse_object(const std::string& data, std::string::size_type& index, value::object_type& object) {
            if (data[index] != '{') {
                return false;
            }
            std::string::size_type index_object = index + 1;
            object = value::object_type();
            skip_whitespace(data, index_object);
            if (index_object >= data.size()) {
                return false;
            }
            if (data[index_object] == '}') {
                index = index_object + 1;
                return true;
            }
            while (index_object < data.size()) {
                skip_whitespace(data, index_object);
                if ((index_object >= data.size()) || (data[index_object] == '}')) {
                    return false;
                }
                value::string_type key;
                if (!parse_string(data, index_object, key)) {
                    return false;
                }
                skip_whitespace(data, index_object);
                if ((index_object >= data.size()) || (data[index_object] != ':')) {
                    return false;
                }
                ++index_object;
                skip_whitespace(data, index_object);
                value item = parse_recursive(data, index_object);
                object.insert({ key, item });
                skip_whitespace(data, index_object);
                if (index_object >= data.size()) {
                    return false;
                }
                if (data[index_object] == '}') {
                    index = index_object + 1;
                    return true;
                }
                if (data[index_object] != ',') {
                    return false;
                }
                ++index_object;
            }
            return false;
        }

        bool parse_array(const std::string& data, std::string::size_type& index, value::array_type& array) {
            if (data[index] != '[') {
                return false;
            }
            std::string::size_type index_array = index + 1;
            array = value::array_type();
            skip_whitespace(data, index_array);
            if (index_array >= data.size()) {
                return false;
            }
            if (data[index_array] == ']') {
                index = index_array + 1;
                return true;
            }
            while (index_array < data.size()) {
                skip_whitespace(data, index_array);
                if ((index_array >= data.size()) || (data[index_array] == ']')) {
                    return false;
                }
                value item = parse_recursive(data, index_array);
                array.push_back(item);
                skip_whitespace(data, index_array);
                if (index_array >= data.size()) {
                    return false;
                }
                if (data[index_array] == ']') {
                    index = index_array + 1;
                    return true;
                }
                if (data[index_array] != ',') {
                    return false;
                }
                ++index_array;
            }
            return false;
        }

    public:
        bool parse(const std::string& data) {
            std::string::size_type index = 0;
            this->root = parse_recursive(data, index);
            skip_whitespace(data, index);
            return (index == data.size());
        }

        std::string compose() const {
            return this->root.to_string();
        }

        value& document() {
            return this->root;
        }
    };

    template <typename contains_type>
    class json::value::convert_to_contains_enum<const contains_type> final {
    public:
        constexpr static const json::value::contains type = json::value::convert_to_contains_enum<contains_type>::type;
    };

    template <>
    class json::value::convert_to_contains_enum<json::value::null_type> final {
    public:
        constexpr static const json::value::contains type = json::value::contains::null_type;
    };

    template <>
    class json::value::convert_to_contains_enum<json::value::bool_type> final {
    public:
        constexpr static const json::value::contains type = json::value::contains::bool_type;
    };

    template <>
    class json::value::convert_to_contains_enum<json::value::number_type> final {
    public:
        constexpr static const json::value::contains type = json::value::contains::number_type;
    };

    template <>
    class json::value::convert_to_contains_enum<json::value::string_type> final {
    public:
        constexpr static const json::value::contains type = json::value::contains::string_type;
    };

    template <>
    class json::value::convert_to_contains_enum<json::value::object_type> final {
    public:
        constexpr static const json::value::contains type = json::value::contains::object_type;
    };

    template <>
    class json::value::convert_to_contains_enum<json::value::array_type> final {
    public:
        constexpr static const json::value::contains type = json::value::contains::array_type;
    };
}

#endif // GTL_FILE_TEXT_JSON_HPP

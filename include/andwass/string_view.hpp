#pragma once
#include <cstring>
#include <cctype>
#include <cmath>
#include <algorithm>

namespace andwass {
class string_view {
    const char *data_ = nullptr;
    std::size_t size_ = 0;
public:
    using pointer = const char*;
    using const_reference = const char&;
    using iterator = pointer;

    using const_reverse_iterator = std::reverse_iterator<iterator>;
    using size_type = std::size_t;

    static constexpr size_type npos = size_type(-1);

    /**
     * @brief Default construct a `string_view`
     *
     * `data()` will return `nullptr`. `size()` will return 0.
     */
    constexpr string_view() = default;

    /**
     * @brief Construct a `string_view` from a pointer and length pair.
     * @param data Pointer to the start of the `string_view`
     * @param size The size of the `string_view`
     */
    constexpr string_view(pointer data, size_type size): data_(data), size_(size) {}

    /**
     * @brief Construct from a NULL-terminated string
     * @param data NULL-terminated string
     *
     * If `data == nullptr` is true then the `string_view` will have size 0, otherwise
     * the size will be determined by `std::strlen`.
     */
    constexpr string_view(pointer data): data_(data) {
        if (data_) {
            size_ = std::strlen(data_);
        }
    }

    template<class Iter>
    constexpr string_view(Iter first, Iter last): data_(first), size_(last-first) {}

    /**
     * @brief Get a pointer to the start of the data
     * @return Pointer to the start of the data.
     */
    [[nodiscard]] constexpr pointer data() const {
        return data_;
    }

    /**
     * @brief Get the size of the `string_view`
     * @return The current size of the `string_view`.
     */
    [[nodiscard]] constexpr size_type size() const {
        return size_;
    }

    /**
     * @brief Helper to check if `string_view` points to an empty range of chars.
     * @return True if `size() == 0` otherwise false.
     */
    [[nodiscard]] constexpr bool is_empty() const {
        return size_ == 0;
    }

    /**
     * @brief Get an interator to the start of the string view.
     * @return A random access iterator pointing to the start of the string view.
     */
    [[nodiscard]] iterator begin() const {
        return data_;
    }

    [[nodiscard]] const_reverse_iterator rbegin() const {
        return const_reverse_iterator(end());
    }

    /**
     * @brief Get an iterator to the character following the last character of the view.
     * @return One-past-the-end of the string view.
     *
     * @note Attempting to dereference the iterator results in undefined behaviour.
     */
    [[nodiscard]] iterator end() const {
        return data_ + size_;
    }

    [[nodiscard]] const_reverse_iterator rend() const {
        return const_reverse_iterator(begin());
    }

    /**
     * @brief Get a const reference to the first char in the string view.
     * @return Reference to the first char in the string view.
     *
     * @note Calling `front()` on an empty string view results in undefined behaviour.
     */
    [[nodiscard]] constexpr const_reference front() const {
        return data_[0];
    }

    /**
     * @brief Get a const reference to the last char in the string view.
     * @return Reference to the last char in the string view.
     *
     * @note Calling `back()` on an empty string view results in undefined behaviour.
     */
    [[nodiscard]] constexpr const_reference back() const {
        return data_[size_-1];
    }

    /**
     * @brief Access a specified character
     * @return Reference to the specified char.
     *
     * @note The behaviour is undefined if `index >= size()`
     */
    [[nodiscard]] constexpr const_reference operator[](size_type index) const {
        return data_[index];
    }

    /**
     * @brief Create a new `string_view` defined by an `offset` and `len`
     * @param offset The offset where the new `string_view` should start
     * @param count The requested length of the new `string_view`.
     * @return A view of the substring `[pos, pos + rcount)` where `rcount` is the smaller of `count` and `size() - pos`.
     *
     * @note If `pos >= size()` then an empty view, `string_view(end(), 0)`, is returned.
     */
    [[nodiscard]] constexpr string_view substr(size_type pos, size_type count = npos) const {
        if (pos >= size_) {
            return string_view(end(), 0);
        }

        auto rcount = (std::min)(count, size() - pos);
        return string_view(data_ + pos, rcount);
    }

    /**
     * @brief Create a new `string_view` that starts with `needle` (or is empty)
     * @param needle The needle to search for
     * @return A view equivalent to `sv.substr(sv.find(needle))`.
     */
    [[nodiscard]] constexpr string_view substr_starting_with(string_view needle) const {
        return substr(find(needle));
    }

    /**
     * @brief Move the start `n` characters forward
     * @param n Number of chars to move the start forward.
     * @return A view containing the removed part.
     *
     * @note This modifies the current view in place.
     *
     * Unlike `std::string_view` this function does not result in undefined behaviour if `n > size()`
     */
    constexpr string_view remove_prefix(size_type n) {
        auto retval = substr(0, n);
        *this = substr(n);
        return retval;
    }

    /**
     * @brief Move the end `n` characters backwards
     * @param n Number of chars to move the end backwards.
     * @return A view containing the removed part.
     *
     * @note This modifies the current view in place.
     *
     * Unlike `std::string_view` this function does not result in undefined behaviour if `n > size()`
     */
    constexpr string_view remove_suffix(size_type n) {
        n = (std::min)(n, size_);
        auto retval = substr(size_ - n);
        size_ -= n;
        return retval;
    }

    /**
     * @brief Checks if the string view starts with a certain prefix.
     * @param sv The prefix to check
     * @return True if `this` starts with `sv`
     */
    [[nodiscard]] constexpr bool starts_with(string_view sv) const {
        return substr(0, sv.size()) == sv;
    }

    /**
     * @brief Checks if the string view ends with a certain suffix.
     * @param sv The suffix to check
     * @return True if `this` ends with `sv`
     */
    [[nodiscard]] constexpr bool ends_with(string_view sv) const {
        if (size() >= sv.size()) {
            return substr(size() - sv.size()) == sv;
        }
        return false;
    }

    /**
     * @brief Find a specified character in the view
     * @param ch The character to find
     * @return The index of `ch` or `npos` if not found.
     */
    [[nodiscard]] constexpr size_type find(char ch) const {
        for(size_type i=0; i<size(); i++) {
            if (data_[i] == ch) {
                return i;
            }
        }
        return npos;
    }

    /**
     * @brief Find the index of a specified view
     * @param needle The needle to search for in the view
     * @return The index such that `sv.substr(sv.find(needle)).starts_with(needle) == true`, or `npos` if not found.
     */
    [[nodiscard]] constexpr size_type find(string_view needle) const {
        if (needle.is_empty()) {
            return 0;
        }
        else if (size() == needle.size()) {
            return *this == needle ? 0:npos;
        }
        else if (size() > needle.size()) {
            const auto end_ = end();
            const auto first = needle.front();
            const auto search_size = size() - needle.size() + 1;
            for (size_type i=0; i < search_size; ++i) {
                const auto begin_ = data_ + i;
                if (*begin_ == first && string_view(begin_, end_).starts_with(needle)) {
                    return i;
                }
            }
        }
        return npos;
    }

    /**
     * @brief Find then nth occurrence of a specified needle
     * @param needle The needle to search for.
     * @param n Which occurrence to find, 0-based
     * @return The start of the nth (0-based) occurrence of the needle.
     *
     * `sv.find_nth(needle, 0)` is equivalent to `sv.find(needle)`.
     */
    [[nodiscard]] constexpr size_type find_nth(string_view needle, size_type n) const {
        string_view haystack = *this;
        for(size_type i=0; i<=n && haystack.size() >= needle.size(); i++) {
            auto found = haystack.find(needle);
            if (found == npos) {
                return npos;
            }
            else if (i == n) {
                return haystack.data() - data() + found;
            }
            else {
                // +1 since we need to step over the start of the found index.
                haystack.remove_prefix(found + 1);
            }
        }
        return npos;
    }

    /**
     * @brief Find the index of the last occurrence of a character
     * @param ch The character to find
     * @return The index of the last occurrence of `needle`, or `npos` if not found.
     */
    [[nodiscard]] constexpr size_type rfind(char ch) const {
        for(size_type i=size(); i > 0; --i) {
            if (data_[i-1] == ch) {
                return i-1;
            }
        }
        return npos;
    }

    /**
     * @brief Find the index of the last occurrence of a string
     * @param needle The needle to search for
     * @return The index to the start of the last occurrence of `needle`, or `npos` if not found.
     */
    [[nodiscard]] constexpr size_type rfind(string_view needle) const {
        if (needle.is_empty()) {
            return size();
        }
        else if (size() == needle.size()) {
            return *this == needle ? 0:npos;
        }
        else if (size() > needle.size()) {
            const auto end_ = end();
            const auto first = needle.front();
            for (size_type i=size() - needle.size() + 1; i > 0; --i) {
                const auto begin_ = data_ + i - 1;
                if (*begin_ == first && string_view(begin_, end_).starts_with(needle)) {
                    return i-1;
                }
            }
        }
        return npos;
    }

    /**
     * @brief Check if a sub string contains a certain needle
     * @param needle The needle to search
     * @return A value equivalent to `find(needle) != npos`
     */
    [[nodiscard]] constexpr bool contains(string_view needle) const {
        return find(needle) != npos;
    }

    /**
     * @brief Check if a sub string contains a certain needle
     * @param needle The needle to search
     * @return A value equivalent to `find(needle) != npos`
     */
    [[nodiscard]] constexpr bool contains(char needle) const {
        return find(needle) != npos;
    }

    /**
     * @brief Compare two views
     * @param right The right hand side of the comparison
     * @return 0 if the two views are equal, a negative value if `this` compares less than `right`, otherwise a positive value.
     */
    [[nodiscard]] constexpr int compare(string_view right) const {
        auto cmp_impl = [this, &right]() {
            const size_t to_check = (std::min(size(), right.size()));
            for(size_t i=0; i<to_check; i++) {
                if (data_[i] < right[i]) {
                    return -1;
                }
                else if(data_[i] > right[i]) {
                    return 1;
                }
            }
            return 0;
        };
        int memcmp_result = cmp_impl();
        if (memcmp_result == 0) {
            if (size() < right.size()) {
                return -1;
            }
            else if (size() > right.size()) {
                return 1;
            }
            else {
                return 0;
            }
        }
        return memcmp_result;
    }

    friend constexpr bool operator==(const string_view& lhs, const string_view& rhs) {
        if (lhs.size() != rhs.size()) {
            return false;
        }
        return lhs.compare(rhs) == 0;
    }

    friend constexpr bool operator!=(const string_view& lhs, const string_view& rhs) {
        return !(lhs == rhs);
    }
};

constexpr string_view operator""_sv(const char* s, std::size_t len) noexcept {
    return string_view(s, len);
}

}// namespace andwass
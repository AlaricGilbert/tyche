#pragma once
#include <cstddef>
#include <exception>
#include <memory>
#include <string>
namespace tyche::utils {
    template <class T> class shared_array;
    template <class T>
    shared_array<T> make_shared(const T* arr, std::size_t length);
    template <class T>
    shared_array<T> make_shared(const T* arr, std::size_t offset,
                                std::size_t length);

    template <class T> class shared_array {
    public:
        typedef T element_type;

        [[nodiscard]] std::size_t length() const { return _length; }

        [[nodiscard]] std::size_t use_count() const { return *_use_count; }

        [[nodiscard]] element_type* get() const { return _pointer; }

        element_type& operator[](std::size_t index) {
            if (index < _length) return _pointer[index];
            throw std::out_of_range("Index of shared_array was out of range");
        }

        element_type operator[](std::size_t index) const {
            if (index < _length) return _pointer[index];
            throw std::out_of_range("Index of shared_array was out of range");
        }

        element_type* swap(element_type* target, std::size_t length) {
            element_type* current_pointer = _pointer;
            _pointer = target;
            _length = length;
            return current_pointer;
        }

        void reset(element_type* target, std::size_t length) {
            _pointer = target;
            _length = length;
        }

        explicit shared_array(std::size_t length) : _use_count(new size_t(1)) {
            _length = length;
            _pointer = new element_type[length];
        }

        shared_array() : _use_count(new size_t(1)) {
            _length = 0;
            _pointer = nullptr;
        }

        shared_array(element_type* arr, std::size_t length)
            : _use_count(new size_t(1)) {
            _length = length;
            _pointer = arr;
        }

        shared_array(const shared_array<T>& other) {
            _pointer = other._pointer;
            _length = other._length;
            _use_count = other._use_count;
            (*_use_count)++;
        }

        shared_array(shared_array<T>&& other) noexcept {
            _pointer = other._pointer;
            _length = other._length;
            _use_count = other._use_count;

            other._pointer = nullptr;
            other._length = 0;
            other._use_count = nullptr;
        }

        shared_array& operator=(const shared_array<T>& other) {
            if (&other == this) return *this;
            _pointer = other._pointer;
            _length = other._length;
            _use_count = other._use_count;
            (*_use_count)++;
            return *this;
        }

        ~shared_array() {
            if (_use_count != nullptr) {
                if (*_use_count == 1) {
                    delete _use_count;
                    if (_pointer != nullptr) delete[] _pointer;
                } else {
                    (*_use_count)--;
                }
            }
        }

    private:
        friend shared_array<T> make_shared<T>(const T* arr, std::size_t length);
        friend shared_array<T> make_shared<T>(const T* arr, std::size_t offset,
                                              std::size_t length);
        T* _pointer;
        std::size_t _length;
        std::size_t* _use_count;
    };
    template <class T>
    shared_array<T> make_shared(const T* arr, std::size_t length) {
        shared_array<T> instance(length);
        instance._pointer = new T[length];
        std::copy_n(arr, length, instance._pointer);
        return instance;
    }
    template <class T>
    shared_array<T> make_shared(const T* arr, std::size_t offset,
                                std::size_t length) {
        shared_array<T> instance(length);
        instance._pointer = new T[length];
        std::copy_n(arr, length, instance._pointer + offset);
        return instance;
    }

    shared_array<char> from_string_with_end(const std::string& str);
    shared_array<char> from_string(const std::string& str);
    template <std::size_t N>
    shared_array<char> from_string_with_end(const char (&str)[N]) {
        return make_shared(str, N);
    }
    template <std::size_t N>
    shared_array<char> from_string(const char (&str)[N]) {
        return make_shared(str, N - 1);
    }
}  // namespace tyche::utils
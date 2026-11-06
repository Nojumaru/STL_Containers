#pragma once

#include <memory>
#include <stdexcept>
#include <utility>
#include <initializer_list>


namespace nojo {

    template <typename T, typename Allocator = std::allocator<T>>
    class vector {
    public:
        using value_type = T;
        using pointer = typename std::allocator_traits<Allocator>::pointer;
        using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
        using reference = value_type&;
        using const_reference = const value_type&;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using allocator_type = Allocator;

        using iterator = pointer;
        using const_iterator = const_pointer;
    private:
        using alloc_traits = std::allocator_traits<Allocator>;

        pointer data_ = nullptr;
        size_type size_ = 0;
        size_type capacity_ = 0;
        Allocator alloc_;

    public:
        //base construct
        vector() noexcept(noexcept(Allocator())) = default;

        explicit vector(const Allocator& alloc) noexcept
            : alloc_(alloc) {}
        
        //copy construct
        vector(const vector& other) {
            if(other.size_ > 0) {
                data_ = alloc_traits::allocate(alloc_, other.capacity_);
                capacity_ = other.capacity_;

                size_type counter = 0;
                try {
                    for(; counter < other.size_; ++counter) {
                        alloc_traits::construct(
                            alloc_,
                            data_ + counter,
                            other.data_[counter]
                        );
                    }
                    size_ = other.size_;
                } catch (...) {
                    for(size_type i = 0; i < counter; ++i) {
                        alloc_traits::destroy(alloc_, data_ + i);
                    }
                    alloc_traits::deallocate(alloc_, data_, capacity_);
                    data_ = nullptr;
                    capacity_ = 0;
                    throw;
                }
            }
        }
        //move construct
        vector(vector&& other) noexcept 
            : data_(other.data_)
            , size_(other.size_)
            , capacity_(other.capacity_)
            , alloc_(std::move(other.alloc_))
        {
            other.data_ = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
        }

        vector(std::initializer_list<value_type> init, const Allocator& alloc = Allocator()) : alloc_(alloc) {
            if (init.size() > 0) {
                data_ = alloc_traits::allocate(alloc_, init.size());
                capacity_ = init.size();

                size_type counter = 0;
                try {
                    for (const auto& obj : init) {
                        alloc_traits::construct(
                            alloc_,
                            data_ + counter,
                            obj
                        );
                        ++counter;
                    }
                    size_ = counter;
                } catch (...) {
                    for (size_type i = 0; i < counter; ++i) {
                        alloc_traits::destroy(alloc_, data_ + i);
                    }
                    alloc_traits::deallocate(alloc_, data_, capacity_);
                    data_ = nullptr;
                    capacity_ = 0;
                    throw;
                }
            }
        }

        vector& operator=(std::initializer_list<value_type> init) {
            vector temp(init, alloc_);
            swap(*this, temp);
            return *this;
        }
        ~vector() {
            clear();
            deallocate_buffer();
        }
       
        //operators
        vector& operator=(vector other) {
            swap(*this, other);
            return *this;
        }
        vector& operator=(vector&& other) noexcept {
            if (this != &other) {
                clear();
                if (data_ != nullptr) {
                    alloc_traits::deallocate(alloc_, data_, capacity_);
                }

                data_ = other.data_;
                size_ = other.size_;
                capacity_ = other.capacity_;
                alloc_ = std::move(other.alloc_);

                other.data_ = nullptr;
                other.size_ = 0;
                other.capacity_ = 0;
            }
            return *this;
        }

        reference operator[](size_type id) noexcept {
            return data_[id];
        }
        const_reference operator[](size_type id) const noexcept {
            return data_[id];
        }
        //calls
        void reserve(size_type new_capacity) {
            if(new_capacity <= capacity_) return;
            pointer new_data = alloc_traits::allocate(alloc_, new_capacity);

            size_type counter = 0;
            try {
                for(;counter < size_; ++counter) {
                    alloc_traits::construct(        
                        alloc_,
                        new_data + counter,
                        std::move_if_noexcept(data_[counter])
                    );
                }
            } catch(...) {
                for(size_type i = 0; i < counter; ++i) {
                    alloc_traits::destroy(alloc_, new_data + i);
                }

                alloc_traits::deallocate(alloc_, new_data, new_capacity);

                throw;
            }
            for (size_type i = 0; i < size_; ++i) {
                alloc_traits::destroy(alloc_, data_ + i);
            }
            if (data_ != nullptr) {
                alloc_traits::deallocate(alloc_, data_, capacity_);
            }
            data_ = new_data;
            capacity_ = new_capacity;
        }
        size_type size() const noexcept { return size_; }
        size_type capacity() const noexcept { return capacity_; };
        bool empty() const noexcept { return size_ == 0; } 

        //iters
        iterator begin() noexcept { return data_; };
        const_iterator begin() const noexcept { return data_; };
        const_iterator cbegin() const noexcept { return data_; };
        
        iterator end() noexcept { return data_ + size_; }
        iterator end() const noexcept { return data_ + size_; }
        iterator cend() const noexcept { return data_ + size_; }

        void push_back(const_reference value) {
            emplace_back(value);
        }

        void push_back(value_type&& value) {
            emplace_back(std::move(value));
        }

        template <typename... Args>
        reference emplace_back(Args&&... args) {
            if (size_ == capacity_) {
                size_type new_capacity = (capacity_ == 0) ? 1 : capacity_ * 2;
                reserve(new_capacity);
            }

            alloc_traits::construct(
                alloc_,
                data_ + size_,
                std::forward<Args>(args)...
            );

            return data_[size_++];
        }

        void clear() noexcept {
            for (size_type i = 0; i < size_; ++i) {
                alloc_traits::destroy(alloc_, data_ + i);
            }
            size_ = 0;
        }
    private:
        friend void swap(vector& first, vector& second) noexcept {
            using std::swap;
            swap(first.data_, second.data_);
            swap(first.size_, second.size_);
            swap(first.capacity_, second.capacity_);
            swap(first.alloc_, second.alloc_);
        }
        void deallocate_buffer() noexcept {
            if (data_) {
                alloc_traits::deallocate(alloc_, data_, capacity_);
                data_ = nullptr;
                capacity_ = 0;
            }
        }
    };  
}
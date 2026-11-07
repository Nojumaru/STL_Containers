#pragma once

#include <memory>
#include <utility>


namespace nojo {
    template <typename Key, typename T, typename Compare = std::less<Key>, typename Allocator = std::allocator<std::pair<const Key, T>>>
    class map    {
        using key_type = Key;
        using value_type  = T;
        using pointer = typename std::allocator_traits<Allocator>::pointer;
        using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
        
    };
}
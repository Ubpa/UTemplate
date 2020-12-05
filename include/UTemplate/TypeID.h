#pragma once

// ref: https://github.com/skypjack/entt

#include "Name.h"

#include <cstdint>
#include <type_traits>

namespace Ubpa::details {
    template<typename>
    struct fnv1a_traits;

    template<>
    struct fnv1a_traits<std::uint32_t> {
        using type = std::uint32_t;
        static constexpr std::uint32_t offset = 2166136261;
        static constexpr std::uint32_t prime = 16777619;
    };

    template<>
    struct fnv1a_traits<std::uint64_t> {
        using type = std::uint64_t;
        static constexpr std::uint64_t offset = 14695981039346656037ull;
        static constexpr std::uint64_t prime = 1099511628211ull;
    };

    class hashed_string {
        using traits_type = fnv1a_traits<size_t>;

        struct const_wrapper {
            // non-explicit constructor on purpose
            constexpr const_wrapper(const char* curr) noexcept : str{ curr } {}
            const char* str;
        };

        // Fowler CNoll CVo hash function v. 1a - the good
        static constexpr size_t helper(const char* curr) noexcept {
            auto value = traits_type::offset;

            while (*curr != 0) {
                value = (value ^ static_cast<traits_type::type>(*(curr++))) * traits_type::prime;
            }

            return value;
        }

        // Fowler CNoll CVo hash function v. 1a - the good
        static constexpr size_t helper(const char* str, size_t n) noexcept {
            auto value = traits_type::offset;

            for (size_t i = 0; i < n; i++)
                value = (value ^ static_cast<traits_type::type>(str[i])) * traits_type::prime;

            return value;
        }

    public:
		using value_type = char;
		using hash_type = size_t;

        template<size_t N>
        static constexpr hash_type value(const value_type(&str)[N]) noexcept {
            return helper(str);
        }

        static constexpr hash_type value(std::string_view str) noexcept {
            return helper(str.data(), str.size());
        }

        static constexpr hash_type value(const_wrapper wrapper) noexcept {
            return helper(wrapper.str);
        }

        static constexpr hash_type value(const value_type* str, size_t size) noexcept {
            size_t partial{ traits_type::offset };
            while (size--) { partial = (partial ^ (str++)[0]) * traits_type::prime; }
            return partial;
        }

        constexpr hashed_string() noexcept
            : str{ nullptr }, hash{}
        {}

        template<size_t N>
        constexpr hashed_string(const value_type(&curr)[N]) noexcept
            : str{ curr }, hash{ helper(curr) }
        {}

        explicit constexpr hashed_string(const_wrapper wrapper) noexcept
            : str{ wrapper.str }, hash{ helper(wrapper.str) }
        {}

        constexpr const value_type* data() const noexcept {
            return str;
        }

        constexpr hash_type value() const noexcept {
            return hash;
        }

        constexpr operator const value_type* () const noexcept { return data(); }

        constexpr operator hash_type() const noexcept { return value(); }

        constexpr bool operator==(const hashed_string& other) const noexcept {
            return hash == other.hash;
        }

    private:
        const value_type* str;
        hash_type hash;
    };
}

namespace Ubpa {
    template<typename T>
    constexpr size_t TypeID = details::hashed_string::value(type_name<T>().name);
    constexpr size_t RuntimeTypeID(std::string_view str) {
        return details::hashed_string::value(str);
    }

    template<typename X, typename Y> struct TypeID_Less;
    template<typename X, typename Y> constexpr bool TypeID_Less_v = TypeID_Less<X, Y>::value;
}

namespace Ubpa {
    template<typename X, typename Y>
    struct TypeID_Less {
        static constexpr bool value = TypeID<X> < TypeID<Y>;
        static_assert(std::is_same_v<X, Y> || TypeID<X> != TypeID<Y>);
    };
}

#pragma once

// ref: https://github.com/skypjack/entt

#include <nameof.hpp>

#include <cstdint>
#include <type_traits>

//#if defined _MSC_VER
//	#define UBPA_PRETTY_FUNCTION __FUNCSIG__
//	//#define UBPA_PRETTY_FUNCTION_CONSTEXPR ENTT_PRETTY_FUNCTION
//#elif defined __clang__ || (defined __GNUC__ && __GNUC__ > 8)
//	#define UBPA_PRETTY_FUNCTION __PRETTY_FUNCTION__
//	//#define UBPA_PRETTY_FUNCTION_CONSTEXPR ENTT_PRETTY_FUNCTION
//#elif defined __GNUC__
//	#define UBPA_PRETTY_FUNCTION __PRETTY_FUNCTION__
//#endif

namespace Ubpa::detail::TypeID_ {
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

        // Fowler�CNoll�CVo hash function v. 1a - the good
        static constexpr size_t helper(const char* curr) noexcept {
            auto value = traits_type::offset;

            while (*curr != 0) {
                value = (value ^ static_cast<traits_type::type>(*(curr++))) * traits_type::prime;
            }

            return value;
        }

        // Fowler�CNoll�CVo hash function v. 1a - the good
        static constexpr size_t helper(const char* str, size_t n) noexcept {
            auto value = traits_type::offset;

            for (size_t i = 0; i < n; i++)
                value = (value ^ static_cast<traits_type::type>(str[i])) * traits_type::prime;

            return value;
        }

    public:
        /*! @brief character type. */
        using value_type = char;
        /*! @brief Unsigned integer type. */
        using hash_type = size_t;

        /**
         * @brief Returns directly the numeric representation of a string.
         *
         * Forcing template resolution avoids implicit conversions. An
         * human-readable identifier can be anything but a plain, old bunch of
         * characters.<br/>
         * Example of use:
         * @code{.cpp}
         * const auto value = hashed_string<char>::to_value("my.png");
         * @endcode
         *
         * @tparam N Number of characters of the identifier.
         * @param str Human-readable identifer.
         * @return The numeric representation of the string.
         */
        template<size_t N>
        static constexpr hash_type value(const value_type(&str)[N]) noexcept {
            return helper(str);
        }

        static constexpr hash_type value(std::string_view str) noexcept {
            return helper(str.data(), str.size());
        }

        /**
         * @brief Returns directly the numeric representation of a string.
         * @param wrapper Helps achieving the purpose by relying on overloading.
         * @return The numeric representation of the string.
         */
        static hash_type value(const_wrapper wrapper) noexcept {
            return helper(wrapper.str);
        }

        /**
         * @brief Returns directly the numeric representation of a string view.
         * @param str Human-readable identifer.
         * @param size Length of the string to hash.
         * @return The numeric representation of the string.
         */
        static hash_type value(const value_type* str, size_t size) noexcept {
            size_t partial{ traits_type::offset };
            while (size--) { partial = (partial ^ (str++)[0]) * traits_type::prime; }
            return partial;
        }

        /*! @brief Constructs an empty hashed string. */
        constexpr hashed_string() noexcept
            : str{ nullptr }, hash{}
        {}

        /**
         * @brief Constructs a hashed string from an array of const characters.
         *
         * Forcing template resolution avoids implicit conversions. An
         * human-readable identifier can be anything but a plain, old bunch of
         * characters.<br/>
         * Example of use:
         * @code{.cpp}
         * hashed_string<char> hs{"my.png"};
         * @endcode
         *
         * @tparam N Number of characters of the identifier.
         * @param curr Human-readable identifer.
         */
        template<size_t N>
        constexpr hashed_string(const value_type(&curr)[N]) noexcept
            : str{ curr }, hash{ helper(curr) }
        {}

        /**
         * @brief Explicit constructor on purpose to avoid constructing a hashed
         * string directly from a `const value_type *`.
         * @param wrapper Helps achieving the purpose by relying on overloading.
         */
        explicit constexpr hashed_string(const_wrapper wrapper) noexcept
            : str{ wrapper.str }, hash{ helper(wrapper.str) }
        {}

        /**
         * @brief Returns the human-readable representation of a hashed string.
         * @return The string used to initialize the instance.
         */
        constexpr const value_type* data() const noexcept {
            return str;
        }

        /**
         * @brief Returns the numeric representation of a hashed string.
         * @return The numeric representation of the instance.
         */
        constexpr hash_type value() const noexcept {
            return hash;
        }

        /*! @copydoc data */
        constexpr operator const value_type* () const noexcept { return data(); }

        /**
         * @brief Returns the numeric representation of a hashed string.
         * @return The numeric representation of the instance.
         */
        constexpr operator hash_type() const noexcept { return value(); }

        /**
         * @brief Compares two hashed strings.
         * @param other Hashed string with which to compare.
         * @return True if the two hashed strings are identical, false otherwise.
         */
        constexpr bool operator==(const hashed_string& other) const noexcept {
            return hash == other.hash;
        }

    private:
        const value_type* str;
        hash_type hash;
    };

    template<typename T>
    struct TypeID {
        /**
         * @brief Returns the numeric representation of a given type.
         * @return The numeric representation of the given type.
         */
        static constexpr size_t id() noexcept {
            std::string_view name = nameof::nameof_type<T>();
            #if defined(_MSC_VER)
            if (name.size() > sizeof("enum") && name[0] == 'e' && name[1] == 'n' && name[2] == 'u' && name[3] == 'm' && name[4] == ' ') {
                name.remove_prefix(sizeof("enum"));
            }
            if (name.size() > sizeof("class") && name[0] == 'c' && name[1] == 'l' && name[2] == 'a' && name[3] == 's' && name[4] == 's' && name[5] == ' ') {
                name.remove_prefix(sizeof("class"));
            }
            if (name.size() > sizeof("struct") && name[0] == 's' && name[1] == 't' && name[2] == 'r' && name[3] == 'u' && name[4] == 'c' && name[5] == 't' && name[6] == ' ') {
                name.remove_prefix(sizeof("struct"));
            }
            #endif
            auto value = hashed_string::value(name);
            return value;
        }
    };
}

namespace Ubpa {
    template<typename T>
    constexpr size_t TypeID = detail::TypeID_::TypeID<T>::id();
    constexpr size_t RuntimeTypeID(std::string_view str) {
        return detail::TypeID_::hashed_string::value(str);
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

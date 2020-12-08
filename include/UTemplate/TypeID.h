#pragma once

#include "Name.h"

namespace Ubpa {
    // hash of <T>'s type name
    template<typename T>
    constexpr size_t TypeID = string_hash(type_name<T>().name);
    constexpr size_t RuntimeTypeID(std::string_view str) { return string_hash(str); }

    template<typename X, typename Y> struct TypeID_Less : IValue<bool, TypeID<X> < TypeID<Y>>
    { static_assert(std::is_same_v<X, Y> || TypeID<X> != TypeID<Y>); };

    template<typename X, typename Y> constexpr bool TypeID_Less_v = TypeID_Less<X, Y>::value;
}

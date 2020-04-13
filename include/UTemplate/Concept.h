#pragma once

#include <type_traits>

#define Concept(name, expr) using name = decltype(expr)

namespace Ubpa::detail::Concept_ {
	template <template <class...> class Trait, class Enabler, class... Args>
	struct Require;
}

namespace Ubpa {
	template <template <class...> class Trait, class... Args>
	constexpr bool Require = detail::Concept_::Require<Trait, void, Args...>::value;
}

namespace Ubpa::detail::Concept_ {
	template <template <class...> class Trait, class Enabler, class... Args>
	struct Require : std::false_type {};

	template <template <class...> class Trait, class... Args>
	struct Require<Trait, std::void_t<Trait<Args...>>, Args...> : std::true_type {};
}

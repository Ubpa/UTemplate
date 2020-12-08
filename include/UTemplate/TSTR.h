#pragma once

// marco
// - UBPA_TSTR
// - UBPA_TSTR_UTIL

#ifndef UBPA_TSTR
#define UBPA_TSTR

#include <string_view>

namespace Ubpa {
	template<typename Char, Char... chars>
	struct TStr;
}

namespace Ubpa::details {
	template<typename STR>
	struct TSTRSizeof;
	template<typename Char>
	struct TSTRSizeof<std::basic_string_view<Char>> {
		static constexpr size_t get(const std::basic_string_view<Char>& str) noexcept {
			return str.size();
		}
	};
	template<typename Char>
	struct TSTRSizeof<const std::basic_string_view<Char>&> : TSTRSizeof<std::basic_string_view<Char>> {};
	template<typename Char>
	struct TSTRSizeof<const std::basic_string_view<Char>> : TSTRSizeof<std::basic_string_view<Char>> {};

	template<typename Char, size_t N>
	struct TSTRSizeof<const Char(&)[N]> {
		static constexpr size_t get(const Char(&str)[N]) noexcept {
			return N - 1;
		}
	};

	template <typename Char, typename T, size_t ...N>
	constexpr decltype(auto) TSTRHelperImpl(std::index_sequence<N...>) {
		return TStr<Char, T::get()[N]...>{};
	}

	template <typename T>
	constexpr decltype(auto) TSTRHelper(T) {
		using Char = std::decay_t<decltype(T::get()[0])>;
		return TSTRHelperImpl<Char, T>(std::make_index_sequence<TSTRSizeof<decltype(T::get())>::get(T::get())>());
	}
}

// [C-style string type (value)]
// in C++20, we can easily put a string into template parameter list
// but in C++17, we just can use this disgusting trick
#define TSTR(s)                                                           \
(Ubpa::details::TSTRHelper([] {                                           \
    struct tmp { static constexpr decltype(auto) get() { return (s); } }; \
    return tmp{};                                                         \
}()))

namespace Ubpa {
	template<typename Char_, Char_... chars>
	struct TStr {
		using Tag = TStr;
		using Char = Char_;
		template<typename T>
		static constexpr bool NameIs(T = {}) noexcept { return std::is_same_v<T, Tag>; }
		static constexpr char name_data[]{ chars...,Char(0) };
		static constexpr std::basic_string_view<Char> name{ name_data };
	};

	template<typename T>
	struct IsTStr : std::false_type {};

	template<typename Char, Char... chars>
	struct IsTStr<TStr<Char, chars...>> : std::true_type {};
}

#endif // UBPA_TSTR

#ifndef UBPA_TSTR_UTIL
#define UBPA_TSTR_UTIL

namespace Ubpa {
	template<typename Str0, typename Str1>
	struct concat_helper;
	template<typename Str0, typename Str1>
	using concat_helper_t = typename concat_helper<Str0, Str1>::type;
	template<typename Char, Char... c0, Char... c1>
	struct concat_helper<TStr<Char, c0...>, TStr<Char, c1...>> {
		using type = TStr<Char, c0..., c1...>;
	};

	template<typename Str0, typename Str1>
	constexpr auto concat(Str0 = {}, Str1 = {}) noexcept {
		return typename concat_helper<Str0, Str1>::type{};
	}

	template<typename... Strs>
	struct concat_seq_helper;
	template<typename... Strs>
	using concat_seq_helper_t = typename concat_seq_helper<Strs...>::type;
	template<typename Str>
	struct concat_seq_helper<Str> {
		using type = Str;
	};
	template<typename Str, typename... Strs>
	struct concat_seq_helper<Str, Strs...> {
		using type = concat_helper_t<Str, concat_seq_helper_t<Strs...>>;
	};
	template<typename... Strs>
	constexpr auto concat_seq(Strs...) noexcept {
		return concat_seq_helper_t<Strs...>{};
	}

	template<typename Seperator, typename... Strs>
	struct concat_seq_seperator_helper;
	template<typename Seperator, typename... Strs>
	using concat_seq_seperator_helper_t = typename concat_seq_seperator_helper<Seperator, Strs...>::type;
	template<typename Seperator>
	struct concat_seq_seperator_helper<Seperator> {
		using type = TStr<typename Seperator::Char>;
	};
	template<typename Seperator, typename Str>
	struct concat_seq_seperator_helper<Seperator, Str> {
		using type = Str;
	};
	template<typename Seperator, typename Str, typename... Strs>
	struct concat_seq_seperator_helper<Seperator, Str, Strs...> {
		using type = concat_helper_t<concat_helper_t<Str, Seperator>, concat_seq_seperator_helper_t<Seperator, Strs...>>;
	};
	template<typename Seperator, typename... Strs>
	constexpr auto concat_seq_seperator(Seperator, Strs...) noexcept {
		return concat_seq_seperator_helper_t<Seperator, Strs...>{};
	}

	template<typename Str, typename X>
	constexpr size_t find(Str = {}, X = {}) noexcept {
		static_assert(IsTStr<Str>::value && IsTStr<X>::value);
		if constexpr (Str::name.size() >= X::name.size()) {
			for (size_t i = 0; i < Str::name.size() - X::name.size() + 1; i++) {
				bool flag = true;
				for (size_t k = 0; k < X::name.size(); k++) {
					if (Str::name[i + k] != X::name[k]) {
						flag = false;
						break;
					}
				}
				if (flag)
					return i;
			}
		}
		return static_cast<size_t>(-1);
	}

	template<typename Str, typename X>
	constexpr size_t find_last(Str = {}, X = {}) noexcept {
		static_assert(IsTStr<Str>::value && IsTStr<X>::value);
		if constexpr (Str::name.size() >= X::name.size()) {
			for (size_t i = 0; i < Str::name.size() - X::name.size() + 1; i++) {
				size_t idx = Str::name.size() - X::name.size() - i;
				bool flag = true;
				for (size_t k = 0; k < X::name.size(); k++) {
					if (Str::name[idx + k] != X::name[k]) {
						flag = false;
						break;
					}
				}
				if (flag)
					return idx;
			}
		}
		return static_cast<size_t>(-1);
	}

	template<typename Str, typename X>
	constexpr bool starts_with(Str = {}, X = {}) noexcept {
		static_assert(IsTStr<Str>::value && IsTStr<X>::value);
		if (Str::name.size() < X::name.size())
			return false;
		for (size_t i = 0; i < X::name.size(); i++) {
			if (Str::name[i] != X::name[i])
				return false;
		}
		return true;
	}

	template<typename Str, typename X>
	constexpr bool ends_with(Str = {}, X = {}) noexcept {
		static_assert(IsTStr<Str>::value && IsTStr<X>::value);
		if (Str::name.size() < X::name.size())
			return false;
		for (size_t i = 0; i < X::name.size(); i++) {
			if (Str::name[Str::name.size() - X::name.size() + i] != X::name[i])
				return false;
		}
		return true;
	}

	template<size_t N, typename Str>
	constexpr auto remove_prefix(Str = {}) {
		static_assert(IsTStr<Str>::value);
		if constexpr (Str::name.size() >= N)
			return TSTR(decltype(Str::name){Str::name.data() + N});
		else
			return TSTR("");
	}

	template<typename Str, typename X>
	constexpr auto remove_prefix(Str = {}, X = {}) {
		static_assert(IsTStr<Str>::value);
		static_assert(IsTStr<X>::value);
		if constexpr (starts_with<Str, X>())
			return remove_prefix<X::name.size(), Str>();
		else
			return Str{};
	}

	template<size_t N, typename Str>
	constexpr auto remove_suffix(Str = {}) {
		static_assert(IsTStr<Str>::value);
		if constexpr (Str::name.size() >= N)
			return TSTR((decltype(Str::name){Str::name.data(), Str::name.size() - N}));
		else
			return TSTR("");
	}

	template<typename Str, typename X>
	constexpr auto remove_suffix(Str = {}, X = {}) {
		static_assert(IsTStr<Str>::value);
		if constexpr (ends_with<Str, X>())
			return remove_suffix<X::name.size(), Str>();
		else
			return Str{};
	}

	template<size_t N, typename Str>
	constexpr auto get_prefix(Str = {}) {
		static_assert(IsTStr<Str>::value);
		if constexpr (Str::name.size() >= N)
			return TSTR((decltype(Str::name){Str::name.data(), N}));
		else
			return Str{};
	}

	template<size_t N, typename Str>
	constexpr auto get_suffix(Str = {}) {
		static_assert(IsTStr<Str>::value);
		if constexpr (Str::name.size() >= N)
			return TSTR(decltype(Str::name){Str::name.data() + Str::name.size() - N});
		else
			return Str{};
	}

	// [Left, Right)
	template<size_t Idx, size_t Cnt, typename Str, typename X>
	constexpr auto replace(Str = {}, X = {}) {
		static_assert(IsTStr<Str>::value);
		static_assert(IsTStr<X>::value);
		constexpr auto prefix = remove_suffix<Str::name.size() - Idx>(Str{});
		constexpr auto suffix = remove_prefix<Idx + Cnt>(Str{});

		return concat(concat(prefix, X{}), suffix);
	}

	template<typename Str, typename From, typename To>
	constexpr auto replace(Str = {}, From = {}, To = {}) {
		static_assert(IsTStr<Str>::value);
		static_assert(IsTStr<From>::value);
		static_assert(IsTStr<To>::value);
		constexpr size_t idx = find(Str{}, From{});
		if constexpr (idx != static_cast<size_t>(-1))
			return replace(replace<idx, From::name.size()>(Str{}, To{}), From{}, To{});
		else
			return Str{};
	}

	template<typename Str, typename X>
	constexpr auto remove(Str = {}, X = {}) {
		return replace(Str{}, X{}, TSTR(""));
	}

	template<size_t Idx, size_t Cnt, typename Str>
	constexpr auto substr(Str = {}) {
		return get_prefix<Cnt>(remove_prefix<Idx, Str>());
	}

	template<auto V, std::enable_if_t<std::is_integral_v<decltype(V)>, int> = 0>
	constexpr auto int_to_TSTR() {
		if constexpr (V == 0)
			return TStr<char, '0'>{};
		else { // not zero
			using T = decltype(V);
			if constexpr (std::is_signed_v<T>) {
				if constexpr (V < 0)
					return concat(TStr<char, '-'>{}, int_to_TSTR<static_cast<std::make_unsigned_t<T>>(-V)>());
				else
					return int_to_TSTR<static_cast<std::make_unsigned_t<T>>(V)>();
			}
			else { // unsigned
				if constexpr (V < 10) {
					return TStr<char, '0' + V>{};
				}
				else
					return concat(int_to_TSTR<V / 10>(), int_to_TSTR<V % 10>());
			}
		}
	}
}

#endif // !UBPA_TSTR_UTIL
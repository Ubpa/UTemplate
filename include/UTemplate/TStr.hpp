#pragma once

// marco
// - UBPA_TSTR
// - UBPA_TSTR_UTIL

#ifndef UBPA_TSTR
#define UBPA_TSTR

#ifdef __cpp_nontype_template_parameter_class
#define UBPA_TSTR_NTTPC 1
#else
  #if defined(_MSC_VER) && _MSC_VER >= 1926
    #define UBPA_TSTR_NTTPC 1
  #endif
#endif

#include <string_view>
#include <utility>
#include <concepts>

namespace Ubpa {
	template<typename Char, std::size_t N>
	struct fixed_cstring {
		using value_type = Char;

		constexpr fixed_cstring(const value_type(&str)[N + 1]) noexcept {
			for (std::size_t i{ 0 }; i < size; ++i)
				data[i] = str[i];
			data[size] = 0;
		}

		constexpr fixed_cstring(std::basic_string_view<value_type> str) noexcept {
			for (std::size_t i{ 0 }; i < size; ++i)
				data[i] = str[i];
			data[size] = 0;
		}

		template<std::size_t N_ = N, std::enable_if_t<N_ == 0, int> = 0>
		constexpr fixed_cstring() noexcept : data{ 0 } {}

		template<std::size_t N_ = N, std::enable_if_t<N_ == 1, int> = 0>
		constexpr fixed_cstring(value_type c) noexcept : data{ c, 0 } {}
		
		template<typename... Chars>
		constexpr fixed_cstring(std::in_place_t, Chars... chars) noexcept : data{ static_cast<value_type>(chars)..., 0 }
		{ static_assert(sizeof...(chars) == size); }

		value_type data[N + 1]{};
		static constexpr std::size_t size = N;
	};

	template<typename Char, std::size_t N>
	fixed_cstring(const Char(&)[N])->fixed_cstring<Char, N - 1>;
}

#ifdef UBPA_TSTR_NTTPC

namespace Ubpa {
	template<fixed_cstring str>
	struct TStr {
		using Char = typename decltype(str)::value_type;

		template<typename T>
		static constexpr bool Is(T = {}) noexcept { return std::is_same_v<T, TStr>; }
		static constexpr const Char* Data() noexcept { return str.data; }
		static constexpr std::size_t Size() noexcept { return str.size; }
		static constexpr std::basic_string_view<Char> View() noexcept { return str.data; }
		constexpr operator std::basic_string_view<Char>() { return View(); }
	};

	template<char... chars>
	using TStrC_of = TStr < fixed_cstring<char, sizeof...(chars)>{ std::in_place, chars... } > ;
	template<auto c>
	using TStr_of_a = TStr< fixed_cstring<decltype(c), 1>(c) >;
}

#define TSTR(s)                                                                                      \
([] {                                                                                                \
    constexpr std::basic_string_view str{s};                                                         \
    return Ubpa::TStr<Ubpa::fixed_cstring<typename decltype(str)::value_type, str.size()>{ str }>{}; \
}())

#else
namespace Ubpa {
	template<typename Char, Char... chars>
	struct TStr;
}

namespace Ubpa::details {
	template <typename Char, typename T, std::size_t ...N>
	constexpr auto TSTRHelperImpl(std::index_sequence<N...>) {
		return TStr<Char, T::get()[N]...>{};
	}

	template <typename T>
	constexpr auto TSTRHelper(T) {
		using SV = decltype(T::get());
		using Char = typename SV::value_type;
		return TSTRHelperImpl<Char, T>(std::make_index_sequence<T::get().size()>{});
	}
}

// [C-style string type (value)]
// in C++20, we can easily put a string into template parameter list
// but in C++17, we just can use this disgusting trick
#define TSTR(s)                                                                       \
([] {                                                                                 \
    struct tmp { static constexpr auto get() { return std::basic_string_view{s}; } }; \
    return Ubpa::details::TSTRHelper(tmp{});                                          \
}())

namespace Ubpa {
	template<typename C, C... chars>
	struct TStr {
		using Char = C;
		template<typename T>
		static constexpr bool Is(T = {}) noexcept { return std::is_same_v<T, TStr>; }
		static constexpr const Char* Data() noexcept { return data; }
		static constexpr std::size_t Size() noexcept { return sizeof...(chars); }
		static constexpr std::basic_string_view<Char> View() noexcept { return data; }
		constexpr operator std::basic_string_view<Char>() { return View(); }
	private:
		static constexpr Char data[]{ chars...,Char(0) };
	};

	template<char... chars>
	using TStrC_of = TStr<char, chars...>;
	template<auto c>
	using TStr_of_a = TStr<decltype(c), c>;
}

#endif // UBPA_TSTR_NTTPC
#endif // UBPA_TSTR

#ifndef UBPA_TSTR_UTIL
#define UBPA_TSTR_UTIL

namespace Ubpa {
	template<typename T>
	concept TStrLike = requires{
		{T::View()}->std::same_as<std::basic_string_view<typename T::Char>>;
	};

#ifdef UBPA_TSTR_NTTPC
	template<typename Str>
	constexpr auto empty_of(Str = {}) noexcept { return TStr < fixed_cstring<typename Str::Char, 0>{} > {}; }
#else
	template<typename Str>
	constexpr auto empty_of(Str = {}) noexcept { return TStr < typename Str::Char > {}; }
#endif

	template<typename Str0, typename Str1>
	struct concat_helper;
	template<typename Str0, typename Str1>
	using concat_helper_t = typename concat_helper<Str0, Str1>::type;

#ifdef UBPA_TSTR_NTTPC
	template<typename Str0, typename Str1>
	struct concat_helper {
	private:
		template<std::size_t... LNs, std::size_t... RNs>
		static constexpr auto get(std::index_sequence<LNs...>, std::index_sequence<RNs...>) noexcept {
			return TStrC_of<Str0::View()[LNs]..., Str1::View()[RNs]...>{};
		}
	public:
		using type = decltype(get(std::make_index_sequence<Str0::Size()>{}, std::make_index_sequence<Str1::Size()>{}));
	};
#else
	template<typename Char, Char... c0, Char... c1>
	struct concat_helper<TStr<Char, c0...>, TStr<Char, c1...>> {
		using type = TStr<Char, c0..., c1...>;
	};
#endif // UBPA_TSTR_NTTPC

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
		using type = decltype(empty_of<Seperator>());
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
	constexpr std::size_t find(Str = {}, X = {}) noexcept {
		if constexpr (Str::Size() >= X::Size()) {
			for (std::size_t i = 0; i < Str::Size() - X::Size() + 1; i++) {
				bool flag = true;
				for (std::size_t k = 0; k < X::Size(); k++) {
					if (Str::View()[i + k] != X::View()[k]) {
						flag = false;
						break;
					}
				}
				if (flag)
					return i;
			}
		}
		return static_cast<std::size_t>(-1);
	}

	template<typename Str, typename X>
	constexpr std::size_t find_last(Str = {}, X = {}) noexcept {
		if constexpr (Str::Size() >= X::Size()) {
			for (std::size_t i = 0; i < Str::Size() - X::Size() + 1; i++) {
				std::size_t idx = Str::Size() - X::Size() - i;
				bool flag = true;
				for (std::size_t k = 0; k < X::Size(); k++) {
					if (Str::View()[idx + k] != X::View()[k]) {
						flag = false;
						break;
					}
				}
				if (flag)
					return idx;
			}
		}
		return static_cast<std::size_t>(-1);
	}

	template<typename Str, typename X>
	constexpr bool starts_with(Str = {}, X = {}) noexcept {
		if constexpr (Str::Size() < X::Size())
			return false;
		else {
			for (std::size_t i = 0; i < X::Size(); i++) {
				if (Str::View()[i] != X::View()[i])
					return false;
			}
			return true;
		}
	}

	template<typename Str, typename X>
	constexpr bool ends_with(Str = {}, X = {}) noexcept {
		if constexpr (Str::Size() < X::Size())
			return false;
		else {
			for (std::size_t i = 0; i < X::Size(); i++) {
				if (Str::View()[Str::Size() - X::Size() + i] != X::View()[i])
					return false;
			}
			return true;
		}
	}

	template<std::size_t N, typename Str>
	constexpr auto remove_prefix(Str = {}) {
		if constexpr (Str::Size() >= N) {
			return TSTR(Str::Data() + N);
		}
		else
			return empty_of(Str{});
	}

	template<typename Str, typename X>
	constexpr auto remove_prefix(Str = {}, X = {}) {
		if constexpr (starts_with<Str, X>())
			return remove_prefix<X::Size(), Str>();
		else
			return Str{};
	}

	template<std::size_t N, typename Char>
	constexpr std::basic_string_view<Char> detail_remove_suffix(std::basic_string_view<Char> str) {
		str.remove_suffix(N);
		return str;
	}

	template<std::size_t N, typename Str>
	constexpr auto remove_suffix(Str = {}) {
		if constexpr (Str::Size() >= N)
			return TSTR(detail_remove_suffix<N>(Str::View()));
		else
			return empty_of(Str{});
	}

	template<typename Str, typename X>
	constexpr auto remove_suffix(Str = {}, X = {}) {
		if constexpr (ends_with<Str, X>())
			return remove_suffix<X::Size(), Str>();
		else
			return Str{};
	}

	template<std::size_t N, typename Char>
	constexpr std::basic_string_view<Char> detail_get_prefix(std::basic_string_view<Char> str) {
		return { str.data(), N };
	}

	template<std::size_t N, typename Str>
	constexpr auto get_prefix(Str = {}) {
		if constexpr (Str::Size() >= N)
			return TSTR(detail_get_prefix<N>(Str::View()));
		else
			return Str{};
	}

	template<std::size_t N, typename Str>
	constexpr auto get_suffix(Str = {}) {
		if constexpr (Str::Size() >= N)
			return TSTR(decltype(Str::View()){Str::Data() + Str::Size() - N});
		else
			return Str{};
	}

	// [Left, Right)
	template<std::size_t Idx, std::size_t Cnt, typename Str, typename X>
	constexpr auto replace(Str = {}, X = {}) {
		constexpr auto prefix = remove_suffix<Str::Size() - Idx>(Str{});
		constexpr auto suffix = remove_prefix<Idx + Cnt>(Str{});

		return concat(concat(prefix, X{}), suffix);
	}

	template<typename Str, typename From, typename To>
	constexpr auto replace(Str = {}, From = {}, To = {}) {
		constexpr std::size_t idx = find(Str{}, From{});
		if constexpr (idx != static_cast<std::size_t>(-1))
			return replace(replace<idx, From::Size()>(Str{}, To{}), From{}, To{});
		else
			return Str{};
	}

	template<typename Str, typename X>
	constexpr auto remove(Str = {}, X = {}) {
		return replace(Str{}, X{}, empty_of(Str{}));
	}

	template<std::size_t Idx, std::size_t Cnt, typename Str>
	constexpr auto substr(Str = {}) {
		return get_prefix<Cnt>(remove_prefix<Idx, Str>());
	}

	template<auto V, std::enable_if_t<std::is_integral_v<decltype(V)>, int> = 0>
	constexpr auto int_to_TSTR() {
		if constexpr (V == 0)
			return TStr_of_a<'0'>{};
		else { // not zero
			using T = decltype(V);
			if constexpr (std::is_signed_v<T>) {
				if constexpr (V < 0)
					return concat(TStr_of_a<'-'>{}, int_to_TSTR<static_cast<std::make_unsigned_t<T>>(-V)>());
				else
					return int_to_TSTR<static_cast<std::make_unsigned_t<T>>(V)>();
			}
			else { // unsigned
				if constexpr (V < 10) {
					return TStr_of_a<static_cast<char>('0' + V)>{};
				}
				else
					return concat(int_to_TSTR<V / 10>(), int_to_TSTR<V % 10>());
			}
		}
	}
}

#endif // !UBPA_TSTR_UTIL
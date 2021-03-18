#pragma once

#include "../Name.hpp"

#include <unordered_map>

template<typename Key, typename T>
struct Ubpa::details::custom_type_name<std::unordered_map<Key, T>> {
	static constexpr auto get() noexcept {
		return concat_seq(
			TSTR("std::unordered_map<{"),
			type_name<Key>(),
			TStr_of_a<','>{},
			type_name<T>(),
			TStrC_of<'}', '>'>{}
		);
	}
};

template<typename Key, typename T, typename Hash>
struct Ubpa::details::custom_type_name<std::unordered_map<Key, T, Hash>> {
	static constexpr auto get() noexcept {
		return concat_seq(
			TSTR("std::unordered_map<{"),
			type_name<Key>(),
			TStr_of_a<','>{},
			type_name<T>(),
			TStr_of_a<','>{},
			type_name<Hash>(),
			TStrC_of<'}', '>'>{}
		);
	}
};

template<typename Key, typename T, typename Hash, typename KeyEqual>
struct Ubpa::details::custom_type_name<std::unordered_map<Key, T, Hash, KeyEqual>> {
	static constexpr auto get() noexcept {
		return concat_seq(
			TSTR("std::unordered_map<{"),
			type_name<Key>(),
			TStr_of_a<','>{},
			type_name<T>(),
			TStr_of_a<','>{},
			type_name<Hash>(),
			TStr_of_a<','>{},
			type_name<KeyEqual>(),
			TStrC_of<'}', '>'>{}
		);
	}
};

template<typename Key, typename T, typename Hash>
struct Ubpa::details::custom_type_name<std::pmr::unordered_map<Key, T, Hash>> {
	static constexpr auto get() noexcept {
		return concat_seq(
			TSTR("std::pmr::unordered_map<{"),
			type_name<Key>(),
			TStr_of_a<','>{},
			type_name<T>(),
			TStr_of_a<','>{},
			type_name<Hash>(),
			TStrC_of<'}', '>'>{}
		);
	}
};

template<typename Key, typename T>
struct Ubpa::details::custom_type_name<std::unordered_multimap<Key, T>> {
	static constexpr auto get() noexcept {
		return concat_seq(
			TSTR("std::unordered_multimap<{"),
			type_name<Key>(),
			TStr_of_a<','>{},
			type_name<T>(),
			TStrC_of<'}', '>'>{}
		);
	}
};

template<typename Key, typename T, typename Hash>
struct Ubpa::details::custom_type_name<std::unordered_multimap<Key, T, Hash>> {
	static constexpr auto get() noexcept {
		return concat_seq(
			TSTR("std::unordered_multimap<{"),
			type_name<Key>(),
			TStr_of_a<','>{},
			type_name<T>(),
			TStr_of_a<','>{},
			type_name<Hash>(),
			TStrC_of<'}', '>'>{}
		);
	}
};

template<typename Key, typename T, typename Hash, typename KeyEqual>
struct Ubpa::details::custom_type_name<std::unordered_multimap<Key, T, Hash, KeyEqual>> {
	static constexpr auto get() noexcept {
		return concat_seq(
			TSTR("std::unordered_multimap<{"),
			type_name<Key>(),
			TStr_of_a<','>{},
			type_name<T>(),
			TStr_of_a<','>{},
			type_name<Hash>(),
			TStr_of_a<','>{},
			type_name<KeyEqual>(),
			TStrC_of<'}', '>'>{}
		);
	}
};

template<typename Key, typename T>
struct Ubpa::details::custom_type_name<std::pmr::unordered_multimap<Key, T>> {
	static constexpr auto get() noexcept {
		return concat_seq(
			TSTR("std::pmr::unordered_multimap<{"),
			type_name<Key>(),
			TStr_of_a<','>{},
			type_name<T>(),
			TStrC_of<'}', '>'>{}
		);
	}
};

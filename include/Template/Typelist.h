#pragma once
#ifndef UBPA_TYPELIST_H_
#define UBPA_TYPELIST_H_

#include "Name.h"
#include "List.h"

namespace Ubpa {
	template<typename... Ts>
	struct Typelist { };

	// [ name ]
	template<typename... Ts>
	struct Name<Typelist<Ts...>> {
		friend std::ostream& operator<<(std::ostream& os, Name<Typelist<Ts...>>) {
			os << "[" << Name<Ts...>() << "]";
			return os;
		}
	};

	// [ List ]

	// basic
	template<>
	struct IsEmpty<Typelist<>> {
		static constexpr bool value = true;
	};

	template<typename Head, typename... Tail>
	struct FrontT<Typelist<Head, Tail...>> {
		using type = Head;
	};

	template<typename T, typename... Ts>
	struct PushFrontT<Typelist<Ts...>, T> {
		using type = Typelist<T, Ts...>;
	};
	template<typename Head, typename... Tail>
	struct PopFrontT<Typelist<Head, Tail...>> {
		using type = Typelist<Tail...>;
	};

	// optimized
	template<typename... Ts>
	struct ClearT<Typelist<Ts...>, false> {
		using type = Typelist<>;
	};

	template<template<typename T> class Op, typename... Ts>
	struct TransformT<Typelist<Ts...>, Op> {
		using type = Typelist<typename Op<Ts>::type...>;
	};
}

#endif // !UBPA_TYPELIST_H_

#pragma once

#include "../Util.hpp"

namespace Ubpa::details {
	template<typename List, typename T, std::size_t N = 0, bool found = false>
	struct Find;

	template<typename List, template<typename>class Op, std::size_t N = 0, bool found = false>
	struct FindIf;

	template<typename List, template <typename I, typename X> class Op, typename I, bool = IsEmpty_v<List>>
	struct Accumulate;

	template<typename List, template<typename...>class T, bool found = false, bool = IsEmpty<List>::value>
	struct ExistInstance;

	template<typename List, typename LastT, template<typename...>class T, bool found = false, bool = IsEmpty<List>::value>
	struct SearchInstance;

	template<typename List, bool haveSame = false>
	struct IsUnique;
}

namespace Ubpa {
	template<template<typename...>class OtherListTemplate, typename... Ts>
	struct ToTypeList<OtherListTemplate, OtherListTemplate<Ts...>> : std::type_identity<TypeList<Ts...>> {};

	// =================================================

	template<typename... Ts, template<typename...>class OtherListTemplate>
	struct ToOtherList<TypeList<Ts...>, OtherListTemplate> : std::type_identity<OtherListTemplate<Ts...>> {};

	// =================================================

	template<typename List>
	struct IsTypeList : is_instance_of<List, TypeList> {};

	// =================================================

	template<typename... Ts>
	struct Length<TypeList<Ts...>> : std::integral_constant<std::size_t, sizeof...(Ts)> {};

	// =================================================

	template<typename List>
	struct IsEmpty : std::integral_constant<bool, Length_v<List> == 0> {};

	// =================================================

	template<typename Head, typename... Tail>
	struct Front<TypeList<Head, Tail...>> : std::type_identity<Head> {};

	// =================================================

	template<typename List>
	struct At<List, 0> : std::type_identity<Front_t<List>> {};

	template<typename List, std::size_t N>
	struct At : At<PopFront_t<List>, N - 1> {};

	// =================================================

	template<typename List, std::size_t... Indices>
	struct Select : std::type_identity<TypeList<At_t<List, Indices>...>> {};

	// =================================================

	template<typename List, typename T>
	struct Find : details::Find<List, T> { };

	template<typename List, template<typename>class Op>
	struct FindIf : details::FindIf<List, Op> { };

	// =================================================

	template<typename List, typename T>
	struct Contain : std::integral_constant<bool, Find_v<List, T> != Find_fail> {};

	// =================================================

	template<typename List, typename... Ts>
	struct ContainTs : std::integral_constant<bool, (Contain_v<List, Ts> &&...)> {};

	// =================================================

	template<typename List, typename... Ts>
	struct ContainList<List, TypeList<Ts...>> : std::integral_constant<bool, (Contain_v<List, Ts> &&...)> {};

	// =================================================

	template<template<typename...> class T, typename... Args>
	struct CanInstantiate<TypeList<Args...>, T> : is_instantiable<T, Args...> {};

	// =================================================

	template<template<typename...> class T, typename... Args>
	struct Instantiate<TypeList<Args...>, T> : std::type_identity<T<Args...>> {};

	// =================================================

	template<typename List, template<typename...>class T>
	struct ExistInstance : details::ExistInstance<List, T> {};

	// =================================================

	template<typename List, template<typename...>class T>
	struct SearchInstance : details::SearchInstance<List, void, T> {};

	// =================================================

	template<typename T, typename... Ts>
	struct PushFront<TypeList<Ts...>, T> : std::type_identity<TypeList<T, Ts...>> {};

	// =================================================

	template<typename T, typename... Ts>
	struct PushBack<TypeList<Ts...>, T> : std::type_identity<TypeList<Ts..., T>> {};

	// =================================================

	template<typename Head, typename... Tail>
	struct PopFront<TypeList<Head, Tail...>> : std::type_identity<TypeList<Tail...>> {};

	// =================================================

	template<typename Head, typename... Tail>
	struct Rotate<TypeList<Head, Tail...>> : std::type_identity<TypeList<Tail..., Head>> {};

	// =================================================

	template<typename List, template <typename I, typename X> class Op, typename I>
	struct Accumulate : details::Accumulate<List, Op, I> {};

	// =================================================

	template<typename List, template<typename>class Test>
	struct Filter {
	private:
		template<typename I, typename X>
		struct PushFrontIf : std::conditional<Test<X>::value, PushFront_t<I, X>, I> {};
	public:
		using type = Accumulate_t<List, PushFrontIf, TypeList<>>;
	};

	// =================================================

	template<typename List>
	struct Reverse : Accumulate<List, PushFront, TypeList<>> {};

	// =================================================

	template<typename List, typename T>
	struct PushBack : Reverse<PushFront_t<Reverse_t<List>, T>> {};

	// =================================================

	template<typename List0, typename List1>
	struct Concat : Accumulate<List1, PushBack, List0> {};

	// =================================================

	template<template<typename T> class Op, typename... Ts>
	struct Transform<TypeList<Ts...>, Op> : std::type_identity<TypeList<typename Op<Ts>::type...>> {};

	// =================================================

	template<template<typename X, typename Y>typename Less>
	struct QuickSort<TypeList<>, Less> : std::type_identity<TypeList<>> {};

	template<template<typename X, typename Y>typename Less, typename T>
	struct QuickSort<TypeList<T>, Less> : std::type_identity<TypeList<T>> {};

	template<template<typename X, typename Y>typename Less, typename Head, typename... Tail>
	struct QuickSort<TypeList<Head, Tail...>, Less> {
	private:
		template<typename X>
		using LessThanHead = Less<X, Head>;
		template<typename X>
		using GEThanHead = std::integral_constant<bool, !Less<X, Head>::value>;
		using LessList = Filter_t<TypeList<Tail...>, LessThanHead>;
		using GEList = Filter_t<TypeList<Tail...>, GEThanHead>;
	public:
		using type = Concat_t<
			typename QuickSort<LessList, Less>::type,
			PushFront_t<typename QuickSort<GEList, Less>::type, Head>>;
	};

	// =================================================

	template<typename List>
	struct IsUnique : details::IsUnique<List> {};
}

namespace Ubpa::details {
	template<typename T, std::size_t N, typename... Ts>
	struct Find<TypeList<Ts...>, T, N, true> : std::integral_constant<std::size_t, N - 1> {};
	template<typename T, std::size_t N>
	struct Find<TypeList<>, T, N, false> : std::integral_constant<std::size_t, Find_fail> {};
	template<typename T, typename Head, std::size_t N, typename... Tail>
	struct Find<TypeList<Head, Tail...>, T, N, false>
		: Find<TypeList<Tail...>, T, N + 1, std::is_same_v<T, Head>> {};

	// =================================================

	template<template<typename>class Op, std::size_t N, typename... Ts>
	struct FindIf<TypeList<Ts...>, Op, N, true> : std::integral_constant<std::size_t, N - 1> {};
	template<template<typename>class Op, std::size_t N>
	struct FindIf<TypeList<>, Op, N, false> : std::integral_constant<std::size_t, Find_fail> {};
	template<template<typename>class Op, typename Head, std::size_t N, typename... Tail>
	struct FindIf<TypeList<Head, Tail...>, Op, N, false>
		: FindIf<TypeList<Tail...>, Op, N + 1, Op<Head>::value> {};

	// =================================================

	template<typename List, template <typename I, typename X> class Op, typename I>
	struct Accumulate<List, Op, I, false> : Accumulate<PopFront_t<List>, Op, typename Op<I, Front_t<List>>::type> { };

	template<typename List, template <typename X, typename Y> class Op, typename I>
	struct Accumulate<List, Op, I, true> {
		using type = I;
	};

	// =================================================

	template<typename List, template<typename...>class T>
	struct ExistInstance<List, T, false, true> : std::false_type {};

	template<typename List, template<typename...>class T, bool isEmpty>
	struct ExistInstance<List, T, true, isEmpty> : std::true_type {};

	template<typename List, template<typename...>class T>
	struct ExistInstance<List, T, false, false> : ExistInstance<PopFront_t<List>, T, is_instance_of_v<Front_t<List>, T>> {};

	// =================================================

	template<typename List, typename LastT, template<typename...>class T>
	struct SearchInstance<List, LastT, T, false, true> { }; // no 'type'

	template<typename List, typename LastT, template<typename...>class T, bool isEmpty>
	struct SearchInstance<List, LastT, T, true, isEmpty> {
		using type = LastT;
	};

	template<typename List, typename LastT, template<typename...>class T>
	struct SearchInstance<List, LastT, T, false, false> : SearchInstance<PopFront_t<List>, Front_t<List>, T, is_instance_of_v<Front_t<List>, T>> {};

	// =================================================

	template<typename List>
	struct IsUnique<List, true> : std::false_type {};
	template<>
	struct IsUnique<TypeList<>, false> : std::true_type {};
	template<typename Head, typename... Tail>
	struct IsUnique<TypeList<Head, Tail...>, false> : IsUnique<TypeList<Tail...>, Contain_v<TypeList<Tail...>, Head>> {};
}

#pragma once

#include <cstddef>

namespace Ubpa {
	template<typename... Ts>
	struct TypeList {};

	template<template<typename...>class OtherListTemplate, typename OtherList>
	struct ToTypeList;
	template<template<typename...>class OtherListTemplate, typename OtherList>
	using ToTypeList_t = typename ToTypeList<OtherListTemplate, OtherList>::type;

	template<typename List, template<typename...>class OtherListTemplate>
	struct ToOtherList;
	template<typename List, template<typename...>class OtherListTemplate>
	using ToOtherList_t = typename ToOtherList<List, OtherListTemplate>::type;

	template<typename List> struct IsTypeList;
	template<typename List> constexpr bool IsTypeList_v = IsTypeList<List>::value;

	template<typename List> struct Length;
	template<typename List> constexpr std::size_t Length_v = Length<List>::value;

	template<typename List> struct IsEmpty;
	template<typename List> constexpr bool IsEmpty_v = IsEmpty<List>::value;

	template<typename List> struct Front;
	template<typename List> using Front_t = typename Front<List>::type;

	template<typename List, std::size_t N> struct At;
	template<typename List, std::size_t N> using At_t = typename At<List, N>::type;

	template<typename List, std::size_t... Indices> struct Select;
	template<typename List, std::size_t... Indices> using Select_t = typename Select<List, Indices...>::type;

	constexpr std::size_t Find_fail = static_cast<std::size_t>(-1);

	template<typename List, typename T> struct Find;
	template<typename List, typename T> constexpr std::size_t Find_v = Find<List, T>::value;

	template<typename List, template<typename>class Op> struct FindIf;
	template<typename List, template<typename>class Op> constexpr std::size_t FindIf_v = FindIf<List, Op>::value;

	template<typename List, typename T> struct Contain;
	template<typename List, typename T> constexpr bool Contain_v = Contain<List, T>::value;

	template<typename List, typename... Ts> struct ContainTs;
	template<typename List, typename... Ts> constexpr bool ContainTs_v = ContainTs<List, Ts...>::value;

	template<typename List0, typename List1> struct ContainList;
	template<typename List0, typename List1> constexpr bool ContainList_v = ContainList<List0, List1>::value;

	template<typename List, template<typename...> class T> struct CanInstantiate;
	template<typename List, template<typename...> class T> constexpr bool CanInstantiate_v = CanInstantiate<List, T>::value;

	template<typename List, template<typename...> class T> struct Instantiate;
	template<typename List, template<typename...> class T> using Instantiate_t = typename Instantiate<List, T>::type;

	template<typename List, template<typename...>class T> struct ExistInstance;
	template<typename List, template<typename...>class T> constexpr bool ExistInstance_v = ExistInstance<List, T>::value;

	// get first template instantiable type
	template<typename List, template<typename...>class T> struct SearchInstance;
	template<typename List, template<typename...>class T> using SearchInstance_t = typename SearchInstance<List, T>::type;

	template<typename List, typename T> struct PushFront;
	template<typename List, typename T> using PushFront_t = typename PushFront<List, T>::type;

	template<typename List, typename T> struct PushBack;
	template<typename List, typename T> using PushBack_t = typename PushBack<List, T>::type;

	template<typename List> struct PopFront;
	template<typename List> using PopFront_t = typename PopFront<List>::type;

	template<typename List> struct Rotate;
	template<typename List> using Rotate_t = typename Rotate<List>::type;

	template<typename List, template <typename I, typename X> class Op, typename I>
	struct Accumulate;
	template<typename List, template <typename I, typename X> class Op, typename I>
	using Accumulate_t = typename Accumulate<List, Op, I>::type;

	template<typename List, template<typename>class Op> struct Filter;
	template<typename List, template<typename>class Op> using Filter_t = typename Filter<List, Op>::type;

	template<typename List> struct Reverse;
	template<typename List> using Reverse_t = typename Reverse<List>::type;

	template<typename List0, typename List1> struct Concat;
	template<typename List0, typename List1> using Concat_t = typename Concat<List0, List1>::type;

	template<typename List, template<typename> class Op> struct Transform;
	template<typename List, template<typename> class Op> using Transform_t = typename Transform<List, Op>::type;

	template<typename List, template<typename X, typename Y>typename Less>
	struct QuickSort;
	template<typename List, template<typename X, typename Y>typename Less>
	using QuickSort_t = typename QuickSort<List, Less>::type;

	template<typename List>
	struct IsUnique;
	template<typename List>
	constexpr bool IsUnique_v = IsUnique<List>::value;
}

#include "details/TypeList.inl"

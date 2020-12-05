#pragma once

#include "Basic.h"

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
	template<typename List> constexpr size_t Length_v = Length<List>::value;

	template<typename List> struct IsEmpty;
	template<typename List> constexpr bool IsEmpty_v = IsEmpty<List>::value;

	template<typename List> struct Front;
	template<typename List> using Front_t = typename Front<List>::type;

	template<typename List, size_t N> struct At;
	template<typename List, size_t N> using At_t = typename At<List, N>::type;

	template<typename List, size_t... Indices> struct Select;
	template<typename List, size_t... Indices> using Select_t = typename Select<List, Indices...>::type;

	template<typename List, typename T> struct Find;
	template<typename List, typename T> constexpr size_t Find_v = Find<List, T>::value;
	constexpr size_t Find_fail = static_cast<size_t>(-1);

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
	struct IsSet;
	template<typename List>
	constexpr bool IsSet_v = IsSet<List>::value;
}

namespace Ubpa::detail::TypeList_ {
	template<typename List, typename T, size_t N = 0, bool found = false>
	struct Find;

	template<typename List, template <typename I, typename X> class Op, typename I, bool = IsEmpty_v<List>>
	struct Accumulate;

	template<typename List, template<typename...>class T, bool found = false, bool = IsEmpty<List>::value>
	struct ExistInstance;

	template<typename List, typename LastT, template<typename...>class T, bool found = false, bool = IsEmpty<List>::value>
	struct SearchInstance;

	template<typename List, bool haveSame = false>
	struct IsSet;
}

namespace Ubpa {
	template<template<typename...>class OtherListTemplate, typename... Ts>
	struct ToTypeList<OtherListTemplate, OtherListTemplate<Ts...>> : IType<TypeList<Ts...>> {};

	// =================================================

	template<typename... Ts, template<typename...>class OtherListTemplate>
	struct ToOtherList<TypeList<Ts...>, OtherListTemplate> : IType<OtherListTemplate<Ts...>> {};

	// =================================================

	template<typename List>
	struct IsTypeList : is_instance_of<List, TypeList> {};

	// =================================================

	template<typename... Ts>
	struct Length<TypeList<Ts...>> : IValue<size_t, sizeof...(Ts)> {};

	// =================================================

	template<typename List>
	struct IsEmpty : IValue<bool, Length_v<List> == 0> {};

	// =================================================

	template<typename Head, typename... Tail>
	struct Front<TypeList<Head, Tail...>> : IType<Head> {};

	// =================================================

	template<typename List>
	struct At<List, 0> : IType<Front_t<List>> {};

	template<typename List, size_t N>
	struct At : At<PopFront_t<List>, N - 1> {};

	// =================================================

	template<typename List, size_t... Indices>
	struct Select : IType<TypeList<At_t<List, Indices>...>> {};

	// =================================================

	template<typename List, typename T>
	struct Find : detail::TypeList_::Find<List, T> { };

	// =================================================

	template<typename List, typename T>
	struct Contain : IValue<bool, Find_v<List,T> != Find_fail> {};

	// =================================================

	template<typename List, typename... Ts>
	struct ContainTs : IValue<bool, (Contain_v<List, Ts> &&...)> {};

	// =================================================

	template<typename List, typename... Ts>
	struct ContainList<List, TypeList<Ts...>> : IValue<bool, (Contain_v<List, Ts> &&...)> {};

	// =================================================

	template<template<typename...> class T, typename... Args>
	struct CanInstantiate<TypeList<Args...>, T> : is_instantiable<T, Args...> {};

	// =================================================

	template<template<typename...> class T, typename... Args>
	struct Instantiate<TypeList<Args...>, T> : IType<T<Args...>> {};

	// =================================================

	template<typename List, template<typename...>class T>
	struct ExistInstance : detail::TypeList_::ExistInstance<List, T> {};

	// =================================================

	template<typename List, template<typename...>class T>
	struct SearchInstance : detail::TypeList_::SearchInstance<List, void, T> {};

	// =================================================

	template<typename T, typename... Ts>
	struct PushFront<TypeList<Ts...>, T> : IType<TypeList<T, Ts...>> {};

	// =================================================

	template<typename T, typename... Ts>
	struct PushBack<TypeList<Ts...>, T> : IType<TypeList<Ts..., T>> {};

	// =================================================

	template<typename Head, typename... Tail>
	struct PopFront<TypeList<Head, Tail...>> : IType<TypeList<Tail...>> {};

	// =================================================

	template<typename Head, typename... Tail>
	struct Rotate<TypeList<Head, Tail...>> : IType<TypeList<Tail..., Head>> {};

	// =================================================

	template<typename List, template <typename I, typename X> class Op, typename I>
	struct Accumulate : detail::TypeList_::Accumulate<List, Op, I> {};

	// =================================================

	template<typename List, template<typename>class Test>
	struct Filter : Accumulate<List, AddIf<PushFront, Test>::template Ttype, TypeList<>> {};

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
	struct Transform<TypeList<Ts...>, Op> : IType<TypeList<typename Op<Ts>::type...>> {};
	
	// =================================================

	template<template<typename X, typename Y>typename Less>
	struct QuickSort<TypeList<>, Less> : IType<TypeList<>> {};

	template<template<typename X, typename Y>typename Less, typename T>
	struct QuickSort<TypeList<T>, Less> : IType<TypeList<T>> {};

	template<template<typename X, typename Y>typename Less, typename Head, typename... Tail>
	struct QuickSort<TypeList<Head, Tail...>, Less> {
	private:
		template<typename X>
		using LessThanHead = Less<X, Head>;
		using LessList = Filter_t<TypeList<Tail...>, LessThanHead>;
		using GEList = Filter_t<TypeList<Tail...>, Negate<LessThanHead>::template Ttype>;
	public:
		using type = Concat_t<
			typename QuickSort<LessList, Less>::type,
			PushFront_t<typename QuickSort<GEList, Less>::type, Head>>;
	};

	// =================================================

	template<typename List>
	struct IsSet : detail::TypeList_::IsSet<List> {};
}

namespace Ubpa::detail::TypeList_ {
	template<typename List, typename T, size_t N, bool found>
	struct Find;
	template<typename T, size_t N, typename... Ts>
	struct Find<TypeList<Ts...>, T, N, true> : IValue<size_t, N - 1> {};
	template<typename T, size_t N>
	struct Find<TypeList<>, T, N, false> : IValue<size_t, Find_fail> {};
	template<typename T, typename Head, size_t N, typename... Tail>
	struct Find<TypeList<Head, Tail...>, T, N, false>
		: Find<TypeList<Tail...>, T, N + 1, std::is_same_v<T, Head>> {};

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
	struct IsSet<List, true> : std::false_type {};
	template<>
	struct IsSet<TypeList<>, false> : std::true_type {};
	template<typename Head, typename... Tail>
	struct IsSet<TypeList<Head, Tail...>, false> : IsSet<TypeList<Tail...>, Contain_v<TypeList<Tail...>, Head>> {};
}

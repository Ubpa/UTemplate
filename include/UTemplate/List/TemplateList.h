#pragma once
#ifndef _UBPA_tEMPLATE_LIST_tEMPLATELIST_H_
#define _UBPA_tEMPLATE_LIST_tEMPLATELIST_H_

#include "../List/TypeList.h"
#include "../Num/Bool.h"

namespace Ubpa {
	/* [ Interface ] // T is template
	*  bool TIsEmpty<List>
	*  T    TFront<List>
	*  List TPushFront_t<List, Ts...>
	*  List TPopFront_t<List, T>
	*  T    TAt<List, size>::Ttype
	*  I    TAccumulate_t<List, <I, T> Op, I>
	*  I    TAccumulateIS_t<List, <I, T, size> Op, I, size...>
	*  List TReverse_t<List>
	*  List TPushBack_t<List, T>
	*  List TConcat_t<List0, List1>
	*  List TTransform_t<List, <T> Op>
	*  List TSelect_t<List, size...>
	*  bool TContain_v<List, T>
	*  bool TContainList_v<List0, List1>
	*/

	template<typename ArgList, template<typename...> class T>
	struct TInstance;
	template<typename ArgList, template<typename...> class T>
	using TInstance_t = typename TInstance<ArgList, T>::type;

	template<typename _ArgList, template<typename...> class... Ts>
	struct TemplateList {
		using ArgList = _ArgList;
	private:
		using Valid = TypeList<TInstance_t<ArgList, Ts>...>;
	};

	template<template<typename...> class T, typename... Ts>
	struct TInstance<TypeList<Ts...>, T> {
		using type = T<Ts...>;
	};

	// [ List ]

	// Empty
	template<typename List>
	struct TIsEmpty { static constexpr bool value = false; };

	template<typename ArgList>
	struct TIsEmpty<TemplateList<ArgList>> {
		static constexpr bool value = true;
	};

	template<typename List>
	constexpr bool TIsEmpty_v = TIsEmpty<List>::value;

	// TFront
	template<typename List>
	struct TFront;

	template<typename ArgList, template<typename...> class Head, template<typename...> class... Tail>
	struct TFront<TemplateList<ArgList, Head, Tail...>> {
		template<typename... Ts>
		using Ttype = Head<Ts...>;
	};

	// TPushFront
	template<typename List, template<typename...> class Ts>
	struct TPushFront;

	template<typename ArgList, template<typename...> class T, template<typename...> class... Ts>
	struct TPushFront<TemplateList<ArgList, Ts...>, T> {
		using type = TemplateList<ArgList, T, Ts...>;
	};

	template<typename List, template<typename...> class... Ts>
	using TPushFront_t = typename TPushFront<List, Ts...>::type;

	// TPopFront
	template<typename List>
	struct TPopFront;

	template<typename List>
	using TPopFront_t = typename TPopFront<List>::type;

	template<typename ArgList, template<typename...> class Head, template<typename...> class... Tail>
	struct TPopFront<TemplateList<ArgList, Head, Tail...>> {
		using type = TemplateList<ArgList, Tail...>;
	};

	// TAt
	template<typename List, size_t N>
	struct TAt;

	template<typename List>
	struct TAt<List, 0> {
		template<typename... Ts>
		using Ttype = typename TFront<List>::template Ttype<Ts...>;
	};

	template<typename List, size_t N>
	struct TAt : TAt<TPopFront_t<List>, N - 1> { };

	// TAccumulate : list
	template<typename List, template <typename I, template<typename...> class X> class Op, typename I, bool = TIsEmpty_v<List>>
	struct TAccumulate;

	template<typename List, template <typename I, template<typename...> class X> class Op, typename I>
	struct TAccumulate<List, Op, I, false> : TAccumulate<TPopFront_t<List>, Op, typename Op<I, TFront<List>::template Ttype>::type> { };

	template<typename List, template <typename I, template<typename...> class X> class Op, typename I>
	struct TAccumulate<List, Op, I, true> {
		using type = I;
	};

	template<typename List, template <typename I, template<typename...> class X> class Op, typename I>
	using TAccumulate_t = typename TAccumulate<List, Op, I>::type;

	// TAccumulateIS : TAccumulate by integer sequence
	template<typename List, template<typename I, typename List, size_t Num> class Op, typename I, size_t... Nums>
	struct TAccumulateIS;

	template<typename List, template<typename I, typename List, size_t Num> class Op, typename I>
	struct TAccumulateIS<List, Op, I> {
		using type = I;
	};

	template<typename List, template<typename I, typename List, size_t Num> class Op, typename I,
		size_t NumHead, size_t... NumTail>
		struct TAccumulateIS<List, Op, I, NumHead, NumTail...>
		: TAccumulateIS<List, Op, typename Op<I, List, NumHead>::type, NumTail...> { };

	template<typename List, template<typename I, typename List, size_t Num> class Op, typename I, size_t... Nums>
	using TAccumulateIS_t = typename TAccumulateIS<List, Op, I, Nums...>::type;

	// TReverse
	template<typename List>
	using TReverse = TAccumulate<List, TPushFront, TemplateList<typename List::ArgList>>;

	template<typename List>
	using TReverse_t = typename TReverse<List>::type;

	// TPushBack
	template<typename List, template<typename...> class T>
	using TPushBack = TReverse<TPushFront_t<TReverse_t<List>, T>>;
	template<typename List, template<typename...> class T>
	using TPushBack_t = typename TPushBack<List, T>::type;

	// TConcat
	template<typename List0, typename List1>
	using TConcat = TAccumulate<TReverse_t<List0>, TPushFront, List1>;

	template<typename List0, typename List1>
	using TConcat_t = typename TConcat<List0, List1>::type;

	// TTransform
	template<typename List, template<template<typename...> class T> class Op>
	struct TTransform;

	template<typename ArgList, template<template<typename...> class T> class Op, template<typename...> class... Ts>
	struct TTransform<TemplateList<ArgList, Ts...>, Op> {
		using type = TemplateList<ArgList, Op<Ts>::template Ttype...>;
	};

	template<typename List, template<template<typename...> class T> class Op>
	using TTransform_t = typename TTransform<List, Op>::type;

	// TSelect
	template<typename List, size_t... Indices>
	struct TSelect {
		using type = TemplateList<typename List::ArgList, TAt<List, Indices>::template Ttype...>;
	};

	template<typename List, size_t... Indices>
	using TSelect_t = typename TSelect<List, Indices...>::type;

	// TContain
	template<typename List, template<typename...> class T, bool found = false, bool = TIsEmpty<List>::value>
	struct TContainRec;

	template<typename List, template<typename...> class T>
	struct TContainRec<List, T, false, true> {
		static constexpr bool value = false;
	};

	template<typename List, template<typename...> class T, bool isEmpty>
	struct TContainRec<List, T, true, isEmpty> {
		static constexpr bool value = true;
	};

	template<typename List, template<typename...> class T>
	struct TContainRec<List, T, false, false> :
		TContainRec<TPopFront_t<List>, T,
		std::is_same<
		TInstance_t<typename List::ArgList, TFront<List>::template Ttype>,
		TInstance_t<typename List::ArgList, T>
		>::value> {};

	template<typename List, template<typename...> class T>
	using TContain = TContainRec<List, T>;

	template<typename List, template<typename...> class T>
	constexpr bool TContain_v = TContain<List, T>::value;

	// TContainList
	template<typename List0, typename List1>
	struct TContainList;

	template<typename List, template<typename...> class... Ts>
	struct TContainList<List, TemplateList<typename List::ArgList, Ts...>> : Conjunction<Bool<TContain_v<List, Ts>>...> {};

	template<typename List0, typename List1>
	constexpr bool TContainList_v = TContainList<List0, List1>::type::value;
}

#endif // !_UBPA_tEMPLATE_LIST_tEMPLATELIST_H_

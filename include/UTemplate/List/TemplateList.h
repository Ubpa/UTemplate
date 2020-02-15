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
	*  bool TInstantiable_v<List, T>
	*  bool TCanInstantiateToList_v<List0, List1>
	*/

	template<template<typename...> class... Ts>
	struct TemplateList {};

	// [ List ]

	// Empty
	template<typename List>
	struct TIsEmpty { static constexpr bool value = false; };

	template<>
	struct TIsEmpty<TemplateList<>> {
		static constexpr bool value = true;
	};

	template<typename List>
	constexpr bool TIsEmpty_v = TIsEmpty<List>::value;

	// TFront
	template<typename List>
	struct TFront;

	template<template<typename...> class Head, template<typename...> class... Tail>
	struct TFront<TemplateList<Head, Tail...>> {
		template<typename... Ts>
		using Ttype = Head<Ts...>;
	};

	// TPushFront
	template<typename List, template<typename...> class Ts>
	struct TPushFront;

	template<template<typename...> class T, template<typename...> class... Ts>
	struct TPushFront<TemplateList<Ts...>, T> {
		using type = TemplateList<T, Ts...>;
	};

	template<typename List, template<typename...> class... Ts>
	using TPushFront_t = typename TPushFront<List, Ts...>::type;

	// TPopFront
	template<typename List>
	struct TPopFront;

	template<typename List>
	using TPopFront_t = typename TPopFront<List>::type;

	template<template<typename...> class Head, template<typename...> class... Tail>
	struct TPopFront<TemplateList<Head, Tail...>> {
		using type = TemplateList<Tail...>;
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
	using TReverse = TAccumulate<List, TPushFront, TemplateList<>>;

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

	template<template<template<typename...> class T> class Op, template<typename...> class... Ts>
	struct TTransform<TemplateList<Ts...>, Op> {
		using type = TemplateList<Op<Ts>::template Ttype...>;
	};

	template<typename List, template<template<typename...> class T> class Op>
	using TTransform_t = typename TTransform<List, Op>::type;

	// TSelect
	template<typename List, size_t... Indices>
	struct TSelect {
		using type = TemplateList<TAt<List, Indices>::template Ttype...>;
	};

	template<typename List, size_t... Indices>
	using TSelect_t = typename TSelect<List, Indices...>::type;

	// TInstantiable
	template<typename List, typename Instance, bool found, bool isEmpty = TIsEmpty_v<List>>
	struct TInstantiableRec;

	template<typename List, typename Instance, bool isEmpty>
	struct TInstantiableRec<List, Instance, true, isEmpty> {
		static constexpr bool value = true;
	};

	template<typename List, typename Instance>
	struct TInstantiableRec<List, Instance, false, true> {
		static constexpr bool value = false;
	};

	template<typename List, typename Instance>
	struct TInstantiableRec<List, Instance, false, false> :
		TInstantiableRec<TPopFront_t<List>, Instance, is_instance_of_v<Instance, TFront<List>::template Ttype>> {};

	template<typename List, typename Instance>
	using TInstantiable = TInstantiableRec<List, Instance, false>;

	template<typename List, typename Instance>
	constexpr bool TInstantiable_v = TInstantiable<List, Instance>::value;

	// InstanceList
	template<typename TList, typename ArgList>
	struct TInstanceList;
	template<typename ArgList, template<typename...> class... Ts>
	struct TInstanceList<TemplateList<Ts...>, ArgList> {
		using type = TypeList<Instance_t<ArgList, Ts>...>;
	};
	template<typename TList, typename ArgList>
	using TInstanceList_t = typename TInstanceList<TList, ArgList>::type;

	// TCanInstantiateToList
	template<typename List, typename InstanceList>
	struct TCanInstantiateToList;

	template<typename List, typename... Instances>
	struct TCanInstantiateToList<List, TypeList<Instances...>> : Conjunction<Bool<TInstantiable_v<List, Instances>>...> {};

	template<typename List, typename InstanceList>
	constexpr bool TCanInstantiateToList_v = TCanInstantiateToList<List, InstanceList>::type::value;

	// generalized form List
	template<typename List, typename InstanceList>
	struct TCanGeneralizeFromList;

	template<typename InstanceList, template<typename...>class... Ts>
	struct TCanGeneralizeFromList<TemplateList<Ts...>, InstanceList> : Conjunction<Bool<ExistInstance_v<InstanceList, Ts>>...> {};

	template<typename List, typename InstanceList>
	constexpr bool TCanGeneralizeFromList_v = TCanGeneralizeFromList<List, InstanceList>::type::value;
}

#endif // !_UBPA_tEMPLATE_LIST_tEMPLATELIST_H_

#pragma once

#include "TypeList.hpp"

namespace Ubpa {
	template<template<typename...> class... Ts>
	struct TemplateList { };

	template<typename TList> struct TLength;
	template<typename TList> constexpr std::size_t TLength_v = TLength<TList>::value;

	template<typename TList> struct TIsEmpty;
	template<typename TList> constexpr bool TIsEmpty_v = TIsEmpty<TList>::value;

	// TFront will introduce new template
	// template<typename TList> struct TFront;

	template<typename TList, template<typename...>class T> struct TPushFront;
	template<typename TList, template<typename...>class T> using TPushFront_t = typename TPushFront<TList, T>::type;

	template<typename TList, template<typename...>class T> struct TPushBack;
	template<typename TList, template<typename...>class T> using TPushBack_t = typename TPushBack<TList, T>::type;

	template<typename TList> struct TPopFront;
	template<typename TList> using TPopFront_t = typename TPopFront<TList>::type;

	// TAt will introduce new template
	// template<typename TList, std::size_t N> struct TAt;

	template<typename TList, template<typename...>class T>
	struct TContain;
	template<typename TList, template<typename...>class T>
	static constexpr bool TContain_v = TContain<TList, T>::value;

	template<typename TList, template <typename I, template<typename...> class X> class Op, typename I>
	struct TAccumulate;
	template<typename TList, template <typename I, template<typename...> class X> class Op, typename I>
	using TAccumulate_t = typename TAccumulate<TList, Op, I>::type;

	template<typename TList> struct TReverse;
	template<typename TList> using TReverse_t = typename TReverse<TList>::type;

	template<typename TList0, typename TList1> struct TConcat;
	template<typename TList0, typename TList1> using TConcat_t = typename TConcat<TList0, TList1>::type;

	template<typename TList, template<template<typename...> class T> class Op>
	struct TTransform;

	template<typename TList, template<template<typename...> class T> class Op>
	using TTransform_t = typename TTransform<TList, Op>::type;

	// TSelect will introduce new template
	// template<typename TList, std::size_t... Indices> struct TSelect;

	template<typename TList, typename Instance> struct TExistGenericity;
	template<typename TList, typename Instance> constexpr bool TExistGenericity_v
		= TExistGenericity<TList, Instance>::value;

	template<typename TList, typename InstanceList> struct TExistGenericities;
	template<typename TList, typename InstanceList> constexpr bool TExistGenericities_v
		= TExistGenericities<TList, InstanceList>::value;

	template<typename TList, typename ArgList> struct TInstance;
	template<typename TList, typename ArgList> using TInstance_t = typename TInstance<TList, ArgList>::type;

	template<typename TList, typename InstanceList> struct TCanGeneralizeFromList;
	template<typename TList, typename InstanceList> constexpr bool TCanGeneralizeFromList_v
		= TCanGeneralizeFromList<TList, InstanceList>::value;
}

#include "details/TemplateList.inl"

#pragma once
#ifndef _UBPA_TEMPLATE_SI_H_
#define _UBPA_TEMPLATE_SI_H_

#include "List/TemplateList.h"

namespace Ubpa {
	namespace detail {
		template<template<typename...>class To, typename From>
		struct SI_Cast;
		template<template<typename...>class To, typename From>
		struct SI_Cast<To, const From*> {
			static auto Func(const From* from) {
				return static_cast<const SearchInstance_t<typename From::AllVBs, To>*>(from);
			}
		};
		template<template<typename...>class To, typename From>
		struct SI_Cast<To, From*> {
			static auto Func(From* from) {
				return static_cast<SearchInstance_t<typename From::AllVBs, To>*>(from);
			}
		};

		template<template<typename...>class To, typename From>
		auto SI_CastC(const From* from) {
			return static_cast<const SearchInstance_t<typename From::AllVBs, To>*>(from);
		}

		struct SI_Nil {
			using TVBList = TemplateList<>;
			using AllVBs = TypeList<>;
		};

		template<typename ArgList, template<typename...>class T>
		struct TImpl;

		template<bool CRTP, typename ArgList, template<typename...>class T> struct ActualInstance;
		template<typename ArgList, template<typename...>class T>
		struct ActualInstance<false, ArgList, T> {
			using type = Instance_t<PushFront_t<ArgList, SI_Nil>, T>;
		};
		template<typename ArgList, template<typename...>class T>
		struct ActualInstance<true, ArgList, T> {
			using type = Instance_t<PushFront_t<PushFront_t<ArgList, TImpl<ArgList, T>>, SI_Nil>, T>;
		};
		template<bool CRTP, typename ArgList, template<typename...>class T>
		using ActualInstance_t = typename ActualInstance<CRTP, ArgList, T>::type;

		template<typename BNList, typename Base, typename ArgList>
		struct SI_TNBList;

		template<template<typename, typename...> class T, typename Base, typename ArgList>
		struct SI_TNBList<TemplateList<T>, Base, ArgList> {
			using type = Instance_t<PushFront_t<ArgList, Base>, T>;
			using AllVBs = PushFront_t<typename Base::AllVBs, type>;
		};

		template<typename Base, typename ArgList, template<typename, typename...> class THead,
			template<typename, typename...> class... TTail>
		struct SI_TNBList<TemplateList<THead, TTail...>, Base, ArgList> {
		private:
			using SI_TNBListTTail = SI_TNBList<TemplateList<TTail...>, Base, ArgList>;
		public:
			using type = Instance_t<PushFront_t<ArgList, typename SI_TNBListTTail::type>, THead>;
			using AllVBs = PushFront_t<typename SI_TNBListTTail::AllVBs, type>;
		};

		template<bool CRTP, typename ArgList, typename TNBList>
		struct TVBList_of_TNBList;
		template<bool CRTP, typename ArgList, typename TNBList>
		using TVBList_of_TNBList_t = typename TVBList_of_TNBList<CRTP, ArgList, TNBList>::type;
		template<bool CRTP, typename ArgList>
		struct TVBList_of_TNBList<CRTP, ArgList, TemplateList<>> {
			using type = TemplateList<>;
		};
		template<bool CRTP, typename ArgList,
			template<typename, typename...> class THead,
			template<typename, typename...> class... TTail>
		struct TVBList_of_TNBList<CRTP, ArgList, TemplateList<THead, TTail...>> {
			using type = TConcatR_t<TVBList_of_TNBList_t<CRTP, ArgList, TemplateList<TTail...>>,
				typename ActualInstance_t<CRTP, ArgList, THead>::TVBList>; // set
		};

		template<typename TVBList, typename TNBList, typename Base, typename... Args>
		struct SI;

		template<typename _TVBList, typename TNBList, typename Base, typename... Args>
		struct SI : SI_TNBList<TNBList, Base, TypeList<Args...>>::type {
			using SI_TNBList<TNBList, Base, TypeList<Args...>>::type::type;
			using TVBList = TConcatR_t<TVBList_of_TNBList_t<false, TypeList<Args...>, TNBList>, _TVBList>;
			using AllVBs = typename SI_TNBList<TNBList, Base, TypeList<Args...>>::AllVBs;
		};
		template<typename _TVBList, typename Base, typename... Args>
		struct SI<_TVBList, TemplateList<>, Base, Args...> : Base {
			using Base::Base;
			using TVBList = _TVBList;
			using AllVBs = PushFront_t<typename Base::AllVBs, Base>;
		};
		template<typename _TVBList, typename... Args>
		struct SI<_TVBList, TemplateList<>, SI_Nil, Args...> {
			using TVBList = _TVBList;
			using AllVBs = TypeList<>;
		};

		template<typename TVBList, typename TNBList, typename Base, typename Impl, typename... Args>
		struct SI_CRTP;

		template<typename _TVBList, typename Base, typename Impl, typename... Args>
		struct SI_CRTP<_TVBList, TemplateList<>, Base, Impl, Args...> : Base {
			using Base::Base;
			using TVBList = _TVBList;
			using AllVBs = PushFront_t<typename Base::AllVBs, Base>;
		};

		template<typename _TVBList, typename Impl, typename... Args>
		struct SI_CRTP<_TVBList, TemplateList<>, SI_Nil, Impl, Args...> {
			using TVBList = _TVBList;
			using AllVBs = TypeList<>;
		};

		template<typename _TVBList, typename TNBList, typename Base, typename Impl, typename... Args>
		struct SI_CRTP : SI_TNBList<TNBList, Base, TypeList<Impl, Args...>>::type {
			using SI_TNBList<TNBList, Base, TypeList<Impl, Args...>>::type::type;
			using TVBList = TConcatR_t<TVBList_of_TNBList_t<true, TypeList<Args...>, TNBList>, _TVBList>;
			using AllVBs = typename SI_TNBList<TNBList, Base, TypeList<Impl, Args...>>::AllVBs;
		};

		template<typename BaseList, typename TopoOrderBaseSet, typename ArgList, bool CRTP>
		struct TopoSort;

		template<typename TopoOrderBaseSet, typename ArgList, bool CRTP>
		struct TopoSort<TemplateList<>, TopoOrderBaseSet, ArgList, CRTP> {
			using type = TopoOrderBaseSet;
		};

		template<typename TopoOrderBaseSet, typename ArgList, bool CRTP,
			template<typename...>class THead,
			template<typename...>class... TTail>
		struct TopoSort<TemplateList<THead, TTail...>, TopoOrderBaseSet, ArgList, CRTP> {
		private:
			using BaseList = TemplateList<THead, TTail...>;
			using HeadInstance = ActualInstance_t<CRTP, ArgList, THead>;
			static constexpr bool isContainHead = TInstantiable_v<TopoOrderBaseSet, HeadInstance>;

			template<bool> struct Rec;
			template<>
			struct Rec<true> { using type = TopoOrderBaseSet; };
			template<>
			struct Rec<false> {
				using TVBList = typename HeadInstance::TVBList;
				using NewTopoOrderBaseSet = typename TopoSort<TVBList, TopoOrderBaseSet, ArgList, CRTP>::type;
				using type = TPushFront_t<NewTopoOrderBaseSet, THead>;
			};

		public:
			using type = typename TopoSort<TemplateList<TTail...>, typename Rec<isContainHead>::type, ArgList, CRTP>::type;
		};
		template<typename BaseList, typename ArgList, bool CRTP>
		using TopoSort_t = typename detail::TopoSort<BaseList, TemplateList<>, ArgList, CRTP>::type;
		template<typename BaseList, typename... Args>
		using SIIT = SI<TemplateList<>, BaseList, SI_Nil, Args...>;
		template<template<typename, typename...> class... Bases>
		using SII = SIIT<TemplateList<Bases...>>;
		template<typename BaseList, typename Impl, typename... Args>
		using SII_CRTP = SI_CRTP<TemplateList<>, BaseList, SI_Nil, Impl, Args...>;
	}

	template<typename TVBList, typename TNBList, typename Base, typename... Args>
	using SI = detail::SI<TVBList, TNBList, Base, Args...>;
	template<typename TVBList, typename Base, typename... Args>
	using SIVT = detail::SI<TVBList, TemplateList<>, Base, Args...>;
	template<typename Base, template<typename...>class... VBs>
	using SIV = SIVT<TemplateList<VBs...>, Base>;
	template<typename TNBList, typename Base, typename... Args>
	using SINT = detail::SI<TemplateList<>, TNBList, Base, Args...>;
	template<typename Base, template<typename...>class... NBs>
	using SIN = SINT<TemplateList<NBs...>, Base>;

	template<typename TVBList, typename TNBList, typename Base, typename Impl, typename... Args>
	using SI_CRTP = detail::SI_CRTP<TVBList, TNBList, Base, Impl, Args...>;
	template<typename TVBList, typename Base, typename Impl, typename... Args>
	using SIVT_CRTP = detail::SI_CRTP<TVBList, TemplateList<>, Base, Impl, Args...>;
	template<typename Base, typename Impl, template<typename...>class... VBs>
	using SIV_CRTP = SIVT_CRTP<TemplateList<VBs...>, Base, Impl>;
	template<typename TNBList, typename Base, typename Impl, typename... Args>
	using SINT_CRTP = detail::SI_CRTP<TemplateList<>, TNBList, Base, Impl, Args...>;
	template<typename Base, typename Impl, template<typename...>class... NBs>
	using SIN_CRTP = SINT_CRTP<TemplateList<NBs...>, Base, Impl>;

	// implementation
	template<typename BaseList, typename... Args>
	using SIIT = detail::SIIT<detail::TopoSort_t<BaseList, TypeList<Args...>, false>, Args...>;
	template<template<typename...>class... Bases>
	using SII = SIIT<TemplateList<Bases...>>;

	template<typename BaseList, typename Impl, typename... Args>
	using SIIT_CRTP = detail::SII_CRTP<detail::TopoSort_t<BaseList, TypeList<Args...>, true>, Impl, Args...>;
	template<typename Impl, template<typename...>class... Bases>
	using SII_CRTP = SIIT_CRTP<TemplateList<Bases...>, Impl>;

	template<template<typename...>class To, typename From>
	auto SI_Cast(From from) {
		return detail::SI_Cast<To, From>::Func(from);
	}
}

#endif // !_UBPA_TEMPLATE_SI_H_

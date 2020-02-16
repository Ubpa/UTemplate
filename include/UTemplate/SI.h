#pragma once
#ifndef _UBPA_TEMPLATE_SI_H_
#define _UBPA_TEMPLATE_SI_H_

#include "List/TemplateList.h"

namespace Ubpa {
	struct SI_Nil {
		using TVBs = TemplateList<>;
		using AllVBs = TypeList<>;
	};

	template<typename Base, typename... Ts>
	struct SI_TNil : Base {
		using Base::Base;

		using TVBs = TemplateList<>;
		using AllVBs = TypeList<>;
	};

	template<typename... Ts>
	struct SI_TNil<SI_Nil, Ts...> {
		using TVBs = TemplateList<>;
		using AllVBs = TypeList<>;
	};

	template<typename TNBList>
	struct SI_TNBList;

	template<>
	struct SI_TNBList<TemplateList<>> {
		template<typename Base, typename... Ts>
		using Ttype = SI_TNil<Base, Ts...>;
	};

	template<template<typename, typename...> class THead,
		template<typename, typename...> class... TTail>
	struct SI_TNBList<TemplateList<THead, TTail...>> {
		template<typename Base, typename... Ts>
		struct Ttype : THead<typename SI_TNBList<TemplateList<TTail...>>::template Ttype<Base, Ts...>, Ts...> {
		private:
			using B = THead<typename SI_TNBList<TemplateList<TTail...>>::template Ttype<Base, Ts...>, Ts...>;
			using BB = typename SI_TNBList<TemplateList<TTail...>>::template Ttype<Base, Ts...>;

		public:
			using THead<typename SI_TNBList<TemplateList<TTail...>>::template Ttype<Base, Ts...>, Ts...>::THead;
			using AllVBs = std::enable_if_t<
				TCanGeneralizeFromList_v<typename B::TVBs, typename BB::AllVBs>,
				typename BB::AllVBs>;
			using TVBs = TConcatR_t<typename BB::TVBs, typename B::TVBs>;
		};
	};

	template<typename TVBList, typename TNBList = TemplateList<>>
	struct SI;

	template<typename TVBList>
	struct SI<TVBList, TemplateList<>> {
		template<typename Base, typename... Ts>
		struct Ttype : Base {
			using Base::Base;
			using TVBs = TVBList;
		};

		template<typename... Ts>
		struct Ttype<SI_Nil, Ts...> {
			using TVBs = TVBList;
		};
	};

	template<typename TVBList, typename TNBList>
	struct SI {
		template<typename Base, typename... Ts>
		struct Ttype : SI_TNBList<TNBList>::template Ttype<Base, Ts...> {
			using Ttype::Ttype;
			using TVBs = TVBList;
		};
	};

	template<typename Base, template<typename...>class... TNBases>
	using SIN = typename SI<TemplateList<>, TemplateList<TNBases...>>::template Ttype<Base>;

	template<typename Base, typename ArgList, template<typename...>class... TNBases>
	using SINT = Instance_t<PushFront_t<ArgList, Base>, SI<TemplateList<>, TemplateList<TNBases...>>::template Ttype>;

	template<typename Base, typename ArgList, typename... CRTPWrapper>
	using SINT_CRTP = SINT<Base, ArgList, CRTPWrapper::template Ttype...>;

	template<typename Base, template<typename...>class... TVBases>
	using SIV = typename SI<TemplateList<TVBases...>>::template Ttype<Base>;

	template<typename Base, typename ArgList, template<typename...>class... TVBases>
	using SIVT = Instance_t<PushFront_t<ArgList, Base>, SI<TemplateList<TVBases...>, TemplateList<>>::template Ttype>;
	
	template<typename Base, typename ArgList, typename... CRTPWrapper>
	using SIVT_CRTP = SIVT<Base, ArgList, CRTPWrapper::template Ttype...>;

	//---------------------------------------------------------------------------------------

	namespace detail {
		template<typename TopoOrderBaseSet>
		struct SII;
		template<>
		struct SII<TemplateList<>> {
			template<typename... Ts>
			using Ttype = SI_Nil;
		};

		template<template<typename, typename...> class THead,
			template<typename, typename...> class... TTail>
		struct SII<TemplateList<THead, TTail...>> {
			template<typename... Ts>
			struct Ttype : THead<typename SII<TemplateList<TTail...>>::template Ttype<Ts...>, Ts...> {
			private:
				using B = THead<typename SII<TemplateList<TTail...>>::template Ttype<Ts...>, Ts...>;
				using BB = typename SII<TemplateList<TTail...>>::template Ttype<Ts...>;

			public:
				using THead<typename SII<TemplateList<TTail...>>::template Ttype<Ts...>, Ts...>::THead;
				using AllVBs = std::enable_if_t<
					TCanGeneralizeFromList_v<typename B::TVBs, typename BB::AllVBs>,
					PushFront_t<typename BB::AllVBs, B>>;
			};
		};
	}

	namespace detail {
		template<typename BaseList, typename TopoOrderBaseSet, typename ArgList>
		struct TopoSort;

		template<typename TopoOrderBaseSet, typename ArgList>
		struct TopoSort<TemplateList<>, TopoOrderBaseSet, ArgList> {
			using type = TopoOrderBaseSet;
		};

		template<typename TopoOrderBaseSet, typename ArgList,
			template<typename...>class THead,
			template<typename...>class... TTail>
		struct TopoSort<TemplateList<THead, TTail...>, TopoOrderBaseSet, ArgList> {
		private:
			using BaseList = TemplateList<THead, TTail...>;
			static constexpr bool isContainHead = TInstantiable_v<TopoOrderBaseSet, Instance_t<ArgList, THead>>;
			template<bool> struct Rec;
			template<>
			struct Rec<true> { using type = TopoOrderBaseSet; };
			template<>
			struct Rec<false> {
				using TVBs = typename Instance_t<ArgList, THead>::TVBs;
				using NewTopoOrderBaseSet = typename TopoSort<TVBs, TopoOrderBaseSet, ArgList>::type;
				using type = TPushFront_t<NewTopoOrderBaseSet, THead>;
			};
		public:
			using type = typename TopoSort<TemplateList<TTail...>, typename Rec<isContainHead>::type, ArgList>::type;
		};
	}

	template<typename BaseList, typename ArgList>
	using TopoSort_t = typename detail::TopoSort<BaseList, TemplateList<>, ArgList>::type;

	template<typename TopoOrderBaseSet, typename... Args>
	using SIIPro = typename detail::SII<TopoOrderBaseSet>::template Ttype<Args...>;

	template<typename BaseList, typename... Args>
	using SIIT = SIIPro<TopoSort_t<BaseList, TypeList<SI_Nil, Args...>>, Args...>;

	template<template<typename...>class... Bases>
	using SII = SIIT<TemplateList<Bases...>>;

	template<template<typename...>class To, typename From>
	SearchInstance_t<typename From::AllVBs, To>* SI_Cast(From* from) {
		return static_cast<SearchInstance_t<typename From::AllVBs, To>*>(from);
	}
	template<template<typename...>class To, typename From>
	const SearchInstance_t<typename From::AllVBs, To>* SI_CastC(const From* from) {
		return static_cast<const SearchInstance_t<typename From::AllVBs, To>*>(from);
	}
}

#endif // !_UBPA_TEMPLATE_SI_H_

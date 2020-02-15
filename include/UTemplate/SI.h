#pragma once
#ifndef _UBPA_TEMPLATE_SI_H_
#define _UBPA_TEMPLATE_SI_H_

#include "List/TemplateList.h"

namespace Ubpa {
	struct SI_Nil {
		using VBs = TemplateList<>;
		using AllVBs = TemplateList<>;
	};
	template<typename Base, typename... Ts>
	struct SI_TNil : Base {
		using VBs = TemplateList<>;
		using AllVBs = TemplateList<>;
	};
	template<typename... Ts>
	struct SI_TNil<SI_Nil, Ts...> {
		using VBs = TemplateList<>;
		using AllVBs = TemplateList<>;
	};

	template<typename NBList>
	struct SI_NBList;

	template<>
	struct SI_NBList<TemplateList<>> {
		template<typename Base, typename... Ts>
		using Ttype = SI_TNil<Base, Ts...>;
	};

	template<template<typename, typename...> class THead,
		template<typename, typename...> class... TTail>
	struct SI_NBList<TemplateList<THead, TTail...>> {
		template<typename Base, typename... Ts>
		struct Ttype : THead<typename SI_NBList<TemplateList<TTail...>>::template Ttype<Base, Ts...>, Ts...> {
		
		private:
			using B = THead<typename SI_NBList<TemplateList<TTail...>>::template Ttype<Base, Ts...>, Ts...>;
			using BB = typename SI_NBList<TemplateList<TTail...>>::template Ttype<Base, Ts...>;

			template<typename B, typename BB>
			struct check {
				using type = typename std::enable_if<
					TInstantiableList_v<typename BB::AllVBs, TInstanceList_t<typename B::VBs, TypeList<SI_Nil, Ts...>>>,
					typename BB::AllVBs
				>::type;
			};
		
		public:
			/*using AllVBs = typename std::enable_if<
				TInstantiableList_v<typename BB::AllVBs, TInstanceList_t<typename B::VBs, TypeList<SI_Nil, Ts...>>>,
				typename BB::AllVBs
			>::type;*/
			using AllVBs = typename check<B, BB>::type;
		};
	};
	
	template<typename VBList, typename NBList = TemplateList<>>
	struct SI;

	template<typename VBList>
	struct SI<VBList, TemplateList<>> {
		template<typename Base, typename... Ts>
		struct Ttype : Base {
			using VBs = VBList;
		};

		template<typename... Ts>
		struct Ttype<SI_Nil, Ts...> {
			using VBs = VBList;
		};
	};

	/*template<>
	struct SI<TemplateList<>, TemplateList<>> {
		template<typename Base, typename... Ts>
		using Ttype = Base;
	};*/

	template<typename VBList, typename NBList>
	struct SI {
		template<typename Base, typename... Ts>
		struct Ttype : SI_NBList<NBList>::template Ttype<Base, Ts...> {
			using VBs = VBList;
		};
	};

	template<typename BList>
	struct SII;
	template<>
	struct SII<TemplateList<>> {
		template<typename Base, typename... Ts>
		struct Ttype : Base {};
	};

	template<template<typename, typename...> class THead,
		template<typename, typename...> class... TTail>
	struct SII<TemplateList<THead, TTail...>> {
		template<typename Base, typename... Ts>
		struct Ttype : THead<typename SII<TemplateList<TTail...>>::template Ttype<Base, Ts...>, Ts...> {

		private:
			using B = THead<typename SII<TemplateList<TTail...>>::template Ttype<Base, Ts...>, Ts...>;
			using BB = typename SII<TemplateList<TTail...>>::template Ttype<Base, Ts...>;

			template<typename B, typename BB>
			struct check {
				using type = typename std::enable_if <
					TInstantiableList_v<typename BB::AllVBs, TInstanceList_t<typename B::VBs, TypeList<SI_Nil, Ts...>>>,
					TPushFront_t<typename BB::AllVBs, THead>
				>::type;
			};

		public:
			/*using AllVBs = typename std::enable_if<
				TInstantiableList_v<typename BB::AllVBs, TInstanceList_t<typename B::VBs, TypeList<SI_Nil, Ts...>>>,
				typename BB::AllVBs
			>::type;*/
			using AllVBs = typename check<B, BB>::type;
		};
	};

}

#endif // !_UBPA_TEMPLATE_SI_H_

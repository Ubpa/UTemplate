#pragma once

#include "TemplateList.h"

namespace Ubpa::detail::SI_ {
	struct Nil {
		template<typename SI_ERROR, typename = typename SI_ERROR::SI_ERROR>
		void operator+(SI_ERROR) = delete;
		template<typename SI_ERROR, typename = typename SI_ERROR::SI_ERROR>
		void operator-(SI_ERROR) = delete;
		template<typename SI_ERROR, typename = typename SI_ERROR::SI_ERROR>
		void operator*(SI_ERROR) = delete;
		template<typename SI_ERROR, typename = typename SI_ERROR::SI_ERROR>
		void operator/(SI_ERROR) = delete;
		template<typename SI_ERROR, typename = typename SI_ERROR::SI_ERROR>
		void operator^(SI_ERROR) = delete;
		template<typename SI_ERROR, typename = typename SI_ERROR::SI_ERROR>
		void operator&(SI_ERROR) = delete;
		template<typename SI_ERROR, typename = typename SI_ERROR::SI_ERROR>
		void operator|(SI_ERROR) = delete;
		/*template<typename SI_ERROR, typename = typename SI_ERROR::SI_ERROR>
		void operator~() = delete;
		template<typename SI_ERROR, typename = typename SI_ERROR::SI_ERROR>
		void operator!() = delete;*/
		template<typename SI_ERROR, typename = typename SI_ERROR::SI_ERROR>
		void operator=(SI_ERROR) = delete;
		template<typename SI_ERROR, typename = typename SI_ERROR::SI_ERROR>
		void operator<(SI_ERROR) = delete;
		template<typename SI_ERROR, typename = typename SI_ERROR::SI_ERROR>
		void operator>(SI_ERROR) = delete;
		template<typename SI_ERROR, typename = typename SI_ERROR::SI_ERROR>
		void operator+=(SI_ERROR) = delete;
		template<typename SI_ERROR, typename = typename SI_ERROR::SI_ERROR>
		void operator-=(SI_ERROR) = delete;
		template<typename SI_ERROR, typename = typename SI_ERROR::SI_ERROR>
		void operator*=(SI_ERROR) = delete;
		template<typename SI_ERROR, typename = typename SI_ERROR::SI_ERROR>
		void operator/=(SI_ERROR) = delete;
		template<typename SI_ERROR, typename = typename SI_ERROR::SI_ERROR>
		void operator%=(SI_ERROR) = delete;
		template<typename SI_ERROR, typename = typename SI_ERROR::SI_ERROR>
		void operator^=(SI_ERROR) = delete;
		template<typename SI_ERROR, typename = typename SI_ERROR::SI_ERROR>
		void operator&=(SI_ERROR) = delete;
		template<typename SI_ERROR, typename = typename SI_ERROR::SI_ERROR>
		void operator|=(SI_ERROR) = delete;
		template<typename SI_ERROR, typename = typename SI_ERROR::SI_ERROR>
		void operator<<(SI_ERROR) = delete;
		template<typename SI_ERROR, typename = typename SI_ERROR::SI_ERROR>
		void operator>>(SI_ERROR) = delete;
		template<typename SI_ERROR, typename = typename SI_ERROR::SI_ERROR>
		void operator>>=(SI_ERROR) = delete;
		template<typename SI_ERROR, typename = typename SI_ERROR::SI_ERROR>
		void operator<<=(SI_ERROR) = delete;
		template<typename SI_ERROR, typename = typename SI_ERROR::SI_ERROR>
		void operator==(SI_ERROR) = delete;
		template<typename SI_ERROR, typename = typename SI_ERROR::SI_ERROR>
		void operator!=(SI_ERROR) = delete;
		template<typename SI_ERROR, typename = typename SI_ERROR::SI_ERROR>
		void operator<=(SI_ERROR) = delete;
		template<typename SI_ERROR, typename = typename SI_ERROR::SI_ERROR>
		void operator>=(SI_ERROR) = delete;
		template<typename SI_ERROR, typename = typename SI_ERROR::SI_ERROR>
		void operator&&(SI_ERROR) = delete;
		template<typename SI_ERROR, typename = typename SI_ERROR::SI_ERROR>
		void operator||(SI_ERROR) = delete;
		template<typename SI_ERROR, typename = typename SI_ERROR::SI_ERROR>
		void operator++(SI_ERROR) = delete;
		template<typename SI_ERROR, typename = typename SI_ERROR::SI_ERROR>
		void operator--(SI_ERROR) = delete;
		template<typename SI_ERROR, typename = typename SI_ERROR::SI_ERROR>
		void operator,(SI_ERROR) = delete;
		template<typename SI_ERROR, typename = typename SI_ERROR::SI_ERROR>
		void operator->*(SI_ERROR) = delete;
		/*template<typename SI_ERROR, typename = typename SI_ERROR::SI_ERROR>
		void operator->() = delete;*/
		template<typename SI_ERROR, typename = typename SI_ERROR::SI_ERROR>
		void operator()(SI_ERROR) = delete;
		template<typename SI_ERROR, typename = typename SI_ERROR::SI_ERROR>
		void operator[](SI_ERROR) = delete;

		using TVBList = TemplateList<>;
		using AllVBs = TypeList<>;
	};

	template<template<typename...>class Interface, typename Impl, typename ArgList>
	using ITraits_IList = typename Instantiate_t<TypeList<Nil, Impl, ArgList>, Interface>::IList;

	template<typename Void,
		template<template<typename...>class, typename, typename>class Trait,
		template<typename...>class Interface, typename Impl, typename ArgList>
	struct ITraits_Have_Helper : std::false_type {};

	template<template<template<typename...>class, typename, typename>class Trait,
		template<typename...>class Interface, typename Impl, typename ArgList>
	struct ITraits_Have_Helper<std::void_t<Trait<Interface, Impl, ArgList>>, Trait, Interface, Impl, ArgList> : std::true_type {};

	template<template<template<typename...>class, typename, typename>class Trait,
		template<typename...>class Interface, typename Impl, typename ArgList>
	static constexpr bool ITraits_Have = ITraits_Have_Helper<void, Trait, Interface, Impl, ArgList>::value;

	template<template<typename...>class Interface, typename Impl, typename ArgList>
	static constexpr bool ITraits_Have_IList = ITraits_Have<ITraits_IList, Interface, Impl, ArgList>;

	template<bool haveIList, template<typename...>class Interface, typename Impl, typename ArgList>
	struct ITraits_Get_IList;
	template<template<typename...>class Interface, typename Impl, typename ArgList>
	struct ITraits_Get_IList<false, Interface, Impl, ArgList> : IType<TemplateList<>> {};
	template<template<typename...>class Interface, typename Impl, typename ArgList>
	struct ITraits_Get_IList<true, Interface, Impl, ArgList> : IType<ITraits_IList<Interface, Impl, ArgList>> {};

	template<typename IList, typename Impl, typename ArgList>
	struct SI;

	template<typename IList, typename Impl, typename ArgList>
	using SI_t = typename SI<IList, Impl, ArgList>::type;

	template<typename Impl, typename ArgList>
	struct SI<TemplateList<>, Impl, ArgList> {
		using type = Nil;
	};

	template<template<typename...>class IHead, template<typename...>class... ITail,
		typename Impl, typename ArgList>
	struct SI<TemplateList<IHead, ITail...>, Impl, ArgList> {
		using type = IHead<SI_t<TemplateList<ITail...>, Impl, ArgList>, Impl, ArgList>;
	};

	template<typename ArgList, template<typename...>class T>
	struct TImpl;

	template<typename IList, typename SortedIList, typename ArgList>
	struct TopoSort;

	template<typename SortedIList, typename ArgList>
	struct TopoSort<TemplateList<>, SortedIList, ArgList>
		: IType<SortedIList> {};
	
	template<template<typename...>class IHead,
		template<typename...>class... ITail,
		typename SortedIList, typename ArgList>
	struct TopoSort<TemplateList<IHead, ITail...>, SortedIList, ArgList> {
		template<bool is_contain> struct Rec;
		template<>
		struct Rec<true> : IType<SortedIList> {};
		template<> struct Rec<false> {
			using HeadChildren = typename ITraits_Get_IList<ITraits_Have_IList<IHead, TImpl<ArgList, IHead>, ArgList>, IHead, TImpl<ArgList, IHead>, ArgList>::type;
			using NewSortedIList = typename TopoSort<HeadChildren, SortedIList, ArgList>::type;
			using type = TPushFront_t<NewSortedIList, IHead>;
		};
		static constexpr bool is_coantain_head = TExistGenericity_v<SortedIList, Instantiate_t<TypeList<Nil, TImpl<ArgList, IHead>, ArgList>, IHead>>;
		using type = typename TopoSort<TemplateList<ITail...>,
			typename Rec<is_coantain_head>::type, ArgList>::type;
	};

	template<typename IList, typename ArgList>
	using TopoSort_t = typename TopoSort<IList, TemplateList<>, ArgList>::type;
}

namespace Ubpa {
	template<typename IList, typename Impl, typename... Args>
	struct SI;

	template<typename IList_, typename Impl, typename... Args>
	struct SI : detail::SI_::SI_t<detail::SI_::TopoSort_t<IList_, TypeList<Args...>>, Impl, TypeList<Args...>> {
		using ArgList = TypeList<Args...>;
		using IList = detail::SI_::TopoSort_t<IList_, ArgList>;
		using Base = detail::SI_::SI_t<IList, Impl, ArgList>;

		using Base::Base;

		template<template<typename...>class Interface>
		static constexpr bool IsContain() noexcept {
			return TExistGenericity_v<IList, Instantiate_t<TypeList<detail::SI_::Nil, Impl, ArgList>, Interface>>;
		}
	};
}

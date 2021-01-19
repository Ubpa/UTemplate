#pragma once

namespace Ubpa::details {
	//
	// SI_ImplTraits_IList
	////////////////////////

	template<typename Enabler, typename Impl>
	struct SI_ImplTraits_IList_Helper : std::type_identity<TemplateList<>> {};

	template<typename Impl>
	struct SI_ImplTraits_IList_Helper<std::void_t<typename SI_ImplTraits<Impl>::IList>, Impl>
		: std::type_identity<typename SI_ImplTraits<Impl>::IList> {};

	template<typename Impl> struct SI_ImplTraits_IList : SI_ImplTraits_IList_Helper<void, Impl> {};
	template<typename Impl> using SI_ImplTraits_IList_t = typename SI_ImplTraits_IList<Impl>::type;

	//
	// SI_InterfaceTraits_IList
	/////////////////////////////

	template<typename Void, template<typename Base, typename Impl>class Interface>
	struct SI_InterfaceTraits_IList_Helper : std::type_identity<TemplateList<>> {};

	template<template<typename Base, typename Impl>class Interface>
	struct SI_InterfaceTraits_IList_Helper<std::void_t<typename SI_InterfaceTraits<Interface>::IList>, Interface>
		: std::type_identity<typename SI_InterfaceTraits<Interface>::IList> {};

	template<template<typename Base, typename Impl>class Interface>
	struct SI_InterfaceTraits_IList : SI_InterfaceTraits_IList_Helper<void, Interface> {};
	template<template<typename Base, typename Impl>class Interface>
	using SI_InterfaceTraits_IList_t = typename SI_InterfaceTraits_IList<Interface>::type;

	//
	// ITopoSort
	//////////////

	template<typename IList, typename SortedIList>
	struct ITopoSort_Helper;
	template<typename IList, typename SortedIList>
	using ITopoSort_Helper_t = typename ITopoSort_Helper<IList, SortedIList>::type;

	template<typename SortedIList>
	struct ITopoSort_Helper<TemplateList<>, SortedIList>
		: std::type_identity<SortedIList> {};

	template<bool NeedRecuresion, typename SortedIList, template<typename Base, typename Impl>class IHead>
	struct ITopoSort_Recursion;
	template<typename SortedIList, template<typename Base, typename Impl>class IHead>
	struct ITopoSort_Recursion<true, SortedIList, IHead> : std::type_identity<SortedIList> {};
	template<typename SortedIList, template<typename Base, typename Impl>class IHead>
	struct ITopoSort_Recursion<false, SortedIList, IHead>
		: std::type_identity<TPushFront_t<ITopoSort_Helper_t<SI_InterfaceTraits_IList_t<IHead>, SortedIList>, IHead>> {};

	template<
		template<typename Base, typename Impl>class IHead,
		template<typename Base, typename Impl>class... ITail,
		typename SortedIList
	>
	struct ITopoSort_Helper<TemplateList<IHead, ITail...>, SortedIList> {
		using type = ITopoSort_Helper_t<TemplateList<ITail...>, typename ITopoSort_Recursion<TContain_v<SortedIList, IHead>, SortedIList, IHead>::type>;
	};


	template<typename IList> struct ITopoSort : ITopoSort_Helper<IList, TemplateList<>> {};
	template<typename IList> using ITopoSort_t = typename ITopoSort<IList>::type;

	//
	// Nil
	////////

	template<typename Impl>
	struct Nil {
		// for using Base::operatorXX in Interface

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
	};

	//
	// SI
	///////

	template<typename IList, typename Impl> struct SI_Helper;
	template<typename IList, typename Impl> using SI_Helper_t = typename SI_Helper<IList, Impl>::type;

	template<typename Impl>
	struct SI_Helper<TemplateList<>, Impl> : std::type_identity<Nil<Impl>> {};

	template<
		template<typename Base, typename Impl>class IHead,
		template<typename Base, typename Impl>class... ITail,
		typename Impl
	>
	struct SI_Helper<TemplateList<IHead, ITail...>, Impl>
		: std::type_identity<IHead<SI_Helper_t<TemplateList<ITail...>, Impl>, Impl>> {};

	//
	// SI_Contains
	////////////////

	template<typename Void, typename T, template<typename Base, typename Impl>class Interface>
	struct SI_Contains_Helper : std::false_type {};
	template<typename T, template<typename Base, typename Impl>class Interface>
	struct SI_Contains_Helper<std::void_t<ITopoSort_t<SI_ImplTraits_IList_t<T>>>, T, Interface>
		: std::bool_constant<TContain_v<ITopoSort_t<SI_ImplTraits_IList_t<T>>, Interface>> {};
}

template<typename Impl>
struct Ubpa::details::SI : SI_Helper<ITopoSort_t<SI_ImplTraits_IList_t<Impl>>, Impl> {};

template<typename T, template<typename Base, typename Impl>class Interface>
struct Ubpa::SI_Contains : details::SI_Contains_Helper<void, T, Interface> {};

#pragma once

#include "TemplateList.h"

#define InterfaceTraits_Regist(Interface, ...) \
template<> \
struct Ubpa::InterfaceTraits<Interface> { \
	using IList = TemplateList<__VA_ARGS__>;\
}

#define InterfaceTraits_Regist_Pro(Interface, ...) \
template<> \
struct Ubpa::InterfaceTraits<Interface> : Ubpa::detail::SI_::IListBase<__VA_ARGS__>

#define ImplTraits_Regist(Impl, ...) \
template<> \
struct Ubpa::ImplTraits<Impl> { \
	using IList = TemplateList<__VA_ARGS__>;\
}

#define ImplTraits_Regist_Pro(Impl, ...) \
template<> \
struct Ubpa::ImplTraits<Impl> : Ubpa::detail::SI_::IListBase<__VA_ARGS__>

namespace Ubpa {
	// IList : TemplateList<...>
	template<typename Impl>
	struct ImplTraits {};

	// IList : TemplateList<...>
	template<template<typename...>class Interface>
	struct InterfaceTraits {};

	// template<template<typename, typename>class Interface>
	// static constexpr bool IsContain() noexcept;
	template<typename Impl>
	struct SI;
}

namespace Ubpa::detail::SI_ {
	struct Nil;

	template<typename Impl> struct ImplTrait_IList;
	template<typename Impl> using ImplTrait_IList_t = typename ImplTrait_IList<Impl>::type;

	template<template<typename, typename> class Interface>
	struct InterfaceTrait_IList;
	template<template<typename, typename> class Interface>
	using InterfaceTrait_IList_t = typename InterfaceTrait_IList<Interface>::type;

	template<typename IList, typename Impl> struct SI;
	template<typename IList, typename Impl> using SI_t = typename SI<IList, Impl>::type;

	template<typename IList> struct TopoSort;
	template<typename IList> using TopoSort_t = typename TopoSort<IList>::type;
}

namespace Ubpa {
	// template<template<typename, typename>class Interface>
	// static constexpr bool IsContain() noexcept;
	template<typename Impl>
	struct SI : detail::SI_::SI_t<detail::SI_::TopoSort_t<detail::SI_::ImplTrait_IList_t<Impl>>, Impl> {
		using IList = detail::SI_::TopoSort_t<detail::SI_::ImplTrait_IList_t<Impl>>;
		using Base = detail::SI_::SI_t<IList, Impl>;

		using Base::Base;

		template<template<typename, typename>class Interface>
		static constexpr bool IsContain() noexcept {
			return TExistGenericity_v<IList, Instantiate_t<TypeList<detail::SI_::Nil, Impl>, Interface>>;
		}
	};
}

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
	};

	template<template<typename...>class... Ts>
	struct IListBase {
		using IList = TemplateList<Ts...>;
	};

	template<typename Impl, typename Enabler>
	struct ImplTrait_IList_Helper : IType<TemplateList<>> {};

	template<typename Impl>
	struct ImplTrait_IList_Helper<Impl, std::void_t<typename ImplTraits<Impl>::IList>>
		: IType<typename ImplTraits<Impl>::IList> {};

	template<typename Impl>
	struct ImplTrait_IList : ImplTrait_IList_Helper<Impl, void> {};

	template<template<typename, typename> class Interface, typename Enabler>
	struct InterfaceTrait_IList_Helper : IType<TemplateList<>> {};

	template<template<typename, typename> class Interface>
	struct InterfaceTrait_IList_Helper<Interface, std::void_t<typename InterfaceTraits<Interface>::IList>>
		: IType<typename InterfaceTraits<Interface>::IList> {};

	template<template<typename, typename> class Interface>
	struct InterfaceTrait_IList : InterfaceTrait_IList_Helper<Interface, void> {};

	template<typename IList, typename Impl>
	struct SI;

	template<typename Impl>
	struct SI<TemplateList<>, Impl> : IType<Nil> {};

	template<template<typename, typename>class IHead,
		template<typename, typename>class... ITail,
		typename Impl>
	struct SI<TemplateList<IHead, ITail...>, Impl>
		: IType<IHead<SI_t<TemplateList<ITail...>, Impl>, Impl>> {};

	template<typename IList, typename SortedIList>
	struct TopoSortImpl;
	template<typename IList, typename SortedIList>
	using TopoSortImpl_t = typename TopoSortImpl<IList, SortedIList>::type;

	template<typename SortedIList>
	struct TopoSortImpl<TemplateList<>, SortedIList>
		: IType<SortedIList> {};

	template<template<typename, typename>class IHead,
		template<typename, typename>class... ITail,
		typename SortedIList>
	struct TopoSortImpl<TemplateList<IHead, ITail...>, SortedIList> {
		template<bool> struct Recursion;
		template<> struct Recursion<true> : IType<SortedIList> {};
		template<> struct Recursion<false>
			: IType<TPushFront_t<TopoSortImpl_t<InterfaceTrait_IList_t<IHead>, SortedIList>, IHead>> {};

		using type = TopoSortImpl_t<TemplateList<ITail...>, typename Recursion<TContain_v<SortedIList, IHead>>::type>;
	};

	template<typename IList>
	struct TopoSort : TopoSortImpl<IList, TemplateList<>> {};
}

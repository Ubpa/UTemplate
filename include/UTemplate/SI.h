#pragma once

#include "TemplateList.h"

#define InterfaceTraits_Register(Interface, ...) \
template<>                                       \
struct Ubpa::InterfaceTraits<Interface> {        \
	using IList = TemplateList<__VA_ARGS__>;     \
}

#define InterfaceTraits_Register_Pro(Interface, ...)                                \
template<>                                                                          \
struct Ubpa::InterfaceTraits<Interface> : Ubpa::details::IListBase<__VA_ARGS__>

#define CombineInterface(Interface, ...)         \
template<typename Base, typename Impl>           \
struct Interface : Base {                        \
    using Base::Base;                            \
};                                               \
InterfaceTraits_Register(Interface, __VA_ARGS__)

#define ImplTraits_Register(Impl, ...)       \
template<>                                   \
struct Ubpa::ImplTraits<Impl> {              \
	using IList = TemplateList<__VA_ARGS__>; \
}

#define ImplTraits_Register_Pro(Impl, ...)                                \
template<>                                                                \
struct Ubpa::ImplTraits<Impl> : Ubpa::details::IListBase<__VA_ARGS__>

namespace Ubpa::details {
	template<typename Impl> struct SI;
	template<typename Impl> using SI_t = typename SI<Impl>::type;
}

namespace Ubpa {
	// IList : TemplateList<...>
	template<typename Impl>
	struct ImplTraits {};

	// IList : TemplateList<...>
	template<template<typename...>class Interface>
	struct InterfaceTraits {};

	// template<template<typename, typename>class Interface>
	// static constexpr bool Contains() noexcept;
	template<typename Impl>
	using SI = details::SI_t<Impl>;

	template<typename T, template<typename...>class Interface>
	struct SI_Contains;
	template<typename T, template<typename...>class Interface>
	static constexpr bool SI_Contains_v = SI_Contains<T, Interface>::value;
}

#include "details/SI.inl"

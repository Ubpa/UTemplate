#pragma once

namespace Ubpa {
	template<template<typename...> class... Ts>
	struct TLength<TemplateList<Ts...>> : IValue<size_t, sizeof...(Ts)> {};

	template<typename TList>
	struct TIsEmpty : IValue<bool, TLength_v<TList> == 0 > {};

	/*
	// TFront will introduce new template
	template<template<typename...> class Head, template<typename...> class... Tail>
	struct TFront<TemplateList<Head, Tail...>> {
		template<typename... Ts>
		using Ttype = Head<Ts...>;
	};
	*/

	template<template<typename...> class T, template<typename...> class... Ts>
	struct TPushFront<TemplateList<Ts...>, T> : IType<TemplateList<T, Ts...>> {};

	template<template<typename...>class T, template<typename...> class... Ts>
	struct TPushBack<TemplateList<Ts...>, T> : IType<TemplateList<Ts..., T>> {};

	template<template<typename...> class Head, template<typename...> class... Tail>
	struct TPopFront<TemplateList<Head, Tail...>> : IType<TemplateList<Tail...>> {};

	/*
	// TAt will introduce new template
	template<typename TList>
	struct TAt<TList, 0> {
		template<typename... Ts>
		using Ttype = typename TFront<TList>::template Ttype<Ts...>;
	};

	template<typename TList, size_t N>
	struct TAt : TAt<TPopFront_t<TList>, N - 1> { };
	*/

	template<template <typename I, template<typename...> class X> class Op, typename I>
	struct TAccumulate<TemplateList<>, Op, I> : IType<I> {};

	template<template <typename I, template<typename...> class X> class Op, typename I,
		template<typename...>class THead, template<typename...>class... TTail>
	struct TAccumulate<TemplateList<THead, TTail...>, Op, I>
		: TAccumulate<TemplateList<TTail...>, Op, typename Op<I, THead>::type> { };

	template<typename TList>
	struct TReverse : TAccumulate<TList, TPushFront, TemplateList<>> {};

	template<typename TList0, typename TList1>
	struct TConcat : TAccumulate<TList1, TPushBack, TList0> {};

	template<template<template<typename...> class T> class Op, template<typename...> class... Ts>
	struct TTransform<TemplateList<Ts...>, Op> : IType<TemplateList<Op<Ts>::template Ttype...>> {};

	// TSelect
	// template<typename TList, size_t... Indices>
	// struct TSelect : IType<TemplateList<TAt<TList, Indices>::template Ttype...>> {};

	template<template<typename...>class... Ts, typename Instance>
	struct TExistGenericity<TemplateList<Ts...>, Instance> : IValue<bool, (is_instance_of_v<Instance, Ts> || ...)> {};

	template<typename ArgList, template<typename...> class... Ts>
	struct TInstance<TemplateList<Ts...>, ArgList> : IType<TypeList<Instantiate_t<ArgList, Ts>...>> {};

	template<typename TList, typename... Instances>
	struct TExistGenericities<TList, TypeList<Instances...>>
		: IValue<bool, (TExistGenericity_v<TList, Instances>&&...)> {};

	template<typename InstanceList, template<typename...>class... Ts>
	struct TCanGeneralizeFromList<TemplateList<Ts...>, InstanceList>
		: IValue<bool, (ExistInstance_v<InstanceList, Ts>&&...)> {};

	template<template<typename...>class... Ts, template<typename...>class T>
	struct TContain<TemplateList<Ts...>, T> : IValue<bool, (is_same_typename_template_v<Ts, T> || ...)> {};
}

#pragma once

namespace Ubpa::details {
	// ref: qobjectdefs_impl.h

	template <typename T> struct RmvLValueRef : std::type_identity<T> {};
	template <typename T> struct RmvLValueRef<T&> : std::type_identity<T> {};
	template <typename T> struct RmvConstRef : std::type_identity<T> {};
	template <typename T> struct RmvConstRef<const T&> : std::type_identity<T> {};

	template<typename A1, typename A2>
	struct AreArgumentsCompatible
		: std::is_same<const typename RmvLValueRef<A1>::type&, const typename RmvLValueRef<A2>::type&> {};
	template<typename A1, typename A2> struct AreArgumentsCompatible<A1, A2&>
		: std::false_type {};
	template<typename A> struct AreArgumentsCompatible<A&, A&>
		: std::true_type {};
	// void as a return value
	template<typename A> struct AreArgumentsCompatible<void, A>
		: std::true_type {};
	template<typename A> struct AreArgumentsCompatible<A, void>
		: std::true_type {};
	template<> struct AreArgumentsCompatible<void, void>
		: std::true_type {};

	// ========================

	template<typename ToArgList, typename FromArgList>
	struct CheckCompatibleArguments
		: std::false_type {};

	template<>
	struct CheckCompatibleArguments<TypeList<>, TypeList<>>
		: std::true_type {};

	template<typename ToArgList>
	struct CheckCompatibleArguments<ToArgList, TypeList<>>
		: std::true_type {};

	template<typename FromArgHead, typename... FromArgTail>
	struct CheckCompatibleArguments<TypeList<>, TypeList<FromArgHead, FromArgTail...>>
		: std::false_type {};

	template<typename ToArgHead, typename FromArgHead, typename... ToArgTail, typename... FromArgTail>
	struct CheckCompatibleArguments<TypeList<ToArgHead, ToArgTail...>, TypeList<FromArgHead, FromArgTail...>> {
		static constexpr bool value =
			AreArgumentsCompatible<typename RmvConstRef<ToArgHead>::type,
				typename RmvConstRef<FromArgHead>::type>::value
			&& CheckCompatibleArguments<TypeList<ToArgTail...>, TypeList<FromArgTail...>>::value;
	};
}

namespace Ubpa::details {
	template<bool IsConst, bool IsVolatile, ReferenceMode Ref, bool IsNoexcept, typename Sig>
	struct FuncTraitsBase;

	template<bool IsConst, bool IsVolatile, ReferenceMode Ref, bool IsNoexcept, typename Ret, typename... Args>
	struct FuncTraitsBase<IsConst, IsVolatile, Ref, IsNoexcept, Ret(Args...)> {
		using ArgList = TypeList<Args...>;
		using Return = Ret;
		using Signature = Ret(Args...);
		static constexpr bool is_const = IsConst;
		static constexpr bool is_volatile = IsVolatile;
		static constexpr ReferenceMode ref = Ref;
		static constexpr bool is_noexcept = IsNoexcept;
	};

	template<bool IsFunc, typename T> struct FuncTraitsDispatch;
	template<typename T> struct FuncTraitsDispatch<false, T> : FuncTraits<decltype(&std::decay_t<T>::operator())> {};
	template<typename T> struct FuncTraitsDispatch<true, T> : FuncTraits<T> {
		using Function = T;
	};
}

// 2*2*3*2 = 24
template<typename Ret, typename... Args> // 0000
struct Ubpa::FuncTraits<Ret(Args...)> 
	: details::FuncTraitsBase<false, false, ReferenceMode::None, false, Ret(Args...)> {};

template<typename Ret, typename... Args> // 1000
struct Ubpa::FuncTraits<Ret(Args...)const>
	: details::FuncTraitsBase<true, false, ReferenceMode::None, false, Ret(Args...)> {};

template<typename Ret, typename... Args> // 0100
struct Ubpa::FuncTraits<Ret(Args...)volatile>
	: details::FuncTraitsBase<false, true, ReferenceMode::None, false, Ret(Args...)> {};

template<typename Ret, typename... Args> // 1100
struct Ubpa::FuncTraits<Ret(Args...)const volatile>
	: details::FuncTraitsBase<true, true, ReferenceMode::None, false, Ret(Args...)> {};

template<typename Ret, typename... Args> // 0010
struct Ubpa::FuncTraits<Ret(Args...)&>
	: details::FuncTraitsBase<false, false, ReferenceMode::Left, false, Ret(Args...)> {};

template<typename Ret, typename... Args> // 1010
struct Ubpa::FuncTraits<Ret(Args...)const&>
	: details::FuncTraitsBase<true, false, ReferenceMode::Left, false, Ret(Args...)> {};

template<typename Ret, typename... Args> // 0110
struct Ubpa::FuncTraits<Ret(Args...)volatile&>
	: details::FuncTraitsBase<false, true, ReferenceMode::Left, false, Ret(Args...)> {};

template<typename Ret, typename... Args> // 1110
struct Ubpa::FuncTraits<Ret(Args...)const volatile&>
	: details::FuncTraitsBase<true, true, ReferenceMode::Left, false, Ret(Args...)> {};

template<typename Ret, typename... Args> // 0020
struct Ubpa::FuncTraits<Ret(Args...)&&>
	: details::FuncTraitsBase<false, false, ReferenceMode::Right, false, Ret(Args...)> {};

template<typename Ret, typename... Args> // 1020
struct Ubpa::FuncTraits<Ret(Args...)const&&>
	: details::FuncTraitsBase<true, false, ReferenceMode::Right, false, Ret(Args...)> {};

template<typename Ret, typename... Args> // 0120
struct Ubpa::FuncTraits<Ret(Args...)volatile&&>
	: details::FuncTraitsBase<false, true, ReferenceMode::Right, false, Ret(Args...)> {};

template<typename Ret, typename... Args> // 1120
struct Ubpa::FuncTraits<Ret(Args...)const volatile&&>
	: details::FuncTraitsBase<true, true, ReferenceMode::Right, false, Ret(Args...)> {};

template<typename Ret, typename... Args> // 0001
struct Ubpa::FuncTraits<Ret(Args...)noexcept>
	: details::FuncTraitsBase<false, false, ReferenceMode::None, true, Ret(Args...)> {};

template<typename Ret, typename... Args> // 1001
struct Ubpa::FuncTraits<Ret(Args...)const noexcept>
	: details::FuncTraitsBase<true, false, ReferenceMode::None, true, Ret(Args...)> {};

template<typename Ret, typename... Args> // 0101
struct Ubpa::FuncTraits<Ret(Args...)volatile noexcept>
	: details::FuncTraitsBase<false, true, ReferenceMode::None, true, Ret(Args...)> {};

template<typename Ret, typename... Args> // 1101
struct Ubpa::FuncTraits<Ret(Args...)const volatile noexcept>
	: details::FuncTraitsBase<true, true, ReferenceMode::None, true, Ret(Args...)> {};

template<typename Ret, typename... Args> // 0011
struct Ubpa::FuncTraits<Ret(Args...) & noexcept>
	: details::FuncTraitsBase<false, false, ReferenceMode::Left, true, Ret(Args...)> {};

template<typename Ret, typename... Args> // 1011
struct Ubpa::FuncTraits<Ret(Args...)const& noexcept>
	: details::FuncTraitsBase<true, false, ReferenceMode::Left, true, Ret(Args...)> {};

template<typename Ret, typename... Args> // 0111
struct Ubpa::FuncTraits<Ret(Args...)volatile& noexcept>
	: details::FuncTraitsBase<false, true, ReferenceMode::Left, true, Ret(Args...)> {};

template<typename Ret, typename... Args> // 1111
struct Ubpa::FuncTraits<Ret(Args...)const volatile& noexcept>
	: details::FuncTraitsBase<true, true, ReferenceMode::Left, true, Ret(Args...)> {};

template<typename Ret, typename... Args> // 0021
struct Ubpa::FuncTraits<Ret(Args...) && noexcept>
	: details::FuncTraitsBase<false, false, ReferenceMode::Right, true, Ret(Args...)> {};

template<typename Ret, typename... Args> // 1021
struct Ubpa::FuncTraits<Ret(Args...)const&& noexcept>
	: details::FuncTraitsBase<true, false, ReferenceMode::Right, true, Ret(Args...)> {};

template<typename Ret, typename... Args> // 0121
struct Ubpa::FuncTraits<Ret(Args...)volatile&& noexcept>
	: details::FuncTraitsBase<false, true, ReferenceMode::Right, true, Ret(Args...)> {};

template<typename Ret, typename... Args> // 1121
struct Ubpa::FuncTraits<Ret(Args...)const volatile&& noexcept>
	: details::FuncTraitsBase<true, true, ReferenceMode::Right, true, Ret(Args...)> {};

// dispatch
template<typename Func>
struct Ubpa::FuncTraits<Func*> : FuncTraits<Func> {
	using Object = void;
	using Function = Func;
};

template<typename T, typename Func>
struct Ubpa::FuncTraits<Func T::*> : FuncTraits<Func> {
	using Object = T;
	using Function = Func;
};

template<typename Func>
struct Ubpa::FuncTraits<Func&> : FuncTraits<Func> {};
template<typename Func>
struct Ubpa::FuncTraits<Func&&> : FuncTraits<Func> {};
template<typename Func>
struct Ubpa::FuncTraits<const Func&> : FuncTraits<Func> {};
template<typename Func>
struct Ubpa::FuncTraits<const Func&&> : FuncTraits<Func> {};

template<typename T>
struct Ubpa::FuncTraits : details::FuncTraitsDispatch<std::is_function_v<T>, T> {};

template<typename Ret, typename... Args>
struct Ubpa::FuncExpand<Ret(Args...)> {
	template<typename Func>
	static auto get(Func&& func) noexcept {
		static_assert(std::is_void_v<Ret> || std::is_convertible_v<FuncTraits_Return<Func>, Ret>,
			"Func's return can't convert to Ret");
		constexpr std::size_t N = Length_v<typename FuncTraits<Func>::ArgList>;
		return get(std::forward<Func>(func), std::make_index_sequence<N>{});
	}

private:
	template<typename Func, std::size_t... Ns>
	static auto get(Func&& func, std::index_sequence<Ns...>) {
		using FromArgList = typename FuncTraits<Func>::ArgList;
		using ToArgList = TypeList<Args...>;
		return [func = std::forward<Func>(func)](Args... args) mutable -> decltype(auto) {
			std::tuple<Args...> argTuple{ std::forward<Args>(args)... };
			static_assert(details::CheckCompatibleArguments<ToArgList, FromArgList>::value,
				"from and to arguments are not compatible.");
			if constexpr (std::is_void_v<Ret>)
				func(std::forward<At_t<ToArgList, Ns>>(std::get<Ns>(argTuple))...);
			else
				return static_cast<Ret>(func(std::forward<At_t<ToArgList, Ns>>(std::get<Ns>(argTuple))...));
		};
	}
};

template<typename Lambda>
constexpr auto Ubpa::DecayLambda(Lambda&& lambda) noexcept {
	return static_cast<std::add_pointer_t<FuncTraits_Signature<std::remove_reference_t<Lambda>>>>(std::forward<Lambda>(lambda));
}

template<typename Obj, typename Func>
struct Ubpa::MemFuncOf {
	static_assert(std::is_function_v<Func>);
	static constexpr auto get(Func Obj::* func) noexcept {
		return func;
	}
};

template<typename Func>
struct Ubpa::FuncOf {
	static_assert(std::is_function_v<Func>);
	static constexpr auto get(Func* func) noexcept {
		return func;
	}
};

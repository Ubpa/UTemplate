#pragma once

#include "Typelist.h"
#include "Concept.h"

#include <tuple>
#include <utility>

namespace Ubpa {
	// type ArgList : TypeList<Args...>
	// type Ret
	// type Obj : if not member function, it is void
	// type Signature : Ret(Args...)
	// bool is_const
	// bool is_volatile
	// ReferenceMode ref
	// bool is_noexcept
	enum class ReferenceMode {
		NONE,
		LEFT,
		RIGHT
	};
	template<typename T> struct FuncTraits;
	template<typename T> using FuncTraits_ArgList = typename FuncTraits<T>::ArgList;
	template<typename T> using FuncTraits_Ret = typename FuncTraits<T>::Ret;
	template<typename T> using FuncTraits_Obj = typename FuncTraits<T>::Obj;
	template<typename T> using FuncTraits_Signature = typename FuncTraits<T>::Signature;
	template<typename T> constexpr bool FuncTraits_is_const = FuncTraits<T>::is_const;
	template<typename T> constexpr bool FuncTraits_is_volatile = FuncTraits<T>::is_volatile;
	template<typename T> constexpr ReferenceMode FuncTraits_ref = FuncTraits<T>::ref;
	template<typename T> constexpr bool FuncTraits_is_noexcept = FuncTraits<T>::is_noexcept;

	template<typename Func>
	struct MemFuncOf;

	template<typename Func>
	struct StaticMemFuncOf;

	// NewFunc == Ret(Args...)
	// static Ret(Args...) run(Func);
	// - { Func's arguments, ... } == { Args... }
	// - Ret == void or Ret <- Func'return type
	template<typename NewFunc> struct FuncExpand;

	template<typename Lambda>
	constexpr auto DecayLambda(Lambda&& lambda);
}

//============================================================

namespace Ubpa::detail::Func_ {
	// ref: qobjectdefs_impl.h

	template <typename T> struct RmvLValueRef : IType<T> {};
	template <typename T> struct RmvLValueRef<T&> : IType<T> {};
	template <typename T> struct RmvConstRef : IType<T> {};
	template <typename T> struct RmvConstRef<const T&> : IType<T> {};

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

namespace Ubpa {
	template<bool IsConst, bool IsVolatile, ReferenceMode Ref, bool IsNoexcept, typename Func>
	struct FuncTraitsBase;

	template<bool IsConst, bool IsVolatile, ReferenceMode Ref, bool IsNoexcept, typename _Ret, typename... Args>
	struct FuncTraitsBase<IsConst, IsVolatile, Ref, IsNoexcept, _Ret(Args...)> {
		using ArgList = TypeList<Args...>;
		using Ret = _Ret;
		using Signature = Ret(Args...);
		static constexpr bool is_const = IsConst;
		static constexpr bool is_volatile = IsVolatile;
		static constexpr ReferenceMode ref = Ref;
		static constexpr bool is_noexcept = IsNoexcept;
	};

	// 2*2*3*2 = 24
	template<typename Ret, typename... Args> // 0000
	struct FuncTraits<Ret(Args...)> : FuncTraitsBase<false, false, ReferenceMode::NONE, false, Ret(Args...)> {};
	template<typename Ret, typename... Args> // 1000
	struct FuncTraits<Ret(Args...)const> : FuncTraitsBase<true, false, ReferenceMode::NONE, false, Ret(Args...)> {};
	template<typename Ret, typename... Args> // 0100
	struct FuncTraits<Ret(Args...)volatile> : FuncTraitsBase<false, true, ReferenceMode::NONE, false, Ret(Args...)> {};
	template<typename Ret, typename... Args> // 1100
	struct FuncTraits<Ret(Args...)const volatile> : FuncTraitsBase<true, true, ReferenceMode::NONE, false, Ret(Args...)> {};
	template<typename Ret, typename... Args> // 0010
	struct FuncTraits<Ret(Args...)&> : FuncTraitsBase<false, false, ReferenceMode::LEFT, false, Ret(Args...)> {};
	template<typename Ret, typename... Args> // 1010
	struct FuncTraits<Ret(Args...)const&> : FuncTraitsBase<true, false, ReferenceMode::LEFT, false, Ret(Args...)> {};
	template<typename Ret, typename... Args> // 0110
	struct FuncTraits<Ret(Args...)volatile&> : FuncTraitsBase<false, true, ReferenceMode::LEFT, false, Ret(Args...)> {};
	template<typename Ret, typename... Args> // 1110
	struct FuncTraits<Ret(Args...)const volatile&> : FuncTraitsBase<true, true, ReferenceMode::LEFT, false, Ret(Args...)> {};
	template<typename Ret, typename... Args> // 0020
	struct FuncTraits<Ret(Args...)&&> : FuncTraitsBase<false, false, ReferenceMode::RIGHT, false, Ret(Args...)> {};
	template<typename Ret, typename... Args> // 1020
	struct FuncTraits<Ret(Args...)const&&> : FuncTraitsBase<true, false, ReferenceMode::RIGHT, false, Ret(Args...)> {};
	template<typename Ret, typename... Args> // 0120
	struct FuncTraits<Ret(Args...)volatile&&> : FuncTraitsBase<false, true, ReferenceMode::RIGHT, false, Ret(Args...)> {};
	template<typename Ret, typename... Args> // 1120
	struct FuncTraits<Ret(Args...)const volatile&&> : FuncTraitsBase<true, true, ReferenceMode::RIGHT, false, Ret(Args...)> {};
	template<typename Ret, typename... Args> // 0001
	struct FuncTraits<Ret(Args...)noexcept> : FuncTraitsBase<false, false, ReferenceMode::NONE, true, Ret(Args...)> {};
	template<typename Ret, typename... Args> // 1001
	struct FuncTraits<Ret(Args...)const noexcept> : FuncTraitsBase<true, false, ReferenceMode::NONE, true, Ret(Args...)> {};
	template<typename Ret, typename... Args> // 0101
	struct FuncTraits<Ret(Args...)volatile noexcept> : FuncTraitsBase<false, true, ReferenceMode::NONE, true, Ret(Args...)> {};
	template<typename Ret, typename... Args> // 1101
	struct FuncTraits<Ret(Args...)const volatile noexcept> : FuncTraitsBase<true, true, ReferenceMode::NONE, true, Ret(Args...)> {};
	template<typename Ret, typename... Args> // 0011
	struct FuncTraits<Ret(Args...)& noexcept> : FuncTraitsBase<false, false, ReferenceMode::LEFT, true, Ret(Args...)> {};
	template<typename Ret, typename... Args> // 1011
	struct FuncTraits<Ret(Args...)const& noexcept> : FuncTraitsBase<true, false, ReferenceMode::LEFT, true, Ret(Args...)> {};
	template<typename Ret, typename... Args> // 0111
	struct FuncTraits<Ret(Args...)volatile& noexcept> : FuncTraitsBase<false, true, ReferenceMode::LEFT, true, Ret(Args...)> {};
	template<typename Ret, typename... Args> // 1111
	struct FuncTraits<Ret(Args...)const volatile& noexcept> : FuncTraitsBase<true, true, ReferenceMode::LEFT, true, Ret(Args...)> {};
	template<typename Ret, typename... Args> // 0021
	struct FuncTraits<Ret(Args...)&& noexcept> : FuncTraitsBase<false, false, ReferenceMode::RIGHT, true, Ret(Args...)> {};
	template<typename Ret, typename... Args> // 1021
	struct FuncTraits<Ret(Args...)const&& noexcept> : FuncTraitsBase<true, false, ReferenceMode::RIGHT, true, Ret(Args...)> {};
	template<typename Ret, typename... Args> // 0121
	struct FuncTraits<Ret(Args...)volatile&& noexcept> : FuncTraitsBase<false, true, ReferenceMode::RIGHT, true, Ret(Args...)> {};
	template<typename Ret, typename... Args> // 1121
	struct FuncTraits<Ret(Args...)const volatile&& noexcept> : FuncTraitsBase<true, true, ReferenceMode::RIGHT, true, Ret(Args...)> {};

	template<typename Func>
	struct FuncTraits<Func*> : FuncTraits<Func> {
		using Obj = void;
	};

	template<typename T, typename Func>
	struct FuncTraits<Func T::*> : FuncTraits<Func> {
		using Obj = T;
	};

	template<bool isFunc, typename T>
	struct FuncTraitsDispatch;

	template<typename T>
	struct FuncTraitsDispatch<false,T> : FuncTraits<decltype(&std::decay_t<T>::operator())> {};

	template<typename T>
	struct FuncTraitsDispatch<true, T> : FuncTraits<T> {};

	template<typename T>
	struct FuncTraits : FuncTraitsDispatch<std::is_function_v<T>, T> {};

	//============================================================

	template<typename Ret, typename... Args>
	struct FuncExpand<Ret(Args...)> {
		template<typename Func>
		static auto run(Func&& func) noexcept {
			static_assert(std::is_void_v<Ret> || std::is_convertible_v<FuncTraits_Ret<Func>, Ret>,
				"Func's return can't convert to Ret");
			constexpr size_t N = Length_v<typename FuncTraits<Func>::ArgList>;
			return run(std::forward<Func>(func), std::make_index_sequence<N>{});
		}

	private:
		template<typename Func, size_t... Ns>
		static auto run(Func&& func, std::index_sequence<Ns...>) {
			using FromArgList = typename FuncTraits<Func>::ArgList;
			using ToArgList = TypeList<Args...>;
			return [func = std::forward<Func>(func)](Args... args) {
				std::tuple<Args...> argTuple{ std::forward<Args>(args)... };
				static_assert(detail::Func_::CheckCompatibleArguments<ToArgList, FromArgList>::value,
					"from and to arguments are not compatible.");
				if constexpr(std::is_void_v<Ret>)
					func(std::get<Ns>(argTuple)...);
				else
					return static_cast<Ret>(func(std::get<Ns>(argTuple)...));
			};
		}
	};

	// =================================================================

	template<typename Lambda>
	constexpr auto DecayLambda(Lambda&& lambda) {
		return static_cast<std::add_pointer_t<FuncTraits_Signature<std::remove_reference_t<Lambda>>>>(lambda);
	}

	template<typename Func>
	struct MemFuncOf {
		template<typename Obj>
		static constexpr auto run(Func Obj::* func) noexcept {
			return func;
		}
	};

	template<typename Func>
	struct StaticMemFuncOf {
		static constexpr auto run(Func* func) noexcept {
			return func;
		}
	};
}

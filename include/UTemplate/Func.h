#pragma once

#include "Typelist.h"

#include <tuple>
#include <utility>

namespace Ubpa {
	// type ArgList : TypeList<Args...>
	// type Ret
	// type Obj
	// bool is_member
	// bool is_const
	template<typename T> struct FuncTraits;
	template<typename T> using FuncTraits_ArgList = typename FuncTraits<T>::ArgList;
	template<typename T> using FuncTraits_Ret = typename FuncTraits<T>::Ret;
	template<typename T> using FuncTraits_Obj = typename FuncTraits<T>::Obj;

	// NewFunc == Ret(Args...)
	// static Ret(Args...) run(Func);
	// - { Func's arguments, ... } == { Args... }
	// - Ret == void or Ret <- Func'return type
	template<typename NewFunc> struct FuncExpand;

	template<typename Func>
	struct MemFuncOf;
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
	template<typename _Ret, typename... Args>
	struct FuncTraits<_Ret(Args...)> {
		using ArgList = TypeList<Args...>;
		using Ret = _Ret;
		static constexpr bool is_member = false;
		static constexpr bool is_const = false;
	};

	template<typename T, typename _Ret, typename... Args>
	struct FuncTraits<_Ret(T::*)(Args...)> {
		using ArgList = TypeList<Args...>;
		using Ret = _Ret;
		using Obj = T;
		static constexpr bool is_member = true;
		static constexpr bool is_const = false;
	};

	template<typename T, typename _Ret, typename... Args>
	struct FuncTraits<_Ret(T::*)(Args...) const> {
		using ArgList = TypeList<Args...>;
		using Ret = _Ret;
		using Obj = T;
		static constexpr bool is_member = true;
		static constexpr bool is_const = true;
	};

	template<typename T>
	struct FuncTraits : FuncTraits<decltype(&std::decay_t<T>::operator())> {};

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

	// =========================

	template<typename Func>
	struct MemFuncOf {
		template<typename Obj>
		static constexpr auto run(Func Obj::* func) noexcept {
			return func;
		}
	};
}

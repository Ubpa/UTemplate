#pragma once

#include "Typelist.h"

#include <tuple>
#include <utility>

namespace Ubpa {
	// type ArgList : TypeList<Args...>
	// type Ret
	// bool is_member
	// bool is_const
	template<typename T> struct FuncTraits;

	// static NewFunc run(Func);
	// require : { Func's arguments, ... } == { Args... }
	// NewFunc
	// - return type is same with Func
	// - arguments are Args
	template<typename... Args> struct FuncExpand;
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
		static constexpr bool is_member = true;
		static constexpr bool is_const = false;
	};

	template<typename T, typename _Ret, typename... Args>
	struct FuncTraits<_Ret(T::*)(Args...) const> {
		using ArgList = TypeList<Args...>;
		using Ret = _Ret;
		static constexpr bool is_member = true;
		static constexpr bool is_const = true;
	};

	template<typename T>
	struct FuncTraits : FuncTraits<decltype(&std::decay_t<T>::operator())> {};

	//============================================================

	template<typename... Args>
	struct FuncExpand {
		template<typename Func>
		static auto run(Func&& func) noexcept {
			constexpr size_t N = Length_v<typename FuncTraits<Func>::ArgList>;
			return run(std::forward<Func>(func), std::make_index_sequence<N>{});
		}

	private:
		template<typename Func, size_t... Ns>
		static auto run(Func&& func, std::index_sequence<Ns...>) {
			using FromArgList = typename FuncTraits<Func>::ArgList;
			using ToArgList = TypeList<Args...>;
			return[func = std::forward<Func>(func)](Args... args) {
				std::tuple<Args...> argTuple{ std::forward<Args>(args)... };
				static_assert(detail::Func_::CheckCompatibleArguments<ToArgList, FromArgList>::value,
					"from and to arguments are not compatible.");
				func(std::get<Ns>(argTuple)...);
			};
		}
	};
}

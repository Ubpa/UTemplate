#pragma once

#include "TypeList.hpp"

#include <tuple>

namespace Ubpa {
	// type Object : if not member function, it is void
	// type ArgList : TypeList<Args...>
	// type Return
	// type Signature : Ret(Args...)
	// type Function : Ret(Args...) const? volatile? &/&&? noexcept 
	// bool is_const
	// bool is_volatile
	// ReferenceMode ref
	// bool is_noexcept
	template<typename T> struct FuncTraits;

	template<typename T> using FuncTraits_Object = typename FuncTraits<T>::Object;
	template<typename T> using FuncTraits_ArgList = typename FuncTraits<T>::ArgList;
	template<typename T> using FuncTraits_Return = typename FuncTraits<T>::Return;
	template<typename T> using FuncTraits_Signature = typename FuncTraits<T>::Signature;
	template<typename T> using FuncTraits_Function = typename FuncTraits<T>::Function;
	template<typename T> constexpr bool FuncTraits_is_const = FuncTraits<T>::is_const;
	template<typename T> constexpr bool FuncTraits_is_volatile = FuncTraits<T>::is_volatile;
	template<typename T> constexpr ReferenceMode FuncTraits_ref = FuncTraits<T>::ref;
	template<typename T> constexpr bool FuncTraits_is_noexcept = FuncTraits<T>::is_noexcept;

	// overload
	template<typename Obj, typename Func> struct MemFuncOf;
	template<typename Func> struct FuncOf;

	// NewFunc == Ret(Args...)
	// static Ret(Args...) run(Func);
	// - { Func's arguments, ... } == { Args... }
	// - Ret == void or Ret <- Func'return type
	template<typename NewFunc> struct FuncExpand;

	template<typename Lambda>
	constexpr auto DecayLambda(Lambda&& lambda) noexcept;
}

#include "details/Func.inl"

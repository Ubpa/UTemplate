#pragma once

#include "Typelist.h"

namespace Ubpa {
	// ArgList : TypeList<Args...>
	// Ret
	template<typename T> struct FuncTraits;
}

//============================================================

namespace Ubpa::detail::FuncTraits {
	template<typename T>
	struct FuncTraits;
}

namespace Ubpa {
	// ArgList, Ret
	template<typename T>
	struct FuncTraits : detail::FuncTraits::FuncTraits<T> {};
}

namespace Ubpa::detail::FuncTraits {
	template<typename T>
	struct FuncTraits;

	template<typename _Ret, typename... Args>
	struct FuncTraits<_Ret(Args...)> {
		using ArgList = TypeList<Args...>;
		using Ret = _Ret;
	};

	template<typename T, typename _Ret, typename... Args>
	struct FuncTraits<_Ret(T::*)(Args...)> {
		using ArgList = TypeList<Args...>;
		using Ret = _Ret;
	};

	template<typename T, typename _Ret, typename... Args>
	struct FuncTraits<_Ret(T::*)(Args...) const> {
		using ArgList = TypeList<Args...>;
		using Ret = _Ret;
	};

	template<typename T>
	struct FuncTraits : FuncTraits<decltype(&std::decay_t<T>::operator())> {};
}

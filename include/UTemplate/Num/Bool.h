#pragma once
#ifndef _UBPA_TEMPLATE_NUM_BOOL_
#define _UBPA_TEMPLATE_NUM_BOOL_

#include "Num.h"

namespace Ubpa {
	// Conjunction
	template<bool v, typename LastN, typename... Ns>
	struct ConjunctionRec;
	template<typename LastN>
	struct ConjunctionRec<true, LastN> {
		using type = LastN;
	};
	template<typename LastN, typename... Ns>
	struct ConjunctionRec<false, LastN, Ns...> {
		using type = LastN;
	};
	template<typename LastN, typename NHead, typename... NTail>
	struct ConjunctionRec<true, LastN, NHead, NTail...> :
		ConjunctionRec<static_cast<bool>(NHead::value), NHead, NTail...> {};

	template<typename... Ns>
	using Conjunction = ConjunctionRec<true, True, Ns...>;
	template<typename... Ns>
	using Conjunction_t = typename Conjunction<Ns...>::type;

	// Disjunction
	template<bool v, typename LastN, typename... Ns>
	struct DisjunctionRec;
	template<typename LastN>
	struct DisjunctionRec<false, LastN> {
		using type = LastN;
	};
	template<typename LastN, typename... Ns>
	struct DisjunctionRec<true, LastN, Ns...> {
		using type = LastN;
	};
	template<typename LastN, typename NHead, typename... NTail>
	struct DisjunctionRec<false, LastN, NHead, NTail...> :
		DisjunctionRec<static_cast<bool>(NHead::value), NHead, NTail...> {};

	template<typename... Ns>
	using Disjunction = DisjunctionRec<false, False, Ns...>;
	template<typename... Ns>
	using Disjunction_t = typename Disjunction<Ns...>::type;
}

#endif // !_UBPA_TEMPLATE_NUM_BOOL_

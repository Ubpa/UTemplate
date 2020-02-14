#pragma once
#ifndef _UBPA_TEMPLATE_LIST_LIST_H_
#define _UBPA_TEMPLATE_LIST_LIST_H_

#include <type_traits>
#include "../Num/Bool.h"

namespace Ubpa {
	/* [ Interface ]
	*  
	*  T    Front<List>
	*  List PushFront<List, Ts...>
	*  List PopFront<List, T>
	*  bool IsEmpty<List>::value
	*  
	*  List Clear<List>
	*  T    At<List, size_t>
	*  I    Accumulate<List, <I, T> Op, I>
	*  I    AccumulateIS<List, <I, T, size_t> Op, I, size_t...>
	*  List Reverse<List>
	*  List PushBack<List, T>
	*  List Transform<List, <T> Op>
	*  List Select<List, size...>
	*  bool Contain<List, T>::value
	*/

	// [ Require ]
	template<typename List>
	struct Front;
	template<typename List>
	using Front_t = typename Front<List>::type;

	template<typename List>
	struct PopFront;
	template<typename List>
	using PopFront_t = typename PopFront<List>::type;

	template<typename List, typename T>
	struct PushFront;
	template<typename List, typename... Ts>
	using PushFront_t = typename PushFront<List, Ts...>::type;

	template<typename List>
	struct IsEmpty { static constexpr bool value = false; };
	template<typename List>
	constexpr bool IsEmpty_v = IsEmpty<List>::value;

	// [ Algorithm ]

	// Clear
	template<typename List, bool = IsEmpty<List>::value>
	struct Clear;

	template<typename List>
	using Clear_t = typename Clear<List>::type;

	template<typename List>
	struct Clear<List, false> : Clear<PopFront<List>> { };

	template<typename List>
	struct Clear<List, true> {
		using type = List;
	};

	// At
	template<typename List, size_t N>
	struct At;

	template<typename List>
	struct At<List, 0> {
		using type = Front_t<List>;
	};

	template<typename List, size_t N>
	struct At : At<PopFront_t<List>, N - 1> { };

	template<typename List, size_t N>
	using At_t = typename At<List, N>::type;

	// Accumulate : list
	template<typename List, template <typename I, typename X> class Op, typename I, bool = IsEmpty_v<List>>
	struct Accumulate;

	template<typename List, template <typename I, typename X> class Op, typename I>
	struct Accumulate<List, Op, I, false> : Accumulate<PopFront_t<List>, Op, typename Op<I, Front_t<List>>::type> { };

	template<typename List, template <typename X, typename Y> class Op, typename I>
	struct Accumulate<List, Op, I, true> {
		using type = I;
	};

	template<typename List, template <typename I, typename X> class Op, typename I>
	using Accumulate_t = typename Accumulate<List, Op, I>::type;

	// Accumulate : integer sequence
	template<typename List, template<typename I, typename List, size_t Num> class Op, typename I, size_t... Nums>
	struct AccumulateIS;

	template<typename List, template<typename I, typename List, size_t Num> class Op, typename I>
	struct AccumulateIS<List, Op, I> {
		using type = I;
	};

	template<typename List, template<typename I, typename List, size_t Num> class Op, typename I,
		size_t NumHead, size_t... NumTail>
	struct AccumulateIS<List, Op, I, NumHead, NumTail...>
		: AccumulateIS<List, Op, typename Op<I, List, NumHead>::type, NumTail...> { };

	template<typename List, template<typename I, typename List, size_t Num> class Op, typename I, size_t... Nums>
	using AccumulateIS_t = typename AccumulateIS<List, Op, I, Nums...>::type;

	// Reverse
	template<typename List>
	using Reverse = Accumulate<List, PushFront, Clear_t<List>>;

	template<typename List>
	using Reverse_t = typename Reverse<List>::type;

	// PushBack
	template<typename List, typename T>
	using PushBack = Reverse<PushFront_t<Reverse_t<List>, T>>;

	template<typename List, typename T>
	using PushBack_t = typename PushBack<List, T>::type;

	// Transform
	template<typename List, template<typename T> class Op>
	struct Transform {
	private:
		template<typename I, typename T>
		using ToI = PushFront<I, typename Op<T>::type>;
	public:
		using type = Reverse_t<Accumulate_t<List, ToI, Clear_t<List>>>;
	};

	template<typename List, template<typename> class Op>
	using Transform_t = typename Transform<List, Op>::type;

	// Select
	template<typename List, size_t... Indices>
	struct Select {
	private:
		template<typename I, typename List, size_t Index>
		using ToI = PushFront<I, At_t<List, Index>>;
	public:
		using type = Reverse_t<AccumulateIS_t<List, ToI, Clear_t<List>, Indices...>>;
	};
	template<typename List, size_t... Indices>
	using Select_t = typename Select<List, Indices...>::type;

	// Contain
	template<typename List, typename T, bool found = false, bool = IsEmpty<List>::value>
	struct ContainRec;

	template<typename List, typename T>
	struct ContainRec<List, T, false, true> {
		static constexpr bool value = false;
	};

	template<typename List, typename T, bool isEmpty>
	struct ContainRec<List, T, true, isEmpty> {
		static constexpr bool value = true;
	};

	template<typename List, typename T>
	struct ContainRec<List, T, false, false> : ContainRec<PopFront_t<List>, T, std::is_same<Front_t<List>, T>::value> {};

	template<typename List, typename T>
	using Contain = ContainRec<List, T>;

	template<typename List, typename T>
	constexpr bool Contain_v = Contain<List, T>::value;

	// ContainList
	template<typename List0, typename List1>
	struct ContainList; // [TODO]

	// Concat
	template<typename List0, typename List1>
	using Concat = Accumulate<Reverse_t<List0>, PushFront, List1>;

	template<typename List0, typename List1>
	using Concat_t = typename Concat<List0, List1>::type;
}

#endif // !_UBPA_TEMPLATE_LIST_LIST_H_

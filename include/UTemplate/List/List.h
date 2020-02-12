#pragma once
#ifndef _UBPA_TEMPLATE_LIST_LIST_H_
#define _UBPA_TEMPLATE_LIST_LIST_H_

namespace Ubpa {
	/* [ Interface ]
	*  
	*  T    Front<List>
	*  List PushFront<List, T>
	*  List PopFront<List, T>
	*  bool IsEmpty<List>
	*  
	*  List Clear<List>
	*  T    At<List, size_t>
	*  I    Accumulate<List, <I, T> Op, I>
	*  I    AccumulateIS<List, <I, T, size_t> Op, I, size_t...>
	*  List Reverse<List>
	*  List PushBack<List, T>
	*  List Transform<List, <T> Op>
	*  List Select<List, size...>
	*/

	// [ Basic ]
	template<typename List>
	struct FrontT;
	template<typename List, typename T>
	struct PushFrontT;
	template<typename List>
	struct PopFrontT;
	template<typename List>
	struct IsEmpty { static constexpr bool value = false; };
	template<typename List>
	struct Length;

	template<typename List>
	using Front = typename FrontT<List>::type;
	template<typename List, typename T>
	using PushFront = typename PushFrontT<List, T>::type;
	template<typename List>
	using PopFront = typename PopFrontT<List>::type;

	// [ Algorithm ]
	
	// Clear
	template<typename List, bool = IsEmpty<List>::value>
	struct ClearT;

	template<typename List>
	using Clear = typename ClearT<List>::type;

	template<typename List>
	struct ClearT<List, false> : ClearT<PopFront<List>> { };

	template<typename List>
	struct ClearT<List, true> {
		using type = List;
	};

	// At
	template<typename List, size_t N>
	struct AtT : AtT<PopFront<List>, N - 1> { };

	template<typename List, size_t N>
	using At = typename AtT<List, N>::type;

	template<typename List>
	struct AtT<List, 0> {
		using type = Front<List>;
	};

	// Accumulate : list
	template<typename List, template <typename I, typename X> class Op, typename I, bool = IsEmpty<List>::value>
	struct AccumulateT;

	template<typename List, template <typename I, typename X> class Op, typename I>
	using Accumulate = typename AccumulateT<List, Op, I>::type;

	template<typename List, template <typename I, typename X> class Op, typename I>
	struct AccumulateT<List, Op, I, false> : AccumulateT<PopFront<List>, Op, typename Op<I, Front<List>>::type> { };

	template<typename List, template <typename X, typename Y> class Op, typename I>
	struct AccumulateT<List, Op, I, true> {
		using type = I;
	};

	// Accumulate : integer sequence
	template<typename List, template<typename I, typename List, size_t Num> class Op, typename I, size_t... Nums>
	struct AccumulateIST;

	template<typename List, template<typename I, typename List, size_t Num> class Op, typename I, size_t... Nums>
	using AccumulateIS = typename AccumulateIST<List, Op, I, Nums...>::type;

	template<typename List, template<typename I, typename List, size_t Num> class Op, typename I>
	struct AccumulateIST<List, Op, I> {
		using type = I;
	};

	template<typename List, template<typename I, typename List, size_t Num> class Op, typename I, size_t NumHead, size_t... NumTail>
	struct AccumulateIST<List, Op, I, NumHead, NumTail...> : AccumulateIST<List, Op, typename Op<I, List, NumHead>::type, NumTail...> { };

	// Reverse
	template<typename List>
	using ReverseT = AccumulateT<List, PushFrontT, Clear<List>>;
	template<typename List>
	using Reverse = typename ReverseT<List>::type;

	// PushBack
	template<typename List, typename T>
	using PushBackT = ReverseT<PushFront<Reverse<List>, T>>;
	template<typename List, typename T>
	using PushBack = typename PushBackT<List, T>::type;

	// Transform
	template<typename List, template<typename T> class Op>
	struct TransformT {
	private:
		template<typename I, typename T>
		using ToIT = PushFrontT<I, typename Op<T>::type>;
	public:
		using type = Reverse<Accumulate<List, ToIT, Clear<List>>>;
	};

	template<typename List, template<typename> class Op>
	using Transform = typename TransformT<List, Op>::type;

	// Select
	template<typename List, size_t... Indices>
	struct SelectT {
	private:
		template<typename I, typename List, size_t Index>
		using ToIT = PushFrontT<I, At<List, Index>>;
	public:
		using type = Reverse<AccumulateIS<List, ToIT, Clear<List>, Indices...>>;
	};
	template<typename List, size_t... Indices>
	using Select = typename SelectT<List, Indices...>::type;
}

#endif // !_UBPA_TEMPLATE_LIST_LIST_H_

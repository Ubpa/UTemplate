#include <iostream>
#include <type_traits>

using namespace std;

template<typename... Ts>
struct Typelist {
	static constexpr size_t length = sizeof...(Ts);
	static constexpr bool empty = length == 0;
};

template<typename List>
struct HeadT;
template<typename List>
using Head = typename HeadT<List>::type;
template<typename Head, typename... Tail>
struct HeadT<Typelist<Head, Tail...>> {
	using type = Head;
};

template<typename List, typename T>
struct PushFrontT;
template<typename List, typename T>
using PushFront = typename PushFrontT<List, T>::type;
template<typename T, typename... Ts>
struct PushFrontT<Typelist<Ts...>, T> {
	using type = Typelist<T, Ts...>;
};

template<typename List>
struct PopFrontT;
template<typename List>
using PopFront = typename PopFrontT<List>::type;
template<typename Head, typename... Tail>
struct PopFrontT<Typelist<Head, Tail...>> {
	using type = Typelist<Tail...>;
};

template<typename List>
using Tail = PopFront<List>;

template<typename List, typename T>
struct PushBackT;
template<typename List, typename T>
using PushBack = typename PushBackT<List, T>::type;
template<typename T, typename... Ts>
struct PushBackT<Typelist<Ts...>, T> {
	using type = Typelist<Ts..., T>;
};

/*
template<typename List>
struct ReverseT;
template<typename List>
using Reverse = typename ReverseT<List>::type;
template<>
struct ReverseT<Typelist<>> {
	using type = Typelist<>;
};
template<typename Head, typename... Tail>
struct ReverseT<Typelist<Head, Tail...>> {
	using type = PushBack<Reverse<Typelist<Tail...>>, Head>;
};
*/

template<typename List, unsigned N>
struct IndexT : IndexT<PopFront<List>, N-1> { };
template<typename List, unsigned N>
using Index = typename IndexT<List, N>::type;
template<typename List>
struct IndexT<List, 0> {
	using type = Head<List>;
};

template<typename List, unsigned... Indices>
struct SelectT {
	using type = Typelist<Index<List, Indices>...>;
};
template<typename List, unsigned... Indices>
using Select = typename SelectT<List, Indices...>::type;

/*
template<typename List, template<typename> class Op>
struct TransformT {
	using type = PushFront<typename TransformT<Tail<List>, Op>::type, typename Op<Head<List>>::type>;
};
template<typename List, template<typename> class Op>
using Transform = typename TransformT<List, Op>::type;
template<template<typename> class Op>
struct TransformT<Typelist<>, Op> {
	using type = Typelist<>;
};
*/
template<typename List, template<typename> class Op>
struct TransformT;
template<typename List, template<typename> class Op>
using Transform = typename TransformT<List, Op>::type;
template<template<typename> class Op, typename... Ts>
struct TransformT<Typelist<Ts...>, Op> {
	using type = Typelist<typename Op<Ts>::type...>;
};

template<typename List, template<typename I, typename X> class Op, typename I>
struct AccumulateT {
	using type = typename AccumulateT<Tail<List>, Op, typename Op<I, Head<List>>::type>::type;
};
template<typename List, template<typename I, typename X> class Op, typename I>
using Accumulate = typename AccumulateT<List, Op, I>::type;
template<template<typename I, typename X> class Op, typename I>
struct AccumulateT<Typelist<>, Op, I> {
	using type = I;
};

template<typename List>
using Reverse = Accumulate<List, PushFrontT, Typelist<>>;

template<typename List>
struct BackT;
template<typename List>
using Back = typename BackT<List>::type;
template<typename... Ts>
struct BackT<Typelist<Ts...>> {
	using type = Head<Reverse<Typelist<Ts...>>>;
};

template<typename List>
struct PopBackT;
template<typename List>
using PopBack = typename PopBackT<List>::type;
template<typename... Ts>
struct PopBackT<Typelist<Ts...>> {
	using type = Reverse<Tail<Reverse<Typelist<Ts...>>>>;
};

// --------------

template<typename T>
struct NameTraits { static constexpr const char * text = "ERROR"; };
template<>
struct NameTraits<void> { static constexpr const char * text = "void"; };
template<>
struct NameTraits<bool> { static constexpr const char * text = "bool"; };
template<>
struct NameTraits<char> { static constexpr const char * text = "char"; };
template<>
struct NameTraits<short> { static constexpr const char * text = "short"; };
template<>
struct NameTraits<int> { static constexpr const char * text = "int"; };
template<>
struct NameTraits<long> { static constexpr const char * text = "long"; };
template<>
struct NameTraits<long long> { static constexpr const char * text = "long long"; };
template<>
struct NameTraits<float> { static constexpr const char * text = "float"; };
template<>
struct NameTraits<double> { static constexpr const char * text = "double"; };
template<>
struct NameTraits<nullptr_t> { static constexpr const char * text = "nullptr_t"; };

template<typename T, bool isUnsigned = std::is_unsigned<T>::value && !std::is_same<T, bool>::value>
struct BasicName;

template<typename T>
struct BasicName<T, true>{
	friend ostream & operator<<(ostream& os, BasicName<T, true> n){
		os << "unsigned " << NameTraits<typename std::make_signed<T>::type>::text;
		return os;
	}
};

template<typename T>
struct BasicName<T, false>{
	friend ostream & operator<<(ostream& os, BasicName<T, false> n){
		os << NameTraits<T>::text;
		return os;
	}
};

template<typename T>
struct Name {
	friend ostream & operator<<(ostream& os, Name<T> n){
		os << BasicName<T>();
		return os;
	}
};

template<typename T>
struct Name<const T> {
	friend ostream & operator<<(ostream& os, Name<const T> n){
		os << "const " << Name<typename std::remove_const<T>::type>();
		return os;
	}
};

template<typename T>
struct Name<volatile T> {
	friend ostream & operator<<(ostream& os, Name<volatile T> n){
		os << "volatile " << Name<typename std::remove_volatile<T>::type>();
		return os;
	}
};

template<typename T>
struct Name<T&> {
	friend ostream & operator<<(ostream& os, Name<T&> n){
		os << Name<typename std::remove_reference<T>::type>() << "&";
		return os;
	}
};

template<typename T>
struct Name<T*> {
	friend ostream & operator<<(ostream& os, Name<T*> n){
		os << Name<typename std::remove_pointer<T>::type>() << "*";
		return os;
	}
};

template<typename T>
struct Name<T&&> {
	friend ostream & operator<<(ostream& os, Name<T&&> n){
		os << NameTraits<typename std::remove_reference<T>::type>::text << "&&";
		return os;
	}
};

template<>
struct Name<Typelist<>> {
	friend ostream & operator<<(ostream& os, Name<Typelist<>> n){
		return os;
	}
};

template<typename T>
struct Name<Typelist<T>> {
	friend ostream & operator<<(ostream& os, Name<Typelist<T>> n){
		os << Name<T>();
		return os;
	}
};

template<typename Head, typename... Tail>
struct Name<Typelist<Head, Tail...>> {
	friend ostream & operator<<(ostream& os, Name<Typelist<Head, Tail...>> n){
		os << Name<Head>() << ", " << Name<Typelist<Tail...>>();
		return os;
	}
};

template<typename RT, typename... Args>
struct Name<RT(Args...)> {
	friend ostream & operator<<(ostream& os, Name<RT(Args...)> n){
		os << "{" << Name<RT>() << "(" << Name<Typelist<Args...>>() << ")" << "}";
		return os;
	}
};

template<typename T, int N>
struct Name<T[N]> {
	friend ostream & operator<<(ostream& os, Name<T[N]> n){
		os << Name<T>() << "[" << N << "]";
		return os;
	}
};

template<typename T, int N>
struct Name<const T[N]> {
	friend ostream & operator<<(ostream& os, Name<const T[N]> n){
		os << "const " << Name<T>() << "[" << N << "]";
		return os;
	}
};

template<typename T>
struct Name<T[]> {
	friend ostream & operator<<(ostream& os, Name<T[]> n){
		os << Name<T>() << "[]";
		return os;
	}
};

template<typename T>
struct Name<const T[]> {
	friend ostream & operator<<(ostream& os, Name<const T[]> n){
		os << "const " << Name<T>() << "[]";
		return os;
	}
};

int main() {
	using list0 = Typelist<int, float, bool>;
	
	cout << "list0: " << Name<list0>() << endl;
	cout << "list0::length: " << list0::length << endl;
	cout << "list0::empty: " << list0::empty << endl;
	cout << "Head<list0>: " << Name<Head<list0>>() << endl;
	cout << "Tail<list0>: " << Name<Tail<list0>>() << endl;
	cout << "PushFront<list0, double>: " << Name<PushFront<list0, double>>() << endl;
	cout << "PopFront<list0>: " << Name<PopFront<list0>>() << endl;
	cout << "PushBack<list0, double>: " << Name<PushBack<list0, double>>() << endl;
	cout << "Reverse<list0>: " << Name<Reverse<list0>>() << endl;
	cout << "Back<list0>: " << Name<Back<list0>>() << endl;
	cout << "PopBack<list0>: " << Name<PopBack<list0>>() << endl;
	cout << "Index<list0, 1>: " << Name<Index<list0, 1>>() << endl;
	cout << "Transform<list0, add_const>: "
		<< Name<Transform<list0, add_const>>() << endl;
	cout << "Accumulate<list0, PushFrontT, Typelist<>>: "
		<< Name<Accumulate<list0, PushFrontT, Typelist<>>>() << endl;
	cout << "Select<list0, 0, 2>: "
		<< Name<Select<list0, 0, 2>>() << endl;
	
	cout << Name<void>() << endl;
	cout << Name<int>() << endl;
	cout << Name<unsigned int>() << endl;
	cout << Name<const int>() << endl;
	cout << Name<volatile int>() << endl;
	cout << Name<int &>() << endl;
	cout << Name<int &&>() << endl;
	cout << Name<int *>() << endl;
	cout << Name<const int * &>() << endl;
	cout << Name<Typelist<int, int>>() << endl;
	cout << Name<void()>() << endl;
	cout << Name<void(*)(int, int)>() << endl;
	cout << Name<int[2]>() << endl;
	cout << Name<const int[2]>() << endl;
	cout << Name<int[]>() << endl;
	cout << Name<const int[]>() << endl;
	cout << Name<void (* const [2])(int, int)>() << endl;
}

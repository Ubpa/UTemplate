#pragma once

#include <string_view>

namespace Ubpa {
	template<typename Char, Char... chars>
	struct TStr;
}

namespace Ubpa::details {
	template<int N, typename Char, int M>
	constexpr Char str_grab(const Char(&str)[M]) noexcept {
		if constexpr (N < M)
			return str[N];
		else
			return 0;
	}

	template<int N, typename Char>
	constexpr Char str_grab(std::basic_string_view<Char> str) noexcept {
		return N < str.size() ? str[N] : 0;
	}

	template<int N>
	constexpr char str_grab(std::basic_string_view<char> str) noexcept {
		return N < str.size() ? str[N] : 0;
	}

	template<int N>
	constexpr wchar_t str_grab(std::basic_string_view<wchar_t> str) noexcept {
		return N < str.size() ? str[N] : 0;
	}

	template<typename Str>
	struct ShrinkTStr;
	template<typename Str>
	using ShrinkTStr_t = typename ShrinkTStr<Str>::type;
	template<typename Rst, typename Str>
	struct ShrinkTStrImpl;
	template<bool IsZero, typename Rst, typename Str>
	struct ShrinkTStrImplAddHead;
	template<typename Rst, typename Str>
	struct ShrinkTStrImplAddHead<true, Rst, Str> { using type = Rst; };
	template<typename Char, Char head_char, Char... rst_chars, Char... chars>
	struct ShrinkTStrImplAddHead<false, TStr<Char, rst_chars...>, TStr<Char, head_char, chars...>>
		: ShrinkTStrImpl<TStr<Char, rst_chars..., head_char>, TStr<Char, chars...>> {};

	template<typename Rst, typename Char, Char head_char, Char... chars>
	struct ShrinkTStrImpl<Rst, TStr<Char, head_char, chars...>>
		: ShrinkTStrImplAddHead<head_char==0, Rst, TStr<Char, head_char, chars...>> {};

	template<typename Rst, typename Char>
	struct ShrinkTStrImpl<Rst, TStr<Char>> { using type = Rst; };

	template<typename Char, Char... chars>
	struct ShrinkTStr<TStr<Char, chars...>> : ShrinkTStrImpl<TStr<Char>, TStr<Char, chars...>> {};
}

/* 2^0 = 1 */
#define DECODE_TSTR_1(n,x) Ubpa::details::str_grab<0x##n##0>(x)

/* 2^1 = 2 */
#define DECODE_TSTR_2(n,x) Ubpa::details::str_grab<0x##n##0>(x), Ubpa::details::str_grab<0x##n##1>(x)

/* 2^2 = 2 */
#define DECODE_TSTR_4(n,x) \
Ubpa::details::str_grab<0x##n##0>(x), Ubpa::details::str_grab<0x##n##1>(x) \
, Ubpa::details::str_grab<0x##n##2>(x), Ubpa::details::str_grab<0x##n##3>(x)

/* 2^3 = 8 */
#define DECODE_TSTR_8(n,x) \
Ubpa::details::str_grab<0x##n##0>(x), Ubpa::details::str_grab<0x##n##1>(x) \
, Ubpa::details::str_grab<0x##n##2>(x), Ubpa::details::str_grab<0x##n##3>(x) \
, Ubpa::details::str_grab<0x##n##4>(x), Ubpa::details::str_grab<0x##n##5>(x) \
, Ubpa::details::str_grab<0x##n##6>(x), Ubpa::details::str_grab<0x##n##7>(x)

/* 2^4 = 16 */
#define DECODE_TSTR_16(n,x) \
Ubpa::details::str_grab<0x##n##0>(x), Ubpa::details::str_grab<0x##n##1>(x) \
, Ubpa::details::str_grab<0x##n##2>(x), Ubpa::details::str_grab<0x##n##3>(x) \
, Ubpa::details::str_grab<0x##n##4>(x), Ubpa::details::str_grab<0x##n##5>(x) \
, Ubpa::details::str_grab<0x##n##6>(x), Ubpa::details::str_grab<0x##n##7>(x) \
, Ubpa::details::str_grab<0x##n##8>(x), Ubpa::details::str_grab<0x##n##9>(x) \
, Ubpa::details::str_grab<0x##n##A>(x), Ubpa::details::str_grab<0x##n##B>(x) \
, Ubpa::details::str_grab<0x##n##C>(x), Ubpa::details::str_grab<0x##n##D>(x) \
, Ubpa::details::str_grab<0x##n##E>(x), Ubpa::details::str_grab<0x##n##F>(x)

/* 2^5 = 32 */
#define DECODE_TSTR_32(n,x) \
DECODE_TSTR_16(n##0,x),DECODE_TSTR_16(n##1,x)

/* 2^6 = 64 */
#define DECODE_TSTR_64(n,x) \
DECODE_TSTR_16(n##0,x), DECODE_TSTR_16(n##1,x), DECODE_TSTR_16(n##2,x) \
, DECODE_TSTR_16(n##3,x)

/* 2^7 = 128 */
#define DECODE_TSTR_128(n,x) \
DECODE_TSTR_16(n##0,x), DECODE_TSTR_16(n##1,x), DECODE_TSTR_16(n##2,x) \
, DECODE_TSTR_16(n##3,x), DECODE_TSTR_16(n##4,x), DECODE_TSTR_16(n##5,x) \
, DECODE_TSTR_16(n##6,x), DECODE_TSTR_16(n##7,x)

/* 2^8 = 256 */
#define DECODE_TSTR_256(n,x) \
DECODE_TSTR_16(n##0,x), DECODE_TSTR_16(n##1,x), DECODE_TSTR_16(n##2,x) \
, DECODE_TSTR_16(n##3,x), DECODE_TSTR_16(n##4,x), DECODE_TSTR_16(n##5,x) \
, DECODE_TSTR_16(n##6,x), DECODE_TSTR_16(n##7,x), DECODE_TSTR_16(n##8,x) \
, DECODE_TSTR_16(n##9,x), DECODE_TSTR_16(n##A,x), DECODE_TSTR_16(n##B,x) \
, DECODE_TSTR_16(n##C,x), DECODE_TSTR_16(n##D,x), DECODE_TSTR_16(n##E,x) \
, DECODE_TSTR_16(n##F,x)

/* 2^9 = 512 */
#define DECODE_TSTR_512(n,x) \
DECODE_TSTR_256(n##0,x), DECODE_TSTR_256(n##1,x)

/* 2^10 = 1024 */
#define DECODE_TSTR_1024(n,x) \
DECODE_TSTR_256(n##0,x), DECODE_TSTR_256(n##1,x), DECODE_TSTR_256(n##2,x) \
, DECODE_TSTR_128(n##3,x), DECODE_TSTR_16(n##38,x), DECODE_TSTR_16(n##39,x) \
, DECODE_TSTR_16(n##3A,x), DECODE_TSTR_16(n##3B,x), DECODE_TSTR_16(n##3C,x) \
, DECODE_TSTR_16(n##3D,x), DECODE_TSTR_16(n##3E,x), DECODE_TSTR_16(n##3F,x)

#ifdef USE_DECODE_TSTR
#if USE_DECODE_TSTR == 0
#define DECODE_TSTR(x) \
Ubpa::details::ShrinkTStr_t<Ubpa::TStr<decltype(DECODE_TSTR_1(,x)), DECODE_TSTR_1(,x)>>
#elif USE_DECODE_TSTR == 1
#define DECODE_TSTR(x) \
Ubpa::details::ShrinkTStr_t<Ubpa::TStr<decltype(DECODE_TSTR_1(,x)), DECODE_TSTR_2(,x)>>
#elif USE_DECODE_TSTR == 2
#define DECODE_TSTR(x) \
Ubpa::details::ShrinkTStr_t<Ubpa::TStr<decltype(DECODE_TSTR_1(,x)), DECODE_TSTR_4(,x)>>
#elif USE_DECODE_TSTR == 3
#define DECODE_TSTR(x) \
Ubpa::details::ShrinkTStr_t<Ubpa::TStr<decltype(DECODE_TSTR_1(,x)), DECODE_TSTR_8(,x)>>
#elif USE_DECODE_TSTR == 4
#define DECODE_TSTR(x) \
Ubpa::details::ShrinkTStr_t<Ubpa::TStr<decltype(DECODE_TSTR_1(,x)), DECODE_TSTR_16(,x)>>
#elif USE_DECODE_TSTR == 5
#define DECODE_TSTR(x) \
Ubpa::details::ShrinkTStr_t<Ubpa::TStr<decltype(DECODE_TSTR_1(,x)), DECODE_TSTR_32(,x)>>
#elif USE_DECODE_TSTR == 6
#define DECODE_TSTR(x) \
Ubpa::details::ShrinkTStr_t<Ubpa::TStr<decltype(DECODE_TSTR_1(,x)), DECODE_TSTR_64(,x)>>
#elif USE_DECODE_TSTR == 7
#define DECODE_TSTR(x) \
Ubpa::details::ShrinkTStr_t<Ubpa::TStr<decltype(DECODE_TSTR_1(,x)), DECODE_TSTR_128(,x)>>
#elif USE_DECODE_TSTR == 8
#define DECODE_TSTR(x) \
Ubpa::details::ShrinkTStr_t<Ubpa::TStr<decltype(DECODE_TSTR_1(,x)), DECODE_TSTR_256(,x)>>
#elif USE_DECODE_TSTR == 9
#define DECODE_TSTR(x) \
Ubpa::details::ShrinkTStr_t<Ubpa::TStr<decltype(DECODE_TSTR_1(,x)), DECODE_TSTR_512(,x)>>
#elif USE_DECODE_TSTR == 10
#define DECODE_TSTR(x) \
Ubpa::details::ShrinkTStr_t<Ubpa::TStr<decltype(DECODE_TSTR_1(,x)), DECODE_TSTR_1024(,x)>>
#elif USE_DECODE_TSTR > 10
#warning !!!: custom TStr length exceeded allowed(1024) !!!
#warning !!!: all TStrs to default maximum typestring length of 64 !!!
#warning !!!: you can use - DUSE_DECODE_TSTR = <power of two> to set length !!!
#define DECODE_TSTR(x) \
Ubpa::details::ShrinkTStr_t<Ubpa::TStr<decltype(DECODE_TSTR_1(,x)), DECODE_TSTR_64(,x)>>
#elif USE_DECODE_TSTR < 0
#warning !!!: You used USE_DECODE_TSTR with a negative size specified !!!
#warning !!!: all TStrs to default maximum typestring length of 64 !!!
#warning !!!: you can use - DUSE_DECODE_TSTR = <power of two> to set length !!!
#define DECODE_TSTR(x) \
Ubpa::details::ShrinkTStr_t<Ubpa::TStr<decltype(DECODE_TSTR_1(,x)), DECODE_TSTR_64(,x)>>
#endif
#else
#define DECODE_TSTR(x) \
Ubpa::details::ShrinkTStr_t<Ubpa::TStr<decltype(DECODE_TSTR_1(,x)), DECODE_TSTR_64(,x)>>
#endif
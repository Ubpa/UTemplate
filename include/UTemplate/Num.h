#pragma once
#ifndef _UBPA_TEMPLATE_NUM_H_
#define _UBPA_TEMPLATE_NUM_H_

namespace Ubpa {
	template<typename T, T N>
	struct Num {
		using type = T;
		static constexpr unsigned value = N;
	};

	template<bool b>
	using Bool = Num<bool, b>;

	template<char c>
	using Char = Num<char, c>;

	template<short N>
	using Short = Num<short, N>;

	template<int N>
	using Int = Num<int, N>;

	template<long N>
	using Long = Num<long, N>;

	// std::size_t == decltype(sizeof(void*)
	template<decltype(sizeof(void*)) N>
	using Size = Num<decltype(sizeof(void*)), N>;
}

#endif // !_UBPA_TEMPLATE_NUM_H_

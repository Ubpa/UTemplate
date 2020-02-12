#pragma once
#ifndef _UBPA_TEMPLATE_NUM_H_
#define _UBPA_TEMPLATE_NUM_H_

namespace Ubpa {
	template<unsigned N>
	struct Num {
		static constexpr unsigned value = N;
		using type = unsigned;
	};
}

#endif // !_UBPA_TEMPLATE_NUM_H_

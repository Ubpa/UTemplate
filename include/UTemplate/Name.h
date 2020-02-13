#pragma once
#ifndef _UBPA_TEMPLATE_NAME_H_
#define _UBPA_TEMPLATE_NAME_H_

#include <iostream>

namespace Ubpa {
	template<typename T>
	struct NameTraits { static constexpr const char* text = "ERROR"; };
	template<>
	struct NameTraits<void> { static constexpr const char* text = "void"; };
	template<>
	struct NameTraits<bool> { static constexpr const char* text = "bool"; };
	template<>
	struct NameTraits<char> { static constexpr const char* text = "char"; };
	template<>
	struct NameTraits<short> { static constexpr const char* text = "short"; };
	template<>
	struct NameTraits<int> { static constexpr const char* text = "int"; };
	template<>
	struct NameTraits<long> { static constexpr const char* text = "long"; };
	template<>
	struct NameTraits<long long> { static constexpr const char* text = "long long"; };
	template<>
	struct NameTraits<float> { static constexpr const char* text = "float"; };
	template<>
	struct NameTraits<double> { static constexpr const char* text = "double"; };
	template<>
	struct NameTraits<std::nullptr_t> { static constexpr const char* text = "nullptr_t"; };

	template<typename T, bool isUnsigned = std::is_unsigned<T>::value && !std::is_same<T, bool>::value>
	struct BasicName;

	template<typename T>
	struct BasicName<T, true> {
		friend std::ostream& operator<<(std::ostream& os, BasicName<T, true>) {
			os << "unsigned " << NameTraits<typename std::make_signed<T>::type>::text;
			return os;
		}
	};

	template<>
	struct BasicName<decltype(sizeof(void*)), true> {
		friend std::ostream& operator<<(std::ostream& os, BasicName<decltype(sizeof(void*)), true>) {
			os << "size_t";
			return os;
		}
	};

	template<typename T>
	struct BasicName<T, false> {
		friend std::ostream& operator<<(std::ostream& os, BasicName<T, false>) {
			os << NameTraits<T>::text;
			return os;
		}
	};

	template<typename... Ts>
	struct Name;

	template<typename T>
	struct Name<T> {
		friend std::ostream& operator<<(std::ostream& os, Name<T>) {
			os << BasicName<T>();
			return os;
		}
	};

	template<typename T>
	struct Name<const T> {
		friend std::ostream& operator<<(std::ostream& os, Name<const T>) {
			os << "const " << Name<typename std::remove_const<T>::type>();
			return os;
		}
	};

	template<typename T>
	struct Name<volatile T> {
		friend std::ostream& operator<<(std::ostream& os, Name<volatile T>) {
			os << "volatile " << Name<typename std::remove_volatile<T>::type>();
			return os;
		}
	};

	template<typename T>
	struct Name<T&> {
		friend std::ostream& operator<<(std::ostream& os, Name<T&>) {
			os << Name<typename std::remove_reference<T>::type>() << "&";
			return os;
		}
	};

	template<typename T>
	struct Name<T*> {
		friend std::ostream& operator<<(std::ostream& os, Name<T*>) {
			os << Name<typename std::remove_pointer<T>::type>() << "*";
			return os;
		}
	};

	template<typename T>
	struct Name<T&&> {
		friend std::ostream& operator<<(std::ostream& os, Name<T&&>) {
			os << NameTraits<typename std::remove_reference<T>::type>::text << "&&";
			return os;
		}
	};

	template<>
	struct Name<> {
		friend std::ostream& operator<<(std::ostream& os, Name<>) {
			return os;
		}
	};

	template<typename Head, typename... Tail>
	struct Name<Head, Tail...> {
		friend std::ostream& operator<<(std::ostream& os, Name<Head, Tail...>) {
			os << Name<Head>() << ", " << Name<Tail...>();
			return os;
		}
	};

	template<typename RT, typename... Args>
	struct Name<RT(Args...)> {
		friend std::ostream& operator<<(std::ostream& os, Name<RT(Args...)>) {
			os << "{" << Name<RT>() << "(" << Name<Args...>() << ")" << "}";
			return os;
		}
	};

	template<typename T, int N>
	struct Name<T[N]> {
		friend std::ostream& operator<<(std::ostream& os, Name<T[N]>) {
			os << Name<T>() << "[" << N << "]";
			return os;
		}
	};

	template<typename T, int N>
	struct Name<const T[N]> {
		friend std::ostream& operator<<(std::ostream& os, Name<const T[N]>) {
			os << "const " << Name<T>() << "[" << N << "]";
			return os;
		}
	};

	template<typename T>
	struct Name<T[]> {
		friend std::ostream& operator<<(std::ostream& os, Name<T[]>) {
			os << Name<T>() << "[]";
			return os;
		}
	};

	template<typename T>
	struct Name<const T[]> {
		friend std::ostream& operator<<(std::ostream& os, Name<const T[]>) {
			os << "const " << Name<T>() << "[]";
			return os;
		}
	};
}

#endif // !_UBPA_TEMPLATE_NAME_H_

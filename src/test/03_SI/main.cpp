#include <iostream>
#include <string>
#include <type_traits>
#include <array>

#include <UTemplate/SI.h>

using namespace std;
using namespace Ubpa;

template<typename Base, typename Impl>
struct IA : Base { IA(float) {} };
template<typename Base, typename Impl>
struct IB : Base { using Base::Base; };
template<typename Base, typename Impl>
struct IC : Base { using Base::Base; };
template<typename Base, typename Impl>
struct ID : Base { using Base::Base; };
template<typename Base, typename Impl>
struct IE : Base { using Base::Base; };

InterfaceTraits_Register(IB, IA);
InterfaceTraits_Register(IC, IA);
InterfaceTraits_Register(ID, IB);
InterfaceTraits_Register(IE, IA);
CombineInterface(IF, IC, ID);

// MSVC: /d1reportSingleClassLayoutG

struct G;
ImplTraits_Register(G, IE, IF);
struct G : SI<G> {
	using SI<G>::SI;
};

struct A {};
int main() {
	static_assert(G::Contains<IA>());
	static_assert(G::Contains<IB>());
	static_assert(G::Contains<IC>());
	static_assert(G::Contains<ID>());
	static_assert(G::Contains<IE>());
	static_assert(G::Contains<IF>());

	SI_Contains_v<G, IA>;
	SI_Contains_v<int, IA>;

	G{ 1.f };
}

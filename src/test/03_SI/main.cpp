#include <iostream>
#include <string>
#include <type_traits>
#include <array>

#include <UTemplate/SI.h>

using namespace std;
using namespace Ubpa;

template<typename Base, typename Impl>
struct IA : Base {};
template<typename Base, typename Impl>
struct IB : Base {};
template<typename Base, typename Impl>
struct IC : Base {};
template<typename Base, typename Impl>
struct ID : Base {};
template<typename Base, typename Impl>
struct IE : Base {};
template<typename Base, typename Impl>
struct IF : Base {};

InterfaceTraits_Regist(IB, IA);
InterfaceTraits_Regist(IC, IA);
InterfaceTraits_Regist(ID, IB);
InterfaceTraits_Regist(IE, IA);
InterfaceTraits_Regist(IF, IC, ID);

// MSVC: /d1reportSingleClassLayoutG

struct G;
ImplTraits_Regist(G, IE, IF);
struct G : SI<G> {};

struct A {};
int main() {
	static_assert(G::IsContain<IA>());
	static_assert(G::IsContain<IB>());
	static_assert(G::IsContain<IC>());
	static_assert(G::IsContain<ID>());
	static_assert(G::IsContain<IE>());
	static_assert(G::IsContain<IF>());

	SI_IsContain_v<G, IA>;
	SI_IsContain_v<int, IA>;
}

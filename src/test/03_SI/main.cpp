#include <iostream>
#include <type_traits>

#include <UTemplate/SI.h>
//#include <UTemplate/List/List.h>
//#include <UTemplate/Num/Bool.h>

using namespace std;
using namespace Ubpa;

using EmptyTList = TemplateList<>;

template<typename Base, typename N>
struct INum : Base {
	static constexpr typename N::type num = N::value;
};

template<typename Base, typename N>
struct ISayNum : SI<TemplateList<INum>>::Ttype<Base, N> {
	void Say() {
		cout << "call Say" << endl;
		cout << static_cast<typename ISayNum::INum*>(this)->num << endl;
	}
};

struct Rst : SII<TemplateList<ISayNum, INum>>::Ttype<SI_Nil, Size<4>> {};
int main() {
	Rst rst;
	cout << rst.num << endl;
	rst.Say();
}

#include <iostream>
#include <string>

class Base {
  std::string s;

 public:
  Base() : s("기반") { std::cout << "기반 클래스" << std::endl; }

  // virtual 함수를 호출하는게 overhead떄문에 보통 함수를 호출하는 것보다 시간이 더 오래걸린다.
  // virtual의 경우 vtable(virtual table)을 생성하는데 함수가 호출될 떄 이 테이블에서
  // 상황에 맞는 함수가 선택된 이후 호출되기 떄문이다.
  virtual void incorrect() { std::cout << "기반 클래스 " << std::endl; }
};
class Derived : public Base {
  std::string s;

 public:
  Derived() : Base(), s("파생") {}

  // override 키워드를 이용해 실수로 override 되지 않는 경우를 막아준다.
  void incorrect() override { std::cout << "파생 클래스 " << std::endl; }
};
int main() {
  Base p;
  Derived c;

  Base* p_c = &c;
  Base* p_p = &p;

  std::cout << " == 실제 객체는 Base == " << std::endl;
  p_p->incorrect();

  // upcasting으로 base의 what이 불려져야 되나 virtual 키워드로 인해 
  // 컴파일 시에 어떤 함수가 실행될 지 정해지지 않고 런타임 시에 정해지는데 
  // 이러한 일을 가리켜서 동적 바인딩(dynamic binding) 이라고 불린다.
  // 반대로 static binding도 있는데 평소에 사용하는 함수와 같은 것이다.
  std::cout << " == 실제 객체는 Derived == " << std::endl;
  p_c->incorrect();
  return 0;
}
#include <iostream>
#include <string>

class Base {
  std::string s;

 public:
  Base() : s("기반") {  std::cout << "기반 클래스" <<  std::endl; }

  void what() {  std::cout << s <<  std::endl; }
};

// Derived와 Base는 is-a 관계이다. Derived is a Base
class Derived : public Base {
  std::string s;

 public:
  Derived() : Base(), s("파생") {
     std::cout << "파생 클래스" <<  std::endl;

    // Base 에서 what() 을 물려 받았으므로
    // Derived 에서 당연히 호출 가능하다
    what();
  }
  
  //overring Derived 의 what 함수가 Base 의 what 함수를 오버라이딩해 Base의 what이 아닌 Derived의 what이 호출된ㄷ.
  void what() {  std::cout << s <<  std::endl; }
};
int main() {
   std::cout << " === 기반 클래스 생성 ===" <<  std::endl;
  Base p;

   std::cout << " === 파생 클래스 생성 ===" <<  std::endl;
  Derived c;

  // is-a이므로 Base 포인터에 선언이 가능하지만 Base는 Derived의 Base부분에만 접근이 가능하고 
  // overring한 what함수에 접근할 수 없어 Base의 what이 불려진다.
  // upcasting이라고 불린다.
  // 이 반대의 경우는 불가능 한데 Base는 Derived의 overring한 what 함수가 없어서 불러올 수 없기 때문이다.
  // 다운캐스팅도 가능하나 권장되지 않고 오류 발생 확률이 높은데 이런 경우를 위해 dynamic_cast를 지원한다. (static_cast와 동일한 사용법)
  std::cout << "=== 포인터 버전 ===" << std::endl;
  Base* p_c = &c;
  p_c->what();


  return 0;
}
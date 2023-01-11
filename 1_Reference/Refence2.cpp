// 레퍼런스를 리턴하는 함수
#include <iostream>

class A {
  int x;

 public:
  A(int c) : x(c) {}

  int& access_x() { return x; } // x의 refence return
  int get_x() { return x; } // x의 값을 return
  void show_x() { std::cout << x << std::endl; }
};

int main() {
  A a(5);
  a.show_x();

  int& c = a.access_x(); // int& c = x와 같다. (x는 a의 멤버 변수 x이다.)
  c = 4; // 즉 c값을 바꾸면 x값을 바꾸는 것과 같다.
  a.show_x();

  int d = a.access_x(); // int d = x 이므로 값이 복사가 되 d는 또다른 독립 변수가 된다.
  d = 3;
  a.show_x();

  // 아래는 오류
  // 레퍼런스가 아닌 타입을 리턴하는 경우는 '값' 의 복사가 이루어지기 때문에 임시 객체가 생성되는데, 
  // 임시객체의 레퍼런스를 가질 수 없기 때문이다.
  // int& e = a.get_x(); // a 클래스의 x는 언젠가 소멸되는 변수라 refence로 만들 수 없다.
  // e = 2;
  // a.show_x();

  int f = a.get_x();
  f = 1;
  a.show_x();
}
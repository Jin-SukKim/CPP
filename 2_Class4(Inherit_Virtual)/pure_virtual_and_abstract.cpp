#include <iostream>

class Animal {
 public:
  Animal() {}
  virtual ~Animal() {}

  // pure virtual function으로 무엇을 하는지 정의되지 않은 함수이다. 뒤에 '=0;'을 붙여서 만든다.
  // 이 함수의 경우 반드시 override되어야 한다.
  // 이 함수는 본체가 없어 호출될 수 없다 그러므로 이러한 순수 가상 함수를 포함하고 있는 class는 객체를 생성할 수 없다.
  // (Animal a는 불가능)
  // 이러한 클래스는 상속받는 클래스를 만들어 객체를 생성(instant)해야 된다.
  // 이렇게 pure virtual function을 최소 한 개 이상 포함하고 있는 class를 abstract class(추상 클래스)라고 한다.
  virtual void speak() = 0;
};

class Dog : public Animal {
 public:
  Dog() : Animal() {}
  void speak() override { std::cout << "왈왈" << std::endl; }
};

class Cat : public Animal {
 public:
  Cat() : Animal() {}
  void speak() override { std::cout << "야옹야옹" << std::endl; }
};

int main() {
  // abstract 클래스는 객체 생성은 불가능 하지만 포인터로 생성은 가능하다.
  Animal* dog = new Dog();
  Animal* cat = new Cat();

  dog->speak();
  cat->speak();
}
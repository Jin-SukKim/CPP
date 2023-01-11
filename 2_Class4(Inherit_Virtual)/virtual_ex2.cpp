#include <iostream>

class Parent {
 public:
  Parent() { std::cout << "Parent 생성자 호출" << std::endl; }

  virtual void show() { std::cout << "Parent !" << std::endl; }

  // 소멸자도 virtual로 해야 되는 이유는 parent포인터로 child를 가르킨 후 다 끝날 떄
  // 부모 생성자 -> 자식 생성자 -> 자식 소멸자 -> 부모 소멸자 가 호출되야 하는데
  // virtual이 아닌 경우 부모 포인터는 자식 소멸자를 호출하지 않아 virtual로 해 모든 소멸자를 호출하게 해야된다.
  virtual ~Parent() { std::cout << "Parent 소멸자 호출" << std::endl; }
};
class Child : public Parent {
 public:
  Child() : Parent() { std::cout << "Child 생성자 호출" << std::endl; }

  void show() { std::cout << "Child !" << std::endl; }

  ~Child() { std::cout << "Child 소멸자 호출" << std::endl; }
};

// Pointer와 Reference둘다 가능하다.
void test(Parent& a) { a.show(); }


int main() {
  std::cout << "--- 평범한 Child 만들었을 때 ---" << std::endl;
  { 
    // 이 {} 를 빠져나가면 c 가 소멸된다.
    Child c; 
    test(c);
  }

  std::cout << "--- Parent 포인터로 Child 가리켰을 때 ---" << std::endl;
  {
    Parent b;
    test(b);
    Parent *p = new Child();
    
    delete p;
  }
}
#include <iostream>

class A {
 public:
  int a;

  A() { std::cout << "A 생성자 호출" << std::endl; }
};

class B {
 public:
  //int a;
  int b;

  B() { std::cout << "B 생성자 호출" << std::endl; }
};

// 다중 상속이 가능하다.
// 생성자 호출 순서는 base 클래스를 상속받는 순서에 관련이 있다.
class C : public A, public B {
 public:
  int c;

  C() : A(), B() { std::cout << "C 생성자 호출" << std::endl; }
};

int main() { 
    C c; 
    
    // 다중 상속시 주의해야할 점은 클래스 A, B 모두 int a를 가지고 있을 경우
    // c.a 는 A의 a인지 B의 a인지 컴파일러는 구분할 수 없어 오류가 생긴다.
    c.a = 3;
}

//  다이아몬드 상속(diamond inheritance)라 불리는 형태의 다중 상속도 주의해야된다.
//  밑의 상속은 그림으로 표현시 다이아몬드 모양이 나온다.
//  만약 Human이 name이란 변수를 가지고 있을시 HandsomeHumand과 SmartHuman은 Human을 상속받아 둘다 name이란 
//  같은 이름의 변수를 가지고 있게된다. 이 경우 Me에서 두 클래스 상속시 Human의 내용이 중복이 되는 문제가 발생한다.
/*
    class Human {
    // ...
    };
    class HandsomeHuman : public Human {
    // ...
    };
    class SmartHuman : public Human {
    // ...
    };
    class Me : public HandsomeHuman, public SmartHuman {
    // ...
    };
*/

// 이러한 경우의 해결방안은 Human을 가상으로 상속하면 된다.
/*
    class Human {
    public:
    // ...
    };
    class HandsomeHuman : public virtual Human {
    // ...
    };
    class SmartHuman : public virtual Human {
    // ...
    };
    class Me : public HandsomeHuman, public SmartHuman {
    // ...
    };
*/
// 위와 같은 형식으로 Human을 Virtual로 상속받으면 Me는 다중 상속 시에 컴파일러가 Human 한 번만 부르게 된다.
// 물론 Me에서 Human은 물론 HandsomeHuman과 SmartHuman의 생성자를 모두 호출해야 된다.

// 다중 상속은 여려가지 디자인 패턴이 있는데 크게 3가지 (bridge pattern(N + M), nested generalization(N x M), 다중 상속(N x M))이 있다.
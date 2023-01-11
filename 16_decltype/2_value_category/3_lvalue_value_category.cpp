/*
    lvalue
        이름을 가진 대부분의 객체들은 해당 객체의 주소값을 취할 수 있으므로 모두 lvalue이다.
        이 lvalue들은 주소값 연산자(&)를 통해 해당 식의 주소값을 알아 낼 수 있다.
        &++i나 &std::endl은 모두 올바른 작업이다.
        그리고 lvalue들은 lvalue reference를 초기화하는데 사용가능하다.

        ex) int 타입 변수 i
            int i;
            i;

            만약 다른 식 j가 있을 떄 구분이 가능하므로 정체를 알 수 있다.
            i 라는 식의 주소 값은 실제 변수 i의 주소값이 된다.
            i는 이동이 불가능하다. int&& x = i;는 컴파일이되지 않는다.
            따라서 i는 lvalue이다.

        종류
            1. 수, 함수의 이름, 어떤 타입의 데이터 멤버 (예컨대 std::endl, std::cin) 등등
            2. 좌측값 레퍼런스를 리턴하는 함수의 호출식. std::cout << 1 이나 ++it 같은 것들
            3. a = b, a += b, a *= b 같이 복합 대입 연산자 식들
            4. ++a, --a 같은 전위 증감 연산자 식들
            5. a.m, p->m 과 같이 멤버를 참조할 때. 이 때 m 은 enum 값이거나 static 이 아닌 멤버 함수인 경우 제외. 
            ex)
                class A {
                    int f();         // static 이 아닌 멤버 함수
                    static int g();  // static 인 멤버 함수
                }

                A a;
                a.g;  // <-- lvalue
                a.f;  // <-- lvalue 아님 (아래 나올 prvalue)

            6. a[n] 과 같은 배열 참조 식들
            7. 문자열 리터럴 "hi"
            등등
        
        주의점!
            void f(int&& a) {
                a;  // <-- ?
            }

            f(3);

            여기서 a는 rvalue reference가 타입이지만 value category는 lvalue를 가지고 있다.
            따라서 
            void f(int&& a) { std::cout << &a; }
            int main() { f(3); }
            은 컴파일이 가능하다.

            만약 a가 rvalue reference이니깐 a는 rvalue라고 생각했다면 type과 value category가 헷갈린 경우이다.
*/
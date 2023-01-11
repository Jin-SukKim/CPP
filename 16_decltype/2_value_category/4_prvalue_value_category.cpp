// prvalue
/*  
    prvalue는 정체를 알 수 없어 주소값을 취할 수 없다.
    따라서 &a++나 &42 등은 모두 오류이다.
    그리고 prvalue는 좌측에 올 수 없다.
    하지만 prvalue는 rvalue reference와 const lvalue reference를 초기화 하는데 사용할 수 있다.

    ex)
        int f() { return 10; }
        f();  // <-- ?

        f()는 주소값을 취할 수 없어 실체가 없다.
        하지만 rvalue reference에 붙을 수 있다.
        따라서 f()는 prvalue이다.

    prvalue 대표적인 것들
        1.  문자열 리터럴을 제외 한 모든 리터럴들. 42, true, nullptr 같은 애들
        2.  레퍼런스가 아닌 것을 리턴하는 함수의 호출식. 예를 들어서 str.substr(1, 2), str1 + str2
        3.  후위 증감 연산자 식. a++, a--
        4.  산술 연산자, 논리 연산자 식들. a + b, a && b, a < b 같은 것들을 말합니다. 
            물론, 이들은 연산자 오버로딩 된 경우들 말고 디폴트로 제공되는 것들을 말합니다.
        5.  주소값 연산자 식 &a
        6.  a.m, p->m 과 같이 멤버를 참조할 때. 이 때 m 은 enum 값이거나 static 이 아닌 멤버 함수여야함.
        7.  this
        8.  enum 값
        9.  람다식 []() { return 0;}; 과 같은 애들.
        등등
    
    ex) const lvalue reference와 rvalue를 초기화 할 수 있다. 
        const int& r = 42;
        int&& rr = 42;
        // int& rrr = 42; <-- 불가능

*/
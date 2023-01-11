// Literal
/*
    constexpr 함수의 제약조건중엔 literal 타입이 아닌 변수의 정의란 조건이 존재한다.
    Literal 타입은 쉽게 컴파일러가 컴파일 타임에 정의할 수 없는 타입이다.

    constexpr로 사용될 수도 있고 constexpr 함수내에서도 사용될 수 있는 타입들은 여러개가 존재한다.
        1. void 형
        2. 스칼라 타입 (char, int, bool, long, float, double) 등등
        3. 레퍼런스 타입
        4. 리터럴 타입의 배열(literal 타입: 변하지 않는 값, 1, 'c' 등등)
        또는 아래 조건들을 만족하는 타입
            디폴트 소멸자를 가지고 다음 중 하나를 만족하는 타입
                a) 람다 함수
                b) Arggregate 타입 (사용자 정의 생성자, 소멸자가 없으며 모든 데이터 멤버들이 public)
                   쉽게 말해 pair 같은 애들을 이야기한다
                c) constexpr 생성자를 가지며 복사 및 이동 생성자가 없음

    C++ 14부터는 constexpr 생성자를 지원해 literal 타입들을 직접 만들 수 있게 되었다.
*/
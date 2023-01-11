// xvalue
/*
    만약 value category가 lvalue와 rvalue로만 구분되면 lvalue로 분류되는
    식을 이동시킬 수 없는 문제가 있다.
    그래서 lvalue임에도 이동 시킬수 있는 식들도 생각해봐야 된다.

    C++에선 크게 rvalue reference를 return하는 함수의 호출식을 예로 들 수 있다.
    대표적으로 std::move 함수가 있다.
        
        template <class T>
        constexpr typename std::remove_reference<T>::type&& move(T&& t) noexcept;

        return 값이 rvalue reference이다.
        따라서 move를 호출한 식은 lvalue처럼 lvalue reference를 초기화하는데 사용할 수 있고,
        prvalue처럼 rvalue reference에 붙이거나 이동 생성자에 전달해서 이동 시킬 수 있다.
*/
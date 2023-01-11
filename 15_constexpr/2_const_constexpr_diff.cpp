// constexpr vs const
/*
    const로 정의된 상수들은 컴파일 타임에 굳이 그 값을 알 필요가 없다.
    ex) 
        int a;
        const int b = a;
        이 코드는 b의 값을 컴파일 타임에는 알 수 없지만 b 값을 지정해주면 바꿀 수 없다.

        반면에
        int a;
        constexpr int b = a;
        constexpr의 경우 반드시 오른쪽에 다른 상수식이 와야된다.
        그래서 위 코드는 컴파일 타임에 a가 뭐가 올지 모르므로 컴파일 오류가 발생한다.

    즉, constexpr은 항상 const이지만, const는 constexpr이 아니다.

    const는 만약 상수식으로 초기화되어도 컴파일러에 따라 컴파일 타임이나 런타임에 초기화될 수도 있따.

    만약 컴파일 타임에 상수를 확실하게 사용하고 싶다면 constexpr을 사용해야된다.
*/
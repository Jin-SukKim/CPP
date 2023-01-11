// initializer_list 와 auto
/*
    {}를 이용해 생성할 때 type으로 auto를 지정하면 initializer_list 객체가 생성된다.
    ex)
        auto list = {1, 2, 3};는 initializer_list<int>가 타입이 된다.

    auto a = {1};     // std::initializer_list<int>
    auto b{1};        // std::initializer_list<int>
    auto c = {1, 2};  // std::initializer_list<int>
    auto d{1, 2};     // std::initializer_list<int>

    위의 4가지 경우 b는 int로 추론되어야 할 것 가지만 initializer_list<int>로 추론된다.
    이는 꽤 비상식적인 경우로 C++ 17부터는 두 가지 형태로 auto타입이 추론된다.

    1.  auto x = {arg1, arg2...} 형태의 경우 arg1, arg2 ... 들이 
        모두 같은 타입이라면 x 는 std::initializer_list<T> 로 추론된다
    
    2.  auto x {arg1, arg2, ...} 형태의 경우 만일 인자가 단 1 개라면 인자의 타입으로 추론되고, 
        여러 개일 경우 오류를 발생시킵니다.
    
    ex) C++ 17부터
        auto a = {1};     // 첫 번째 형태이므로 std::initializer_list<int>
        auto b{1};        // 두 번째 형태 이므로 그냥 int
        auto c = {1, 2};  // 첫 번째 형태이므로 std::initializer_list<int>
        auto d{1, 2};  // 두 번째 형태 인데 인자가 2 개 이상이므로 컴파일 오류
    
    uniform initialization과 auto를 같이 사용할 때 주의할 점이 있다.
    문자열을 다룰 떄 auto list = {"a", "b", "cc"}; 를 할 경우
    list는 initializer_list<std::string>이 아닌 initializer_list<const *char>이 된다.

    이 문제는 C++ 14에서 추가된 literal 연산자를 통해 해결할 수 있다.

    using namespace std::literals;  // 문자열 리터럴 연산자를 사용하기 위해 추가해줘야함.
    auto list = {"a"s, "b"s, "c"s};

    이렇게 한다면 initializer_list<std::string>으로 추론된다.
*/
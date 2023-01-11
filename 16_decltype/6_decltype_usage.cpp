// decltype 쓰임새
/*
    auto로도 충분할 것 같은데 decltype이 사용되는 이유는 여러가지가 있다.
    auto는 정확한 타입을 표현하지 않는다.
    ex)
        int i = 4;
        auto j = i;  // int j = i;
        decltype(i) j = i;  // int j = i;
        는 같지만 auto는 정확한 타입을 표현하지 않는다.

        const int i = 4;
        auto j = i;         // int j = i;
        decltype(i) k = i;  // const int k = i;

    이 외에도 배열의 경우 auto는 암시적으로 포인터로 변환하지만 
    decltype의 경우 배열 타입 그대로를 전달할 수 있다.
    ex)
        int arr[10];
        auto arr2 = arr;     // int* arr2 = arr;
        decltype(arr) arr3;  // int arr3[10];

    즉 decltype을 이용해 타입 그대로를 전달할 수 있다.

    또 템플릿의 경우 객체의 타입이 인자들에 의해 결정되는 경우가 있다
    ex)
        template <typename T, typename U>
        void add(T t, U u, ??무슨타입?? result) {
            *result = t + u;
        }  
        add 함수는 t와 u를 더해서 result에 저장하는 함수이지만 result의 타입이
        t와 u에 의해서 결정된다. (t = double, u = int면 result는 double*가 된다.)

        이런 경우 dectlype을 이용해 해결한다.

        template <typename T, typename U>
        void add(T t, U u, decltype(t + u)* result) {
            *result = t + u;
        }

    하지만 만약 

        template <typename T, typename U>
        decltype(t + u) add(T t, U u) {
            return t + u;
        }
    와 같이 아예 decltype으로 함수의 타입을 결정하게 만든다면 컴파일 에러가 난다.
    t와 u의 정의가 decltype 나중에 나오기 때문에이다.
    함수의 return값을 인자들 정의 부분 뒤에 써야된다.

    C++ 14부터는 auto와 decltype 둘 다 사용해 구현이 가능하다.

        template <typename T, typename U>
        auto add(T t, U u) -> decltype(t + u) {
            return t + u;
        }

    함수 타입을 auto로 두고 -> 뒤에 함수의 실제 return 타입을 지정해주는 것이다.
    lambda 함수의 문법과 매우 유사하다.

*/
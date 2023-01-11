// if constexpr
/*
    타입에 따라 달라지는 함수를 짜고 싶다면 if constexpr과 템플릿을 사용하면 된다.
    ex) get_value 라는 함수가 있는데, 이 함수는 인자가 포인터 타입이면 *을 한 것을 return하고
        아니면 원래 인자를 return하는 함수이다.

        template <typename T>
        void show_value(T t) {
            std::cout << "포인터가 아니다 : " << t << std::endl;
        }

        template <typename T>
        void show_value(T* t) {
            std::cout << "포인터 이다 : " << *t << std::endl;
        }

        템플릿 타입 유추를 이용하면 비슷한 역할을 하는 코드를 위와 같이 작성할 수 있다.
        하지만 show_value 함수의 문제는 정확히 어떤 형태의 T를 요구하는지 한 눈에 알아보기 어렵고
        같은 함수를 두 번 써야된다는 단점이 있다.

    C++의 type_traits는 여러가지 함수 템플릿을 제공하는데 std::is_pointer 를 사용해 포인터인지 아닌지
    확인하는 함수가 있다.

    template <typename T>
    void show_value(T t) {
        // 전달된 인자 T가 포인터라면 value가 true가 되고 아니면 false를 return
        if (std::is_pointer<T>::value) {
            std::cout << "포인터 이다 : " << *t << std::endl;
        } else {
            std::cout << "포인터가 아니다 : " << t << std::endl;
        }
    }

    이 함수를 사용해 템플릿을 만들면 위와 같이 되는데 컴파일 오류를 발생시킨다.
    show_value(x)가 인스턴스화 되면 t는 int타입이 되는데 t에 *가 붙게된다.
    실행은 되지 않아도 컴파일에 포함되 에러가 나는데 if constexpr을 사용해 해결할 수 있다.

    if constexpr은 조건이 반드시 bool 타입으로 변환될 수 있어야 하는 컴파일 타임 상수식이여만 한다.
    대신, if constexpr이 true라면 else에 해당하는 부분은 컴파일 되지 않고 완전히 무시된다.
    False라면 else에 해당하는 부분만 컴파일되므로 에러가 나지 않는다.
*/
#include <iostream>
#include <type_traits>

template <typename T>
void show_value(T t)
{
    if constexpr (std::is_pointer<T>::value) // std:::is_pointer<t>::value대신 std::is_pointer<T>를 그냥 사용해도된다.
    {
        std::cout << "포인터 이다 : " << *t << std::endl;
    }
    else
    {
        std::cout << "포인터가 아니다 : " << t << std::endl;
    }
}

int main()
{
    int x = 3;
    show_value(x);

    int *p = &x;
    show_value(p);
}
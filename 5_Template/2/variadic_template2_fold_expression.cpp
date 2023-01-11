// Fold Expression
// C++ 11 에서 도입된 가변 길이 템플릿은 매우 편리하지만 한 가지 단점이 있어야 합니다.
// 재귀 함수 형태로 구성해야 하기 때문에, 반드시 재귀 호출 종료를 위한 함수를 따로 만들어야 한다는 것

// C++ 17 에 새로 도입된 Fold 형식을 사용한다면 이를 훨씬 간단하게 표현할 수 있습니다.

#include <iostream>

/*
Fold 방식의 종류로 아래 표와 같이 총 4 가지가 있습니다.
1. 단항 우측 Fold
2. 단항 좌측 Fold
3. 이항 우측 Fold
4. 이항 좌측 Fold
*/

// 단항 Fold
template <typename... Ints>
int sum_all(Ints... nums)
{
    // 단항 좌측 Fold (Unary left fold). Fold 식을 쓸 때 꼭 ()로 감싸줘야된다.
    return (... + nums); // Fold 형식 : return (... + nums)는 return ((((1 + 4) + 2) + 3) + 10);와 같다.
}

// 이항 Fold
template <typename Int, typename... Ints>
Int diff_from(Int start, Ints... nums)
{
    // 이항 좌측 Fold. 왜냐하면 start 가 초기값이고 nums 가 파라미터 팩 부분이기 때문이다.
    return (start - ... - nums); // return (((((100 - 1) - 4) - 2) - 3) - 10);와 같다.
}

// , 연산자를 사용하면 각각의 인자들에 대해 원하는 식을 실행할 수 있다.
class A
{
public:
    void do_something(int x) const
    {
        std::cout << "Do something with " << x << std::endl;
    }
};

template <typename T, typename... Ints>
void do_many_things(const T &t, Ints... nums)
{
    // 각각의 인자들에 대해 do_something 함수들을 호출한다.
    (t.do_something(nums), ...); //  사실상 모든 인자들에 대해서 각각 t.do_something(arg) 를 실행한 것과 같다.
}

int main()
{
    // 1 + 4 + 2 + 3 + 10
    std::cout << sum_all(1, 4, 2, 3, 10) << std::endl;

    // 100 - 1 - 4 - 2 - 3 - 10
    std::cout << diff_from(100, 1, 4, 2, 3, 10) << std::endl;

    A a;
    do_many_things(a, 1, 3, 2, 4);
}

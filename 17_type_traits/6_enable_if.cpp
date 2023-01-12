// SFINAE를 통해 오버로딩 후보군에서 제외를 쉽게 해주는 type_traits의 메타함수인 enable_if이다.
/*
    enable_if의 정의 :

        template<bool B, class T = void>
        struct enable_if {};

        template<class T>
        struct enable_if<true, T> { typedef T type; };

    B 부분에 확인하고픈 조건을 전달한다.
    만약 B가 true라면 enable_if::type의 타입이 T가 되고, false라면 enable_if의 type이 존재하지 않게 된다.

    ex) 어떤 함수의 인자 T가 정수일 때만 오버로딩하고 싶을 때
        std::enable_if<std::is_integral<T>::value>::type
        B 자리에 원하는 조건인 std::is_integral<T>::value를 전달

        std::integral<T>::value가 true 일때 std::enable_if에 value가 정의되어 컴파일 오류를 발생시키지 않는다.
        false라면 type이 존재하지 않게되 가능한 오버로딩이 없다고 컴파일 에러가 난다.

    C++ 14부터는 enable_if<>::type과 같이 자주 쓰이는 패턴에 대한 alias들을 활용해
    typename std::enable_if<>::type과 같이 긴 표현을 간단하게 표현할 수 있게 해준다.
    ex)
        template <typename T,
          typename = typename std::enable_if<std::is_integral<T>::value>::type>
        void test(const T &t)
        {
            std::cout << "t : " << t << std::endl;
        }

        위 코드를

        template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
        void test(const T& t) {
            std::cout << "t : " << t << std::endl;
        }

        와 같이 표현할 수 있다.


        alias는 아래와 같이 정의되어 있다.
        template <bool B, class T = void>
        using enable_if_t = typename enable_if<B, T>::type;  // C++ 14 부터 사용 가능

        template <class T>
        inline constexpr bool is_integral_v =
            is_integral<T>::value;  // C++ 17 부터 사용 가능.


*/
#include <iostream>
#include <type_traits>

/*
    typename = 은 템플릿에 default 인자를 전달하는 부분이다.
    원래는 typename u = 처럼 템플릿 인자를 받지만 지금은 식 자체만 필요하므로 굳이 인자를 정의하지 않았다.
    std::enable_if 앞에 typename이 붙은 이유는 std::enable_if<>::type이 의존 타입이기 때문이다.
*/
template <typename T,
          typename = typename std::enable_if<std::is_integral<T>::value>::type>
void test(const T &t)
{
    std::cout << "t : " << t << std::endl;
}

struct A
{
};

// enable_if 또 다른 예시
/*
    vector<int> v(10, 3); 경우 첫번쨰 생산자가 vector를 생성하기를 원했지만
    두번째 생산자인 반복자를 받는 생산자가 선택된다.

    문제는 첫번째 생산자가 size_t를 받는다는 것인데 부호가 없는 정수 타입을 받는다.
    10은 부호가 있는 정수 int라 컴팡일러는 unsigned_int로 임의로 캐스팅할 수 도 있지만
    더 나은 후보인 두 번째 생산자가 있어서 캐스팅을 안하고 호출한 것이다.
    Iterator를 그냥 int로 오버로딩 하면 되기 때문이다.

    따라서 iterator를 실제 반복자라고 강제할 필요가 있다.
    실제론 없지만 is_iterator라는 반복자를 확인하는 메타 함수가 있다고 한다면
    template <typename Iterator,
          typename = std::enable_if_t<is_iterator<Iterator>::value>>
    vector(Iterator start, Iterator end) {
        std::cout << "반복자를 이용한 생성자 호출" << std::endl;
    }
    처럼 사용하면 반복자일때만 들어가게 할 수 있다.
*/
template <typename T>
class vector
{
public:
    // num개가 들어있는 vector 만드는 생성자
    vector(size_t num, const T &element)
    {
        std::cout << element << " 를 " << num << " 개 만들기" << std::endl;
    }

    // 반복자 시작과 끝을 받는 생성자
    /* 
        반복자의 경우 클래스가 따로 정해져 있는 것이 아니라 그냥 start, end, ++ 등등
        함수만 들어있는 클래스라면 반복자처럼 사용가능하다.
    */
    template <typename Iterator>
    vector(Iterator start, Iterator end)
    {
        std::cout << "반복자를 이용한 생성자 호출" << std::endl;
    }
};

int main()
{
    test(1);     // int
    test(false); // bool
    test('c');   // char

    // 정수가 아닌 객체
    // test(A{}); // 컴파일 오류

    // 또 다른 예제
    vector<int> v(10, 3);
}
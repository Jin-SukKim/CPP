// c++은 여러 계산을 위해 많이 사용되는데 물리적 수치 계산에 필요한 것이 바로 '단위'이다.(ex: 인공위성 궤도, 입자운동 등등)
// 단위ex: 킬로그램 (kg), 미터 (m), 초 (s) 등등

// 이렇게 단위가 붙은 데이터를 처리할 때 중요한 점은 바로 데이터를 연산할 때 항상 단위를 확인해야 된다는 점이다.
/*  ex
float v1, v2;  // v1, v2 는 속도
std::cout << v1 + v2;
단위가 같기 떄문에 당연히 계산 가능하다.

반면
float v;        // 속도; m/s
float a;        // 가속도; m/s^2
std::cout << v + a;  // ???
이러한 경우 프로그래머의 실수이지만 컴퓨터는 float끼리 더한것 이므로 계산이 되지만 수치적으로 오류가 발생한 것과 같다.

실제로 NASA 의 경우 단위를 잘못 처리해서 1조원 짜리 화성 탐사선을 날려먹은 경우가 존재한다.

이를 위해 클래스를 만들어 클래스 객체에 보관할 수도 있고 operator+ 등으로 연산자 오버로딩을 통해 단위를 체크해 처리해도 된다.
하지만 틀린 단위를 연산하는 코드가 드물게 일어나는 경우 런타임에서 이런 문제를 발견하지 못하고 넘어갈 수도 있어서 문제가 된다.

가장 이상적인 상황은 단위가 맞지 않는 연산을 수행하는 코드가 있다면 아예 컴파일 시에 오류를 발생시켜 틀린 단위 연산을 막고
프로그램을 실행 시키면서 기다리는 수고도 덜 수 있다.
*/

// 이를 위한 example
// 물리량 데이터 단위를 표시하기 위한 struct (단순화)
/*
    속도 : Dim<0, 1, -1> => 속도 m/s = kg^0 x m^1 x s^-1
    힘   : Dim<1, 1, -2> => 힘 kg x m/s^2 = kg^1 x m^1 x s^-2 등등
*/
/*
물론 템플릿 인자로 타입을 받기 때문에 단순히 Dim<0, 1, -1> 이렇게 사용할 수 있는 것이 아닙니다.
대신에 앞서 만들었던Ratio 클래스를 이용해서 저 숫자들을 '타입' 으로 표현해주어야 합니다.

실제로는 Dim<0, 1, -1>가 아닌 Dim<Ratio<1, 1>, Ratio<1, 1>, Ratio<-2, 1>>처럼 사용해야된다
*/
#include <iostream>
#include "5_template/3/using.cpp"

template <typename U, typename V, typename W>
struct Dim
{
    using M = U; // kg
    using L = V; // m
    using T = W; // s

    using type = Dim<M, L, T>;
};

/*
typename이 많이 붙어있는 이유는 M의 경우 U에 의존한 타임(dependent type)이고
다른 타입들도 U와 V에 의존하는 타입이기 떄문이다.
*/
template <typename U, typename V>
struct add_dim_
{
    typedef Dim<typename Ratio_add<typename U::M, typename V::M>::type,
                typename Ratio_add<typename U::L, typename V::L>::type,
                typename Ratio_add<typename U::T, typename V::T>::type>
        type;
};

template <typename U, typename V>
struct subtract_dim_
{
    typedef Dim<typename Ratio_subtract<typename U::M, typename V::M>::type,
                typename Ratio_subtract<typename U::L, typename V::L>::type,
                typename Ratio_subtract<typename U::T, typename V::T>::type>
        type;
};

// 실제로 데이터를 담는 구조
template <typename T, typename D>
struct quantity
{
    T q;                // 데이터
    using dim_type = D; // 차원 정보, 데이터와는 다르게 'Dim 타입' 그자체로 표현

    quantity(T q) : q(q) {}

    // 두 개의 데이터를 더하거나 빼기 위해서는 반드시 단위가 일치해야 합니다.
    // 이 말은, dim_type 이 같은 타입이어야만 하다는 것이지요. 즉 단위가 다르면 오류 발생
    // +, - 오버로딩
    quantity operator+(quantity<T, D> quant) { return quantity<T, D>(q + quant.q); }
    quantity operator-(quantity<T, D> quant) { return quantity<T, D>(q - quant.q); }

    // *, /는 굳이 Dim이 같지 않아도 된다. 다만 연산 수행 시 새로운 단위가 나온다(속도 등)
    template <typename D2>
    quantity<T, typename add_dim_<D, D2>::type> operator*(quantity<T, D2> quant)
    {
        return quantity<T, typename add_dim_<D, D2>::type>(q * quant.q);
    }

    template <typename D2>
    quantity<T, typename subtract_dim_<D, D2>::type> operator/(
        quantity<T, D2> quant)
    {
        return quantity<T, typename subtract_dim_<D, D2>::type>(q / quant.q);
    }

    // 차원이 없는 값도 계산 (ex: 3 * kg)
    quantity<T, D> operator*(T scalar) { return quantity<T, D>(q * scalar); }
    quantity<T, D> operator/(T scalar) { return quantity<T, D>(q / scalar); }
};

int main()
{
    using one = Ratio<1, 1>;
    using zero = Ratio<0, 1>;

    quantity<double, Dim<one, zero, zero>> kg(1);
    quantity<double, Dim<zero, one, zero>> meter(1);
    quantity<double, Dim<zero, zero, one>> second(1);

    // Good
    kg + kg;

    // Bad, kg 와 meter 의 단위가 다르기 때문에 발생
    kg + meter;

    /*
    // F 의 타입은?
    F = kg * meter / (second * second); //  F 의 차원은 계산해보면 (1, 1, -2)이 나온다
    // F 의 dim 타입은 <Ratio<1, 1>, Ratio<1, 1>, Ratio<-2, 1>> 가 된다.

    // 즉 quantity<double, Dim<one, one, Ratio<-2, 1>>> F = kg * meter / (second * second);로 나타낼 수 있따.
    
    매번 변수를 정의할 때 마다 저렇게 길고 긴 타입을 써주는 것은 매우 귀찮은 일이다.
    새로 계산 해서 나오는 단위는 컴파일러도 알고 있는데 굳이 써줘야 할 필요없이 컴파일러가 알아서 타입을 
    생각하도록 할 수 있는데 '- auto' 키워드를 사용하면 된다.
    */
}

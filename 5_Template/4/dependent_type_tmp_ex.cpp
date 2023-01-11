#include <iostream>

// 소수(prime)판별하기

/*
// prime 판별 함수
bool is_prime(int N) {
  if (N == 2) return true;
  if (N == 3) return true;

  for (int i = 2; i <= N / 2; i++) {
    if (N % i == 0) return false;
  }

  return true;
}
*/


/*
// TMP
template <>
struct is_prime<2> {
  static const bool result = true;
};

template <>
struct is_prime<3> {
  static const bool result = true;
};

template <int N>
struct is_prime {
  static const bool result = !check_div<N, 2>::result;
};

template <int N, int d>
struct check_div {
  static const bool result = (N % d == 0) || check_div<N, d + 1>::result;
};

// 오류부분
// 템플릿 부분 특수화 시에 반드시 다른 연산자가 붙지 않고 단순한 식별자만 입력해주어야만 합니다.
template <int N>
struct check_div<N, N / 2> {
  static const bool result = (N % (N / 2) == 0);
};

*/


// N을 int인자가 아닌 타입으로 구현하고 N / 2 역시 직접 계산이 아닌 N / 2를 나타내는 타입으로 대체
template <int n>
struct INT
{
    static const int num = n; 
};

template<typename n, typename m>
struct add
{
    using result = INT<n::num + m::num>;
};

template<typename n, typename m>
struct divide
{
    using result = INT<n::num / m::num>;
};

using one = INT<1>;
using two = INT<2>;
using three = INT<3>;

template <typename N, typename d>
struct check_div {
  // result 중에서 한 개라도 true 면 전체가 true
  static const bool result =
    (N::num % d::num == 0) || check_div<N, typename add<d, one>::result>::result;
};

template <typename N>
struct _is_prime {
  static const bool result = !check_div<N, two>::result;
};

template <>
struct is_prime<two> {
  static const bool result = true;
};

template <>
struct is_prime<three> {
  static const bool result = true;
};

// 템플릿 인자에 따라서 어떠한 타입이 달라질 수 있는 것을 의존 타입(dependent type) 이라고 부릅니다.
// result 는 N 에 의존하기 때문에 의존 타입
template <typename N>
struct check_div<N, typename divide<N, two>::result> {
  static const bool result = (N::num % (N::num / 2) == 0);
};

// is_prime<INT<11>>::result을 is_prime<num>::result로 사용하기
template <int N>
struct is_prime {
  static const bool result = _is_prime<INT<N>>::result;
};

int main()
{
  std::cout << std::boolalpha;
  std::cout << "Is prime ? :: " << is_prime<2>::result << std::endl; // true
  std::cout << "Is prime ? :: " << is_prime<10>::result << std::endl; // false
  std::cout << "Is prime ? :: " << is_prime<11>::result << std::endl; // true
  std::cout << "Is prime ? :: " << is_prime<61>::result << std::endl; // true
}
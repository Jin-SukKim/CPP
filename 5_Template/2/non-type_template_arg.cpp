// template인자로 타입만 받을 수 있는게 아니다.
// 밑에는 type t와 동시에 int을 함수와 같이 인자로 받는다.
/*

한 가지 중요한 점은 템플릿 인자로 전달할 수 있는 타입들이 아래와 같이 제한적입니다.

C++ 20 부터 이 제한이 좀 더 완화되었습니다.

1.정수 타입들 (bool, char, int, long 등등). 당연히 float 과 double 은 제외

2. 포인터 타입

3. enum 타입

4. std::nullptr_t (널 포인터)

타입이 아닌 템플릿 인자를 가장 많이 활용하는 예시는 컴파일 타임에 값들이 정해져야 하는 것들이 되겠습니다.
(Template은 컴파일 도중에 인스턴스화 되기 때문에 이것을 이용하는 것이다.)
대표적인 예시로 배열을 들 수 있겠지요.
C 에서의 배열의 가장 큰 문제점은 함수에 배열을 전달할 때 배열의 크기에 대한 정보를 잃어버린다는 점입니다.
하지만 템플릿 인자로 배열의 크기를 명시한다면 (어차피 배열의 크기는 컴파일 타임에 정해지는 것이니까), 이 문제를 완벽하게 해결 할 수 있습니다.
(std::array를 통해 사용한다.)
*/
#include <iostream>
#include <array>

template <typename T, int num>
T add_num(T t)
{
    return t + num;
}

int main()
{
    int x = 3;
    std::cout << "x : " << add_num<int, 5>(x) << std::endl;

    /*
       고정 배열(fixed array)과 동적 배열(dynamic array),두 가지 배열 모두 C++에 내장되어 있지만,
       포인터로 형 변환되었을 시 배열 길이 정보가 손실되고, 동적 배열은 지저분한 할당 해제 문제가 있다.
       이러한 문제를 해결하기 위해 C++ 표준라이브러리는 배열 관리를 쉽게 해주는 std::array 와 std::vector가 있다.
    */
    // 마치 C 에서의 배열 처럼 {} 을 통해 배열을 정의할 수 있다.
    // {} 문법은 16 - 1 강에서 자세히 다루므로 여기서는 그냥 이렇게
    // 쓰면 되는구나 하고 이해하고 넘어가면 됩니다.
    std::array<int, 5> arr = {1, 2, 3, 4, 5};
    // int arr[5] = {1, 2, 3, 4, 5}; 와 동일

    for (int i = 0; i < arr.size(); i++)
    {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}
/*
ex2 :

#include <iostream>
#include <array>

// 인자가 무조건 길이가 5인 int std::array를 받아야된다.
void print_array(const std::array<int, 5>& arr) {
  for (int i = 0; i < arr.size(); i++) {
    std::cout << arr[i] << " ";
  }
  std::cout << std::endl;
}

int main() {
  std::array<int, 5> arr = {1, 2, 3, 4, 5};

  print_array(arr);
}
*/

/*
ex3 : ex2의 개선

#include <iostream>
#include <array>

template <typename T>
void print_array(const T& arr) {
  for (int i = 0; i < arr.size(); i++) {
    std::cout << arr[i] << " ";
  }
  std::cout << std::endl;
}

int main() {
  std::array<int, 5> arr = {1, 2, 3, 4, 5};
  std::array<int, 7> arr2 = {1, 2, 3, 4, 5, 6, 7};
  std::array<int, 3> arr3 = {1, 2, 3};

  print_array(arr);
  print_array(arr2);
  print_array(arr3);
}
*/
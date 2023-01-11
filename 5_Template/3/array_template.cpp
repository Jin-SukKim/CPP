// 템플릿 인자로는 타입 뿐만이 아니라 특정한 조건을 만족하는 값들도 올 수 있습니다.

/* 나만의 std::array 구현 */
#include <iostream>

template <typename T, unsigned int N>
class Array {
  T data[N];

 public:
  // 배열을 받는 레퍼런스 arr
  Array(T (&arr)[N]) {
    for (int i = 0; i < N; i++) {
      data[i] = arr[i];
    }
  }

  T* get_array() { return data; }

  unsigned int size() { return N; }

  void print_all() {
    for (int i = 0; i < N; i++) {
      std::cout << data[i] << ", ";
    }
    std::cout << std::endl;
  }
};

int main() {
  int arr[3] = {1, 2, 3};

  // 배열 wrapper 클래스
  Array<int, 3> arr_w(arr);

  arr_w.print_all();
}

/*
Array<int, 5> Array<int, 3>는 다른 클래스이다.

std::cout << (typeid(Array<int, 3>) == typeid(Array<int, 5>)) << std::endl; //  다른 클래스인지 확인하는 법

다른 템플릿으로 인스턴스됬기 때문이다.
*/


/* example : 
#include <iostream>
#include <typeinfo>

template <int N>
struct Int {
  // static 타입 멤버의 특성 상, 이 클래스가 생성한 객체들 사이에서 공유되는 값
  // const는 나타내는 값이 변하지 않게 한다.
  static const int num = N;
};

template <typename T, typename U>
struct add {
  typedef Int<T::num + U::num> result;
};

int main() {
  typedef Int<1> one;
  typedef Int<2> two;

  typedef add<one, two>::result three;
 
  // 덧셈이 수행 되는 시기는 컴파일 타임이고, 런타임 시에는 단순히 그 결과를 보여주게 되는 것
  std::cout << "Addtion result : " << three::num << std::endl;
}
*/
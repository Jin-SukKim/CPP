
#include <iostream>

// 함수에 디폴트 인자를 지정할 수 있는 것처럼 템플릿도 디폴트 인자를 지정할 수 있습니다.
template <typename T, int num = 5>
T add_num(T t) {
  return t + num;
}

template <typename T>
struct Compare {
  bool operator()(const T& a, const T& b) const { return a < b; }
};

template <typename T, typename Comp>
T Min(T a, T b) {
  Comp comp;
  if (comp(a, b)) {
    return a;
  }
  return b;
}

int main() {
  int x = 3;
  std::cout << "x : " << add_num(x) << std::endl;

  int a = 3, b = 4;
  std::cout << "min : " << Min<int, Compare<int>>(a, b);
}

// 타입을 defualt로 지정하는 법.
/*

#include <iostream>
#include <string>

template <typename T>
struct Compare {
  bool operator()(const T& a, const T& b) const { return a < b; }
};

template <typename T, typename Comp = Compare<T>>
T Min(T a, T b) {
  Comp comp;
  if (comp(a, b)) {
    return a;
  }
  return b;
}

int main() {
  int a = 3, b = 5;
  std::cout << "Min " << a << " , " << b << " :: " << Min(a, b) << std::endl;

  std::string s1 = "abc", s2 = "def";
  std::cout << "Min " << s1 << " , " << s2 << " :: " << Min(s1, s2)
            << std::endl;
}
*/
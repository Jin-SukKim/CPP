// 한 객체를 이용해 다양한 Type의 변수를 이용하고 싶을 때 사용(data가 string인지 char 상관없이 다 저장가능하다.)
// 템플릿 첫 활용
#include <iostream>
#include <string>

// 즉 사용자가 원하는 타입을 넣어주면 알아서 타입을 변환시켜주는 틀이다.
template <typename T> // template의 정의, 템플릿 인자로 T를 받고 T는 어떠한 타입의 이름이다.
class Vector          // template 밑에 class가 오므로 class의 template이지만 함수의 template도 가능하다.
{
    T *data;
    int capacity;
    int length;

public:
    // 생성자
    Vector(int n = 1) : data(new T[n]), capacity(n), length(0) {}

    // 맨 뒤에 새로운 원소를 추가한다.
    void push_back(T s)
    {
        if (capacity <= length)
        {
            T *temp = new T[capacity * 2];
            for (int i = 0; i < length; i++)
            {
                temp[i] = data[i];
            }
            delete[] data;
            data = temp;
            capacity *= 2;
        }

        data[length] = s;
        length++;
    }

    // 임의의 위치의 원소에 접근한다.
    T operator[](int i) { return data[i]; }

    // x 번째 위치한 원소를 제거한다.
    void remove(int x)
    {
        for (int i = x + 1; i < length; i++)
        {
            data[i - 1] = data[i];
        }
        length--;
    }

    // 현재 벡터의 크기를 구한다.
    int size() { return length; }

    ~Vector()
    {
        if (data)
        {
            delete[] data;
        }
    }
};

// 클래스 템플릿에 인자를 전달해서 실제 코드를 생성하는 것을 클래스 템플릿 인스턴스화 (class template instantiation)라고 한다.
int main()
{
    // int 를 보관하는 벡터를 만든다.
    Vector<int> int_vec; // templat에 타입을 명시해주는 방법 Class<type>. T에 int를 전달
    int_vec.push_back(3);
    int_vec.push_back(2);

    std::cout << "-------- int vector ----------" << std::endl;
    std::cout << "첫번째 원소 : " << int_vec[0] << std::endl;
    std::cout << "두번째 원소 : " << int_vec[1] << std::endl;

    Vector<std::string> str_vec;
    str_vec.push_back("hello");
    str_vec.push_back("world");
    std::cout << "-------- std::string vector -------" << std::endl;
    std::cout << "첫번째 원소 : " << str_vec[0] << std::endl;
    std::cout << "두번째 원소 : " << str_vec[1] << std::endl;
}

// template이 int 타입을 T로 받은 경우 밑에와 같이 치환된 코드가 짜여진다.
/*
class Vector {
  int* data;
  int capacity;
  int length;

 public:
  // 어떤 타입을 보관하는지
  typedef T value_type;

  // 생성자
  Vector(int n = 1) : data(new int[n]), capacity(n), length(0) {}

  // 맨 뒤에 새로운 원소를 추가한다.
  void push_back(int s) {
    if (capacity <= length) {
      int* temp = new int[capacity * 2];
      for (int i = 0; i < length; i++) {
        temp[i] = data[i];
      }
      delete[] data;
      data = temp;
      capacity *= 2;
    }

    data[length] = s;
    length++;
  }

  // 임의의 위치의 원소에 접근한다.
  int operator[](int i) { return data[i]; }

  // x 번째 위치한 원소를 제거한다.
  void remove(int x) {
    for (int i = x + 1; i < length; i++) {
      data[i - 1] = data[i];
    }
    length--;
  }

  // 현재 벡터의 크기를 구한다.
  int size() { return length; }

  ~Vector() {
    if (data) {
      delete[] data;
    }
  }
};
*/
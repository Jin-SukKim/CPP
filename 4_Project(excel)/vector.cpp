#include <string>
using std::string;
// 가변길이 배열 (문자열 저장용)
class Vector {
  string* data; // 데이터 보관용
  int capacity; // 현재 할당된 크기
  int length;   // 실제로 사용하고 있는 크기

 public:
  // 생성자
  Vector(int n = 1);
 
  // 맨 뒤에 새로운 원소를 추가한다.
  void push_back(string s);

  // 임의의 위치의 원소에 접근한다.
  string operator[](int i);

  // x 번째 위치한 원소를 제거한다.
  void remove(int x);

  // 현재 벡터의 크기를 구한다.
  int size();

  ~Vector();
};

// 클래스 내부 함수 선언에서 디폴드 인자를 명시하였다면 함수 본체에서 명시하면 안되고, 
// 반대로 함수 본체에서 명시하였다면 클래스 내부 함수 선언에 명시하면 안됩니다.
// 즉 둘 중 한 곳에만 명시해야된다.
Vector::Vector(int n) : data(new string[n]), capacity(n), length(0) {}

void Vector::push_back(string s) {
  if (capacity <= length) {
    string* temp = new string[capacity * 2];
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

string Vector::operator[](int i) { return data[i]; }

void Vector::remove(int x) {
  for (int i = x + 1; i < length; i++) {
    data[i - 1] = data[i];
  }
  length--;
}

int Vector::size() { return length; }

Vector::~Vector() {
  if (data) {
    delete[] data;
  }
}
#include <iostream>
#include <cstring>

class Complex {
 private:
  double real, img;

  double get_number(const char *str, int from, int to) const;
 public:
  Complex(double real, double img) : real(real), img(img) {}
  Complex(const Complex& c) { real = c.real, img = c.img; }
  Complex(const char* str);


  //통상적으로 자기 자신을 리턴하지 않는 이항 연산자들, 예를 들어 위와 같은 +, -, *, / 들은 모두 외부 함수로 선언하는 것이 원칙 입니다. 
  Complex operator+(const Complex& c) const;
  Complex operator-(const Complex& c) const;
  Complex operator*(const Complex& c) const;
  Complex operator/(const Complex& c) const;

  Complex& operator=(const Complex& c);

  Complex& operator+=(const Complex& c);
  Complex& operator-=(const Complex& c);
  Complex& operator*=(const Complex& c);
  Complex& operator/=(const Complex& c);

  // 파일러는 정확히 일치 하지 않는 경우, 가장 가까운 가능한 오버로딩 되는 함수를 찾게되서 밑의 function이 없어도 Compiler가 자동으로 처리 가능하다. 
  // complex.operator+(const char* str) 이 없으면 compiler는 
  // Complex::Complex(const char* str)을 이용해 complex.operator+(Complex(const char* str))로 바꿔서 실행한다.
  /*
  Complex operator+(const char* str) const;
  Complex operator-(const char* str) const;
  Complex operator*(const char* str) const;
  Complex operator/(const char* str) const;
  */
  
  // 컴파일러는 가까운 함수부터 찾는다.
  // 컴파일러는 a + a를 해석할 때 a.operator+(a); 또는 operator+(a, a); 두가지 형태 중 하나를 고른다
  // friend를 이용해 이 함수는 Complex 의 private 멤버 변수들에 접근할 수 있습니다.
  // a + "2+1i"는 a.operator+(a)로 작동하지만 "2+1i" + a는 "2+1i".operator+(a)란 형태는 error를 일으키므로
  // 또 다른 형태의 함수를 작성해준다.
  /*
    그러나 compiler에서 operator+(const char* str) const;와 operator+(const Complex& a, const Complex& b); 둘 모두 가능한 경우
    compiler error를 일으킬 수 있는다.
    통상적으로 자기 자신을 리턴하지 않는 이항 연산자들, 예를 들어 위와 같은 +, -, *, / 들은 모두 외부 함수로 선언하는 것이 원칙 입니다. 
    반대로 자기 자신을 리턴하는 이항 연산자들, 예를 들어 +=, -= 같은 애들은 모두 멤버 함수로 선언하는 것이 원칙 입니다.
  
  friend Complex operator+(const Complex& a, const Complex& b);
  */

  void println() { std::cout << "( " << real << " , " << img << " ) " << std::endl; }
};

// reference가 아닌 Complex 클래스 자체를 return 하는 이유는 값을 확실하게 return 시켜 사칙 연산에서 계산이 틀리지 않게 하기 위해서이다.
// reference로 return 하는 경우 값이 복사되는 것이 아닌 reference자체가 복사가 되므로 사칙연산 오류가 난다.
// ex) Complex a = b + c + b; -> (b.plus(c)).plus(b) -> (b + c) + (b + c)로 변해 의도한 a = 2 * b + c;이 아닌 a = 2 * b + 2 * c가 된다.
// b.plus(c)를 진행하며 reference 복사로 b에 (b + c)가 들어가 그 다음 .plus(b)가 (b + c)로 변해 (b + c) + (b + c)가 된다. 
// reference(참조자)를 return 하는 경우 b의 메모리 값 자체가 b + c로 바뀌지만 Complex class 자체를 return 시켜 값을 return하면
// 다른 메모리에 값이 들어가(복사된다) b의 메모리에 저장되어 있는 값이 변하지 않아 의도하던 대로 결과 값이 나온다.

Complex Complex::operator+(const Complex& c) const {
  Complex temp(real + c.real, img + c.img);
  return temp;
}
Complex Complex::operator-(const Complex& c) const {
  Complex temp(real - c.real, img - c.img);
  return temp;
}
Complex Complex::operator*(const Complex& c) const {
  Complex temp(real * c.real - img * c.img, real * c.img + img * c.real);
  return temp;
}
Complex Complex::operator/(const Complex& c) const {
  Complex temp(
    (real * c.real + img * c.img) / (c.real * c.real + c.img * c.img),
    (img * c.real - real * c.img) / (c.real * c.real + c.img * c.img));
  return temp;
}



// reference를 return 하는 이유 : 대입 연산 이후에 불필요한 복사를 방지하기 위해
// ex) a = b = c; 
// b = c; 가 b 를 리턴해야지, a = b; 가 성공적으로 수행될 수 있기 때문이다.
Complex& Complex::operator=(const Complex& c)
{
  real = c.real;
  img = c.img;
  return *this;
}

Complex& Complex::operator+=(const Complex& c) {
  (*this) = (*this) + c;
  return *this;
}
Complex& Complex::operator-=(const Complex& c) {
  (*this) = (*this) - c;
  return *this;
}
Complex& Complex::operator*=(const Complex& c) {
  (*this) = (*this) * c;
  return *this;
}
Complex& Complex::operator/=(const Complex& c) {
  (*this) = (*this) / c;
  return *this;
}

//cstdlib 의 atof 함수를 이용해 더 간단하게 만들수 있다.
double Complex::get_number(const char* str, int from, int to) const {
  bool minus = false;
  if (from > to) return 0;

  if (str[from] == '-') minus = true;
  if (str[from] == '-' || str[from] == '+') from++;

  double num = 0.0;
  double decimal = 1.0;

  bool integer_part = true;
  for (int i = from; i <= to; i++) {
    if (isdigit(str[i]) && integer_part) {
      num *= 10.0;
      num += (str[i] - '0');
    } else if (str[i] == '.')
      integer_part = false;
    else if (isdigit(str[i]) && !integer_part) {
      decimal /= 10.0;
      num += ((str[i] - '0') * decimal);
    } else
      break;  // 그 이외의 이상한 문자들이 올 경우
  }

  if (minus) num *= -1.0;

  return num;
}

Complex::Complex(const char* str) {
  // 입력 받은 문자열을 분석하여 real 부분과 img 부분을 찾아야 한다.
  // 문자열의 꼴은 다음과 같습니다 "[부호](실수부)(부호)i(허수부)"
  // 이 때 맨 앞의 부호는 생략 가능합니다. (생략시 + 라 가정)

  int begin = 0, end = strlen(str);
  img = 0.0;
  real = 0.0;

  // 먼저 가장 기준이 되는 'i' 의 위치를 찾는다.
  int pos_i = -1;
  for (int i = 0; i != end; i++) {
    if (str[i] == 'i') {
      pos_i = i;
      break;
    }
  }

  // 만일 'i' 가 없다면 이 수는 실수 뿐이다.
  if (pos_i == -1) {
    real = get_number(str, begin, end - 1);
    return;
  }

  // 만일 'i' 가 있다면,  실수부와 허수부를 나누어서 처리하면 된다.
  real = get_number(str, begin, pos_i - 1);
  img = get_number(str, pos_i + 1, end - 1);

  if (pos_i >= 1 && str[pos_i - 1] == '-') img *= -1.0;
}

/*
Complex Complex::operator+(const char* str) const {
  Complex temp(str);
  return (*this) + temp;
}
Complex Complex::operator-(const char* str) const {
  Complex temp(str);
  return (*this) - temp;
}
Complex Complex::operator*(const char* str) const {
  Complex temp(str);
  return (*this) * temp;
}
Complex Complex::operator/(const char* str) const {
  Complex temp(str);
  return (*this) / temp;
}
*/

/*
friend example: 
Complex operator+(const Complex& a, const Complex& b) {
  Complex temp(a.real + b.real, a.img + b.img);
  return temp;
}
*/
Complex operator+(const Complex& a, const Complex& b) {
  Complex temp(a);
  return temp.operator+(b);
}


int main() {
  Complex a(1.0, 2.0);
  Complex b(3.0, -2.0);
  Complex c(0.0, 0.0);
  c = a * b + a / b + a + b;
  c.println();

  a = "-1.1 + i3.923" + a;

  a = a + a;
  a.println();
}
#include <cstring>
#include <iostream>

/*
무분별한 friend이용은 권장되지 않는다.
접근할 수 없는 private variable들에 접근이 가능하기 때문이다.
*/
class Complex
{
private:
  double real, img;
  double get_number(const char *str, int from, int to);

public:
  Complex(double real, double img) : real(real), img(img) {}
  Complex(const Complex &c) { real = c.real, img = c.img; }
  Complex(const char *str);

  Complex operator+(const Complex &c);
  Complex operator-(const Complex &c);
  Complex operator*(const Complex &c);
  Complex operator/(const Complex &c);

  Complex &operator+=(const Complex &c);
  Complex &operator-=(const Complex &c);
  Complex &operator*=(const Complex &c);
  Complex &operator/=(const Complex &c);
  Complex &operator=(const Complex &c);

  friend Complex operator+(const Complex &a, const Complex &b);
  
  // 실제로 std::cout << 'test'은 std::cout.operator<<('test')와 같다.
  // 즉 iostream 내부에는 operator<<가 많은 type으로 오버로딩되어 있을것이다.
  // (cout은 ostream에 정의되어 있는데 iostream에서 include를 이용해 사용중이다.)
  friend std::ostream &operator<<(std::ostream &os, const Complex &c);
  void println()
  {
    std::cout << "( " << real << " , " << img << " ) " << std::endl;
  }
};

// private variable인 real과 img에 접근하기 위해 friend로 설정한다.
// 혹은 Complex 에 print(std::ostream& os) 와 같은 멤버 함수를 추가한 뒤, 이를 operator<< 에서 호출한다.
std::ostream &operator<<(std::ostream &os, const Complex &c)
{
  os << "( " << c.real << " , " << c.img << " ) ";
  return os;
}
Complex operator+(const Complex &a, const Complex &b)
{
  Complex temp(a.real + b.real, a.img + b.img);
  return temp;
}
Complex::Complex(const char *str)
{
  // 입력 받은 문자열을 분석하여 real 부분과 img 부분을 찾아야 한다.
  // 문자열의 꼴은 다음과 같습니다 "[부호](실수부)(부호)i(허수부)"
  // 이 때 맨 앞의 부호는 생략 가능합니다. (생략시 + 라 가정)
  int begin = 0, end = strlen(str);
  img = 0.0;
  real = 0.0;
  // 먼저 가장 기준이 되는 'i' 의 위치를 찾는다.
  int pos_i = -1;
  for (int i = 0; i != end; i++)
  {
    if (str[i] == 'i')
    {
      pos_i = i;
      break;
    }
  }
  // 만일 'i' 가 없다면 이 수는 실수 뿐이다.
  if (pos_i == -1)
  {
    real = get_number(str, begin, end - 1);
    return;
  }
  // 만일 'i' 가 있다면, 실수부와 허수부를 나누어서 처리하면 된다.
  real = get_number(str, begin, pos_i - 1);
  img = get_number(str, pos_i + 1, end - 1);
  if (pos_i >= 1 && str[pos_i - 1] == '-')
    img *= -1.0;
}
double Complex::get_number(const char *str, int from, int to)
{
  bool minus = false;
  if (from > to)
    return 0;
  if (str[from] == '-')
    minus = true;
  if (str[from] == '-' || str[from] == '+')
    from++;
  double num = 0.0;
  double decimal = 1.0;
  bool integer_part = true;
  for (int i = from; i <= to; i++)
  {
    if (isdigit(str[i]) && integer_part)
    {
      num *= 10.0;
      num += (str[i] - '0');
    }
    else if (str[i] == '.')
      integer_part = false;
    else if (isdigit(str[i]) && !integer_part)
    {
      decimal /= 10.0;
      num += ((str[i] - '0') * decimal);
    }
    else
      break; // 그 이외의 이상한 문자들이 올 경우
  }
  if (minus)
    num *= -1.0;
  return num;
}
Complex Complex::operator+(const Complex &c)
{
  Complex temp(real + c.real, img + c.img);
  return temp;
}
Complex Complex::operator-(const Complex &c)
{
  Complex temp(real - c.real, img - c.img);
  return temp;
}
Complex Complex::operator*(const Complex &c)
{
  Complex temp(real * c.real - img * c.img, real * c.img + img * c.real);
  return temp;
}
Complex Complex::operator/(const Complex &c)
{
  Complex temp(
      (real * c.real + img * c.img) / (c.real * c.real + c.img * c.img),
      (img * c.real - real * c.img) / (c.real * c.real + c.img * c.img));
  return temp;
}
Complex &Complex::operator+=(const Complex &c)
{
  (*this) = (*this) + c;
  return *this;
}
Complex &Complex::operator-=(const Complex &c)
{
  (*this) = (*this) - c;
  return *this;
}
Complex &Complex::operator*=(const Complex &c)
{
  (*this) = (*this) * c;
  return *this;
}
Complex &Complex::operator/=(const Complex &c)
{
  (*this) = (*this) / c;
  return *this;
}
Complex &Complex::operator=(const Complex &c)
{
  real = c.real;
  img = c.img;
  return *this;
}
int main()
{
  Complex a(0, 0);
  a = "-1.1 + i3.923" + a;
  std::cout << "Value of a : " << a << std::endl;
}
#include <iostream>
#include <sstream> // istringstream이 정의되어 있다.
#include <string>

// string stream을 활용해 atoi 와 같은 함수를 사용할 필요 없이 간편하게 문자열에서 숫자로 변환하는 함수를 만들 수 있다.
double to_number(std::string s) {
  std::istringstream ss(s);
  double x;

  ss >> x;
  return x;
}

// 숫자에서 문자열로 바꾸는 함수이다.
std::string to_str(int x) {
  // 거꾸로 데이터를 출력할 수 있는 스트림이다.
  std::ostringstream ss; 
  ss << x; // 자동으로 숫자에서 문자열로의 변환

  return ss.str(); // str 함수로 현재 문자열 스트림에 쓰여 있는 값을 불러온다.
}

int main() {
  // 문자열을 하나의 스트림이라 생각하게 해주는 가상화 장치
  std::istringstream ss("123");  // 문자열 123이 기록되어 있는 입력 스트림을 생성했다.
  int x;
  // 파일에서 숫자를 읽는 것과 같이 123을 읽어낸다. 
  ss >> x;

  std::cout << "입력 받은 데이터 :: " << x << std::endl;

  std::cout << "변환:: 1 + 2 = " << to_number("1") + to_number("2") << std::endl;
  
  std::cout << "문자열로 변환:: 1 + 2 = " << to_str(1 + 2) << std::endl;
  return 0;
}
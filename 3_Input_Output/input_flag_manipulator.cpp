#include <string>
#include <iostream>

int main() {
  int t;
  while (true) {
    // 입력 형식을 base16으로 바꾼다.
    std::cin.setf(std::ios_base::hex, std::ios_base::basefield);
    // 또는 밑에와 같이 입력 형식을 바꿀수도 있다. 
    // std::hex 같은 걸 조작자(manipulator)라 불린다. (입력 혹은 출력 방식을 바꿔주는 함수이다.)
    // std::cin >> std::hex >> t;
    // std::hex는 함수이고 std::ios_base::hex는 상수값이다.
    // endl은 출력의 조작자이다.
    std::cin >> t;
    std::cout << "입력 :: " << t << std::endl;
    if (std::cin.fail()) {
      std::cout << "제대로 입력해주세요" << std::endl;
      std::cin.clear();  // 플래그들을 초기화 하고
                         // std::cin.ignore(100,'n');//개행문자가 나올 때까지
                         // 무시한다
    }
    if (t == 0) break;
  }
}
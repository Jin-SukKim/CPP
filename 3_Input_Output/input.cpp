#include <iostream>
#include <string>

int main() {
  int t;
  while (true) {
    std::cin >> t;
    std::cout << "입력 :: " << t << std::endl;
    if (std::cin.fail()) {
      std::cout << "제대로 입력해주세요" << std::endl;
      std::cin.clear();            // 플래그들을 초기화 하고
      std::cin.ignore(100, '\n');  // 개행문자가 나올 때 까지 무시한다(버퍼를 비워버린다)
    }
    if (t == 1) break;
  }
}

/*
goodbit : 스트림에 입출력 작업이 가능할 때

badbit : 스트림에 복구 불가능한 오류 발생시

failbit : 스트림에 복구 가능한 오류 발생시

eofbit : 입력 작업시에 EOF 도달시

#include <iostream>
#include <string>

int main() {
  int t;
  std::cin >> t;  // 고의적으로 문자를 입력하면 failbit 가 켜진다
  std::cout << "fail 비트가 켜진 상태이므로, 입력받지 않는다" << std::endl;
  std::string s;
  std::cin >> s;
}

*/
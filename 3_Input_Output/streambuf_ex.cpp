#include <iostream>
#include <string>

int main() {
  std::string s;
  std::cin >> s;

  // 위치 지정자를 한 칸 옮기고, 그 다음 문자를 훔쳐본다 (이 때는 움직이지 않음)
  // 엿보기만 하고 스트림 위치 지정자는 움직이지 않는다.
  char peek = std::cin.rdbuf()->snextc();
  if (std::cin.fail()) std::cout << "Failed";
  std::cout << "두 번째 단어 맨 앞글자 : " << peek << std::endl;
  std::cin >> s;
  std::cout << "다시 읽으면 : " << s << std::endl;
}

// streambuf가 중요한 이뉴는 snextc같은 1바이트가 아닌
// wchar_t, 즉 다중 바이트 문자들(UTF-8같은 것)에 대한 처리도 용이하게 사용할 수 있어서이다.
// streambuf는 get_area와 put_area즉 입출력 영역이 보통 나누어지고 각각 stream은 보통 3개의 포인터가 존재하는데
// 처음을 가르키고 있는 포인터, 다음 읽을 위치를 지정하고 있는 포인터, 그리고 마지막을 가르키고 있는 포인터가 있다.
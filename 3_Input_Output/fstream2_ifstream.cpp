#include <fstream>
#include <iostream>
#include <string>

int main() {
  // 파일 읽기 준비
  std::ifstream in("test.txt");
  std::string s;

  if (in.is_open()) {
    // 위치 지정자를 파일 끝으로 옮긴다.
    in.seekg(0, std::ios::end);

    // 그리고 그 위치를 읽는다. (파일의 크기)
    int size = in.tellg();

    // 그 크기의 문자열을 할당한다.
    s.resize(size);

    // 위치 지정자를 다시 파일 맨 앞으로 옮긴다.
    in.seekg(0, std::ios::beg);

    // 파일 전체 내용을 읽어서 문자열에 저장한다.
    in.read(&s[0], size);
    std::cout << s << std::endl;
  } else {
    std::cout << "파일을 찾을 수 없습니다!" << std::endl;
  }

  return 0;
}


/*
// getline 으로 읽어들이기
#include <fstream>
#include <iostream>
#include <string>

int main() {
  // 파일 읽기 준비
  std::ifstream in("test.txt");
  char buf[100]; // 지정한 버퍼의 크키가 다 차면 정상적으로 데이터를 받을 수 없다.

  if (!in.is_open()) {
    std::cout << "파일을 찾을 수 없습니다!" << std::endl;
    return 0;
  }

  while (in) {
    // 파일에서 개행문자 (\n) 이 나올 때 가지 최대 지정한 크기 - 1 만큼 읽게하는데
    // 마지막은 null 종료 문자를 읽어야 되기 떄문이다.
    in.getline(buf, 100);            
    // in.getline(buf, 100, '.'); // 지정된 문자가 나올 때 까지 읽는다.
    std::cout << buf << std::endl;
  }

  return 0;
}
*/

/*
// std::string 에 정의된 getline 사용(위 단점 개선)
#include <fstream>
#include <iostream>
#include <string>

int main() {
  // 파일 읽기 준비
  std::ifstream in("test.txt");

  if (!in.is_open()) {
    std::cout << "파일을 찾을 수 없습니다!" << std::endl;
    return 0;
  }

  std::string s;
  // while 문 조건으로 절대 in.eof() 를 사용하면 안된다.
  // eof 함수는 파일 위치 지시자가 파일에 끝에 도달한 이후 에 true 를 리턴해서이다.
  // 즉 파일 끝 직전에 아직 EOF를 위치 지정자가 읽지 않아 in >> data, data에 아무것도 들어가지 않고 남게된다.
  // istream 객체는 다음 읽기가 안전할 때만 true 로 캐스팅됩니다.
  while (in) {
    getline(in, s);     // ifstream에 정의된 getline이 아닌 string에 정의된 getline을 사용한다. 
    std::cout << s << std::endl;
  }

  return 0;
}
*/
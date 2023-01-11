// 파일에서의 입출력
#include <fstream>
#include <iostream>
#include <string>

int main() {
  // 파일 읽기 준비
  std::ifstream in("test.txt");
  std::string s;

  if (in.is_open()) {
    // cin처럼 in 객체를 이용해 파일을 읽는다.
    in >> s;
    std::cout << "입력 받은 문자열 :: " << s << std::endl;
  } else {
    std::cout << "파일을 찾을 수 없습니다!" << std::endl;
  }

  // c언어와는 다르게 파일을 읽은 후 fclose를 안해주는데 그 이유는 ifstream 즉 파일 입력 클래스의 소멸자에서 close를 해주기 때문이다.

  return 0;
}

// close를 직접해줘야 하는 경우는 새로운 파일에서 입력을 하고 싶은 경우
// 기존 파일 스트림과의 연결을 종료하고 새로운 파일을 연결시켜 줘야하기 때문이다.
/*
#include <fstream>
#include <iostream>
#include <string>

int main() {
  // 파일 읽기 준비
  std::ifstream in("test.txt");
  std::string s;

  if (in.is_open()) {
    in >> s;
    std::cout << "입력 받은 문자열 :: " << s << std::endl;
  } else {
    std::cout << "파일을 찾을 수 없습니다!" << std::endl;
  }

  in.close();               // 연결 종료
  in.open("other.txt");     // 새로운 연결 생성

  if (in.is_open()) {
    in >> s;
    std::cout << "입력 받은 문자열 :: " << s << std::endl;
  } else {
    std::cout << "파일을 찾을 수 없습니다!" << std::endl;
  }

  return 0;
}
*/


// 이진수로 읽기 (little endian)
/*
#include <fstream>
#include <iostream>
#include <string>

int main() {
  // 파일 읽기 준비
  // 생성자 옵션에서 binary 형태로 받겠다고 명시 
  //(binary말고 다른 옵션들도 OR을 이용해서 여러가지 옵션을 조합가능하다.)
  std::ifstream in("test.txt", std::ios::binary); 
  
  std::string s;

  int x;
  if (in.is_open()) {
    // read(첫번쨰 인자 버퍼, 읽을 바이트 수)
    in.read((char*)(&x), 4);                    // int 변수를 4 byte짜리 char 배열로 생각하게 해서 전달.
    // char x[10];      // 실제로는 그냥 char 배열에 저장시키면 된다.
    // in.read(x, 10);
    std::cout << std::hex << x << std::endl;
  } else {
    std::cout << "파일을 찾을 수 없습니다!" << std::endl;
  }

  return 0;
}
*/
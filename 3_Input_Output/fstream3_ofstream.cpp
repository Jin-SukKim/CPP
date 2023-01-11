#include <iostream>
#include <fstream>
#include <string>

/*
ios::app : 자동으로 파일 끝에서 부터 읽기와 쓰기를 실시합니다. (즉 파일을 열 때 위치 지정자가 파일 끝을 가리키고 있게 됩니다)
           파일 위치 지정자가 기존 파일의 끝이 시작점이라 생각하여 움직인다. (원본 파일 내용을 무조건적으로 보장한다.)

ios::ate : 자동으로 파일 끝에서 부터 읽기와 쓰기를 실시합니다. (즉 파일을 열 때 위치 지정자가 파일 끝을 가리키고 있게 됩니다)
           위치 지정자를 그 이전으로 옮길 수 있습니다. 즉 위치 지정자의 위치가 기존 파일을 포함해 움직인다. (원본 파일 내용을 보장할 수 없다.)

ios::trunc : 파일 스트림을 열면 기존에 있던 내용들이 모두 지워집니다. 기본적으로 ofstream 객체를 생성할 때 이와 같은 설정으로 만들어집니다.

ios::in, std::ios::out : 파일에 입력을 할 지 출력을 할 지 지정하며, ifstream 과 ofstream 객체를 생성할 때 각각은 이미 설정되어 있습니다.
*/
int main() {
  // 파일 쓰기 준비
  // 열려는 파일이 존재하지 않으면 해당 파일을 생성하고 열게되고 이미 존재한 경우 기존 내용 삭제되고 새로운 내용으로 덮어 씌운다.
  std::ofstream out("test.txt");
  // std::ofstream out("test.txt", std::ios::app); // 기존 내용을 덮어씌우지 않고 새로운 내용을 뒤에 붙여 쓴다.

  std::string s;
  if (out.is_open()) {
    out << "이걸 쓰자~~";
  }

  return 0;
}

/*
//  std::ofstream 연산자 오버로딩

#include <fstream>
#include <iostream>
#include <string>

class Human {
  std::string name;
  int age;

 public:
  Human(const std::string& name, int age) : name(name), age(age) {}
  std::string get_info() {
    return "Name :: " + name + " / Age :: " + std::to_string(age);
  }

  friend std::ofstream& operator<<(std::ofstream& o, Human& h);
};

std::ofstream& operator<<(std::ofstream& o, Human& h) {
  o << h.get_info();
  return o;
}
int main() {
  // 파일 쓰기 준비
  std::ofstream out("test.txt");

  Human h("이재범", 60);
  out << h << std::endl;

  return 0;
}
*/
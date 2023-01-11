// 복사 생성자의 중요성
#include <string.h>
#include <iostream>

class Photon_Cannon {
  int hp, shield;
  int coord_x, coord_y;
  int damage;

  char *name;

 public:
  Photon_Cannon(int x, int y);
  Photon_Cannon(int x, int y, const char *cannon_name);
  Photon_Cannon(const Photon_Cannon &pc);
  ~Photon_Cannon();

  void show_status();
};
Photon_Cannon::Photon_Cannon(int x, int y) {
  hp = shield = 100;
  coord_x = x;
  coord_y = y;
  damage = 20;

  name = NULL;
}

/*
c++ 컴파일러는 default 복사 생성자를 지원해 굳이 안적어도 된다.
default 복사 생성자는 한계가 있는데 class variable 중 동적 할당한 게 있으면
class 한개가 파괴 될시 동적 할당된 variable의 메모리를 해제 할 텐데 
나중에 dafault 복사 생성자로 만든 class가 동일한 동적 할당된 메모리를 가르키고 있지만 이미 해제되 에러를 발생한다.
!한번 해제된 메모리는 다시 해제될 수 없다!
이런 경우를 shallow copy라고 한다.

이경우 deep copy를 사용하면 된다.
다른 메모리에 동적 할당을 해 내용을 복사한다.
*/ 

Photon_Cannon::Photon_Cannon(const Photon_Cannon &pc) {
  std::cout << "복사 생성자 호출! " << std::endl;
  hp = pc.hp;
  shield = pc.shield;
  coord_x = pc.coord_x;
  coord_y = pc.coord_y;
  damage = pc.damage;

  name = new char[strlen(pc.name) + 1];
  strcpy(name, pc.name);
}
Photon_Cannon::Photon_Cannon(int x, int y, const char *cannon_name) {
  hp = shield = 100;
  coord_x = x;
  coord_y = y;
  damage = 20;

  name = new char[strlen(cannon_name) + 1];
  strcpy(name, cannon_name);
}
Photon_Cannon::~Photon_Cannon() {
  if (name) delete[] name;
}
void Photon_Cannon::show_status() {
  std::cout << "Photon Cannon :: " << name << std::endl;
  std::cout << " Location : ( " << coord_x << " , " << coord_y << " ) "
            << std::endl;
  std::cout << " HP : " << hp << std::endl;
}
int main() {
  Photon_Cannon pc1(3, 3, "Cannon");
  Photon_Cannon pc2 = pc1;

  pc1.show_status();
  pc2.show_status();
}
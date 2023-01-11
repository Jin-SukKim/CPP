#include <iostream>

class Marine {
  // 프로그램이 종료 될 때 소멸되는 variable (메모리의 stack에 저장)
  static int total_marine_num;// 클래스의 모든 객체들이 공유하는 변수(static은 단 한번 선언된다)
  
  // 멤버 변수는 클래스 내부에서 바로 초기화 불가능 단 const static인 경우 가능하다.
  const static int i = 0;

  int hp;                // 마린 체력
  int coord_x, coord_y;  // 마린 위치
  bool is_dead;

  // 클래스 내부에 레퍼런스 변수나 상수를 넣고 싶다면 이들을 생성자에서 무조건 초기화 리스트를 사용해서 초기화시켜야 된다.
  const int default_damage;  // 기본 공격력

 public:
  Marine();              // 기본 생성자
  Marine(int x, int y);  // x, y 좌표에 마린 생성
  Marine(int x, int y, int default_damage);

  //상수 함수 ((기존의 함수의 정의) const;)
  int attack() const;                    // 데미지를 리턴한다.
  Marine& be_attacked(int damage_earn);  // 입는 데미지
  void move(int x, int y);               // 새로운 위치

  void show_status();  // 상태를 보여준다.
  static void show_total_marine();
  ~Marine() { total_marine_num--; }
};

// 클래스의 static 변수를 초기화하는 방법
int Marine::total_marine_num = 0;


/*
static 함수 내에는 static 변수를 사용하는 데 함수가 클래스에 종속 되므로 
만약 멤버 변수가 함수에서 호출될 시 어떤 클래스에서 부르는 지 모르기 때문이다.
*/
void Marine::show_total_marine() {
  std::cout << "전체 마린 수 : " << total_marine_num << std::endl;
}

/*
생성자 대신 초기화 리스트(Initializer list) 사용. var(arg)에서 var과 arg의 이름이 같아도 된다. (var = 멤버 변수, arg = 인자로 받은 변수)
생성자와 초기화 리스트의 차이
초기화 리스트 :
    int a = 10, 생성과 초기화 동시 즉 원하는 생성자 바로 호출
생성자 :
    생성 후 대입
    int a, Default 생성자 호출 후
    a = 10, 원하는 생성자 호출
*/
Marine::Marine()
    : hp(50), coord_x(0), coord_y(0), default_damage(5), is_dead(false) {
  total_marine_num++;
}

Marine::Marine(int x, int y)
    : coord_x(x),
      coord_y(y),
      hp(50),

      default_damage(5),
      is_dead(false) {
  total_marine_num++;
}

Marine::Marine(int x, int y, int default_damage)
    : coord_x(x),
      coord_y(y),
      hp(50),
      default_damage(default_damage),
      is_dead(false) {
  total_marine_num++;
}

void Marine::move(int x, int y) {
  coord_x = x;
  coord_y = y;
}

/*
const function example
const function(상수 함수)은 다른 변수의 값을 바꾸지 않는 함수로 사용한다.
즉 이 함수 내에서는 객체들의 읽기만 수행한다.
그러므로 호출 가능한 함수는 다른 상수 함수를 사용한다.

보통은 private 멤버 변수를 get_var()등의 function으로 값을 return 받는다.

TIP: mutable type variable은 예외적으로 const function 내부에서 값의 변화가 가능하다.
*/
int Marine::attack() const { return default_damage; }

// this : 객체 자신을 가리키는 포인터의 역할, 즉 자신의 멤버 변수를 가르킨다. (static 함수만이 this 키워드가 없다.)
// this-> : 생략 가능하다. this->hp 대신 hp 가능
Marine& Marine::be_attacked(int damage_earn) {
  this->hp -= damage_earn;
  if (this->hp <= 0) this->is_dead = true;

  return *this; // 객체 그 자신을 return 즉 ex1.be_attacked(~)면 ex1객체를 return
}
void Marine::show_status() {
  std::cout << " *** Marine *** " << std::endl;
  std::cout << " Location : ( " << coord_x << " , " << coord_y << " ) "
            << std::endl;
  std::cout << " HP : " << hp << std::endl;
  std::cout << " 현재 총 마린 수 : " << total_marine_num << std::endl;
}

// static 함수 테스트
void create_marine() {
  Marine marine3(10, 10, 4);
  /*
  클래스에 종속되는 함수 이므로 (객체).(멤버 함수)가 아닌 (클래스)::(static 함수) 형식으로 호출한다.
  */
  Marine::show_total_marine(); // static 함수 호출
}

int main() {
  Marine marine1(2, 3, 5);
  marine1.show_status();

  Marine marine2(3, 5, 10);
  marine2.show_status();

  Marine::show_total_marine();
  
  std::cout << std::endl << "마린 1 이 마린 2 를 두 번 공격! " << std::endl;
  marine2.be_attacked(marine1.attack()).be_attacked(marine1.attack());

  marine1.show_status();
  marine2.show_status();
}
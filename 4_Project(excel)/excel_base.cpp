#include <iostream>
#include <string>

/*
일단 엑셀의 셀들의 정보 (일단 단순한 std::string 이라고 생각합시다) 에 대한 Cell 클래스가 있고 이 Cell 객체들을 모아두는 Table 클래스가 있습니다. 
Table 클래스에는 2차원 배열로 Cell 객체들에 대한 정보 (참고로 Cell 객체가 생성 될 때 마다 동적으로 Cell 객체를 생성합니다.) 가 보관되어 있습니다. 
또한 Table 클래스에 전체 데이터를 출력하는 print_table 함수가 가상으로 정의되어 있습니다.

여러분은 Table 클래스를 상속 받는 TextTable, CSVTable, HTMLTable 클래스를 만들어서 print_table 함수를 오버라이드 할 함수들을 제작할 것입니다. 
예를 들어 TextTable 클래스의 print_table 함수는 텍스트 형식으로, CSVTable 은 CSV 파일 형식으로 등등
*/
class Table;
class Cell {
  Table* table;  // 어느 테이블에 속해있는지
  std::string data;
  int x, y;  // 테이블 에서의 위치
 public:
  Cell(const std::string& data) : data(data){};
};

class Table {
  Cell*** data_base;  // 왜 3중 포인터 인지 잘 생각해보세요!
 public:
  Table();
  virtual std::string print_table() = 0;
  void reg_cell(Cell* c, int row, int col);  // Cell 을 등록한다
  std::string get_cell_std::string(int row, int col);  // 해당 위치의 Cell 데이터를 얻는다.
  ~Table();
};

std::ostream& operator<<(std::ostream& o, Table& t) {
  o << t.print_table();
  return o;
}
class TextTable : public Table {};
class CSVTable : public Table {};
class HTMLTable : public Table {};


/*
다음 난이도

하지만 실제 엑셀의 경우 셀이 문자열 데이터만 들어가는 것이 아니라, 숫자나 날짜 심지어 수식 까지도 들어갈 수 있습니다. 
따라서 우리는 Cell 을 상속 받는 4 개의 StringCell. NumberCell, DateCell, ExprCell 클래스들을 만들어야 합니다.

또한 Cell 클래스에 to_numeric (데이터를 숫자로 바꾼다)과 std::stringify (데이터를 문자열로 바꾼다) 함수들을 가상으로 정의하고, 
4개의 클래스들이 이를 상속 받아서 구현할 수 있게 해야 합니다. (참고로 문자열을 숫자로 변환하면 그냥 0 이 되게 하면 됩니다)

또한 ExprCell 의 경우 간단한 수식에 대한 정보를 가지는 객체로, Cell 들 간의 연산을 사용할 수 있습니다. 
에를 들어서 A1+B2+C6-6 와 같은 데이터가 들어 있는 ExprCell 에 to_numeric 함수를 호출하면A1, 
B2, C6 의 값을 더하고 (각 셀에 to_numeric 을 해서), 6 을 빼준 결과값이 나와야 합니다.

참고로 프로그래밍의 편의를 위해서 ExprCell 의 경우, 셀을 지칭하는 것은 딱 두 글자 (A1, Z9 처럼) 로 하고, 
숫자는 오직 한 자리 수 정수, 그리고 가능한 연산자는 +, -, *, / 로 하겠습니다.

아마도 여태 까지 강좌에서 한 프로그래밍 중에 가장 도전적인 것이 아닌가 싶습니다. 
참고로 위를 구현하기 위해서 여러가지 자료형이 필요할 텐데 (있으면 편리합니다!), 
대표적으로 벡터와 스택이 있습니다. 벡터는 가변 길의 배열로, 
배열 처럼 사용하면서 사용자가 임의의 위치에 자료를 넣다 뺏다 할 수 있는 구조고, 
스택은 pop 과 push 밖에 없는 자료형으로, push 을 하면 새로운 데이터가 맨 위에 삽입되고, 
pop 을 하면 맨 위에 있는 것 부터 제거되어 나오게 됩니다. (즉 먼저 넣은것이 나중에 나오는 자료구조 입니다) 
저는 이들 자료 구조를 새롭게 만들어서 사용하였습니다. 여러분들도 아마 필요하실 것입니다.
*/
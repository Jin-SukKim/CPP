class Cell
{
protected:
    int x, y;
    Table *table;

public:
    // 순수 가상 함수로 정의해서 이를 상속 받는 클래스에서 이 함수들을 반드시 구현
    virtual string stringify() = 0;
    virtual int to_numeric() = 0;

    // 상속 받는 클래스에서 데이터를 보관
    Cell(int x, int y, Table *table);
};

// 문자열
class StringCell : public Cell
{
    string data;

public:
    string stringify();
    int to_numeric();

    StringCell(string data, int x, int y, Table *t);
};

// 정수
class NumberCell : public Cell
{
    int data;

public:
    string stringify();
    int to_numeric();

    NumberCell(int data, int x, int y, Table *t);
};

// 시간
class DateCell : public Cell
{
    // yyyy-mm-dd
    // 1970년 부터 현재 시간 까지 몇 초가 흘렀는지 보관하는 정수형 변수
    time_t data;

public:
    string stringify();
    int to_numeric();

    DateCell(string s, int x, int y, Table *t);
};

Cell::Cell(int x, int y, Table* table) : x(x), y(y), table(table) {}

StringCell::StringCell(string data, int x, int y, Table* t)
    : data(data), Cell(x, y, t) {}
string StringCell::stringify() { return data; }
int StringCell::to_numeric() { return 0; }

/*


NumberCell


*/
NumberCell::NumberCell(int data, int x, int y, Table* t)
    : data(data), Cell(x, y, t) {}

string NumberCell::stringify() { return to_string(data); }
int NumberCell::to_numeric() { return data; }

/*


DateCell


*/
string DateCell::stringify() {
  char buf[50];
  tm temp;
  localtime_s(&temp, &data);

  strftime(buf, 50, "%F", &temp);

  return string(buf);
}
int DateCell::to_numeric() { return static_cast<int>(data); }

DateCell::DateCell(string s, int x, int y, Table* t) : Cell(x, y, t) {
  // 입력받는 Date 형식은 항상 yyyy-mm-dd 꼴이라 가정한다.
  int year = atoi(s.c_str());
  int month = atoi(s.c_str() + 5);
  int day = atoi(s.c_str() + 8);
  
  //  timeinfo 객체를 초기화
  //  tm 클래스는 일월년 시분초 단위로 데이터를 보관하는 클래스
  tm timeinfo;

  timeinfo.tm_year = year - 1900;
  timeinfo.tm_mon = month - 1;
  timeinfo.tm_mday = day;
  timeinfo.tm_hour = 0;
  timeinfo.tm_min = 0;
  timeinfo.tm_sec = 0;
  
  // DateCell 은 time_t 형태로 데이터를 보관하고 있는데 그 변환을 위해 mktime 에 timeinfo 를 전달하면 변환가능
  data = mktime(&timeinfo);
}
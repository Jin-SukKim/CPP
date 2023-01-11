// 큰 테이블에서 한 칸을 의미하는 객체
namespace MyExcel
{
class Cell
{
    protected:
        int x, y;       // Cell의 위치
        Table *table;   // 어느 테이블에 위치해 있는지에 관한 데이터

        string data;    // 내용을 보관하는 문자열

    public:
        Cell(string data, int x, int y, Table *table);
        
        // cell 값을 문자열로 변환하는 가상함수
        virtual string stringify();
        // cell 값을 정수 데이터로 변환하는 가상함수
        virtual int to_numeric();
};

Cell::Cell(string data, int x, int y, Table* table) : data(data), x(x), y(y), table(table) {}

string Cell::stringify() { return data; }

int Cell::to_numeric() { return 0; }
}
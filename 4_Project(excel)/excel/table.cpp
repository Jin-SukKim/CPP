#include "cell.cpp"

namespace MyExcel
{
    // pure virtual 함수가 포함되어 있어 Table 객체를 직접 생성할 수 없다.
    class Table
    {
    protected:
        // 행 및 열의 최대 크기
        int max_row_size, max_col_size;

        // 데이터를 보관하는 테이블
        // Cell* 을 보관하는 2차원 배열이라 생각하면 편하다
        Cell ***data_table; // cell 객체를 동적으로 생성하고 그 객체에 대한 포인터를 2차원 배열로 저장

    public:
        Table(int max_row_size, int max_col_size);

        ~Table();

        // 새로운 셀을 row 행 col 열에 등록한다.
        void reg_cell(Cell *c, int row, int col);

        // 해당 셀의 정수값을 반환한다.
        // s : 셀 이름 (Ex. A3, B6 과 같이)
        int to_numeric(const string &s);

        // 행 및 열 번호로 셀을 호출한다.
        int to_numeric(int row, int col);

        // 해당 셀의 문자열을 반환한다.
        string stringify(const string &s);
        string stringify(int row, int col);

        // pure virtual 함수
        virtual string print_table() = 0;
    };

    Table::Table(int max_row_size, int max_col_size)
        : max_row_size(max_row_size), max_col_size(max_col_size)
    {
        data_table = new Cell **[max_row_size];
        for (int i = 0; i < max_row_size; i++)
        {
            data_table[i] = new Cell *[max_col_size];
            for (int j = 0; j < max_col_size; j++)
            {
                data_table[i][j] = NULL;
            }
        }
    }

    Table::~Table()
    {
        for (int i = 0; i < max_row_size; i++)
        {
            for (int j = 0; j < max_col_size; j++)
            {
                if (data_table[i][j])
                    delete data_table[i][j]; // 동적으로 생성된 cell 삭제
            }
        }
        for (int i = 0; i < max_row_size; i++)
        {
            delete[] data_table[i]; // cell 배열 삭제(1차원 배열)
        }
        delete[] data_table; // 테이블 전체 삭제(2차원배열)
    }

    void Table::reg_cell(Cell *c, int row, int col)
    {
        if (!(row < max_row_size && col < max_col_size))
            return;

        // 해당 위치에 cell이 존재할 경우 삭제 후 등록
        if (data_table[row][col])
        {
            delete data_table[row][col];
        }
        data_table[row][col] = c;
    }

    int Table::to_numeric(const string &s)
    {
        // Cell 이름으로 받는다.
        int col = s[0] - 'A';
        int row = atoi(s.c_str() + 1) - 1;

        if (row < max_row_size && col < max_col_size)
        {
            if (data_table[row][col])
            {
                return data_table[row][col]->to_numeric();
            }
        }
        return 0;
    }

    int Table::to_numeric(int row, int col)
    {
        if (row < max_row_size && col < max_col_size && data_table[row][col])
        {
            return data_table[row][col]->to_numeric();
        }
        return 0;
    }

    string Table::stringify(const string &s)
    {
        // Cell 이름으로 받는다.
        int col = s[0] - 'A';
        int row = atoi(s.c_str() + 1) - 1;

        if (row < max_row_size && col < max_col_size)
        {
            if (data_table[row][col])
            {
                return data_table[row][col]->stringify();
            }
        }
        return 0;
    }

    string Table::stringify(int row, int col)
    {
        if (row < max_row_size && col < max_col_size && data_table[row][col])
        {
            return data_table[row][col]->stringify();
        }
        return "";
    }

    // 파일이나 표준 스트림(cout) 입출력에 쉽게 사용할 수 있도록
    std::ostream &operator<<(std::ostream &o, Table &table)
    {
        o << table.print_table();
        return o;
    }
}
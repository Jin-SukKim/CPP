#include <math.h>
#include <iostream>
#include <string>
#include <fstream>
#include "table.cpp"
#include "utils.hpp"
using std::max;
using std::to_string;

namespace MyExcel
{
    class TxtTable : public virtual Table
    {
        // print_table 에서 사용할 부가적인 함수
        string repeat_char(int n, char c);

        // print_table 에서 사용할 부가적인 함수
        // 숫자로 된 열 번호를 A, B, .... Z, AA, AB, ...  이런 순으로 매겨준다.
        string col_num_to_str(int n);

    public:
        TxtTable(int row, int col);

        // 각 열의 최대 문자열 길이를 계산한 뒤에, 이를 바탕으로 각 열의 폭을 결정해서 표를 출력
        // 텍스트로 표를 깨끗하게 출력해준다.
        string print_table();
    };

    TxtTable::TxtTable(int row, int col) : Table(row, col) {}

    // 텍스트로 표를 깨끗하게 출력해준다.
    string TxtTable::print_table()
    {
        string total_table;

        int *col_max_wide = new int[max_col_size];
        for (int i = 0; i < max_col_size; i++)
        {
            unsigned int max_wide = 2;
            for (int j = 0; j < max_row_size; j++)
            {
                if (data_table[j][i] &&
                    data_table[j][i]->stringify().length() > max_wide)
                {
                    max_wide = data_table[j][i]->stringify().length();
                }
            }
            col_max_wide[i] = max_wide;
        }
        // 맨 상단에 열 정보 표시
        total_table += "    ";
        int total_wide = 4;
        for (int i = 0; i < max_col_size; i++)
        {
            if (col_max_wide[i])
            {
                int max_len = max(2, col_max_wide[i]);
                total_table += " | " + col_num_to_str(i);
                total_table += repeat_char(max_len - col_num_to_str(i).length(), ' ');

                total_wide += (max_len + 3);
            }
        }

        total_table += "\n";
        // 일단 기본적으로 최대 9999 번째 행 까지 지원한다고 생각한다.
        for (int i = 0; i < max_row_size; i++)
        {
            total_table += repeat_char(total_wide, '-');
            total_table += "\n" + to_string(i + 1);
            total_table += repeat_char(4 - to_string(i + 1).length(), ' ');

            for (int j = 0; j < max_col_size; j++)
            {
                if (col_max_wide[j])
                {
                    int max_len = max(2, col_max_wide[j]);

                    string s = "";
                    if (data_table[i][j])
                    {
                        s = data_table[i][j]->stringify();
                    }
                    total_table += " | " + s;
                    total_table += repeat_char(max_len - s.length(), ' ');
                }
            }
            total_table += "\n";
        }

        return total_table;
    }

    string TxtTable::repeat_char(int n, char c)
    {
        string s = "";
        for (int i = 0; i < n; i++)
            s.push_back(c);

        return s;
    }
    // 숫자로 된 열 번호를 A, B, .... Z, AA, AB, ...  이런 순으로 매겨준다.
    string TxtTable::col_num_to_str(int n)
    {
        string s = "";
        if (n < 26)
        {
            s.push_back('A' + n);
        }
        else
        {
            char first = 'A' + n / 26 - 1;
            char second = 'A' + n % 26;

            s.push_back(first);
            s.push_back(second);
        }

        return s;
    }
}
int main()
{
    MyExcel::TxtTable table(5, 5);
    std::ofstream out("test.txt");

    table.reg_cell(new Cell("Hello~", 0, 0, &table), 0, 0);
    table.reg_cell(new Cell("C++", 0, 1, &table), 0, 1);

    table.reg_cell(new Cell("Programming", 1, 1, &table), 1, 1);
    std::cout << std::endl
              << table;
    out << table;
}
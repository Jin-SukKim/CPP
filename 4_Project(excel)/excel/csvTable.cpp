namespace MyExcel
{
    class CSVTable : public virtual Table
    {
    public:
        CSVTable(int row, int col);

        string print_table();
    };

    CSVTable::CSVTable(int row, int col) : Table(row, col) {}

    string CSVTable::print_table()
    {
        string s = "";
        for (int i = 0; i < max_row_size; i++)
        {
            for (int j = 0; j < max_col_size; j++)
            {
                if (j >= 1)
                    s += ",";
                // CSV 파일 규칙에 따라 문자열에 큰따옴표가 포함되어 있다면 "" 로
                // 치환하다.
                string temp;
                if (data_table[i][j])
                    temp = data_table[i][j]->stringify();

                for (int k = 0; k < temp.length(); k++)
                {
                    if (temp[k] == '"')
                    {
                        // k 의 위치에 " 를 한 개 더 집어넣는다.
                        temp.insert(k, 1, '"');

                        // 이미 추가된 " 를 다시 확인하는 일이 없게 하기 위해
                        // k 를 한 칸 더 이동시킨다.
                        k++;
                    }
                }
                temp = '"' + temp + '"';
                s += temp;
            }
            s += '\n';
        }
        return s;
    }
}
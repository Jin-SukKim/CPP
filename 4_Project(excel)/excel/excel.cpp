// 키보드로 명령을 처리하는 엑셀 프로그램
//  사용자의 입력을 받아서 실제 테이블을 생성하고 이를 관리해주는 클래스
class Excel
{
    Table *current_table;

public:
    Excel(int max_row, int max_col, int choice);

    // 사용자의 입력을 인자로 받아서, 이를 처리하는 역할
    int parse_user_input(string s);
    void command_line();
};

// 어떠한 형태의 테이블을 형성할 지 결정
Excel::Excel(int max_row, int max_col, int choice = 0)
{
    switch (choice)
    {
    case 0:
        current_table = new TxtTable(max_row, max_col);
        break;
    case 1:
        current_table = new CSVTable(max_row, max_col);
        break;
    default:
        current_table = new HtmlTable(max_row, max_col);
    }
}

/*
    ex: setn A1 10 입력시, A1 셀을 NumberCell 로 생성하며, 10 의 값으로 초기화
    ex2: sets B2 hello world!의 경우 B2 셀을 StringCell 로 생성하며 "hello world!" 로 초기화
    날짜와 수식의 경우도 마찬가지이며, 각각 setd 와 sete 의 명령어를 사용하고 있습니다.
    그 외에도, out 을 통해서 원하는 파일에 출력할 수 도 있고, exit 를 하면 프로그램을 종료할 수 있습니다.
    사용자의 입력을 받아서 적절한 명령을 처리
*/
int Excel::parse_user_input(string s)
{
    int next = 0;
    string command = "";
    for (int i = 0; i < s.length(); i++)
    {
        if (s[i] == ' ')
        {
            command = s.substr(0, i);
            next = i + 1;
            break;
        }
        else if (i == s.length() - 1)
        {
            command = s.substr(0, i + 1);
            next = i + 1;
            break;
        }
    }

    string to = "";
    for (int i = next; i < s.length(); i++)
    {
        if (s[i] == ' ' || i == s.length() - 1)
        {
            to = s.substr(next, i - next);
            next = i + 1;
            break;
        }
        else if (i == s.length() - 1)
        {
            to = s.substr(0, i + 1);
            next = i + 1;
            break;
        }
    }

    // Cell 이름으로 받는다.
    int col = to[0] - 'A';
    int row = atoi(to.c_str() + 1) - 1;

    string rest = s.substr(next);

    if (command == "sets")
    {
        current_table->reg_cell(new StringCell(rest, row, col, current_table), row,
                                col);
    }
    else if (command == "setn")
    {
        current_table->reg_cell(
            new NumberCell(atoi(rest.c_str()), row, col, current_table), row, col);
    }
    else if (command == "setd")
    {
        current_table->reg_cell(new DateCell(rest, row, col, current_table), row,
                                col);
    }
    else if (command == "sete")
    {
        current_table->reg_cell(new ExprCell(rest, row, col, current_table), row,
                                col);
    }
    else if (command == "out")
    {
        ofstream out(to);
        out << *current_table;
        std::cout << to << " 에 내용이 저장되었습니다" << std::endl;
    }
    else if (command == "exit")
    {
        return 0;
    }

    return 1;
}

// 사용자의 입력을 계속 기다리면서 내용이 업데이트 될 때 마다 화면에 표를 출력
void Excel::command_line()
{
    string s;
    std::getline(cin, s);

    while (parse_user_input(s))
    {
        std::cout << *current_table << std::endl
                  << ">> ";
        getline(cin, s);
    }
}

int main() {
  std::cout
      << "테이블 (타입) (최대 행 크기) (최대 열 크기) 를 순서대로 입력해주세요"
      << std::endl;
  std::cout << "* 참고 * " << std::endl;
  std::cout << "1 : 텍스트 테이블, 2 : CSV 테이블, 3 : HTML 테이블"
            << std::endl;

  int type, max_row, max_col;
  std::cin >> type >> max_row >> max_col;
  MyExcel::Excel m(max_row, max_col, type - 1);
  m.command_line();
}

/*
문제 1
ExprCell 의 쉭에서 셀의 이름은 A3 과 같이 단 두 글자만 가능하다는 제약 조건이 있었습니다. 이를 임의의 크기의 이름도 가능하게 확장해보세요. (난이도 : 下)

문제 2
마찬가지로 가능한 숫자도 임의의 길이가 상관없게 확장해보세요. (난이도 : 下)

문제 3
사실 위와 같이 수식을 계산하는 경우 한 가지 문제가 있습니다. 바로 셀들이 서로를 참조할 수 있다는 것입니다. 
예를 들어서 A1 = B1 이고 B1 = A1 으로 설정하였다면 B1 의 값을 알기 위해 A1 의 값을 알아야 하고, 
그럼 A1 의 값을 알기 위해 B1 의 값을 알아야 하고... 와 같은 순환 참조 문제가 발생합니다.

따라서 사용자가 타의든 자의든 순환 참조가 있는 식을 입력하였을 때 이를 감지하고 입력을 방지하는 루틴을 제공해야 합니다. 
(실제 Excel 에서도 순환 참조되는 식을 입력하면 오류가 발생합니다) (난이도 : 上)

문제 4
실제 Excel 의 경우 수식에서 여러가지 함수들을 지원합니다. 여기서도 수식에서 간단한 함수들을 지원하게 해보세요.. (난이도 : 上)
*/
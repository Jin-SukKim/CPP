class ExprCell : public Cell
{
    string data;
    string *parsed_expr;

    Vector exp_vec;

    // 입력받은 연산자 우선 순위를 반환합니다.
    int precedence(char c);

    // 수식을 분석합니다. 계산하기 편하게 해주는 함수
    // 중위 표기법으로 입력하고 후위 표기법으로 변환해주는 함수
    void parse_expression();

public:
    ExprCell(string data, int x, int y, Table *t);

    string stringify();

    // 후위 표기법 계산
    /*
        우리가 평소에 사용하는 계산방법을 중위 표기법이라고 한다(ex: 2 + 5 * (4 + 3) + 2 / 9). 연산자가 중간에 들어가 있다.
        이런 경우 /,*를 +-보다 먼저 계산해야되는 걸 알 수 있는데 컴퓨터는 이런 것을 하기 힘들다.

        반면 컴퓨터가 해석하기 쉬운 후위 표기법으로 나타내면 좀 더 쉽게 코딩이 가능하다(ex: 3 4 5 * + 4 7 2 - * +).
        연산자들이 피연산자 뒤쪽에 위치해 있다는 점과, 또 하나는 괄호가 사라졌다. 읽어들이는 순서 대로 계산이 가능하다.
        후위 표기법 계산법
        1. 피연산자를 만나면 스택(FILO)에 push 합니다. (스택에 3 4 5 순서대로 저장)
        2. 연산자를 만나면 스택에서 두 개를 pop 한 뒤에 그 둘에 해당 연산을 한 후, 그 결과를 다시 스택에 push 합니다.
            (4 5 를 뺸 후 *로 계산 후 저장 -> 계산 된 값과 3 을 뺸 후 + 계산 -> ...)

    */
    int to_numeric();
};

// 후위 표기법으로 변환 후 exp_vec에 저장 
// exp_vec은 벡터 클래스 객체로 각각의 원소가 후위 표기법으로 변환된 수식의 각각의 토큰이 된다. 
// (ex: 3, 4, 5, *, +, 4, 7, 2, -, *, +로 만든 배열)
int ExprCell::to_numeric()
{
    double result = 0;
    NumStack stack;

    // 각 토큰에 접근
    for (int i = 0; i < exp_vec.size(); i++)
    {
        string s = exp_vec[i];

        // 셀 일 경우
        // 셀 이름 (A3, B2, ..) 이나 숫자일 경우 스택에 push
        if (isalpha(s[0]))
        {
            stack.push(table->to_numeric(s));
        }
        // 숫자 일 경우 (한 자리라 가정)
        else if (isdigit(s[0]))
        {
            stack.push(atoi(s.c_str()));
        }
        // 연산자를 만날 경우 스택에서 두 번 pop 을 해서 해당하는 피연산자들에 해당 연산자를 적용해서 다시 스택에 push
        else
        {
            double y = stack.pop();
            double x = stack.pop();
            switch (s[0])
            {
            case '+':
                stack.push(x + y);
                break;
            case '-':
                stack.push(x - y);
                break;
            case '*':
                stack.push(x * y);
                break;
            case '/':
                stack.push(x / y);
                break;
            }
        }
    }
    return stack.pop(); // 최종 결과값을 pop 하며 이를 리턴
}

// 연산자의 우선 순위 (숫자가 클수록 상위 우선순위)
int ExprCell::precedence(char c)
{
    switch (c)
    {
    case '(':
    case '[':
    case '{':
        return 0;
    case '+':
    case '-':
        return 1;
    case '*':
    case '/':
        return 2;
    }
    return 0;
}

/*
    변환 방법
    1. 피연산자 (셀 이름이나 숫자) 일 경우 그냥 exp_vec 에 넣습니다.

    2. 여는 괄호( (, [, { 와 같은 것들 ) 을 만날 경우 스택에 push 합니다.

    3. 닫는 괄호( ), ], } 와 같은 것들 ) 을 만날 경우 여는 괄호가 pop 될 때 까지 pop 되는 연산자들을 exp_vec 에 넣습니다.

    4. 연산자일 경우 자기 보다 우선순위가 낮은 연산자가 스택 최상단에 올 때 까지 (혹은 스택이 빌 때 까지) 
       스택을 pop 하고 (낮은 것은 pop 하지 않습니다), pop 된 연산자들을 exp_vec 에 넣습니다. 그리고 마지막에 자신을 스택에 push 합니다.
*/
// 사용자가 입력하는 숫자는 1 자리 정수이고, 셀 이름 역시 2자 로 제한 (나중에 확장 가능)
void ExprCell::parse_expression()
{
    Stack stack;

    // 수식 전체를 () 로 둘러 사서 exp_vec 에 남아있는 연산자들이 push 되게 해줍니다.
    data.insert(0, "(");
    data.push_back(')');

    for (int i = 0; i < data.length(); i++)
    {
        // 피연산자를 만날 경우 exp_vec 에 무조건 집어넣는다.
        if (isalpha(data[i]))
        {
            exp_vec.push_back(data.substr(i, 2));
            i++;
        }
        else if (isdigit(data[i]))
        {
            exp_vec.push_back(data.substr(i, 1));
        }
        // 괄호의 경우 여는 괄호를 만나면 스택에 push 하고, 닫는 괄호를 만나면 위 처럼 여는 괄호가 스택에서 나올 때 까지 pop 하고, 
        // 그 pop 한 연산자들을 벡터에 넣으면 됩니다. 주의할 점은 pop 한 연산자가 괄호일 경우 넣지 않는다는 점입니다.
        else if (data[i] == '(' || data[i] == '[' ||
                 data[i] == '{')
        { // Parenthesis
            stack.push(data.substr(i, 1));
        }
        else if (data[i] == ')' || data[i] == ']' || data[i] == '}')
        {
            string t = stack.pop();
            while (t != "(" && t != "[" && t != "{")
            {
                exp_vec.push_back(t);
                t = stack.pop();
            }
        }
        // 연산자일 경우를 살펴봅시다. peek 의 경우 스택의 최상단 원소를 pop 하지는 않고 무엇인지만 살펴보는 것입니다. 
        // 만일 최상단 원소의 우선 순위가 현재 연산자의 우선순위 보다 높다면 이를 스택에서 pop 하고 이를 exp_vec 에 넣어야 겠지요. 
        // 위 while 문은 그 과정을 나타내고 있습니다.
        else if (data[i] == '+' || data[i] == '-' || data[i] == '*' ||
                 data[i] == '/')
        {
            while (!stack.is_empty() &&
                   precedence(stack.peek()[0]) >= precedence(data[i]))
            {
                exp_vec.push_back(stack.pop());
            }
            // 맨 마지막에 스택에 현재 연산자를 넣습니다.
            stack.push(data.substr(i, 1));
        }
    }

    
}

int main()
{
    MyExcel::TxtTable table(5, 5);
    table.reg_cell(new NumberCell(2, 1, 1, &table), 1, 1);
    table.reg_cell(new NumberCell(3, 1, 2, &table), 1, 2);

    table.reg_cell(new NumberCell(4, 2, 1, &table), 2, 1);
    table.reg_cell(new NumberCell(5, 2, 2, &table), 2, 2);
    table.reg_cell(new ExprCell("B2+B3*(C2+C3-2)", 3, 3, &table), 3, 2);
    table.reg_cell(new StringCell("B2 + B3 * ( C2 + C3 - 2 ) = ", 3, 2, &table),
                   3, 1);

    std::cout << table;
}
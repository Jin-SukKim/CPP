// 
/*
    std::move()함수를 이용해 강제적으로 rvalue reference를 인자로 받는 이동생성자를 호출한다.
    my_swap() 함수에서 메모리 낭비를 최적화하기 위해 사용했다. 복사생성자보다 빠르다.

    operator=를 오버로딩해 rvalue reference를 이용해 값을 이동할 수 있게 만들었다.
*/
#include <iostream>
#include <cstring>

class MyString
{
    char *string_content; // 문자열 데이터를 가리키는 포인터
    int string_length;    // 문자열 길이

    int memory_capacity; // 현재 할당된 용량

public:
    MyString();

    // 문자열로 부터 생성
    MyString(const char *str);

    // 복사 생성자
    MyString(const MyString &str);

    // 이동 생성자
    MyString(MyString &&str);

    // 일반적인 대입 연산자
    MyString &operator=(const MyString &s);

    // 이동 대입 연산자
    MyString &operator=(MyString &&s);

    ~MyString();

    int length() const;

    void println();
};

MyString::MyString()
{
    std::cout << "생성자 호출 ! " << std::endl;
    string_length = 0;
    memory_capacity = 0;
    string_content = NULL;
}

MyString::MyString(const char *str)
{
    std::cout << "생성자 호출 ! " << std::endl;
    string_length = strlen(str);
    memory_capacity = string_length;
    string_content = new char[string_length];

    for (int i = 0; i != string_length; i++)
        string_content[i] = str[i];
}
MyString::MyString(const MyString &str)
{
    std::cout << "복사 생성자 호출 ! " << std::endl;
    string_length = str.string_length;
    string_content = new char[string_length];

    for (int i = 0; i != string_length; i++)
        string_content[i] = str.string_content[i];
}
MyString::MyString(MyString &&str)
{
    std::cout << "이동 생성자 호출 !" << std::endl;
    string_length = str.string_length;
    string_content = str.string_content;
    memory_capacity = str.memory_capacity;

    // 임시 객체 소멸 시에 메모리를 해제하지
    // 못하게 한다.
    str.string_content = nullptr;
    str.string_length = 0;
    str.memory_capacity = 0;
}
MyString::~MyString()
{
    if (string_content)
        delete[] string_content;
}
MyString &MyString::operator=(const MyString &s)
{
    std::cout << "복사!" << std::endl;
    if (s.string_length > memory_capacity)
    {
        delete[] string_content;
        string_content = new char[s.string_length];
        memory_capacity = s.string_length;
    }
    string_length = s.string_length;
    for (int i = 0; i != string_length; i++)
    {
        string_content[i] = s.string_content[i];
    }

    return *this;
}

/*
    rvalue reference를 통해 이동 연산자를 만들었다.
    전체 문자열을 복사가 아닌 기존 문자열을 string_content에 복사했다.
*/
MyString &MyString::operator=(MyString &&s)
{
    std::cout << "이동!" << std::endl;
    string_content = s.string_content;
    memory_capacity = s.memory_capacity;
    string_length = s.string_length;

    s.string_content = nullptr;
    s.memory_capacity = 0;
    s.string_length = 0;
    return *this;
}
int MyString::length() const { return string_length; }
void MyString::println()
{
    for (int i = 0; i != string_length; i++)
        std::cout << string_content[i];

    std::cout << std::endl;
}

// lvalue를 rvalue로 변환시켜 사용
// std::move()를 통해 이동한것이 아닌 이동 생성자나 이동 대입 연산자를 호출할 때 수행한다.
template <typename T>
void my_swap(T &a, T &b)
{   
    // 이동생성자이기 때문에 복사생성자보다 빠르다.
    T tmp(std::move(a));

    // a에 b를 이동하고, b에 tmp를 이동시켰다.
    // operator=를 오버로딩해 대입이 아닌 이동이 되었다.
    a = std::move(b);
    b = std::move(tmp);
}
int main()
{
    MyString str1("abc");
    MyString str2("def");
    std::cout << "Swap 전 -----" << std::endl;
    std::cout << "str1 : ";
    str1.println();
    std::cout << "str2 : ";
    str2.println();

    std::cout << "Swap 후 -----" << std::endl;
    my_swap(str1, str2);
    std::cout << "str1 : ";
    str1.println();
    std::cout << "str2 : ";
    str2.println();
}
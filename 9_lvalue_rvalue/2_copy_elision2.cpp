/*
    nullptr은 NULL을 대체할 수 있는 키워드 이다.
    NULL은 단순히 #define으로 정의되어 있는 상수값 0이다.
    그러므로 NULL은 값 0을 의미하는지 포인터 주소값 0을 의미하는지 헷갈리수 있다.

    하지만 nullptr로 포인터 주소값 0을 표현해주면 실수를 줄여줄 수 있다.
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

    void reserve(int size);
    MyString operator+(const MyString &s);
    ~MyString();

    int length() const;

    void print();
    void println();
};

MyString::MyString()
{
    std::cout << "생성자 호출 ! " << std::endl;
    string_length = 0;
    memory_capacity = 0;
    string_content = nullptr;   // 기존의 null을 대체하는 키워드
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
    memory_capacity = str.string_length;
    string_content = new char[string_length];

    for (int i = 0; i != string_length; i++)
        string_content[i] = str.string_content[i];
}
MyString::~MyString() { delete[] string_content; }
void MyString::reserve(int size)
{
    if (size > memory_capacity)
    {
        char *prev_string_content = string_content;

        string_content = new char[size];
        memory_capacity = size;

        for (int i = 0; i != string_length; i++)
            string_content[i] = prev_string_content[i];

        if (prev_string_content != nullptr)
            delete[] prev_string_content;
    }
}

/*
    복사생략 최적화가 수행될 수도 있지만 수행되지 않은 함수
    str에 str1 + str2를 넣는데 새로운 str이 복사생성된다 하지만 이 경우 굳이 생성하지 않고도 str1 + str2의 return값을
    str에 그대로 넣으면 되지만 컴파일러는 복사생략을 수행하지 않았따. 그럼 메모리할당과 해제로 시간과 공간이 소모될수 있다.
    이 문제를 C++은 lvalue와 rvalue로 해결했다.
*/
MyString MyString::operator+(const MyString &s)
{
    MyString str;
    str.reserve(string_length + s.string_length);
    for (int i = 0; i < string_length; i++)
        str.string_content[i] = string_content[i];
    for (int i = 0; i < s.string_length; i++)
        str.string_content[string_length + i] = s.string_content[i];
    str.string_length = string_length + s.string_length;
    return str;
}

int MyString::length() const { return string_length; }
void MyString::print()
{
    for (int i = 0; i != string_length; i++)
        std::cout << string_content[i];
}
void MyString::println()
{
    for (int i = 0; i != string_length; i++)
        std::cout << string_content[i];

    std::cout << std::endl;
}

int main()
{
    MyString str1("abc");
    MyString str2("def");
    std::cout << "-------------" << std::endl;
    MyString str3 = str1 + str2;
    str3.println();
}
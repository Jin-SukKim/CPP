/*
    str3 = str1 + str2 코드가 작성될 시 copy elision의 최적화가 실행되지 않는 것을 고치기 위해선
    str3 생성 시에 임시로 생성된 (str1 + str2) 객체의 string_content가 가리키는 문자열의 주소값을 str3의 string_content로 바꿔주면 된다.

    문제는 임시 객체(str1 + st2)가 소멸 시 string_content를 메모리에서 해제하는데, 
    이 경우 str3가 가리키고 있던 문자열이 메모리에서 소멸된다.(포인터가 가르키고 있는게 없어진다. 즉 str3의 string_content도 소멸된다.)
    
    이를 방지하기 위해선, 임시 생성된 객체의 string_content를 nullptr로 바꿔주고,
    소멸자에서 string_content가 nullptr이면 소멸하지 않도록 해주야된다.

    하지만, 이 방법은 기존의 복사 생성자에서는 사용할 수 없다.
    복사 생성자는 인자를 const MyString&으로 받기 때문에, 인자의 값을 변경할 수 없게 된다.
    즉 임시 객체의 string_content값을 수정할 수 없기 때문에 문제가 된다.

    이 문제는 const MyString&이 lvalue와 rvalue 모두 받을 수 있다는 점 때문에 발생했다.(const 값은 lvalue와 rvalue모두 인자로 받을 수 있다.)
    이것을 해결하기 위해 C++ 11부터 제공하는 rvalue reference를 이용해 rvalue만 특이적으로 받게 만들어야 된다.

    MyString::MyString(MyString&& str) 라는 rvalue reference를 받는 이동생성자를 만들었다.

    rvalue reference를 받기 위해서는 &을 두 번 사용해 정의해야 된다.

    하지만 MyString&& str의 str은 lvalue이다. 실체가 있기 때문이다(이름을 가지고 주소값도 가지고있다.)
    즉 str은 type이 'MyString의 rvalue reference'인 lvalue라고 보면된다.
    따라서 lvalue처럼 좌측에도 올 수 있다.

    소멸자도 nullptr이 아닌 경우만 delete하도록 바꿔준다.

    일반적인 rvalue reference 사용법

        int a;
        int& l_a = a;
        int& ll_a = 3;  // 불가능

        int&& r_b = 3;
        int&& rr_b = a;  // 불가능

    rvalue reference는 반드시 rvalue의 reference만 가능하다. (lvalue는 불가능)

    rvalue reference의 특징은 reference하는 임시 객체가 소멸되지 않도록 붙들고 있다는 점이다.
    ex)
        MyString&& str3 = str1 + str2;
        str3.println();

        str3이 str1 + str2를 return되는 임시 객체의 reference가 되면서 그 임시 객체가 소멸되지 않도록 한다.
        println 함수에서 더해진 문자열이 이 예제를 잘 보여준다.
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
    string_content = nullptr;
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

// rvalue reference를 사용한 이동 생성자
/*

*/
MyString::MyString(MyString &&str)
{
    std::cout << "이동 생성자 호출 !" << std::endl;
    string_length = str.string_length;

    // 임시 객체의 string_content가 가르키는 메모리를 새로 생긴 메모리로 옮겨준다.
    // 기존 복사생성자는 문자열 전체를 새로 복사해야 되지만 이 경우 단순히 주소값만 복사해주면 끝나므로 간단하다.
    string_content = str.string_content;
    memory_capacity = str.memory_capacity;

    // rvalue reference이지만 str은 type이 MyString rvalue reference인 lvalue 이므로 좌측에 올 수 있다.
    // 임시 객체 소멸 시에 메모리를 해제하지 못하게 한다.
    str.string_content = nullptr;
}

// string_content가 nullptr이 아닐때만 delete를 한다.   
MyString::~MyString()
{
    if (string_content)
        delete[] string_content;
}

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
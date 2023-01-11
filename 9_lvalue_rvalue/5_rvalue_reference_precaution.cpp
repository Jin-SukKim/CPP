// 이동 생성자 작성 시 주의할 점
/*
    만약 MyString을 C++ Container들(vector etc)에 넣기 위해 주의할 점이 있다.
    이동 생성자를 반드시 noexcept로 명시해줘야 한다는 점이다.

    vector는 새로운 원소를 추가할 때, 할당한 메모리가 부족하면 새로 할당한 후에 기존 원소들을 옮겨준다.
    복사 생성자를 사용할 경우 원소가 하나씩 복사된다.

    만약 이 과정에서 예외가 발생한다면 새로 할당한 메모리를 소멸시킨 후 사용자에게 예외를 전달하면 된다.
    새로 할당한 메모리를 소멸시키는 과정에서 이미 복사된 원소들도 소멸시키므로 자원(메모리)가 낭비되는 일은 없다.

    반면 이동 생성자를 사용했을 경우 예외가 발생한다면 문제가 생긴다.
    복사 생성자를 사용하면 새로 할당한 메모리를 소멸시켜도 기존의 메모리에 원소들이 존재해 문제가 없지만
    이동 생성자는 기존의 메모리에 있던 원소들이 이동되어 사라졌기 때문에 새로 할당한 메모리를 섯불리 해제할 수 없다.

    따라서 Container(vector, list, etc)의 경우 이동 생성자에서 예외가 발생했을 때 이를 제대로 처리할 수 없다.
    이러한 이유로 인해 vector는 이동 생성자가 noexcept가 아닌 이상 이동 생성자를 사용하지 않는다.

*/
#include <iostream>
#include <cstring>
#include <vector>

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

    ~MyString();
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

// vector는 noexcept가 아닌이상 이동 생성자를 호출하지 않고 복사 생성자를 사용한다.
MyString::MyString(MyString &&str)
{
    std::cout << "이동 생성자 호출 !" << std::endl;
    string_length = str.string_length;
    string_content = str.string_content;
    memory_capacity = str.memory_capacity;

    // 임시 객체 소멸 시에 메모리를 해제하지
    // 못하게 한다.
    str.string_content = nullptr;
}

// noexcept를 해줘야 호출한다.
/*
MyString::MyString(MyString &&str) noexcept
{
    std::cout << "이동 생성자 호출 !" << std::endl;
    string_length = str.string_length;
    string_content = str.string_content;
    memory_capacity = str.memory_capacity;

    // 임시 객체 소멸 시에 메모리를 해제하지
    // 못하게 한다.
    str.string_content = nullptr;
}
*/

MyString::~MyString()
{
    if (string_content)
        delete[] string_content;
}

int main()
{
    MyString s("abc");
    std::vector<MyString> vec;
    vec.resize(0);

    // push_back 복사생성자 호출
    std::cout << "첫 번째 추가 ---" << std::endl;
    vec.push_back(s);
    std::cout << "두 번째 추가 ---" << std::endl;

    // push_back 함수에서 s를 기반을 복사생성자 호출 후 이동 생성자로 기존 vec값 이동
    vec.push_back(s);
    std::cout << "세 번째 추가 ---" << std::endl;

    // push_back 함수에서 s를 기반으로 복사생성자 호출 후
    // 원소가 하나씩 복사되므로 이미 원소가 2개인 vec은 이동생성자 두번 호출
    vec.push_back(s);
}
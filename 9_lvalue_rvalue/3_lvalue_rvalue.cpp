// 좌측값 (lvalue) 와 우측값 (rvalue)
/*
    모든 C++의 Expression(표현식)은 두 가지 카테고리로 구분할 수 있다.

        1. 이 구문이 어떤 타입을 가지고 있는가
        2. 어떠한 종류의 값을 가지고 있는가

    ex) int a = 3;
    a는 메모리 상에 존재하는 변수, 즉 a의 주소값을 &a를 통해 알아낼 수 있다.

    보통 이렇게 주소값을 취할 수 있는 값을 lvalue(좌측값)이라고 한다.
    lvalue는 어떠한 표현식의 왼쪽 오른쪽 모두에 올 수 있다.(무조건 왼쪽에 있어야하는게 아니다.)

    3은 주소값을 취할 수 없는 상수값이다. 즉 a와 다르게 표현식 연산시에 잠깐 존재할 뿐인 실체가 없는 값이다.

    주소값을 취할 수 없는 값을 rvalue(우측값)이라고 한다.
    우측값은 오른쪽에만 올 수 있다. 좌측값과 같이 왼쪽에 존재해서는 안된다.

    ex)
        int a;         // a 는 좌측값
        int& l_a = a;  // l_a 는 좌측값 레퍼런스

        int& r_b = 3;  // 3 은 우측값. 따라서 오류

    결국 Reference는 lvalue에만 가질 수 있다.
    이와 같이 &를 이용해 정의하는 reference를 lvalue reference라고 하고 좌측값 reference도 좌측값이 된다.

    2_copy_elision2.cpp의 MyString str3 = str1 + str2; 는 MyString str3(str1.operator+(str2));와 동일하다.
    operator+의 정의를 보면 MyString MyString::operator+(const MyString &s)로 우측값을 return하고 있다.
    operator+를 보면 lvalue reference를 인자로 받는데 rvalue가 들어가는 것을 알 수 있다.
    rvalue가 lvalue reference를 인자로 받는 MyString(const MyString &str);를 호출할 수 없는데
    예외적으로 const T& 의 타입에 한해서 rvalue로 reference로 받을 수 있다.

    이유는 const reference이기 때문에 임시로 존재하는 객체의 값을 참조할 뿐 이를 변경할 수 없기 때문이다.
*/

// lvalue reference return
// 결국 lvalue return과 비슷하다.
int &func1(int &a) { return a; }

// 일반값 int를 return
int func2(int b) { return b; }

int main()
{
    int a = 3;
    // func1(a)가 return하는 reference값을 4로 바꾸라는 의미로 a가 4로 변한다.
    func1(a) = 4;   
    // lvalue reference를 return하므로 return 값의 주소값 역시 알아낼 수 있다.
    std::cout << &func1(a) << std::endl;

    int b = 2;
    
    // 일반 int값을 return 하므로 문장 실행이 끝나면 없어진다.
    // 즉 실체가 없는 값인 우측값(rvalue)라 오른쪽에만 올 수 있다.
    a = func2(b);                        // 가능
    func2(b) = 5;                        // 오류 1
    std::cout << &func2(b) << std::endl; // 오류 2
}
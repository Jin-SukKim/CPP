// exception handling (예외처리) - throw, 예외 처리 하기 - try 와 catch
/*
    std::vector<int> v(3);  // 크기가 3 인 벡터 만듦
    std::cout << v.at(4);   // ??

    std::vector<int> v(1000000000); // ?
    이런 것들과 같이 문법상 문제는 없지만 사용자의 실수나 개발자의 실수 등으로 일어날 수 있는
    정상적인 상황을 벗어난 예외적인 상황을 처리하기 위해 만들어 졌다.

    기존의 C에선 언어 차원에서 제공되는 예외 처리형식이 없었따.
    char *c = (char *)malloc(1000000000);
    if (c == NULL) {
        printf("메모리 할당 오류!");
        return;
    }
    와 같이 결과등을 확인해 예외를 처리했는데 코드가 복잡해 함수가 깊어진다면 매우 불편해진다

    C에선 예외처리를 위해 다른 값을 return해 처리했지만
    C++에선 throw문을 사용해 예외를 명시적으로 나타냈다.

    throw 로 예외로 전달하고 싶은 객체를 써주면 된다.
    예외로 아무것이나 던져도 되지만 std lib에서 여러 종류의 예외를 표준으로 정의되어 있다.
    (out_of_range, overflow_error, length_error, runtime_error 등등)

    throw문이 실행되면 즉시 함수가 종료되고 예외처리하는 부분까지 점프해 throw밑의 코드들은 작동하지 않는다.
    한가지 중요한 점은 함수에서 예외처리하는 부분에 도달하기까지 빠져나가면서 stack에 생성되었던 객체들을 빠짐없이 소멸시켜준다.
    (물론 소멸자들이 제대로 작성되어 있어야된다.)

    try - catch 문을 사용해 throw한 예외를 처리해준다
        try 안에서 무언가 예외가 발생할만한 코드가 실행
    
    만일 예외가 발생하지 않았다면 try - catch가 없던것처럼 코드가 실행된다.
    그리고 catch문은 무시된다.
    
    만약 예외가 발생한다면 stack에 생성된 모든 객체들의 소멸자들이 호출되고, 가장 가까운 catch문이 실행된다.
    즉 throw가 실행된 이후에 catch문으로 점프된다.

    catch문 안에 정의된 예외에 맞는 객체를 받게된다.
*/

#include <iostream>
#include <stdexcept>

template <typename T>
class Vector
{
public:
    Vector(size_t size) : size_(size)
    {
        data_ = new T[size_];
        for (int i = 0; i < size_; i++)
        {
            data_[i] = 3;
        }
    }
    const T &at(size_t index) const
    {
        // at 함수는 const T&를 return 하기 때문에 오류메시지를 return할 수 없어서 throw문을 사용
        if (index >= size_)
        {   
            // 예외 발생
            throw std::out_of_range("vector 의 index 가 범위를 초과하였습니다.");
        }
        return data_[index];
    }
    ~Vector() { delete[] data_; }

private:
    T *data_;
    size_t size_;
};
int main()
{
    Vector<int> vec(3);

    int index, data = 0;
    std::cin >> index;

    try
    {
        data = vec.at(index);
    }
    catch (std::out_of_range &e)
    {
        std::cout << "예외 발생 ! " << e.what() << std::endl;
    }
    // 예외가 발생하지 않았다면 3을 이 출력되고, 예외가 발생하였다면 원래 data 에
    // 들어가 있던 0 이 출력된다.
    std::cout << "읽은 데이터 : " << data << std::endl;
}
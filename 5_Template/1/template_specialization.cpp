/*
Vector<bool> int_vec;
C++ 에서 기본으로 처리하는 단위가 1 byte이다.
bool은 1bit만으로 충분히 저장 가능한데 8bit를 모두 사용해서 저장하면 메모리가 낭비되는 것과 같다.
따라서 template<bool>같은건 특별히 따로 처리하는데 일부 이러한 경우들이 존재한다.
*/

/*

template <typename A, typename B, typename C>
class test {};
만약 이러한 template이 있고 A와 C를 int와 double일 때 따로 처리하고 싶다고 한다면

template <typename B>
class test<int, B, double> {}; 와 같이 처리하면된다.

B도 같이 int로 특수화 처리하고 싶다하면

template <>
class test<int, int, double> {};로 해준다.

전달하는 템플릿 인자가 없더라도 특수화 하고 싶다면 template<> 라도 남겨줘야 된다

즉 bool과 같은 경우

template<>
class Vctor<bool> {}; 와 같이 처리해주면 된다.
*/

// int 배열을 이용해 bool 데이터들을 저장. int는 4 byte = 32 bits 이므로 32개의 bool 값을 저장할 수 있다.
// 즉 bool 값을 알기 위해 정확이 1bit만 int에서 뽑아서 사용해야 되는 것이다.
#include <iostream>
#include <string>

template <typename T>
class Vector
{
    T *data;
    int capacity;
    int length;

public:
    // 어떤 타입을 보관하는지
    typedef T value_type; // 즉 별칭을 지어주는 것과 같다.

    // 생성자
    Vector(int n = 1) : data(new T[n]), capacity(n), length(0) {}

    // 맨 뒤에 새로운 원소를 추가한다.
    void push_back(T s)
    {
        if (capacity <= length)
        {
            T *temp = new T[capacity * 2];
            for (int i = 0; i < length; i++)
            {
                temp[i] = data[i];
            }
            delete[] data;
            data = temp;
            capacity *= 2;
        }

        data[length] = s;
        length++;
    }

    // 임의의 위치의 원소에 접근한다.
    T operator[](int i) { return data[i]; }

    // x 번째 위치한 원소를 제거한다.
    void remove(int x)
    {
        for (int i = x + 1; i < length; i++)
        {
            data[i - 1] = data[i];
        }
        length--;
    }

    // 현재 벡터의 크기를 구한다.
    int size() { return length; }

    ~Vector()
    {
        if (data)
        {
            delete[] data;
        }
    }
};

// N 번째 bool 데이터는 N / 32 번째 int 에 들어가 있고, 그 안에서 정확히 N % 32 번째 비트이다.
template <>
class Vector<bool>
{
    unsigned int *data; // shift 연산 시 값 오류 발생할 수 있어 unsigned int 사용
    int capacity;       // 배열의 크기
    int length;         // bool 데이터 길이 (즉 1 capacity당 32 length의 bool 값이 들어갈 수 있다.)

public:
    typedef bool value_type;

    // 생성자
    Vector(int n = 1)
        : data(new unsigned int[n / 32 + 1]), capacity(n / 32 + 1), length(0)
    {
        for (int i = 0; i < capacity; i++)
        {
            data[i] = 0;
        }
    }

    // 맨 뒤에 새로운 원소를 추가한다.
    void push_back(bool s)
    {
        if (capacity * 32 <= length)
        {
            unsigned int *temp = new unsigned int[capacity * 2];
            for (int i = 0; i < capacity; i++)
            {
                temp[i] = data[i];
            }
            for (int i = capacity; i < 2 * capacity; i++)
            {
                temp[i] = 0;
            }

            delete[] data;
            data = temp;
            capacity *= 2;
        }

        // bool이 true인 경우 or 연산을 하면 그 비트는 무조건 1이 된다.
        //  OR 연산은 특정 비트에만 선택적으로 1로 바꾸는데 매우 좋은 연산이다.
        if (s)
        {
            // >>, <<를 이용해 shift 연산을 할 수 있다. 밑의 코드는 왼쪽으로 1칸씩 shift하겠다는 의미이다.
            // 1 << (length % 32) 을 통해서 정확히 length % 32 번째 비트만 1 로 바꾼뒤 or로 해당 위치를 1로한다.
            data[length / 32] |= (1 << (length % 32)); 
        }

        length++;
    }

    // 임의의 위치의 원소에 접근한다.
    // AND 연산을 통해 해당 위치의 bit값을 가져올 수 있다. 즉 해당 위치에 1을 AND 연산하면 bit는 0일때 0, 1일때 1을 return한다.
    bool operator[](int i) { return (data[i / 32] & (1 << (i % 32))) != 0; }

    // x 번째 위치한 원소를 제거한다.
    void remove(int x)
    {
        for (int i = x + 1; i < length; i++)
        {
            int prev = i - 1;
            int curr = i;

            // 만일 curr 위치에 있는 비트가 1 이라면
            // prev 위치에 있는 비트를 1 로 만든다.
            if (data[curr / 32] & (1 << (curr % 32)))
            {
                data[prev / 32] |= (1 << (prev % 32));
            }
            // 아니면 prev 위치에 있는 비트를 0 으로 지운다.
            else
            { 
                unsigned int all_ones_except_prev = 0xFFFFFFFF; // 모두를 1로 바꾼다.
                all_ones_except_prev ^= (1 << (prev % 32)); //  XOR 연산자로, 두 비트가 같으면 0, 다르면 1 이되는 연산 
                data[prev / 32] &= all_ones_except_prev;    // AND를 이용해 원하는 bit만 0으로 만든다.
            }
        }
        length--;
    }

    // 현재 벡터의 크기를 구한다.
    int size() { return length; }
    ~Vector()
    {
        if (data)
        {
            delete[] data;
        }
    }
};

int main()
{
    // int 를 보관하는 벡터를 만든다.
    Vector<int> int_vec;
    int_vec.push_back(3);
    int_vec.push_back(2);

    std::cout << "-------- int vector ----------" << std::endl;
    std::cout << "첫번째 원소 : " << int_vec[0] << std::endl;
    std::cout << "두번째 원소 : " << int_vec[1] << std::endl;

    Vector<std::string> str_vec;
    str_vec.push_back("hello");
    str_vec.push_back("world");
    std::cout << "-------- std::string vector -------" << std::endl;
    std::cout << "첫번째 원소 : " << str_vec[0] << std::endl;
    std::cout << "두번째 원소 : " << str_vec[1] << std::endl;

    Vector<bool> bool_vec;
    bool_vec.push_back(true);
    bool_vec.push_back(true);
    bool_vec.push_back(false);
    bool_vec.push_back(false);
    bool_vec.push_back(false);
    bool_vec.push_back(true);
    bool_vec.push_back(false);
    bool_vec.push_back(true);
    bool_vec.push_back(false);
    bool_vec.push_back(true);
    bool_vec.push_back(false);
    bool_vec.push_back(true);
    bool_vec.push_back(false);
    bool_vec.push_back(true);
    bool_vec.push_back(false);
    bool_vec.push_back(true);
    bool_vec.push_back(false);

    std::cout << "-------- bool vector ---------" << std::endl;
    for (int i = 0; i < bool_vec.size(); i++)
    {
        std::cout << bool_vec[i];
    }
    std::cout << std::endl;
}
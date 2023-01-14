// Structured binding (C++ 17 이상)
/*
    C++ 17부터는 structured binding이라는 테크닉이 추가되어
    tuple을 좀 더 편리하게 다룰 수 있게 되었다.

    tuple에서는 값을 얻기 위해 일일히 std::get<>()을 사용해 값을 가져와야 했다.
    하지만 structured binding 방식을 사용하면 더 간단하게 표현할 수 있다.
    ex) 기존 std::get을 이용해 일일히 값 가져오기
        int age = std::get<0>(student);
        std::string name = std::get<1>(student);
        bool is_male = std::get<2>(student);

        structured binding 방법 사용하기
        auto [age, name, is_male] = student;
        std::cout << name << age << std::boolalpha << is_male << std::endl;

    structured binding 사용법 :
        auto / & 혹은 && 도 가능 / [/ tuple 안에 원소들을 받기 위한 객체 /] = tp;

        tuple안에 객체들을 복사하지 않고 reference만 받고 싶다면
        auto& [age, name, is_male] = student;

    여기서 중요한 점은 tuple의 모든 원소들을 반드시 받아와야 된다는 점이다.
    structured binding을 사용해 중간의 원소만 가져올 수 없다.

    이 방식의 쓰임새는 매우 많다.
    꼭 tuple 말고도 데이터 멤버들이 정의되어 있는 structure의
    데이터 필드들을 받는데도 사용 가능하다.
    ex)
        struct Data {
            int i;
            std::string s;
            bool b;
        };

        Data d;
        auto [i, s, b] = d;

    pair와 같은 클래스들 역시 structured binding을 사용할 수 있다.
    ex)
        std::map<int, std::string> m = {{3, "hi"}, {5, "hello"}};
        for (auto& [key, val] : m) {
            std::cout << "Key : " << key << " value : " << val << std::endl;
        }

        기존에는 iterator로 받아 first와 second로 키와 대응되는 값을 나타냈지만
        structured binding을 사용해 훨씬 간단하게 사용했다.
*/
#include <iostream>
#include <string>
#include <tuple>

std::tuple<int, std::string, bool> GetStudent(int id)
{
    if (id == 0)
    {
        return std::make_tuple(30, "철수", true);
    }
    else
    {
        return std::make_tuple(28, "영희", false);
    }
}

int main()
{
    auto student = GetStudent(1);

    int age = std::get<0>(student);
    std::string name = std::get<1>(student);
    bool is_male = std::get<2>(student);

    std::cout << "이름 : " << name << std::endl;
    std::cout << "나이 : " << age << std::endl;
    std::cout << "남자 ? " << std::boolalpha << is_male << std::endl;
}
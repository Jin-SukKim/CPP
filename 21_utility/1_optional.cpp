// std::optional (C++ 17 이상)
/*
    std::optional을 이용해 원하는 값을 보관할 수도, 안 할수도 있는 클래스를 만들수 있다.

    std::optional 정의 :
        std::optional<type>

        ex) std::optional<std::string>
        템플릿 인자로 보관하고자 하는 객체의 타입을 전달해준다.

    std::optional은 전달된 타입의 객체를 보관하든지 아니면 안하든지
    둘 중 하나의 상태만을 가지게 된다.

    optional은 전달된 타입을 받는 생성자가 정의되어 있어
    optional 함수에서는 그냥 return을 하더라도 optional 객체를 알아서 만들어서 return한다.

    이 때 가장 큰 장점은 객체를 보관하는 과정에서 동적 할당이 발생하지 않는다는 것이다.
    따라서 불필요한 오버헤드가 없다.
    쉽게 말해 optional자체에 객체가 포함되어 있다.

    만약 아무런 객체도 가지고 있지 않은 빈 optional 객체를 return하고 싶다면
    그냥 nullopt 객체를 return하면 된다.

    optional 객체가 가지고 있는 객체에 접근하고 싶다면 value() 함수를 호출하면 된다.
    optional<>().value()
    
    value 함수 대신 * 를 이용해 역참조해도 된다.
    *optional<>()
    ex)
        GetValueFromMap(data, 2).value() 는 *GetValueFromMap(data, 2)와 같다.

    주의할 점은 만일 optional이 가지고 있는 객체가 없다면 std::bad_optional_access 예외를 던지게 된다.
    따라서 반드시 optional가 들고 있는 객체에 접근하기 전 실제 값을 가지고 있는지 확인해야 된다.

    이것은 has_value() 함수를 통해 확인할 수 있다.
    optional<>().has_value()

    팁은 optional 객체 자체에 bool로 변환하는 캐스팅 연산자가 포함되어 있으므로
    if(optional<>())은 optional<>().has_value()과 같다.
    ex)
        if (GetValueFromMap(data, 4)) 는 if (GetValueFromMap(data, 4).has_value())와 같다.
    
*/

//
#include <iostream>
#include <map>
#include <string>

// map 에서 주어진 키에 대응하는 값이 있는지 확인하는 함수
/*
    문제는 맵에 주어진 키가 들어가 있지 않는 경우
    빈 string 객체를 return하지만 만약 진짜로 어떤 키에 대응하는 값이
    빈 문자열이면 복잡해진다.

    즉 이 함수는 맵에 키가 존재하지 않는 경우와 키가 존재하는데 대응하는 값이
    빈 문자열인 경우를 제대로 구분할 수 없다.
*/
std::string GetValueFromMap(const std::map<int, std::string> &m, int key)
{
    auto itr = m.find(key);
    if (itr != m.end())
    {
        return itr->second;
    }

    return std::string();
}

/*
    std::pair를 이용해 대응하는 값과 함꼐 실제 맵에 존재하는지 유무를
    같이 전달하도록 했다.
    이 방식도 한가지 문제가 있다.
    바로 맵에 키가 존재하지 않을 경우 default 객체를 return해야하는 것이다.
    이것은 몇가지 문제점이 존재한다.
    1. 객체의 디폴트 생성자가 정의되어 있지 않을 수 도 있고
    2. 객체를 디폴트 생성하는 것이 매우 오래 걸릴 수 도 있다

    이 문제를 해결하기 위해 원하는 값을 보관할 수도, 안 할수도 있는 클래스를 도입하는 것이다.
    std::optional을 이용해 만들 수 있따.
    물론 GetValueFromMap에 bool&을 받아 객체 유무를 따로 확인할 수도 있지만
    std::optional을 이용하는 것이 디자인 측면에서 깔끔하다.
*/
std::pair<std::string, bool> GetValueFromMap(
    const std::map<int, std::string> &m, int key)
{
    auto itr = m.find(key);
    if (itr != m.end())
    {
        return std::make_pair(itr->second, true);
    }

    return std::make_pair(std::string(), false);
}

/*
    std::string 타입을 optional에 보관할수도 안할수도 있다.

    

*/
std::optional<std::string> GetValueFromMap(const std::map<int, std::string> &m,
                                           int key)
{
    // 키에 대응하는 값이 존재한다면 그냥 해당값을 return
    auto itr = m.find(key);
    if (itr != m.end())
    {
        // optional에 보관하고자 하는 타입을 받는 생성자가 정의되어 있기 때문에
        // 그냥 return해도 optional 객체를 알아서 생성해서 return한다.
        return itr->second;
    }

    // nullopt 는 <optional> 에 정의된 객체로 비어있는 optional 을 의미한다.
    return std::nullopt;
}

int main()
{
    std::map<int, std::string> data = {{1, "hi"}, {2, "hello"}, {3, "hiroo"}};
    std::cout << "맵에서 2 에 대응되는 값은? " << GetValueFromMap(data, 2)
              << std::endl;
    std::cout << "맵에서 4 에 대응되는 값은? " << GetValueFromMap(data, 4)
              << std::endl;

    // pair 써서 확인하기
    std::cout << "맵에서 2 에 대응되는 값은? " << GetValueFromMap(data, 2).first
              << std::endl;
    std::cout << "맵에 4 는 존재하나요 " << std::boolalpha
              << GetValueFromMap(data, 4).second << std::endl;

    // std::optional 사용
    std::cout << "맵에서 2 에 대응되는 값은? " << GetValueFromMap(data, 2).value()
              << std::endl;
    std::cout << "맵에 4 는 존재하나요 " << std::boolalpha
              << GetValueFromMap(data, 4).has_value() << std::endl;
}
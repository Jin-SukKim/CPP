// 원소를 탐색하는 함수(find, `find_if, any_of, all_of` 등등)
/*
    find function :
        template <class InputIt, class T>
        InputIt find(InputIt first, InputIt last, const T& value)

        first부터 last까지 돌면서 value를 찾는다.

    주의할 점은 컨테이너에서 자체적으로 find 함수를 제공할 경우 더 빠를수도 있다.
    알고리즘 lib의 find 함수는 컨테이너가 어떻게 생겼는지 모르기 때문이다.

    find_if :
        함수 객체를 인자로 받아서 그 결과가 참인 원소들을 찾게된다.

    all_of :
        모든 원소들이 전부 조건을 충족해야 true 를 return (AND 연산과 비슷하다)

    any_of : 
        인자로 받은 범위안의 모든 원소들 중에서 조건을 하나라도 충족하는 것이 있다면 true 를 return (OR 연산과 비슷하다.)
*/

#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

template <typename Iter>
void print(Iter begin, Iter end)
{
    while (begin != end)
    {
        std::cout << "[" << *begin << "] ";
        begin++;
    }
    std::cout << std::endl;
}

struct User
{
    std::string name;
    int level;

    User(std::string name, int level) : name(name), level(level) {} 
    bool operator==(const User &user) const
    {
        if (name == user.name && level == user.level)
            return true;
        return false;
    }
};

class Party
{
    std::vector<User> users;

public:
    bool add_user(std::string name, int level)
    {
        User new_user(name, level);
        if (std::find(users.begin(), users.end(), new_user) != users.end()) // 중복확인
        {
            return false;
        }
        users.push_back(new_user);
        return true;
    }

    // all_of, 모든 원소가 조건에 맞는지 확인
    // 파티원 모두가 15 레벨 이상이여야지 던전 입장 가능
    bool can_join_dungeon()
    {
        return std::all_of(users.begin(), users.end(),
                           [](User &user)
                           { return user.level >= 15; });
    }

    // any_of, 원소가 한개라도 조건에 맞는지 확인
    // 파티원 중 한명 이라도 19렙 이상이면 특별 아이템 사용 가능
    bool can_use_special_item()
    {
        return std::any_of(users.begin(), users.end(),
                           [](User &user)
                           { return user.level >= 19; });
    }
};

int main()
{
    std::vector<int> vec;
    vec.push_back(5);
    vec.push_back(3);
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    vec.push_back(4);

    // find
    auto result = std::find(vec.begin(), vec.end(), 3);
    std::cout << "3 은 " << std::distance(vec.begin(), result) + 1 << " 번째 원소"
              << std::endl;

    // find_if
    auto current = vec.begin();
    while (true)
    {
        current =
            std::find_if(current, vec.end(), [](int i)
                         { return i % 3 == 2; });
        if (current == vec.end())
            break;
        std::cout << "3 으로 나눈 나머지가 2 인 원소는 : " << *current << " 이다 "
                  << std::endl;
        current++;
    }

    Party party;
    party.add_user("철수", 15);
    party.add_user("영희", 18);
    party.add_user("민수", 12);
    party.add_user("수빈", 19);

    std::cout << std::boolalpha;
    std::cout << "던전 입장 가능 ? " << party.can_join_dungeon() << std::endl;
    std::cout << "특별 아이템 사용 가능 ? " << party.can_use_special_item()
              << std::endl;
}
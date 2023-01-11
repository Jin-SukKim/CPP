// map - set가 매우 비슷하나 set은 key만 저장했다면 map은 key와 value 모두 저장한다. (key - value)
#include <iostream>
#include <map>
#include <string>

/*
    map 역시 set과 같이 중복된 원소는 저장할 수 없다.
    이미 key가 존재할 경우 insert함수는 무시된다.
*/
template <typename K, typename V>
void print_map(std::map<K, V> &m)
{
    // 맵의 모든 원소들을 출력하기
    for (auto itr = m.begin(); itr != m.end(); ++itr)
    {
        std::cout << itr->first << " " << itr->second << std::endl;
    }

    // kv 에는 맵의 key 와 value 가 std::pair 로 들어갑니다. (range-base for loop)
    /*
    for (const auto& kv : m) {
        std::cout << kv.first << " " << kv.second << std::endl;
    }
    */
}

// find 함수를 사용해 안전하게 키에 대응되는 값을 찾는다.
// find 함수는 map에서 해당하는 키를 찾아서 이를 가리키는 반복자를 return한다. 만약에, 키가 존재하지 않는다면 end() 를 return.
template <typename K, typename V>
void search_and_print(std::map<K, V> &m, K key)
{
    auto itr = m.find(key);
    if (itr != m.end())
    {
        std::cout << key << " --> " << itr->second << std::endl;
    }
    else
    {
        std::cout << key << "은(는) 목록에 없습니다" << std::endl;
    }
}

int main()
{
    std::map<std::string, double> pitcher_list;

    // 참고로 2017년 7월 4일 현재 투수 방어율 순위입니다.

    // 맵의 insert 함수는 pair 객체를 인자로 받습니다.
    pitcher_list.insert(std::pair<std::string, double>("박세웅", 2.23));
    pitcher_list.insert(std::pair<std::string, double>("해커 ", 2.93));

    pitcher_list.insert(std::pair<std::string, double>("피어밴드 ", 2.95));

    // 타입을 지정하지 않아도 간단히 std::make_pair 함수로
    // std::pair 객체를 만들 수 도 있습니다.
    pitcher_list.insert(std::make_pair("차우찬", 3.04));
    pitcher_list.insert(std::make_pair("장원준 ", 3.05));
    pitcher_list.insert(std::make_pair("헥터 ", 3.09));

    // 혹은 insert 를 안쓰더라도 [] 로 바로
    // 원소를 추가할 수 있습니다.
    pitcher_list["니퍼트"] = 3.56;
    pitcher_list["박종훈"] = 3.76;
    pitcher_list["켈리"] = 3.90;

    print_map(pitcher_list);

    std::cout << "박세웅 방어율은? :: " << pitcher_list["박세웅"] << std::endl;

    // 주의점
    pitcher_list["오승환"] = 3.58;
    // 만약 map에 없는 key를 참조하려고 하면 map의 [] 연산자는 자동으로 default 생성자를 호출해 원소를 추가한다.
    std::cout << "류현진 방어율은? :: " << pitcher_list["류현진"] << std::endl;

    std::cout << "-----------------" << std::endl;
    print_map(pitcher_list);

    std::cout << "-----------------" << std::endl;

    search_and_print(pitcher_list, std::string("오승환"));
    search_and_print(pitcher_list, std::string("잭"));
}
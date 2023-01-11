// unordered_set 과 unordered_map
/*
    이 컨테이너들의 원소들은 정렬되어 있지 않다.
    따라서 출력해 보면 랜덤한 순서로 출력된다.

    unordered_set과 unordered_map의 장점은 insert, erase, find 모두가 O(1)O(1) 으로 수행된다

    이 두 함수는 삽입/검색할 때 hash function을 사용하는데
    해시 함수란 임의의 크기의 데이터를 고정된 크기의 데이터로 대응시켜주는 함수이다.
    이 때 보통 고정된 크기의 데이터라고 하면 일정 범위의 정수값을 의미한다.

    unordered 함수들의 경우 해시함수는 1 부터 D까지의 값을 return하고 그 해시값(hash함수로 계산한 값)을 원소를
    저장할 공간의 번호로 사용한다. 해시함수의 구조상 최대한 1부터 D까지 고른 값을 반환하도록 되어있다.
    그래서 고루고루 모든 공간을 사용하게 된다.

    해시 함수의 가장 중요한 성질은, 만약에 같은 원소를 해시 함수에 전달한다면 같은 해시값을 return한하는 점이다..
    이 덕분에 원소의 탐색을 빠르게 수행할 수 있다.

    그래서 만약 unordered_set에 '빨간색'이 있는지 확인할 때 해시 함수에 대입하면 1을 return한다고 하면
    1번 공간을 확인하면 '빨간색'이 있는지 확인이 가능하다.

    해시 함수는 해시값 계산을 상수 시간에 처리해 unordered 함수들이 상수 시간에 탐색을 처리할 수 있따.

    물론 다른 원소임에도 불구하고 같은 해시값을 갖는 경우도 있는데 이걸 해시충돌(hash conllision)이라고 한다.

    그래서 이 두 함수는 빠르지만 최악의 경우 O(n)속도도 나올 수 있기 때문에 최적화를 해야할 때 사용하는 것을 권장한다.
*/
#include <iostream>
#include <string>
#include <unordered_set>

template <typename K>
void print_unordered_set(const std::unordered_set<K> &m)
{
    // 셋의 모든 원소들을 출력하기
    for (const auto &elem : m)
    {
        std::cout << elem << std::endl;
    }
}

/*
    클래스에서 unordered_set과 unordered_map을 사용하려면 클래스의 객체를 위한 Hash 함수를 직접 만들어야한다.
    set과 map과는 다르게 정렬되지 않기 때문에 operator< 오버로딩은 필요없지만 hash collision을 막기 위해
    operator==를 오버로딩해 원소들을 비교해 충돌을 방지해야 된다.
*/
template <typename K>
void is_exist(std::unordered_set<K> &s, K key)
{
    auto itr = s.find(key);
    if (itr != s.end())
    {
        std::cout << key << " 가 존재!" << std::endl;
    }
    else
    {
        std::cout << key << " 가 없다" << std::endl;
    }
}
class Todo
{
    int priority; // 중요도. 높을 수록 급한것!
    std::string job_desc;

public:
    Todo(int priority, std::string job_desc)
        : priority(priority), job_desc(job_desc) {}

    bool operator==(const Todo &t) const
    {
        if (priority == t.priority && job_desc == t.job_desc)
            return true;
        return false;
    }

    friend std::ostream &operator<<(std::ostream &o, const Todo &t);
    friend struct std::hash<Todo>;
};

// Todo 해시 함수를 위한 함수객체(Functor)
/*
    hash<string> hash_fn;
    size_t hash_val = hash_fn(str);  // str 의 해시값 계산

    하지만 Todo class의 해시 함수를 만들고 싶은 것 이므로 Todo 특수화 버전을 만든다.
*/
// namespace 안에 새로운 클래스/함수를 추가하기 위해서는 위처럼 명시적으로 namespace (이름) 를 써줘야만한다.
namespace std
{
    template <>
    struct hash<Todo>
    {
        // 보통 size_t 타입은 int 랑 동일한데, 이 말은 해시값으로 0 부터 4294967295 까지 가능하다
        size_t operator()(const Todo &t) const
        {   
            // stl lib에서 기본적인 타입에 대한 hash 함수를 제공한다.
            hash<string> hash_func;

            // priority는 int 값이므로 해시값 자체로 사용, job_desc는 string이므로 hash_func을 이용해 해시값 계산
            // 두 해쉬값을 섞기위해 xor 연산 을 사용했다.
            return t.priority ^ (hash_func(t.job_desc));
        }
    };
} // namespace std
std::ostream &operator<<(std::ostream &o, const Todo &t)
{
    o << "[중요도 : " << t.priority << " ] " << t.job_desc;
    return o;
}

int main()
{
    std::unordered_set<std::string> s;

    s.insert("hi");
    s.insert("my");
    s.insert("name");
    s.insert("is");
    s.insert("psi");
    s.insert("welcome");
    s.insert("to");
    s.insert("c++");

    print_unordered_set(s);
    std::cout << "----------------" << std::endl;
    is_exist(s, std::string("c++"));
    is_exist(s, std::string("c"));

    std::cout << "----------------" << std::endl;
    std::cout << "'hi' 를 삭제" << std::endl;
    s.erase(s.find("hi"));
    is_exist(s, std::string("hi"));

    std::unordered_set<Todo> todos;

    todos.insert(Todo(1, "농구 하기"));
    todos.insert(Todo(2, "수학 숙제 하기"));
    todos.insert(Todo(1, "프로그래밍 프로젝트"));
    todos.insert(Todo(3, "친구 만나기"));
    todos.insert(Todo(2, "영화 보기"));
    print_unordered_set(todos);
    std::cout << "----------------" << std::endl;
}

/*  TIP!
    데이터의 존재 유무 만 궁금할 경우 → set

    중복 데이터를 허락할 경우 → multiset (insert, erase, find 모두 O(logN). 최악의 경우에도 O(logN))

    데이터에 대응되는 데이터를 저장하고 싶은 경우 → map

    중복 키를 허락할 경우 → multimap (insert, erase, find 모두 O(logN). 최악의 경우에도 O(logN))

    속도가 매우매우 중요해서 최적화를 해야하는 경우 → unordered_set, unordered_map

    (insert, erase, find 모두 O(1). 최악의 경우엔 O(N) 그러므로 해시함수와 상자 개수를 잘 설정해야 한다!)

*/
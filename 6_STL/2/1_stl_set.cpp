// 연관 컨테이너(associative container) : key - value 구조를 가진 컨테이너
/*
    연관 컨테이너는 두가지 종류가 있다.

    1. 특정 키가 컨테이너에 존재하는지 유무 - set, multiset

    2. 특정 키에 대응되는 값이 무엇인지 질의 - map, multimap (당연히 존재 유무도 파악 가능, 대응되는 값이 없다면 값을 가져올수 없다)

    맵의 경우 셋 보다 사용하는 메모리가 크기 때문에 키의 존재 유무 만 궁금하다면 셋을 사용하는 것이 좋다.
*/

// set, 찾기/추가/제거 - O(logN)
/*
    set이 빠른 이유는 원소가 추가될 때 정렬된 상태로 추가하고 이때 set는 내부적으로 트리 구조로 구성되어 있어서이다.
    이 트리 구조는 왼쪽의 모든 노드들은 자기자신보다 작고 오른쪽의 노드들은 자신보다 크다라는 규칙이 있다.
    set에 10 20 25 30 40 50이 있을 경우, 25를 찾고 싶으면 제일 처음 최상위 노드와 비교하고 규칙을 따라 트리의 노드를 따라가며 찾는다.
    그래서 적은 횟수의 비교로 속도가 빠르다.

    또한 set은 중복된 원소가 없다.
    set에 추가하려는 원소가 이미 존재하는 경우 set은 자체적으로 insert 작업을 하지 않아 원소 중복을 없앤다.
    만약 원소를 중복해서 넣으려면 multiset을 사용해야 된다.
*/
#include <iostream>
#include <set>

// set은 임의의 위치에 바로 접근이 불가능하고 list처럼 순차적으로 접근해야된다.
template <typename T>
void print_set(std::set<T> &s)
{
    // 셋의 모든 원소들을 출력하기
    std::cout << "[ ";
    for (typename std::set<T>::iterator itr = s.begin(); itr != s.end(); ++itr)
    {
        std::cout << *itr << " ";
    }
    std::cout << " ] " << std::endl;
}

template <typename T, typename C>
void print_set(std::set<T, C> &s)
{
    // 셋의 모든 원소들을 출력하기
    for (const auto &elem : s)
    {
        std::cout << elem << " " << std::endl;
    }
}

/*
    class 객체를 set으로 저장하는 경우 operator< 가 오버로딩되어 있지 않으면 set을 사용할 수 없다.
    set은 원소를 저장할 때 비교를 통해 정렬된 상태로 저장되기 때문에 class에 <가 없다면
    비교를 진행할 수 없어서 오류가 발생한다.
*/
class Todo
{
    int priority; // 중요도. 높을 수록 급한것!
    std::string job_desc;

public:
    Todo(int priority, std::string job_desc)
        : priority(priority), job_desc(job_desc) {}

    // set 내부 정렬을 위한 비교자 오버로딩. const를 사용하는 이유는 set이 정렬시에 const 반복자를 사용하기 떄문이다.
    bool operator<(const Todo &t) const
    {
        if (priority == t.priority)
        {
            return job_desc < t.job_desc;
        }
        return priority > t.priority;
    }

    // 클래스에 비교자 오버로딩을 두지않고 set을 사용하는 법(외부 라이브러리를 사용하는 경우 등)
    friend struct TodoCmp;

    friend std::ostream &operator<<(std::ostream &o, const Todo &td);
};

// set을 사용하기 위해 반드시 객체간 비교를 수행해야 된다.
struct TodoCmp
{
    bool operator()(const Todo &t1, const Todo &t2) const
    {
        if (t1.priority == t2.priority)
        {
            return t1.job_desc < t2.job_desc;
        }
        return t1.priority > t2.priority;
    }
};

std::ostream &operator<<(std::ostream &o, const Todo &td)
{
    o << "[ 중요도: " << td.priority << "] " << td.job_desc;
    return o;
}

int main()
{
    std::set<int> s;
    // 원소가 어디있는지 중요한게 아닌 존재하는지가 중요하기에 위치정보는 필요없다.
    // set은 원소가 추가될 때 정렬된 상태를 유지하며 추가된다.
    s.insert(10); // 원소 추가
    s.insert(50);
    s.insert(20);
    s.insert(40);
    s.insert(30);

    std::cout << "순서대로 정렬되서 나온다" << std::endl;
    print_set(s);

    std::cout << "20 이 s 의 원소인가요? :: ";

    // 원소가 존재하는 지 확인. 없을 경우 end()를 return
    auto itr = s.find(20);
    if (itr != s.end())
    {
        std::cout << "Yes" << std::endl;
    }
    else
    {
        std::cout << "No" << std::endl;
    }

    std::cout << "25 가 s 의 원소인가요? :: ";
    itr = s.find(25);
    if (itr != s.end())
    {
        std::cout << "Yes" << std::endl;
    }
    else
    {
        std::cout << "No" << std::endl;
    }

    s.insert(30);
    s.insert(20);
    s.insert(10);

    std::cout << "중복" << std::endl;
    print_set(s);

    // class set
    std::set<Todo> todos;

    todos.insert(Todo(1, "농구 하기"));
    todos.insert(Todo(2, "수학 숙제 하기"));
    todos.insert(Todo(1, "프로그래밍 프로젝트"));
    todos.insert(Todo(3, "친구 만나기"));
    todos.insert(Todo(2, "영화 보기"));

    // class 내부에 비교자를 오버로딩하지 않고 set의 template의 두번째 인자로 compare(비교자)를 넘긴다.
    std::set<Todo, TodoCmp> todos2;

    todos2.insert(Todo(1, "농구 하기"));
    todos2.insert(Todo(2, "수학 숙제 하기"));
    todos2.insert(Todo(1, "프로그래밍 프로젝트"));
    todos2.insert(Todo(3, "친구 만나기"));
    todos2.insert(Todo(2, "영화 보기"));

    print_set(todos2);

    std::cout << "-------------" << std::endl;
    std::cout << "숙제를 끝냈다면!" << std::endl;
    todos2.erase(todos2.find(Todo(2, "수학 숙제 하기")));
    print_set(todos2);
}
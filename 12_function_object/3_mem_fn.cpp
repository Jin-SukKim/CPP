// mem_fn - 멤버 함수들을 함수 객체로
/*
    std::transform은 algorithm lib의 함수로 각 원소들에 대해 인자로 전달된 함수를 실행시킨 후 결과를 전달된 Container에 넣어준다.

    template <class InputIt, class OutputIt, class UnaryOperation>
    OutputIt transform(InputIt first1, InputIt last1, OutputIt d_first,
                    UnaryOperation unary_op) {
        while (first1 != last1) {
            *d_first++ = unary_op(*first1);
            first1++;
        }
    return d_first;
    }

    *d_first++ = unary_op(*first1);의 경우 unary_op가 멤버 함수가 아닐 경우 밑의 코드와 같이 사용해도 되지만
    unary_op가 멤버 함수인 경우 오류를 발생한다.
    ex) &vecotr<int>::size를 unary_op인자로 받는데 size는 멤버 함수이다.
        즉
        unary_op(*first1); 은 &vector<int>::size(*first1);이 되는데
        멤버 함수는 (*first1).(*&vector<int>::size)나 first1->(*&vector<int>::size)와 같이 호출되어야 하기 떄문이다.

    그래서 std::function을 이용해 멤버함수를 전달하면 오류를 발생시키지 않는다.

    하지만 매번 std::function 객체를 생성해줘서 전달하면 매번 코드를 추가로 작성해줘야 하므로
    functional 라이브러리에서 제공해주는 std::mem_fn을 활용하면 std::function 객체를 return 시켜준다.

    mem_fn은 전달된 멤버 함수를 function 객체로 만들어서 return 해준다.
    물론 lambda 함수로도 동일한 작업을 수행할 수 있다.

    mem_fn(&vector<int>::size) 대신에 [](const auto& v){ return v.size()} 를 전달하면 된다.

*/

// vector들을 가지는 vector의 vector 원소들이 각각의 크기로 벡터를 만들기
#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>
using std::vector;

int main()
{
    vector<int> a(1);
    vector<int> b(2);
    vector<int> c(3);
    vector<int> d(4);

    vector<vector<int>> container;
    container.push_back(b);
    container.push_back(d);
    container.push_back(a);
    container.push_back(c);

    vector<int> size_vec(4);

    // size가 멤버 함수여서 오류를 발생시킨다.
    std::transform(container.begin(), container.end(), size_vec.begin(),
                   &vector<int>::size);
    for (auto itr = size_vec.begin(); itr != size_vec.end(); ++itr)
    {
        std::cout << "벡터 크기 :: " << *itr << std::endl;
    }

    // 오류를 발생시키지 않게 std::function으로 변화해서 전달
    std::function<size_t(const vector<int> &)> sz_func = &vector<int>::size;

    vector<int> size_vec(4);
    std::transform(container.begin(), container.end(), size_vec.begin(), sz_func);
    for (auto itr = size_vec.begin(); itr != size_vec.end(); ++itr)
    {
        std::cout << "벡터 크기 :: " << *itr << std::endl;
    }

    // 매번 std::function을 만들어서 전달하기 보다는 std::funciton 객체를 return해주는 std::mem_fn을 이용하면 훨씬 수월하다.
    transform(container.begin(), container.end(), size_vec.begin(),
              std::mem_fn(&vector<int>::size));
    for (auto itr = size_vec.begin(); itr != size_vec.end(); ++itr)
    {
        std::cout << "벡터 크기 :: " << *itr << std::endl;
    }
}
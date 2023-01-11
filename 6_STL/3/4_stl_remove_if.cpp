/*
    정해진 값이 아닌 특정한 조건을 만족하는 원소들을 제거하기 위해 원소가 조건을 만족하는지
    remove_if 함수를 사용해 판단한다.
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

// 구조체에 operator() 오버로딩을 통해 조건을 전달한다. (실제 함수도 전달 가능하다.)
struct is_odd
{
  bool operator()(const int &i) { return i % 2 == 1; }
};

// 실제 함수도 전달 가능하다.
bool odd(const int &i) { return i % 2 == 1; }

/*
remove_if 함수는 함수 객체가 인자를 딱 1 개 만 받는다고 가정합니다.
따라서 호출되는 operator() 을 통해선 원소에 대한 정보 말고는 추가적인 정보를 전달하기는 어렵습니다.
하지만 예를 들어서 홀수인 원소들을 삭제하되 처음 2개만 삭제한다고 해봅시다.
함수 객체의 경우 사실 클래스의 객체이기 때문에 멤버 변수를 생각할 수 있습니다.
*/
struct is_odd2
{
  int *num_delete;

  is_odd(int *num_delete) : num_delete(num_delete) {}

  bool operator()(const int &i)
  {
    if (*num_delete >= 2)
      return false;

    if (i % 2 == 1)
    {
      (*num_delete)++;
      return true;
    }

    return false;
  }
};

template <class ForwardIterator, class UnaryPredicate>
ForwardIterator remove_if(ForwardIterator first, ForwardIterator last,
                          UnaryPredicate pred)
{
  ForwardIterator result = first;
  while (first != last)
  {
    if (!pred(*first))
    { // <-- 함수 객체 pred 를 실행하는 부분
      *result = std::move(*first);
      ++result;
    }
    ++first;
  }
  return result;
}

template <class ForwardIt, class UnaryPredicate>
ForwardIt remove_if(ForwardIt first, ForwardIt last, UnaryPredicate pred)
{
  first = std::find_if(first, last, pred); // <- pred 한 번 복사됨
  if (first != last)
    // 아래 for 문은 first + 1 부터 시작한다고 봐도 된다 (++i != last)
    for (ForwardIt i = first; ++i != last;)
      if (!pred(*i)) // <-- pred 호출 하는 부분
        *first++ = std::move(*i);
  return first;
}

int main()
{
  std::vector<int> vec;
  vec.push_back(5);
  vec.push_back(3);
  vec.push_back(1);
  vec.push_back(2);
  vec.push_back(3);
  vec.push_back(4);

  std::cout << "처음 vec 상태 ------" << std::endl;
  print(vec.begin(), vec.end());

  std::cout << "벡터에서 홀수 인 원소 제거 ---" << std::endl;
  // 세번째 인자를 통해 조건을 전달한다.
  vec.erase(std::remove_if(vec.begin(), vec.end(), is_odd()), vec.end());
  // vec.erase(std::remove_if(vec.begin(), vec.end(), odd), vec.end());

  print(vec.begin(), vec.end());

  std::vector<int> vec2;
  vec2.push_back(5);
  vec2.push_back(3);
  vec2.push_back(1);
  vec2.push_back(2);
  vec2.push_back(3);
  vec2.push_back(4);

  std::cout << "처음 vec 상태 ------" << std::endl;
  print(vec2.begin(), vec2.end());

  std::cout << "벡터에서 홀수인 원소 앞의 2개 제거 ---" << std::endl;
  vec2.erase(std::remove_if(vec2.begin(), vec2.end(), is_odd()), vec2.end());
  print(vec2.begin(), vec2.end());
}